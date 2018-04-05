#include <iostream>
#include <fstream>
#include <string>
using namespace std;

ofstream outputFile;								// ofstream ��ü�� �����Ѵ�.(��������� �����ϰ�, �� �� �ְ����ش�.)
int txtType = 0;									// txtType�� command.txt�̸� 1�� �ٲ۴�. (insert�� probe Ƚ���� ����� �� �ֵ��� �Ѵ�.)

struct student {		// �л� ������ ����ִ� ����ü
	int number;			// �й�
	string name;		// �̸�
	string department;	// �а�
	int grade;			// �г�
};

class Node {			// Ʈ���� ����
public:
	Node*	parent;		// �θ� ���
	Node*	left;		// ���� �ڽĳ��
	Node*	right;		// ������ �ڽĳ��
	student	data;		// ����� ������(�л��� ����)

	Node() {			// ������ �⺻�� NULL �ʱ�ȭ
		parent = NULL;
		left = NULL;
		right = NULL;
	}
};

class Tree {
public:
	Node* root;																					// ��Ʈ���(�ֻ��� ���)
	int probe;																					// Ž��Ƚ��

	bool limiteLength(int number, string name, string department, int grade);					// �Է¹��� �л������� �ʱ����ǰ� �ȸ��� ��� 1�� ��ȯ�ϴ� �Լ�
	void setNode(Node* node, int number, string name, string department, int grade);			// ����� ������ �������ش�.

	void insert(int number, string name, string department, int grade);							// ���� �Լ�
	void nodePosition(Node* newNode, Node* node);												// ��尡 ���Ե� ��ġ�� �����ش�.

	int height(Node* node);																		// ����� ���̸� ��ȯ �� �ش�.
	bool heightProblem(Node* node, int h);														// ����� ���� �ڽ��� ���� ���̰� 2�̻����� Ȯ���Ѵ�.

	void restructuringCheak(Node* newNode);														// ����� �Լ�(���ʰ� ������ �ڽ��� ���̰� 2�̻� ���̳� ��� �Լ��� ����ȴ�.)
	void restructuring(Node* newNode, Node* current);
	Node* xyz(Node* child, Node* ancestor);

	void print(int number);																		// ��� �Լ�
	Node* search(int k, Node* v);																// Ž�� �Լ�


	Tree() {																					// Ʈ�� ������ Ž��Ƚ���� 0���� �ʱ�ȭ
		root = NULL;
		probe = 0;
	}
};
bool Tree::limiteLength(int number, string name, string department, int grade) { // �Է¹��� ���� �ʱ����ǰ� �ȸ��� ��� 1�� ��ȯ�ϴ� �Լ�
	if (number < 10000000 || number >= 100000000 || name.length() > 20    // �й� 8�ڸ�, �̸� 20byte, �а� 20bybe, �г� 1~4 ������ �ƴҰ��
		|| department.length() > 20 || grade < 1 || grade > 4) {
		outputFile << "�߰��� �� ���� " << probe << endl;								// �߰��� �� ������ ��Ÿ����
		return 1;																		// 1 ��ȯ
	}
	else
		return 0;
}
void Tree::setNode(Node* node, int number, string name, string department, int grade) {
	node->data.number = number;
	node->data.name = name;
	node->data.department = department;
	node->data.grade = grade;
}

bool equals = 0;		// ���������� ���� ���� ���� �� 1�� �����Ѵ�.
void Tree::insert(int number, string name, string department, int grade) {
	probe = 0;													// �ʱ� Ž��Ƚ�� 0
	if (limiteLength(number, name, department, grade) == 0) {
		Node* newNode = new Node;								// ���ο� ��带 �����Ѵ�.
		setNode(newNode, number, name, department, grade);		// ����� �����͸� �Է� ���� ������ �����Ѵ�.

		if (root == NULL) {										// ��Ʈ��尡 ������ ���ο� ��带 ��Ʈ�� �����Ѵ�.
			root = newNode;
		}
		else
			nodePosition(newNode, root);						// ����� ��ġ�� �����ش�.
		if (equals != 1) {										// ���� ���� ������
			if (txtType == 1)									// txtType�� command.txt�϶�
				outputFile << probe << endl;					// Ž��Ƚ���� ����Ѵ�.
			restructuringCheak(newNode);						// ������ ��ȭ�� �ʿ����� Ȯ���ϴ� �Լ��� ����.
		}
	}
	equals = 0;													// �ٽ� equals�� 0���� �ʱ�ȭ�Ѵ�.
}

