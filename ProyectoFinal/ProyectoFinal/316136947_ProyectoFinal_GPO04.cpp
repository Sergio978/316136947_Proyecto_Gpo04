#include <iostream>
#include<windows.h>
#include <cmath>

// GLEW is a library that provides methods that can be implemented at runtime
#include <GL/glew.h>

// GLFW is a library dedicated to the management of windows and keyboard events
#include <GLFW/glfw3.h>

// Other Libs for image loading
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models and for uploading textures into OpenGL
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode); //This function is called when a especified key via GLFW is pressed
void MouseCallback(GLFWwindow* window, double xPos, double yPos);// This function is for the camera configuration using mouse as input 
void DoMovement(); //The function indicates what will be performed by the keys that are pressed and under which strict event

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera:
Camera camera(glm::vec3(10.0f, 10.0f, 35.0f)); //An instance of the camera class is declared and initialized
/* lastXand lastY, 
allow you to store the last mouse positions in the app, which we initialize to be in the center 
of the screen(the screen size is 800 by 600) initially */
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

bool keys[1024]; //An array is defined, this stores the key set via GLFW 
bool firstMouse = true; //Defining a global bool variable to check if this is the first time we receive mouse input

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);


//Animation
/*
	Variables with which it is allowed to start and perform a certain animation
*/
bool active;
float rot = 0.0f;
float rotP = 0.0f;
bool open = false;
bool close = true;
bool anim = false;
bool anim_dos = false;
float avanzo = 10.8f;
bool open2 = false;
bool close2 = true;
bool anim_tres = false;
float giro = 0.0f;

