#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Child;
class Node;
class Tree;
	
struct info { //노드의 정보를 나타내는 구조체
	string name;	
	int group;
	string duty;
	int id; 
};

class Child { // 노드의 자식 링크드리스트를 만들기 위한 클래스
public:
	Child* next;
	Child* prev;
	Node* cNode; // 자식노드를 가르킴

	Child() {
		next = NULL;
		prev = NULL;
		cNode = NULL;
	}
};

class Node {
public:
	Node* parent; 
	Child* headChild; // 노드의 자식링크드리스트의 맨 앞 자식을 가르킴
	Child* tailChild; // 노드의 자식링크드리스트의 마지막 자식을 가르킴
	info info;
	int childNum; // 자식의 개수

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

	void childList(Node* newNode); // 자식 링크드리스트 삽입
	void nodeInsert(string name, int group, string duty, int id); //노드의 정보를 받아 삽입
	void groupPrint(int group); // 전체 목록이나 분반의 목록을 받아 출력
	void Print(Node* printNode, Child* printChild); // groupPrint에서 파라미터를 받아 자식 출력
	void Delete(string name);
	Tree() {
		root = NULL;
	}

};

void Tree::childList(Node* newNode) { 
	Child* parChild = new Child; //newNode를 가르키는 부모의 Child

	parChild->cNode = newNode;
	int gruopNum = newNode->info.group; //분반번호 = 넘겨받은 자식의 분반번호

	if (newNode->parent->headChild == NULL) { //자식을 처음 생성했을 때
		newNode->parent->headChild = parChild;
		newNode->parent->tailChild = parChild;
		newNode->parent->childNum++;
	}
	else{
		if (newNode->parent == root) {
			for (Child* current = root->headChild; current != NULL; current = current->next) { // 같은 분반의 강의교수가 있는지 확인하여, 있다면 추가하지 못하게 한다.
				if (gruopNum == current->cNode->info.group) {
					cout << "추가할 수 없음" << endl;
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
	if (duty == "책임교수" && group == 0) {  // 책임교수이면 루트(가장 상위 노드)가 된다.
		root = newNode;
	}
	else if (duty == "강의교수" && group != 0) { //강의교수일 경우 루트를 부모로 하고 루트의 자식 링크드리스트에 추가시킨다.
		newNode->parent = root;
		childList(newNode);
	}
	else if (duty == "수강생" && group != 0) { //수강생일 경우 강의교수를 찾아 부모로 지정하고, 해당 강의교수의 자식 링크드리스트에 추가시킨다.
		for (Child* current = root->headChild; current != NULL; current = current->next) { 
			if (current->cNode->info.group == group) { 
				if (current->cNode->childNum >= 30) { //해당 분반의 인원이 30명이 초과되었을 경우 추가시킬수 없음을 나타낸다.
					cout << "추가할 수 없음" << endl;
					delete newNode; //추가할 수 없으므로 생선한 노드를 다시 삭제한다.
					break;
				}
				else {
					newNode->parent = current->cNode;
					childList(newNode);
					break;
				}
			}
			else if (current == root->tailChild) { // 찾는 분반의 강의교수가 없을 경우 추가시킬수 없음을 나타낸다.
				cout << "추가할 수 없음" << endl;
				delete newNode;
				break;
			}
		}
	}
}

void Tree::groupPrint(int group) {

	if (group == 0) { //전체출력
		cout << root->info.name << " " << root->info.group << " " <<
			root->info.duty << " " << root->info.id << endl;      //책임교수 출력
		for (Child* current = root->headChild; current != NULL; current = current->next) {
			Print(current->cNode, current->cNode->headChild); //자식노드를 출력하는 함수
			if (current == root->tailChild)
				break;
		}
	}
	else //분반출력
		for (Child* current = root->headChild; current != NULL; current = current->next) {
			if (current->cNode->info.group == group) {
				Print(current->cNode, current->cNode->headChild);
				break;
			}
			else if (current == root->tailChild) { //찾는 분반이 없을 때 나타낸다.
				cout << "없음" << endl;
				break;
			}
		}
}

void Tree::Print(Node* cNode, Child* printChild) { //preorder traversal

	cout << cNode->info.name << " " << cNode->info.group << " " <<
		cNode->info.duty << " " << cNode->info.id << endl;
	if (printChild == NULL) {} //자식이 없을경우 중지
	else{
		cNode = printChild->cNode;
		printChild = printChild->next;
		Print(cNode, printChild);
	}
}

void Tree::Delete(string name) { //이름을 받아 해당 노드를 삭제한다.

	Child* dchild = new Child;
	Node* dnode = new Node;
	delete dnode;
	dchild = NULL;
	dnode = NULL;

	for (Child* i = root->headChild; dnode == NULL; i = i->next) {
		if (name == i->cNode->info.name) { //강의교수 삭제
			if (i->cNode->headChild != NULL) { //삭제하려고 하는 강의교수의 수강생이 있을 경우 경고메세지
				cout << "삭제할 수 없음" << endl;
				break;
			}
			else if (root->headChild == root->tailChild) { //루트의 자식이 없을 경우
				root->headChild = NULL;
				root->tailChild = NULL;
			}
			else if (i == root->headChild) { // 삭제하는 노드가 루트의 헤드일 경우
				i->next->prev = root->headChild;
				root->headChild = i->next;
			}
			else if (i == root->tailChild) { //삭제하는 노드가 루트이 테일일 경우
				i->prev->next = root->tailChild;
				root->tailChild = i->prev;
			}
			else { //중간 삭제
				i->prev->next = i->next;
				i->next->prev = i->prev;
			}
			root->childNum--;
			dnode = i->cNode;
			dchild = i;
			break;
		}
		for (Child* j = i->cNode->headChild; dnode == NULL; j = j->next) { //수강생 중에 이름이 있는지 확인
			if (j == NULL) break;
			else if (name == j->cNode->info.name) { //수강생 삭제
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
			cout << "삭제할 수 없음" << endl; ////삭제하려고 하는 강의교수나 수강생이 없을 경우 경고메세지
			break;
		}
	}
	delete dnode; //삭제할 값의 노드를 삭제한다.
	delete dchild;
}

int main() {

	string name;
	string duty;
	int group, id;
	Tree Tree;
	char kind; 

	string inputName, commandName;
	cout << "초기 입력 파일명 : ";
	cin >> inputName;
	cout << "질의 입력 파일명 : ";
	cin >> commandName;

	ifstream inputFile(inputName); //입력파일을 쓸 수 있도록한다.
	int count = 0;

	if (!inputFile.is_open()) {
		cout << "파일이 존재하지 않습니다." << endl;
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
		cout << "파일이 존재하지 않습니다." << endl;
	}
	else {
		while (!commandFile.eof()) {
			count2++;
			commandFile >> kind;
			if (kind == 's') { //탐색
				commandFile >> group;
				Tree.groupPrint(group);
			}
			else if (kind == 'i') { //추가
				commandFile >> name >> group >> duty >> id;
				if (group == 0 || group > 30) { //분반이 30개를 초과하거나 책임교수를 생성하려고 할 때 발생한다.
					cout << "추가할 수 없음" << endl;
					continue;
				}
				Tree.nodeInsert(name, group, duty, id);
			}
			else if (kind == 'd') { //삭제
				commandFile >> name;
				Tree.Delete(name);
			}
		}
	}
	system("pause");
	return 0;
}
