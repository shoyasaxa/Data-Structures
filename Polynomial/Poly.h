#ifndef POLY_H
#define POLY_H


#include <iostream>
#include <string>
#include "Node.h"

using namespace std;


// ------------- overloaded operators ------------

template <typename T>
class Poly;

template <typename T>
Poly<T> operator+ (const Poly<T>&);

template <typename T>
class Poly;

template <typename T>
Poly<T> operator- (const Poly<T>&);


// -------------- class Poly declaration 

/*----------------------------------------DESIGN DECISION---------------------------------------------------------------
	I have decided to implement my Poly as a chaining structure. My assignment 4 explores this extensively, but essentially,
	an array structure would not be efficient if the polynomial was something like x^10000000. Array's index capabilities would
	make the implementation fast for the most part, but its inflexibility to change the size forces the program to allocate much more
	space than it should. Thus, I decided that a chaining structure would be much more consistent in performance, in both time and space. 
	In cases when the polynomial inputs are huge and have huge exponents, chaining implementation would outperform the array implementation.

*/




template <typename T> 
class Poly 
{
public:
	Poly<T> operator+ (const Poly<T>&);
	Poly<T> operator- (const Poly<T>&);
	const Poly<T>& operator= (const Poly<T>&);
	Poly();
	Poly(const Poly<T>&);
	~Poly();
	void addLast(const T &, const T&);   
	void addFirst(const T &, const T&);
	void display(); 
	Poly<T> multiply(const Poly<T> &);
	Poly<T> exponentiation(const int);
	long int eval(const int);
	long int pow(int, int); 
	void clear(); 
	bool empty() const;
	void removeFirst();

	void merge(long int**, int, int, int);
	void mergeSort(long int** , int, int);

private: 
	Node<T> *head, *tail;
	unsigned numTerms; 

};


//Default constructor.
template<typename T >
Poly< T >::Poly()
{
	numTerms = 0;
	head = NULL;
	tail = NULL;
}

//copy constructor 
template <typename T>
Poly<T>::Poly(const Poly<T>& rhs)
{
	this->numTerms = 0;
	this->head = NULL;
	this->tail = NULL;    //first 0-out everything 

	Node<T> *otherCurrent = rhs.head;

	while (otherCurrent != NULL)
	{
		this->addLast(otherCurrent->coef, otherCurrent->exponent);
		otherCurrent = otherCurrent->next;
	}
}


//Class destructor 	
template<typename T >
Poly< T >::~Poly()
{
	this->clear();
}

//overloaded = operator
template <typename T> 
const Poly<T>& Poly<T>::operator= (const Poly<T>& rhs )
{
	if (this != &rhs)
	{
		this->clear(); 

		this->numTerms = 0;
		this->head = NULL;
		this->tail = NULL;    //first 0-out everything 

		Node<T> *otherCurrent = rhs.head;

		while (otherCurrent != NULL)
		{
			this->addLast(otherCurrent->coef, otherCurrent->exponent);
			otherCurrent = otherCurrent->next;
		}
	}


	return *this;
}


//Clears this list by removing all of its elements.
template<typename T >
void Poly<T>::clear()
{
	while (!(this->empty()))
	{
		this->removeFirst();

	} // END while list not empty

	  // should be redundant, but no harm done
	head = NULL;
	tail = NULL; 
	numTerms = 0;
}

//Returns true if this list is empty; returns false otherwise.
template<typename T >
bool Poly<T>::empty()	const
{
	bool isEmpty = false;

	if (head == NULL)
	{
		if (tail == NULL)
		{
			if (numTerms == 0)
			{
				isEmpty = true;
			}
		}
	}

	return isEmpty;
}

//Removes and returns the first object in this list.
template<typename T >
void Poly<T>::removeFirst() 
{
	if (this->empty())
	{
		//list is empty, nothing to do
	}
	else if ((head == tail) && (head != NULL))
	{
		//only one object on list
		delete head;

		head = NULL;
		tail = NULL;
		numTerms = 0;
	}
	else
	{
		//more than one object on the list
		Node<T> *current = head->next;

		delete head;
		numTerms--;
		head = current;
	}
}

