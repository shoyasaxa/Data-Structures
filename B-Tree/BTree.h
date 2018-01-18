#ifndef BTREE_H
#define BTREE_H

#include "BTreeNode.h"
#include <string>
#include <iostream>

using namespace std; 

template <int M>
class BTree
{
public:
	BTree();
	~BTree();

	void splitChildBTree(Node<M>*, Node<M>*, int);
	void insertNonFullBTree(Node<M>*, int);
	void insertBTree(int);

	void printInternal(const Node<M>*) const; 
	void printInOrder(const Node<M>*) const; 

	void remove(Node<M>*, int);
	void removeFromNonLeafNode(Node<M>*, int);  
	void takeFromLeftSibling(Node<M>*, int);
	void takeFromRightSibling(Node<M>*, int);
	void merge(Node<M>*, int);

	Node<M> *root; 

private:
	const static int degree = M; 
};


/* Project Discussion

One thing I worked the most and had an epiphany about was splitting/merging node after insert/delete, as opposed to split/merge before insert/delete, which is what class notes tell us to do.
If you split/merge before insert/delete, in some cases, you can actually end up with less than min number of required keys needed. For example, if degree = 5 and the tree was simply [1,2,3,4], and 
you wanted to add a key 5, if you split/merge before insert/ delete, the tree would actually become 

    2                       when it should be    3 
  1   3 4 5                                  1 2   4 5

  So, if we split/merge after insert/delete, the root will be [1,2,3,4,5], then you can proceed to split correctly. 



  Also, I have implemented this project using integer templating so that we can allocate the children and keys on the stack and not on the heap. This will help significantly the 
  number of memory access. 
	

*/



//default constructor 
template <int M>
BTree<M>::BTree()
{
	root = NULL;
}


// deconstructor 
template <int M>
BTree<M>::~BTree()
{  
	delete this->root;
}


template <int M>
void BTree<M>::splitChildBTree(Node<M>* parent, Node<M> *child, int index)
{
	Node<M> * newNode = new Node<M>;
	newNode->isLeaf = child->isLeaf; 
	newNode->numKeys = (degree)/2;  

	for (int i = 0; i < degree / 2; i++)
	{
		newNode->keys[i] = child->keys[i + (degree+1) / 2];
	}

	if (child->isLeaf == false )
	{
		for (int i = 0; i < degree/2+1; i++) 
		{
			newNode->children[i] = child->children[i + (degree+1) / 2];
			newNode->numChildren++;
			child->numChildren--;
		}
	}

	child->numKeys = (degree-1)/2;
	
	for (int i = parent->numKeys - 1; i >= index; i--)
	{
		parent->keys[i + 1] = parent->keys[i];
	}

	parent->keys[index] = child->keys[(degree -1)/2];

	parent->numKeys = parent->numKeys + 1; 

	//add newNode to parent's children 
	parent->numChildren++;

	for (int i = parent->numChildren - 1; i >= index + 1; i--)
	{
		parent->children[i + 1] = parent->children[i];
	}
	
	parent->children[index + 1] = newNode;

}


template <int M>
void BTree<M>::insertNonFullBTree(Node<M>* node, int key)
{
	int i = node->numKeys;

	if (node->isLeaf)
	{
		while (i >= 1 && key < node->keys[i-1])
		{
			node->keys[i] = node->keys[i-1];
			i--;
		}

		node->keys[i] = key;
		node->numKeys = node->numKeys+1;

	}
	else
	{
		while (i >= 1 && key < node->keys[i - 1])
		{
			i--;
		}
		i++;
		insertNonFullBTree(node->children[i - 1], key);

		// if insertion above caused node to have more keys than it should, split
		if (node->children[i - 1]->numKeys == degree)
		{
			splitChildBTree(node, node->children[i - 1], i - 1);
		}

	}
}

