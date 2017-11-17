#ifndef NODE_H
#define NODE_H

using namespace std;

//class declaration for Node class

template<typename T>
class Node
{

public:
	long int coef;  //long int so that it can hold bigger values to avoid overflow
	int exponent;  // assumed to be ints 
	Node<T> *next;
	Node(long int, int, Node<T>* = 0);
};


//Node default constructor 
template <typename T>
Node<T>::Node(long int coefficient, int exp, Node<T> *p)
{
	coef = coefficient;
	exponent = exp; 
	next = p;
} // END default constructor for Node class





#endif