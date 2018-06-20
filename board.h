/////////////////////////////////////////////////////////////
// Tile board functions for eight_tile_solver.
//
/// Dustin Fast (dustin.fast@outlook.com), 2016

#pragma once

#include <iostream>
#include <string>
#include <conio.h>
#include <ctime>
#include <Windows.h>
#include <regex>
#include <math.h>

using namespace std;

// Arrow key ascii, for puzzle navigation
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

void printGameBoards(int board[3][3], int goalboard[3][3], int boardsize); // Display board to console, against backdrop of goal state
void printSingleBoard(int board[3][3], string type, int boardsize); // Display a board to console. type: state=state board, goal=goal board
void initializeBoards(int board[3][3], int goalboard[3][3], int boardsize); //Set board and solved board to goal state
bool slideTile(int board[3][3], int move, int boardsize); // make a move. Returns true/false on valid/invalid move
bool isBoardSolved(int board[3][3], int goalboard[3][3], int boardsize); //returns true if board state matches goal state
bool isBoardSolvable(int board[3][3], int goalboard[3][3], int boardsize); //Accepts matrices and returns true if board is solvable, based on parity of given board
bool isBoardSolvable(int* board, int* goalboard, int boardsize); //Accepts linear model and returns true if board is solvable, based on parity of given board
void randomizeStateBoard(int board[3][3], int goalboard[3][3], int boardsize); //randomizes board state 
void doGraphicalBoardSetup(int board[3][3], string type, int boarddimension); //accepts graphical user input to adjust tiles on the board visually.
void doManualBoardSetup(int board[3][3], int goalboard[3][3], string type, int boarddimension); //accepts linear input vs graphical
void getLinearFromMatrix(int board[3][3], int boardsize, int container[9]); //populates container with a linear representation of the board.
int getManhattanHeuristic(int board[3][3], int goalboard[3][3], int boardsize); // return manhattan heuristic of all tiles
int getTilesOutOfOrderHeuristic(int board[3][3], int goalboard[3][3], int boardsize); //returns tiles out of order heuristic

//Sets up the states with the GUI. Type must be 'goal' or 'state'
void doGraphicalBoardSetup(int board[3][3], string type, int boarddimension)
{
	//Handle move input
	char input = ' ';
	bool legalMove = false;
	string direction = "None";

	while (input != 13) //While the user does not do an ENTER keypress
	{
		system("CLS");
		printSingleBoard(board, type, boarddimension);
		cout << endl;
		if (type == "goal")
			cout << "Setup: GOAL state is shown. Use arrow keys to manipulate, then press ENTER when done." << endl;
		else if (type == "state")
			cout << "Setup: START state is shown. Use arrow keys to manipulate, then press ENTER when done." << endl;

		if (direction != "None")
			if (legalMove)
				cout << "Last Move: " << direction << " was Legal.";
			else
				cout << "Last Move: " << direction << " was NOT Legal.";
		else
			cout << "Last Move: " << direction;


		input = _getch(); //Get user move

		switch (input)
		{
		case UP:
			legalMove = slideTile(board, 2, boarddimension);
			direction = "Up";
			break;
		case LEFT:
			legalMove = slideTile(board, 0, boarddimension);    
			direction = "Left";
			break;
		case DOWN:
			legalMove = slideTile(board, 3, boarddimension);
			direction = "Down";
			break;
		case RIGHT:
			legalMove = slideTile(board, 1, boarddimension);
			direction = "Right";
			break;
		default:
			direction = "INVALID";
			legalMove = false;
		}
	}
}
//Sets up the states from strings entered from keyboard. Type must be 'goal' or 'state'
//Adapted from http://www.cplusplus.com/forum/general/84737/
void doManualBoardSetup(int board[3][3], int goalboard[3][3], string type, int boarddimension)
{
	//Handle move input
	string input = "";
	bool legalMove = false;
	string direction = "None";

	while (input == "") 
	{
		system("CLS");
		printSingleBoard(board, type, boarddimension);
		cout << endl;
		if (type == "goal")
			cout << "Setup: GOAL state is shown. Enter new state as a string of digits (ex: 123456780): " << endl;
		else if (type == "state")
			cout << "Setup: START state is shown. Enter new state as a string of digits (ex: 123456780): " << endl;
		cin >> input;

		//validate user input for size and content
		bool bDupFound = false;
		string strPattern = R"([0-8]{9})";
		regex statePattern(strPattern);
		
		for (int i = 0; i < 9; i++)
		{
			char temp = input[i];
			for (int j = 0; j < 9; j++)
				if (temp == input[j] && i != j)
					bDupFound = true;
		}
		if (!regex_match(input, statePattern) || bDupFound ) //if string contains correct correctors with no duplicates
		{
			if (type == "state")
				cout << "Invalid input (puzzle is unsolvable as entered) - Please try again.";
			else
				cout << "Invalid input - Please try again. ";
			input = "";
			system("pause");
			continue;
		}

		//assign input to board. Note: " - '0'" converts from string to int
		board[0][0] = input[0] - '0';
		board[0][1] = input[1] - '0';
		board[0][2] = input[2] - '0';
		board[1][0] = input[3] - '0';
		board[1][1] = input[4] - '0';
		board[1][2] = input[5] - '0';
		board[2][0] = input[6] - '0';
		board[2][1] = input[7] - '0';
		board[2][2] = input[8] - '0';

	}
}
// Print boards (state and goal) and replace zero with a character 177
//From http://www.cplusplus.com/forum/general/84737/
void printGameBoards(int board[3][3], int goalboard[3][3], int boardsize)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int row = 0; row < boardsize; row++)
	{
		for (int column = 0; column < boardsize; column++)
		{
			if (board[row][column] == 0)
			{
				SetConsoleTextAttribute(hConsole, 7); //Default color
				cout << "\xB1\xB1 ";
			}
			else
			{
				if (board[row][column] == goalboard[row][column]) //If state location is equal to goalboard location then set to green
					SetConsoleTextAttribute(hConsole, 10);
				else
					SetConsoleTextAttribute(hConsole, 12); //else, set to red
				if (board[row][column]<10) // Print a 0 first if # < 10
					cout << "0";
				cout << board[row][column] << " ";
			}
		}
		cout << endl;
	}
	SetConsoleTextAttribute(hConsole, 7);
}

