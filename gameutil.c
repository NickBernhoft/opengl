// engine stuff like inputs and basic checks

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE // just incase it doesnt detect glad
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <linmath.h>
#include <stdlib.h>

#include "gameutil.h"




// temp variables
int wasd[5] = {0};




/*
make a vertex buffer fron an array of floats
this causes a crash?
*/
body* initBody(float vertices_array[], int arr_len, float xpos, float ypos)
{
    body* bodyPtr = malloc(sizeof(body));

    bodyPtr->vertices = malloc(sizeof(float) * arr_len);
    memcpy(bodyPtr->vertices, vertices_array, arr_len * sizeof(float));
    
    bodyPtr->vertCount = arr_len;
    bodyPtr->pos[0] = xpos;
    bodyPtr->pos[1] = ypos;
    bodyPtr->speed[0] = 0.0;
    bodyPtr->speed[1] = 0.0;
    bodyPtr->accel[0] = 0.0;
    bodyPtr->accel[1] = 0.0;

    printBody(bodyPtr);

    return bodyPtr;
}

void freeBody(body* bodyIn)
{
    free(bodyIn->vertices);
    free(bodyIn);
}

void printBody(body* bodyPtr)
{
    printf("vertices %x:\n", bodyPtr);
    for(int i = 0; i < bodyPtr->vertCount; i++)
    {
        printf("%f, ", bodyPtr->vertices[i]);
    }
    printf("\nvertCt:\t%d\n", bodyPtr->vertCount);
    printf("pos:\t%f, %f\n", bodyPtr->pos[0], bodyPtr->pos[1]);
    printf("speed:\t%f, %f\n", bodyPtr->speed[0], bodyPtr->speed[1]);
    printf("accel:\t%f, %f\n", bodyPtr->accel[0], bodyPtr->accel[1]);

}






/*
clamp float between min and max
*/
float fclamp(float input, float max, float min)
{
    return fminf(fmaxf(input, min), max);
}



/*
creates a string with the contents of a file
*/
const char* loadFile(char* filename) {
    FILE* fp = fopen(filename, "rb"); // Open file in binary read mode
    char* buffer = NULL;
    long filesize = 0;

    if (!fp) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return NULL;
    }

    // Go to the end of the file to get the size
    if (fseek(fp, 0L, SEEK_END) == 0) {
        filesize = ftell(fp);
        if (filesize == -1) {
            perror("ftell error");
            fclose(fp);
            return NULL;
        }

        // Allocate memory for the buffer (+1 for null terminator)
        buffer = (char*)malloc(sizeof(char) * (filesize + 1));
        if (!buffer) {
            fprintf(stderr, "loadFile() Memory allocation failed\n");
            fclose(fp);
            return NULL;
        }

        // Go back to the start of the file
        rewind(fp);

        // Read the entire file into memory
        size_t newLen = fread(buffer, sizeof(char), (size_t)filesize, fp);
        if (ferror(fp) != 0) {
            fprintf(stderr, "loadFile() Error reading file\n");
            free(buffer);
            fclose(fp);
            return NULL;
        } else {
            buffer[newLen] = '\0'; // Ensure null-termination
        }
    }

    fclose(fp);
    return buffer; // Caller is responsible for freeing the memory
}



/*
creates a single array vertex array buffer handle
TODO: make the size of the vertex array automatically fit the input array
*/
GLuint buildVertexBuffer(const float vertices[])
{
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertices, GL_STATIC_DRAW);

    return vertex_buffer;
}



/*
returns a single vertex shader handle
*/
const GLuint buildVertexShader(const char* vertex_shader_text)
{
    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    return vertex_shader;
}



/*
returns a single fragment shader handle
*/
const GLuint buildFragmentShader(const char* fragment_shader_text)
{
    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    return fragment_shader;
}



const GLuint buildProgram(GLuint vertex_shader, const GLuint fragment_shader)
{
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    return program;
}







