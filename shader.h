#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class shader{
	public:
		//program ID
		unsigned int ID;
		
		//constructor reads and builds shader
		shader(const char* vertexPath, const char* fragmentPath){
			//retrieve source code from file path
			std::string vertexCode;
			std::string fragmentCode;
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;
			
			//see that ifstream can throw exceptions
			vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
			
			try{
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);
				
				std::stringstream vShaderStream, fShaderStream;
				
				//read file buffer and but into stream
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				
				vShaderFile.close();
				fShaderFile.close();
				
				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();
				
			}
			catch(std::ifstream::failure& e){
				std::cout << "ERROR::SHADER::FILE_NOT_READ"<< e.what() << std::endl;
			}
			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();
			
			// compile shaders
			unsigned int vertex, fragment;
			//vertex
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex,1,&vShaderCode,NULL);
			glCompileShader(vertex);
			
			checkCompileErrors(vertex, "vertex");
			
			//fragment
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment,1,&fShaderCode,NULL);
			glCompileShader(fragment);
			
			checkCompileErrors(fragment, "fragment");
			
			//shader program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);
			
			checkCompileErrors(ID, "program");
			
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}
		
		//functions
		void use(){ 
			glUseProgram(ID);
		}  
		
		void setBool(const std::string &name, bool value) const
		{         
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
		}
		void setInt(const std::string &name, int value) const
		{ 
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
		}
		void setFloat(const std::string &name, float value) const
		{ 
			glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
		} 
		
	private:
		void checkCompileErrors(unsigned int shader, std::string type){
			
			int success;
			char infoLog[1024];
			
			if (type != "program")
			{
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
			else
			{
				glGetProgramiv(shader, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				}
			}
		}
};

#endif