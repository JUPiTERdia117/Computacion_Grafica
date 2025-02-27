//Practica#2 Monroy Salazar
//Fecha de entrega 13 de febrero de 2025
//315118894

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica 2", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		-0.2f,  0.5f, 0.0f,    1.0f,0.0f,0.0f,  //A
		-0.1f,  0.35f, 0.0f,   0.0f,0.0f,1.0f,//B
		0.1f,  0.35f, 0.0f,	   0.0f,1.0f,0.0f,//C
		0.2f,  0.5f, 0.0f,     1.0f,0.0f,1.0f,//D
		0.25f,  0.25f, 0.0f,   0.0f,1.0f,1.0f,//E
		0.0f,  0.0f, 0.0f,     1.0f,1.0f,1.0f,// O
		-0.25f,  0.25f, 0.0f,  0.5f,0.0f,1.0f,//F
		-0.05f,  0.05f, 0.0f,  0.0f,0.5f,1.0f,//G
		0.05f,  0.05f, 0.0f,   0.5f,0.5f,1.0f,//H

		-0.25f,  -0.5f, 0.0f,  1.0f,0.5f,1.0f,//I
		0.25f,  -0.5f, 0.0f,   1.0f,0.5f,0.5f,//J

		-0.2f,  -0.35f, 0.0f,  0.5f,1.0f,1.0f,//K
		-0.15f,  -0.25f, 0.0f, 0.5f,0.5f,0.5f,//L
		-0.25f,  -0.05f, 0.0f, 0.5f,1.0f,1.0f,//M
		-0.27f,  -0.15f, 0.0f, 0.0f,0.0f,1.0f,//N
		-0.45f,  -0.17f, 0.0f, 1.0f,0.0f,0.5f,//P




		
	};
	unsigned int indices[] = {  // note that we start from 0!
		0,1,6,//ABF
		2,3,4,//CDE
		4,5,6,//EOF
		1,2,4,//BCE
		4,6,1,//EFB
		5,9,10,//OIJ
		5,7,9,//OGI
		5,8,10,//OHJ
		12,13,14,//LMN
		11,12,14,//KLN
		13,14,15,//KLN
		
	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glBindVertexArray(VAO);


		glPointSize(10);
		glDrawArrays(GL_POINTS, 0, sizeof(vertices));

		//glDrawArrays(GL_LINES,0,4);
		glDrawArrays(GL_LINE_LOOP, 0, sizeof(vertices));
        
        //glDrawArrays(GL_TRIANGLES,0,3);
        glDrawElements(GL_TRIANGLES, 33,GL_UNSIGNED_INT, 0);
		

        
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}