bool anim_P = false;
float rise = 9.1;
float scale= 1.5;
float PosY[380] = { 9.14105, 9.18211, 9.22316, 9.26421, 9.30526, 9.34632, 9.38737, 9.42842, 9.46948, 9.51053, 9.55158, 9.59263, 9.63369, 9.67474, 9.71579, 9.75685, 9.7979, 9.83895, 9.88, 9.92106, 9.96211, 10.0032, 10.0442, 10.0853, 10.1263, 10.1674, 10.2084, 10.2495, 10.2905, 10.3316, 10.3726, 10.4137, 10.4547, 10.4958, 10.5368, 10.5779, 10.619, 10.66, 10.7011, 10.7421, 10.7832, 10.8242, 10.8653, 10.9063, 10.9474, 10.9884, 11.0295, 11.0705, 11.1116, 11.1526, 11.1937, 11.2347, 11.2758, 11.3169, 11.3579, 11.399, 11.44, 11.4811, 11.5221, 11.5632, 11.6042, 11.6453, 11.6863, 11.7274, 11.7684, 11.8095, 11.8505, 11.8916, 11.9326, 11.9737, 12.0148, 12.0558, 12.0969, 12.1379, 12.179, 12.22, 12.2611, 12.3021, 12.3432, 12.3842, 12.4253, 12.4663, 12.5074, 12.5484, 12.5895, 12.6305, 12.6716, 12.7126, 12.7537, 12.7948, 12.8358, 12.8769, 12.9179, 12.959, 13, 13.0411, 13.0821, 13.1232, 13.1642, 13.2053, 13.2463, 13.2874, 13.3284, 13.3695, 13.4105, 13.4516, 13.4927, 13.5337, 13.5748, 13.6158, 13.6569, 13.6979, 13.739, 13.78, 13.8211, 13.8621, 13.9032, 13.9442, 13.9853, 14.0263, 14.0674, 14.1084, 14.1495, 14.1905, 14.2316, 14.2727, 14.3137, 14.3548, 14.3958, 14.4369, 14.4779, 14.519, 14.56, 14.6011, 14.6421, 14.6832, 14.7242, 14.7653, 14.8063, 14.8474, 14.8884, 14.9295, 14.9706, 15.0116, 15.0527, 15.0937, 15.1348, 15.1758, 15.2169, 15.2579, 15.299, 15.34, 15.3811, 15.4221, 15.4632, 15.5042, 15.5453, 15.5863, 15.6274, 15.6685, 15.7095, 15.7506, 15.7916, 15.8327, 15.8737, 15.9148, 15.9558, 15.9969, 16.0379, 16.079, 16.12, 16.1611, 16.2021, 16.2432, 16.2842, 16.3253, 16.3663, 16.4074, 16.4484, 16.4895, 16.5305, 16.5716, 16.6127, 16.6537, 16.6948, 16.7358, 16.7769, 16.8179, 16.859, 16.9, 16.8579, 16.8158, 16.7737, 16.7316, 16.6895, 16.6474, 16.6053, 16.5632, 16.5211, 16.479, 16.4369, 16.3948, 16.3527, 16.3105, 16.2684, 16.2263, 16.1842, 16.1421, 16.1, 16.0579, 16.0158, 15.9737, 15.9316, 15.8895, 15.8474, 15.8053, 15.7632, 15.7211, 15.679, 15.6369, 15.5948, 15.5527, 15.5105, 15.4684, 15.4263, 15.3842, 15.3421, 15.3, 15.2579, 15.2158, 15.1737, 15.1316, 15.0895, 15.0474, 15.0053, 14.9632, 14.9211, 14.879, 14.8369, 14.7948, 14.7526, 14.7105, 14.6684, 14.6263, 14.5842, 14.5421, 14.5, 14.4579, 14.4158, 14.3737, 14.3316, 14.2895, 14.2474, 14.2053, 14.1632, 14.1211, 14.079, 14.0368, 13.9947, 13.9526, 13.9105, 13.8684, 13.8263, 13.7842, 13.7421, 13.7, 13.6579, 13.6158, 13.5737, 13.5316, 13.4895, 13.4474, 13.4053, 13.3632, 13.3211, 13.2789, 13.2368, 13.1947, 13.1526, 13.1105, 13.0684, 13.0263, 12.9842, 12.9421, 12.9, 12.8579, 12.8158, 12.7737, 12.7316, 12.6895, 12.6474, 12.6053, 12.5632, 12.521, 12.4789, 12.4368, 12.3947, 12.3526, 12.3105, 12.2684, 12.2263, 12.1842, 12.1421, 12.1, 12.0579, 12.0158, 11.9737, 11.9316, 11.8895, 11.8474, 11.8052, 11.7631, 11.721, 11.6789, 11.6368, 11.5947, 11.5526, 11.5105, 11.4684, 11.4263, 11.3842, 11.3421, 11.3, 11.2579, 11.2158, 11.1737, 11.1316, 11.0895, 11.0473, 11.0052, 10.9631, 10.921, 10.8789, 10.8368, 10.7947, 10.7526, 10.7105, 10.6684, 10.6263, 10.5842, 10.5421, 10.5, 10.4579, 10.4158, 10.3737, 10.3315, 10.2894, 10.2473, 10.2052, 10.1631, 10.121, 10.0789, 10.0368, 9.9947, 9.9526, 9.91049, 9.86839, 9.82628, 9.78418, 9.74207, 9.69996, 9.65786, 9.61575, 9.57365, 9.53154, 9.48944, 9.44733, 9.40522, 9.36312, 9.32101, 9.27891, 9.2368, 9.1947, 9.15259, 9.11049, 9.06838, 9.02627, 8.98417, 8.94206, 8.89996 };

