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

#include <irrklang\irrKlang.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//#include <gtx\vector_angle.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "MeshGeo.h"
#include"ShaderGeo.h"
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

//using namespace irrklang;

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture pisoTexture;
Texture picos_piniata1;
Texture picos_piniata2;
Texture cuerpo_piniata;
Texture mesas;

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
Model Pan;

//Jerarquia del avatar
Model FinnCuerpo;
Model FinnBrazoDerecho;
Model FinnBrazoIzquierdo;
Model FinnPiernaDerecha;
Model FinnPiernaDerechaBa;
Model FinnPiernaIzquierda;
Model FinnPiernaIzquierdaBa;

Model Jake;
Model Finn;

Skybox skyboxNight;
Skybox skyboxDay;

//materiales
Material Material_brillante;
Material Material_opaco;
Material Material_finn;
Material Material_neutro;

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

//Vertex Shader del modelado geometrico
static const char* vShaderGeo = "shaders/shader.vert";
static const char* fShaderGeo = "shaders/shader.frag";

std::vector<MeshGeo*> meshListGeo;
std::vector<ShaderGeo>shaderListGeo;

//Ciclo dia y noche
std::clock_t start;
double duration, edge = 0.0;
int nightLights = 0;
bool dia;

//Caminante del agua (Basica)
bool inicia_caminante = true;
float caminante_x = 0.0;
float caminante_z = 0.0;
float offset_caminante = 0.2;
int num_recta = 1;
float angulo_caminante = 0.0f;
float limite_angulo_izq = 0.0f;
float limite_angulo_der = 0.0f;
float offset_angulo_caminante = 10.0f;
bool x_ok = false;
bool z_ok = false;
bool angulo_ok = true;
glm::vec3 knuckles_pos = glm::vec3(60.0f, 5.0f, 150.0f);
float knuckles_x = 0.0;
bool knuckles_x_ok = false;
float knuckles_y = 0.0;
bool knuckles_y_ok = false;
float rota_knuckles_z = 0.0;
bool rota_knuckles_z_ok = false;
bool arriba_knuckes = true;
float offset_knuckles = 0.3;
float offset_rota_knuckles = 2.0;
//Movimiento de las nubes (Compleja)
float mueve_x_nubes = 0.0f;
float mueve_z_nubes = 0.0f;
float rota_y_nubes = 0.0f;
float offset_rota_nubes = 0.1;
float offset_mueve_nubes = 5;
float angulo_mueve_nubes = 10;
//Movimiento de los muñecos (Basica)
float mueve_x_munecos = 0.0f;
float mueve_z_munecos = 0.0f;
float rota_y_munecos = 0.0f;
float rota_z_munecos = 0.0f;
float offset_rota_munecos = 1.0;
float offset_mueve_munecos = 0.3;
int pasos = 0;
bool inicio_munecos = false;
bool adelante_munecos = true;
bool derecha_munecos = true;
bool angulo_z_munecos_ok = false;
bool angulo_y_munecos_ok = false;

//float angulo_mueve_nubes = 10;
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
//Rei chiquita
float mueve_x_rei = 0.0f;
float mueve_z_rei = 0.0f;
float mueve_y_rei = 0.0f;
float num_lem_rei = 0.0f;
float den_lem_rei = 0.0f;
float rota_y_rei = 0.0f;
float rota_z_rei = 0.0f;
float rota_x_rei = 0.0f;
float offset_rota_rei = 1.0;
float offset_mueve_rei = 0.3;
float angulo_lemn = 0.0f;
bool inicio_rei = false;
bool angulo_z_rei_ok = false;
bool angulo_y_rei_ok = false;
bool angulo_x_rei_ok = false;
//Shubba duck
float mueve_x_shubba = 0.0f;
float mueve_z_shubba = 0.0f;
float mueve_y_shubba = 0.0f;

float rota_z_shubba = 0.0f;
float rota_x_shubba = 0.0f;
float rota_y_shubba = 0.0f;

