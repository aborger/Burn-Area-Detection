#include "List.h"
template <class ItemType>
struct Node
{
	ItemType data;
	Node *next;
	Node *prev;
};
template <class ItemType>
List<ItemType>::List() {
	head = NULL;
	tail = NULL;
	current = head;
}

// Creates a new node at the end of the list and fills it with "newData"
template <class ItemType>
void List<ItemType>::newNodeTail(ItemType newData) {
	Node *newNode = new Node;
	newNode->data = newData;
	newNode->next = NULL;
	newNode->prev = NULL;
	if (head == NULL) { // if head is empty put as head and tail
		head = newNode;
		tail = newNode;
		newNode = NULL;
	}
	else { // if not, old tail next = newNode, and tail now = newNode
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
	}
}

template <class ItemType>
void List<ItemType>::newNodeHead(ItemType newData) {
	Node *newNode = new Node;
	newNode->data = newData;
	newNode->next = NULL;
	newNode->prev = NULL;
	if (head == NULL) { // if head is empty put as head and tail
		head = newNode;
		tail = newNode;
		newNode = NULL;
	}
	else { // if not, old tail next = newNode, and tail now = newNode
		head->prev = newNode;
		newNode->next = head;
		head = newNode;
	}
}
// Puts Nodes in order as they are created
template <class ItemType>
void List<ItemType>::newSortedNode(ItemType newData) {
	Node *newNode = new Node;
	newNode->data = newData;
	newNode->next = head; // puts new node "behind" head
	if (head == NULL) { // if head is empty put as head and tail
		head = newNode;
		tail = newNode;
		newNode->next = NULL;
		newNode->prev = NULL;
		newNode = NULL;
	}
	else {
		if (newData <= head->data) { // if newData is less than head, make newData head and point to old head
			newNode->next = head;
			head->prev = newNode;
			head = newNode;
		}
		else {
			if (newNode->next->next == NULL) { // only head and new node in list
				head->next = newNode;
				newNode->prev = head;
				newNode->next = NULL;
				tail = newNode;
			}
			else {
				newNode->next = head->next; //move newNode to the right of head
				head->next->prev = newNode;
				head->next = newNode;
				newNode->prev = head;

				bool breakLoop = false;
				do {
					if (newData <= newNode->next->data) {
						// not sure whether or not to delete newNode, not deleting might create leak, deleting might corrupt
						// newNode is in the right spot
						breakLoop = true;
					}
					else {
						// moves newNode to the right of newNode->next, but instead of changine its nexts and previouses it swaps the data
						ItemType temp = newNode->data;
						newNode->data = newNode->next->data;
						newNode->next->data = temp;
						newNode = newNode->next;
					}
				} while (newNode->next != NULL && breakLoop == false);
			}
		}
	}
}
template <class ItemType>
int List<ItemType>::getListSize() {
	int size = 0;
	Node *nextNode = head;
	while (nextNode != NULL) {
		nextNode = nextNode->next;
		size++;
	}
	return size;
}

template <class ItemType>
void List<ItemType>::setAt(int element, ItemType newData) {
	Node *temp = head;
	for (int a = 0; a < element; a++) {
		temp = temp->next;
	}
	if (temp == NULL) {
		cout << "Element was empty" << endl;
	}
	temp->data = newData;
}

template <class ItemType>
void List<ItemType>::setCurrent(ItemType newData) {
	current->data = newData;
}

template <class ItemType>
ItemType List<ItemType>::getCurrent() {
	return current->data;
}

template <class ItemType>
void List<ItemType>::nextCurrent() {
	current = current->next;
}

template <class ItemType>
void List<ItemType>::tailCurrent() {
	current = tail;
}

template <class ItemType>
void List<ItemType>::prevCurrent() {
	current = current->prev;
}

template <class ItemType>
void List<ItemType>::resetCurrent() {
	current = head;
}


template <class ItemType>
ItemType List<ItemType>::getAt(int element) {
	Node *temp = head;
	for (int a = 0; a < element; a++) {
		temp = temp->next;
	}
	if (temp == NULL) {
		//cout << "Element was empty" << endl;
		//return 0;
	}
	return temp->data;
	delete temp;
}

template <class ItemType>
ItemType List<ItemType>::getTail() {
	return tail->data;
}

/*template <class ItemType>
ItemType List<ItemType>::getNext(Node current) {
	return current.next;
}

template <class ItemType>
ItemType List<ItemType>::getData(Node current) {
	return current.data;
}*/

template <class ItemType>
void List<ItemType>::deleteAt(int element) {
	if (head != NULL) {
		Node *temp = head;
		if (temp != NULL) { // make sure list isnt empty
			for (int a = 0; a < element; a++) {
				temp = temp->next;
			}
			if (head == tail) { // delete only element
				head = NULL;
				delete temp;
			}
			else if (temp->next == NULL) {// delete last element in list
				temp->prev->next = NULL;
				tail = temp->prev;
				delete temp;
			}
			else if (element == 0) {// delete head
				temp->next->prev = NULL;
				head = temp->next;
				delete temp;
			}
			else {
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				delete temp;
			}
		}
	}
}

template <class ItemType>
void List<ItemType>::deleteTail() {
	if (head != NULL) {
		tail = tail->prev;
		delete tail->next;
		tail->next = NULL;
	}
}

// If searchTerm is in list, returns the items element, if not returns -1
template <class ItemType>
int List<ItemType>::contains(ItemType search) {
	Node *temp = head;
	int currentElement = 0;
	while (temp != NULL) {
		if (temp->data == search) { // check if searchTem has been found
			return currentElement;
		}
		else { // go to next element
			temp = temp->next;
			currentElement++;
		}
	}
	// not in list
	delete temp;
	return -1;
}

template <class ItemType>
List<ItemType>::~List() {}