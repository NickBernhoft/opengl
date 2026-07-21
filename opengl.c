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
    int width = 640;
    int height = 480;


    if (!glfwInit())
    {
        printf("error in GLFW init :(\n");
    }
 
    GLFWwindow* window = glfwCreateWindow(width, height, "PLAT", NULL, NULL);
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


    // load shaders (embedded)
    vertex_shader_text = (const char[]){
    #embed "shaders/vertex_shader.vs" // #emebd must have its own line 
        , 0
    };

    fragment_shader_text = (const char[]){
    #embed "shaders/fragment_shader.fs"
        , 0
    };


    /*actual OpenGL code*/


    //using a global program for now

    // making the program
    const GLuint vertex_shader = buildVertexShader(vertex_shader_text);
    const GLuint fragment_shader = buildFragmentShader(fragment_shader_text);

    const GLuint program = buildProgram(vertex_shader, fragment_shader);

    // setting the uniform handles
    const GLint transformVec_location = glGetUniformLocation(program, "transformVec");
    const GLint colorVec_location = glGetUniformLocation(program, "colorVec");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    // attributes are the buffered data that the shaders do their math on


    // use global vertex array for now
    //define how the format that the shader will interpruit the buffered data in
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);



    // init bodies
    body* triangle_body;
    triangle_body = initBody(vertices, BODY, 6, 0.0, 0.0);

    body* plat_body;
    plat_body = initBody(plat, PLAT, 12, 0.0, 0.0);

    // list of bodies to be rendered
    bodyList* renderBodies = initBodyList();
    bodyListAdd(renderBodies, triangle_body);
    bodyListAdd(renderBodies, plat_body);




    // pre loop settings
    glUseProgram(program);
    glBindVertexArray(vertex_array);

    int count = 0; 
    while (!glfwWindowShouldClose(window))
    {
        printf("Frame: %d\n", count);
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT); // black background
        glfwPollEvents();

   
        // render all bodies in the list
        int i;
        cmdLnUp(8 * 2);
        for(i = 0; i <= renderBodies->end; i++)
        {
            body* curBody = renderBodies->bodies[i];

            runBody(curBody);

            // temporairly doing the hit detection manually
            int paltCol = bodyPlatColision(triangle_body, plat_body);
            if (paltCol != 0) {printf("plat Body Collision");}

            //bind VBO
            glBindBuffer(GL_ARRAY_BUFFER, curBody->vertexBuffer);
            // sets VAO to use current VBO
            glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, 0, 0);

            glUniform2fv(transformVec_location, 1, (float[2]){curBody->posX, curBody->posY});
            glUniform3f(colorVec_location, 1.0f, 1.0f, 1.0f); // tirangle fill color
            glDrawArrays(GL_TRIANGLES, 0, curBody->vertCount / 2);
        }






        glfwSwapBuffers(window); // display frame
        count++;
    }


    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


