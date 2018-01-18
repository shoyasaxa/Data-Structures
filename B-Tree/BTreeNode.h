#ifndef BTREENODE_H
#define BTREENODE_H

#include<iostream>
#include "BTree.h"

using namespace std; 



//class declaration for Node class

template<int M>
class Node
{

public:
	int numKeys; 
	int numChildren;
	// I decided to create numChildren as well just to make code more intuitive

	// I will allocate one more than needed, since my implementation is add->if more than keys are supposed to be->split 
	int keys[M]; 
	Node<M> * children[M+1];
	bool isLeaf;
	const static int degree = M; 

	//Node();
	Node();
	~Node();

};

template <int M>
Node<M>::Node()
{
	numKeys = 0;
	numChildren = 0;
	isLeaf = false; 
	
	for (int i = 0; i < degree + 1; i++)//initialize everything as null
	{
		children[i] = NULL;
	}
	 
	for (int i = 0; i < degree; i++)
	{
		keys[i] = 0; // intialize to sentinel  
	}
	
} // END default constructor for Node class

 
template <int M> 
Node<M>::~Node()
{ 
	//nothing, as arrays are allocated on stack 
}






#endif // !BTREENODE_H
