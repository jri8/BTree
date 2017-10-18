#pragma once

template<class T>
class Node
{
	template <typename F>
friend std::ostream& operator <<(std::ostream &os, const Node<F> &rhsobj);

private:



public:
	T* data; //pointer to array that holds data (numbers)
	Node** pointers; //pointer to array that holds pointers to other nodes
	int m; //branching factor m pointers, m-1 data members 

	int elems; //number of elements in data array needs to be bewteen ceil((m-1)/2) and m-1
	int ptrs; //number of pointers in pointer array needs to be between ceil(m/2) and m children

	bool isLeaf();

	Node();
	Node(int x);
	~Node();

};

template <typename T>
Node<T>::Node()
{
	m = 5;
	data = new T[m]; 

	pointers = new Node*[m + 1];
	for (int i = 0; i < m + 1; i++)
		pointers[i] = NULL;

	elems = 0;
	ptrs = 0;
}

template <typename T>
Node<T>::Node(int x)
{
	m = x;
	data = new T[m]; //extra space in array for spliting array
	pointers = new Node*[m + 1];
	for (int i = 0; i < m + 1; i++)
		pointers[i] = NULL;

	elems = 0;
	ptrs = 0;
}

template <typename T>
Node<T>::~Node()
{
	delete data;
	delete pointers;
}

template <typename T>
bool Node<T>::isLeaf()
{
	if (ptrs == 0)
		return true;
	else
		return false;
}

template <typename F>
std::ostream& operator<<(std:: ostream &os, const Node<F> &rhsobj)
{
	os << "|";
	for (int i = 0; i < rhsobj.elems; i++)
	{
		os << " " << rhsobj.data[i] << " " ;
	}
	os << "|";
	return os;
}