float offset_rota_shubba = 1.0;
float offset_mueve_shubba = 0.3;
float angulo_shuba = 0.0f;
bool angulo_z_shuba_ok = false;
bool angulo_x_shuba_ok = false;
bool angulo_y_shuba_ok = false;
//bool adelante_munecos = true;
//bool derecha_munecos = true;


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
bool inicia_musica = true;
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
float avatar_y = 0.0;
float rota_avatar_brazos = 0.0f;
bool angulo_z_avatar_ok = false;
float rota_avatar_piernas = 0.0f;
bool angulo_x_avatar_ok = false;
float rota_avatar_pies = 0.0f;
bool angulo_x_pies_ok = false;



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

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		9, 8, 11,
		11, 10, 9,
		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		19, 18, 17,
		17, 16, 19,
		// top
		21, 20, 23,
		23, 22, 21
	};

	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		u		v
		-0.5f, -0.5f,  0.5f,	0.01f,	0.01f,//0
		0.5f, -0.5f,  0.5f,		0.25f,	0.01f,//1
		0.5f,  0.5f,  0.5f,		0.25f,	0.33f,//2
		-0.5f,  0.5f,  0.5f,	0.01f,	0.33f,//3
		// right 
		0.5f, -0.5f,  0.5f,		0.0f,	0.34f,//4.
		0.5f, -0.5f, -0.5f,		0.25f,	0.34f,//5.
		0.5f, 0.5f,  -0.5f,		0.25f,	0.66f,//6
		0.5f, 0.5f,  0.5f,		0.0f,	0.66f,//7
		// back
		-0.5f, -0.5f, -0.5f,	0.5f,	0.34f,//8
		0.5f, -0.5f, -0.5f,		0.25f,	0.34f,//9
		0.5f,  0.5f, -0.5f,		0.25f,	0.66f,//10
		-0.5f,  0.5f, -0.5f,	0.5f,	0.66f,//11
		// left
		-0.5f, -0.5f,  0.5f,	0.75f,	0.34f,//12
		-0.5f, -0.5f, -0.5f,	0.505f,	0.34f,//13
		-0.5f,  0.5f, -0.5f,	0.505f,	0.665f,//14 
		-0.5f,  0.5f,  0.5f,	0.75f,	0.665f,//15
		//bottom
		-0.5f, -0.5f,  0.5f,	0.505f,	1.0f,//16
		0.5f, -0.5f,  0.5f,		0.747f,	1.0f,//17
		0.5f,  -0.5f, -0.5f,	0.747f,	0.67f,//18
		-0.5f,  -0.5f, -0.5f,	0.505f,	0.67f,//19
		//top
		0.5f,  0.5f,  0.5f,		1.0f,	0.34f,//20
		-0.5f,  0.5f, 0.5f,		0.76f,	0.34f,//21
		-0.5f,  0.5f, -0.5f,	0.76f,	0.67f,//22
		0.5f,  0.5f,  -0.5f,	1.0f,	0.67f,//23


	};
	MeshGeo* cubo = new MeshGeo();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 120, 36);
	meshListGeo.push_back(cubo);

}

