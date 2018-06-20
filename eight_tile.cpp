/////////////////////////////////////////////////////////////
// Main function for eight_tile_solver.
//
/// Dustin Fast (dustin.fast@outlook.com), 2017

#include <iostream>
#include <string>
#include <conio.h>
#include <ctime>
#include <Windows.h>
#include "board.h"
#include "search_algs.h"

using namespace std;

int main()
{
	char chUserInput = ' ';
	int nBoardSize = 3 ; //nBoardSize = size of board aligned with nth index, so we can use it as an index for array stateboard
	int stateboard[3][3]; // Current state
	int goalboard[3][3]; // Solved board state

	while (true)
	{
		//Ini boards to standard solved order {(1, 2, 3), (4, 5, 6), (7, 8, 0)}
		initializeBoards(stateboard, goalboard, nBoardSize); 
		system("CLS"); //Clear screen on windows OS. Will not work on Linux
		cout << "Welcome to the 8-tile solver. Enter a mode to begin:\n"
				"1: Use default puzzle (123056478)\n"
				"2: Enter puzzle graphically\n"
				"3: Enter puzze textually\n"
				"\nESC: Exit";

		chUserInput = _getch(); // Get user ch input

		if (chUserInput == '3')
		{
			//Goal Board setup - manually
			doManualBoardSetup(goalboard, goalboard, "goal", nBoardSize);
			//State Board setup - manually
			doManualBoardSetup(stateboard, goalboard, "state", nBoardSize);
			
		}
		else if (chUserInput == '2')
		{
			//Goal Board setup - graphically
			printSingleBoard(goalboard, "goal", nBoardSize);
			doGraphicalBoardSetup(goalboard, "goal", nBoardSize);
			//State Board setup - graphically
			printSingleBoard(stateboard, "state", nBoardSize);
			doGraphicalBoardSetup(stateboard, "state", nBoardSize);
		}
		else if (chUserInput == '1')
		{
			//Default
			//GOAL: 123456780 (set by initializeBoards()) and STATE: 123056478
			stateboard[0][0] = 1;
			stateboard[0][1] = 2;
			stateboard[0][2] = 3;
			stateboard[1][0] = 0;
			stateboard[1][1] = 5;
			stateboard[1][2] = 6;
			stateboard[2][0] = 4;
			stateboard[2][1] = 7;
			stateboard[2][2] = 8;
		}
		else if (chUserInput == 27) //ESC key. i.e. Exit request
			return 0;
		else
			continue;

		system("CLS");
		//ensure goal state is reachable from start state
		if (!isBoardSolvable(stateboard, goalboard, nBoardSize))
		{
			cout << "Puzzle is unsolveable. Returning to Main Menu. ";
			system("pause");
			continue;
		}

		string strAlgOne = "Breadth-First-Serach";
		string strAlgTwo = "Greedy Best-First-Search (Manahattan Distance heuristic)";
		string strAlgThree = "A* (Misplaced Tile heuristic)";
		string strAlgFour = "A* (Manhattan Distance heuristic)";
		string strAlgChosen = "";

		while (true)
		{
			cout << "Puzzle is solvable. Select a search algorithm to solve:\n";
			cout << "   1. " + strAlgOne + "\n   2. " + strAlgTwo + "\n";
			cout << "   3. " + strAlgThree + "\n   4. " + strAlgFour + "\n";
			chUserInput = _getch();

			if (chUserInput == '1')
			{
				if (!doBFSSearch(stateboard, goalboard, nBoardSize))
					cout << "Error solving puzzle. Check your input and try again.";
				break;
			}
			if (chUserInput == '2')
			{
				if (!doOtherSearch(stateboard, goalboard, nBoardSize, "Manhattan"))
					cout << "Error solving this puzzle. Check your input and try again.";
				break;
			}
			if (chUserInput == '3')
			{
				if (!doOtherSearch(stateboard, goalboard, nBoardSize, "A* TOOP"))
					cout << "Error solving this puzzle. Check your input and try again.";
				break;
			}
			if (chUserInput == '4')
			{
				if (!doOtherSearch(stateboard, goalboard, nBoardSize, "A* Manhattan"))
					cout << "There was an error solving this puzzle. Check your input and try again.";
				break;
			}

		}
			
		//system("CLS"); //Windows only
		//printGameBoards(stateboard, goalboard, nBoardSize); //debug
		cout << endl;
		//cout << "Awaiting algorithm input...\n ";

		system("PAUSE");
	}

	
	return 0;
}