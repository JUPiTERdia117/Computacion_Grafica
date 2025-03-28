//Practica#8 (Iluminacion) Monroy Salazar
//Fecha de entrega 28 de Marzo de 2025
//315118894



// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(0.0f, 6.0f, 0.0f);
float lightAngle = glm::radians(90.0f); // Ángulo inicial en 90 grados (eje Y)
float lightRadius = 6.0f;
glm::vec3 lightPos2(0.0f, 6.0f, 0.0f);
float lightAngle2 = glm::radians(90.0f); // Ángulo inicial en 90 grados (eje Y)
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

bool useSecondLight = false; // Controla si la segunda luz está activa

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 Iluminacion Monroy Salazar", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    



    // Load models
    Model dog((char*)"Models/RedDog.obj");
    Model chair((char*)"Models/Chair/kursi.obj");
    Model tablec((char*)"Models/Table/Chair_and_Table_School.obj");
    Model room((char*)"Models/Room/Room.obj");
    Model board((char*)"Models/Piza/pizar.obj");
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Actualizar posición de la luz basada en el ángulo
        lightPos.x = lightRadius * cos(lightAngle);
        lightPos.y = lightRadius * sin(lightAngle);

        lightPos2.x = lightRadius * cos(lightAngle2);
        lightPos2.y = lightRadius * sin(lightAngle2);
       

        
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x , lightPos.y, lightPos.z);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        
        GLint lightPos2Loc = glGetUniformLocation(lightingShader.Program, "light2.position");
        glUniform3f(lightPos2Loc, lightPos2.x, lightPos2.y , lightPos2.z );



        // Set lights properties
        
        if (useSecondLight)
        {
            // Luz 2 activa, Luz 1 inactiva
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);

            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.5f, 0.5f, 0.5f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.75f, 0.75f, 0.75f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.2f, 0.2f, 0.2f);

            // Set material properties

            // Azul
            
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.1f, 0.1f, 0.3f);  
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.3f, 0.3f, 0.6f);  
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.5f, 0.5f, 1.0f); 
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 8.0f);
        }
        else
        {
            // Luz 1 activa, Luz 2 inactiva
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.4f, 0.4f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.65f, 0.65f, 0.65f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.4f, 0.4f, 0.4f);

            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.0f, 0.0f, 0.0f);

            // Set material properties
            //Amarillo
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.4f, 0.4f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.9f, 0.9f, 0.9f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);
        }


        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        

        





        

        
       

        //Cuarto
        glm::mat4 modelR(1);
        modelR = glm::translate(modelR, glm::vec3(0.0f, -0.45f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelR));
        room.Draw(lightingShader);


        //Silla
        glm::mat4 modelChair(1);
        modelChair = glm::translate(modelChair, glm::vec3(0.0f, -0.5f, 0.0f));
        modelChair = glm::scale(modelChair, glm::vec3(0.4f, 0.4f, 0.4f));
        modelChair = glm::rotate(modelChair, -55.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelChair));
        chair.Draw(lightingShader);

        //Silla2
        glm::mat4 modelChair2(1);
        modelChair2 = glm::translate(modelChair2, glm::vec3(-1.0f, -0.5f, 0.0f));
        modelChair2 = glm::scale(modelChair2, glm::vec3(0.4f, 0.4f, 0.4f));
        modelChair2 = glm::rotate(modelChair2, -55.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelChair2));
        chair.Draw(lightingShader);

        //Silla3
        glm::mat4 modelChair3(1);
        modelChair3 = glm::translate(modelChair3, glm::vec3(1.0f, -0.5f, 0.0f));
        modelChair3 = glm::scale(modelChair3, glm::vec3(0.4f, 0.4f, 0.4f));
        modelChair3 = glm::rotate(modelChair3, -55.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelChair3));
        chair.Draw(lightingShader);

        //Silla4
        glm::mat4 modelChair4(1);
        modelChair4 = glm::translate(modelChair4, glm::vec3(0.0f, -0.5f, 1.0f));
        modelChair4 = glm::scale(modelChair4, glm::vec3(0.4f, 0.4f, 0.4f));
        modelChair4 = glm::rotate(modelChair4, -55.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelChair4));
        chair.Draw(lightingShader);

        //Silla5
        glm::mat4 modelChair5(1);
        modelChair5 = glm::translate(modelChair5, glm::vec3(-1.0f, -0.5f, 1.0f));
        modelChair5 = glm::scale(modelChair5, glm::vec3(0.4f, 0.4f, 0.4f));
        modelChair5 = glm::rotate(modelChair5, -55.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelChair5));
        chair.Draw(lightingShader);

        //Silla3
        glm::mat4 modelChair6(1);
        modelChair6 = glm::translate(modelChair6, glm::vec3(1.0f, -0.5f, 1.0f));
        modelChair6 = glm::scale(modelChair6, glm::vec3(0.4f, 0.4f, 0.4f));
        modelChair6 = glm::rotate(modelChair6, -55.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelChair6));
        chair.Draw(lightingShader);


        //Pizarron
        glm::mat4 modelBoard(1);
        modelBoard = glm::translate(modelBoard, glm::vec3(0.0f, 0.0f, -3.0f));
        modelBoard = glm::scale(modelBoard, glm::vec3(0.01f, 0.01f, 0.01f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBoard));
        board.Draw(lightingShader);



        //Mesa
        glm::mat4 modelTable(1);
        modelTable = glm::translate(modelTable, glm::vec3(-2.3f, -0.5f, -2.0f));
        modelTable = glm::rotate(modelTable, 135.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        modelTable = glm::scale(modelTable, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTable));
        tablec.Draw(lightingShader);


        // Draw the loaded model dogo
        glm::mat4 mDog(1);
        mDog = glm::translate(mDog, glm::vec3(-2.2f, 0.58f, -1.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mDog));
        dog.Draw(lightingShader);
        

        




        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 lampModel(1.0f);
        if (useSecondLight)
        {
            // Dibujar lámpara de la segunda luz
            lampModel = glm::translate(lampModel, lightPos2);
        }
        else
        {
            // Dibujar lámpara de la primera luz
            lampModel = glm::translate(lampModel, lightPos);
        }
        lampModel = glm::scale(lampModel, glm::vec3(0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(lampModel));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_R] && !useSecondLight)
    {
        lightAngle += 2.0f * deltaTime; // Rotar en sentido horario
    }

    if (keys[GLFW_KEY_F] && !useSecondLight)
    {
        lightAngle -= 2.0f * deltaTime; // Rotar en sentido antihorario
    }

    if (keys[GLFW_KEY_T] && useSecondLight)
    {
        lightAngle2 += 2.0f * deltaTime; // Rotar en sentido horario
    }

    if (keys[GLFW_KEY_G] && useSecondLight)
    {
        lightAngle2 -= 2.0f * deltaTime; // Rotar en sentido antihorario
    }
    

    if (keys[GLFW_KEY_Z])
    {
        useSecondLight = !useSecondLight; // Alternar estado al presionar Z
    }


}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}


