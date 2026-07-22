// engine stuff like inputs and basic checks

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE // just incase it doesnt detect glad
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <linmath.h>
#include <stdlib.h>
#include <float.h>

#include "gameutil.h"


// body buffer


// temp variables
int wasd[5] = {0};
enum movmentType mov_type = SPEED; // default to non accelleration based movment




/*
TODO: calculate the bounding box of the object.
*/
body* initBody(float vertices_array[], enum bodyType type, int arr_len, float xpos, float ypos)
{
    body* bodyPtr = calloc(1, sizeof(body)); // set all variables to 0

    bodyPtr->vertices = malloc(sizeof(float) * arr_len);
    memcpy(bodyPtr->vertices, vertices_array, arr_len * sizeof(float));
    bodyPtr->vertexBuffer = buildVertexBuffer(bodyPtr->vertices, arr_len);

    bodyPtr->vertCount = arr_len;
    bodyPtr->type = type;

    // set bounding box
    bodyPtr->bounds = bodyBounds(bodyPtr);

    printBody(bodyPtr);

    return bodyPtr;
}

/* creates a bounding box */
rect2D bodyBounds(body* b)
{
    rect2D ret;
    float minX = FLT_MAX, minY = FLT_MAX;
    float maxX = -FLT_MAX, maxY = -FLT_MAX;

    for(int i = 0; i < b->vertCount; i++)
    {
        if(i % 2 == 0) // for x vertices
        {
            minX = fminf(b->vertices[i], minX);
            maxX = fmaxf(b->vertices[i], maxX);
        }
        else // for y vertices
        {
            minY = fminf(b->vertices[i], minY);
            maxY = fmaxf(b->vertices[i], maxY);
        }
    }

    ret.x = minX;
    ret.y = minY;
    ret.w = maxX - minX;
    ret.h = maxY - minY;

    return ret;
}

void freeBody(body* b)
{
    free(b->vertices);
    free(b);
}

void printBody(body* bodyPtr)
{
    printf("vertices %x:\n", bodyPtr);
    for(int i = 0; i < bodyPtr->vertCount; i++)
    {
        printf("%f, ", bodyPtr->vertices[i]);
    }
    printf("\nvertCt:\t%d\n", bodyPtr->vertCount);
    printf("bounds:\t%f, %f, %f, %f\n", bodyPtr->bounds.x, bodyPtr->bounds.y, bodyPtr->bounds.w, bodyPtr->bounds.h);    
    printf("pos:\t%f, %f\n", bodyPtr->posX, bodyPtr->posY);
    printf("speed:\t%f, %f\n", bodyPtr->speedX, bodyPtr->speedY);
    printf("accel:\t%f, %f\n", bodyPtr->accelX, bodyPtr->accelY);
}

void cmdLnUp(int count)
{
    for(int i = 0; i < count; i++)
    {   
        // change this if the number of lines in printBody chanes
        printf("\033[F");
    }
    fflush(stdout);
}

bodyList* initBodyList()
{
    bodyList* list = malloc(sizeof(bodyList));
    list->end = 0;
    list->size = BL_DEFAULT_LEN;
    list->bodies[0] = NULL;
    return list;
}

/*
TODO: add auto resizing and auto remove
    empty slots from removed bodies.
*/
int bodyListAdd(bodyList* list, body* bodyAdd)
{
    // if unitialized
    if(list->bodies[0] == NULL)
    {
        list->bodies[0] = bodyAdd;
    }
    else if(list->end + 1 < list->size)
    {
        list->end++;
        list->bodies[list->end] = bodyAdd;
        return 0;
    }
    else
    {
        printf("Body List Full, make it dynamic lol");
        return -1;
    }
}

/* TODO */
int bodyPlatColision(body* b, body* plat)
{
    float newBoundsX = b->bounds.x;
    float newBoundsY = b->bounds.y;

    // check if theres a collision first
    if ( !(b->bounds.x < plat->bounds.x + plat->bounds.w &&
        b->bounds.x + b->bounds.w > plat->bounds.x &&
        b->bounds.y < plat->bounds.y + plat->bounds.h &&
        b->bounds.y + b->bounds.h > plat->bounds.y))
    {
        return 0;
    }

   
    return 1;
}

