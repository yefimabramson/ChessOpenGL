#pragma once
#include <Piece.h>

#include <iostream>
using namespace std;

// Include GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

//#include <playground/Window.h>

extern GLFWwindow* glfwwindow;
extern float startX;
extern float startY;
extern float squareLength;
extern mat4 pieceScalingMatrix;

mat4 whiteknightRotationMatrix = rotate(mat4(1.0f), -90.0f, vec3(0, 1, 0));
mat4 blackknightRotationMatrix = rotate(mat4(1.0f), 90.0f, vec3(0, 1, 0));


void addMove(vector<vector<int>> &moves, vector<int> move)
{
	moves.reserve(moves.size() + 1);
	moves.resize(moves.size() + 1);
	moves[moves.size() - 1] = move;
}

mat4 getTranslationMatrix(int x, int y)
{
	return translate(startX + (x - 1) * squareLength, 0.0001f, startY - (y - 1) * squareLength);
}

Piece::Piece(int x, int y){ this->x = x; this->y = y; }

Piece::Piece(int xpos, int ypos, GLuint vb, GLuint ub, GLuint nb, int s, char c)
{
	vertexbuffer = vb;
	uvbuffer = ub;
	normalbuffer = nb;
	size = s;
	color = c;
	x = xpos;
	y = ypos;
	TranslationMatrix = getTranslationMatrix(x, y);
	ModelMatrix = TranslationMatrix * pieceScalingMatrix;
}
King::King(int x, int y, GLuint vb, GLuint ub, GLuint nb, int s, char c) : Piece(x, y, vb, ub, nb, s, c){}
Queen::Queen(int x, int y, GLuint vb, GLuint ub, GLuint nb, int s, char c) : Piece(x, y, vb, ub, nb, s, c){}
Bishop::Bishop(int x, int y, GLuint vb, GLuint ub, GLuint nb, int s, char c) : Piece(x, y, vb, ub, nb, s, c){}
Knight::Knight(int x, int y, GLuint vb, GLuint ub, GLuint nb, int s, char c) : Piece(x, y, vb, ub, nb, s, c)
{
	RotationMatrix = c == 'W' || c == 'w' ? whiteknightRotationMatrix : c == 'B' || c == 'b' ? blackknightRotationMatrix : mat4(1.0f);
	ModelMatrix = TranslationMatrix * RotationMatrix * pieceScalingMatrix;
}
Rook::Rook(int x, int y, GLuint vb, GLuint ub, GLuint nb, int s, char c) : Piece(x, y, vb, ub, nb, s, c){}
Pawn::Pawn(int x, int y, GLuint vb, GLuint ub, GLuint nb, int s, char c) : Piece(x, y, vb, ub, nb, s, c){}

void Piece::draw(Window window, mat4 ProjectionMatrix, mat4 ViewMatrix)
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
	glDrawArrays(GL_TRIANGLES, 0, size);
}

bool Piece::moveTo(int x, int y, vector<vector<Piece*>> board)
{
	getMoves(board);
	if (find(moves.begin(), moves.end(), vector<int>{ x, y }) != moves.end())
	{
		this->x = x;
		this->y = y;
		TranslationMatrix = getTranslationMatrix(x, y);
		ModelMatrix = TranslationMatrix * pieceScalingMatrix;
		cout << "Successful move to location" << endl;
		return true;
	}
	if (find(canEat.begin(), canEat.end(), vector<int>{ x, y }) != canEat.end())
	{
		board[x][y][0].eat();
		this->x = x;
		this->y = y;
		TranslationMatrix = getTranslationMatrix(x, y);
		ModelMatrix = TranslationMatrix * pieceScalingMatrix;
		cout << "Successful piece taken" << endl;
		return true;
	}
	else
	{
		cout << "Cannot move to location" << endl;
		return false;
	}
}
bool Pawn::moveTo(int x, int y, vector<vector<Piece*>> board)
{
	bool moved = Piece::moveTo(x, y, board);
	if (moved){ firstMove = false; }
	return moved;
}

void Piece::rotate(mat4 RotationMatrix){
	ModelMatrix = RotationMatrix * TranslationMatrix * pieceScalingMatrix;
}
void Knight::rotate(mat4 RotationMatrix)
{
	ModelMatrix = RotationMatrix * TranslationMatrix * this->RotationMatrix * pieceScalingMatrix;
}