void Tree::nodePosition(Node* newNode, Node* node) {			// ����� ��ġ�� �������ִ� �Լ�
	probe++;													// ����Լ��� �Լ��� ȣ���� ������ Ž��Ƚ���� 1 �����Ѵ�.
	if (node != NULL) {											// ��尡 null�� �ƴҶ�
		if (newNode->data.number == node->data.number) {		// Ʈ���� �̹� ���� ���� ��尡 ���� �� ��
			outputFile << "�߰��� �� ���� " << probe << endl;
			equals = 1;
			delete newNode;										// ������ ��� ����
		}
		else {
			if (newNode->data.number < node->data.number) {		//������ ����� ���� < ���� ����� ���� (���� Ž��)
				if (node->left == NULL) { 						// �ڸ��� ������� �� �ڸ��� ����
					node->left = newNode;
					newNode->parent = node;
				}
				else {											 //node->left != NULL ������ ����� ���� > �θ� ����� ���� (����� ���� Ž��)
					nodePosition(newNode, node->left);
				}
			}
			else if (newNode->data.number > node->data.number) { // ������ ����� ���� > ���� ����� ���� (������ Ž��)
				if (node->right == NULL) {						 // �ڸ��� ������� �� �ڸ��� ����
					node->right = newNode;
					newNode->parent = node;
				}
				else {											 //node->right != NULL ������ ����� ���� > �θ� ����� ���� (����� ���� Ž��)
					nodePosition(newNode, node->right);
				}
			}
		}
	}
}

int Tree::height(Node* node) {		// ���̸� ��ȯ�ϴ� �Լ�
	int left, right;				// ����� ����, ������ �ڽ��� ���̸� ��Ÿ���� ����

	if (node == NULL)				// ��尡 ������� �ܺγ���̹Ƿ� h = 0�̴�. ���� 0�� ��ȯ�Ѵ�.
		return 0;

	left = height(node->left);		// ���� �ڽ��� ���̸� ���ϱ� ����, ����� ���� �ڽ��� ���̿� ���� �Լ��� �����Ѵ�.
	right = height(node->right);	// ������ �ڽ��� ���̸� ���ϱ� ����, ����� ������ �ڽ��� ���̿� ���� �Լ��� �����Ѵ�.

	if (left > right)				// ���� �ڽ��� ���̰� ������ �ڽ��� ���̺��� Ŭ ��
		return left + 1;			// �� ����� ���̴� �����ڽ��� ���� +1 �̹Ƿ�, ���� ��ȯ�Ѵ�.
	else
		return right + 1;			// �� ����� ���̴� �������ڽ��� ���� +1 �̹Ƿ�, ���� ��ȯ�Ѵ�.
}

bool Tree::heightProblem(Node* node, int h) {
	// �ش� ����� �θ� ����������� Ȯ���ϴ� �Լ�
	//(�θ��� �� �ڽĵ��� ���̸� �������� 2 �̻� ���̳��� �θ�� �������, h�� ����� ����)

	int opposite;								// ����� �ݴ��� �ڽ��� ����

	if (node->parent == NULL)					// ����� �θ� ������ ���� ���� �����Ƿ� 0 ��ȯ
		return 0;

	if (node->parent->left == node)				// ��尡 �θ��� �����ڽ��̸�
		opposite = height(node->parent->right);	// ������ �ڽ��� ���̸� ���Ѵ�.
	else										// ��尡 �θ��� �������ڽ��̸�
		opposite = height(node->parent->left);  // ���� �ڽ��� ���̸� ���Ѵ�.

	if (h - opposite > 1 || opposite - h < -1)  // ���̰� 2�̻� ���̰� �� ��� 
		return 1;								// ������ ������ ��Ÿ���� 1 ��ȯ(�θ��忡 ������ ����)
	else
		return 0;
}

void Tree::restructuringCheak(Node* newNode) {											// Ʈ���� ���̸� �����ϰ� ���ֱ� ���� ������ �ٲٴ� �Լ�

	int h = 0;																			// ����� �����̴�.
	for (Node* current = newNode; current != NULL; current = current->parent) {			// ������带 ã�� ���� �� NULL�� �ɶ����� ����� �θ� Ž���Ѵ�.
		h++;																			// ���� ����� ���̸� ��Ÿ����.
		if (current->right == NULL && current->left == NULL) 							// ����� �ڽĵ��� ���� ���(�ܺγ��) ����(����� �θ� Ž��)���� �Ѿ��.
			continue;
		else if (h >= 3 && current->right == NULL || h >= 3 && current->left == NULL)	// ���� ����� ���̰� 3 �̻��̰�, ���� �ڽ� ��尡 ���� �� ������ �����Ѵ�.
		{
		}
		else if (heightProblem(current, h) == 1)  										// �θ��� �ݴ��� ���� ���̰� 2 �̻����� Ȯ���Ѵ�.
		{
			current = current->parent;													// current�� �θ� ���� ����̹Ƿ� current�� �θ�� �����Ѵ�.
		}
		else
			continue;

		restructuring(newNode, current);												// ������ �����ϴ� �Լ�
		break; 																			// ���������� ����ġ�� �ݺ����� ������.
	}
}

