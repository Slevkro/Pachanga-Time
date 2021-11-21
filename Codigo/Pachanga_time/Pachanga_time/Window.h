#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevez() { return muevez; }
	bool getapaga_luz_pinata() { return apaga_luz_pinata; }
	GLfloat getmueve_llanta() { return mueve_llanta; }
	GLfloat getmueve_helices() { return mueve_helices; }
	GLfloat getmueve_x_pos_chopper() { return mueve_x_pos_chopper; }
	GLfloat getmueve_y_pos_chopper() { return mueve_y_pos_chopper; }
	GLfloat getMueveCaminante() { return mueve_caminante; }
	void setMueveCaminante(GLfloat movimiento) { mueve_caminante = movimiento; }
	GLfloat getMueveNubes() { return mueve_nubes; }
	void setMueveNubes(GLfloat inicio) { mueve_nubes = inicio; }
	GLfloat getMueveRoca() { return mueve_roca; }
	GLfloat getBaile() { return baile; }

	GLfloat getMunecos() { return mueve_munecos; }
	GLfloat getRei() { return mueve_rei; }

	int getTipoCamara() { return tipo_camara; }
	void setTipoCamara(int tipo) { tipo_camara = tipo; }
	bool getCameraFirstChange() { return cameraFirstChange; }
	void setCameraFirstChange(bool change) { cameraFirstChange = change; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool apaga_luz_pinata;
	GLfloat muevex, muevez, mueve_helices;
	GLfloat mueve_llanta, mueve_x_pos_chopper, mueve_y_pos_chopper;
	GLfloat mueve_caminante = 0.0f;
	GLfloat mueve_nubes = 0.0f;
	GLfloat mueve_roca = 0.0f;
	GLfloat baile = 0.0f;
	GLfloat mueve_munecos = 0.0f;
	GLfloat mueve_rei = 0.0f;
	int tipo_camara = 1;
	bool mouseFirstMoved;
	bool cameraFirstChange;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