void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // wasd movment
    if(key == GLFW_KEY_W && action == GLFW_PRESS) // set key status pressed
        wasd[0] = 1;
    if(key == GLFW_KEY_W && action == GLFW_RELEASE) // set key status empty
        wasd[0] = 0;

    if(key == GLFW_KEY_A && action == GLFW_PRESS)   wasd[1] = 1;
    if(key == GLFW_KEY_A && action == GLFW_RELEASE) wasd[1] = 0;

    if(key == GLFW_KEY_S && action == GLFW_PRESS)   wasd[2] = 1;
    if(key == GLFW_KEY_S && action == GLFW_RELEASE) wasd[2] = 0;

    if(key == GLFW_KEY_D && action == GLFW_PRESS)   wasd[3] = 1;
    if(key == GLFW_KEY_D && action == GLFW_RELEASE) wasd[3] = 0;

    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)   wasd[5] = 1;
    if(key == GLFW_KEY_SPACE && action == GLFW_RELEASE) wasd[5] = 0;



}

void addFriction(body* body)
{
    // tp prevent drifting
    if(body->speed[0] < FRICTION && body->speed[0] > FRICTION * -1.0)
        body->speed[0] = 0.0;

    if(body->speed[1] < FRICTION && body->speed[1] > FRICTION * -1.0)
        body->speed[1] = 0.0;


    // slows down speed by the FRICTION modifier
    if(body->speed[0] > FRICTION)
        body->speed[0] -= FRICTION;

    if(body->speed[0] < FRICTION * -1.0)
        body->speed[0] += FRICTION;

    if (body->speed[0] > FRICTION && body->speed[0] < FRICTION * -1.0)
        body->speed[0] = 0;

    if(body->speed[1] > FRICTION)
        body->speed[1] -= FRICTION;

    if(body->speed[1] < FRICTION * -1)
        body->speed[1] += FRICTION;

    if (body->speed[1] > FRICTION * 2 && body->speed[0] < FRICTION * -1.0)
        body->speed[1] = 0;
}

void processKeyboardInputs(body* body)
{   
    body->accel[1] += MOVE_MULT * wasd[0];
    body->accel[0] += MOVE_MULT * wasd[1] * -1;
    body->accel[1] += MOVE_MULT * wasd[2] * -1;
    body->accel[0] += MOVE_MULT * wasd[3];


    // no accelleration without inputs
    if(!wasd[0] && !wasd[2] || wasd[0] && wasd[2])
        body->accel[1] = 0.0;

    if(!wasd[1] && !wasd[3] || wasd[1] && wasd[3])
        body->accel[0] = 0.0;

    // jump
    if(wasd[5] == 1)
    {
        body->accel[1] += 0.015;
        wasd[5] = 0;
    }
}

void limitSpeed(body* body)
{
    //speed limit
    body->speed[0] = fclamp(body->speed[0], MAXSPEED, MAXSPEED * -1.0);
    body->speed[1] = fclamp(body->speed[1], MAXSPEED, MAXSPEED * -1.0);
}

void limitAccel(body* body)
{
    body->accel[0] = fclamp(body->accel[0], MAXACCEL, MAXACCEL * -1.0);
    body->accel[1] = fclamp(body->accel[1], MAXACCEL, MAXACCEL * -1.0);
}



void addGravity(body* body)
{
    body->accel[1] -= GRAVITY;
}

/*
directly modifies the body given
*/
void boundCheck(body* body)
{
    float *transformVec = body->pos;

    if(transformVec[0] > 1.0)
    {
        body->pos[0] = 1.0;
        body->speed[0] = 0.0;
        body->accel[0] = 0.0;     
    }


    if(transformVec[0] < -1.0)
    {
        body->pos[0] = -1.0;
        body->speed[0] = 0.0;
        body->accel[0] = 0.0;
    }


    if(transformVec[1] > 1.0)
    {
        body->pos[1] = 1.0;
        body->speed[1] = 0.0;
        body->accel[1] = 0.0;

    }
    
    if(body->pos[1] < -1.0)
    {
        body->pos[1] = -1.0;
        body->speed[1] = 0.0;   // if you hit the bottom bound arrest your accelleration
        body->accel[1] = 0.0;
    }
}


void runBody(body* body)
{
    processKeyboardInputs(body);
    boundCheck(body);
    addFriction(body);
    addGravity(body);
    limitSpeed(body);
    limitAccel(body);
}
