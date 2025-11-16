// ProyectoF.cpp
// Proyecto Final - Casa de Billy y Mandy
// Laboratorio de Computación Gráfica e interaccion humano-computadora
//Alumno:Josué Vázquez Cárdenas 

//Librerias utilizadas
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// SOIL2 for texture loading
#include "SOIL2/SOIL2.h"

// Custom Headers
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "modelAnim.h"


// Function prototypes
//Registra movimientos en el teclado
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
//Registra movimiento en el mouse
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
//Detecta el movimiento en teclas presionadas 
void DoMovement();
//Funcion para analisis de las animaciones
void Animation();
//Dibuja las primitivas del cubo
void ObjetoDraw(glm::mat4 base, glm::vec3 escala, glm::vec3 traslado, GLint uniformModel);

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 900;
int SCREEN_WIDTH, SCREEN_HEIGHT;


// Camera
//Posicion inicial de la camara
Camera camera(glm::vec3(-5.0f, 5.0f, 37.0f));
GLfloat lastX = WIDTH / 2.0f;//Posicion del mouse en X
GLfloat lastY = HEIGHT / 2.0f;//Posicion del mouse en Y
bool keys[1024];//Arreglo para registro de teclas
bool firstMouse = true; //Evitar salto de camara al entrar a la venta

// Light attributes
bool active; // For debug light
glm::vec3 Light1 = glm::vec3(0); // For debug light color animation
glm::vec3 PosIni(-0.0f, 0.2f, -0.0f);

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 5.0f, 0.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 0.0f, 0.0f)};

// Cube vertices (Pos, Normal, TexCoords)
float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

// --- Variables de textura ---
GLuint sofaTexture;//Textura del sofa
GLuint lampBaseTexture;
GLuint lampShadeTexture;//Textura de lampara
GLuint tvCasingTexture;//Textura de la tv
GLuint tvScreenTexture;//Textura de la tv apagada
GLuint inodoroTexture;//Textura del inodoro
GLuint plantaTexture;//Textura de la planta interior
GLuint arbustoTexture;//Textura del arbusto
GLuint tvStaticTexture;//Textura del portal

//Variables para Animaciones Simples 
//Variables de puertas
float lampWobble = 0.0f;
bool lampAnimationActive = true;
bool lampLightActive = true; // Interruptor para la luz

// Variables de animacion de puertas
float doorE_angle = 0.0f;
float doorS_angle = 0.0f;
float doorB_angle = 0.0f;

bool doorE_open = false;
bool doorS_open = false;
bool doorB_open = false;

