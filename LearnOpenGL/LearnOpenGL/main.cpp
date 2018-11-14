#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <stb/stb_image.h>

using namespace std;

float mixAmount = 0.2;

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

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixAmount += 0.01;
		if (mixAmount >= 1.0)
		{
			mixAmount = 1.0;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixAmount -= 0.01;
		if (mixAmount <= 0.0)
		{
			mixAmount = 0.0;
		}
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

	Shader shader("vertex.vs", "fragment.fs");

	// Vertex Data
	float vertices[] = {
	//   x     y    z    r    g    b    s    t
		 0.5,  0.5, 0.0, 1.0, 0.0, 0.0, 2.0, 2.0, // 0
		 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 2.0, 0.0, // 1
		-0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, // 2
		-0.5,  0.5, 0.0, 1.0, 1.0, 0.0, 0.0, 2.0  // 3
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	float textureCoordinates[] = {
		0.0, 0.0,
		1.0, 0.0,
		0.5, 1.0
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

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind first buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);	

	// create texture and set options
	unsigned int texture, texture2;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image
	int width, height, nrChannels;
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}

	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}

	stbi_image_free(data);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		// Process input
		processInput(window);

		// Clear current screen
		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures to texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Render output using shader program
		shader.use();
		shader.setInt("texture1", 0);
		shader.setInt("texture2", 1);
		shader.setFloat("mixAmount", mixAmount);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap out buffers and listen for events for the next iteration
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}