#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow* window);
void glfw_error(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// window settings
// ---------------
#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int main(){
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

	//glad: set window size
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//resize viewport on window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// RENDER LOOP
	//------------
	while(!glfwWindowShouldClose(window)){
			// input
			// -----
			processInput(window);

			// clear color buffer
			// ------------------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glfwSwapBuffers(window);
			glfwPollEvents();
	}

	// clean resources
	glfwTerminate();

	return 0;
}

// process all input: query GLFW wheter relevant keys are pressed
// --------------------------------------------------------------
void processInput(GLFWwindow *window){
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
				std::cout << "Escape key pressed" << std::endl;
				glfwSetWindowShouldClose(window, true);
		}
}

// resize window: resize glad viewport according to window size
// ------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

// error callback
// --------------
void glfw_error(int error, const char* description){
	fputs(description, stderr);
}

