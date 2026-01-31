#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#define GLFW_DLL

//math bs

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>

const float cameraSpeed=0.001f;

class camera{
	public:
	
		glm::vec3 position;
		
		glm::vec3 target;
		glm::vec3 front;
		glm::vec3 up; 
		glm::vec3 right;
		
		glm::vec3 worldUp;
		glm::mat4 view;
		unsigned int viewLoc;
		
		bool firstMouse;
		
		float yaw;
		float pitch;
		float lastX;
		float lastY;
		float fov;
		
		camera(){
			position   = glm::vec3(0.0f, 0.0f,  3.0f);
			front = glm::vec3(0.0f, 0.0f, -1.0f);
			target = glm::vec3(0.0f, 0.0f, 0.0f);
			up = glm::vec3(0.0f, 1.0f, 0.0f);
			worldUp=up;
			yaw   = -90.0f;
			pitch =  0.0f;
			fov   =  45.0f;
			firstMouse = true;
			lastX =  400.0f / 2.0;
			lastY =  400.0f / 2.0;
			
			
			updateCamera();
		}
		
		void updateView(unsigned int shaderID){
			view = glm::lookAt(position, position+front, up);
			viewLoc=glGetUniformLocation(shaderID, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		}
		
		void keyboardMove(GLFWwindow* window){
			if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
				position += cameraSpeed * front;
			}
			if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){
				position -= cameraSpeed * front;
			}
			if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
				position -= right * cameraSpeed;
			}
			if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
				position += right * cameraSpeed;
			}
		}
		
		void mouseProcess(double xpos, double ypos)
		{
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}
		  
			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; 
			lastX = xpos;
			lastY = ypos;

			float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw   += xoffset;
			pitch += yoffset;

			if(pitch > 89.0f)
				pitch = 89.0f;
			if(pitch < -89.0f)
				pitch = -89.0f;

			updateCamera();
		}
		
		void scrollProcess(double xoffset, double yoffset)
		{
			fov -= (float)yoffset;
			if (fov < 1.0f)
				fov = 1.0f;
			if (fov > 45.0f)
				fov = 45.0f; 
		}
	private:
		void updateCamera(){
			glm::vec3 fnt;
			fnt.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			fnt.y = sin(glm::radians(pitch));
			fnt.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			front = glm::normalize(fnt);
			
			right = glm::normalize(glm::cross(front, worldUp));
			up    = glm::normalize(glm::cross(right, front));
		}
}; 

#endif