//Variables Animacion compleja Tv
bool tvAnimationActive = false; // Inicia apagada
float tvAnimTime = 0.0f;      // Temporizador para la animacion

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();
	//Creacion de la ventana de visualizacion
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final - Casa Billy y Mandy", nullptr, nullptr);
	
	//Prueba de error, se genera en casa de que la pantalla no se muestre
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	//Especificaciones de pantalla
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	//Registros de CALLBACKS, para teclado y mouse
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	//Inicializacion de GLEW
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Shaders
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");//Shader para la iluminacion Phong
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");//Shader para objetos simples
	Shader modelShader("Shader/modelLoading.vs", "Shader/modelLoading.frag");//Shader para la carga de modelos 
	Shader animShader("Shader/anim.vs", "Shader/anim.frag");//Shader para la animacion de Billy

	// Carga de Modelos
	Model casa((char*)"Models/Casa/Casa.obj");//Modelo de la casa 
	//Modelo de las puertas
	Model PuertaE((char*)"Models/Puertas/PuertaE.obj");
	Model PuertaS((char*)"Models/Puertas/PuertaS.obj");
	Model PuertaB((char*)"Models/Puertas/PuertaB.obj");

	Model Fantasma((char*)"Models/Espectro/ghost2.obj");//Modelo del espectro
	ModelAnim animacionPersonaje("Animaciones/Billy/movimiento.dae");//Modelo de la animacion del personaje
	animacionPersonaje.initShaders(animShader.Program);//Animacion del personaje

	// VAO y VBO para los cubos (primitivas)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO); //Creacion del (VAO) para la configuracion
	glGenBuffers(1, &VBO);//Creacion del (VBO) para almacenar los datos de los vertices
	glBindVertexArray(VAO);//Activacion del VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//Activacion del VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Vertex Attribs
	//Atributos de posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Atributos de Normales
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//Atributos de texturas
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	// --- Carga de Texturas ---
	int width, height;//Dimensiones de la textura
	unsigned char* image;//Puntero a los datos de pixeles

	// Textura Arbusto
	glGenTextures(1, &arbustoTexture);//variable para la textura
	glBindTexture(GL_TEXTURE_2D, arbustoTexture);//Inicializacion de la textura para su configuracion
	//Configuracion de los parametros de texturizado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Carga de la imagen arbusto.jpg
	image = SOIL_load_image("images/arbusto.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);//Calculo de niveles del mipmap
	}
	glBindTexture(GL_TEXTURE_2D, 0);//Desvincula la textura actual

	// Textura Sofa
	glGenTextures(1, &sofaTexture);//variable para la textura
	glBindTexture(GL_TEXTURE_2D, sofaTexture);//Inicializacion de la textura para su configuracion
	//Configuracion de los parametros de texturizado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Carga de la imagen sofa.jpg
	image = SOIL_load_image("images/sofa.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);//Calculo de niveles del mipmap
	}
	glBindTexture(GL_TEXTURE_2D, 0);//Desvincula la textura actual

	// Texturas Lampara (Base)
	glGenTextures(1, &lampBaseTexture);//variable para la textura
	glBindTexture(GL_TEXTURE_2D, lampBaseTexture);//Inicializacion de la textura para su configuracion
	//Configuracion de los parametros de texturizado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Carga de la imagen lampara.jpg
	image = SOIL_load_image("images/lampara.JPG", &width, &height, 0, SOIL_LOAD_RGB);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);//Calculo de niveles del mipmap
	}
	glBindTexture(GL_TEXTURE_2D, 0);//Desvincula la textura actual

	// Texturas Lampara (Pantalla)
	glGenTextures(1, &lampShadeTexture);//variable para la textura
	glBindTexture(GL_TEXTURE_2D, lampShadeTexture);//Inicializacion de la textura para su configuracion
	//Configuracion de los parametros de texturizado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Carga de la imagen Tela.jpg
	image = SOIL_load_image("images/Tela.JPG", &width, &height, 0, SOIL_LOAD_RGB);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);//Calculo de niveles del mipmap
	}
	glBindTexture(GL_TEXTURE_2D, 0);//Desvincula la textura actual

	// Texturas TV (Carcasa)
	glGenTextures(1, &tvCasingTexture);//variable para la textura
	glBindTexture(GL_TEXTURE_2D, tvCasingTexture);//Inicializacion de la textura para su configuracion
	//Configuracion de los parametros de texturizado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Carga de la imagen cuerpo.png
	image = SOIL_load_image("images/cuerpo.PNG", &width, &height, 0, SOIL_LOAD_RGB);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);//Calculo de niveles del mipmap
	}
	glBindTexture(GL_TEXTURE_2D, 0);//Desvincula la textura actual

	// Texturas TV (Pantalla)
	glGenTextures(1, &tvScreenTexture);//variable para la textura
	glBindTexture(GL_TEXTURE_2D, tvScreenTexture);//Inicializacion de la textura para su configuracion
	//Configuracion de los parametros de texturizado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Carga de la imagen pantalla.png
	image = SOIL_load_image("images/pantalla.PNG", &width, &height, 0, SOIL_LOAD_RGB);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);//Calculo de niveles del mipmap
	}
	glBindTexture(GL_TEXTURE_2D, 0);//Desvincula la textura actual

	// Textura TV portal
	glGenTextures(1, &tvStaticTexture);//variable para la textura
	glBindTexture(GL_TEXTURE_2D, tvStaticTexture);//Inicializacion de la textura para su configuracion
	//Configuracion de los parametros de texturizado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Carga de la imagen portal.jpg
	image = SOIL_load_image("images/portal.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);//Calculo de niveles del mipmap
	}
	glBindTexture(GL_TEXTURE_2D, 0);//Desvincula la textura actual

	// Textura Inodoro Bañera
	glGenTextures(1, &inodoroTexture);//variable para la textura
	glBindTexture(GL_TEXTURE_2D, inodoroTexture);//Inicializacion de la textura para su configuracion
	//Configuracion de los parametros de texturizado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carga de la imagen baño.jpg
	image = SOIL_load_image("images/baño.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);//Calculo de niveles del mipmap
	}
	glBindTexture(GL_TEXTURE_2D, 0);//Desvincula la textura actual

	// Textura Planta
	glGenTextures(1, &plantaTexture);//variable para la textura
	glBindTexture(GL_TEXTURE_2D, plantaTexture);//Inicializacion de la textura para su configuracion
	//Configuracion de los parametros de texturizado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Carga de la imagen planta.jpg
	image = SOIL_load_image("images/planta.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);//Calculo de niveles del mipmap
	}
	glBindTexture(GL_TEXTURE_2D, 0);//Desvincula la textura actual
	

	// Matriz de Proyección
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		//Gestion de tiempo y entrada
		// Deltatime, PollEvents, DoMovement
		GLfloat currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();//Procesa eventos de ventana para cerrar y pulsaciones en el teclado
		DoMovement();//Procesa entradas de movimiento constante

		Animation(); // Actualiza las animaciones

		// Clear buffers, Enable Depth Test
		glClearColor(0.5f, 0.8f, 1.0f, 1.0f);//Color del fondo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Limpieza y profundidad del buffer
		glEnable(GL_DEPTH_TEST);//Profundidad en objetos 3D

		// --- MATRICES COMUNES ---
		glm::mat4 view = camera.GetViewMatrix();//Matriz de vista
		glm::mat4 model = glm::mat4(1.0f);//Matriz de modelo

		// Dibujo de la casa
		modelShader.Use();
		//Matrices para la camara y proyeccion al shader
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//Renderizado de la casa
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		casa.Draw(modelShader);//Dibuja el modelo  de la casa

		//// Dibujo de cada una de las puertas animadas
		// === Puerta Entrada
		glm::mat4 modelDoorE = glm::mat4(1.0f);
		glm::vec3 hingePosE = glm::vec3(6.0f, 0.0f, -4.8f);//Posicion de la bisagra
		//Transformaciones en la puerta entrada
		modelDoorE = glm::translate(modelDoorE, -hingePosE);
		modelDoorE = glm::rotate(modelDoorE, glm::radians(doorE_angle), glm::vec3(0.0f, 0.5f, 0.0f));
		modelDoorE = glm::translate(modelDoorE, hingePosE);
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDoorE));
		PuertaE.Draw(modelShader);//Dibuja el modelo  de la puerta entrada

		// === Puerta Sala
		glm::mat4 modelDoorS = glm::mat4(1.0f);
		glm::vec3 hingePosS = glm::vec3(5.3f, 0.0f, 2.35f);//Posicion de la bisagra
		//Transformaciones en la puerta Sala
		modelDoorS = glm::translate(modelDoorS, -hingePosS);
		modelDoorS = glm::rotate(modelDoorS, glm::radians(doorS_angle), glm::vec3(0.0f, 0.5f, 0.0f));
		modelDoorS = glm::translate(modelDoorS, hingePosS);
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDoorS));
		PuertaS.Draw(modelShader);//Dibuja el modelo  de la puerta Sala

		// === Puerta Baño 
		glm::mat4 modelDoorB = glm::mat4(1.0f);
		glm::vec3 hingePosB = glm::vec3(3.3f, 0.0f, 5.25f);//Posicion de la bisagra
		//Transformaciones en la puerta Baño
		modelDoorB = glm::translate(modelDoorB, -hingePosB);
		modelDoorB = glm::rotate(modelDoorB, glm::radians(doorB_angle), glm::vec3(0.0f, 0.5f, 0.0f));
		modelDoorB = glm::translate(modelDoorB, hingePosB);
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDoorB));
		PuertaB.Draw(modelShader);//Dibuja el modelo  de la puerta Baño

		// DIBUJAR OBJETOS CON LUZ (lightingShader)
		lightingShader.Use();
		//Optimizacion para la ubicacion de cada frame
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

		//Envio de Uniforms "Globales"
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//Envio de posicion de la camara al shader
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		//Declaracion del brillo a utilizar
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);
		//Ubicacion para la repeticion de texturas
		GLint tilingLoc = glGetUniformLocation(lightingShader.Program, "texTiling");

		// --- Configuracion de Luces para lightingShader
		// Luz Direccional (Sol)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);//Luz ambiental
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);//Color de la luz
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);//Color del brillo

		// Luz de Punto 0 (Luz de debug)
		//Calcula un color dinamico para un efecto de parpadeo
		glm::vec3 lightColor;
		lightColor.x = abs(sin((float)glfwGetTime() * Light1.x));
		lightColor.y = abs(sin((float)glfwGetTime() * Light1.y));
		lightColor.z = sin((float)glfwGetTime() * Light1.z);
		//Envia propiedades de la luz al punto del shader
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x * 0.5f, lightColor.y * 0.5f, lightColor.z * 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		//Parametros de atenuacion constante, lineal y cuadratica
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		//-------------------Dibujo de objetos---------------------

		//DIBUJAR LOS ARBUSTOS-- -

		glBindVertexArray(VAO);//Activa el VAO del cubo
		glActiveTexture(GL_TEXTURE0);//Activa su textura en 0
		glBindTexture(GL_TEXTURE_2D, arbustoTexture);//Vincula a su textura
		glUniform2f(tilingLoc, 1.0f, 1.0f); // Asegura que la textura no se repita

		glm::mat4 modelArbustoBase = glm::mat4(1.0f);
		// Arbusto 1
		ObjetoDraw(modelArbustoBase, glm::vec3(1.5f, 0.8f, 1.5f), glm::vec3(0.0f, 0.6f, 6.2f), modelLoc);//Parte central
		ObjetoDraw(modelArbustoBase, glm::vec3(1.0f, 0.2f, 1.0f), glm::vec3(0.0f, 1.1f, 6.2f), modelLoc);//Parte superior
		ObjetoDraw(modelArbustoBase, glm::vec3(1.0f, 0.2f, 1.0f), glm::vec3(0.0f, 0.1f, 6.2f), modelLoc);//Parte inferior
		// Arbusto 2
		ObjetoDraw(modelArbustoBase, glm::vec3(0.7f, 0.5f, 0.7f), glm::vec3(3.0f, 0.25f, 6.2f), modelLoc);//Parte central
		ObjetoDraw(modelArbustoBase, glm::vec3(0.8f, 0.6f, 0.8f), glm::vec3(3.0f, 0.8f, 6.2f), modelLoc);//Parte superior
		ObjetoDraw(modelArbustoBase, glm::vec3(1.2f, 0.8f, 1.2f), glm::vec3(3.0f, 0.5f, 6.2f), modelLoc);//Parte inferior
		// Arbusto 3
		ObjetoDraw(modelArbustoBase, glm::vec3(1.7f, 0.8f, 1.7f), glm::vec3(-3.0f, 0.6f, 6.2f), modelLoc);//Parte central
		ObjetoDraw(modelArbustoBase, glm::vec3(1.2f, 0.2f, 1.2f), glm::vec3(-3.0f, 1.1f, 6.2f), modelLoc);//Parte superior
		ObjetoDraw(modelArbustoBase, glm::vec3(1.2f, 0.2f, 1.2f), glm::vec3(-3.0f, 0.1f, 6.2f), modelLoc);//Parte inferior
		// Arbusto 4
		ObjetoDraw(modelArbustoBase, glm::vec3(1.7f, 0.8f, 1.7f), glm::vec3(-8.0f, 0.6f, 6.2f), modelLoc);//Parte central
		ObjetoDraw(modelArbustoBase, glm::vec3(1.2f, 0.2f, 1.2f), glm::vec3(-8.0f, 1.1f, 6.2f), modelLoc);//Parte superior
		ObjetoDraw(modelArbustoBase, glm::vec3(1.2f, 0.2f, 1.2f), glm::vec3(-8.0f, 0.1f, 6.2f), modelLoc);//Parte inferior
		// Arbusto 5
		ObjetoDraw(modelArbustoBase, glm::vec3(1.7f, 0.8f, 1.7f), glm::vec3(-12.0f, 0.6f, 6.2f), modelLoc);//Parte central
		ObjetoDraw(modelArbustoBase, glm::vec3(1.2f, 0.2f, 1.2f), glm::vec3(-12.0f, 1.1f, 6.2f), modelLoc);//Parte superior
		ObjetoDraw(modelArbustoBase, glm::vec3(1.2f, 0.2f, 1.2f), glm::vec3(-12.0f, 0.1f, 6.2f), modelLoc);//Parte inferior

		glBindTexture(GL_TEXTURE_2D, 0);// Desvincula la textura

		// Dibujo del sofá ---
		glBindVertexArray(VAO);//Activa el VAO del cubo
		glActiveTexture(GL_TEXTURE0);//Activa su textura en 0
		glBindTexture(GL_TEXTURE_2D, sofaTexture);//Vincula a su textura

		//Ubicacion del sofa
		glm::mat4 modelSofaBase = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, -0.9f));
		//Ajuste de la textura
		glUniform2f(tilingLoc, 6.0f, 2.0f);
		//Base del sofa
		ObjetoDraw(modelSofaBase, glm::vec3(4.0f, 0.8f, 1.5f), glm::vec3(0.0f, 0.4f, 0.0f), modelLoc);
		//Ajuste de la textura
		glUniform2f(tilingLoc, 6.0f, 2.0f);
		//Respaldo de sofa
		ObjetoDraw(modelSofaBase, glm::vec3(4.0f, 1.8f, 0.5f), glm::vec3(0.0f, 0.9f, -1.0f), modelLoc);
		//Ajuste de la textura
		glUniform2f(tilingLoc, 1.35f, 1.1f);
		//Reposa brazos derecho
		ObjetoDraw(modelSofaBase, glm::vec3(1.35f, 1.1f, 0.3f), glm::vec3(0.83f, 1.4f, -0.59f), modelLoc);
		//Reposa brazos izquierdo
		ObjetoDraw(modelSofaBase, glm::vec3(1.35f, 1.1f, 0.3f), glm::vec3(-0.83f, 1.4f, -0.59f), modelLoc);
		//Cojin izquierdo
		ObjetoDraw(modelSofaBase, glm::vec3(0.5f, 0.6f, 1.5f), glm::vec3(-1.75f, 1.1f, 0.0f), modelLoc);
		//Cojin derecho
		ObjetoDraw(modelSofaBase, glm::vec3(0.5f, 0.6f, 1.5f), glm::vec3(1.75f, 1.1f, 0.0f), modelLoc);
		glBindTexture(GL_TEXTURE_2D, 0);// Desvincula la textura

		// Dibujo del inodoro
		glBindVertexArray(VAO);//Activa el VAO del cubo
		glActiveTexture(GL_TEXTURE0);//Activa su textura en 0
		glBindTexture(GL_TEXTURE_2D, inodoroTexture); // Usamos la textura del baño
		glUniform2f(tilingLoc, 1.0f, 1.0f); // Tiling 1x1

		// --- Matriz Base del Inodoro ---
		glm::mat4 modelInodoroBase = glm::translate(glm::mat4(1.0f), glm::vec3(2.8f, 0.0f, -7.25f));
		// Tanque
		ObjetoDraw(modelInodoroBase, glm::vec3(0.5f, 0.9f, 1.2f), glm::vec3(0.0f, 1.2f, 0.4f), modelLoc);
		// Taza
		ObjetoDraw(modelInodoroBase, glm::vec3(1.5f, 0.2f, 1.0f), glm::vec3(-0.4f, 0.10f, 0.4f), modelLoc);
		ObjetoDraw(modelInodoroBase, glm::vec3(0.5f, 0.1f, 0.4f), glm::vec3(-0.7f, 0.30f, 0.4f), modelLoc);
		ObjetoDraw(modelInodoroBase, glm::vec3(0.6f, 0.1f, 0.5f), glm::vec3(-0.7f, 0.40f, 0.4f), modelLoc);
		ObjetoDraw(modelInodoroBase, glm::vec3(0.7f, 0.1f, 0.6f), glm::vec3(-0.7f, 0.50f, 0.4f), modelLoc);
		ObjetoDraw(modelInodoroBase, glm::vec3(0.8f, 0.1f, 0.7f), glm::vec3(-0.7f, 0.60f, 0.4f), modelLoc);
		ObjetoDraw(modelInodoroBase, glm::vec3(0.9f, 0.1f, 0.8f), glm::vec3(-0.7f, 0.70f, 0.4f), modelLoc);
		ObjetoDraw(modelInodoroBase, glm::vec3(1.0f, 0.1f, 0.9f), glm::vec3(-0.7f, 0.80f, 0.4f), modelLoc);

		// --- DIBUJAR LA BAÑERA ---
		//Ubicacion de la bañera
		glm::mat4 modelBaneraBase = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -4.0f));
		// Parte superior de la bañera
		ObjetoDraw(modelBaneraBase, glm::vec3(1.6f, 0.2f, 2.6f), glm::vec3(0.0f, 1.2f, 0.0f), modelLoc);
		//Forro de la bañera
		ObjetoDraw(modelBaneraBase, glm::vec3(1.4f, 0.2f, 2.3f), glm::vec3(0.0f, 1.0f, 0.0f), modelLoc);
		ObjetoDraw(modelBaneraBase, glm::vec3(1.4f, 0.2f, 2.2f), glm::vec3(0.0f, 0.8f, 0.0f), modelLoc);
		ObjetoDraw(modelBaneraBase, glm::vec3(1.4f, 0.2f, 2.1f), glm::vec3(0.0f, 0.6f, 0.0f), modelLoc);
		ObjetoDraw(modelBaneraBase, glm::vec3(1.4f, 0.2f, 2.0f), glm::vec3(0.0f, 0.4f, 0.0f), modelLoc);
		//Patas
		ObjetoDraw(modelBaneraBase, glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(0.50f, 0.2f, 0.9f), modelLoc);
		ObjetoDraw(modelBaneraBase, glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(0.5f, 0.2f, -0.9f), modelLoc);
		ObjetoDraw(modelBaneraBase, glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(-0.5f, 0.2f, -0.9f), modelLoc);
		ObjetoDraw(modelBaneraBase, glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(-0.5f, 0.2f, 0.9f), modelLoc);

		glBindTexture(GL_TEXTURE_2D, 0);// Desvincula la textura

		// Dibujar la planta
		glBindVertexArray(VAO);//Activa el VAO del cubo
		glActiveTexture(GL_TEXTURE0);//Activa su textura en 0
		glBindTexture(GL_TEXTURE_2D, plantaTexture);// Usamos la textura del baño
		glUniform2f(tilingLoc, 1.0f, 1.0f); // Tiling 1x1

		//Ubicacion de la planta
		glm::mat4 modelPlantaBase = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 2.0f, -2.75f));

		// Repisa
		ObjetoDraw(modelPlantaBase, glm::vec3(1.2f, 0.1f, 0.4f), glm::vec3(0.0f, 0.05f, 0.0f), modelLoc);
		//Maceta
		ObjetoDraw(modelPlantaBase, glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.0f, 0.3f, 0.0f), modelLoc);
		// Tallo
		ObjetoDraw(modelPlantaBase, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(0.0f, 0.7f, 0.0f), modelLoc);
		// Hoja 1
		glm::mat4 modelHoja1 = modelPlantaBase;
		//Ubicacion de la hoja
		modelHoja1 = glm::translate(modelHoja1, glm::vec3(0.150f, 0.9f, 0.0f));
		//Rotacion del cubo
		modelHoja1 = glm::rotate(modelHoja1, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ObjetoDraw(modelHoja1, glm::vec3(0.5f, 0.05f, 0.3f), glm::vec3(0.0f, 0.0f, 0.0f), modelLoc);
		// Hoja 2
		glm::mat4 modelHoja2 = modelPlantaBase;
		//Ubicacion de la hoja
		modelHoja2 = glm::translate(modelHoja2, glm::vec3(-0.150f, 0.9f, 0.0f));
		//Rotacion del cubo
		modelHoja2 = glm::rotate(modelHoja2, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ObjetoDraw(modelHoja2, glm::vec3(0.5f, 0.05f, 0.3f), glm::vec3(0.0f, 0.0f, 0.0f), modelLoc);
		// Hoja 3
		glm::mat4 modelHoja3 = modelPlantaBase;
		//Ubicacion de la hoja
		modelHoja3 = glm::translate(modelHoja3, glm::vec3(0.0f, 0.8f, 0.0f));
		//Rotacion del cubo
		modelHoja3 = glm::rotate(modelHoja3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ObjetoDraw(modelHoja3, glm::vec3(0.5f, 0.05f, 0.3f), glm::vec3(0.0f, 0.0f, 0.0f), modelLoc);

		glBindTexture(GL_TEXTURE_2D, 0);// Desvincula la textura


		// Calcula la Matriz de Lámpara Tambaleante
		//Posicion de la lampara
		glm::vec3 lampBaseWorldPos = glm::vec3(1.5f, 0.0f, -0.9f);
		//Creacion de la matriz de la lampara
		glm::mat4 modelLampBase = glm::translate(glm::mat4(1.0f), lampBaseWorldPos);
		//Aplica la rotacion de la lampara generando una especie de tambaleo
		modelLampBase = glm::rotate(modelLampBase, glm::radians(lampWobble), glm::vec3(0.0f, 0.0f, 1.0f));

		// Configuracion de la Luz de la Lámpara (pointLights[1])
		// Se coloca la posicion de la bombilla a la base de la lampara
		glm::vec3 lampBulbLocalPos = glm::vec3(0.0f, 2.8f, 0.0f);
		//Posicion final de la luz aplicada en el mundo 
		glm::vec3 finalLightPos = modelLampBase * glm::vec4(lampBulbLocalPos, 1.0f);

		//Envio de posicion al shader para realizar la animacion
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), finalLightPos.x, finalLightPos.y, finalLightPos.z);

		//interruptor para determinar si la lampara se encuentra apagada o encendida
		if (lampLightActive)
		{
			// Luz ENCENDIDA (Amarilla)
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.2f, 0.2f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		}
		else
		{
			// Luz APAGADA (Negro)
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		}
		// La atenuación se envía siempre
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Dibujo de la Lampara
		lightingShader.Use();
		glBindVertexArray(VAO);//Activa el VAO del cubo
		glActiveTexture(GL_TEXTURE0);//Activa su textura en 0
		glBindTexture(GL_TEXTURE_2D, lampBaseTexture);// Vincula la textura de la lampara
		glUniform2f(tilingLoc, 1.0f, 1.0f);

		ObjetoDraw(modelLampBase, glm::vec3(0.8f, 0.1f, 0.8f), glm::vec3(0.0f, 0.05f, 0.0f), modelLoc);//Base de la lampara
		ObjetoDraw(modelLampBase, glm::vec3(0.1f, 1.5f, 0.1f), glm::vec3(0.0f, 0.8f, 0.0f), modelLoc);//Primer palo de la lampara
		ObjetoDraw(modelLampBase, glm::vec3(1.0f, 0.1f, 1.0f), glm::vec3(0.0f, 1.6f, 0.0f), modelLoc);//plato de la lampara
		ObjetoDraw(modelLampBase, glm::vec3(0.5f, 0.05f, 0.5f), glm::vec3(0.0f, 1.65f, 0.0f), modelLoc);//Segundo plato de la lampara
		ObjetoDraw(modelLampBase, glm::vec3(0.1f, 1.0f, 0.1f), glm::vec3(0.0f, 2.1f, 0.0f), modelLoc);//Segundo palo de la lampara

		glBindTexture(GL_TEXTURE_2D, lampShadeTexture);// Vincula la textura de la lampara para la parte de la tela
		glUniform2f(tilingLoc, 1.0f, 1.0f);
		//Dibujo de la lampara
		ObjetoDraw(modelLampBase, glm::vec3(1.0f, 0.2f, 1.0f), glm::vec3(0.0f, 2.6f, 0.0f), modelLoc);
		ObjetoDraw(modelLampBase, glm::vec3(0.9f, 0.2f, 0.9f), glm::vec3(0.0f, 2.8f, 0.0f), modelLoc);
		ObjetoDraw(modelLampBase, glm::vec3(0.8f, 0.2f, 0.8f), glm::vec3(0.0f, 3.0f, 0.0f), modelLoc);
		ObjetoDraw(modelLampBase, glm::vec3(0.7f, 0.2f, 0.7f), glm::vec3(0.0f, 3.2f, 0.0f), modelLoc);
		ObjetoDraw(modelLampBase, glm::vec3(0.6f, 0.2f, 0.6f), glm::vec3(0.0f, 3.4f, 0.0f), modelLoc);
		glBindTexture(GL_TEXTURE_2D, 0);// Desvincula la textura


		// DIBUJAR LA TV Y ANIMACIÓN COMPLEJA
		glBindVertexArray(VAO);//Activa el VAO del cubo
		glActiveTexture(GL_TEXTURE0);//Activa su textura en 0

		// Dibujar la carcasa de la TV
		//Ubicacion 
		glm::mat4 modelTVBase = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.5f, 3.0f));
		//Rotacion 
		modelTVBase = glm::rotate(modelTVBase, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glBindTexture(GL_TEXTURE_2D, tvCasingTexture);// Usamos la textura del baño
		glUniform2f(tilingLoc, 2.0f, 1.5f);//Ajuste de dimensiones por textura
		//Cuerpo Tv
		ObjetoDraw(modelTVBase, glm::vec3(2.0f, 1.5f, 1.0f), glm::vec3(0.0f, 0.75f, 0.0f), modelLoc);
		ObjetoDraw(modelTVBase, glm::vec3(1.5f, 1.0f, 0.5f), glm::vec3(0.0f, 0.75f, -0.7f), modelLoc);
		glUniform2f(tilingLoc, 0.1f, 0.5f);//Ajuste de dimensiones por textura
		
		//Patas
		ObjetoDraw(modelTVBase, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(-0.9f, -0.25f, 0.4f), modelLoc);
		glUniform2f(tilingLoc, 0.1f, 0.5f);//Ajuste de dimensiones por textura
		ObjetoDraw(modelTVBase, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(0.9f, -0.25f, 0.4f), modelLoc);
		glUniform2f(tilingLoc, 0.1f, 0.5f);//Ajuste de dimensiones por textura
		ObjetoDraw(modelTVBase, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(-0.9f, -0.25f, -0.4f), modelLoc);
		glUniform2f(tilingLoc, 0.1f, 0.5f);//Ajuste de dimensiones por textura
		ObjetoDraw(modelTVBase, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(0.9f, -0.25f, -0.4f), modelLoc);
		glUniform2f(tilingLoc, 0.15f, 0.15f);//Ajuste de dimensiones por textura
		//Boton
		ObjetoDraw(modelTVBase, glm::vec3(0.15f, 0.15f, 0.1f), glm::vec3(0.9f, 0.15f, 0.55f), modelLoc);

		// Dibujar la pantalla de TV dinamica
		if (tvAnimationActive)
		{
			// Usa la textura del portal 
			glBindTexture(GL_TEXTURE_2D, tvStaticTexture);
		}
		else
		{
			// Usa la textura normal de pantalla apagada
			glBindTexture(GL_TEXTURE_2D, tvScreenTexture);
		}
		glUniform2f(tilingLoc, 1.0f, 1.0f);//Ajusta la textura
		ObjetoDraw(modelTVBase, glm::vec3(1.6f, 1.1f, 0.1f), glm::vec3(0.0f, 0.75f, 0.51f), modelLoc);
		glBindTexture(GL_TEXTURE_2D, 0);//Dedvincula la textura

		// Animacion compleja salida del espectro
		if (tvAnimationActive)
		{
			// Calcula la animacion no lineal 
			float emergeZ = 0.4f + (tvAnimTime * 0.5f);//Movimiento lineal el espectro avanza en Z
			float wiggleX = sin(tvAnimTime * 8.0f) * 0.3f;//Movimiento sinusoidal, el espectro se tambalea en el eje de X

			// Crear la matriz base del espectro
			glm::mat4 modelFantasma = modelTVBase; // Parte de la TV
			modelFantasma = glm::translate(modelFantasma, glm::vec3(wiggleX, 0.85f, emergeZ)); // Aplica animacion
			modelFantasma = glm::rotate(modelFantasma, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));//Rotacion, para ajustar la posicion del fantasma
			modelFantasma = glm::scale(modelFantasma, glm::vec3(0.28f));//Tamaño del espectro

			// Dibujar el modelo .obj 
			GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
			//Se le aplica la iluminacion de la escena al fantasma
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFantasma));

			// Reseteo del Tiling a 1x1 para el fantasma
			GLint tilingLoc = glGetUniformLocation(lightingShader.Program, "texTiling");
			glUniform2f(tilingLoc, 1.0f, 1.0f);

			// Dibujo del fantasma con el lightingShader
			Fantasma.Draw(lightingShader);
		}
		
		//------Animacion compleja Billy caminando
		//Activacion del shader para la animacion
		animShader.Use();

		//Se obtienen las ubicaiones uniform para el shader
		modelLoc = glGetUniformLocation(animShader.Program, "model");
		viewLoc = glGetUniformLocation(animShader.Program, "view");
		projLoc = glGetUniformLocation(animShader.Program, "projection");

		//Se envían las matrices de camara y proyeccion 
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Configuracion de la matriz para el modelo de Billy
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 15.0f));//Ajuste de ubicacion
		model = glm::scale(model, glm::vec3(1.8f)); // Escala del personaje
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//Dibuja el personaje animado
		animacionPersonaje.Draw(animShader);
		glBindVertexArray(0);//Desvincula el VAO

		//// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW
	glfwTerminate();
	return 0;//Finaliza el programa con exito
}