Node* Tree::xyz(Node* child, Node* ancestor) {		// �� ����� ���� ���Ͽ� child�� ancestor�� ����� �ڽĿ� ������ Ȯ��
	if (child->data.number > ancestor->data.number)	// �ڽ��� ���� �θ��� ������ �� ũ��
		return ancestor->right;						// ������ �����ʳ�带 �ڽ����� �����Ѵ�.
	else
		return ancestor->left;						// ������ ���ʳ�带 �ڽ����� �����Ѵ�.
}

void Tree::restructuring(Node* newNode, Node* current) {	// ���� �ڽ� ����� ���̰� 2 �̻����̳��� �ʵ��� Ʈ���� ������ �ٲ۴�.
	Node* a = NULL;		// ���� ū ���� ���
	Node* b = NULL;		// �ι�° ū ���� ���
	Node* c = NULL;		// ���� ���� ���� ���
	Node* x = NULL;		// ��������� �ڼ�
	Node* y = NULL;		// ��������� �ڽ�
	Node* z = NULL;		// �������


						//x,y,z�� ����
	z = current;
	y = xyz(newNode, z);
	x = xyz(newNode, y);

	//z, y, x�� �й��� ���Ͽ� ���� ������ a, b, c�� ���Ѵ�.
	if (z->data.number > y->data.number && z->data.number > x->data.number) { // z > y,x
		c = z;
		if (y->data.number > x->data.number) {
			b = y;
			a = x;
		}
		else {
			b = x;
			a = y;
		}
	}
	else if (y->data.number > z->data.number && y->data.number > x->data.number) { // y > z,x
		c = y;
		if (z->data.number > x->data.number) {
			b = z;
			a = x;
		}
		else {
			b = x;
			a = z;
		}
	}
	else { // x > y,z
		c = x;
		if (y->data.number > z->data.number) {
			b = y;
			a = z;
		}
		else {
			b = z;
			a = y;
		}
	}


	//a,b,c�� �� ����Ʈ���� �������Ͽ� �� ��忡 �ٿ��ش�.
	Node* t0 = NULL;
	Node* t1 = NULL;
	Node* t2 = NULL;
	Node* t3 = NULL;

	if (z->right == y && y->right == x) {
		t1 = y->left;
		t2 = x->left;
	}
	else if (z->left == y && y->left == x) {
		t1 = y->right;
		t2 = x->right;
	}
	else if (z->right == y && y->left == x) {
		t1 = x->left;
		t2 = x->right;
	}
	else if (z->left == y && y->right == x) {
		t1 = x->right;
		t2 = x->left;
	}
	/*
	if (z->right == y && y->right == x) {
	t0 = z->left;
	t1 = y->left;
	t2 = x->left;
	t3 = x->right;
	}
	else if (z->left == y && y->left == x) {
	t0 = z->right;
	t1 = y->right;
	t2 = x->right;
	t3 = x->left;
	}
	else if (z->right == y && y->left == x) {
	t0 = z->left;
	t1 = x->left;
	t2 = x->right;
	t3 = y->right;
	}
	else if (z->left == y && y->right == x) {
	t0 = z->right;
	t1 = x->right;
	t2 = x->left;
	t3 = y->left;
	}
	*/
	bool rotationType = 0;						// ȸ�������� ��Ÿ���� ����
	if (y == z->right) {						// y�� z�� ������ �ڽ��̸�
		rotationType = 1;						// rotationType�� 1�� �ٲ۴�.
	}

	if (z != root) {							// z�� ��Ʈ��尡 �ƴϰ�
		if (z == z->parent->right) {			// z�� z�� �θ��� ������ �ڽ��̸�
			z->parent->right = b;				// z�� �θ����� ������ �ڽ��� b�� �缳���Ѵ�.
		}
		else
			z->parent->left = b;
	}

	// a < b < c ������ ����, b�� a,b�� �θ� ��尡 �ȴ�.
	b->parent = z->parent;						// b�� �θ� z�� �θ�� b�� �θ���� �ٲ�
	b->left = a;								// b�� �� �ڽĳ�忡 a, b�� �����Ѵ�.
	b->right = c;
	a->parent = b;								// a,b�� �θ� b�� �����Ѵ�.
	c->parent = b;

	if (rotationType == 1) {
		//a->left = t0;
		a->right = t1;
		if (t1 != NULL)
			t1->parent = a;
		c->left = t2;
		if (t2 != NULL)
			t2->parent = c;
		//c->right = t3;
	}
	else if (rotationType == 0) {
		//a->left = t3;
		a->right = t2;
		if (t2 != NULL)
			t2->parent = a;
		c->left = t1;
		if (t1 != NULL)
			t1->parent = c;
		//c->right = t0;
	}
	if (root == z)
		root = b;
}

