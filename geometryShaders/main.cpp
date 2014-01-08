#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
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

// Shader creation helper
GLuint createShader(GLenum type, const GLchar* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&src, nullptr);
    glCompileShader(shader);
    return shader;
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
    
    GLchar* vertexSource;
    GLchar* fragmentSource;
    GLchar* geometrySource;
    
    loadShaderSource("./resources/shaders/geometryShaders/vertexShader.vert", vertexSource);
    loadShaderSource("./resources/shaders/geometryShaders/fragmentShader.frag", fragmentSource);
    loadShaderSource("./resources/shaders/geometryShaders/geometryShader.geom", geometrySource);
    
    // Compile and activate shaders
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexSource);
    GLuint geometryShader = createShader(GL_GEOMETRY_SHADER, geometrySource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Create VBO with point coordinates
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat points[] = {
    //  Coordinates  Color             Sides
        -0.45f,  0.45f, 1.0f, 0.0f, 0.0f,  4.0f,
         0.45f,  0.45f, 0.0f, 1.0f, 0.0f,  8.0f,
         0.45f, -0.45f, 0.0f, 0.0f, 1.0f, 16.0f,
        -0.45f, -0.45f, 1.0f, 1.0f, 0.0f, 32.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // Create VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Specify layout of point data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

    GLint sidesAttrib = glGetAttribLocation(shaderProgram, "sides");
    glEnableVertexAttribArray(sidesAttrib);
    glVertexAttribPointer(sidesAttrib, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));
	
    while (!glfwWindowShouldClose(window))
    {
    	
    	// Render frame
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_POINTS, 0, 4);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
