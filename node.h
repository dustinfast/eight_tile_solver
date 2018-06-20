/////////////////////////////////////////////////////////////
// Node object for eight_tile_solver.
//
// Node has State, Action, Parent, and Path Cost properites and ptr to Parent
// In this implementation, state is an int* of size 9. This may be adjusted for
// larger puzzles.
// Includes interface and implementation.
//
/// Dustin Fast (dustin.fast@outlook.com), 2016

#pragma once

#include <string>
#include <iostream>
#include "board.h"

using namespace std;

class Node
{
public:
	Node(int state[9]);								//root node constructor
	Node(int state[9], int action, Node* parent);   //Child node constructor. Does action to state in constructor
	int* getLinearState();							//Accessors..
	void getLinearState(int container[9]);
	void getMatrixState(int container[3][3]);
	int getAction();
	int getDepth();									//Return the depth of the node by following the parent pointer up to the root
	int getTotalCost();								//Returns m_nTotalCost, which must be set independently by cvalling setTotalCost, else it is 0
	Node* getParent();
	Node* getSelf();
	void setTotalCost(int cost);
	friend bool operator <(const Node &A, const Node &B);


protected:
	int m_nState[9];		// Representation of a 3x3 matrix state in linear form (ex: 1,2,3,4,5,6,7,8,0)
	int m_nTotalCost;
	int m_nAction;			// Data Element, which is a string to hold an action ([0] = left, [1] = right, [2] = up, [3] = down. )
	Node* m_pParent;		//ptrs to parent node (may be NULL)
};

//Root Node constructor
Node::Node(int state[9]) : m_nTotalCost(0), m_nAction(-1), m_pParent(nullptr)
{
	for (int i = 0; i < 9; i++)
		m_nState[i] = state[i];
}
//Child Node constructor
Node::Node(int state[9], int action, Node* parent) : m_nTotalCost(0), m_nAction(action), m_pParent(parent)
{
	for (int i = 0; i < 9; i++)
		m_nState[i] = state[i];

	int nTemp[3][3];
	getMatrixState(nTemp);
	slideTile(nTemp, action, 3);
	getLinearFromMatrix(nTemp, 3, m_nState);
}

//Operator Overloads
// < is used by priority queues, thus we need to tell them how to order the list with this operator overload
//bool operator <(const Node &A, const Node &B)
//{
//	return A.m_nTotalCost < B.m_nTotalCost;
//}

//Accessors
int* Node::getLinearState()
{
	return m_nState;
}
void Node::getLinearState(int container[9])
{
	for (int i = 0; i < 9; i++)
		container[i] = m_nState[i];
}
void Node::getMatrixState(int container[3][3])
{
	container[0][0] = m_nState[0];
	container[0][1] = m_nState[1];
	container[0][2] = m_nState[2];
	container[1][0] = m_nState[3];
	container[1][1] = m_nState[4];
	container[1][2] = m_nState[5];
	container[2][0] = m_nState[6];
	container[2][1] = m_nState[7];
	container[2][2] = m_nState[8];
}

//Return the depth of the node by following the parent pointer up to the root
int Node::getDepth()
{
	int nDepth = 0;
	Node *pNode = this;
	while (pNode != nullptr)
	{
		nDepth++;
		pNode = pNode->getParent();
	}
	return nDepth;
}

int Node::getTotalCost()
{
	return m_nTotalCost;
}
int Node::getAction()
{
	return m_nAction;
}
Node* Node::getParent()
{
	return m_pParent;
}
Node* Node::getSelf()
{
	return this;
}

//Mutators
void Node::setTotalCost(int cost)
{
	m_nTotalCost = cost;
}

class Compare
{
public:
	bool operator() (Node *A, Node *B)
	{
		return A->getTotalCost() < B->getTotalCost();
	}
};