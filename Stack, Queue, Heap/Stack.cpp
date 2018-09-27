#include <iostream>
using namespace std;

#define SIZE	50

class Node {
public:
	Node* next;
	Node* prev;
	int n;

	Node() {
		next = NULL;
		prev = NULL;
	}
};

class Stack_array {
private:
	int* arr;
	int size = 0;
	int max;

public:
	Stack_array() {
		arr = new int[SIZE];
		max = SIZE;
	}

	Stack_array(int N) {
		arr = new int[N];
		max = N;
	}

	~Stack_array() {
		delete arr[];
	}

	int top() {
		if (empty())
			return -1;
		else
			return arr[size - 1];
	}

	bool empty() {
		if (size == 0)
			return 1;
		else
			return 0;
	}

	bool full() {
		if (size == max)
			return 1;
		else
			return 0;
	}


	bool push(int n) {
		if (full())
			return 0;

		arr[size] = n;
		size++;
		return 1;
	}

	void pop() {
		if (empty())
			return;

		size--;
	}

};

class Stack_list {
private:
	Node* root;
	Node* tail;

public:
	Stack_list() {
		root = NULL;
		tail = NULL;
	}

	~Stack_list() {
		Node* node = root;
		while (node != NULL) {
			Node* temp = node;
			node = node->next;

			delete temp;
		}
	}

	int top() {
		if (empty())
			return -1;
		else
			return tail->n;
	}

	bool empty() {
		if (root == NULL)
			return 1;
		else
			return 0;
	}

	void push(int n) {
		Node* node = new Node;
		node->n = n;

		if (root == NULL && tail == NULL) {
			root = node;
			tail = node;
		}
		else {
			tail->next = node;
			node->prev = tail;
			tail = node;
		}
	}

	void pop() {
		if (empty())
			return;

		Node* node = tail;

		if (root == tail) {
			root = NULL;
			tail = NULL;
		}
		else {
			tail = tail->prev;
			tail->next = NULL;
		}

		delete node;
	}

};

int main() {
	Stack_array s1;
	Stack_list s2;

	s1.push(1);
	s1.push(2);

	s2.push(3);
	s2.push(4);

	cout << s1.top() << endl;
	cout << s2.top() << endl;

	s1.pop();
	s2.pop();

	cout << s1.top() << endl;
	cout << s2.top() << endl;

}