float Esc[380] = { 1.30105, 1.30211, 1.30316, 1.30421, 1.30526, 1.30632, 1.30737, 1.30842, 1.30947, 1.31053, 1.31158, 1.31263, 1.31368, 1.31474, 1.31579, 1.31684, 1.31789, 1.31895, 1.32, 1.32105, 1.3221, 1.32316, 1.32421, 1.32526, 1.32632, 1.32737, 1.32842, 1.32947, 1.33053, 1.33158, 1.33263, 1.33368, 1.33474, 1.33579, 1.33684, 1.33789, 1.33895, 1.34, 1.34105, 1.3421, 1.34316, 1.34421, 1.34526, 1.34632, 1.34737, 1.34842, 1.34947, 1.35053, 1.35158, 1.35263, 1.35368, 1.35474, 1.35579, 1.35684, 1.35789, 1.35895, 1.36, 1.36105, 1.3621, 1.36316, 1.36421, 1.36526, 1.36631, 1.36737, 1.36842, 1.36947, 1.37053, 1.37158, 1.37263, 1.37368, 1.37474, 1.37579, 1.37684, 1.37789, 1.37895, 1.38, 1.38105, 1.3821, 1.38316, 1.38421, 1.38526, 1.38631, 1.38737, 1.38842, 1.38947, 1.39053, 1.39158, 1.39263, 1.39368, 1.39474, 1.39579, 1.39684, 1.39789, 1.39895, 1.4, 1.40105, 1.4021, 1.40316, 1.40421, 1.40526, 1.40631, 1.40737, 1.40842, 1.40947, 1.41052, 1.41158, 1.41263, 1.41368, 1.41474, 1.41579, 1.41684, 1.41789, 1.41895, 1.42, 1.42105, 1.4221, 1.42316, 1.42421, 1.42526, 1.42631, 1.42737, 1.42842, 1.42947, 1.43052, 1.43158, 1.43263, 1.43368, 1.43474, 1.43579, 1.43684, 1.43789, 1.43895, 1.44, 1.44105, 1.4421, 1.44316, 1.44421, 1.44526, 1.44631, 1.44737, 1.44842, 1.44947, 1.45052, 1.45158, 1.45263, 1.45368, 1.45473, 1.45579, 1.45684, 1.45789, 1.45895, 1.46, 1.46105, 1.4621, 1.46316, 1.46421, 1.46526, 1.46631, 1.46737, 1.46842, 1.46947, 1.47052, 1.47158, 1.47263, 1.47368, 1.47473, 1.47579, 1.47684, 1.47789, 1.47895, 1.48, 1.48105, 1.4821, 1.48316, 1.48421, 1.48526, 1.48631, 1.48737, 1.48842, 1.48947, 1.49052, 1.49158, 1.49263, 1.49368, 1.49473, 1.49579, 1.49684, 1.49789, 1.49894, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5 };

int i=0;
// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-6.9f, 6.75f, 6.4f), //Located in the first ceiling lamp inside the living room of the house
	glm::vec3(-6.9f, 6.75f, -4.2f), //Located in the second ceiling lamp inside the living room of the house
	glm::vec3(4.5f,15.6f,-2.5f), //Located in the ceiling lamp inside tommy's room
	glm::vec3(10.8f, 11.75f, 3.2f), //Located in the light bulb of the lamp object at tommy's room
	glm::vec3(-5.05f, 5.0f, -8.48f) //Located in the light bulb of the lamp object at the living room of the house
};

/*
Vertices data are defined, in this case position and normals; 
the geometry of the vertices form a cube, it will be used if it's necessary
*/
float vertices[] = {
	//Position             //Normals
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

/*
A vector 3 is declared for each light element, 
so that the ambiental and diffuse property can be controlled,
allowing a light color to be handled
*/
glm::vec3 Light1 = glm::vec3(0);
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);
glm::vec3 Light4 = glm::vec3(0);
glm::vec3 Light5 = glm::vec3(0);

