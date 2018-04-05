#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Child;
class Node;
class Tree;
	
struct info { //����� ������ ��Ÿ���� ����ü
	string name;	
	int group;
	string duty;
	int id; 
};

class Child { // ����� �ڽ� ��ũ�帮��Ʈ�� ����� ���� Ŭ����
public:
	Child* next;
	Child* prev;
	Node* cNode; // �ڽĳ�带 ����Ŵ

	Child() {
		next = NULL;
		prev = NULL;
		cNode = NULL;
	}
};

class Node {
public:
	Node* parent; 
	Child* headChild; // ����� �ڽĸ�ũ�帮��Ʈ�� �� �� �ڽ��� ����Ŵ
	Child* tailChild; // ����� �ڽĸ�ũ�帮��Ʈ�� ������ �ڽ��� ����Ŵ
	info info;
	int childNum; // �ڽ��� ����

	Node() {
		parent = NULL;
		headChild = NULL;
		tailChild = NULL;
		childNum = 0;		
	}

};

class Tree {
public:
	Node* root;

	void childList(Node* newNode); // �ڽ� ��ũ�帮��Ʈ ����
	void nodeInsert(string name, int group, string duty, int id); //����� ������ �޾� ����
	void groupPrint(int group); // ��ü ����̳� �й��� ����� �޾� ���
	void Print(Node* printNode, Child* printChild); // groupPrint���� �Ķ���͸� �޾� �ڽ� ���
	void Delete(string name);
	Tree() {
		root = NULL;
	}

};

void Tree::childList(Node* newNode) { 
	Child* parChild = new Child; //newNode�� ����Ű�� �θ��� Child

	parChild->cNode = newNode;
	int gruopNum = newNode->info.group; //�йݹ�ȣ = �Ѱܹ��� �ڽ��� �йݹ�ȣ

	if (newNode->parent->headChild == NULL) { //�ڽ��� ó�� �������� ��
		newNode->parent->headChild = parChild;
		newNode->parent->tailChild = parChild;
		newNode->parent->childNum++;
	}
	else{
		if (newNode->parent == root) {
			for (Child* current = root->headChild; current != NULL; current = current->next) { // ���� �й��� ���Ǳ����� �ִ��� Ȯ���Ͽ�, �ִٸ� �߰����� ���ϰ� �Ѵ�.
				if (gruopNum == current->cNode->info.group) {
					cout << "�߰��� �� ����" << endl;
					delete newNode;
					delete parChild;
					break;
				}
				else if (current == root->tailChild) {
					parChild->prev = newNode->parent->tailChild;
					newNode->parent->tailChild->next = parChild;
					newNode->parent->tailChild = parChild;
					newNode->parent->childNum++;
					break;
				}
			}
		}
		else if (newNode->parent != root) {
			parChild->prev = newNode->parent->tailChild;
			newNode->parent->tailChild->next = parChild;
			newNode->parent->tailChild = parChild;
			newNode->parent->childNum++;
			}
		}
	}

void Tree::nodeInsert(string name, int group, string duty, int id) {

	Node* newNode = new Node;
	newNode->info = { name, group, duty, id };
	if (duty == "å�ӱ���" && group == 0) {  // å�ӱ����̸� ��Ʈ(���� ���� ���)�� �ȴ�.
		root = newNode;
	}
	else if (duty == "���Ǳ���" && group != 0) { //���Ǳ����� ��� ��Ʈ�� �θ�� �ϰ� ��Ʈ�� �ڽ� ��ũ�帮��Ʈ�� �߰���Ų��.
		newNode->parent = root;
		childList(newNode);
	}
	else if (duty == "������" && group != 0) { //�������� ��� ���Ǳ����� ã�� �θ�� �����ϰ�, �ش� ���Ǳ����� �ڽ� ��ũ�帮��Ʈ�� �߰���Ų��.
		for (Child* current = root->headChild; current != NULL; current = current->next) { 
			if (current->cNode->info.group == group) { 
				if (current->cNode->childNum >= 30) { //�ش� �й��� �ο��� 30���� �ʰ��Ǿ��� ��� �߰���ų�� ������ ��Ÿ����.
					cout << "�߰��� �� ����" << endl;
					delete newNode; //�߰��� �� �����Ƿ� ������ ��带 �ٽ� �����Ѵ�.
					break;
				}
				else {
					newNode->parent = current->cNode;
					childList(newNode);
					break;
				}
			}
			else if (current == root->tailChild) { // ã�� �й��� ���Ǳ����� ���� ��� �߰���ų�� ������ ��Ÿ����.
				cout << "�߰��� �� ����" << endl;
				delete newNode;
				break;
			}
		}
	}
}

