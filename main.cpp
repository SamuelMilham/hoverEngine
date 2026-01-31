#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>

#include <math.h>
#include <glad/gl.h>


#define GLFW_DLL

//too lazy to make my own image loader lol
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glfw3.h>

//gave up on making this from scratch will figure out later when
//get motivation for it
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



#include "shader.h"
#include "camera.h"



const int windowWidth= 400;
const int windowHeight=400;

const double PI=3.1415926535;

camera Camera;

void processInput(GLFWwindow *window){
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
	
	Camera.keyboardMove(window);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera.mouseProcess(xpos,ypos);
}
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	Camera.scrollProcess(xOffset, yOffset);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0,0,width,height);
}

const char* importObjFile(const char* objPath){
	std::string obj;
	std::ifstream objFile;
	objFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	
	try{
				objFile.open(objPath);
				
				std::stringstream objStream;
				
				//read file buffer and but into stream
				objStream << objFile.rdbuf();
				
				objFile.close();
				
				obj = objStream.str();
				//std::cout<<obj;
				
			}
	catch(std::ifstream::failure& e){
		std::cout << "ERROR::OBJ::FILE_NOT_READ"<< e.what() << std::endl;
	}
	std::vector<float> verticies;
	std::vector<float> textures;
	std::vector<float> normals;
	std::vector<glm::vec3> faces;
	glm::vec3 faceChunk={0.0f,0.0f,0.0f};
	std::string faceNum;
	
	std::string num="";
	int numsParsed=0;
	bool isSpace=false;
	int numLen=0;
	int j=0;
	int facePhase=0;
	
	//ensure there isnt an overflow
	obj+=(std::string) "}";
	
	for(int i=0; i<(int)obj.length(); i++){
		//std::cout<<obj[i];
		//numLen=0;
		if (obj.substr(i,1)==(std::string)"v"){
			numsParsed=0;
			if (obj.substr(i+1,1)==(std::string) " "){
				//parse the line
				//.substr(i,1)
				//std::cout<<obj.substr(i+1,1);
				j=i+1;
				std::cout<<"verts:";
				while(numsParsed<3){
					while(!isSpace){
						numLen++;
						if (obj.substr(j+numLen+1,1)==(std::string) " "){
							isSpace=true;
							
						}
						else if(obj.substr(j+numLen+1,1)==(std::string) "v"){
							isSpace=true;
						}
					}
					verticies.push_back(stof(obj.substr(j,numLen)));
					num=obj.substr(j,numLen);
					std::cout<<num;
					isSpace=false;
					j+=numLen;
					numLen=0;
					numsParsed++;
				}
			}
			else if(obj.substr(i+1,1)==(std::string)"t"){
				//std::cout<<obj.substr(i+1,1);
				j=i+2;
				std::cout<<"texts";
				while(numsParsed<2){
					while(!isSpace){
						numLen++;
						if (obj.substr(j+numLen+1,1)==(std::string) " "){
							isSpace=true;
							
						}
						else if(obj.substr(j+numLen+1,1)==(std::string) "v"){
							isSpace=true;
						}
					}
					textures.push_back(stof(obj.substr(j,numLen)));
					num=obj.substr(j,numLen);
					std::cout<<num;
					isSpace=false;
					j+=numLen;
					numLen=0;
					numsParsed++;
				}
			}
			else if(obj.substr(i+1,1)==(std::string)"n"){
				//std::cout<<obj.substr(i+1,1);
				j=i+2;
				std::cout<<"norms";
				while(numsParsed<3){
					while(!isSpace){
						numLen++;
						if (obj.substr(j+numLen+1,1)==(std::string) " "){
							isSpace=true;
							
						}
						else if(obj.substr(j+numLen+1,1)==(std::string) "v"){
							isSpace=true;
						}
					}
					
					normals.push_back(stof(obj.substr(j,numLen)));
					num=obj.substr(j,numLen);
					std::cout<<num;
					isSpace=false;
					j+=numLen;
					numLen=0;
					numsParsed++;
				}
			}
			
		}
		else if(obj.substr(i,1)==(std::string)"f"){
			numsParsed=0;
		
			//parse the line
			//.substr(i,1)
			//std::cout<<obj.substr(i+1,1);
			j=i+1;
			//std::cout<<"faces:";
			
			while(numsParsed<4){
				faceNum="";
				facePhase=0;
				faceChunk.x=0.0f;
				faceChunk.y=0.0f;
				faceChunk.z=0.0f;
				while(!isSpace){
					numLen++;
					if (obj.substr(j+numLen+1,1)==(std::string) " "){
						isSpace=true;
						
					}
					else if(obj.substr(j+numLen+1,1)==(std::string) "f"){
						isSpace=true;
					}
					else if(obj.substr(j+numLen+1,1)==(std::string) "}"){
							isSpace=true;
					}
				}
				//std::cout<<"faces:";
				std::cout<<obj.substr(j,numLen)<<std::endl;
				for(int i=0; i<obj.substr(j,numLen).length(); i++){
					if(obj.substr(j,numLen).substr(i,1)==(std::string)"/" ){
						if(facePhase==0){
							faceChunk.x=stof(faceNum);
						}
						else if(facePhase==1){
							faceChunk.y=stof(faceNum);
						}
						else if(facePhase==2){
							faceChunk.z=stof(faceNum);
						}
						faceNum="";
						facePhase++;
					}
					else{
						faceNum+=obj.substr(j,numLen).substr(i,1);
					}
				}
				if(faceNum==""){
					faceNum="0";
				}
				//faceChunk.z=stof(faceNum);
				std::cout<<faceChunk.x<<std::endl;
				std::cout<<faceChunk.y<<std::endl;
				std::cout<<faceChunk.z<<std::endl;
				faces.push_back(faceChunk);
				
				
				isSpace=false;
				j+=numLen;
				numLen=0;
				numsParsed++;
			}
			
		}
		
	}
	//Triangle Fanning
	
	return "d";
}

