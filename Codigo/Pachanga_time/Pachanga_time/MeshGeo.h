#pragma once
#include<glew.h>
class MeshGeo
{
public:
	MeshGeo();
	void CreateMesh(GLfloat *vertices,unsigned int *indices, unsigned int numOfVertices, unsigned int numberOfIndices);
	void RenderMesh();
	void RenderMeshGeometry();
	void ClearMesh();
	~MeshGeo();
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

};
class MeshColor
{
public:
	MeshColor();
	void CreateMeshColor(GLfloat *vertices, unsigned int numOfVertices);
	void RenderMeshColor();
	void ClearMeshColor();
	~MeshColor();
private:
	GLuint VAO, VBO;
	GLsizei  vertexCount;
};

