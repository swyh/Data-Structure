#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <vector>
using namespace std;

#define RED			5
#define BLACK		6
#define LEAF		NULL
#define NON_LEAF	8
#define MAX_INT		99999999
#define SIZE		5

// Class.h-----------------------------------------------------------------------#

class Member {
public:
	int idx;				// 회원번호
	char name[20] = { 0 };	// 이름
	char phone[11] = { 0 }; // 전화번호
	int grade;	//현재금액에 대한 회원등급(일반: 0 , 실버: 30,000, 골드: 50,000, 다이아: 100,000)
	int money;	//현재금액
	vector<pair<int, int>> change_money_list;	// 현재금액 변동내역 저장

												// 생성자
	Member() {};
	Member(int idx, char* name, char* phone, int grade, int money)
		: idx(idx), grade(grade), money(money) {
		strcpy(this->name, name);
		strcpy(this->phone, phone);
	};

	void setGrade();	// 회원등급 설정
	int charge_money(int add_money); // 충전하기, 등급 리턴
	bool spend_money(int money); // 돈의 변화
	void print_change_money_list(int cnt);	//돈의 변화 출력
};

class Node {
public:
	int color;	// RED and BLACK
	Node* left;	// left child
	Node* right;	// right child
	Node* parent;	// parent child
	Member* member;	// 회원정보

	Node(Member* member) {
		left = LEAF;
		right = LEAF;
		parent = NULL;
		color = RED;
		this->member = member;
	};
};

class RBTree {
public:
	int insert_member(int idx, char* name, char* phone, int grade, int money);	// 회원 삽입
	bool insert_new_member(int idx, char* name, char* phone); // 신규회원가입
	pair<int, Member*> search(int idx);	// 회원 찾기
	void print_member(int depth, Member* member);	// 회원정보 출력

	Node* getRoot() {	// root를 리턴
		return root;
	}

private:
	Node* root = NULL;

	int append_node(Node* node);	// idx가 들어갈 자리의 부모 반환, idx 이미 존재할 때 해당 depth 반환
	inline int getIdx(Node* node);	// node에 속한 memeber의 idx 반환

									//insert
	bool doubleRed(Node* node);	// doubleRed인지 판별
	bool isBlack(Node* node);	// black node인지 판별
	Node* sibling(Node* node);	// node의 sibling의 Node 포인터 반환

	inline bool isLeft(Node* node);	// node가 부모의 왼쪽노드인지 확인
	inline bool isRight(Node* node);	// node가 부모의 오른쪽 노드인지 확인
	int getDepth(Node* node);	// node의 depth를 구한다.

	Node* restructure(Node* z);	// restructuring 수행
	Node* recolor(Node* z);	// recoloring 수행

};

class Ground {
public:
	int ground_idx[1000][1000] = { 0 };	// 땅의 주인이 되는 회원번호 저장
	int ground_price[1000][1000] = { 0 };	// 땅값 저장
	bool buy_ground(Member* buyer, Member* seller, int x, int y, int price);	// buyer와 seller 간의 좌표 x,y에 대하여 price 가격에 거래를 수행
};

class Priority_queue {
private:
	pair<int, int> arr[SIZE] = { make_pair(0,0) };	// value : money, key : id
	int size = 0;	// arr 내 element size
	int last_idx = -1;	// 다음 삽입할 idx
	void sort();	// arr 정렬

public:
	pair<int, int> top();	// arr의 최소 value의 pair 반환
	int top_value();	// arr의 최소 value의 value 반환
	int top_key();	// arr의 최소 value의 key 반환

	void push(int value, int key);	// arr에 value, key pair를 삽입
	bool update(int value, int key);	//  arr의 key에 대한 value 값 수정, 없을 시 0 반환
	void print();	// arr의 value, key 출력

	bool isFull();	// arr가 꽉 찬 경우 1 반환
	bool isEmpty();	// arr가 빈 경우 1 반환

	pair<int, int> getLimit() {	// arr의 최소 value 이전의 index 반환
		return arr[last_idx - 1];
	}
	void setTop(pair<int, int> p) {	// arr의 최소 value에 대한 pair를 setting
		arr[last_idx] = p;
	}
};

class Rich {
private:
	Priority_queue arr;	// 현재금액 상위 5명을 저장하는 자료구조

public:
	void print_rich();	// 현재금액 상위 5명을 출력
	void new_rich(int money, int id);	// money, id가 arr에 들어갈 수 있는지 확인 후 삽입
	void increse_rich(int money, int id);	// 돈이 증가 하였을 경우의 루틴
	void decrese_rich(int money, int id, Node* root);	// 돈이 감소하였을 경우의 루틴
};