int main(){
	 
	
	//std::cout << "Hello World";
	if (!glfwInit()){
        return -1;
	}
	float vertices[] = {
	// positions         // texture coords
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
	//Create Window
	GLFWwindow* window;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	
	window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
	
	if (!window)
    {
        glfwTerminate();
        return -1;
    }
	glfwMakeContextCurrent(window);
	if (!gladLoaderLoadGL())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	
	
	shader ourShader("vertex.glsl", "fragment.glsl");
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	 glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
	
	
	
	
	//array
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);  
	
	glBindVertexArray(VAO);
	
	//vertex
	
	unsigned int VBO;
	glGenBuffers(1,&VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//element

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
	


	
	//texture1
	 unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); 
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	
	int tWidth, tHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *tData = stbi_load("container.jpg", &tWidth, &tHeight, &nrChannels, 0); 
	
	if(tData){
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout<<"Failed to load texture"<<std::endl;
	}
	
	stbi_image_free(tData);
	
	//texture2 
	
	glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
	
	 // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	tData = stbi_load("awesomeface.png", &tWidth, &tHeight, &nrChannels, 0); 
	
	if(tData){
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout<<"Failed to load texture"<<std::endl;
	}
	
	stbi_image_free(tData);
	
	
	
	
	//postition
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 
	
	//color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	//glEnableVertexAttribArray(1);
	
	//texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//normal mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	ourShader.use();
	
	ourShader.setInt("texture1", 0); 
	ourShader.setInt("texture2", 1); 
	
	
	glEnable(GL_DEPTH_TEST); 
	
	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};
	ourShader.use();
	const char *cube=importObjFile("3dModels/Cube.obj");
	
	//projection stuff
	unsigned int projectionLoc  = glGetUniformLocation(ourShader.ID, "projection");
    glm::mat4 projection    = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(Camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		
		glfwPollEvents();
		
		//draw on screen
		
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		
		
		
		//texture stuff
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		glm::mat4 model         = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
       
	    unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
		
		Camera.updateView(ourShader.ID);
		
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
		
		
		glBindVertexArray(VAO);
		for(unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i; 
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(window);
	}
	
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
	glfwTerminate();
	return 0;
}
