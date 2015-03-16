#pragma once
#include <Board.h>

#include <iostream>
#include <vector>
using namespace std;

// Include GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <common/texture.hpp>

extern float scaleFactor;

const char * boardFile = "BoardNew.obj";
const char * testFile = "suzanne.obj";
int size;
GLuint textureChecker;
GLuint textureWood;
vector<vector<Piece*>> board(9, vector<Piece*>(9, NULL));

mat4 ScalingMatrix, ModelMatrix;
GLuint vertexbuffer, uvbuffer, normalbuffer;

Board::Board(GLuint vb, GLuint ub, GLuint nb, int s){
	vertexbuffer = vb;
	uvbuffer = ub;
	normalbuffer = nb;
	size = s;
	textureChecker = loadBMP_custom("Chess_Board2.bmp");
	textureWood = loadBMP_custom("Wood.bmp");

	//length *= scaleFactor;
	ScalingMatrix = scale(scaleFactor, scaleFactor, scaleFactor);
	ModelMatrix = ScalingMatrix;
}
void Board::draw(Window window, mat4 ProjectionMatrix, mat4 ViewMatrix)
{
	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glUniformMatrix4fv(window.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(window.ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

	// 1st attribute buffer : verticies
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		window.vertexPosition_modelspaceID,
		3,			// size
		GL_FLOAT,	// type
		GL_FALSE,	// normalized?
		0,			// stride
		(void*)0	// array buffer offset
		);

	// 2nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		window.vertexUVID,
		2,			// size
		GL_FLOAT,	// type
		GL_FALSE,	// normalized?
		0,			// stride
		(void*)0	// array buffer offset
		);

	// 3rd attribute buffer : Normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		window.vertexNormal_modelspaceID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	// Draw the board!

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureChecker);
	// Set our "myTexture Smapler" sample to user Texture 0
	glUniform1i(window.TextureID, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureWood);
	// Set our "myTexture Smapler" sample to user Texture 0
	glUniform1i(window.TextureID, 1);

	glDrawArrays(GL_TRIANGLES, 6, size);
}
void Board::move(int x1, int y1, int x, int y){
	Piece* p = board[x1][y1];
	if (p == NULL)
	{ 
		cout << "No piece there" << endl;
		return; 
	}
	if (p[0].moveTo(x, y, board))
	{
		board[x][y] = p;
		board[x1][y1] = NULL;
	}
}
void Board::place(Piece* p)
{ 
	board[p[0].x][p[0].y] = p; 
}
void Board::rotate(mat4 RotateMatrix){ ModelMatrix = RotateMatrix * ScalingMatrix; }
