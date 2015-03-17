#ifndef BOARD_H
#define BOARD_H

#include "Window.h"
#include "Piece.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace std;
using namespace glm;


class Board{
public:
	Board(GLuint vb, GLuint ub, GLuint nb, int s);
	void draw(Window window, mat4 ProjectionMatrix, mat4 ViewMatrix);
	void move(int x1, int y1, int x, int y);
	void place(Piece* p);
	void rotate(mat4 RotateMatrix);
};

#endif
