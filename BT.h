#pragma once
#include "Node.h"
#include <cmath>
#include <algorithm>

using std::cout;
using std::endl;


template <class T>
class BT
{
private:
	void insert(T e, Node<T>* &x, Node<T>* &parent);
	void split(T e, Node<T>* &x, Node<T>* &parent);

	void printOrder(Node<T>* &x);
	void printLevel(Node<T> *&x, int level);
	bool find(T e, Node<T>* &x);

	bool remove(T e, Node<T>* &x);
	void clearTree(Node<T>* &x);

	int minElems;

public:
	Node<T> *root;
	int bf; //branching factor
	BT();
	BT(int b);
	~BT();

	void insert(T e);
	void print();
	bool find(T e) { return find(e, root); }
	void remove(T e);
	int height(Node<T>* &x);
	void clearTree() { clearTree(root); }


};


template <typename T>
BT<T>::BT()
{
	bf = 5;
	root = new Node<T>(bf);
	root->ptrs = 0;
	minElems = (bf - 1) / 2;

}

template <typename T>
BT<T>::BT(int b)
{
	bf = b;
	root = new Node<T>(bf);
	root->ptrs = 0;
	minElems = (bf - 1) / 2;
}


template <typename T>
BT<T>::~BT()
{
	this->clearTree(root);
}

template <typename T>
void BT<T>::insert(T e)
{
	if (this->find(e) == false)
		insert(e, root, root);
	else
		cout << e << " is already in the tree" << endl;;
}




template <typename T> 
void BT<T>::insert(T e, Node<T>* &x, Node<T>* &parent)
{
	if (root == NULL)
		root = new Node<T>(bf);


	if (x->isLeaf()) //x is a leaf
	{
		if (x->elems == bf - 1) //x is full (preform split)
		{
			int i = x->elems - 1; //after loop i should be index to insert new value
			while (i >= 0 && x->data[i] > e)
			{
				x->data[i + 1] = x->data[i];
				//	x->pointers[i + 1] = x->pointers[i + 2];
				i--;
			}
			x->data[i + 1] = e;
			x->elems += 1;

		//	cout << "calling split on node: " << (*x) << endl; //flag
			split(e, x, root);
		}

		else //x is not full (normal insert)
		{
			int i = x->elems - 1; //after loop i should be index to insert new value

			while (i >= 0 && x->data[i] > e)
			{
				x->data[i + 1] = x->data[i];
				i--;
			}
			x->data[i + 1] = e;
			x->elems += 1;
		}
	}


	else //x is not a leaf
	{
		int i = x->elems - 1;
		while (i >= 0 && x->data[i] > e)
		{
			i--;
		}
		insert(e, x->pointers[i + 1], x);
	}
}


template <typename T>
void BT<T>::split(T e, Node<T>* &x, Node<T>* &parent)
{
	int i = parent->ptrs - 1;
	while (i >= 0 && parent->pointers[i] != x)
	{
		i--;
	}

	if (i == -1 && x != root) //leaf x is not child of parent
	{
		int j = parent->elems - 1;
		while (j >= 0 && parent->data[j] > e) //find correct pointer to go to next node
		{
			j--;
		}


		split(e, x, parent->pointers[j + 1]); //call again with new node to check if parent
	}
	else //x is child of parent
	{
		Node<T>* newNode = new Node<T>(bf);
		int midInd = ceil(static_cast<double>(x->elems) / 2) - 1;
		int midVal = x->data[midInd];

		int j = 0;
		int i;


		for (i = midInd + 1; i < x->elems; i++) //copy half of elems from node x to newNode DOES NOT COPY POINTERS AS OF YET
		{
			newNode->data[j] = x->data[i];
			j++;
		}
		newNode->elems += j;
		x->elems -= j;
		x->elems -= 1; //subtracting one extra because midindx is move to parent


		//copying pointers
		j = 0;
		for (i = midInd + 1; i < x->ptrs; i++)
		{
			newNode->pointers[j] = x->pointers[i];
			j++;
		}
		x->ptrs -= j;
		newNode->ptrs += j;


		if (x != root)
		{
			int pi = parent->elems - 1; //parent indexer

			while (pi >= 0 && parent->data[pi] > midVal) //inserting mid val into parent node
			{
				parent->data[pi + 1] = parent->data[pi]; //moving data
				parent->pointers[pi + 2] = parent->pointers[pi + 1]; //need to move pointers because parent node will have pointers
				pi--;
			}
			
			parent->data[pi + 1] = midVal; //inserting data
			parent->elems += 1; //incrementing data


			parent->pointers[pi + 2] = newNode; //inserting new child node
			parent->ptrs += 1; //incrementing pointers

			if (parent->elems == bf) //if the parent node is full
			{
				split(midVal, parent, root); //split the parent node too!
			}
		}

		else //x does equal root (make a new root node)
		{
			
			Node<T>* newRoot = new Node<T>(bf);
			newRoot->data[0] = midVal;
			newRoot->elems = 1;

			newRoot->pointers[0] = x;
			newRoot->pointers[1] = newNode;
			newRoot->ptrs = 2;

			root = newRoot;
			root->elems = newRoot->elems;
			root->ptrs = newRoot->ptrs;

		}
	}

}