// DoMovement
void DoMovement()
{
	// Controles de Cámara
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])//Movimiento hacia adelante
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])//Movimiento hacia atras
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])//Movimiento hacia la izquierda
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])//Movimiento hacia la derecha
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

}
//Funcion para control de las animaciones 
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Controles de Puertas
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_1)
			doorE_open = !doorE_open; // apertura de la puerta de la entrada

		if (key == GLFW_KEY_2)
			doorS_open = !doorS_open; // apertura de la puerta de la sala

		if (key == GLFW_KEY_3)
			doorB_open = !doorB_open; // apertura de la puerta del baño
	}

	// Control de Animación de Lámpara
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		lampAnimationActive = !lampAnimationActive; // Movimiento de la lampara
	}

	// Control de Luz de Lámpara
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		lampLightActive = !lampLightActive; // Interruptor de la LUZ de la lampara
	}

	// Control Animacion Compleja TV ---
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (!tvAnimationActive)//Solo inicia mientras la tv no este activa
		{
			tvAnimationActive = true;//Activa la bandera
			tvAnimTime = 0.0f; // Reinicia el tiempo de la animación
		}
	}

	// Cerrar ventana
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);//Señal de cierre de ventana
	}

	// Actualizar array de teclas
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;//Marca la tecla como presionada
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;//Marca la tecla como soltada
		}
	}
	// Control de Luz de Debug
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
		}
		else
		{
			Light1 = glm::vec3(0);
		}
	}
}

