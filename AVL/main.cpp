#include <iostream>
#include <fstream>
#include <string>
using namespace std;

ofstream outputFile;								// ofstream 객체를 선언한다.(출력파일을 생성하고, 쓸 수 있게해준다.)
int txtType = 0;									// txtType이 command.txt이면 1로 바꾼다. (insert의 probe 횟수를 출력할 수 있도록 한다.)

struct student {		// 학생 정보를 담고있는 구조체
	int number;			// 학번
	string name;		// 이름
	string department;	// 학과
	int grade;			// 학년
};

class Node {			// 트리의 구조
public:
	Node*	parent;		// 부모 노드
	Node*	left;		// 왼쪽 자식노드
	Node*	right;		// 오른쪽 자식노드
	student	data;		// 노드의 데이터(학생의 정보)

	Node() {			// 생성자 기본값 NULL 초기화
		parent = NULL;
		left = NULL;
		right = NULL;
	}
};

class Tree {
public:
	Node* root;																					// 루트노드(최상위 노드)
	int probe;																					// 탐사횟수

	bool limiteLength(int number, string name, string department, int grade);					// 입력받은 학생정보가 초기조건과 안맞을 경우 1을 반환하는 함수
	void setNode(Node* node, int number, string name, string department, int grade);			// 노드의 정보를 설정해준다.

	void insert(int number, string name, string department, int grade);							// 삽입 함수
	void nodePosition(Node* newNode, Node* node);												// 노드가 삽입될 위치를 정해준다.

	int height(Node* node);																		// 노드의 높이를 반환 해 준다.
	bool heightProblem(Node* node, int h);														// 노드의 양쪽 자식의 높이 차이가 2이상인지 확인한다.

	void restructuringCheak(Node* newNode);														// 재건축 함수(왼쪽과 오른쪽 자식의 높이가 2이상 차이날 경우 함수가 실행된다.)
	void restructuring(Node* newNode, Node* current);
	Node* xyz(Node* child, Node* ancestor);

	void print(int number);																		// 출력 함수
	Node* search(int k, Node* v);																// 탐색 함수


