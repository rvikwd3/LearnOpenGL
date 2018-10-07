#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

#define SCR_WIDTH 800
#define SCR_HEIGHT 800

void processInput(GLFWwindow* window);
void glfw_error(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::string readTextFile(const char*);

int main(){
/*	Setup & OpenGL Context Creation                                           {{{
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

	// glfw:	window creation
	// ------------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL){
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
	}

	glfwMakeContextCurrent(window);

	// glad:	load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
	}

	// glad: set window size
	// ---------------------
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//resize viewport on window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

/* ==================================================================
* Setup end }}}*/

	// Vertex array
	// ------------
	float vertices[]= {
            0.5f,   0.5f,   0.0f,   // top right
            0.5f,   -0.5f,  0.0f,   // bottom right
            -0.5f,  -0.5f,  0.0f,   // bottom left
            -0.5f,  0.5f,   0.0f    // top left
	};

    unsigned int indices[]{
            0,  1,  3,  // first triangle
            1,  2,  3   // second triangle
    };

	// Vertex Array Object & Vertex Buffer Object
	// --------------------
	unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO vertex array
    // ---------------------
    glBindVertexArray(VAO);

	// Bind VBO to target GL_ARRAY_BUFFER
	// ---------------------_-----_------
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Bind EBO to target GL_ELEMENT_ARRAY_BUFFER
    // ------------------------------------------
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Copy vertex data into VBO
	// -------------------------
	glBufferData(GL_ARRAY_BUFFER,
				sizeof(vertices),
				vertices,
				GL_STATIC_DRAW
	);

    // Copy Indices data to EBO
    // ------------------------
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

/*	Vertex Shader                                                             {{{
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
	}else{
			std::cout << "\nVertex Shader successfully compiled\n";
	}

/*	==================================================================
*	Vertex Shader end }}} */

/*	Fragment Shader                                                           {{{
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
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success){
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}else{
			std::cout << "\nFragment Shader successfully compiled\n";
	}

/*	==================================================================
*	Fragment Shader end }}} */

/*	Shader Program                                                            {{{
*	================================================================== */

	// Prepare shader program
	// ----------------------
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// Link vertex & fragment shader
	// -----------------------------
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check if linking failed
	// -----------------------
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success){
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}else{
			std::cout << "\nShader Program successfully compiled\n";
	}

/*	==================================================================
*	Shader Program end }}} */

	// Delete shader objects after being linked
	// ----------------------------------------
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Interpret VBO buffer data as vertices
	// -------------------------------------
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    // Safely unbind VBO as array buffer & unbind VAO (optional)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    /*  Do not unbind the EBO */

    // Polygon mode
    // ------------
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


/*	RENDER LOOP                                                               {{{
*	================================================================== */

	while(!glfwWindowShouldClose(window)){
			// input
			// -----
			processInput(window);

			// clear color buffer
			// ------------------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// use shader program
			// ------------------
			glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            //glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// glfw:	draw buffer and poll IO events
			// ---------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
	}
/*	==================================================================
*	Render Loop End }}} */

    // Deallocate resources
    // --------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();


	std::cout << "\n----------------------------------------------------------\
			\nSuccessfully exited\n";

	return 0;
}

std::string readTextFile(const char* filename){ //                           {{{
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

// process all input: query GLFW wheter relevant keys are pressed
// --------------------------------------------------------------
void processInput(GLFWwindow *window){ //                                    {{{
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
				std::cout << "Escape key pressed" << std::endl;
				glfwSetWindowShouldClose(window, true);
		}
} //}}}

// error callback
// --------------
void glfw_error(int error, const char* description){ //                      {{{
	fputs(description, stderr);
} //}}}

// resize window: resize glad viewport according to window size
// ------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){ //{{{
	glViewport(0, 0, width, height);
} //}}}
