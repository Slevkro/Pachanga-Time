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
Model Nubes;


Model Jake;

Skybox skyboxNight;
Skybox skyboxDay;

//materiales
Material Material_brillante;
Material Material_opaco;

//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];


//Sphere cabeza = Sphere(0.5, 20, 20);
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
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};


	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
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
	Terreno_Base.LoadModel("Models/terreno-arbol.obj");

	Piedra = Model();
	Piedra.LoadModel("Models/piedra-animada.obj");

	Nubes = Model();
	Nubes.LoadModel("Models/clouds.obj");

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
	Lago.LoadModel("Models/lago.obj");

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

	//posición inicial del helicóptero
	glm::vec3 posblackhawk = glm::vec3(-20.0f, 6.0f, -1.0);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
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
		0.0f, 10.0f,
		168.0f, -15.0f, 286.0f, //POSICION
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna que pasa a ser para el helicoptero
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	

	//Luz auto derecha
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//Luz auto izquierda
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	
	//luz fija
	//spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
	//	1.0f, 2.0f,
	//	5.0f, 10.0f, 0.0f,
	//	0.0f, -5.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	15.0f);
	//spotLightCount++;

	//luz de helicóptero
		
	//luz de faro

	

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	//Ciclo de dia y noche 
	start = std::clock();
	
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
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//150 segundos = 1:30 min
		if (duration <= 90) {
			skyboxDay.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else {
			skyboxNight.DrawSkybox(camera.calculateViewMatrix(), projection);
			if (duration >= 180) {
				start = std::clock();
			}
		}
		
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		printf("x: %f, y: %f, z: %f \n", camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//luz ligada a la cámara de tipo flash 
		//glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		if (mainWindow.getapaga_luz_pinata()) {
			shaderList[0].SetPointLights(pointLights, pointLightCount -1);
		}
		else {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
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
		model = glm::translate(model, glm::vec3(200.0f, -15.0f, 210.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Among_Us.RenderModel();

		//Knucles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 5.0f, 150.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 140 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Knuckles.RenderModel();

		//Pista de baile
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 106.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		model = glm::rotate(model, 3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dance_Floor.RenderModel();

		//Lago
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-135.0f, -21.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lago.RenderModel();

		//Jesus
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-250.0f, -15.0f, -50.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1, 0.1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Jesus.RenderModel();

		//Rei
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 10.0f, 70.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rei.RenderModel();

		//Shuba
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 10.0f, 120.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Shuba.RenderModel();

		//Marceline entrada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, -30.0f, 280.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Marceline.RenderModel();

		//Cheems fuerte Piñata
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, -85.0f, 370.0f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		//model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cheems.RenderModel();

		//Cheems chiquito Piñata
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, -110.0f, 420.0f));
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

		//Nubes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nubes.RenderModel();

		//PIEDRA ANIMADA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Piedra.RenderModel();


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