template<typename T >
void Poly<T>::addFirst(const T & coefficient, const T & exponent)
{
	Node<T> *addMe = new Node<T>(coefficient, exponent, NULL);

	if (this->empty())
	{
		// this is first object to be added to the list
		head = addMe;
		tail = addMe;
		numTerms = 1;
	}

	else
	{
		addMe->next = head;
		head = addMe;
		numTerms++;
	}
}


//Adds the specified object to the end of the list.
template<typename T >
void Poly<T>::addLast(const T & coefficient, const T & exponent )
{
	Node<T> *addMe = new Node<T>(coefficient, exponent, NULL); 


	if (this->empty())
	{
		//list is currently empty, this will be the only object on the list 
		head = tail = addMe;
		numTerms = 1;
	}
	else
	{
		//more than one object on the list
		tail->next = addMe;
		tail = addMe;
		numTerms++;
	}
}

//displays the polynomial in regular representation
template<typename T>
void Poly<T>::display()
{
	Node<T> *current = head; 

	for (unsigned int i = 0; i < numTerms; i++)
	{
		cout << current->coef;

		if (current->exponent != 0)
		{
			cout << "X^" << current->exponent;
		}

		if (i != numTerms - 1)
		{
			cout << " + "; 
		}
		current = current->next; 
	}
	cout << endl; 
}

//overloaded + operator 
template<typename T>
Poly<T> Poly<T>::operator+(const Poly<T>&rhs)
{
	try
	{
		Poly<T> result;
		Node<T> *thisIter = this->head;
		Node<T> *rhsIter = rhs.head;


		while ((thisIter != NULL) && (rhsIter != NULL))
		{
			if (rhsIter->exponent > thisIter->exponent)
			{
				result.addLast(rhsIter->coef, rhsIter->exponent);
				rhsIter = rhsIter->next;
			}
			if (thisIter->exponent > rhsIter->exponent)
			{
				result.addLast(thisIter->coef, thisIter->exponent);
				thisIter = thisIter->next;
			}
			if (thisIter->exponent == rhsIter->exponent)
			{
				long int newCoefficient = thisIter->coef + rhsIter->coef;

				//overflow check
				if (newCoefficient - thisIter->coef != rhsIter->coef)
				{
					newCoefficient = ~0; //sentinel 
					throw string("Overflow Detected operator+!");
				}

				result.addLast(newCoefficient, thisIter->exponent);
				thisIter = thisIter->next;
				rhsIter = rhsIter->next;
			}
		}

		while (thisIter != NULL)
		{
			result.addLast(thisIter->coef, thisIter->exponent);
			thisIter = thisIter->next;
		}
		while (rhsIter != NULL)
		{
			result.addLast(rhsIter->coef, rhsIter->exponent);
			rhsIter = rhsIter->next;
		}

		return result;
	}
	catch (string s)
	{
		cout << s << endl;
		Poly<T> sentinel;
		sentinel.addFirst(~0, ~0); //put in a sentinel value

		return sentinel; 
	}
}

//overloaded - operator 
template<typename T>
Poly<T> Poly<T>::operator-(const Poly<T>&rhs )
{
	try
	{
		Poly<T> result;
		Node<T> *thisIter = this->head;
		Node<T> *rhsIter = rhs.head;


		while ((thisIter != NULL) && (rhsIter != NULL))
		{
			if (rhsIter->exponent > thisIter->exponent)
			{
				result.addLast( -1 * rhsIter->coef, rhsIter->exponent); //have to revert coefficient since subtracting 
				rhsIter = rhsIter->next;
			}
			if (thisIter->exponent > rhsIter->exponent)
			{
				result.addLast(thisIter->coef, thisIter->exponent);
				thisIter = thisIter->next;
			}
			if (thisIter->exponent == rhsIter->exponent)
			{
				long int newCoefficient = thisIter->coef - rhsIter->coef;

				//overflow check
				if (newCoefficient + rhsIter->coef != thisIter->coef)
				{
					newCoefficient = ~0; //sentinel 
					throw string("Overflow Detected in operator- ");
				}

				result.addLast(newCoefficient, thisIter->exponent);
				thisIter = thisIter->next;
				rhsIter = rhsIter->next;
			}
		}

		while (thisIter != NULL)
		{
			result.addLast(thisIter->coef, thisIter->exponent);
			thisIter = thisIter->next;
		}
		while (rhsIter != NULL)
		{
			result.addLast(rhsIter->coef, rhsIter->exponent);
			rhsIter = rhsIter->next;
		}

		return result;
	}
	catch (string s)
	{
		cout << s << endl;
		Poly<T> sentinel;
		sentinel.addFirst(~0, ~0); //put in a sentinel value

		return sentinel;
	}
}