/*
Deltatime:
Stores the time it took to render the last frame. 
By using this approach the camera speed will be balanced
*/
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main() //Main function
{
	//Initializes the GLFW library
	glfwInit();
	
	/*
	Set all the required options for GLFW
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Tommy's House", nullptr, nullptr);

	//In case the window object is not created correctly error message is sent in console
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		/*
		If there is a fail, this function will destroy all remaining windows and cursors, 
		restores any modified gamma ramps and frees any other allocated resources.
		*/
		glfwTerminate();

		return EXIT_FAILURE; //Defined value for failure, 1
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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


	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	
	//Models
	Model House((char*)"Models/House/House_No_Windows_unsized.obj");
	Model Windows((char*)"Models/House/Windows_unsized.obj");
	Model Saguan((char*)"Models/House/Saguan_unsized.obj");

	Model Cortinas((char*)"Models/Cortinas/Cortinas.obj");
	Model Bed((char*)"Models/Cama/Bed.obj");

	Model Chest_B((char*)"Models/Chest/Base.obj");
	Model Chest_S((char*)"Models/Chest/Superior.obj");

	Model Pelota((char*)"Models/Accesorios/pelota.obj");

	Model Reptar((char*)"Models/Reptar/reptar.obj");

	Model Cajonera((char*)"Models/Cajonera/cajonera.obj");
	Model Cajonera_S((char*)"Models/Cajonera/Cajon_Sup.obj");
	Model Cajonera_M((char*)"Models/Cajonera/Cajon_Med.obj");
	Model Cajonera_I((char*)"Models/Cajonera/Cajon_Inf.obj");

	Model Lampara((char*)"Models/Lamp/Lamp.obj");
	Model foco((char*)"Models/Lamp/foco.obj");

	Model Mamila((char*)"Models/Accesorios/mamila.obj");
	Model Sonaja((char*)"Models/Accesorios/Sonaja.obj");

	Model Sofa((char*)"Models/Sofa/sofa.obj");
	Model reposador((char*)"Models/Sofa/reposador.obj");

	Model C_lamp((char*)"Models/Ceiling_Lamp/Base.obj");
	Model C_foco((char*)"Models/Ceiling_Lamp/foco.obj");

	Model Cuna((char*)"Models/Cuna/Base_Cuna.obj");
	Model Cuna_G1((char*)"Models/Cuna/Giratorio1_cuna.obj");
	Model Cuna_G2((char*)"Models/Cuna/Giratorio2_cuna.obj");

	Model B_lamp((char*)"Models/Base_Lamp/lampara.obj");
	Model B_foco((char*)"Models/Base_Lamp/foco.obj");

	Model comoda((char*)"Models/comoda/comoda.obj");

	Model Baby_pen((char*)"Models/baby_pen/babypen.obj");

	Model table((char*)"Models/table/table.obj");
	Model chair((char*)"Models/chair/chair.obj");
	Model phone((char*)"Models/Phone/Phone.obj");

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

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -20.0f, 30.0f, -20.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.1f, 0.1f, 0.1f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), Light1.x, Light1.y, Light1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), Light1.x, Light1.y, Light1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.22f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.20f);


		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), Light2.x, Light2.y, Light2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), Light2.x, Light2.y, Light2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.22f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.20f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), Light3.x, Light3.y, Light3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), Light3.x, Light3.y, Light3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.22f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.20f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, 13.2f, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), Light4.x, Light4.y, Light4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), Light4.x, Light4.y, Light4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 1.8f);

		// Point light 5
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), Light5.x, Light5.y, Light5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), Light5.x, Light5.y, Light5.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 1.2f);

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 22.0f); //Brillo del objeto

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);
		glBindVertexArray(0);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		House.Draw(lightingShader);
		glBindVertexArray(0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.70);
		Windows.Draw(lightingShader);
		glDisable(GL_BLEND);  
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
		glBindVertexArray(0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Saguan.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.4f, 8.5f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.1f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cortinas.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.5f, 8.5f, -11.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.995f, 1.2f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cortinas.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.2f, 9.22f, -8.9));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 1.6f, 1.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bed.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 10.7f, -2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Chest_B.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(11.0f, 10.7f, -2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Chest_S.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.2f, rise, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, scale, 1.5f));
		model = glm::rotate(model, glm::radians(rotP), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pelota.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.9f, 9.1f, -3.0f));
		model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Reptar.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.8f, 9.15f, 4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cajonera.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(avanzo, 9.15f, 4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cajonera_S.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(avanzo, 9.15f, 4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cajonera_M.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(avanzo, 9.15f, 4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cajonera_I.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.8f, 11.75f, 3.2f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, 0.23f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.8f, 11.75f, 5.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mamila.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.2f, 9.15f, 2.0f));
		model = glm::rotate(model, glm::radians(-110.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sonaja.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.2f, 0.4f, -1.6f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.35f, 2.35f, 2.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sofa.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-7.386f, 0.42f, -0.6f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.15f, 1.15f, 1.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		reposador.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.5f,15.6f,-2.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		C_lamp.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.9f, 6.75f, -4.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		C_lamp.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.9f, 6.75f, 6.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		C_lamp.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.25f, 7.35f, -8.15f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.2f, 2.0f, 1.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cuna.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.25f, 7.35f, -8.15f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.2f, 2.0f, 1.8f));
		model = glm::rotate(model, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cuna_G1.Draw(lightingShader);
		glBindVertexArray(0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.25f, 7.35f, -8.15f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.2f, 2.0f, 1.8f));
		model = glm::rotate(model, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 5.0);
		Cuna_G2.Draw(lightingShader);
		glDisable(GL_BLEND);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
		glBindVertexArray(0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.8f, 0.5f, -11.15f));
		model = glm::rotate(model, glm::radians(-130.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.3f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_lamp.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.845f, 0.715f, -1.6f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.25f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		comoda.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-7.0f, 0.50f, -9.25f));
		model = glm::scale(model, glm::vec3(1.0f, 0.8f, 0.85f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Baby_pen.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.05f, 0.5f, 10.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		table.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-11.0f, 0.5f, 10.5f));
		model = glm::scale(model, glm::vec3(0.65f, 0.78f, 0.75f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		chair.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.5f, 2.95f, 10.0f));
		model = glm::scale(model, glm::vec3(0.95f, 0.95f, 0.95f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		phone.Draw(lightingShader);
		glBindVertexArray(0);


		//For the light bulb in the lamp object tommy's room
		lampShader.Use();

		// Get location objects for the matrices on the lamp shader
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.23f)); // Make it a smaller light bulb
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[3]);
		model = glm::scale(model, glm::vec3(0.23f)); // Make it a smaller light bulb
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		foco.Draw(lampShader);
		glBindVertexArray(0);

		//For the ceiling lamp 1 in the lamp object tommy's room
		lampShader.Use();

		// Get location objects for the matrices on the lamp shader
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[2]);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		C_foco.Draw(lampShader);
		glBindVertexArray(0);

		//For the ceiling lamp 2 in the lamp object living room
		lampShader.Use();

		// Get location objects for the matrices on the lamp shader
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[1]);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		C_foco.Draw(lampShader);
		glBindVertexArray(0);

		//For the ceiling lamp 3 in the lamp object living room
		lampShader.Use();

		// Get location objects for the matrices on the lamp shader
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		C_foco.Draw(lampShader);
		glBindVertexArray(0);

		//For the bulb lamp in the lamp object at the living room
		lampShader.Use();

		// Get location objects for the matrices on the lamp shader
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[4]);
		model = glm::rotate(model, glm::radians(-130.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.3f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_foco.Draw(lampShader);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
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

	if (anim and close == true and open == false) //Primer momento esta cerrada
	{
		if (rot < 46) {
			rot += 1.0f;
			if (rot == 45) {
				open = true;
				close = false;
				anim = false;
			}
		}
	}

	if (anim and open == true and close == false) { //Segundo momento esta cerrada

		if (rot > -1) {
			rot -= 1.0f;
			if (rot == 0) {
				close = true;
				open = false;
				anim = false;
			}
		}
	}

	if (anim_dos and close2 == true and open2 == false)//Implica cerrado
	{
		if (avanzo < 10.9f)
		{
			avanzo -= 0.1f;

			if (avanzo == 9.699996f) {
				open2 = true;
				close2 = false;
				anim_dos = false;
			}
		}

	}

	if (anim_dos and open2 == true and close2 == false)//Implica abierto
	{
		if (avanzo > 9.699995f)
		{
			avanzo += 0.1f;
			if (avanzo == 10.8f) {
				open2 = false;
				close2 = true;
				anim_dos = false;
			}
		}

	}

	if (anim_tres == true) {
		giro += 0.5f;
		if (giro == 360.5f) {
			anim_tres = false;
			giro = 0;
		}
	}

	if (anim_P == true) {
		if (i < 380) {
			rise = PosY[i];
			scale = Esc[i];
			//printf("R: %f\n Es:%f\n", rise, scale);
			if (i == 379) {
				i = 0;
				anim_P = false;
				rise = 9.1f;
				scale = 1.5f;
			}
			
		}
		i++;
		if (rotP < 381) {
			printf("%f\n", rotP);
			rotP++;
		}
		if (rotP == 380) {
			rotP = 0;
		}
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

	if (keys[GLFW_KEY_1])
	{
		anim = true;
	}

	if (keys[GLFW_KEY_2])
	{
		anim_dos = true;
	}

	if (keys[GLFW_KEY_3])
	{
		anim_tres = true;
		PlaySound(TEXT("Models/Accesorios/Rugrats.wav"), NULL, SND_ASYNC);
	}

	if (keys[GLFW_KEY_4]) {
		anim_P = true;
	}

	if (keys[GLFW_KEY_6])
	{
		active = !active;
		if (active)
		{
			Light5 = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			Light5 = glm::vec3(0);
		}
	}

	if (keys[GLFW_KEY_7])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			Light1 = glm::vec3(0);
		}
	}

	if (keys[GLFW_KEY_8]) {
		active = !active;
		if (active)
		{
			Light2 = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			Light2 = glm::vec3(0);

		}
	}

	if (keys[GLFW_KEY_9]) {
		active = !active;
		if (active)
		{
			Light3 = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			Light3 = glm::vec3(0);

		}
	}

	if (keys[GLFW_KEY_0]) {
		active = !active;
		if (active)
		{
			Light4 = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			Light4 = glm::vec3(0);

		}
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