// Print board and replace zero with a character 177
// Adapted From http://www.cplusplus.com/forum/general/84737/
void printSingleBoard(int board[3][3], string type, int boardsize)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int row = 0; row < boardsize; row++)
	{
		for (int column = 0; column < boardsize; column++)
		{
			if (board[row][column] == 0)
			{
				SetConsoleTextAttribute(hConsole, 7); //Default color
				cout << "\xB1\xB1 ";
			}
			else
			{
				if (type == "goal") 
					SetConsoleTextAttribute(hConsole, 10);
				else
					SetConsoleTextAttribute(hConsole, 12); //else, set to red
				if (board[row][column]<10) // Print a 0 first if # < 10
					cout << "0";
				cout << board[row][column] << " ";
			}
		}
		cout << endl;
	}
	SetConsoleTextAttribute(hConsole, 7);
}
//Sets board and goal to standard goal state of 1 2 3 4 5 6 7 8 0.
void initializeBoards(int board[3][3], int goalboard[3][3], int boardsize)
{
	int i = 1;
	for (int row = 0; row < boardsize; row++)
	{
		for (int column = 0; column < boardsize; column++)
		{
			if (i == 9)
				i = 0;
			board[row][column] = i;
			goalboard[row][column] = i;
			i++;
		}
	}
}