//multiplies one polynomial with another. Returns a result polynomial
//that is still in order 
template <typename T> 
Poly<T> Poly<T>::multiply(const Poly<T> &rhs)
{
	try
	{
		Node<T> *thisIter = this->head;
		Node<T> *rhsIter = rhs.head;

		int newSize = this->numTerms * rhs.numTerms;
		int k = 0;

		/*
			DESIGN DECISION1 : Instead of already creating a linked list from the first embedded for loops right below this,
			I am going to create an array at this point despite the extra space for the new array.
			
			Justification: I have to perform mergesort on the list anyways to efficiently add up the 
			terms with the same exponents; running mergesort is much faster on an array than a linked list 
			since array has indexing capabilities. 

		*/
		long int **terms = new long int*[newSize];

		for (unsigned int i = 0; i < this->numTerms; i++)
		{
			rhsIter = rhs.head; // reset for every time 

			for (unsigned j = 0; j < rhs.numTerms; j++)
			{
				terms[k] = new long int[2];
				terms[k][0] = thisIter->coef * rhsIter->coef;
				terms[k][1] = thisIter->exponent + rhsIter->exponent;

				//check for overflow
				if (terms[k][0] / thisIter->coef != rhsIter->coef ||
					terms[k][1] - thisIter->exponent != rhsIter->exponent)
				{
					terms[k][0] = ~0; // sentinel
					terms[k][1] = ~0; // sentinel
					throw string("Overflow Detected in multiply");
				}
	
				k++;

				rhsIter = rhsIter->next;
			}
			thisIter = thisIter->next;
		}
		
		mergeSort(terms, 0, newSize - 1);


		
		Poly<T> result;

		unsigned i = 0;
		unsigned j = 1;

		// add slots that have same exponents and populate resulting Polynomial 
		// NOTE: There are embedded while loops, but the iterator i and j are efficiently inclemented 
		// to still run in O(n)
		while (i < newSize)
		{
			j = 1; 

			if (i + j < newSize) // to not check terms[newSize], which would be out of bounds
			{
			
				if (terms[i][1] == terms[i + j][1]) //check if next term has same exponent 
				{
					while (terms[i][1] == terms[i + j][1]) 
					{
						// if same exponent, iterate through to add up all the terms with same exponent
						
						long int oldTerm2 = terms[i][0]; //needed to check for overflow later
														//since we are about to change terms[i][0]
						terms[i][0] += terms[i + j][0];

						//check overflow
						if (terms[i][0] - oldTerm2 != terms[i + j][0])
						{
							terms[i][0] = ~0;
							terms[i][1] = ~0; //set to sentinel, not needed since already returning sentinel
											// in catch statement but might as well
							throw string("Overflow Detected in Multiply!");
						}

						if (i + j >= newSize) // to not check out of bounds for the array 
						{
							i = newSize;
							break; //just break out of while loop
						}
						else
						{
							j++;
						}
					}
					result.addFirst(terms[i][0], terms[i][1]);
					i += j; //to iterate through any ones that we have already looked through 
				}
				else //if next term's coefficient is not the same, simply add it to the result
				{
					result.addFirst(terms[i][0], terms[i][1]);
					i++;
				}
			}
			else // to take care of one last slot 
			{
				result.addFirst(terms[i][0], terms[i][1]);
				i++; 
			}
		}

		// deallocate terms 
		for (int i = 0; i < newSize; i++)
		{
			delete[] terms[i];
		}

		delete[] terms;	
		
		return result;

	}
	catch (string s)
	{
		cout << s << endl;
		Poly<T> sentinel;
		sentinel.addFirst(~0, ~0); //put in a sentinel value

		return sentinel;
	}
	
}