void CrearPiramide()
{
	unsigned int piramide_indices[] = {
		//TOP
		// front
		0, 1, 2,
		// right
		3, 4, 5,
		// back
		6, 7, 8,
		// left
		9, 10, 11
	};

	GLfloat piramide_vertices[] = {
		//x		y		z		u		v
		//BOTTOM

		// front [6]
		0.5f, 0.0f,  0.5f,		0.375f,	0.2f,//12
		-0.5f, 0.0f,  0.5f,		0.25f,	0.4f,//13
		0.0f,  -0.87f,  0.0f,	0.125f,	0.2f,//14
		// right [4]
		0.5f, 0.0f,  -0.5f,		0.625f,	0.21f,//15
		0.5f, 0.0f,  0.5f,		0.51f,	0.395,//16
		0.0f,  -0.87f,  0.0f,	0.74f,	0.39f,//17
		// back [8]
		0.5f, 0.0f,  -0.5f,		0.375f,	0.6f,//18
		-0.5f, 0.0f,  -0.5f,	0.125f,	0.6f,//19  
		0.0f,  -0.87f,  0.0f,	0.25f,	0.8f,//20
		// left [2]
		-0.5f, 0.0f,  -0.5f,	0.5f,	0.41f,//21
		-0.5f, 0.0f,  0.5f,		0.39f,	0.59f,//22
		0.0f,  -0.87f,  0.0f,	0.62f,	0.6f//23
	};
	MeshGeo* piramide = new MeshGeo();
	piramide->CreateMesh(piramide_vertices, piramide_indices, 60, 12);
	meshListGeo.push_back(piramide);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
	//Para las geometrias
	ShaderGeo* shader2 = new ShaderGeo();
	shader2->CreateFromFiles(vShaderGeo, fShaderGeo);
	shaderListGeo.push_back(*shader2);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CrearPiramide();
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
	pisoTexture = Texture("Textures/shuba-yellow.tga");
	pisoTexture.LoadTextureA();
	mesas = Texture("Textures/base-cafe-arbol.tga");
	mesas.LoadTextureA();
	cuerpo_piniata = Texture("Textures/piniata.tga");
	cuerpo_piniata.LoadTextureA();
	picos_piniata2 = Texture("Textures/lago-agua.tga");
	picos_piniata2.LoadTextureA();
	picos_piniata1 = Texture("Textures/base-clouds.tga");
	picos_piniata1.LoadTextureA();
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

	Pan = Model();
	Pan.LoadModel("Models/pan.obj");

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
	Finn.LoadModel("Models/finn-cuerpo-jerarquia.obj");

	FinnCuerpo = Model();
	FinnCuerpo.LoadModel("Models/finn-cuerpo.obj");

	FinnBrazoDerecho = Model();
	FinnBrazoDerecho.LoadModel("Models/finn-brazo-derecho.obj");

	FinnBrazoIzquierdo = Model();
	FinnBrazoIzquierdo.LoadModel("Models/finn-brazo-izquierdo.obj");

	FinnPiernaDerecha = Model();
	FinnPiernaDerecha.LoadModel("Models/finn-pierna-derecha.obj");

	FinnPiernaDerechaBa = Model();
	FinnPiernaDerechaBa.LoadModel("Models/finn-pierna-derecha-ba.obj");

	FinnPiernaIzquierda = Model();
	FinnPiernaIzquierda.LoadModel("Models/finn-pierna-izquierda.obj");

	FinnPiernaIzquierdaBa = Model();
	FinnPiernaIzquierdaBa.LoadModel("Models/finn-pierna-izquierda-ba.obj");

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
	Material_opaco = Material(0.3f, 4);
	Material_finn = Material(0.1f, 60); //Absorbe la luz y refleja un poco [Cartoon]
	Material_neutro = Material(0.0f, 0); 

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

	//Libreria de audio
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	irrklang::ISoundEngine* ambiental = irrklang::createIrrKlangDevice();
	//Sonido ambiente
	ambiental->play2D("Media/background.ogg", true);
	ambiental->setSoundVolume(0.3);

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
		//if (mainWindow.getTipoCamara() != 4) {
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//}
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//150 segundos = 1:30 min
		if (duration <= 10) { //Dia
			mainLight.setLight(glm::vec3(1.0f, 1.0f, 1.0f));
			if (mainWindow.getTipoCamara() != 4) { 
				skyboxDay.DrawSkybox(camera.calculateViewMatrix(false), projection);
			}
			else { //Camara aerea
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
			else { //Camara aerea
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

			if (inicia_caminante) {
				engine->play2D("Media/Duel_of_Fates.ogg");
				inicia_caminante = false;
			}

			//Knuckles
			knuckles_pos = glm::vec3(-116.0f, -2.0f, -6.0f);
			if (num_recta >= 2 and num_recta <= 4) {
				if (knuckles_x > -20) {
					knuckles_x -= offset_knuckles * deltaTime;
				}
				else {
					knuckles_x_ok = true;
				}

				if (rota_knuckles_z > 130) {
					rota_knuckles_z_ok = true;
					knuckles_x_ok = false;
				}

				if (knuckles_x_ok) {
					rota_knuckles_z += offset_rota_knuckles * deltaTime;
				}

				if (knuckles_y > -11 and rota_knuckles_z_ok) {
					knuckles_y -= offset_knuckles * deltaTime;
				}
				else {
					knuckles_y_ok = true;
				}

				//if (knuckles_y_ok) {
				//	printf("T-T");
				//	//if (arriba_knuckes) {
				//	//	knuckles_y += offset_knuckles * deltaTime;
				//	//	if (knuckles_y < -8) {
				//	//		arriba_knuckes = false;
				//	//	}
				//	//}
				//	//else  {
				//	//	knuckles_y -= offset_knuckles * deltaTime;
				//	//	if (knuckles_y < -12) {
				//	//		arriba_knuckes = true;
				//	//	}
				//	//}
				//}
			}
			
			
			//Diosito
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
				offset_caminante = 0.2f;
				mainWindow.setMueveCaminante(0.0f);
				knuckles_pos = glm::vec3(60.0f, 5.0f, 150.0f);
				knuckles_x = 0.0;
				knuckles_x_ok = false;
				knuckles_y = 0.0;
				knuckles_y_ok = false;
				rota_knuckles_z = 0.0;
				rota_knuckles_z_ok = false;
				arriba_knuckes = true;
				offset_knuckles = 0.3;
				offset_rota_knuckles = 2.0;

				if (!inicia_caminante) {
					engine->removeSoundSource("Media/Duel_of_Fates.ogg");
					inicia_caminante = true;
				}

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

		//Animacion en los muñecos de nieve
		if (mainWindow.getMunecos() == 1) {
			inicio_munecos = true;
			//Movimiento
			if (adelante_munecos) {
				if (mueve_x_munecos < 3) {
					mueve_x_munecos += offset_mueve_munecos * deltaTime;
				}
				else {
					adelante_munecos = false;
				}
			}
			else {
				if (mueve_x_munecos > -3) {
					mueve_x_munecos -= offset_mueve_munecos * deltaTime;
				}
				else {
					adelante_munecos = true;
				}
			}
			
			if (pasos < 4) {
				//Rotacion
				if (angulo_z_munecos_ok) {
					rota_z_munecos += offset_rota_munecos * deltaTime;
					if (rota_z_munecos > 30) {
						angulo_z_munecos_ok = false;
						pasos += 1;
					}
				}
				else {
					rota_z_munecos -= offset_rota_munecos * deltaTime;
					if (rota_z_munecos < -5) {
						angulo_z_munecos_ok = true;
					}
				}
			}
			else {
				//Giro completo
				if (pasos == 4) {
					rota_y_munecos += offset_rota_munecos * 2 * deltaTime;
					if (rota_y_munecos > 360) {
						rota_y_munecos = 0;
						pasos = 0;
					}
				}
			}
		}

		//Animacion compleja para rei
		if (mainWindow.getRei() == 1.0f) {
			rota_y_rei += offset_rota_rei * 10 * deltaTime;
			if (angulo_z_rei_ok) {
				rota_z_rei += offset_rota_rei * deltaTime;
				if (rota_z_rei > 35) {
					angulo_z_rei_ok = false;
				}
			}
			else {
				rota_z_rei -= offset_rota_rei * deltaTime;
				if (rota_z_rei < -35) {
					angulo_z_rei_ok = true;
				}
			}

			if (angulo_x_rei_ok) {
				rota_x_rei += offset_rota_rei * deltaTime;
				if (rota_x_rei > 60) {
					angulo_x_rei_ok = false;
				}
			}
			else {
				rota_x_rei -= offset_rota_rei * deltaTime;
				if (rota_x_rei < -60) {
					angulo_x_rei_ok = true;
				}
			}

			num_lem_rei = 15 * sqrt(2) * cos(angulo_lemn);
			den_lem_rei = pow(sin(angulo_lemn), 2) + 1;
			mueve_x_rei = num_lem_rei / den_lem_rei;

			num_lem_rei = 15 * sqrt(2) * cos(angulo_lemn) * sin(angulo_lemn);
			mueve_z_rei = (num_lem_rei / den_lem_rei);

			//angulo_lemn += 0.01;

			angulo_lemn += 0.01 * deltaTime;

		}

		//Animacion para shubba
		if (mainWindow.getRei() == 1.0f) {

			mueve_x_shubba = 20 * sin(angulo_shuba * toRadians);
			mueve_z_shubba = 5 * cos(angulo_shuba * toRadians);
			angulo_shuba += 1.5 * deltaTime;
			
			if (angulo_z_shuba_ok) {
				rota_z_shubba += 5 * deltaTime;
				if (rota_z_shubba > 35) {
					angulo_z_shuba_ok = false;
				}
			}
			else {
				rota_z_shubba -= 5 * deltaTime;
				if (rota_z_shubba < -35) {
					angulo_z_shuba_ok = true;
				}
			}

			if (angulo_x_shuba_ok) {
				rota_x_shubba += 3 * deltaTime;
				if (rota_x_shubba > 50) {
					angulo_x_shuba_ok = false;
				}
			}
			else {
				rota_x_shubba -= 3 * deltaTime;
				if (rota_x_shubba < -50) {
					angulo_x_shuba_ok = true;
				}
			}

			//if (angulo_y_shuba_ok) {
				rota_y_shubba += 1 * deltaTime;
			//	if (rota_y_shubba > 35) {
			//		angulo_y_shuba_ok = false;
			//	}
			//}
			//else {
			//	rota_y_shubba -= 1 * deltaTime;
			//	if (rota_y_shubba < -35) {
			//		angulo_y_shuba_ok = true;
			//	}
			//}
		
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
					engine->play2D("Media/wow.ogg");
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
				rota_avatar_brazos = 0.0f;
			}
			else {  //Se toma la anterior
				camara_x_inicial = cam_entrada_anterior.x;
				camara_z_inicial = cam_entrada_anterior.z;
			}
			nueva_y_camara = -55.0f;
			break;

		case 2: //Nieve
			if (inicio_nieve) { //Se toma la inicial
				camara_x_inicial = cam_nieve_anterior.x = 116.0f;
				camara_z_inicial = cam_nieve_anterior.x = 413.0f;
				inicio_nieve = false;
				rota_avatar_brazos = 0.0f;
			}
			else {  //Se toma la anterior
				camara_x_inicial = cam_nieve_anterior.x;
				camara_z_inicial = cam_nieve_anterior.z;
			}
			//camara_x_inicial = 116.0f;
			//nueva_y_camara = -85.0f;
			//camara_z_inicial = 413.0f;
			nueva_y_camara = -75.0f;
			break;

		case 3: //Arbol
			if (inicio_arbol) { //Se toma la inicial
				camara_x_inicial = cam_arbol_anterior.x = 18.0f;
				camara_z_inicial = cam_arbol_anterior.z = 186.0f;
				inicio_arbol = false;
				rota_avatar_brazos = 0.0f;
			}
			else {  //Se toma la anterior
				camara_x_inicial = cam_arbol_anterior.x;
				camara_z_inicial = cam_arbol_anterior.z;
			}
			nueva_y_camara = 35.0f;
			break;

		case 4: //Aerea

			if (inicio_aerea) { //Se toma la inicial
				camara_x_inicial = cam_aerea_anterior.x = -187.0f;
				camara_z_inicial = cam_aerea_anterior.z = 302.0f;
				inicio_aerea = false;
				rota_avatar_brazos = 0.0f;
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
			//if (inicia_musica) {
			//	engine->play2D("Media/getout.ogg");
			//	inicia_musica = false;
			//}
			
			if (inicia_baile) {
				start_baile = std::clock();
				inicia_baile = false;
				inicia_musica = false;
				engine->play2D("Media/La_Chona.ogg");
			}
			tiempo_baile = (std::clock() - start_baile) / (double)CLOCKS_PER_SEC;
			//printf("Tiempo: %f\n", tiempo_baile);

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
			if (!inicia_musica) {
				engine->removeSoundSource("Media/La_Chona.ogg");
				inicia_musica = true;
			}
		}

		//Animacion del avatar
		//view_camara = camera.getCameraView();
		//numerador_angulo = (view_camara.x * avatar.x) + (view_camara.z * avatar.z);
		//denominador_angulo = sqrt(pow(view_camara.x, 2) + pow(view_camara.z, 2)) 
		//	* sqrt(pow(avatar.x, 2) + pow(avatar.z, 2));
		rotacion_avatar_y = camera.getCameraDirection().z;
		//printf("Rotacion Avatar: %f\n", rotacion_avatar_y);

		if (mainWindow.getMueveW() || mainWindow.getMueveS() || mainWindow.getMueveA() || mainWindow.getMueveD()) { 

			if (mainWindow.getTipoCamara() != 5) { //Esta sobre algun plano
				//Brazos
				if (angulo_z_avatar_ok) {
					rota_avatar_brazos += 4.5 * deltaTime;
					if (rota_avatar_brazos > 75) {
						angulo_z_avatar_ok = false;
					}
				}
				else {
					rota_avatar_brazos -= 4.5 * deltaTime;
					if (rota_avatar_brazos < -75) {
						angulo_z_avatar_ok = true;
					}
				}

				//Piernas
				if (angulo_x_avatar_ok) {
					rota_avatar_piernas += 2.5 * deltaTime;
					if (rota_avatar_piernas > 30) {
						angulo_x_avatar_ok = false;
					}
				}
				else {
					rota_avatar_piernas -= 2.5 * deltaTime;
					if (rota_avatar_piernas < -30) {
						angulo_x_avatar_ok = true;
					}
				}

				//Pies
				if (angulo_x_pies_ok) {
					rota_avatar_pies += 2.5 * deltaTime;
					if (rota_avatar_pies > 0) {
						angulo_x_pies_ok = false;
					}
				}
				else {
					rota_avatar_pies -= 2.5 * deltaTime;
					if (rota_avatar_pies < -25) {
						angulo_x_pies_ok = true;
					}
				}

			}
			else { //Si esta volando
				if (angulo_z_avatar_ok) {
					rota_avatar_brazos += 3 * deltaTime;
					if (rota_avatar_brazos > 30) {
						angulo_z_avatar_ok = false;
					}
				}
				else {
					rota_avatar_brazos -= 3 * deltaTime;
					if (rota_avatar_brazos < -30) {
						angulo_z_avatar_ok = true;
					}
				}
			}

			
		}
		glm::vec3 color = glm::vec3(0.816f, 0.384f, 0.384f);	//ROSA 1
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
		glm::mat4 model_aux_postes(1.0);
		glm::mat4 model_piniata(1.0);


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
		model_aux_postes = glm::mat4(1.0);
		model_piniata = glm::mat4(1.0);

		//Terreno Base
		model = glm::translate(model, glm::vec3(0.0f,  0.5f , -1.5f));
		model_centro_auto = model;
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
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
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(camera.getCameraPosition().x - 20.0f, camera.getCameraPosition().y - 30.0, camera.getCameraPosition().z));
		////model = glm::translate(model, glm::vec3(-177.0f - 40.0f, -65.0f - 30.0, 302.0f));
		//model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		////model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -70 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, rotacion_avatar_y, glm::vec3(0.0f, -1.0f, 0.0f));
		////model = glm::rotate(model, rotacion_avatar_y, glm::vec3(0.0f, 1.0f, 0.0f));
		////model = glm::rotate(model, rotacion_avatar_y, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Finn.RenderModel();

		

		//Knucles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(knuckles_pos.x + knuckles_x, knuckles_pos.y + knuckles_y, knuckles_pos.z) );
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 140 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rota_knuckles_z * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
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
		model = glm::translate(model, glm::vec3(0.0f + mueve_x_rei, 17.0f + mueve_z_rei, 80.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::rotate(model, rota_z_rei * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rota_y_rei * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rota_x_rei * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rei.RenderModel();

		//Shuba
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f + mueve_x_shubba, 8.0f, 130.0f + mueve_z_shubba));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rota_z_shubba * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rota_x_shubba * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, rota_y_shubba * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
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

		//Pan 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, -13.0f, -118.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		//model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pan.RenderModel();

		//Pan 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -13.0f, -158.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		//model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pan.RenderModel();

		//Hombres de nieve //Bernie
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 4.0f + mueve_x_munecos, -200.0f));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rota_z_munecos * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rota_y_munecos * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Snowman.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(67.0f, 108.0f, -75.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		//Hombres de nieve Entrada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f + mueve_x_munecos, -92.0f, 345.0f));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rota_z_munecos * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rota_y_munecos * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Snowman.RenderModel();

		//Hombres de nieve
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -103.0f, 310.0f + mueve_x_munecos));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));
		model = glm::rotate(model, 115 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rota_z_munecos * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rota_y_munecos * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Snowman.RenderModel();



		//Hombres de nieve
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(167.0f, 2.0f + mueve_x_munecos, 42.0f));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));
		model = glm::rotate(model, 95 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rota_z_munecos * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rota_y_munecos * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Snowman.RenderModel();

		////--------------------------------
		////Modelado geometrico
		//model = glm::mat4(1.0f);
		
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
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(96.0f, 60.0f, 83.0f));
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

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(103.0f, 91.0f, 62.0f));
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
		Material_neutro.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//PIEDRA ANIMADA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(168.0f , 4.0f + mueve_y_roca + altura_y_roca, 153.0f + mueve_x_roca));
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rota_y_roca * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rota_z_roca * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Piedra.RenderModel();
		//Esferas
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(45.0f, 131.0f, 73.0f));
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
		model = glm::translate(model, glm::vec3(40.0f, 86.0f, 20.0f));
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
		model = glm::translate(model, glm::vec3(86.0f, 117.0f, 114.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(39.0f, 88.0f, -45.0f));
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
		model = glm::translate(model, glm::vec3(49.0f, 55.0f, -79.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

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
		model = glm::translate(model, glm::vec3(88.0f, 102.0f, 78.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		//-------------
		

		

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

		

		//-------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(63.0f, 93.0f, 89.0f));
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

		//-------------
		
		

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(40.0f, 116.0f, -69.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		

		

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(59.0f, 43.0f, -84.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(77.0f, 84.0f, -52.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		color = glm::vec3(0.47f, 0.47f, 0.47f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		//Finn Jerarquia -------------------------------------

		glm::mat4 matriz_cuerpo(1.0);
		glm::mat4 matriz_pierna(1.0);
		//CUERPO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(camera.getCameraPosition().x - 20.0f, camera.getCameraPosition().y - 30.0, camera.getCameraPosition().z));
		model = glm::rotate(model, -70 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotacion_avatar_y, glm::vec3(0.0f, -1.0f, 0.0f));
		matriz_cuerpo = model;
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_finn.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnCuerpo.RenderModel();
		//BRAZO DERECHO
		model = matriz_cuerpo;
		model = glm::translate(model, glm::vec3(7.5f, 18.65f, -44.9f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		if (mainWindow.getTipoCamara() != 5) {
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		model = glm::rotate(model, rota_avatar_brazos * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FinnBrazoDerecho.RenderModel();
		//BRAZO IZQUIERDO
		model = matriz_cuerpo;
		model = glm::translate(model, glm::vec3(-7.31f, 18.69f, -44.9f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		if (mainWindow.getTipoCamara() != 5) {
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		model = glm::rotate(model, rota_avatar_brazos * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FinnBrazoIzquierdo.RenderModel();
		//BRAZO PIERNA DERECHA
		model = matriz_cuerpo;
		model = glm::translate(model, glm::vec3(2.5f, 10.92f, -45.23f));
		model = glm::rotate(model, rota_avatar_piernas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		matriz_pierna = model;
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FinnPiernaDerecha.RenderModel();
		//PIE DERECHO
		model = matriz_pierna;
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.25f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		model = glm::rotate(model, rota_avatar_pies * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FinnPiernaDerechaBa.RenderModel();

		//PIERNA IZQUIERDA
		model = matriz_cuerpo;
		model = glm::translate(model, glm::vec3(-2.5f, 10.92f, -45.23f));
		model = glm::rotate(model, rota_avatar_piernas * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		matriz_pierna = model;
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FinnPiernaIzquierda.RenderModel();
		//PIE IZQUIERDO
		model = matriz_pierna;
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
		model = glm::rotate(model, rota_avatar_pies * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));;
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FinnPiernaIzquierdaBa.RenderModel();

		

		//PIEDRA NIEVE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(262.0f, -99.0f, 398.0f));
		model = glm::scale(model, glm::vec3(10.0f, 30.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_neutro.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PiedraNieve.RenderModel();

		//--------------------------------
		//MODELADO GEOMETRICO
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

		//Mesa 1

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(72.0f, 12.0f, -133.0f));
		model = glm::scale(model, glm::vec3(3.5f, 12.0f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(52.0f, 12.0f, -133.0f));
		model = glm::scale(model, glm::vec3(3.5f, 12.0f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(72.0f, 12.0f, -103.0f));
		model = glm::scale(model, glm::vec3(3.5f, 12.0f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(52.0f, 12.0f, -103.0f));
		model = glm::scale(model, glm::vec3(3.5f, 12.0f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, 18.0f, -118.0f));
		model = glm::scale(model, glm::vec3(22.0f, 1.0f, 32.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		//-----------------------------------------
		//Mesa 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 12.0f, -173.0f));
		model = glm::scale(model, glm::vec3(3.5f, 12.0f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.0f, 12.0f, -173.0f));
		model = glm::scale(model, glm::vec3(3.5f, 12.0f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, 12.0f, -143.0f));
		model = glm::scale(model, glm::vec3(3.5f, 12.0f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.0f, 12.0f, -143.0f));
		model = glm::scale(model, glm::vec3(3.5f, 12.0f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 18.0f, -158.0f));
		model = glm::scale(model, glm::vec3(24.0f, 1.0f, 34.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		//-----------------------------------------------------
		//MODELADO JERARQUICO

		//Postes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 42.0f, -53.0f));
		model = glm::rotate(model, 25 * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		model_aux_postes = model;
		model = glm::scale(model, glm::vec3(4.5f, 75.0f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(-2.25f - 7.5f, 37.5f, -0.0f));
		model_aux_postes = model;
		model = glm::scale(model, glm::vec3(15.0f, 22.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -0.0f));
		model_aux_postes = model;
		model = glm::scale(model, glm::vec3(15.0f, 22.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -0.0f));
		model_aux_postes = model;
		model = glm::scale(model, glm::vec3(15.0f, 22.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		picos_piniata2.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -0.0f));
		model_aux_postes = model;
		model = glm::scale(model, glm::vec3(15.0f, 22.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -0.0f));
		model_aux_postes = model;
		model = glm::scale(model, glm::vec3(15.0f, 22.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		picos_piniata2.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -0.0f));
		model_aux_postes = model;
		model = glm::scale(model, glm::vec3(15.0f, 22.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -0.0f));
		model_aux_postes = model;
		model = glm::scale(model, glm::vec3(15.0f, 22.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		picos_piniata2.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(-7.5f - 2.25f, -37.5f, 0.0f));
		model_aux_postes = model;
		model = glm::scale(model, glm::vec3(4.5f, 75.0f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[0]->RenderMesh();

		//Piñata

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(137.0f, -10.0f, 362.0f));
		model = glm::rotate(model, -15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(260.0f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		meshListGeo[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(137.0f, -10.0f, 362.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpo_piniata.UseTexture();
		meshListGeo[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(157.0f, -40.0f, 362.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model_aux_postes = model;
		model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //Renderiza esfera

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(0.0f , -8.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 18.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 18.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(0.0f, 8.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 18.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesas.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 8.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 18.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(10.0f, 18.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		picos_piniata2.UseTexture();
		meshListGeo[1]->RenderMesh();

		model = model_aux_postes;
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(10.0f, 18.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		picos_piniata2.UseTexture();
		meshListGeo[1]->RenderMesh();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	engine->drop();
	ambiental->drop();
	return 0;
}
