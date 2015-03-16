#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <glfw3.h>

class Window {
public:
	GLuint programID;
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint vertexPosition_modelspaceID;
	GLuint vertexUVID;
	GLuint vertexNormal_modelspaceID;
	GLuint TextureID;
	GLuint LightID;
	Window();
};

#endif