//exponentiate a polynomial to an integer value 
template <typename T>
Poly<T> Poly<T>::exponentiation(const int exp)
{
	if (exp < 0)
	{
		cout << "can't exponentiate to a negative number. Returning as is." << endl;
		return *this; 
	}
	
	if (exp == 0)
	{
		Poly<T> one;
		one.addFirst(1, 0);
		return one; 
	}

	if (exp == 1)
		return *this;
	
	Poly<T> result = *this;  // must compute on result to not edit the original poly

	// exponentiation by squaring implementation

	if (exp % 2 == 0) //even exponent: (x^2)^(n/2)
	{
		result = result.multiply(result);
		Poly<T> squared = result; // holds 'x^2'
		for (int i = 1; i < exp / 2; i++)
		{
			result = result.multiply(squared);
		}
	}
	else // odd exponent: x (x^2)^((n-1)/2)
	{
		Poly<T> x = *this; //holds 'x'
		result = result.multiply(result);
		Poly<T> squared = result; 

		for (int i = 1; i < (exp - 1) / 2; i++)
		{
			result = result.multiply(squared);
		}
		result = result.multiply(x);
	}

	return result; 
}

//evaluates polynomial for x= whatever input value 
template <typename T> 
long int Poly<T>::eval(const int num)
{
	try
	{
		Node<T> *current = this->head;

		long int result = 0;

		while (current != NULL)
		{
			long int oldResult = result; //needed to check for overflow later 
			result += current->coef * pow(num, current->exponent);
			
			//check for overflow error on the result 
			if (oldResult != result - current->coef * pow(num, current->exponent))
			{
				result = ~0; //set to sentinel 
				throw string("Overflow detected in eval");
			}

			current = current->next;
		}	
		
		return result; 
	}
	catch (string s)
	{
		cout << s << endl;
		return ~0; //return sentinel 
	}

	
}

// my own implementation of cmath's pow. I have this because I wasn't sure if we were
// allowed to use cmath's pow function. 
template <typename T> 
long int Poly<T>::pow( int base, int exp)
{
	try
	{
		if (exp == 1)
			return base;
		
		long int result = 1;
		while (exp > 0) {

			if ( exp & 1 ) 
			{ 
				long int oldResult = result; 
				result = (result*base);

				if (oldResult != result / base) //check for overflow 
				{
					result = ~0; //set to sentinel 
					throw string("Overflow Detected in Pow Function! Value will not be correct!");
				}

			}
			long int oldBase = base; 
			base = (base * base);
			exp >>= 1;

			// need the old values to check for int overflow error 
			if (oldBase != base / oldBase)
			{
				result = ~0; //set to sentinel 
				throw string("Overflow Detected in Pow Function! Value will not be correct!"); 
			}

		}

		return result;
	}
	catch (string s)
	{
		cout << s << endl; 
		return ~0; //return sentinel
	}
}


//merge sort on array 
template <typename T>
void Poly<T>::merge(long int** arr, int left, int mid, int right)
{
	int i, j, k;
	int leftSize = mid - left + 1;
	int rightSize = right - mid;

	// initialize temporary arrays


	long int** Left = new long int*[leftSize];
	long int** Right = new long int*[rightSize];

	// Copy data to these temporary arrays
	for (i = 0; i < leftSize; i++)
		Left[i] = arr[left + i];

	for (j = 0; j < rightSize; j++)
		Right[j] = arr[mid + 1 + j];



	// index for first subarray, used for left half of the array
	i = 0;

	// index for second subarray, used for right half of the array
	j = 0;

	// index of the merged subarray
	k = left;

	// Merge the temp arrays back into array 
	while (i < leftSize && j < rightSize)
	{
		//compare exponents 
		if (Left[i][1] <= Right[j][1])
		{
			arr[k] = Left[i];
			i++;
		}
		else
		{
			arr[k] = Right[j];
			j++;
		}
		k++;
	}

	// Copy the remaining elements of Left subarray, if there are still any left
	while (i < leftSize)
	{
		arr[k] = Left[i];
		i++;
		k++;
	}

	// Do the same, but for right side, if any left 
	while (j < rightSize)
	{
		arr[k] = Right[j];
		j++;
		k++;
	}
}

template <typename T>
void Poly<T>::mergeSort(long int** arr, int left, int right)
{
	if (left < right)
	{
		int mid = left + (right - left) / 2;

		mergeSort(arr, left, mid);
		mergeSort(arr, mid + 1, right);
		merge(arr, left, mid, right);
	}
}


#endif