void Tree::groupPrint(int group) {

	if (group == 0) { //��ü���
		cout << root->info.name << " " << root->info.group << " " <<
			root->info.duty << " " << root->info.id << endl;      //å�ӱ��� ���
		for (Child* current = root->headChild; current != NULL; current = current->next) {
			Print(current->cNode, current->cNode->headChild); //�ڽĳ�带 ����ϴ� �Լ�
			if (current == root->tailChild)
				break;
		}
	}
	else //�й����
		for (Child* current = root->headChild; current != NULL; current = current->next) {
			if (current->cNode->info.group == group) {
				Print(current->cNode, current->cNode->headChild);
				break;
			}
			else if (current == root->tailChild) { //ã�� �й��� ���� �� ��Ÿ����.
				cout << "����" << endl;
				break;
			}
		}
}

void Tree::Print(Node* cNode, Child* printChild) { //preorder traversal

	cout << cNode->info.name << " " << cNode->info.group << " " <<
		cNode->info.duty << " " << cNode->info.id << endl;
	if (printChild == NULL) {} //�ڽ��� ������� ����
	else{
		cNode = printChild->cNode;
		printChild = printChild->next;
		Print(cNode, printChild);
	}
}

void Tree::Delete(string name) { //�̸��� �޾� �ش� ��带 �����Ѵ�.

	Child* dchild = new Child;
	Node* dnode = new Node;
	delete dnode;
	dchild = NULL;
	dnode = NULL;

	for (Child* i = root->headChild; dnode == NULL; i = i->next) {
		if (name == i->cNode->info.name) { //���Ǳ��� ����
			if (i->cNode->headChild != NULL) { //�����Ϸ��� �ϴ� ���Ǳ����� �������� ���� ��� ���޼���
				cout << "������ �� ����" << endl;
				break;
			}
			else if (root->headChild == root->tailChild) { //��Ʈ�� �ڽ��� ���� ���
				root->headChild = NULL;
				root->tailChild = NULL;
			}
			else if (i == root->headChild) { // �����ϴ� ��尡 ��Ʈ�� ����� ���
				i->next->prev = root->headChild;
				root->headChild = i->next;
			}
			else if (i == root->tailChild) { //�����ϴ� ��尡 ��Ʈ�� ������ ���
				i->prev->next = root->tailChild;
				root->tailChild = i->prev;
			}
			else { //�߰� ����
				i->prev->next = i->next;
				i->next->prev = i->prev;
			}
			root->childNum--;
			dnode = i->cNode;
			dchild = i;
			break;
		}
		for (Child* j = i->cNode->headChild; dnode == NULL; j = j->next) { //������ �߿� �̸��� �ִ��� Ȯ��
			if (j == NULL) break;
			else if (name == j->cNode->info.name) { //������ ����
				i->cNode->childNum--;
				if (i->cNode->headChild == i->cNode->tailChild) {
					i->cNode->headChild = NULL;
					i->cNode->tailChild = NULL;
				}
				else if (j == i->cNode->headChild) {
					j->next->prev = i->cNode->headChild;
					i->cNode->headChild = j->next;
				}
				else if (j == i->cNode->tailChild) {
					j->prev->next = i->cNode->tailChild;
					i->cNode->tailChild = j->prev;
				}
				else {
					j->prev->next = j->next;
					j->next->prev = j->prev;
				}
				dnode = j->cNode;
				dchild = j;
				break;
			}
			else if (j == i->cNode->tailChild) { 
				break;
			}
		}
		if (dnode != NULL) break;
		if (i == root->tailChild) {
			cout << "������ �� ����" << endl; ////�����Ϸ��� �ϴ� ���Ǳ����� �������� ���� ��� ���޼���
			break;
		}
	}
	delete dnode; //������ ���� ��带 �����Ѵ�.
	delete dchild;
}

int main() {

	string name;
	string duty;
	int group, id;
	Tree Tree;
	char kind; 

	string inputName, commandName;
	cout << "�ʱ� �Է� ���ϸ� : ";
	cin >> inputName;
	cout << "���� �Է� ���ϸ� : ";
	cin >> commandName;

	ifstream inputFile(inputName); //�Է������� �� �� �ֵ����Ѵ�.
	int count = 0;

	if (!inputFile.is_open()) {
		cout << "������ �������� �ʽ��ϴ�." << endl;
	}
	else {
		while (!inputFile.eof()) {
			count++;
			inputFile >> name >> group >> duty >> id;
			Tree.nodeInsert(name, group, duty, id);
		}
	}

	ifstream commandFile(commandName);
	int count2 = 0;

	if (!commandFile.is_open()) {
		cout << "������ �������� �ʽ��ϴ�." << endl;
	}
	else {
		while (!commandFile.eof()) {
			count2++;
			commandFile >> kind;
			if (kind == 's') { //Ž��
				commandFile >> group;
				Tree.groupPrint(group);
			}
			else if (kind == 'i') { //�߰�
				commandFile >> name >> group >> duty >> id;
				if (group == 0 || group > 30) { //�й��� 30���� �ʰ��ϰų� å�ӱ����� �����Ϸ��� �� �� �߻��Ѵ�.
					cout << "�߰��� �� ����" << endl;
					continue;
				}
				Tree.nodeInsert(name, group, duty, id);
			}
			else if (kind == 'd') { //����
				commandFile >> name;
				Tree.Delete(name);
			}
		}
	}
	system("pause");
	return 0;
}