void Tree::print(int number) {																	// ��� �Լ�
	probe = 0;																					// �ʱ� Ž��Ƚ�� 0
	Node* node;																					// �Է��� �й��� �ش��ϴ� ����� �ּҸ� ������ �������� ����
	node = search(number, root);																// ����� �ּҸ� ã�� �����Ѵ�. ��Ʈ������ Ž���� �����Ѵ�.
	if (node != NULL) {
		outputFile << node->data.number << " " << node->data.name << " " <<
			node->data.department << " " << node->data.grade << " " << probe << endl;
	}
	else
		outputFile << "���� " << probe << endl;
}

Node* Tree::search(int k, Node* v) {															// Ž�� �Լ�

	if (v == NULL)																				// v�� null�� �Ǹ� v�� �����Ѵ�. (�ܺγ����� Ž���߰ų�, ��Ʈ��尡 null�� ����̴�. ���� ã�� ���� ����.)
		return v;
	probe++;																					// ����Լ��̹Ƿ� �Լ��� ȣ�� �� ������ Ž��Ƚ���� �����Ѵ�.

	if (k < v->data.number) {																	// ã�����ϴ� �й� k < ���� Ž���� ��� v�� �й� => v�� ���� ��� Ž��
		search(k, v->left);
	}
	else if (k == v->data.number)																// ã�����ϴ� �й��� v�� �й��� ������ ã�°��̹Ƿ� v�� ��ȯ
		return v;
	else {																						// ã�����ϴ� �й� k > ���� Ž���� ��� v�� �й� => v�� ������ ��� Ž��
		search(k, v->right);
	}
}

int main() {

	int select = 0;
	int number;
	string name;
	string department;
	int grade;

	Tree tree;	// Ʈ��Ŭ���� ����
	string inputName, commandName, outputName; // inputName : �ʱ� �Է� ���� �̸�, commandName : ���� �Է� ���� �̸�

	cout << "�ʱ� �Է� ���ϸ� : ";
	cin >> inputName;
	cout << "���� �Է� ���ϸ� : ";
	cin >> commandName;
	cout << "��� ���ϸ� : ";
	cin >> outputName;

	outputFile.open(outputName);

	ifstream inputFile(inputName);  // �Է������� �� �� �ֵ��� �Ѵ�.
	int count = 0;

	if (!inputFile.is_open()) {		// ���ϸ�� ��ġ�ϴ� ������ ���� ���
		cout << "������ �������� �ʽ��ϴ�." << endl;
	}
	else {
		if (!inputFile.eof()) {
			count++;
			while (!inputFile.eof()) {
				inputFile >> number >> name >> department >> grade;  // �ʱ��Է������� �л� �������� �޴´�.
				tree.insert(number, name, department, grade);	    // insert �Լ��� �̿��Ͽ� ���Ŭ������ �л��� ������ �ִ´�.
			}
		}
	}

	ifstream commandFile(commandName); // �Է������� �� �� �ֵ��� �Ѵ�.
	int count2 = 0;
	char kind;	// �Է� ���� ����� ����

	if (!commandFile.is_open()) {
		cout << "������ �������� �ʽ��ϴ�." << endl;
	}
	else {
		txtType = 1;	// txtType�� 1�� �ٲ��� insert���� Ž�� Ƚ���� ��µ� �� �ְ� �Ѵ�.

		while (!commandFile.eof()) {
			count2++;
			commandFile >> kind;
			if (kind == 's') {		 // Ž��
				commandFile >> number;
				tree.print(number); // �ش� �й��� ������ Ž���Ѵ�.
			}
			else if (kind == 'i') {  // �߰�
				commandFile >> number >> name >> department >> grade;
				tree.insert(number, name, department, grade); // �ش� �л��� ������ �����Ѵ�.
			}
			else
				break;
		}
	}

	outputFile.close();
	return 0;
}
