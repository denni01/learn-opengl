#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"    FragColor = ourColor;\n"
"}\n\0";

const char *blueFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.1, 0.1, 1.0, 1.0);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Configures some OpenGL things so we don't have to manually configure it
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// Create vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	// Print message if vertex shader compilation failed
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "Error compiling shader:\n" << infoLog << endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	// Print message if fragment shader compilation failed
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "Error compiling shader:\n" << infoLog << endl;
	}

	int blueFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(blueFragmentShader, 1, &blueFragmentShaderSource, NULL);
	glCompileShader(blueFragmentShader);

	glGetShaderiv(blueFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(blueFragmentShader, 512, NULL, infoLog);
		cout << "Error compiling shader:\n" << infoLog << endl;
	}

	// Link Shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Print message if shader linking failed
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "Error linking shaders:\n" << infoLog << endl;
	}

	int secondShaderProgram = glCreateProgram();
	glAttachShader(secondShaderProgram, vertexShader);
	glAttachShader(secondShaderProgram, blueFragmentShader);
	glLinkProgram(secondShaderProgram);

	glGetProgramiv(secondShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(secondShaderProgram, 512, NULL, infoLog);
		cout << "Error linking shaders:\n" << infoLog << endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		0.5, 0.5, 0.0, // 0
		0.5, -0.5, 0.0, // 1
		-0.5, -0.5, 0.0, // 2
		-0.5, 0.5, 0.0 // 3
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	// Create a vertex buffer object, vertex array object, and element buffer object
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind a vertex array object to configure vertex attribute data
	glBindVertexArray(VAO);

	// Copy data into vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unbind first buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float secondTriangleVertices[] = {
		0.75, 0.5, 0.0, // 0
		0.75, -0.5, 0.0, // 1
		1.0, 0.5, 0.0 // 2
	};

	unsigned int secondTriangleIndices[] = {
		0, 1, 2 // third triangle
	};

	// Create a vertex buffer object, vertex array object, and element buffer object for the last triangle
	unsigned int VBO2, VAO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	// Bind second vertex array object to configure vertex attribute data
	glBindVertexArray(VAO2);

	// Copy data into vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangleVertices), secondTriangleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(secondTriangleIndices), secondTriangleIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unbind second buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		// Process input
		processInput(window);

		// Clear current screen
		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);


		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0) + 0.5;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		// Render output using shader program
		glUseProgram(shaderProgram);
		glUniform4f(vertexColorLocation, 0.0, greenValue, 0.0, 1.0);
		

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(secondShaderProgram);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		// Swap out buffers and listen for events for the next iteration
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}