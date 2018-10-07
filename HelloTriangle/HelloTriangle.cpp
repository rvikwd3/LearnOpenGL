#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

#define SCR_WIDTH 800
#define SCR_HEIGHT 800

void glfw_error(int error, const char* description);
std::string readTextFile(const char*);

int main(){
	/*	Setup & OpenGL Context Creation {{{ 
	 *	================================================================== */

	std::cout << "Main initialized()" << std::endl;

	//set callback in case of errors
	glfwSetErrorCallback(glfw_error);

	//glfw:	initialize and configure
	//------------------------------
	if(!glfwInit()){
			printf("glfwInit() failed\n");
			exit(EXIT_FAILURE);
	}

	std::cout << "glfwInit() succeeded!" << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//glfw:	window creation
	//---------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL){
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
	}

	glfwMakeContextCurrent(window);

	//glad:	load all OpenGL function pointers
	//---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
	}
	
	/* ==================================================================
	 * Setup end }}}				*/
	
	// Vertex array
	// ------------
	float vertices[]= {
		-0.5f,	-0.5f,	0.0f,
		0.5f,	-0.5f,	0.0f,
		0.0f,	0.5f,	0.0f
	};

	// Vertex Buffer Object
	// --------------------
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Bind VBO to target GL_ARRAY_BUFFER
	// ---------------------_-----_------
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy vertex data into VBO
	// -------------------------
	glBufferData(GL_ARRAY_BUFFER,
				sizeof(vertices),
				vertices, 
				GL_STATIC_DRAW
	);

	/*	Vertex Shader {{{ 
	 *	================================================================== */

	// Prepare vertex shader source
	std::string str;
	str = readTextFile("vertexShaderSource.glsl");

	GLchar const* vertexShaderSource = str.c_str();

	// Create vertex shader
	// --------------------
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for shader compile errors
	// -------------------------------
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success){
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	/*	================================================================== 
	*	Vertex Shader end }}} */
	
	/*	Fragment Shader {{{
	 *	================================================================== */
	
	// Prepare fragment shader source
	str = readTextFile("fragmentShaderSource.glsl");
	GLchar const* fragmentShaderSource = str.c_str();

	// Create fragment shader
	// ----------------------
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for shader compile errors
	// -------------------------------
	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success){
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/*	================================================================== 
	*	Fragment Shader end }}} */


	return 0;
}

std::string readTextFile(const char* filename){ //{{{
		std::ifstream file(filename);
		std::string str;
		std::string file_contents;

		if(!file){
				std::cout << "Error in opening " << filename << "!" << std::endl;
				return 0;
		}

		std::cout << "####################\n" << filename << " Content:" << std::endl;
		
		while(std::getline(file, str)){
				file_contents += str;
				file_contents.push_back('\n');

				std::cout << str << std::endl;
		}
		std::cout << "####################\n\n";
		return file_contents;
} //}}}

// error callback 
// --------------
void glfw_error(int error, const char* description){ //{{{
	fputs(description, stderr);
} //}}}

