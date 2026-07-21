#ifndef GAME_UTIL_H
#define GAME_UTIL_H



// constants
#define MOVE_MULT 0.01f
#define GRAVITY 0.001f
#define FRICTION 0.003f
#define MAXSPEED 0.1f
#define MAXACCEL 0.01f

#define BL_DEFAULT_LEN 128

enum bodyType : uint8_t
{
	BODY = 0,
	PLAT = 1
};

typedef struct rect2D { float x, y, w, h; } rect2D;


/*
body is the fundamental object type in this engine
it conatinas all data needed to render with openGL
and the data relevent to the engine
*/
typedef struct body
{
	// vertex array attached to these physics bodies
	float* vertices; // reletave to pos
	unsigned int vertCount;
	GLuint vertexBuffer;

	rect2D bounds; // or hitbox

	// physics variables
	float posX, posY, speedX, speedY, accelX, accelY; 
	enum bodyType type;
}body;

/*were using an array for performance*/
typedef struct bodyList
{
	int end;
	int size;
	body* bodies[BL_DEFAULT_LEN];
}bodyList;


/*Function prototypes*/
float fclamp(float input, float max, float min);
const char* loadFile(char* filename);

/*openGL shortcuts*/
GLuint buildVertexBuffer(const float* vertices, size_t size);
const GLuint buildVertexShader(const char* vertex_shader_text);
const GLuint buildFragmentShader(const char* fragment_shader_text);
const GLuint buildProgram(GLuint vertex_shader, const GLuint fragment_shader);

/*GLFW*/
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


/*custom engine stuff*/
body* initBody(float vertices_array[], enum bodyType type, int arr_len, float xpos, float ypos);
void printBody(body* bodyPtr);
void cmdLnUp(int count);
bodyList* initBodyList();
int bodyListAdd(bodyList* list, body* bodyAdd);
int bodyPlatColision(body* b, body* plat);
rect2D bodyBounds(body* b);
void imvBody(body* b, float x, float y);
void mvBody(body* b, float x, float y);
void mvBodyBounds(body* b, float x, float y);

void processKeyboardInputs(body* body);
void boundCheck(body* body);
void addFriction(body* body);
void addGravity(body* body);
void limitSpeed(body* body);
void limitAccel(body* body);
void runBody(body* body);
void runPhy(body* body);



#endif