template <typename T>
void BT<T>::print()
{
	if (root == NULL)
	{
		cout << "there are no elements in the tree" << endl;
	}
	else 
	{
		cout << "print in level order.\nNodes distinguished by | | " << endl << endl;
		printOrder(root);
	}
}

template <typename T>
void BT<T>::printLevel(Node<T>* &x, int level)
{
	if (x == NULL)
		return;
	if (level == 0)
		std::cout << "\t" << *(x);
	else if (level > 0)
	{
		for (int i = 0; i < x->ptrs; i++)
			printLevel(x->pointers[i], level - 1);
		
	}
}

template <typename T>
void BT<T>::printOrder(Node<T>* &x)
{
	int h = height(x);
	
	for (int i = 0; i <= h; i++)
	{
		printLevel(x, i);
		cout << endl;
	}

}


template <typename T>
bool BT<T>::find(T e, Node<T>* &x)
{

	if (x == NULL)
		return false;
	else
	{
		int i = x->elems - 1;

		while (i >= 0 && x->data[i] >= e)
		{
			if (x->data[i] == e) //if e is in this node return true
			 	return true;
			i--;
		}
		find(e, x->pointers[i + 1]); //else call find again with the node it should be in
	}

}




template <typename T>
void BT<T>::remove(T e)
{
	if (!this->find(e))
		cout << "value is not in tree" << endl;
	else
	{
		remove(e, root);
	}
}




//returns true as flag to parent node to signal underflow
//returns false to parent to signal no underflow

