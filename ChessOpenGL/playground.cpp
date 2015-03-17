#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* glfwwindow;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/controls.hpp"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"
#include "common/text2D.hpp"

#include "Window.h"
#include "Board.h"
#include "Piece.h"

float scaleFactor = 0.0625f;
float squareLength;
float squareRadius;
float piecescalefactor; 
mat4 pieceScalingMatrix;
float startX; // 0,0
float startY;
int moveInput[] = { 0,0,0,0 };

void printMove()
{
	stringstream sstm;
	string toPrint = "";
	if (moveInput[0] != 0)
	{
		sstm << (char)(moveInput[0] + 'A' - 1);
		if (moveInput[1] != 0)
		{
			sstm << (char)(moveInput[1] + '1' - 1) << "->";
			if (moveInput[2] != 0)
			{
				sstm << (char)(moveInput[2] + 'A' - 1);
				if (moveInput[3] != 0)
				{
					sstm << (char)(moveInput[3] + '1' - 1);
				}
			}
		}
	}
	else
	{
		sstm << " ";
	}
	toPrint = sstm.str();
	//string str(temp);
	//const char* toprint = str.c_str();
	printText2D(toPrint.c_str(), 0, 10, 20);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS){
		if (moveInput[0] == 0){ if ('A' <= key && key <= 'H'){ moveInput[0] = key - 'A' + 1; } }
		else if (moveInput[1] == 0){ if ('1' <= key && key <= '8'){ moveInput[1] = key - '1' + 1; } }
		else if (moveInput[2] == 0){ if ('A' <= key && key <= 'H'){ moveInput[2] = key - 'A' + 1; } }
		else if (moveInput[3] == 0){ if ('1' <= key && key <= '8'){ moveInput[3] = key - '1' + 1; } }
	}
}