void Animation() {

	// Animación de Puertas
	float doorSpeed = 1.5f; // velocidad en grados por frame

	// Puerta Entrada
	if (doorE_open && doorE_angle < 90.0f)
		doorE_angle += doorSpeed;//Abrir
	else if (!doorE_open && doorE_angle > 0.0f)
		doorE_angle -= doorSpeed;//Cerrar

	// Puerta Salida
	if (doorS_open && doorS_angle < 90.0f)
		doorS_angle += doorSpeed;//Abrir
	else if (!doorS_open && doorS_angle > 0.0f)
		doorS_angle -= doorSpeed;//Cerrar

	// Puerta Baño
	if (doorB_open && doorB_angle < 90.0f)
		doorB_angle += doorSpeed;//Abrir
	else if (!doorB_open && doorB_angle > 0.0f)
		doorB_angle -= doorSpeed;//Cerrar

	// Animación Simple de Lámpara
	if (lampAnimationActive)
	{
		lampWobble = sin((float)glfwGetTime() * 2.0f) * 5.0f;//Angulo de tambaleo de la lampara
	}
	else
	{
		lampWobble = 0.0f;//En caso de estar activa resetea 
	}

	// --- NUEVO: Actualización de Animación Compleja TV ---
	if (tvAnimationActive)
	{
		tvAnimTime += deltaTime; // Incrementa el temporizador

		// Detener la animación después de 5 segundos
		if (tvAnimTime > 5.0f)
		{
			tvAnimationActive = false;//Desactiva la bandera
			tvAnimTime = 0.0f; //Resetea el temporizador
		}
	}
}

// MouseCallback
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = (float)xPos;//Establece la posicion inicial en X
		lastY = (float)yPos;//Establece la posicion inicial en Y
		firstMouse = false;//Desactiva la bandera
	}
	GLfloat xOffset = (float)xPos - lastX;//Calculo del movimiento del mouse
	GLfloat yOffset = lastY - (float)yPos;//Se invierte el eje Y
	lastX = (float)xPos;//Almacena la posicion para el proximo fotograma
	lastY = (float)yPos;
	camera.ProcessMouseMovement(xOffset, yOffset);//Envia la posicion de la camara para calcular los nuevos angulos
}

// Función para dibujar primitivas de cubo
void ObjetoDraw(glm::mat4 base, glm::vec3 escala, glm::vec3 traslado, GLint uniformModel)
{
	glm::mat4 modelo = glm::mat4(1.0f);//Inicializa la matriz
	modelo = glm::translate(modelo, traslado);//Aplica la posicion de cada pieza
	modelo = glm::scale(modelo, escala);//Ajusta el tamaño de cada pieza
	modelo = base * modelo;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelo));//Envia la matriz al shader
	glDrawArrays(GL_TRIANGLES, 0, 36);//Dibuja el cubo con 36 vertices definidos en el VAO
}