	Tree() {																					// 트리 생성시 탐사횟수를 0으로 초기화
		root = NULL;
		probe = 0;
	}
};
bool Tree::limiteLength(int number, string name, string department, int grade) { // 입력받은 값이 초기조건과 안맞을 경우 1을 반환하는 함수
	if (number < 10000000 || number >= 100000000 || name.length() > 20    // 학번 8자리, 이름 20byte, 학과 20bybe, 학년 1~4 정수가 아닐경우
		|| department.length() > 20 || grade < 1 || grade > 4) {
		outputFile << "추가할 수 없음 " << probe << endl;								// 추가할 수 없음을 나타내고
		return 1;																		// 1 반환
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

bool equals = 0;		// 전역변수로 같은 값이 있을 때 1로 변경한다.
void Tree::insert(int number, string name, string department, int grade) {
	probe = 0;													// 초기 탐사횟수 0
	if (limiteLength(number, name, department, grade) == 0) {
		Node* newNode = new Node;								// 새로운 노드를 생성한다.
		setNode(newNode, number, name, department, grade);		// 노드의 데이터를 입력 받은 값으로 설정한다.

		if (root == NULL) {										// 루트노드가 없으면 새로운 노드를 루트로 설정한다.
			root = newNode;
		}
		else
			nodePosition(newNode, root);						// 노드의 위치를 정해준다.
		if (equals != 1) {										// 같은 값이 없으면
			if (txtType == 1)									// txtType이 command.txt일때
				outputFile << probe << endl;					// 탐사횟수를 출력한다.
			restructuringCheak(newNode);						// 구조의 변화가 필요한지 확인하는 함수로 간다.
		}
	}
	equals = 0;													// 다시 equals를 0으로 초기화한다.
}

void Tree::nodePosition(Node* newNode, Node* node) {			// 노드의 위치를 조정해주는 함수
	probe++;													// 재귀함수로 함수를 호출할 때마다 탐사횟수가 1 증가한다.
	if (node != NULL) {											// 노드가 null이 아닐때
		if (newNode->data.number == node->data.number) {		// 트리에 이미 같은 값의 노드가 존재 할 때
			outputFile << "추가할 수 없음 " << probe << endl;
			equals = 1;
			delete newNode;										// 생성한 노드 삭제
		}
		else {
			if (newNode->data.number < node->data.number) {		//삽입한 노드의 숫자 < 조상 노드의 숫자 (왼쪽 탐색)
				if (node->left == NULL) { 						// 자리가 비었으면 그 자리에 연결
					node->left = newNode;
					newNode->parent = node;
				}
				else {											 //node->left != NULL 삽입한 노드의 숫자 > 부모 노드의 숫자 (노드의 왼쪽 탐색)
					nodePosition(newNode, node->left);
				}
			}
			else if (newNode->data.number > node->data.number) { // 삽입한 노드의 숫자 > 조상 노드의 숫자 (오른쪽 탐색)
				if (node->right == NULL) {						 // 자리가 비었으면 그 자리에 연결
					node->right = newNode;
					newNode->parent = node;
				}
				else {											 //node->right != NULL 삽입한 노드의 숫자 > 부모 노드의 숫자 (노드의 왼쪽 탐색)
					nodePosition(newNode, node->right);
				}
			}
		}
	}
}

int Tree::height(Node* node) {		// 높이를 반환하는 함수
	int left, right;				// 노드의 왼쪽, 오른쪽 자식의 높이를 나타내는 변수

	if (node == NULL)				// 노드가 비었으면 외부노드이므로 h = 0이다. 따라서 0을 반환한다.
		return 0;

	left = height(node->left);		// 왼쪽 자식의 높이를 구하기 위해, 노드의 왼쪽 자식의 높이에 대한 함수를 적용한다.
	right = height(node->right);	// 오른쪽 자식의 높이를 구하기 위해, 노드의 오른쪽 자식의 높이에 대한 함수를 적용한다.

	if (left > right)				// 왼쪽 자식의 높이가 오른쪽 자식의 높이보다 클 때
		return left + 1;			// 이 노드의 높이는 왼쪽자식의 높이 +1 이므로, 값을 반환한다.
	else
		return right + 1;			// 이 노드의 높이는 오른쪽자식의 높이 +1 이므로, 값을 반환한다.
}

bool Tree::heightProblem(Node* node, int h) {
	// 해당 노드의 부모가 문제노드인지 확인하는 함수
	//(부모의 각 자식들의 높이를 비교했을때 2 이상 차이나면 부모는 문제노드, h는 노드의 높이)

	int opposite;								// 노드의 반대편 자식의 높이

	if (node->parent == NULL)					// 노드의 부모가 없으면 비교할 값이 없으므로 0 반환
		return 0;

	if (node->parent->left == node)				// 노드가 부모의 왼쪽자식이면
		opposite = height(node->parent->right);	// 오른쪽 자식의 높이를 구한다.
	else										// 노드가 부모의 오른쪽자식이면
		opposite = height(node->parent->left);  // 왼쪽 자식의 높이를 구한다.

	if (h - opposite > 1 || opposite - h < -1)  // 높이가 2이상 차이가 날 경우 
		return 1;								// 문제가 있음을 나타내는 1 반환(부모노드에 문제가 있음)
	else
		return 0;
}

void Tree::restructuringCheak(Node* newNode) {											// 트리의 높이를 적절하게 해주기 위해 구조를 바꾸는 함수

	int h = 0;																			// 노드의 높이이다.
	for (Node* current = newNode; current != NULL; current = current->parent) {			// 문제노드를 찾기 위해 가 NULL이 될때까지 노드의 부모를 탐색한다.
		h++;																			// 현재 노드의 높이를 나타낸다.
		if (current->right == NULL && current->left == NULL) 							// 노드의 자식들이 없는 경우(외부노드) 다음(노드의 부모 탐색)으로 넘어간다.
			continue;
		else if (h >= 3 && current->right == NULL || h >= 3 && current->left == NULL)	// 현재 노드의 높이가 3 이상이고, 한쪽 자식 노드가 없을 때 구조를 변경한다.
		{
		}
		else if (heightProblem(current, h) == 1)  										// 부모의 반대편 노드와 높이가 2 이상인지 확인한다.
		{
			current = current->parent;													// current의 부모가 문제 노드이므로 current를 부모로 설정한다.
		}
		else
			continue;

		restructuring(newNode, current);												// 구조를 조정하는 함수
		break; 																			// 구조조정을 끝마치고 반복문을 끝낸다.
	}
}

Node* Tree::xyz(Node* child, Node* ancestor) {		// 두 노드의 값을 비교하여 child가 ancestor의 어느쪽 자식에 속한지 확인
	if (child->data.number > ancestor->data.number)	// 자식의 수가 부모의 수보다 더 크면
		return ancestor->right;						// 조상의 오른쪽노드를 자식으로 지정한다.
	else
		return ancestor->left;						// 조상의 왼쪽노드를 자식으로 지정한다.
}

void Tree::restructuring(Node* newNode, Node* current) {	// 양쪽 자식 노드의 높이가 2 이상차이나지 않도록 트리의 구조를 바꾼다.
	Node* a = NULL;		// 가장 큰 수의 노드
	Node* b = NULL;		// 두번째 큰 수의 노드
	Node* c = NULL;		// 가장 작은 수의 노드
	Node* x = NULL;		// 문제노드의 자손
	Node* y = NULL;		// 문제노드의 자식
	Node* z = NULL;		// 문제노드


						//x,y,z를 지정
	z = current;
	y = xyz(newNode, z);
	x = xyz(newNode, y);

	//z, y, x의 학번을 비교하여 작은 순으로 a, b, c를 정한다.
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


	//a,b,c의 각 서브트리를 재조정하여 각 노드에 붙여준다.
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
	bool rotationType = 0;						// 회전종류를 나타내는 변수
	if (y == z->right) {						// y가 z의 오른쪽 자식이면
		rotationType = 1;						// rotationType을 1로 바꾼다.
	}

	if (z != root) {							// z가 루트노드가 아니고
		if (z == z->parent->right) {			// z가 z의 부모의 오른쪽 자식이면
			z->parent->right = b;				// z의 부모노드의 오른쪽 자식을 b로 재설정한다.
		}
		else
			z->parent->left = b;
	}

	// a < b < c 작은값 왼쪽, b가 a,b의 부모 노드가 된다.
	b->parent = z->parent;						// b의 부모를 z의 부모로 b를 부모노드로 바꿈
	b->left = a;								// b의 각 자식노드에 a, b를 연결한다.
	b->right = c;
	a->parent = b;								// a,b의 부모를 b로 설정한다.
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

void Tree::print(int number) {																	// 출력 함수
	probe = 0;																					// 초기 탐사횟수 0
	Node* node;																					// 입력한 학번에 해당하는 노드의 주소를 저장할 포인터형 변수
	node = search(number, root);																// 노드의 주소를 찾는 저장한다. 루트노드부터 탐색을 시작한다.
	if (node != NULL) {
		outputFile << node->data.number << " " << node->data.name << " " <<
			node->data.department << " " << node->data.grade << " " << probe << endl;
	}
	else
		outputFile << "없음 " << probe << endl;
}

Node* Tree::search(int k, Node* v) {															// 탐색 함수

	if (v == NULL)																				// v가 null이 되면 v를 리턴한다. (외부노드까지 탐색했거나, 루트노드가 null인 경우이다. 따라서 찾는 값은 없다.)
		return v;
	probe++;																					// 재귀함수이므로 함수를 호출 할 때마다 탐사횟수가 증가한다.

	if (k < v->data.number) {																	// 찾고자하는 학번 k < 현재 탐색한 노드 v의 학번 => v의 왼쪽 노드 탐색
		search(k, v->left);
	}
	else if (k == v->data.number)																// 찾고자하는 학번과 v의 학번이 같으면 찾는값이므로 v를 반환
		return v;
	else {																						// 찾고자하는 학번 k > 현재 탐색한 노드 v의 학번 => v의 오른쪽 노드 탐색
		search(k, v->right);
	}
}

int main() {

	int select = 0;
	int number;
	string name;
	string department;
	int grade;

	Tree tree;	// 트리클래스 생성
	string inputName, commandName, outputName; // inputName : 초기 입력 파일 이름, commandName : 질의 입력 파일 이름

	cout << "초기 입력 파일명 : ";
	cin >> inputName;
	cout << "질의 입력 파일명 : ";
	cin >> commandName;
	cout << "출력 파일명 : ";
	cin >> outputName;

	outputFile.open(outputName);

	ifstream inputFile(inputName);  // 입력파일을 쓸 수 있도록 한다.
	int count = 0;

	if (!inputFile.is_open()) {		// 파일명과 일치하는 파일이 없을 경우
		cout << "파일이 존재하지 않습니다." << endl;
	}
	else {
		if (!inputFile.eof()) {
			count++;
			while (!inputFile.eof()) {
				inputFile >> number >> name >> department >> grade;  // 초기입력파일의 학생 정보들을 받는다.
				tree.insert(number, name, department, grade);	    // insert 함수를 이용하여 헤시클래스에 학생의 정보를 넣는다.
			}
		}
	}

	ifstream commandFile(commandName); // 입력파일을 쓸 수 있도록 한다.
	int count2 = 0;
	char kind;	// 입력 받을 기능의 종류

	if (!commandFile.is_open()) {
		cout << "파일이 존재하지 않습니다." << endl;
	}
	else {
		txtType = 1;	// txtType을 1로 바꿔줘 insert에서 탐사 횟수가 출력될 수 있게 한다.

		while (!commandFile.eof()) {
			count2++;
			commandFile >> kind;
			if (kind == 's') {		 // 탐색
				commandFile >> number;
				tree.print(number); // 해당 학번의 정보를 탐색한다.
			}
			else if (kind == 'i') {  // 추가
				commandFile >> number >> name >> department >> grade;
				tree.insert(number, name, department, grade); // 해당 학생의 정보를 삽입한다.
			}
			else
				break;
		}
	}

	outputFile.close();
	return 0;
}
