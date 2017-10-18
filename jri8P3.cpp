

#include<fstream>
#include<string>
#include<iostream>
#include<sstream>
#include "BT.h"
#include "Node.h"


using namespace std;
template <typename T>
void readFile(string, BT<T>&);
template <typename T>
void loadTree(string, BT<T>&);

const string A = "A:"; //adds key
const string L = "L:"; //loads file
const string D = "D:"; //removes key
const string S = "S:"; //finds key
const string P = "P:"; //Prints tree
const string T = "T:"; //Terminates code


int main(int argc, const char * argv[])
{
	int branchingFactor = 5;

	string fname = argv[1];

	cout << "Please eneter a number greater than 2 for the branching factor: ";
	//if branching factor = 2 then the max elems is 1 and min elems is 0
	//Just use a binary search tree if you want to do that
	
	cin >> branchingFactor;
	cin.ignore(500, '\n');

	while (branchingFactor <= 2) //loops until value is greater than 2
	{
		cout << "\nPlease try again: ";
		cin >> branchingFactor;
		cin.ignore(500, '\n');
	}


	

	BT<int> Btree(branchingFactor);


	readFile(fname, Btree); 


	cout << "[Enter any key to end program]" << endl;
	cin.get();
	return 0;
}












template <typename T>
void readFile(string fname, BT<T> &Btree)
{
	ifstream inFile;
	inFile.open(fname.c_str());
	stringstream stream;
	string line;
	string code;
	int key;
	int data;
	if (!inFile)
	{
		cout << "The file name and path is not vaild.\nPlease try again" << endl;
	}
	else
	{
		while (getline(inFile, line))
		{
			cout << line << endl;
			if (line[0] == 'A') //add value to tree
			{
				line = line.substr(3);
				stream << line;
				stream >> key;
				cout << "adding key " << key << " in Btree" << endl;
				Btree.insert(key);
				stream.clear();
			}
			else if (line[0] == 'D') //Delete value from tree NEED TO IMPLEMENT!!!!!!
			{
				line = line.substr(3);
				stream << line;
				stream >> key;
				cout << "deleting key " << key << " from Btree" << endl;

				Btree.remove(key);

				stream.clear();
			}
			else if (line[0] == 'L') //loads data into tree from separate file
			{
				line = line.substr(3);
				cout << "Loading data from file " << line << endl;
				loadTree(line, Btree);
				
			}
			else if (line[0] == 'P') //prints tree
			{
				cout << "Printing Btree" << endl;
				Btree.print();
				cout << endl;
			}
			else if (line[0] == 'S') //searches for value in tree
			{
				line = line.substr(3);
				stream << line;
				stream >> key;
				cout << "searching for key " << key << " in Btree" << endl;

				if (Btree.find(key))
					cout << key << " is in the Tree" << endl;
				else
					cout << key << " is not in the Tree" << endl;

				stream.clear();
			}
			else if (line[0] == 'T') //Terminates program
			{
				inFile.close();
				cout << "Terminating Program" << endl;
			}
			else //Not valid input
			{
				cout << line[0] << " is not a valid code!" << endl;
			}
		}
	}
}

template <typename T>
void loadTree(string fname, BT<T>& Btree)
{
	ifstream inFile;
	//string str;
	//stringstream stream;
	int key;
	int count = 0;

	inFile.open(fname.c_str());
	if (!inFile)
	{
		cout << "The file name and path is not vaild.\nPlease try again" << endl;
	}
	else
	{
		while (inFile >> key)
		{
			count++;
			Btree.insert(key);
		}


		cout << count << " keys loaded" << endl;
	}

}