template <int M>
void BTree<M>::insertBTree(int key)
{
	if (this->root == NULL)
	{
		// create Root
		root = new Node<M>;
		root->isLeaf = true; 
		root->keys[0] = key;
		root->numKeys = 1;
	}
	else
	{
		insertNonFullBTree(this->root, key);

		// if insert above caused root to have one more key than it should
		if (root->numKeys == degree) 
		{
			Node<M> *newRoot = new Node<M>;
			newRoot->isLeaf = false;
			newRoot->children[0] = root;
			newRoot->numChildren++; 

			splitChildBTree(newRoot, root, 0);
			root = newRoot;
		}
	}

}


template <int M>
void BTree<M>::printInternal(const Node<M> *node) const
{ 
	int i;
	cout << "[";
	//cout << "isLeaf: " << node->isLeaf << "| ";
	//cout << "numChildren: " << node->numChildren << "| ";
	//cout << "numKey: " << node->numKeys << "| ";
	
	for (i = 0; i < node->numKeys; i++)
	{
		cout << node->keys[i];
		
		if (i != node->numKeys-1)
			cout << ",";
	}
	
	for (i = 0; i < node->numChildren; i++)
	{
		printInternal(node->children[i]);
	}
	cout << "]";

}

// I implemented this just to accompany assignment 6 
// prints keys from lowest to highest
template <int M> 
void BTree<M>::printInOrder(const Node<M>*node) const
{
	int i;
	for (i = 0; i < node->numKeys; i++)
	{
		if (node->isLeaf == false)
			printInOrder(node->children[i]);
		cout << " " << node->keys[i];
	}

	if (node->isLeaf == false)
		printInOrder(node->children[i]); 
	
}



// ------------------------------------------------------------------------------------------------
// -------------------------------------REMOVE FUNCTIONS BELOW-------------------------------------
// ------------------------------------------------------------------------------------------------

template <int M> 
void BTree<M>::remove(Node<M> *node,int key)
{
	int index = 0;
	while (index < node->numKeys && node->keys[index] < key)
	{
		index++;
	}

	if (index < node->numKeys && node->keys[index] == key)
	{
		if (node->isLeaf) //the key is in the leaf
		{
			for (int i = index + 1; i < node->numKeys; i++)
			{
				node->keys[i - 1] = node->keys[i];
			}
			
			node->numKeys--;
		}
		else
		{  // else, key is in nonLeaf, so recursively go down
			removeFromNonLeafNode(node, index);
		}
	}
	else
	{

		if (node->isLeaf)
		{
			cout << "Key is not found in the tree. Returning" << endl;
			return;
		}

		if (index > node->numKeys) 
		{
			remove(node->children[index - 1], key);
		}
		else
		{
			remove(node->children[index], key); 
		}

		if (node->children[index]->numKeys < (degree - 1) / 2) 
		{

			// if left sibling has more keys than the min requirement, take from it 
			if (index != 0 && node->children[index - 1]->numKeys >= (degree + 1) / 2)
			{ 
				takeFromLeftSibling(node, index);
			}

			// take from right sibling if right sibling has more than min requirement of keys
			else if ( (index != node->numKeys) && (node->children[index + 1]->numKeys >= (degree + 1) / 2))
			{
				takeFromRightSibling(node, index);
			}
			else
			{
				// else, merge 
				if (index != node->numKeys)
				{
					merge(node, index);
				}
				else
				{
					merge(node, index - 1);
				}
			}
		}

		// SPECIAL CASE: If root has 0 keys and root needs to be demoted
		if ((root->numKeys == 0) && (root->isLeaf ==false) )
		{
			root = root->children[0]; // move down root, stack will deallocate it later for me 
		}
	}
}

template <int M>
void BTree<M>::removeFromNonLeafNode(Node<M>* node, int index) //index?
{
	int key = node->keys[index];

	//if predecessor has more than min requirement of keys, get the most RightKey from it
	if (node->children[index]->numKeys >= (degree+1)/2)
	{
		// recursively move down until you reach the most right key
		Node<M> *traversal = node->children[index];
		while (!traversal->isLeaf)
			traversal = traversal->children[traversal->numKeys];

		int mostRightKey = traversal->keys[traversal->numKeys - 1];

		node->keys[index] = mostRightKey;
		this->remove(node->children[index], mostRightKey);
	}

	// same thing, but get most left key if successor has more than min requirement of keys
	else if (node->children[index + 1]->numKeys >= (degree+1)/2)
	{
		// recursively move down until you reach mostLeftKey
		Node<M> *traversal = node->children[index + 1];
		while (!traversal->isLeaf)
			traversal = traversal->children[0];
		int mostLeftKey = traversal->keys[0]; 
		
		node->keys[index] = mostLeftKey;
		this->remove(node->children[index+1], mostLeftKey); 
	}

	//else, must merge 
	else
	{
		merge(node, index);
		remove(node->children[index], key);
	}
}

