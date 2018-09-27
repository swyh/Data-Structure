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

class Queue_array {
private:
	int* arr;
	int size = 0;
	int max_size;

	int front_idx = 0;
	int end_idx = 0;
public:
	Queue_array() {
		arr = new int[SIZE];
		max_size = SIZE;
	}

	Queue_array(int N) {
		arr = new int[N];
		max_size = N;
	}

	bool empty() {
		if (size == 0)
			return 1;
		else
			return 0;
	}

	bool full() {
		if (size == max_size)
			return 1;
		else
			return 0;
	}

	int front() {
		return arr[front_idx];
	}

	void push(int n) {
		if (full())
			return;

		arr[end_idx] = n;
		end_idx = (end_idx + 1) % max_size;
		size++;
	}

	void pop() {
		if (empty())
			return;

		front_idx = (front_idx + 1) % max_size;
		size--;
	}

};

class Queue_list {
private:
	Node* front_node;
	Node* end;
public:
	Queue_list() {
		front_node = NULL;
		end = NULL;
	}

	bool empty() {
		if (front_node == NULL)
			return 1;
		else
			return 0;
	}

	int front() {
		if (empty())
			return -1;
		else
			return front_node->n;
	}

	void push(int n) {
		Node* node = new Node;
		node->n = n;

		if (empty()) {
			front_node = node;
			end = node;
		}
		else {
			end->next = node;
			node->prev = end;
			end = node;
		}
	}

	void pop() {
		if (empty())
			return;

		Node* node = front_node;
		if (node == end) {
			front_node = NULL;
			end = NULL;
		}
		else {
			front_node = node->next;
			front_node->prev = NULL;
		}

		delete node;
	}
};

int main() {
	Queue_list q1;
	q1.push(1);
	q1.push(2);
	q1.push(3);

	while (!q1.empty()) {
		cout << q1.front() << endl;
		q1.pop();
	}
}