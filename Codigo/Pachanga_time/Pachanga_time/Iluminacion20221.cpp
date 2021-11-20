/*
Semestre 2022-1
Práctica : Iluminación
Cambios en el shader, en lugar de enviar la textura en el shader de fragmentos, enviaremos el finalcolor
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//#include <gtx\vector_angle.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"


//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;


Model Terreno_Base;
Model Among_Us;
Model Bang_Girl;
Model Cheems;
Model Cheems_Ch;
Model Dance_Floor;
Model Jesus;
Model Knuckles;
Model Lago;
Model Marceline;
Model Nieve;
Model Bernie;
Model Rei;
Model Snowman;
Model Shuba;
Model Piedra;
Model PiedraNieve;
Model Nubes;


Model Jake;
Model Finn;

Skybox skyboxNight;
Skybox skyboxDay;

//materiales
Material Material_brillante;
Material Material_opaco;

//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLightsNight[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];


Sphere sp = Sphere(1.0, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//Ciclo dia y noche
std::clock_t start;
double duration, edge = 0.0;
int nightLights = 0;
bool dia;

//Caminante del agua (Basica)
float caminante_x = 0;
float caminante_z = 0;
float offset_caminante = 0.5;
int num_recta = 1;
float angulo_caminante = 0.0f;
float limite_angulo_izq = 0.0f;
float limite_angulo_der = 0.0f;
float offset_angulo_caminante = 10.0f;
bool x_ok = false;
bool z_ok = false;
bool angulo_ok = true;
//Movimiento de las nubes (Compleja)
float mueve_x_nubes = 0.0f;
float mueve_z_nubes = 0.0f;
float rota_y_nubes = 0.0f;
float offset_rota_nubes = 0.1;
float offset_mueve_nubes = 5;
float angulo_mueve_nubes = 10;
//Marceline (Extra)
float offset_mueve_marceline = 5;
float mueve_marceline = 0.0;
float angulo_mueve_marceline = 10;
//Movimiento de la roca (Compleja)
float rota_y_roca = 0.0f;
float rota_z_roca = 0.0f;
float offset_rota_roca = 0.1f;
float offset_mueve_roca = 0.1f;
float mueve_x_roca = 0.0f;
float mueve_y_roca = 0.0f;
float altura_y_roca = 0.0f;
//float offset_mueve_roca = 5;
bool rotacion_completa = false;
bool altura_completa = false;
bool inicia_animacion_roca = true;
bool termina_animacion_roca = false;
double tiempo_animacion;
double start_animacion;
float velocidad = 0.0;
//Camara
bool primer_cambio_camara = true;
bool inicio_entrada = true;
bool inicio_nieve = true;
bool inicio_arbol = true;
bool inicio_aerea = true;
float nueva_x_camara, nueva_y_camara, nueva_z_camara = 0.0f;
float camara_x_inicial, camara_y_inicial, camara_z_inicial = 0.0f;
glm::vec3 vectorUp, cam_entrada_anterior, cam_nieve_anterior, cam_arbol_anterior, 
		cam_aerea_anterior;
//Luces Spotlight
//glm::vec3 luz_pista_alpha, luz_pista_beta, luz_pista_gamma;
float luz_pista_x, luz_pista_y, luz_pista_z;
float edge_baile = 0.5;
float color_pista_x, color_pista_y, color_pista_z;
double start_baile, tiempo_baile;
bool inicia_baile;
float pos_luz_alpha_x = -12.0f;
float pos_luz_alpha_z = 121.0f;
float pos_luz_beta_x = 21.0f;
float pos_luz_beta_z = 94.0f;
float pos_luz_gamma_x = -19.0f;
float pos_luz_gamma_z = 84.0f;
float angulo_luz_alpha, angulo_luz_beta, angulo_luz_gamma = 0.0f;
bool sube = false;
bool derecha = true;
float offset_luz_vertical, offset_luz_horizontal = 0.5;
//float offset_mueve_luces = 0.1f;

//Avatar
float rotacion_avatar_y = 0;
glm::vec3 view_camara;
glm::vec3 avatar = glm::vec3(1.0f, 0.0f, 0.0f);
float numerador_angulo, denominador_angulo = 0.0f;



//cálculo del promedio de las normales para iluminacion de Phong, no Ground
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}




void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

//void CrearCubo()
//{
//	unsigned int cubo_indices[] = {
//		// front
//		0, 1, 2,
//		2, 3, 0,
//		// right
//		4, 5, 6,
//		6, 7, 4,
//		// back
//		8, 9, 10,
//		10, 11, 8,
//
//		// left
//		12, 13, 14,
//		14, 15, 12,
//		// bottom
//		16, 17, 18,
//		18, 19, 16,
//		// top
//		20, 21, 22,
//		22, 23, 20,
//	};
//
//
//	GLfloat cubo_vertices[] = {
//		// front
//		//x		y		z		S		T			NX		NY		NZ
//		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
//		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
//		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
//		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
//		// right
//		//x		y		z		S		T
//		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
//		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
//		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
//		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
//		// back
//		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
//		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
//		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
//		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,
//
//		// left
//		//x		y		z		S		T
//		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
//		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
//		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
//		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,
//
//		// bottom
//		//x		y		z		S		T
//		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
//		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
//		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
//		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,
//
//		//UP
//		 //x		y		z		S		T
//		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
//		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
//		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
//		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,
//
//	};
//
//	Mesh *cubo = new Mesh();
//	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
//	meshList.push_back(cubo);
//
//}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.0f, 0.5f);

	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	//brickTexture = Texture("Textures/brick.png");
	//brickTexture.LoadTextureA();
	//dirtTexture = Texture("Textures/dirt.png");
	//dirtTexture.LoadTextureA();
	//plainTexture = Texture("Textures/plain.png");
	//plainTexture.LoadTextureA();
	//dadoTexture = Texture("Textures/dado.tga");
	//dadoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	//Tagave = Texture("Textures/Agave.tga");
	//Tagave.LoadTextureA();
	//
	/*
	Models/among-us.obj
	Models/terreno.obj
	Models/cheems.obj
	Models/knuckles.obj
	Models/lago.obj
	Models/marceline.obj
	Models/snow.obj INEFICIENTE
	bernie.obj
	Models/snowman.obj
	Models/dance-floor.obj
	jesus.obj
	*/

	Terreno_Base = Model();
	Terreno_Base.LoadModel("Models/terreno.obj");

	Piedra = Model();
	Piedra.LoadModel("Models/piedra-centrada.obj");

	PiedraNieve = Model();
	PiedraNieve.LoadModel("Models/piedra-nieve.obj");

	Nubes = Model();
	Nubes.LoadModel("Models/nice-clouds.obj");

	//Llanta_M = Model();
	//Llanta_M.LoadModel("Models/llanta.obj");
	//Blackhawk_M = Model();
	//Blackhawk_M.LoadModel("Models/terreno_base.obj");
	//Camino_M = Model();
	//Camino_M.LoadModel("Models/railroad track.obj");

	//Dado_M = Model();
	//Dado_M.LoadModel("Models/dadoanimales.obj");

	Among_Us = Model();
	Among_Us.LoadModel("Models/among-us.obj");

	Dance_Floor = Model();
	Dance_Floor.LoadModel("Models/dance-floor.obj");

	Lago = Model();
	Lago.LoadModel("Models/lago-pintado.obj");

	Rei = Model();
	Rei.LoadModel("Models/rei.obj");

	Marceline = Model();
	Marceline.LoadModel("Models/marceline.obj");

	Jesus = Model();
	Jesus.LoadModel("Models/jesus.obj");

	Cheems = Model();
	Cheems.LoadModel("Models/cheems.obj");

	Cheems_Ch = Model();
	Cheems_Ch.LoadModel("Models/cheems-ch.obj");

	Bernie = Model();
	Bernie.LoadModel("Models/bernie.obj");

	Bang_Girl = Model();
	Bang_Girl.LoadModel("Models/bang-girl.obj");

	Knuckles = Model();
	Knuckles.LoadModel("Models/knuckles.obj");

	Snowman = Model();
	Snowman.LoadModel("Models/snowman.obj");

	Shuba = Model();
	Shuba.LoadModel("Models/shuba-duck.obj");

	Finn = Model();
	Finn.LoadModel("Models/finn.obj");

	std::vector<std::string> skyboxNightFaces;
	std::vector<std::string> skyboxDayFaces;
	skyboxNightFaces.push_back("Textures/Skybox/sky-night-right.tga");
	skyboxNightFaces.push_back("Textures/Skybox/sky-night-left.tga");
	skyboxNightFaces.push_back("Textures/Skybox/sky-night-bottom.tga");
	skyboxNightFaces.push_back("Textures/Skybox/sky-night-up.tga");
	skyboxNightFaces.push_back("Textures/Skybox/sky-night-front.tga");
	skyboxNightFaces.push_back("Textures/Skybox/sky-night-back.tga");

	skyboxDayFaces.push_back("Textures/Skybox/sky-day-right.tga");
	skyboxDayFaces.push_back("Textures/Skybox/sky-day-left.tga");
	skyboxDayFaces.push_back("Textures/Skybox/sky-day-bottom.tga");
	skyboxDayFaces.push_back("Textures/Skybox/sky-day-up.tga");
	skyboxDayFaces.push_back("Textures/Skybox/sky-day-front.tga");
	skyboxDayFaces.push_back("Textures/Skybox/sky-day-back.tga");

	/*
	skyboxFaces.push_back("Textures/Skybox/sky-night-right.tga");
	skyboxFaces.push_back("Textures/Skybox/sky-night-leftf.tga");
	skyboxFaces.push_back("Textures/Skybox/sky-night-bottom.tga");
	skyboxFaces.push_back("Textures/Skybox/sky-night-up.tga"); //--
	skyboxFaces.push_back("Textures/Skybox/sky-night-back.tga");
	skyboxFaces.push_back("Textures/Skybox/sky-night-front.tga");
	*/

	skyboxNight = Skybox(skyboxNightFaces);
	skyboxDay = Skybox(skyboxDayFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(1.0f, 40);

	//posición inicial del helicóptero
	glm::vec3 posblackhawk = glm::vec3(-20.0f, 6.0f, -1.0);

	//luz direccional, sólo 1 y siempre debe de existir [0.3373f, 0.4902f, 0.6235f,]
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int pointLightCountNight = 0;

	//Luces de día
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f, //COLOR
		0.0f, 10.0f,
		-19.0f, -33.0f, 325.0f, //POSICION
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//Declaración de segunda luz puntual
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f, //COLOR
		0.0f, 10.0f,
		-23.0f, -33.0f, 292.0f, //POSICION
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//Declaración de tercera luz puntual en piñata
	pointLights[2] = PointLight(1.0f, 1.0f, 0.0f, //COLOR
		10.0f, 0.0f,
		166.0f, -98.0f, 397.0f, //POSICION
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//LUCES DE NOCHE

	pointLightsNight[2] = PointLight(1.0f, 0.0f, 0.0f, //COLOR
		0.0f, 10.0f,
		-19.0f, -33.0f, 325.0f, //POSICION
		0.3f, 0.2f, 0.1f);
	pointLightCountNight++;

	//Declaración de segunda luz puntual
	pointLightsNight[1] = PointLight(0.0f, 1.0f, 0.0f, //COLOR
		0.0f, 10.0f,
		-23.0f, -33.0f, 292.0f, //POSICION
		0.3f, 0.2f, 0.1f);
	pointLightCountNight++;

	//Declaración de tercera luz puntual en piñata
	pointLightsNight[0] = PointLight(1.0f, 1.0f, 0.0f, //COLOR
		10.0f, 0.0f,
		166.0f, -98.0f, 397.0f, //POSICION
		0.3f, 0.2f, 0.1f);
	pointLightCountNight++;

	unsigned int spotLightCount = 0;

	//Luces SpotLights para la pista de baile
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		pos_luz_alpha_x, 60.0f, pos_luz_alpha_z, //Posicion
		0.0f, -1.0f, 0.0f, //Direccion
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		pos_luz_beta_x, 60.0f, pos_luz_beta_z,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		pos_luz_gamma_x, 60.0f, pos_luz_gamma_z,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	


	

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	//Ciclo de dia y noche 
	start = std::clock();
	//Modelado geometrico 
	GLuint uniformColor = 0;
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		if (mainWindow.getTipoCamara() != 4) {
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}
		//else { //Ahorrarse el set pos para que mire hacia abajo
			//camera.mouseControl(0.0f, -90.0f);
		//}
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//150 segundos = 1:30 min
		if (duration <= 10) { //Dia
			mainLight.setLight(glm::vec3(1.0f, 1.0f, 1.0f));
			if (mainWindow.getTipoCamara() != 4) {
				skyboxDay.DrawSkybox(camera.calculateViewMatrix(false), projection);
			}
			else {
				skyboxDay.DrawSkybox(camera.calculateViewMatrix(true), projection);
			}
			dia = true;
			nightLights = 2;
		}
		else { //Noche
			mainLight.setLight(glm::vec3(0.3373f, 0.4902f, 0.6235f)); //Color de luz ambiental noche
			if (mainWindow.getTipoCamara() != 4) {
				skyboxNight.DrawSkybox(camera.calculateViewMatrix(false), projection);
			}
			else {
				skyboxNight.DrawSkybox(camera.calculateViewMatrix(true), projection);
			}
			dia = false;
			nightLights = 0;
			if (duration >= 20) { //Regresar al dia
				start = std::clock();
			}
		}

		//Animacion basica para el caminante del agua
		if (mainWindow.getMueveCaminante() == 1.0f) {
			switch (num_recta)
			{
			case 1:
				limite_angulo_der = - 90;
				limite_angulo_izq = 0;
				if (caminante_x < 66) {
					caminante_x += offset_caminante * deltaTime;
				}
				else {
					num_recta = 2;
				}
				break;
			case 2:
				limite_angulo_der = -45;
				limite_angulo_izq = 45;
				if (caminante_x < (66+70)) {
					caminante_x += offset_caminante * deltaTime;
				}
				else {
					x_ok = true;
				}
				if (caminante_z < 72) {
					caminante_z += offset_caminante * deltaTime;
				}
				else {
					z_ok = true;
				}
				if (x_ok and z_ok) {
					x_ok = false;
					z_ok = false;
					num_recta = 3;
				}
				break;
			case 3:
				limite_angulo_der = 0;
				limite_angulo_izq = 90;
				if (caminante_x > (66 + 70 - 71)) {
					caminante_x -= offset_caminante * deltaTime;
				}
				else {
					x_ok = true;
				}
				if (caminante_z < (72+43)) {
					caminante_z += offset_caminante * deltaTime;
				}
				else {
					z_ok = true;
				}
				if (x_ok and z_ok) {
					x_ok = false;
					z_ok = false;
					num_recta = 4;
				}
				break;
			case 4:
				limite_angulo_der = -45;
				limite_angulo_izq = 45;
				if (caminante_x > (66 + 70 - 71 - 66)) {
					caminante_x -= offset_caminante * deltaTime;
				}
				else {
					x_ok = true;
				}
				if (caminante_z > (72 + 43 - 129)) {
					caminante_z -= offset_caminante * deltaTime;
				}
				else {
					z_ok = true;
				}
				if (x_ok and z_ok) {
					x_ok = false;
					z_ok = false;
					num_recta = 5;
				}
				break;
			case 5:
				caminante_x = 0;
				caminante_z = 0;
				num_recta = 1;
				limite_angulo_izq = 0.0f;
				limite_angulo_der = 0.0f;
				offset_caminante = 0.5f;
				mainWindow.setMueveCaminante(0.0f);
				break;
			default:
				break;
			}

			if (angulo_ok) {
				angulo_caminante -= offset_angulo_caminante * deltaTime;
				if (angulo_caminante < limite_angulo_der) {
					angulo_ok = false;
				}
			}
			else {
				angulo_caminante += offset_angulo_caminante * deltaTime;
				if (angulo_caminante > limite_angulo_izq) {
					angulo_ok = true;
				}
			}
		}

		//Animacion compleja para el movimiento de las nubes
		if (mainWindow.getMueveNubes() == 1.0) {
			rota_y_nubes += offset_rota_nubes * deltaTime;
			mueve_x_nubes = 20 * sin(angulo_mueve_nubes * toRadians * 0.3);
			mueve_z_nubes = 5 * cos(angulo_mueve_nubes * toRadians * 0.5);
			angulo_mueve_nubes += offset_mueve_nubes * deltaTime;
		}

		//Animacion para el easter egg de la roca
		if (mainWindow.getMueveRoca() == 1.0f) {
			if (!termina_animacion_roca) {
				if (inicia_animacion_roca) {
					start_animacion = std::clock();
					inicia_animacion_roca = false;
				}
				tiempo_animacion = (std::clock() - start_animacion) / (double)CLOCKS_PER_SEC;
				printf("Tiempo: %f\n", tiempo_animacion);

				if (tiempo_animacion < 2) {
					velocidad = 5;
				}
				else if (tiempo_animacion < 5) {
					velocidad = 10;
				}
				else{
					velocidad = 25;
				}
				rota_y_roca += velocidad * deltaTime; //Rotacion
				if (tiempo_animacion > 8) {
					rotacion_completa = true;
				}

				if (velocidad == 25) { //Rotacion completa
					if (altura_y_roca < 170.0f) {
						altura_y_roca += 1 * deltaTime;
					}
					else {
						altura_completa = true;
					}
					if (altura_completa) {
						mueve_x_roca = offset_mueve_roca;
						offset_mueve_roca += 1;
						mueve_y_roca = -(0.01 * pow(mueve_x_roca, 2));
						if (rota_z_roca < 35) {
							rota_z_roca += 0.1 * deltaTime;
						}
						
					}
					if (mueve_y_roca < -250.0f and altura_completa) {
						termina_animacion_roca = true;
					}
				}
				
			}
			
			
		}
		
		//Cambio de camaras ligadas al piso y camaras aereas.

		switch (mainWindow.getTipoCamara())
		{
		case 1: //Entrada
			if (inicio_entrada) { //Se toma la inicial
				camara_x_inicial = cam_entrada_anterior.x = -177.0f;
				camara_z_inicial = cam_entrada_anterior.x = 302.0f;
				inicio_entrada = false;
			}
			else {  //Se toma la anterior
				camara_x_inicial = cam_entrada_anterior.x;
				camara_z_inicial = cam_entrada_anterior.z;
			}
			nueva_y_camara = -65.0f;
			break;

		case 2: //Nieve
			if (inicio_nieve) { //Se toma la inicial
				camara_x_inicial = cam_nieve_anterior.x = 116.0f;
				camara_z_inicial = cam_nieve_anterior.x = 413.0f;
				inicio_nieve = false;
			}
			else {  //Se toma la anterior
				camara_x_inicial = cam_nieve_anterior.x;
				camara_z_inicial = cam_nieve_anterior.z;
			}
			//camara_x_inicial = 116.0f;
			//nueva_y_camara = -85.0f;
			//camara_z_inicial = 413.0f;
			nueva_y_camara = -85.0f;
			break;

		case 3: //Arbol
			if (inicio_arbol) { //Se toma la inicial
				camara_x_inicial = cam_arbol_anterior.x = 18.0f;
				camara_z_inicial = cam_arbol_anterior.z = 186.0f;
				inicio_arbol = false;
			}
			else {  //Se toma la anterior
				camara_x_inicial = cam_arbol_anterior.x;
				camara_z_inicial = cam_arbol_anterior.z;
			}
			nueva_y_camara = 20.0f;
			break;

		case 4: //Aerea

			if (inicio_aerea) { //Se toma la inicial
				camara_x_inicial = cam_aerea_anterior.x = -187.0f;
				camara_z_inicial = cam_aerea_anterior.z = 302.0f;
				inicio_aerea = false;
			}
			else {  //Se toma la anterior
				camara_x_inicial = cam_aerea_anterior.x;
				camara_z_inicial = cam_aerea_anterior.z;
			}
			nueva_y_camara = 180.0f;
			break;

		case 5: //Libre
			camara_x_inicial = camera.getCameraPosition().x;
			nueva_y_camara = camera.getCameraPosition().y;
			camara_z_inicial = camera.getCameraPosition().z;
			break;

		default:
			break;
		}

		if (mainWindow.getCameraFirstChange()) {
			nueva_x_camara = camara_x_inicial;
			nueva_z_camara = camara_z_inicial;
			mainWindow.setCameraFirstChange(false);
		}
		else {
			nueva_x_camara = camera.getCameraPosition().x;
			nueva_z_camara = camera.getCameraPosition().z;

			switch (mainWindow.getTipoCamara()) //Guardado de posiciones anteriores
			{
			case 1:
				cam_entrada_anterior.x = camera.getCameraPosition().x;
				cam_entrada_anterior.z = camera.getCameraPosition().z;
				break;
			case 2:
				cam_nieve_anterior.x = camera.getCameraPosition().x;
				cam_nieve_anterior.z = camera.getCameraPosition().z;
				break;
			case 3:
				cam_arbol_anterior.x = camera.getCameraPosition().x;
				cam_arbol_anterior.z = camera.getCameraPosition().z;
				break;
			case 4:
				cam_aerea_anterior.x = camera.getCameraPosition().x;
				cam_aerea_anterior.z = camera.getCameraPosition().z;
				break;
			default:
				break;
			}
		}

		camera.setPosition(glm::vec3(nueva_x_camara, nueva_y_camara, nueva_z_camara));

		//Show de luces spotlight
		if (mainWindow.getBaile() == 1.0) {
			if (inicia_baile) {
				start_baile = std::clock();
				inicia_baile = false;
			}
			tiempo_baile = (std::clock() - start_baile) / (double)CLOCKS_PER_SEC;
			printf("Tiempo: %f\n", tiempo_baile);

			if (tiempo_baile > edge_baile) {
				for (int i = 0; i < 3; i++) {
					color_pista_x = (float)rand() / (float)RAND_MAX;
					color_pista_y = (float)rand() / (float)RAND_MAX;
					color_pista_z = (float)rand() / (float)RAND_MAX;
					spotLights[i].SetColor(glm::vec3(color_pista_x, color_pista_y, color_pista_z));
				}
				edge_baile += 0.5;
			}
			
			//Movimiento de la primera luz
			
			pos_luz_gamma_x = 4 * sin(angulo_luz_gamma * toRadians) - 19.0;
			pos_luz_gamma_z = 7 * cos(angulo_luz_gamma * toRadians) + 84.0;
			angulo_luz_gamma += 35 * deltaTime;
			spotLights[1].SetPos(glm::vec3(pos_luz_gamma_x, 60.0f, pos_luz_gamma_z));

			pos_luz_alpha_x = 20 * sin(angulo_luz_alpha * toRadians) + 3.0;
			pos_luz_alpha_z = 5 * cos(angulo_luz_alpha * toRadians) + 130.0;
			angulo_luz_alpha += 25 * deltaTime;
			spotLights[0].SetPos(glm::vec3(pos_luz_alpha_x, 60.0f, pos_luz_alpha_z));

			pos_luz_beta_x = 5 * sin(angulo_luz_beta * toRadians) + 21.0;
			pos_luz_beta_z = 5 * cos(angulo_luz_beta * toRadians) + 94.0;
			angulo_luz_beta += 10 * deltaTime;
			

			if (sube) {
				pos_luz_beta_x += (0.5 + offset_luz_vertical) * deltaTime;
				offset_luz_vertical += 0.5;
				if (pos_luz_beta_x > 20.0) {
					sube = false;
					offset_luz_vertical = 0.5;
				}
			}
			else {
				pos_luz_beta_x -= (0.5 + offset_luz_vertical) * deltaTime;
				offset_luz_vertical += 0.5;
				if (pos_luz_beta_x < -26.0) {
					sube = true;
					offset_luz_vertical = 0.5;
				}
			}

			if (derecha) {
				pos_luz_beta_z += (0.5 + offset_luz_vertical) * deltaTime;
				offset_luz_vertical += 0.5;
				if (pos_luz_beta_z > 138.0) {
					derecha = false;
					offset_luz_vertical = 0.5;
				}
			}
			else {
				pos_luz_beta_z -= (0.5 + offset_luz_vertical) * deltaTime;
				offset_luz_vertical += 0.5;
				if (pos_luz_beta_z < 80.0) {
					derecha = true;
					offset_luz_vertical = 0.5;
				}
			}

			spotLights[2].SetPos(glm::vec3(pos_luz_beta_x, 60.0f, pos_luz_beta_z));


		}
		else {
			tiempo_baile = 0.0;
			start_baile = 0.0;
			inicia_baile = true;
			edge_baile = 0.5;
		}

		//Animacion del avatar
		//view_camara = camera.getCameraView();
		//numerador_angulo = (view_camara.x * avatar.x) + (view_camara.z * avatar.z);
		//denominador_angulo = sqrt(pow(view_camara.x, 2) + pow(view_camara.z, 2)) 
		//	* sqrt(pow(avatar.x, 2) + pow(avatar.z, 2));
		rotacion_avatar_y = camera.getCameraDirection().z;
		//printf("Rotacion Avatar: %f\n", rotacion_avatar_y);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		if (mainWindow.getTipoCamara() != 4) {
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix(false)));
		}
		else {
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix(true)));
		}
		
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		printf("x: %f, y: %f, z: %f \n", camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//luz ligada a la cámara de tipo flash 
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount - nightLights);

		if (dia) { //Luz que enciende y apaga
			if (mainWindow.getapaga_luz_pinata()) {
				shaderList[0].SetPointLights(pointLightsNight, pointLightCount - nightLights - 1);
			}
			else {
				shaderList[0].SetPointLights(pointLightsNight, pointLightCount - nightLights);
			}
		}
		else {
			if (mainWindow.getapaga_luz_pinata()) {
				shaderList[0].SetPointLights(pointLights, pointLightCount - nightLights - 1);
			}
			else {
				shaderList[0].SetPointLights(pointLights, pointLightCount - nightLights);
			}
		}
		
		

		
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 model_centro_auto(1.0);
		glm::mat4 model_chopper(1.0);


		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//pisoTexture.UseTexture();
		////agregar material al plano de piso
		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[2]->RenderMesh();

		//agregar su coche y ponerle material
		model = glm::mat4(1.0);
		model_centro_auto = glm::mat4(1.0);
		model_chopper = glm::mat4(1.0);

		//Terreno Base
		model = glm::translate(model, glm::vec3(0.0f,  0.5f , -1.5f));
		model_centro_auto = model;
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Terreno_Base.RenderModel();

		////Piedra Animada
		//model = glm::translate(model, glm::vec3(0.0f , 0.0f, 0.0f ));
		//model_centro_auto = model;
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Piedra.RenderModel();

		//Among us
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.0f, 2.0f, 162.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Among_Us.RenderModel();

		//Finn
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(camera.getCameraPosition().x + 20.0f, camera.getCameraPosition().y - 30.0, camera.getCameraPosition().z));
		//model = glm::translate(model, glm::vec3(-177.0f + 5.0, -65.0f - 30.0, 302.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotacion_avatar_y, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::rotate(model, rotacion_avatar_y, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, rotacion_avatar_y, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Finn.RenderModel();

		//Knucles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 5.0f, 150.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 140 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Knuckles.RenderModel();

		//Lago
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-135.0f, -21.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lago.RenderModel();

		//Jesus
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-265.0f + caminante_x, -16.0f, -16.0f + caminante_z));
		model = glm::scale(model, glm::vec3(0.1f, 0.1, 0.1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, angulo_caminante * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Jesus.RenderModel();

		//Rei
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-26.0f, 5.0f, 80.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rei.RenderModel();

		//Shuba
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 8.0f, 138.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Shuba.RenderModel();

		//Marceline entrada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, -30.0f + mueve_marceline, 280.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Marceline.RenderModel();

		//Movimiento de marceline
		mueve_marceline = 10 * sin(angulo_mueve_marceline * toRadians * 0.3);
		angulo_mueve_marceline += offset_mueve_marceline * deltaTime;

		//Cheems fuerte Piñata
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -85.0f, 370.0f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		//model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cheems.RenderModel();

		//Cheems chiquito Piñata
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(186.0f, -110.0f, 420.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		//model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cheems_Ch.RenderModel();

		//Bernie
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 9.0f, -130.0f));
		model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bernie.RenderModel();

		//Bang Girl
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 7.0f, -130.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bang_Girl.RenderModel();

		//Hombres de nieve
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 2.0f, -200.0f));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Snowman.RenderModel();

		//Hombres de nieve
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, -92.0f, 345.0f));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Snowman.RenderModel();

		//Hombres de nieve
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -103.0f, 310.0f));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));
		model = glm::rotate(model, 115 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Snowman.RenderModel();

		//Hombres de nieve
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(167.0f, 2.0f, 42.0f));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));
		model = glm::rotate(model, 95 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Snowman.RenderModel();

		////--------------------------------
		////Modelado geometrico
		//model = glm::mat4(1.0f);
		glm::vec3 color = glm::vec3(0.816f, 0.384f, 0.384f);	//ROSA 1
		//model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		//color = glm::vec3(0.47f, 0.47f, 0.47f);
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//sp.render(); //Renderiza esfera

		//Nubes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 50.0f + mueve_x_nubes, 20.0f + mueve_z_nubes));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rota_y_nubes * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nubes.RenderModel();

		//PIEDRA ANIMADA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(168.0f , 4.0f + mueve_y_roca + altura_y_roca, 153.0f + mueve_x_roca));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rota_y_roca * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rota_z_roca * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Piedra.RenderModel();

		//Pista de baile
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 106.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		model = glm::rotate(model, 3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Dance_Floor.RenderModel();

		//Esferas del arbol
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(41.0f, 88.0f, 71.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(51.0f, 109.0f, 77.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(45.0f, 131.0f, 73.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(44.0f, 148.0f, 70.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera
		//-------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(31.0f, 97.0f, 52.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(34.0f, 120.0f, 52.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(34.0f, 142.0f, 40.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(44.0f, 170.0f, 54.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		//-------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(44.0f, 120.0f, 22.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(40.0f, 86.0f, 20.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(50.0f, 99.0f, 12.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(60.0f, 126.0f, 7.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		//-------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(34.0f, 111.0f, 34.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(44.0f, 165.0f, 30.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(89.0f, 150.0f, 12.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(94.0f, 174.0f, 57.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		//-------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(48.0f, 148.0f, 16.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(85.0f, 105.0f, 6.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(104.0f, 46.0f, 21.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(109.0f, 78.0f, 41.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		//-------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(108.0f, 114.0f, 28.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(110.0f, 126.0f, 57.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(97.0f, 140.0f, 69.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(88.0f, 102.0f, 78.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		//-------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(103.0f, 91.0f, 62.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(84.0f, 14.0f, 83.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(108.0f, 150.0f, 37.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(53.0f, 59.0f, 86.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(76.0f, 153.0f, 83.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		//-------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(63.0f, 93.0f, 89.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(86.0f, 117.0f, 114.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(64.0f, 123.0f, 121.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(85.0f, 107.0f, 104.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera


		//PIEDRA NIEVE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(262.0f, -99.0f, 398.0f));
		model = glm::scale(model, glm::vec3(10.0f, 30.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PiedraNieve.RenderModel();

		//--------------------------------
		//Modelado geometrico
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-102.0f, -97.0f, 375.0f));
		model = glm::scale(model, glm::vec3(18.0f, 10.0f, 40.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-47.0f, -80.0f, 262.0f));
		model = glm::scale(model, glm::vec3(20.0f, 2.0f, 15.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-74.0f, -87.0f, 237.0f));
		model = glm::scale(model, glm::vec3(15.0f, 10.0f, 30.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		//Nieve arbol

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-57.0f, -4.0f, 126.0f));
		model = glm::scale(model, glm::vec3(20.0f, 4.0f, 15.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(90.0f, 6.0f, 63.0f));
		model = glm::scale(model, glm::vec3(20.0f, 5.0f, 15.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-101.0f, 3.0f, -84.0f));
		model = glm::scale(model, glm::vec3(20.0f, 2.0f, 25.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-121.0f, 3.0f, -97.0f));
		model = glm::scale(model, glm::vec3(20.0f, 7.0f, 25.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-131.0f, 3.0f, -90.0f));
		model = glm::scale(model, glm::vec3(20.0f, 2.0f, 25.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(97.0f, 8.0f, -78.0f));
		model = glm::scale(model, glm::vec3(33.0f, 4.0f, 40.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(12.0f, 4.0f, -72.0f));
		model = glm::scale(model, glm::vec3(30.0f, 2.0f, 25.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-69.0f, 5.0f, -189.0f));
		model = glm::scale(model, glm::vec3(33.0f, 7.0f, 40.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(158.0f, -1.5f, 90.0f));
		model = glm::scale(model, glm::vec3(30.0f, 2.0f, 25.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-27.0f, -3.0f, 6.0f));
		model = glm::scale(model, glm::vec3(20.0f, 5.0f, 34.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		//-----------------------------------------------------
		////Modelado Jerarquico
		//glm::vec3 color;
		//model = glm::mat4(1.0f);
		////color = glm::vec3(0.891f, 0.663f, 0.663f);
		//color = glm::vec3(1.0f, 0.0f, 0.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.0f));
		//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//meshList[4]->RenderMesh();

		////Luz Auto derecha
		//glm::vec3 faros = glm::vec3(mainWindow.getmuevex() -10.0, 0.0f, -4.0f + mainWindow.getmuevez());
		//glm::vec3 dir = glm::vec3(1.02f, 0.0f, 0.0f);
		//spotLights[2].SetFlash(faros, dir);

		////Luz Auto izquierda
		//faros = glm::vec3(mainWindow.getmuevex() - 10.0, 0.0f, -8.5f + mainWindow.getmuevez());
		//dir = glm::vec3(1.02f, 0.0f, 0.0f);
		//spotLights[1].SetFlash(faros, dir);


		//Agregar llantas con jerarquía y rotación propia
		//model = glm::mat4(1.0);

		////LLANTA DELANTERA IZQUIERDA
		//model = model_centro_auto;
		//model = glm::translate(model, glm::vec3(-7.1f, -0.5f, -8.0f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, mainWindow.getmueve_llanta(), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////LLANTA TRACERA IZQUIERDA
		//model = model_centro_auto;
		//model = glm::translate(model, glm::vec3(-15.6f, -0.5f, -8.0f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, mainWindow.getmueve_llanta(), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////LLANTA TRACERA DERECHA
		//model = model_centro_auto;
		//model = glm::translate(model, glm::vec3(-15.6f, -0.5f, -1.5f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, mainWindow.getmueve_llanta(), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();

		////LLANTA DELANTERA DERECHA
		//model = model_centro_auto;
		//model = glm::translate(model, glm::vec3(-7.1f, -0.5f, -1.5f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, mainWindow.getmueve_llanta(), glm::vec3(1.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Llanta_M.RenderModel();


		//agregar incremento en X con teclado
		//model = glm::mat4(1.0);
		////model = glm::translate(model, glm::vec3(-20.0f, 6.0f, -1.0));
		//model = glm::translate(model, posblackhawk); 

		//GLfloat mov_x_pos = mainWindow.getmueve_x_pos_chopper();
		//GLfloat mov_y_pos = mainWindow.getmueve_y_pos_chopper();

		////model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.5f, -1.5f));
		//model = glm::translate(model, glm::vec3(mov_x_pos, 0.5f + mov_y_pos , -1.5f));
		//model_chopper = model; //Para la jerarquia del helicoptero

		//model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		////model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////agregar material al helicóptero
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Blackhawk_M.RenderModel();
		//¿Cómo ligas la luz al helicóptero?
		// 
		//Luz chopper
		//glm::vec3 luz_chopper = glm::vec3(mov_x_pos -22.0f, 1.0f + mov_y_pos, -2.5f);
		//glm::vec3 dir_chopper = glm::vec3(0.0f, -1.05f, 0.0f);
		//spotLights[0].SetFlash(luz_chopper, dir_chopper);

		////Helice chopper
		//model = model_chopper;
		//model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		////model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, mainWindow.getmueve_helices(), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Helice.RenderModel();



		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -1.53f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Camino_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