// if left sibling has more keys than required, take from it 
template <int M>
void BTree<M>::takeFromLeftSibling(Node<M>*node , int index)
{
	Node<M>*child = node->children[index];
	Node<M> *childSibling = node->children[index-1];

	for (int i = child->numKeys - 1; i >= 0; --i) 
		child->keys[i + 1] = child->keys[i];

	if (child->isLeaf == false)
	{
		for (int i = child->numKeys; i >= 0; --i)
			child->children[i + 1] = child->children[i];

		child->numChildren++;
	}

	child->keys[0] = node->keys[index - 1];

	if (node->isLeaf == false)
	{
		if (child->isLeaf == false && childSibling->isLeaf == false)
		{
			child->children[0] = childSibling->children[childSibling->numKeys];
			childSibling->numChildren--;
		}
		
	}
	node->keys[index - 1] = childSibling->keys[childSibling->numKeys - 1];

	child->numKeys++;
	childSibling->numKeys--;

}

// if right sibling has more keys than min requirement, take from it 
template <int M>
void BTree<M>::takeFromRightSibling(Node<M>* node, int index)
{
	Node<M> *child = node->children[index];
	Node<M> *childSibling = node->children[index + 1];

	child->keys[(child->numKeys)] = node->keys[index];

	if (!(child->isLeaf))
	{
		child->children[(child->numKeys) + 1] = childSibling->children[0];
		child->numChildren++;
	}
		
	node->keys[index] = childSibling->keys[0];

	for (int i = 1; i < childSibling->numKeys; ++i)
	{
		childSibling->keys[i - 1] = childSibling->keys[i];
	}

	if (!(childSibling->isLeaf))
	{
		for (int i = 1; i <= childSibling->numKeys; ++i)
			childSibling->children[i - 1] = childSibling->children[i];
	
		childSibling->numChildren--;
	}
	 
	child->numKeys++;
	childSibling->numKeys--;
}

template <int M>
void BTree<M>::merge(Node<M>* node, int index)
{
	Node<M> *child = node->children[index];
	Node<M> *childSibling = node->children[index + 1];

	bool leftSiblingHasLessKeys = false; 

	if (child->numKeys == (degree - 1) / 2)
	{
		child->keys[(degree - 1) / 2] = node->keys[index]; 
	}
	else
	{
		leftSiblingHasLessKeys = true;
		child->keys[(degree- 1)/2 -1 ] = node->keys[index];
	}

	for (int i = 0; i < childSibling->numKeys; ++i)
	{
		child->keys[i + (degree + 1) / 2] = childSibling->keys[i];
		 
		if (leftSiblingHasLessKeys)
		{
			child->keys[i + (degree - 1) / 2 ] = childSibling->keys[i];
		}
		else
		{
			child->keys[i + (degree + 1) / 2 ] = childSibling->keys[i];
		}
		
	}
	
	if (!child->isLeaf)
	{
		for (int i = 0; i < childSibling->numChildren; ++i)
		{
			child->children[i + (child->numChildren)] = childSibling->children[i];
		}
		
		child->numChildren = child->numChildren + childSibling->numChildren;
	}

	for (int i = index + 1; i < node->numKeys; ++i)
	{
		node->keys[i - 1] = node->keys[i];
	}

	for (int i = index + 2; i < node->numChildren; ++i)
	{
		node->children[i - 1] = node->children[i];
	}

	child->numKeys += childSibling->numKeys + 1;
	node->numKeys--;
	node->numChildren--;

}





#endif // !BTREE_H
