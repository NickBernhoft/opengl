#ifndef GAME_UTIL_H
#define GAME_UTIL_H



// constants
#define MOVE_MULT 0.001f
#define GRAVITY 0.001f
#define FRICTION 0.003f
#define MAXSPEED 0.1f
#define MAXACCEL 0.01f


typedef struct body
{
	// vertex array attached to these physics bodies
	float* vertices;
	unsigned int vertCount;

	float pos[2];	// position
	float speed[2];
	float accel[2];

}body;


/*
Function prototypes
*/
float fclamp(float input, float max, float min);
const char* loadFile(char* filename);

//openGL shortcuts
GLuint buildVertexBuffer(const float* vertices);
const GLuint buildVertexShader(const char* vertex_shader_text);
const GLuint buildFragmentShader(const char* fragment_shader_text);
const GLuint buildProgram(GLuint vertex_shader, const GLuint fragment_shader);

// GLFW
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


// custom stuff
body* initBody(float vertices_array[], int arr_len, float xpos, float ypos);
void printBody(body* bodyPtr);
void processKeyboardInputs(body* body);
void boundCheck(body* body);
void addFriction(body* body);
void addGravity(body* body);
void limitSpeed(body* body);
void limitAccel(body* body);
void runBody(body* body);



#endif