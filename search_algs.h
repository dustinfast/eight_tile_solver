/////////////////////////////////////////////////////////////
// Search Algorithms lib for eight_tile_solver.
//
/// Dustin Fast (dustin.fast@outlook.com), 2016

#pragma once

#include <iostream>
#include <string>
#include <conio.h>
#include <ctime>
#include <Windows.h>
#include <queue>
#include <vector>
#include <stack>
#include "node.h"
#include "board.h"


using namespace std;

bool doBFSSearch(int rootboard[3][3], int goalboard[3][3], int boardsize);
bool doOtherSearch(int rootboard[3][3], int goalboard[3][3], int boardsize, string type);
bool isStateInQueue(Node *n, queue<Node*> q); // util function
bool isStateInPQueue(Node *n, priority_queue<Node*> q); // util function
void doSolutionPrint(Node *node, int goalboard[3][3]); //prints solution path and delete's the nodes in memory

													  
//Comparator function for priority queue
struct PtrLess
{
	bool operator()(Node* left, Node* right)
	{
		return left->getTotalCost() > right->getTotalCost();
	}
};

//Attempts to solve puzzle with BFS.
//The BFS uses a FIFO queue and does not assign any cost to the nodes.
//  This is the only difference between doBFSSearch() and doOtherSearch()
bool doBFSSearch(int rootboard[3][3], int goalboard[3][3], int boardsize)
{
	int nExpanded = 0;
	unsigned int nMaxInSearchSpace = 0;
	cout << "\nSolving...\n\n";


	//Create root node, open list, and closed list
	int nTemp[9];
	getLinearFromMatrix(rootboard, 3, nTemp);
	Node* node = new Node(nTemp);
	queue<Node*> qOpenList;
	queue<Node*> qClosedList;

	//Check if goal state already
	if (isBoardSolved(rootboard, goalboard, 3))
	{
		cout << "\nStart state was Goal state.\n\n";
		printGameBoards(rootboard, goalboard, 3);
		return true;
	}

	//Root state is not goal state, so continue with Breadth First Search by
	//  adding the current state to the open list and then doing our BFS.
	qOpenList.push(node);

	while (true)
	{
		if (qOpenList.empty())
			return false; //if the open list is empty, we failed at solving
		node = qOpenList.front();
		qOpenList.pop();
		qClosedList.push(node);

		//determine possible moves
		vector<int> vMoves; //holds possible moves for this state. [0] = left, [1] = right, [2] = up, [3] = down. 
		int board[3][3];		//container for current state
		node->getMatrixState(board); //board is now the 3d matrix of current state

		if (isMoveLegal(board, 0, 3)) //try left
			vMoves.push_back(0);
		if (isMoveLegal(board, 1, 3)) //try right
			vMoves.push_back(1);
		if (isMoveLegal(board, 2, 3)) //try up
			vMoves.push_back(2);
		if (isMoveLegal(board, 3, 3)) //try down
			vMoves.push_back(3);

		for (unsigned int i = 0; i < vMoves.size(); i++) //for each action in Actions(node.state) do:
		{
			//Creates new child node with the state of having made the current move 
			//(When the nodes "child" constructor is called here, the move is performed)
			int nTemp[9];
			getLinearFromMatrix(board, 3, nTemp);
			Node* child = new Node(nTemp, vMoves[i], node);
			nExpanded++;

			//look for state in open and closed lists. 
			//  If it exists in either we don't do anything with it. If it does we check for sol and add to queue
			if (!isStateInQueue(child, qOpenList) && !isStateInQueue(child, qClosedList))
			{
				int nTemp2[3][3];
				child->getMatrixState(nTemp2);
				if (isBoardSolved(nTemp2, goalboard, 3))
				{
					//Solution found. Output metrics
					doSolutionPrint(child, goalboard);
					cout << "\nSolution found at depth " << child->getDepth();
					cout << " after expanding " << nExpanded << " nodes.\nA maximum of " << nMaxInSearchSpace << " nodes existed in the search space.\n\n";
					return true;
				}
				else
				{
					qOpenList.push(child);
					if (qOpenList.size() > nMaxInSearchSpace)
						nMaxInSearchSpace = qOpenList.size();
				}
			}
			else
				delete child; //delete the node since we're not going to use it
		}
	}
}

