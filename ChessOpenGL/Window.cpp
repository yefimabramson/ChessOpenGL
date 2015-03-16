#pragma once
#include <playground/Window.h>

#include <stdio.h>
#include <GL/glew.h>
#include <glfw3.h>

#include <common/shader.hpp>
#include <common/text2D.hpp>

extern GLFWwindow* glfwwindow;

GLuint programID;
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;
GLuint vertexPosition_modelspaceID;
GLuint vertexUVID;
GLuint vertexNormal_modelspaceID;
GLuint TextureID;
GLuint LightID;

int winX = 800;
int winY = 600;

Window::Window(){
	// Initialise GLFW
	if (!glfwInit())
	{
		throw "Failed to initialize GLFW";
	}
	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	printf("%i major, %i minor, %i rev\n", major, minor, rev);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	glfwwindow = glfwCreateWindow(winX, winY, "ChessGame", NULL, NULL);
	if (glfwwindow == NULL){
		glfwTerminate();
		throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.";
	}
	glfwMakeContextCurrent(glfwwindow);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		throw "Failed to initialize GLEW";
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(glfwwindow, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");

	// Get a handle for our buffers
	vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	vertexUVID = glGetAttribLocation(programID, "vertexUV");
	vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

	// Get a handle for our "myTexture Sampler" uniform
	TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Get a handle for our "LightPosistion" Uniform
	glUseProgram(programID);
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	initText2D("Holstein.DDS");
}