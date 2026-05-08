#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE // just incase it doesnt detect glad
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linmath.h>

#include "gameutil.h"

static const char* vertex_shader_text;
static const char* fragment_shader_text;
const GLuint vertex_shader;



float vertices[] = 
{
    -0.1, -0.1,
    0.0, 0.1,
    0.1, -0.1 
};

float plat[] = 
{
    -0.6, -0.6,
    -0.6, -0.5,
    0.6, -0.6 ,

    0.6, -0.6,
    0.6, -0.5,
    -0.6, -0.5 

};
 


int main(void)
{

    // init bodies
    body* triangle_body;
    triangle_body = initBody(vertices, 6, 0.0, 0.0);

    body* plat_body;
    plat_body = initBody(plat, 12, 0.0, 0.0);

    int width = 640;
    int height = 480;


    if (!glfwInit())
    {
        printf("error in GLFW init :(\n");
    }
 
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL tester", NULL, NULL);
    if (!window)
    {
        printf("error creating window :(\n");
        glfwTerminate();
    }

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGL(); // set glad context
    glfwSwapInterval(1); // v-sync


    // load shaders
    vertex_shader_text = loadFile("vertex_shader.vs");
    fragment_shader_text = loadFile("fragment_shader.fs");

    printf("testing file reads:\n");
    printf("%s\n\n", vertex_shader_text);
    printf("%s\n\n", fragment_shader_text);




    // actual OpenGL code

    // making the program
    GLuint vertex_buffer = buildVertexBuffer(triangle_body->vertices);
    const GLuint vertex_shader = buildVertexShader(vertex_shader_text);
    const GLuint fragment_shader = buildFragmentShader(fragment_shader_text);
    const GLuint program = buildProgram(vertex_shader, fragment_shader);

    // setting the uniforms
    const GLint transformVec_location = glGetUniformLocation(program, "transformVec");
    const GLint colorVec_location = glGetUniformLocation(program, "colorVec");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    // attributes are the buffered data going into the shaders

    //vertex array to be rendered
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, 0, 0);



    int count = 0; 
    while (!glfwWindowShouldClose(window))
    {
        printf("Hello :D %d\n", count);
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT); // black background
        glfwPollEvents();

        // update stuff changed by inputs right before drawing the frame
        runBody(triangle_body);

        // main loop code

        
        // triangle body shader
        glUseProgram(program);
        glUniform2fv(transformVec_location, 1, triangle_body->pos);
        glUniform3f(colorVec_location, 1.0f, 1.0f, 1.0f); // tirangle fill color

        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, triangle_body->vertCount / 2); // execute the shader + pipeline


        glfwSwapBuffers(window); // display frame
        count++;
        glfwPollEvents();
    }


    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