/* incemental move of the body and its bounds*/
void imvBody(body* b, float x, float y)
{
    b->bounds.x += x;
    b->bounds.y += y;
    b->posX += x;
    b->posY += y;
}

/* absolute move of the body about the posx/y. */
void mvBody(body* b, float x, float y)
{
    // find the difference between where we are and where were going
    double xDiff = (double)x - (double)b->posX;
    double yDiff = (double)y - (double)b->posY;

    // add this difference incremental style
    b->bounds.x += xDiff;
    b->bounds.y += yDiff;
    b->posX += xDiff;
    b->posY += yDiff;
}

/* absolute move of the body about the x/y of the bounds*/
void mvBodyBounds(body* b, float x, float y)
{
    // find the difference between where we are and where were going
    double xDiff = (double)x - (double)b->bounds.x;
    double yDiff = (double)y - (double)b->bounds.y;

    // add this difference incremental style
    b->bounds.x += xDiff;
    b->bounds.y += yDiff;
    b->posX += xDiff;
    b->posY += yDiff;
}

/*
clamp float between min and max
*/
float fclamp(float input, float max, float min)
{
    return fminf(fmaxf(input, min), max);
}


/*
creates a filled vertex buffer in VRAM and then returns a handle to it.
*/
GLuint buildVertexBuffer(const float vertices[], size_t size)
{
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
    if(key == GLFW_KEY_W && action == GLFW_PRESS)   wasd[0] = 1;
    if(key == GLFW_KEY_W && action == GLFW_RELEASE) wasd[0] = 0;

    if(key == GLFW_KEY_A && action == GLFW_PRESS)   wasd[1] = 1;
    if(key == GLFW_KEY_A && action == GLFW_RELEASE) wasd[1] = 0;

    if(key == GLFW_KEY_S && action == GLFW_PRESS)   wasd[2] = 1;
    if(key == GLFW_KEY_S && action == GLFW_RELEASE) wasd[2] = 0;

    if(key == GLFW_KEY_D && action == GLFW_PRESS)   wasd[3] = 1;
    if(key == GLFW_KEY_D && action == GLFW_RELEASE) wasd[3] = 0;

    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)   wasd[5] = 1;
    if(key == GLFW_KEY_SPACE && action == GLFW_RELEASE) wasd[5] = 0;



}


void processKeyboardInputs(body* b)
{   
    if(mov_type == SPEED)
    {
        b->speedX = MOVE_MULT * (wasd[3] - wasd[1]);
        b->speedY = MOVE_MULT * (wasd[0] - wasd[2]);
    }

    if(mov_type == ACCEL)
    {
        b->accelY += MOVE_MULT * wasd[0];
        b->accelX += MOVE_MULT * wasd[1] * -1;
        b->accelY += MOVE_MULT * wasd[2] * -1;
        b->accelX += MOVE_MULT * wasd[3];


        // no accelleration without inputs
        if(!wasd[0] && !wasd[2] || wasd[0] && wasd[2])
            b->accelY = 0.0;

        if(!wasd[1] && !wasd[3] || wasd[1] && wasd[3])
            b->accelX = 0.0;

        // jump
        if(wasd[5] == 1)
        {
            b->accelY += 0.1;
            wasd[5] = 0;
        }
    }
}

void runPhy(body* body)
{
    body->speedX += body->accelX;
    body->speedY += body->accelY;

    body->posX += body->speedX;
    body->posY += body->speedY;

    body->bounds.x += body->speedX;
    body->bounds.y += body->speedY;
}


void runBody(body* body)
{
    printBody(body);

    // process movment
    if(body->type == BODY)
    {
        processKeyboardInputs(body);
        runPhy(body);
    }
 
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