//Function------------------------------------------------------------------------#

pair<int, int> compare(pair<int, int> a, pair<int, int> b) {	// money가 더 크거나, 같다면 idx는 더 작은 값을 반환
	if (a.first > b.first)
		return a;
	else if (b.first > a.first)
		return b;
	else {
		if (a.second < b.second)
			return a;
		else if (b.second < a.second)
			return b;
		else
			return a;
	}
}

pair<int, int> travel(pair<int, int> limit, pair<int, int> max_val, Node* node) {	// Tree를 순회(Preorder 방식)
	if (node == NULL)
		return make_pair(0, MAX_INT);

	pair<int, int> candidate = make_pair(node->member->money, node->member->idx);

	if ((limit.first > candidate.first) || (limit.first == candidate.first && limit.second < candidate.second)) {
		max_val = compare(max_val, candidate);
		//printf("max_val %d, %d / node %d, %d\n", max_val.first, max_val.second, candidate.first, candidate.second);

	}

	max_val = compare(max_val, travel(limit, max_val, node->left));
	max_val = compare(max_val, travel(limit, max_val, node->right));

	return max_val;
}

pair<int, int> search_max(pair<int, int> limit, Node* root) {	// limit보다는 작은 최대값을 root를 따라 찾음
	pair<int, int> max_val = make_pair(0, MAX_INT);
	return travel(limit, max_val, root);
}

//Priority_queue.cpp------------------------------------------------------------------------#

pair<int, int> Priority_queue::top() {
	if (isEmpty())
		return pair<int, int>(0, 0);
	else
		return arr[last_idx];
}

int Priority_queue::top_value() {
	if (isEmpty())
		return 0;
	else
		return arr[last_idx].first;
}

int Priority_queue::top_key() {
	return arr[last_idx].second;
}

void Priority_queue::push(int value, int key) {
	if (size == SIZE) {
		if (arr[last_idx].first == value && arr[last_idx].second < key)	// 값은 같은데 키 값이 더 클 경우
			return;
	}
	last_idx = (last_idx < SIZE - 1) ? (last_idx + 1) : last_idx;	// arr 가득찬 경우에는 last_idx를 마지막 index로 고정
	size = (size < SIZE) ? (size + 1) : size;
	arr[last_idx] = make_pair(value, key);
	sort();
}

bool Priority_queue::update(int value, int key) {	// 부자에 속하는지 확인, 있으면 idx, 없으면 -1
	for (int i = 0; i < size; i++) {
		if (arr[i].second == key) {	// arr 내 존재할 경우 update
			arr[i].first = value;
			sort();
			return 1;
		}
	}
	return 0;	// 존재하지 않을 경우
}

void Priority_queue::print() {
	if (size == 0)	// 아무것도 없을 경우
		printf("Not found!\n");
	else {
		for (int i = 0; i < size; i++)
			printf("%d %d\n", arr[i].second, arr[i].first);
	}
}