//Make a move on the board. Returns false if move is not legal.
//Legal values for move are [0] = left, [1] = right, [2] = up, [3] = down.
//Adapted from http://www.cplusplus.com/forum/general/84737/
bool slideTile(int board[3][3], int move, int boardsize)
{
	int emptyRow;
	int emptyCol;
	bool legalMoves[4] = { 1,1,1,1 }; //array of legal moves, [0] = left, [1] = right, [2] = up, [3] = down. 

	//Find location of empty tile
	for (int row = 0; row < boardsize; row++)
	{
		for (int column = 0; column < boardsize; column++)
		{
			if (board[row][column] == 0)
			{
				emptyRow = row;
				emptyCol = column;
			}
		}
	}

	//Determine legal moves ( [0] = left, [1] = right, [2] = up, [3] = down)
	if (emptyRow >= boardsize - 1) //If emptyrow is greater than boardheight, can't move up
		legalMoves[2] = false;
	if (emptyRow == 0) //If empty row is 0, can't move down
		legalMoves[3] = false;

	if (emptyCol == 0) //If we're on the left edge, can't move right
		legalMoves[1] = false;
	if (emptyCol >= boardsize - 1) //If we're on the right edge, can't move left
		legalMoves[0] = false;

	switch (move) //Do tile move as long as it's legal
	{
	case 0:
		if (legalMoves[move])
		{
			board[emptyRow][emptyCol] = board[emptyRow][emptyCol + 1];
			board[emptyRow][emptyCol + 1] = 0;
			emptyCol = emptyCol + 1;
		}
		else
			return false;
		break;
	case 1:
		if (legalMoves[move])
		{
			board[emptyRow][emptyCol] = board[emptyRow][emptyCol - 1];
			board[emptyRow][emptyCol - 1] = 0;
			emptyCol = emptyCol - 1;
		}
		else
			return false;
		break;
	case 2:
		if (legalMoves[move])
		{
			board[emptyRow][emptyCol] = board[emptyRow + 1][emptyCol];
			board[emptyRow + 1][emptyCol] = 0;
			emptyRow = emptyRow + 1;
		}
		else
			return false;
		break;
	case 3:
		if (legalMoves[move])
		{
			board[emptyRow][emptyCol] = board[emptyRow - 1][emptyCol];
			board[emptyRow - 1][emptyCol] = 0;
			emptyRow = emptyRow - 1;
		}
		else
			return false;
		break;
	}
	return true;
}

//Determines legality of a move. Returns false if move is not legal, else returns true
//Legal values for move are [0] = left, [1] = right, [2] = up, [3] = down.
//adapted from http://www.cplusplus.com/forum/general/84737/
bool isMoveLegal(int board[3][3], int move, int boardsize)
{
	int emptyRow;
	int emptyCol;
	bool legalMoves[4] = { 1,1,1,1 }; //array of legal moves, [0] = left, [1] = right, [2] = up, [3] = down. 

	//Find location of empty tile
	for (int row = 0; row < boardsize; row++)
	{
		for (int column = 0; column < boardsize; column++)
		{
			if (board[row][column] == 0)
			{
				emptyRow = row;
				emptyCol = column;
			}
		}
	}
	
	//Determine legal moves ( [0] = left, [1] = right, [2] = up, [3] = down)
	if (emptyRow >= boardsize - 1) //If emptyrow is greater than boardheight, can't move up
		legalMoves[2] = false;
	if (emptyRow == 0) //If empty row is 0, can't move down
		legalMoves[3] = false;

	if (emptyCol == 0) //If we're on the left edge, can't move right
		legalMoves[1] = false;
	if (emptyCol >= boardsize - 1) //If we're on the right edge, can't move left
		legalMoves[0] = false;

	if (legalMoves[move])
		return true;
	else
		return false;
}

//Randomizes board. Leaves goalboard intact, but uses it to ensure our randomized state isn't the solved state.
//From http://www.cplusplus.com/forum/general/84737/
void randomizeStateBoard(int board[3][3], int goalboard[3][3], int boardsize)
{
	time_t t;
	srand((unsigned)time(&t)); //TODO: MOVE to main
	int move;
	while (isBoardSolved(board, goalboard, boardsize)) //If the board ends up being solved at the end of the randomize, randomize the board again
	{
		for (int i = 0; i < 100000; i++) //Series of 100000 random moves
		{
			move = rand() % 4;
			slideTile(board, move, boardsize);
		}
	}
}

//Checks board state to see if it is solved
//From http://www.cplusplus.com/forum/general/84737/
bool isBoardSolved(int board[3][3], int goalboard[3][3], int boardsize)
{
	bool boardSolved = true;
	int row = 0;
	int col = 0;
	while (boardSolved && row < boardsize)
	{
		if (goalboard[row][col] == board[row][col]) //Compare each index of solved board with game board
		{
			col++;
			if (col == boardsize-1)
			{
				row++;
				col = 0;
			}
		}
		else //Once a discrepancy is found, set boardSolved to false to break the loop
			boardSolved = false;
	}
	return boardSolved;
}

