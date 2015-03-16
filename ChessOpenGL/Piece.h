#ifndef PIECE_H
#define PIECE_H

#include <playground\Window.h>
//#include <playground/Board.h>
#include <vector>
//#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
using namespace std;
using namespace glm;

class Piece
{
private:
	int size;
protected:
	mat4 TranslationMatrix;
	mat4 ModelMatrix;
public:
	int x, y;
	GLuint vertexbuffer, uvbuffer, normalbuffer;
	vector<vector<int>> moves;
	vector<vector<int>> canEat;
	char color;
	Piece(int x, int y);
	Piece(int xpos, int ypos, GLuint vb, GLuint ub, GLuint nb, int s, char c);
	void draw(Window window, glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix);
	virtual bool moveTo(int x, int y, vector<vector<Piece*>> board);
	virtual void rotate(glm::mat4 RotationMatrix);
	virtual void getMoves(vector<vector<Piece*>> board) = 0;
	void eat();
	void tempToString();
};

class King : public Piece
{
public:
	King(int xpos, int ypos, GLuint vb, GLuint ub, GLuint nb, int s, char c);
	void getMoves(vector<vector<Piece*>> board);
};

class Queen : public Piece
{
public:
	Queen(int xpos, int ypos, GLuint vb, GLuint ub, GLuint nb, int s, char c);
	void getMoves(vector<vector<Piece*>> board);
};

class Bishop : public Piece
{
public:
	Bishop(int xpos, int ypos, GLuint vb, GLuint ub, GLuint nb, int s, char c);
	void getMoves(vector<vector<Piece*>> board);
};

class Knight : public Piece
{
private:
	mat4 RotationMatrix;
public:
	Knight(int xpos, int ypos, GLuint vb, GLuint ub, GLuint nb, int s, char c);
	void rotate(mat4 RotationMatrix);
	void getMoves(vector<vector<Piece*>> board);
};

class Rook : public Piece
{
public:
	Rook(int xpos, int ypos, GLuint vb, GLuint ub, GLuint nb, int s, char c);
	void getMoves(vector<vector<Piece*>> board);
};

class Pawn : public Piece
{
private:
	bool firstMove = true;
public:
	Pawn(int xpos, int ypos, GLuint vb, GLuint ub, GLuint nb, int s, char c);
	bool moveTo(int x, int y, vector<vector<Piece*>> board);
	void getMoves(vector<vector<Piece*>> board);
};

#endif