void Priority_queue::sort() {	// value를 기준으로 내림차순 정렬
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (arr[i].first < arr[j].first || (arr[i].first == arr[j].first && arr[i].second > arr[j].second)) {
				pair<int, int> temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
}

bool Priority_queue::isEmpty() {
	if (size == 0)
		return 1;
	else return 0;
}

bool Priority_queue::isFull() {
	if (size == SIZE)
		return 1;
	else return 0;
}

//Rich.cpp------------------------------------------------------------------------#

void Rich::print_rich() {
	arr.print();
}

void Rich::new_rich(int money, int id) { // 신규등록
	if (arr.top_value() <= money)
		arr.push(money, id);
}

void Rich::increse_rich(int money, int id) {	// 돈이 많아졌을때
	if (arr.top_value() > money)
		return;

	int idx = arr.update(money, id);	// id가 arr에 속해있을 경우 update

	if (idx == 0)	// 안속했을때
		arr.push(money, id);
}

void Rich::decrese_rich(int money, int id, Node* root) {	// 돈이 적어졌을때
	int idx = arr.update(money, id);	// arr 안에 id가 속했을 경우 update
	if (idx) { 	// 속했을때
		if (arr.top_value() == money && arr.top_key() == id)	// money, id가 arr 내의 최소값일 경우
			arr.setTop(search_max(arr.getLimit(), root)); // money 보다 큰 최대값이 있는지 찾음
	}
}

bool Ground::buy_ground(Member* buyer, Member* seller, int x, int y, int price) { // 땅 주인이 있으면 주인번호 리턴, 없으면 -1 리턴
	bool buy;
	int owner_idx = ground_idx[x][y], owner_price = ground_price[x][y];

	if (owner_idx == buyer->idx) {	// 이미 내가 주인
		buy = false;
	}
	else if (owner_idx != 0) {	// 이미 주인이 있음.
		if (price > buyer->money || price < ground_price[x][y])
			buy = false;
		else {
			buyer->spend_money(price);
			seller->charge_money(price);

			ground_idx[x][y] = buyer->idx;
			ground_price[x][y] = price;
			owner_idx = buyer->idx;

			buy = true;
		}
	}
	else if (price > buyer->money) {	// 주인 없는데 돈 부족
		owner_idx = -1;
		buy = false;
	}
	else {	// 주인 없고 돈 충분
		buyer->spend_money(price);

		ground_idx[x][y] = buyer->idx;
		ground_price[x][y] = price;
		owner_idx = buyer->idx;

		buy = true;
	}
	printf("%d %d %d\n", buy, buyer->money, owner_idx);

	if (buy)
		return 1;
	else
		return 0;
}

//Member.cpp------------------------------------------------------------------------#

void Member::setGrade() {
	if (money < 30000)
		grade = 0;
	else if (money < 50000)
		grade = 1;
	else if (money < 100000)
		grade = 2;
	else
		grade = 3;
}

int Member::charge_money(int add_money) {
	money += add_money;
	change_money_list.push_back(make_pair(1, add_money));	// 현재금액 변동 리스트에 추가

	setGrade();	// 등급 재설정
	return grade;
}

bool Member::spend_money(int use_money) {
	if (money >= use_money) {
		money -= use_money;
		change_money_list.push_back(make_pair(0, use_money));// 현재금액 변동 리스트에 추가

		setGrade();	// 등급 재설정
		return 1;
	}
	else
		return 0;
}

void Member::print_change_money_list(int cnt) {
	if (change_money_list.size() == 0) {	// 현재금액 변동 내역이 없을 경우 0 출력 후 종료
		printf("0\n");
		return;
	}

	for (int i = change_money_list.size() - 1; i >= 0; i--) {	// cnt 만큼 현재금액 변동 내역 출력
		printf("%d %d\n", change_money_list[i].first, change_money_list[i].second);
		cnt--;
		if (cnt == 0)
			return;
	}
}

//RBTree.cpp------------------------------------------------------------------------#

inline int RBTree::getIdx(Node* node) {
	return node->member->idx;
}

bool RBTree::doubleRed(Node* node) {
	if (node->parent == NULL)
		return false;
	else if (node->color == RED && node->parent->color == RED)	// node와 부모 모두 red일 때
		return true;
	else
		return false;

}

bool RBTree::isBlack(Node* node) {
	if (node == LEAF || node->color == BLACK)	// node가 leaf node이거나 color가 black 일때
		return true;
	else
		return false;
}

Node* RBTree::sibling(Node* node) {
	if (node->parent->right == node)	// 노드의 부모의 반대편 자식 노드 반환
		return node->parent->left;
	else
		return node->parent->right;
}

int RBTree::append_node(Node* node) {
	if (root == NULL) {	// 트리가 비었을 때
		root = node;
		node->color = BLACK;	// root property 만족
		return 0;
	}

	Node* temp = root;
	int depth = 0;

	//binary search tree
	while (temp != LEAF) {
		if (getIdx(node) < getIdx(temp)) { // left subtree
			if (temp->left == LEAF) {	// leaf node에 달아줌
				temp->left = node;
				node->parent = temp;
				break;
			}
			else
				temp = temp->left;
		}
		else if (getIdx(node) > getIdx(temp)) {// 오른쪽 subtree는 크거나 같음
			if (temp->right == LEAF) {
				temp->right = node;
				node->parent = temp;
				break;
			}
			else
				temp = temp->right;
		}
		else // 이미 값이 존재할 경우
			return -depth;
		depth++;
	}
	return depth + 1;
}

int RBTree::getDepth(Node* node) {
	int depth = 0;
	Node* temp = node;
	while (root != temp) {	// temp가 root가 될 때까지 반복
		temp = temp->parent;
		depth++;
	}
	return depth;
}

int RBTree::insert_member(int idx, char* name, char* phone, int grade, int money) { // 회원 삽입
																					// 생성
	Member* new_member = new Member(idx, name, phone, grade, money);
	Node* new_node = new Node(new_member);

	int depth = append_node(new_node);//먼저 삽입할 위치를 찾고 달아줌

	if (depth < 0)	// 이미 회원이 존재
		return depth;

	Node* z = new_node;

	//더블레드일 경우
	while (doubleRed(z)) {	// doubleRed일 경우 (부모와 부모의부모 존재)
		if (isBlack(sibling(z->parent))) {	// 삼촌노드가 black일 경우
			z = restructure(z);
			break;
		}
		else // 삼촌노드가 red일 경우
			z = recolor(z);	// z의 부모를 반환받고 while문 반복
	}
	return getDepth(new_node);	// depth를 재계산하여 반환
}

inline bool RBTree::isLeft(Node* node) {
	return (node->parent->left == node) ? true : false;
}

inline bool RBTree::isRight(Node* node) {
	return (node->parent->right == node) ? true : false;
}

Node* RBTree::restructure(Node* z) { // z->v->w
	Node* v = z->parent;	// z,v : red
	Node* x = v->parent;	// Black
	Node* t1 = NULL;
	Node* t2 = NULL;
	Node* t3 = NULL;
	Node* t4 = NULL;
	Node* parent_node = NULL;
	Node* left_node = NULL;
	Node* right_node = NULL;


	//4가지 케이스
	if (isLeft(z)) {
		if (isLeft(v)) { // 3
			t1 = z->left;
			t2 = z->right;
			t3 = v->right;
			t4 = x->right;

			parent_node = v;
			left_node = z;
			right_node = x;
		}
		else if (isRight(v)) { //	1
			t1 = x->left;
			t2 = z->left;
			t3 = z->right;
			t4 = v->right;

			parent_node = z;
			left_node = x;
			right_node = v;
		}
	}
	else if (isRight(z)) {
		if (isLeft(v)) {	// 2
			t1 = v->left;
			t2 = z->left;
			t3 = z->right;
			t4 = x->right;

			parent_node = z;
			left_node = v;
			right_node = x;
		}
		else if (isRight(v)) {	// 4
			t1 = x->left;
			t2 = v->left;
			t3 = z->left;
			t4 = z->right;

			parent_node = v;
			left_node = x;
			right_node = z;
		}
	}

	//subtree
	left_node->left = t1;
	left_node->right = t2;
	right_node->left = t3;
	right_node->right = t4;

	parent_node->left = left_node;
	parent_node->right = right_node;

	//parent
	if (x != root) {	// 부모가 존재
		parent_node->parent = x->parent;
		if (isLeft(x))
			x->parent->left = parent_node;
		else
			x->parent->right = parent_node;
	}
	else {
		parent_node->parent = NULL;
		root = parent_node;
	}
	left_node->parent = parent_node;
	right_node->parent = parent_node;

	if (t1 != LEAF)
		t1->parent = left_node;
	if (t2 != LEAF)
		t2->parent = left_node;
	if (t3 != LEAF)
		t3->parent = right_node;
	if (t4 != LEAF)
		t4->parent = right_node;



	//color
	parent_node->color = BLACK;
	left_node->color = RED;
	right_node->color = RED;

	return parent_node;
}

Node* RBTree::recolor(Node* z) {	// 부모는 red, 자식들은 black으로 설정
	Node* v = z->parent;
	Node* w = sibling(v);
	Node* x = v->parent;


	if (x != root)	// root는 무조건 검정이여야 하므로
		x->color = RED;

	v->color = BLACK;
	w->color = BLACK;

	return x;
}

bool RBTree::insert_new_member(int idx, char* name, char* phone) { // 신규회원가입
	int depth = insert_member(idx, name, phone, 0, 0);
	if (depth < 0) {	// 실패
		printf("%d 0\n", -depth);
		return 0;
	}
	else {	// 성공
		printf("%d 1\n", depth);
		return 1;
	}
}

pair<int, Member*> RBTree::search(int idx) {// RBTree 내에서 회원 찾기
	int depth = 0;
	pair<int, Member*> result(0, NULL);
	if (root == NULL)
		return result;

	Node* temp = root;

	//binary search tree
	while (temp != LEAF) {
		if (idx < getIdx(temp)) { // left subtree
			if (temp->left == LEAF) // leaf일 경우 못 찾음
				return result;
			else
				temp = temp->left;
		}
		else if (idx > getIdx(temp)) // 오른쪽 subtree는 크거나 같음
		{
			if (temp->right == LEAF)
				return result;
			else
				temp = temp->right;
		}
		else
			break;
		depth++;
	}

	if (temp != LEAF) {
		result.first = depth;
		result.second = temp->member;
	}

	return result;
}

void RBTree::print_member(int depth, Member* member) {	// 찾은 멤버에 대해 정보출력
	if (member == NULL) {
		printf("Not found!\n");
		return;
	}
	printf("%s %s %d %d %d\n", member->name, member->phone, member->grade, member->money, depth);
}

//main.cpp------------------------------------------------------------------------#

void read_file(const char* input_file, RBTree& tree, Ground& ground, Rich& rich) {

	FILE *rfp;

	char name[20] = { '\0' }, phone[11] = { '\0' };
	int id = 0, x = 0, y = 0, grade = 0, money = 0;

	if (rfp = fopen(input_file, "r")) {	// 파일을 연다.
		while (fscanf(rfp, "%d %s %s %d %d %d %d", &id, name, phone, &x, &y, &grade, &money) != EOF) {	// 파일을 한줄씩 받아 각 instance 함수 실행
			tree.insert_member(id, name, phone, grade, money);
			ground.ground_idx[x][y] = id;
			rich.new_rich(money, id);
		}
		fclose(rfp);	// 파일을 닫는다.
	}
	else
		printf("error\n");
}

int main() {
	RBTree tree;
	Ground ground;
	Rich rich;

	char input_file[100] = { '\0' };	// 입력파일
	printf("입력파일의 이름을 입력하세요 : ");
	scanf("%s", input_file);

	read_file(input_file, tree, ground, rich);	//input_file을 읽어 각 instance 함수 실행

	Member* member = NULL;
	Member* buyer = NULL;
	Member* seller = NULL;
	pair<int, Member*> depth_and_member = make_pair(0, (Member*)NULL);	// depth와 member 정보를 저장하는 pair
	char name[20] = { '\0' };
	char phone[11] = { '\0' };
	int idx, change_grade, add_money, cnt, buy_money;
	char command;	// 명령어

	while (1) {
		scanf("%c", &command);

		switch (command)
		{
		case 'I':	// 삽입
			int x, y;
			scanf("%d %s %s %d %d", &idx, name, phone, &x, &y);

			if (tree.insert_new_member(idx, name, phone)) {	// 회원가입 승인
				if (ground.ground_idx[x][y] == 0)
					ground.ground_idx[x][y] = idx;
			}
			break;

		case 'P':	// 회원정보 확인
			scanf("%d", &idx);

			depth_and_member = tree.search(idx);
			tree.print_member(depth_and_member.first, depth_and_member.second);
			break;

		case 'A':	// 충전
			scanf("%d %d", &idx, &add_money);

			depth_and_member = tree.search(idx);	// depth와 member 정보를 담음
			if (depth_and_member.second == NULL) // member를 못찾았을때
				printf("Not found!\n");
			else {	// member를 찾았을때
				change_grade = depth_and_member.second->charge_money(add_money);	// 금액 충전

				printf("%d %d\n", depth_and_member.first, change_grade);

				rich.increse_rich(depth_and_member.second->money, idx);	// 현재금액 상위 5명 갱신
			}
			break;

		case 'F':	// 검색
			rich.print_rich();	// 현재금액 상위 5명 출력
			break;

		case 'R':	// 특정 회원의 최근 금액변동내역 조회
			scanf("%d %d", &idx, &cnt);

			member = tree.search(idx).second;	// idx에 대한 member 가져옴
			if (member == NULL)	// 없을 경우
				printf("Not found!\n");
			else // 있으면 member에 대해 현재 금액 변동내역 출력
				member->print_change_money_list(cnt);
			break;

		case 'B':	// 땅 구매
			scanf("%d %d %d %d", &idx, &x, &y, &buy_money);

			buyer = tree.search(idx).second;	// 땅 사는 회원의 member 정보
			seller = tree.search(ground.ground_idx[x][y]).second; // 땅 파는 회원의 member 정보

			if (ground.buy_ground(buyer, seller, x, y, buy_money)) {	// 구매가 성공하였을 때
				rich.decrese_rich(buyer->money, buyer->idx, tree.getRoot());	// 구매자의 현재금액 감소
				if (seller != NULL)	// 판매자가 없을 경우
					rich.increse_rich(seller->money, seller->idx);
			}

			break;
		}
		if (command == 'Q')	// 종료
			break;
	}
}