int main(void)
{
	Window window = Window();

	// Load the texture
	GLuint textureWhitePiece = loadBMP_custom("Wood3.bmp");
	GLuint textureBlackPiece = loadBMP_custom("Wood2.bmp");
	//GLuint Texture = loadDDS("uvmap.DDS");

	// Read our .obj file
	vector<vec3> boardvertices;
	vector<vec2> boarduvs;
	vector<vec3> boardnormals;
	float boardlength;
	loadOBJ("BoardNew.obj", boardvertices, boarduvs, boardnormals, boardlength);
	boardlength = fabs(boardvertices[0].x - boardvertices[1].x);

	vector<vec3> kingvertices;
	vector<vec2> kinguvs;
	vector<vec3> kingnormals;
	float kingradius = 0;
	loadOBJ("KingNew.obj", kingvertices, kinguvs, kingnormals, kingradius);

	vector<vec3> queenvertices;
	vector<vec2> queenuvs;
	vector<vec3> queennormals;
	float queenradius = 0;
	loadOBJ("QueenNew.obj", queenvertices, queenuvs, queennormals, queenradius);

	vector<vec3> bishopvertices;
	vector<vec2> bishopuvs;
	vector<vec3> bishopnormals;
	float bishopradius = 0;
	loadOBJ("BishopNew.obj", bishopvertices, bishopuvs, bishopnormals, bishopradius);

	vector<vec3> knightvertices;
	vector<vec2> knightuvs;
	vector<vec3> knightnormals;
	float knightradius = 0;
	loadOBJ("KnightNew.obj", knightvertices, knightuvs, knightnormals, knightradius);

	vector<vec3> rookvertices;
	vector<vec2> rookuvs;
	vector<vec3> rooknormals;
	float rookradius = 0;
	loadOBJ("RookNew.obj", rookvertices, rookuvs, rooknormals, rookradius);

	vector<vec3> pawnvertices;
	vector<vec2> pawnuvs;
	vector<vec3> pawnnormals;
	float pawnradius = 0;
	loadOBJ("PawnNew.obj", pawnvertices, pawnuvs, pawnnormals, pawnradius);

	glfwSetScrollCallback(glfwwindow, ScrollCallback);
	glfwSetKeyCallback(glfwwindow, keyboardCallback);

	// Load it into a VBO

	/*****************************************************************************************************/

	GLuint boardvertexbuffer;
	glGenBuffers(1, &boardvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, boardvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, boardvertices.size() * sizeof(vec3), &boardvertices[0], GL_STATIC_DRAW);

	GLuint boarduvbuffer;
	glGenBuffers(1, &boarduvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, boarduvbuffer);
	glBufferData(GL_ARRAY_BUFFER, boarduvs.size() * sizeof(vec2), &boarduvs[0], GL_STATIC_DRAW);

	GLuint boardnormalbuffer;
	glGenBuffers(1, &boardnormalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, boardnormalbuffer);
	glBufferData(GL_ARRAY_BUFFER, boardnormals.size() * sizeof(vec3), &boardnormals[0], GL_STATIC_DRAW);

	/*****************************************************************************************************/

	GLuint kingvertexbuffer;
	glGenBuffers(1, &kingvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, kingvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, kingvertices.size() * sizeof(vec3), &kingvertices[0], GL_STATIC_DRAW);

	GLuint kinguvbuffer;
	glGenBuffers(1, &kinguvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, kinguvbuffer);
	glBufferData(GL_ARRAY_BUFFER, kinguvs.size() * sizeof(vec2), &kinguvs[0], GL_STATIC_DRAW);

	GLuint kingnormalbuffer;
	glGenBuffers(1, &kingnormalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, kingnormalbuffer);
	glBufferData(GL_ARRAY_BUFFER, kingnormals.size() * sizeof(vec3), &kingnormals[0], GL_STATIC_DRAW);

	/*****************************************************************************************************/

	GLuint queenvertexbuffer;
	glGenBuffers(1, &queenvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, queenvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, queenvertices.size() * sizeof(vec3), &queenvertices[0], GL_STATIC_DRAW);

	GLuint queenuvbuffer;
	glGenBuffers(1, &queenuvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, queenuvbuffer);
	glBufferData(GL_ARRAY_BUFFER, queenuvs.size() * sizeof(vec2), &queenuvs[0], GL_STATIC_DRAW);

	GLuint queennormalbuffer;
	glGenBuffers(1, &queennormalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, queennormalbuffer);
	glBufferData(GL_ARRAY_BUFFER, queennormals.size() * sizeof(vec3), &queennormals[0], GL_STATIC_DRAW);

	/*****************************************************************************************************/

	GLuint bishopvertexbuffer;
	glGenBuffers(1, &bishopvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bishopvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, bishopvertices.size() * sizeof(vec3), &bishopvertices[0], GL_STATIC_DRAW);

	GLuint bishopuvbuffer;
	glGenBuffers(1, &bishopuvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bishopuvbuffer);
	glBufferData(GL_ARRAY_BUFFER, bishopuvs.size() * sizeof(vec2), &bishopuvs[0], GL_STATIC_DRAW);

	GLuint bishopnormalbuffer;
	glGenBuffers(1, &bishopnormalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bishopnormalbuffer);
	glBufferData(GL_ARRAY_BUFFER, bishopnormals.size() * sizeof(vec3), &bishopnormals[0], GL_STATIC_DRAW);

	/*****************************************************************************************************/

	GLuint knightvertexbuffer;
	glGenBuffers(1, &knightvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, knightvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, knightvertices.size() * sizeof(vec3), &knightvertices[0], GL_STATIC_DRAW);

	GLuint knightuvbuffer;
	glGenBuffers(1, &knightuvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, knightuvbuffer);
	glBufferData(GL_ARRAY_BUFFER, knightuvs.size() * sizeof(vec2), &knightuvs[0], GL_STATIC_DRAW);

	GLuint knightnormalbuffer;
	glGenBuffers(1, &knightnormalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, knightnormalbuffer);
	glBufferData(GL_ARRAY_BUFFER, knightnormals.size() * sizeof(vec3), &knightnormals[0], GL_STATIC_DRAW);

	/*****************************************************************************************************/

	GLuint rookvertexbuffer;
	glGenBuffers(1, &rookvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rookvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, rookvertices.size() * sizeof(vec3), &rookvertices[0], GL_STATIC_DRAW);

	GLuint rookuvbuffer;
	glGenBuffers(1, &rookuvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rookuvbuffer);
	glBufferData(GL_ARRAY_BUFFER, rookuvs.size() * sizeof(vec2), &rookuvs[0], GL_STATIC_DRAW);

	GLuint rooknormalbuffer;
	glGenBuffers(1, &rooknormalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rooknormalbuffer);
	glBufferData(GL_ARRAY_BUFFER, rooknormals.size() * sizeof(vec3), &rooknormals[0], GL_STATIC_DRAW);

	/*****************************************************************************************************/

	GLuint pawnvertexbuffer;
	glGenBuffers(1, &pawnvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pawnvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, pawnvertices.size() * sizeof(vec3), &pawnvertices[0], GL_STATIC_DRAW);

	GLuint pawnuvbuffer;
	glGenBuffers(1, &pawnuvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pawnuvbuffer);
	glBufferData(GL_ARRAY_BUFFER, pawnuvs.size() * sizeof(vec2), &pawnuvs[0], GL_STATIC_DRAW);

	GLuint pawnnormalbuffer;
	glGenBuffers(1, &pawnnormalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pawnnormalbuffer);
	glBufferData(GL_ARRAY_BUFFER, pawnnormals.size() * sizeof(vec3), &pawnnormals[0], GL_STATIC_DRAW);

	boardlength *= scaleFactor;
	squareLength = boardlength / 8;
	squareRadius = squareLength / 2;
	piecescalefactor = squareRadius / kingradius;
	piecescalefactor *= .90f;
	pieceScalingMatrix = scale(mat4(1.0f), vec3(piecescalefactor, piecescalefactor, piecescalefactor));
	startX = squareRadius - 4 * squareLength;
	startY = -squareRadius + 4 * squareLength;

	mat4 boardScalingMatrix = scale(mat4(1.0f), vec3(scaleFactor, scaleFactor, scaleFactor));
	mat4 boardModelMatrix = boardScalingMatrix;

	Board board = Board(boardvertexbuffer, boarduvbuffer, boardnormalbuffer, boardvertices.size());

	Rook   whiterook1 = Rook(1, 1, rookvertexbuffer, rookuvbuffer, rooknormalbuffer, rookvertices.size(), 'W');
	Knight whiteknight1 = Knight(2, 1, knightvertexbuffer, knightuvbuffer, knightnormalbuffer, knightvertices.size(), 'W');
	Bishop whitebishop1 = Bishop(3, 1, bishopvertexbuffer, bishopuvbuffer, bishopnormalbuffer, bishopvertices.size(), 'W');
	Queen  whitequeen = Queen(4, 1, queenvertexbuffer, queenuvbuffer, queennormalbuffer, queenvertices.size(), 'W');
	King   whiteking = King(5, 1, kingvertexbuffer, kinguvbuffer, kingnormalbuffer, kingvertices.size(), 'W');
	Bishop whitebishop2 = Bishop(6, 1, bishopvertexbuffer, bishopuvbuffer, bishopnormalbuffer, bishopvertices.size(), 'W');
	Knight whiteknight2 = Knight(7, 1, knightvertexbuffer, knightuvbuffer, knightnormalbuffer, knightvertices.size(), 'W');
	Rook   whiterook2 = Rook(8, 1, rookvertexbuffer, rookuvbuffer, rooknormalbuffer, rookvertices.size(), 'W');
	Pawn   whitepawn1 = Pawn(1, 2, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'W');
	Pawn   whitepawn2 = Pawn(2, 2, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'W');
	Pawn   whitepawn3 = Pawn(3, 2, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'W');
	Pawn   whitepawn4 = Pawn(4, 2, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'W');
	Pawn   whitepawn5 = Pawn(5, 2, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'W');
	Pawn   whitepawn6 = Pawn(6, 2, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'W');
	Pawn   whitepawn7 = Pawn(7, 2, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'W');
	Pawn   whitepawn8 = Pawn(8, 2, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'W');

	/*************************************************************************************************************************/

	Rook   blackrook1 = Rook(1, 8, rookvertexbuffer, rookuvbuffer, rooknormalbuffer, rookvertices.size(), 'B');
	Knight blackknight1 = Knight(2, 8, knightvertexbuffer, knightuvbuffer, knightnormalbuffer, knightvertices.size(), 'B');
	Bishop blackbishop1 = Bishop(3, 8, bishopvertexbuffer, bishopuvbuffer, bishopnormalbuffer, bishopvertices.size(), 'B');
	Queen  blackqueen = Queen(4, 8, queenvertexbuffer, queenuvbuffer, queennormalbuffer, queenvertices.size(), 'B');
	King   blackking = King(5, 8, kingvertexbuffer, kinguvbuffer, kingnormalbuffer, kingvertices.size(), 'B');
	Bishop blackbishop2 = Bishop(6, 8, bishopvertexbuffer, bishopuvbuffer, bishopnormalbuffer, bishopvertices.size(), 'B');
	Knight blackknight2 = Knight(7, 8, knightvertexbuffer, knightuvbuffer, knightnormalbuffer, knightvertices.size(), 'B');
	Rook   blackrook2 = Rook(8, 8, rookvertexbuffer, rookuvbuffer, rooknormalbuffer, rookvertices.size(), 'B');
	Pawn   blackpawn1 = Pawn(1, 7, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'B');
	Pawn   blackpawn2 = Pawn(2, 7, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'B');
	Pawn   blackpawn3 = Pawn(3, 7, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'B');
	Pawn   blackpawn4 = Pawn(4, 7, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'B');
	Pawn   blackpawn5 = Pawn(5, 7, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'B');
	Pawn   blackpawn6 = Pawn(6, 7, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'B');
	Pawn   blackpawn7 = Pawn(7, 7, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'B');
	Pawn   blackpawn8 = Pawn(8, 7, pawnvertexbuffer, pawnuvbuffer, pawnnormalbuffer, pawnvertices.size(), 'B');

	Piece* whitepieces[16] = { &whiterook1, &whiteknight1, &whitebishop1, &whitequeen, &whiteking, &whitebishop2, &whiteknight2, &whiterook2,
		&whitepawn1, &whitepawn2, &whitepawn3, &whitepawn4, &whitepawn5, &whitepawn6, &whitepawn7, &whitepawn8 };
	Piece* blackpieces[16] = { &blackrook1, &blackknight1, &blackbishop1, &blackqueen, &blackking, &blackbishop2, &blackknight2, &blackrook2,
		&blackpawn1, &blackpawn2, &blackpawn3, &blackpawn4, &blackpawn5, &blackpawn6, &blackpawn7, &blackpawn8 };

	for (Piece* piece : whitepieces){ board.place(piece); }
	for (Piece* piece : blackpieces){ board.place(piece); }

	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// use our shader
		glUseProgram(window.programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		mat4 ProjectionMatrix = getProjectionMatrix();
		mat4 ViewMatrix = getViewMatrix();
		mat4 RotationMatrix = getRotationMatrix();

		vec3 lightPos = vec3(0, 10, 10);
		glUniform3f(window.LightID, lightPos.x, lightPos.y, lightPos.z);

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(window.ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glEnableVertexAttribArray(window.vertexPosition_modelspaceID);
		glEnableVertexAttribArray(window.vertexUVID);
		glEnableVertexAttribArray(window.vertexNormal_modelspaceID);

		//Board

		board.rotate(RotationMatrix);

		board.draw(window, ProjectionMatrix, ViewMatrix);

		// White Pieces

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWhitePiece);
		// Set our "myTexture Smapler" sample to user Texture 0
		glUniform1i(window.TextureID, 0);

		for (Piece* p : whitepieces)
		{ 
			p[0].rotate(RotationMatrix);
			p[0].draw(window, ProjectionMatrix, ViewMatrix); 
		}

		/***************************************************************************************************************************/

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBlackPiece);
		// Set our "myTexture Smapler" sample to user Texture 0
		glUniform1i(window.TextureID, 0);

		for (Piece* p : blackpieces)
		{ 
			p[0].rotate(RotationMatrix);
			p[0].draw(window, ProjectionMatrix, ViewMatrix); 
		}

		glDisableVertexAttribArray(window.vertexPosition_modelspaceID);
		glDisableVertexAttribArray(window.vertexUVID);
		glDisableVertexAttribArray(window.vertexNormal_modelspaceID);

		if (moveInput[3] != 0)
		{
			board.move(moveInput[0], moveInput[1], moveInput[2], moveInput[3]);
			cout << (char)(moveInput[0] + 'A' - 1) << moveInput[1] << "->" << (char)(moveInput[2] + 'A' - 1) << moveInput[3] << endl;
			moveInput[0] = 0;
			moveInput[1] = 0;
			moveInput[2] = 0;
			moveInput[3] = 0;
		}

		printText2D("Version 0.6.0", 0, 0, 10);
		printMove();

		// Swap buffers
		glfwSwapBuffers(glfwwindow);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(glfwwindow, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(glfwwindow) == 0);
	
	cleanupText2D();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