template <typename T>
bool BT<T>::remove(T e, Node<T>* &x)
{
	//cases for removing a key from a node.
	//1. If key is in leaf, remove key
	//2. if leaf underflows, combine with more populus neighbor, and the parent key between them (super node)
	//3. if newly created node will underflow if split, then do not split it (parent node loses one key) if it will not underflow, then put median to parent node and split into child nodes
	//4. if parent node will underflow when it loses a key, then combine with most populus neighbor (in same fashion as above)
	//5. Either underflow will continue to root or stop at one of the parent levels
	//6. Underflow at root (occurs when root has only one value), if root is a leaf, then no big deal the tree just becomes empty
	//7. If root is not a leaf, then it will have two children who will both underflow of they split (with root incorperated)
	//8. Therefore, if the root underflows just make the root equal to the new node that contains the root.  
	
	//parent must fix the underflowed node
	//
	bool underFlow = false;
	bool childUnderFlow = false;
	bool inNode = false;
	
	int i = x->elems - 1;



	while (i >= 0 && x->data[i] >= e && !inNode)
	{
		if (x->data[i] == e)
			inNode = true;
		i--;
	}
	if (inNode == false)
	{
		childUnderFlow = remove(e, x->pointers[i + 1]);
		//operations for parent node if flag is true (underflow)
		//if leaf node try to rotate parent key with smallest or largest key in siblings, if this fails then combine with siblings and parent key (decreasing the size of the parent node) *if this signals underflow then report this to parent.
		//if parent node then find immediate predecessor or sucessor and replace the node to delete with that key, then call delete on the node which the predecessor is in and the predecessor as parameters
		//if the parent node underflows check if root.  if root, it needs to take care of it itself, if it is not root then it passes it on to its parent
		
		if (childUnderFlow == true)
		{
			cout << "child of node " << *x << "has underflowed" << endl;
			int siblingInd = 0;
			if (i+1 == 0) //if key to delete is in first child node
				siblingInd = 1; //use second child node as sibling
			else if (i+1 == x->ptrs - 1) //if key to delete is in last child node
				siblingInd = x->ptrs - 2; //use second to last child node as sibling
			else //two possible siblings to use
			{
				if (x->pointers[i + 2]->elems <= x->pointers[i]->elems) //use the one that has more elements
					siblingInd = i;
				else
					siblingInd = i + 2;
			}

			if (x->pointers[siblingInd]->elems - 1 >= minElems) //has enough to share with sibling no further underflow will occur
			{
				if (siblingInd > i+1)
				{
					x->pointers[i+1]->data[x->pointers[i+1]->elems] = x->data[i+1];
					x->data[i+1] = x->pointers[siblingInd]->data[0];
					//shift sibling data and decrement sibling->elem
					int shift = 0;

					while (shift < x->elems) //shifting remaining elements over (to the left)
					{
						x->pointers[siblingInd]->data[shift] = x->pointers[siblingInd]->data[shift + 1];
						shift++;
					}
					x->pointers[siblingInd]->elems -= 1;
					x->pointers[i+1]->elems += 1;
				}
				else
				{
					//shift i data before insert
					int shift = x->pointers[i+1]->elems;
					while (shift > 0) //shifting remaining elements over (to the right)
					{
						x->pointers[i+1]->data[shift] = x->pointers[i+1]->data[shift - 1];
						shift--;
					}

					x->pointers[i+1]->data[0] = x->data[siblingInd];


					//shift = x->elems;
					//while (shift > 0) //shifting elements in parent node over (to the right)
					//{
					//	x->data[shift] = x->data[shift - 1];
					//	shift--;
					//}

					x->data[siblingInd] = x->pointers[siblingInd]->data[x->pointers[siblingInd]->elems-1];

					x->pointers[siblingInd]->elems -= 1;
					x->pointers[i+1]->elems += 1;
				}
				

			}
			else //does not have enough, must combine (possible underflow)
			{
				

				//copying all elements into pointer connected to i+1 pointer of parent (node than contains the underflow)
				int parentIn = std::min(i + 1, siblingInd);
				//putting all keys into node out of parent pointer i+1
				int in = x->pointers[i+1]->elems -1; //after loop i should be index to insert new value
				//cout << "parent index " << parentIn << endl;
				//cout << "parent " << *(x) << endl;
				//out << *(x->pointers[i + 1]) << endl;

				while (in >= 0 && x->pointers[i + 1]->data[in] > e)
				{
					x->pointers[i + 1]->data[in + 1] = x->pointers[i + 1]->data[in];
					in--;
				}
				x->pointers[i + 1]->data[in + 1] = x->data[parentIn];
				x->pointers[i + 1]->elems += 1;

				cout << *(x->pointers[i + 1]) << endl;

				for (int k = 0; k < x->pointers[siblingInd]->elems; k++)
				{
					in = x->pointers[i + 1]->elems - 1; //after loop i should be index to insert new value

					while (in >= 0 && x->pointers[i + 1]->data[in] > x->pointers[siblingInd]->data[k])
					{
						x->pointers[i + 1]->data[in + 1] = x->pointers[i + 1]->data[in];
						in--;
					}
					x->pointers[i + 1]->data[in + 1] = x->pointers[siblingInd]->data[k];
					x->pointers[i + 1]->elems += 1;
				}

				
				//cout << *(x->pointers[i + 1]) << endl << endl << endl;
		



				delete x->pointers[siblingInd];

				x->pointers[parentIn] = x->pointers[i+1]; //attaching node to tree
				x->ptrs -= 1;



				int y = parentIn;
				while (y < x->elems - 1) //shifting remaining elements over
				{
					x->data[y] = x->data[y + 1];
					y++;
				}

				x->elems -= 1;

				cout << endl << endl << endl;
				print();
				cout << endl << endl << endl;

				
				y = siblingInd;
				while (y < x->ptrs) //shifting remaining elements over
				{
					x->pointers[y] = x->pointers[y + 1];
					y++;
				}


				cout << endl << endl << endl;
				print();
				cout << endl << endl << endl;

				if (x == root)
				{
					
					//x->elems = x->pointers[parentIn]->elems;
					//x->ptrs = x->pointers[parentIn]->ptrs;
				
				}



				//check if root here
				if (x->elems < minElems)
					underFlow = true;
				else
					underFlow = false;
				return underFlow;

			}
		}
		else
		{
			//do nothing
		}
	}
	else //key to delete is in this node
	{
		cout << e << " has been found in node " << *(x) << endl; //flag

		int j = 0;
		while (x->data[j] != e) //finding element number of node to delete
			j++;

		if (x->isLeaf()) //delete node and if less than min number of keys signal underflow to parent
		{
			while (j < x->elems - 1) //shifting remaining elements over
			{
				x->data[j] = x->data[j + 1];
				j++;
			}

			x->elems -= 1; //decrementing number of elements because of the elemented that has been deleted.

		//	cout << "the minimum number of elements is :" << minElems << endl;
			if (x->elems < minElems) //after remove if the node has less than the minimum number of nodes, then return flag to parent.
				underFlow = true;
			else
				underFlow = false;


			//if (underFlow == true)
			//	cout << *x << " has underflowed";

			if (x == root && x->elems == 0)
			{
				//cout << "root has 0 values" << endl; //flag
				underFlow = true;
				delete root;
				root = NULL;
			}

			return underFlow;
		}

		else //Node is not a leaf node, need to find immediate successor or predecessor to swap and then call delete on that node (a leaf node) n
		{
			Node<T>* next;
			next = x->pointers[j]; 
			
			while (next->isLeaf() == false) //finding immediate predecessor 
				next = next->pointers[next->ptrs - 1];

			x->data[j] = next->data[next->elems - 1]; //replacing value to be deleted with value at predecessor

			remove(next->data[next->elems - 1], next); //calling delete on the predecessor
		}
	}
}

template <typename T>
int BT<T>::height(Node<T>* &x)
{
	if (x == NULL)
		return -1;
	else
	{
		int height = this->height(x->pointers[0]);

		return height + 1;
	}

}

template <typename T>
void BT<T>::clearTree(Node<T>* &x)
{
	if (x == NULL)
	{

	}
	else
	{
		for (int i = 0; i < x->ptrs; i++)
			clearTree(x->pointers[i]);
		delete x;

	}
}
