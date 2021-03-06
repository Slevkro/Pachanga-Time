#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = muevez = mueve_helices = 2.0f;
	mueve_llanta = mueve_x_pos_chopper = mueve_y_pos_chopper = 0.0f;
	apaga_luz_pinata = false;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci?n de GLFW
	if (!glfwInit())
	{
		printf("Fall? inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama?o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall? inicializaci?n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est? usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//Encender y apagar luz puntual
	if (key == GLFW_KEY_P)
	{
		theWindow->apaga_luz_pinata = false;
	}

	if (key == GLFW_KEY_L)
	{
		theWindow->apaga_luz_pinata = true;
	}

	if (key == GLFW_KEY_J)
	{
		theWindow->mueve_caminante = 1.0f;
	}

	if (key == GLFW_KEY_G)
	{
		theWindow->mueve_nubes = 1.0f;
	}

	if (key == GLFW_KEY_M)
	{
		theWindow->mueve_roca = 1.0f;
	}

	if (key == GLFW_KEY_H)
	{
		theWindow->mueve_caminante = 0.0f;
		theWindow->mueve_nubes = 0.0f;
		theWindow->mueve_rei = 0.0f;
		theWindow->mueve_munecos = 0.0f;
	}

	if (key == GLFW_KEY_Z)
	{
		theWindow->tipo_camara = 1;
		theWindow->cameraFirstChange = true;
	}

	if (key == GLFW_KEY_X)
	{
		theWindow->tipo_camara = 2;
		theWindow->cameraFirstChange = true;
	}

	if (key == GLFW_KEY_C)
	{
		theWindow->tipo_camara = 3;
		theWindow->cameraFirstChange = true;
	}

	if (key == GLFW_KEY_V)
	{
		theWindow->tipo_camara = 4;
		theWindow->cameraFirstChange = true;
	}

	if (key == GLFW_KEY_B)
	{
		theWindow->tipo_camara = 5;
		theWindow->cameraFirstChange = true;
	}

	if (key == GLFW_KEY_K)
	{
		theWindow->mueve_munecos = 1.0;
	}

	if (key == GLFW_KEY_I)
	{
		theWindow->mueve_rei = 1.0;
	}

	if (key == GLFW_KEY_U)
	{
		theWindow->test_jerarquia += 5.0;
	}

	//Avatar
	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS) {
			theWindow->mueve_w = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->mueve_w = false;
		}
	}

	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS) {
			theWindow->mueve_s = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->mueve_s = false;
		}
	}

	if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS) {
			theWindow->mueve_a = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->mueve_a = false;
		}
	}

	if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS) {
			theWindow->mueve_d = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->mueve_d = false;
		}
	}

	if (key == GLFW_KEY_N)
	{
		if (action == GLFW_PRESS) {
			theWindow->baile = 1.0;
			printf("se PRESIONO la tecla xd");
		}
		else if (action == GLFW_RELEASE) {
			theWindow->baile = 0.0;
			printf("se SOLTO la tecla xd");
		}
		
	}


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