//Attempts to solve puzzle with the given algorithm type 
//type options are:
// Manhattan 
// A* TOOP (Depth + Tiles Out Of Place heuristic)
// A* Manhattan (Depth + Manhattan Distance as heuristic)
bool doOtherSearch(int rootboard[3][3], int goalboard[3][3], int boardsize, string type)
{
	int nExpanded = 0;
	unsigned int nMaxInSearchSpace = 0;

	cout << "\nSolving...\n\n";


	//Create root node, open list, and closed list
	int nTemp[9];
	getLinearFromMatrix(rootboard, 3, nTemp);
	Node* node = new Node(nTemp);
	priority_queue<Node *, std::vector< Node * >, PtrLess> qOpenList;
	priority_queue<Node *, std::vector< Node * >, PtrLess> qClosedList;

	//Check if goal state already
	if (isBoardSolved(rootboard, goalboard, 3))
	{
		cout << "\nStart state was Goal state.\n\n";
		printGameBoards(rootboard, goalboard, 3);
		return true;
	}

	//Root state is not goal state, so continue with Breadth First Search by
	//  adding the current state to the open list and then doing our BFS.
	qOpenList.push(node);

	while (true)
	{
		if (qOpenList.empty())
			return false; //if the open list is empty, we failed at solving
		node = qOpenList.top();
		qOpenList.pop();
		qClosedList.push(node);

		//determine possible moves
		vector<int> vMoves; //holds possible moves for this state. [0] = left, [1] = right, [2] = up, [3] = down. 
		int board[3][3];		//container for current state
		node->getMatrixState(board); //board is now the 3d matrix of current state	

		if (isMoveLegal(board, 0, 3)) //try left
			vMoves.push_back(0);
		if (isMoveLegal(board, 1, 3)) //try right
			vMoves.push_back(1);
		if (isMoveLegal(board, 2, 3)) //try up
			vMoves.push_back(2);
		if (isMoveLegal(board, 3, 3)) //try down
			vMoves.push_back(3);

		for (unsigned int i = 0; i < vMoves.size(); i++) //for each action in Actions(node.state) do:
		{
			//Creates new child node with the state of having made the current move 
			//(When the nodes "child" constructor is called here, the move is performed)
			int nTemp[9];
			getLinearFromMatrix(board, 3, nTemp);
			Node* child = new Node(nTemp, vMoves[i], node);
			nExpanded++;

			//look for state in open and closed lists. 
			//  If it exists in either we don't do anything with it. else if not sol we add to queue
			/*cout << isStateInPQueue(child, qOpenList) << " : ";
			cout << isStateInPQueue(child, qClosedList) << endl;*/
			/*if (!isStateInPQueue(child, qOpenList) && !isStateInPQueue(child, qClosedList))
			{*/
				int childboard[3][3];
				child->getMatrixState(childboard);
				if (isBoardSolved(childboard, goalboard, 3))
				{
					//Solution found. Output metrics
					cout << "\nSolution found at depth " << child->getDepth();
					cout << " after expanding " << nExpanded << " nodes.\nA maximum of " << nMaxInSearchSpace << " nodes existed in the search space.\n\n";
					doSolutionPrint(child, goalboard);
					return true;
				}
				else
				{
					//type options are
					// Manhattan 
					// A* TOOP = (Depth + Tiles Out Of Place) as heuristic
					// A* Manhattan = (Depth + Manhattan Distance) as heuristic
					if (type == "Manhattan")
					{
						child->setTotalCost(getManhattanHeuristic(childboard, goalboard, 3));
					}
					else if (type == "A* TOOP")
					{
						child->setTotalCost(child->getDepth() + getTilesOutOfOrderHeuristic(childboard, goalboard, 3));
					}
					else if (type == "A* Manhattan")
					{
						child->setTotalCost(child->getDepth() + getManhattanHeuristic(childboard, goalboard, 3));
					}
					else
						return false;


					if (qOpenList.size() >= 250) //break infinite loop if one appears to have started.
						return false;

					qOpenList.push(child);
					if (qOpenList.size() > nMaxInSearchSpace)
						nMaxInSearchSpace = qOpenList.size();

				}
			//}
			//else
			//	delete child; //delete the node since we're not going to use it
		}
	}
}

//utility functions
bool isStateInQueue(Node *n, queue<Node*> q)
{
	for (unsigned int j = 0; j < q.size(); j++)
	{
		Node *temp = q.front();
		q.pop();
		int nTemp1[9];
		n->getLinearState(nTemp1);
		int nTemp2[9];
		temp->getLinearState(nTemp2);

		if (nTemp2[0] == nTemp1[0] && nTemp2[1] == nTemp1[1] && nTemp2[2] == nTemp1[2] && nTemp2[3] == nTemp1[3]
			&& nTemp2[4] == nTemp1[4] && nTemp2[5] == nTemp1[5] && nTemp2[5] == nTemp1[5] && nTemp2[6] == nTemp1[6]
			&& nTemp2[7] == nTemp1[7] && nTemp2[8] == nTemp1[8] && nTemp2[9] == nTemp1[9])
			return true;
	}
	return false;

}

bool isStateInPQueue(Node *n, priority_queue<Node*> q)
{
	for (unsigned int j = 0; j < q.size(); j++)
	{
		Node *temp = q.top();
		q.pop();
		int nTemp1[9];
		n->getLinearState(nTemp1);
		int nTemp2[9];
		temp->getLinearState(nTemp2);

		if (nTemp2[0] == nTemp1[0] && nTemp2[1] == nTemp1[1] && nTemp2[2] == nTemp1[2] && nTemp2[3] == nTemp1[3] && nTemp2[4] == nTemp1[4] && nTemp2[5] == nTemp1[5] && nTemp2[5] == nTemp1[5] && nTemp2[6] == nTemp1[6] && nTemp2[7] == nTemp1[7] && nTemp2[8] == nTemp1[8] && nTemp2[9] == nTemp1[9])
			return true;
	}
	return false;
}

void doSolutionPrint(Node *node, int goalboard[3][3])
{
	cout << "Path from Start to Goal (Red tiles denote out-of-place tiles):\n";
	stack<Node*> s;

	//Push all path nodes onto a stack so we can print them in reverse order.
	while (node != nullptr)
	{
		s.push(node);
		node = node->getParent();
	}

	//Pop them all off the stack and print them
	while (!s.empty())
	{
		Node *node = s.top();
		s.pop();
		int nTemp[3][3];
		node->getMatrixState(nTemp);
		printGameBoards(nTemp, goalboard, 3);
		cout << endl;
		delete node;
	}
}

