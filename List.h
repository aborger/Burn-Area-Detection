#pragma once
#include <iostream>

using namespace std;
// Doubly linked list so sort is faster
// Creates a template so the list can be used again
template <typename ItemType>
class List
{
private:
	// declares a head and tail of list (beginning and end)
	struct Node
	{
		ItemType data;
		Node *next;
		Node *prev;
	};
	Node *head, *tail;
	Node *current; //can change to quickly move through list
public:
	// Constructor sets head and tail to NULL
	List<ItemType>();

	// Creates a new node at the end of the list and fills it with "newData"
	void newNodeTail(ItemType newData);

	void newNodeHead(ItemType newData);
	// Puts Nodes in order as they are created
	void newSortedNode(ItemType newData);
	int getListSize();

	void setAt(int element, ItemType newData);
	void setCurrent(ItemType newData);

	ItemType getCurrent();
	void resetCurrent();
	void tailCurrent();
	void nextCurrent();
	void prevCurrent();
	ItemType getAt(int element);

	//Node getNext(Node current);
	//ItemType List<ItemType>::getData(Node current);

	void deleteAt(int element);

	// If searchTerm is in list, returns the items element, if not returns -1
	int contains(ItemType search);


	~List<ItemType>();



};

#include "List.cpp"