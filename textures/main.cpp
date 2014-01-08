#define GLEW_STATIC

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <shaderSourceLoader.h>

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
	
	// Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {
    //  Position   Color             Texcoords
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	GLint result = GL_FALSE;
	
	GLchar*   vertexSource;
    GLchar*   fragmentSource;
	
    // Create and compile the vertex shader
    loadShaderSource("./resources/shaders/textures/vertexShader", vertexSource);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);
    glCompileShader(vertexShader);
    
    // Check Vertex Shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (result == GL_TRUE) {
    	std::cout<<"Loaded vertex shader"<<std::endl;
    } else {
    	std::cout<<"Failed to load vertex shader"<<std::endl;
    }
    
    unloadShaderSource(vertexSource);

    // Create and compile the fragment shader
    loadShaderSource("./resources/shaders/textures/fragmentShader", fragmentSource);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
    glCompileShader(fragmentShader);
    
    // Check Fragment Shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (result == GL_TRUE) {
    	std::cout<<"Loaded fragment shader"<<std::endl;
    } else {
    	std::cout<<"Failed to load fragment shader"<<std::endl;
    }
    
    unloadShaderSource(fragmentSource);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

    // Load textures
    GLuint textures[2];
    glGenTextures(2, textures);
    
    int width, height;
    unsigned char* image;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    image = SOIL_load_image("./resources/textures/kitty.png", &width, &height, 0, SOIL_LOAD_RGB);
    printf( "SOIL: '%s'\n", SOIL_last_result() );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    image = SOIL_load_image("./resources/textures/puppy.png", &width, &height, 0, SOIL_LOAD_RGB);
    printf( "SOIL: '%s'\n", SOIL_last_result() );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   

    while (!glfwWindowShouldClose(window))
    {
    	
    	// Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw a rectangle from the 2 triangles using 6 indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(2, textures);

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