//Do parity check to ensure goal state is reachable from current state:
//For every int look at each other int to see if it is "greater" than it (i.e. it is out of order)
bool isBoardSolvable(int board[3][3], int goalboard[3][3], int boardsize)
{
	//Populates linear arrays with the numbers from the matrix, for convenience working with them.
	int tempGoalState[9];
	getLinearFromMatrix(goalboard, boardsize, tempGoalState); 
	int tempStartState[9];
	getLinearFromMatrix(board, boardsize, tempStartState);

	//For each element, count number of elements which are on right side of it and are smaller than it.
	int nGoalInvCount = 0;
	for (int i = 0; i < 8; i++)
		for (int j = i + 1; j < 9; j++)
			if (tempGoalState[j] && tempGoalState[i] && tempGoalState[i] > tempGoalState[j])
				nGoalInvCount++;
	int nStartInvCount = 0;
	for (int i = 0; i < 8; i++)
		for (int j = i + 1; j < 9; j++)
			if (tempStartState[j] && tempStartState[i] && tempStartState[i] > tempStartState[j])
				nStartInvCount++;

	//Solvable iff both states have same parity
	if (nGoalInvCount % 2 == 0 && nStartInvCount % 2 == 0)
		return true;
	else if (nGoalInvCount % 2 != 0 && nStartInvCount % 2 != 0)
		return true;

	return false;
}

bool isBoardSolvable(int* board, int* goalboard, int boardsize)
{
	int* tempGoalState = goalboard;
	int* tempStartState = board;

	//For each element, count number of elements which are on right side of it and are smaller than it.
	int nGoalInvCount = 0;
	for (int i = 0; i < 8; i++)
		for (int j = i + 1; j < 9; j++)
			if (tempGoalState[j] && tempGoalState[i] && tempGoalState[i] > tempGoalState[j])
				nGoalInvCount++;
	int nStartInvCount = 0;
	for (int i = 0; i < 8; i++)
		for (int j = i + 1; j < 9; j++)
			if (tempStartState[j] && tempStartState[i] && tempStartState[i] > tempStartState[j])
				nStartInvCount++;

	//Solvable iff both states have same parity
	if (nGoalInvCount % 2 == 0 && nStartInvCount % 2 == 0)
		return true;
	else if (nGoalInvCount % 2 != 0 && nStartInvCount % 2 != 0)
		return true;

	return false;
}

//returns an int* of size 8 from a 3zx3 matrix as a linear list.
void getLinearFromMatrix(int board[3][3], int boardsize, int container[9])
{
	int nContIndex = 0;
	for (int i = 0; i < boardsize; i++) 
	{
		for (int j = 0; j < boardsize; j++)
		{
			container[nContIndex] = board[i][j];
			nContIndex++;
		}
	}
}

//Look at each tile to see if it is out of order and by how much
int getTilesOutOfOrderHeuristic(int board[3][3], int goalboard[3][3], int boardsize)
{
	//Populates linear arrays with the numbers from the matrix, for convenience working with them.
	int goalState[9];
	getLinearFromMatrix(goalboard, boardsize, goalState);
	int currState[9];
	getLinearFromMatrix(board, boardsize, currState);

	int nTOOP = 0;
	for (int x = 0; x < 9; x++)
	{
		int nValue = currState[x];
		if (nValue != 0) // (Skip blank tile)
		{
			//find the x/y coords in the goal state for the current val
			int nGoalX = 0;
			for (int i = 0; i < 9; i++)
			{
				if (goalState[i] == nValue)
				{
					nGoalX = i;
					break;
				}
			}
			if (nGoalX != x)
				nTOOP++;
		}
	}
	return nTOOP;
}

int getManhattanHeuristic(int board[3][3], int goalboard[3][3], int boardsize)
{
	//Populates linear arrays with the numbers from the matrix, for convenience working with them.
	int goalState[9];
	int currState[9];
	getLinearFromMatrix(goalboard, boardsize, goalState);
	getLinearFromMatrix(board, boardsize, currState);

 	int nManhattanDist = 0;
	for (int x = 0; x < 9; x++)
	{
		int nValue = currState[x];
		if (nValue != 0) // (Skip blank tile)
		{
			//find the x/y coords in the goal state for the current val
			int nGoalX = 0;
			for (int i = 0; i < 9; i++)
			{
				if (goalState[i] == nValue)
				{
					nGoalX = i;
					break;
				}
			}
			int dx = x - nGoalX; // x-distance to goal coordinate
			nManhattanDist += abs(dx);
		}
	}
	return nManhattanDist;
}