//void Piece::getMoves(vector<vector<Piece*>> board){}
void King::getMoves(vector<vector<Piece*>> board)
{
	moves.clear();
	if (x < 9 && board[x + 1][y] == NULL) { addMove(moves, { x + 1, y }); }
	else if (x < 9 && board[x + 1][y][0].color != color) { addMove(canEat, { x + 1, y }); }
	if (x < 9 && y < 9 && board[x + 1][y + 1] == NULL) { addMove(moves, { x + 1, y + 1 }); }
	else if (x < 9 && y < 9 && board[x + 1][y + 1][0].color != color) { addMove(canEat, { x + 1, y + 1 }); }
	if (y < 9 && board[x][y + 1] == NULL) { addMove(moves, { x, y + 1 }); }
	else if (y < 9 && board[x][y + 1][0].color != color) { addMove(canEat, { x, y + 1 }); }
	if (x > 0 && y < 9 && board[x - 1][y + 1] == NULL) { addMove(moves, { x - 1, y + 1 }); }
	else if (x > 0 && y < 9 && board[x - 1][y + 1][0].color != color) { addMove(canEat, { x - 1, y + 1 }); }
	if (x > 0 && board[x - 1][y] == NULL) { addMove(moves, { x - 1, y }); }
	else if (x > 0 && board[x - 1][y][0].color != color) { addMove(canEat, { x - 1, y }); }
	if (x > 0 && y > 0 && board[x - 1][y - 1] == NULL) { addMove(moves, { x - 1, y - 1 }); }
	else if (x > 0 && y > 0 && board[x - 1][y - 1][0].color != color) { addMove(canEat, { x - 1, y - 1 }); }
	if (y > 0 && board[x][y - 1] == NULL) { addMove(moves, { x, y - 1 }); }
	else if (y > 0 && board[x][y - 1][0].color != color) { addMove(canEat, { x, y - 1 }); }
	if (x < 9 && y > 0 && board[x + 1][y - 1] == NULL) { addMove(moves, { x + 1, y - 1 }); }
	else if (x < 9 && y > 0 && board[x + 1][y - 1][0].color != color) { addMove(canEat, { x + 1, y - 1 }); }
	//for (vector<int> move : moves){ for (int place : move){ cout << place; } cout << endl; }
}
void Queen::getMoves(vector<vector<Piece*>> board)
{
	moves.clear();
	for (int x1 = x - 1; x1 > 1; x1--)
	{
		if (board[x1][y] == NULL){ addMove(moves, { x1, y }); }
		else
		{
			if (board[x1][y][0].color != color){ addMove(canEat, { x1, y }); }
			break;
		}
	}
	for (int x2 = x + 1; x2 < 9; x2++)
	{
		if (board[x2][y] == NULL){ addMove(moves, { x2, y }); }
		else
		{
			if (board[x2][y][0].color != color){ addMove(canEat, { x2, y }); }
			break;
		}
	}
	for (int y1 = y - 1; y1 > 1; y1--)
	{
		if (board[x][y1] == NULL){ addMove(moves, { x, y1 }); }
		else
		{
			if (board[x][y1][0].color != color){ addMove(canEat, { x, y1 }); }
			break;
		}
	}
	for (int y2 = y + 1; y2 < 9; y2++)
	{
		if (board[x][y2] == NULL){ addMove(moves, { x, y2 }); }
		else
		{
			if (board[x][y2][0].color != color){ addMove(canEat, { x, y2 }); }
			break;
		}
	}

	for (int count1 = 1; count1 < x && count1 < y; count1++)
	{
		if (board[x - count1][y - count1] == NULL) { addMove(moves, { x - count1, y - count1 }); }
		else
		{
			if (board[x - count1][y - count1][0].color != color){ addMove(canEat, { x - count1, y - count1 }); }
			break;
		}
	}
	for (int count2 = 1; count2 < x && count2 + y < 9; count2++)
	{
		if (board[x - count2][y + count2] == NULL) { addMove(moves, { x - count2, y + count2 }); }
		else
		{
			if (board[x - count2][y + count2][0].color != color){ addMove(canEat, { x - count2, y + count2 }); }
			break;
		}
	}
	for (int count3 = 1; count3 + x < 9 && count3 < y; count3++)
	{
		if (board[x + count3][y - count3] == NULL) { addMove(moves, { x + count3, y - count3 }); }
		else
		{
			if (board[x + count3][y - count3][0].color != color){ addMove(canEat, { x + count3, y - count3 }); }
			break;
		}
	}
	for (int count4 = 1; count4 + x < 9 && count4 + y < 9; count4++)
	{
		if (board[x + count4][y + count4] == NULL) { addMove(moves, { x + count4, y + count4 }); }
		else
		{
			if (board[x + count4][y + count4][0].color != color){ addMove(canEat, { x + count4, y + count4 }); }
			break;
		}
	}
	//for (vector<int> move : moves){ for (int place : move){ cout << place; } cout << endl; }
}
void Bishop::getMoves(vector<vector<Piece*>> board)
{
	moves.clear();
	for (int count1 = 1; count1 < x && count1 < y; count1++)
	{
		if (board[x - count1][y - count1] == NULL) { addMove(moves, { x - count1, y - count1 }); }
		else 
		{
			if (board[x - count1][y - count1][0].color != color){ addMove(canEat, { x - count1, y - count1 }); }
			break;
		}
	}
	for (int count2 = 1; count2 < x && count2 + y < 9; count2++)
	{
		if (board[x - count2][y + count2] == NULL) { addMove(moves, { x - count2, y + count2 }); }
		else
		{
			if (board[x - count2][y + count2][0].color != color){ addMove(canEat, { x - count2, y + count2 }); }
			break;
		}
	}
	for (int count3 = 1; count3 + x < 9 && count3 < y; count3++)
	{
		if (board[x + count3][y - count3] == NULL) { addMove(moves, { x + count3, y - count3 }); }
		else
		{
			if (board[x + count3][y - count3][0].color != color){ addMove(canEat, { x + count3, y - count3 }); }
			break;
		}
	}
	for (int count4 = 1; count4 + x < 9 && count4 + y < 9; count4++)
	{
		if (board[x + count4][y + count4] == NULL) { addMove(moves, { x + count4, y + count4 }); }
		else
		{
			if (board[x + count4][y + count4][0].color != color){ addMove(canEat, { x + count4, y + count4 }); }
			break;
		}
	}
	//for (vector<int> move : moves){ for (int place : move){ cout << place; } cout << endl; }
}
void Knight::getMoves(vector<vector<Piece*>> board)
{
	moves.clear();
	if (x < 9 && y < 8 && board[x + 1][y + 2] == NULL){ addMove(moves, { x + 1, y + 2 }); }
	else if (x < 9 && y < 8 && board[x + 1][y + 2][0].color != color){ addMove(canEat, { x + 1, y + 2 }); }
	if (x < 8 && y < 9 && board[x + 2][y + 1] == NULL){ addMove(moves, { x + 2, y + 1 }); }
	else if (x < 8 && y < 9 && board[x + 2][y + 1][0].color != color){ addMove(canEat, { x + 2, y + 1 }); }
	if (x < 9 && y > 2 && board[x + 1][y - 2] == NULL){ addMove(moves, { x + 1, y - 2 }); }
	else if (x < 9 && y > 2 && board[x + 1][y - 2][0].color != color){ addMove(canEat, { x + 1, y - 2 }); }
	if (x < 8 && y > 0 && board[x + 1][y - 1] == NULL){ addMove(moves, { x + 2, y - 1 }); }
	else if (x < 8 && y > 0 && board[x + 1][y - 1][0].color != color){ addMove(canEat, { x + 2, y - 1 }); }
	if (x > 0 && y > 1 && board[x - 1][y - 1] == NULL){ addMove(moves, { x - 1, y - 2 }); }
	else if (x > 0 && y > 1 && board[x - 1][y - 1][0].color != color){ addMove(canEat, { x - 1, y - 2 }); }
	if (x > 1 && y > 0 && board[x - 1][y - 1] == NULL){ addMove(moves, { x - 2, y - 1 }); }
	else if (x > 1 && y > 0 && board[x - 1][y - 1][0].color != color){ addMove(canEat, { x - 2, y - 1 }); }
	if (x > 0 && y < 9 && board[x - 1][y + 1] == NULL){ addMove(moves, { x - 1, y + 2 }); }
	else if (x > 0 && y < 9 && board[x - 1][y + 1][0].color != color){ addMove(canEat, { x - 1, y + 2 }); }
	if (x > 1 && y < 9 && board[x - 1][y + 1] == NULL){ addMove(moves, { x - 2, y + 1 }); }
	else if (x > 1 && y < 9 && board[x - 1][y + 1][0].color != color){ addMove(canEat, { x - 2, y + 1 }); }
}
void Rook::getMoves(vector<vector<Piece*>> board)
{
	moves.clear();
	for (int x1 = x - 1; x1 > 1; x1--)
	{ 
		if (board[x1][y] == NULL){ addMove(moves, { x1, y }); }
		else
		{
			if (board[x1][y][0].color != color){ addMove(canEat, { x1, y }); }
			break;
		}
	}
	for (int x2 = x + 1; x2 < 9; x2++)
	{
		if (board[x2][y] == NULL){ addMove(moves, { x2, y }); }
		else
		{
			if (board[x2][y][0].color != color){ addMove(canEat, { x2, y }); }
			break;
		}
	}
	for (int y1 = y - 1; y1 > 1; y1--)
	{
		if (board[x][y1] == NULL){ addMove(moves, { x, y1 }); }
		else
		{
			if (board[x][y1][0].color != color){ addMove(canEat, { x, y1 }); }
			break;
		}
	}
	for (int y2 = y + 1; y2 < 9; y2++)
	{
		if (board[x][y2] == NULL){ addMove(moves, { x, y2 }); }
		else
		{
			if (board[x][y2][0].color != color){ addMove(canEat, { x, y2 }); }
			break;
		}
	}
	//for (vector<int> move : moves){ for (int place : move){ cout << place; } cout << endl; }
}
void Pawn::getMoves(vector<vector<Piece*>> board)
{
	moves.clear();
	canEat.clear();
	if (color == 'W')
	{
		if (y != 8){
			if (board[x][y + 1] == NULL) { addMove(moves, { x, y + 1 }); }
			if (x < 9 && board[x + 1][y + 1] != NULL && board[x + 1][y + 1][0].color == 'B'){ addMove(canEat, { x + 1, y + 1 }); }
			if (x > 0 && board[x - 1][y + 1] != NULL && board[x - 1][y + 1][0].color == 'B'){ addMove(canEat, { x - 1, y + 1 }); }
		}
		if (firstMove && board[x][y + 2] ==  NULL){ addMove(moves, { x, y + 2 }); }
	}
	else if (color == 'B')
	{
		if (y != 1 && board[x][y - 1] == NULL) { addMove(moves, { x, y - 1 }); }
		if (y != 1){
			if (board[x][y - 1] == NULL) { addMove(moves, { x, y - 1 }); }
			if (x < 9 && board[x + 1][y - 1] != NULL && board[x + 1][y - 1][0].color == 'W'){ addMove(canEat, { x + 1, y - 1 }); }
			if (x > 0 && board[x - 1][y - 1] != NULL && board[x - 1][y - 1][0].color == 'W'){ addMove(canEat, { x - 1, y - 1 }); }
		}
		if (firstMove && board[x][y - 2] == NULL){ addMove(moves, { x, y - 2 }); }
	}
	//for (vector<int> move : moves){ for (int place : move){ cout << place; } cout << endl; }
}

int deleteXBlack = -1;
int deleteXWhite = 10;
int deleteYBlack = 1;
int deleteYWhite = 1;

void Piece::eat()
{
	if (color == 'B')
	{
		x = deleteXBlack;
		y = deleteYBlack;
		deleteYBlack++;
		if (deleteYBlack == 9)
		{
			deleteYBlack = 1;
			deleteXBlack--;
		}
	}
	else if (color == 'W')
	{
		x = deleteXWhite;
		y = deleteYWhite;
		deleteYWhite++;
		if (deleteYWhite == 9)
		{
			deleteYWhite = 1;
			deleteXWhite++;
		}
	}
	TranslationMatrix = getTranslationMatrix(x, y);
	ModelMatrix = TranslationMatrix * pieceScalingMatrix;
}

void Piece::tempToString()
{ 
	cout << x << ", " << y << endl; 
}
