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
	int idx;				// ȸ����ȣ
	char name[20] = { 0 };	// �̸�
	char phone[11] = { 0 }; // ��ȭ��ȣ
	int grade;	//����ݾ׿� ���� ȸ�����(�Ϲ�: 0 , �ǹ�: 30,000, ���: 50,000, ���̾�: 100,000)
	int money;	//����ݾ�
	vector<pair<int, int>> change_money_list;	// ����ݾ� �������� ����

												// ������
	Member() {};
	Member(int idx, char* name, char* phone, int grade, int money)
		: idx(idx), grade(grade), money(money) {
		strcpy(this->name, name);
		strcpy(this->phone, phone);
	};

	void setGrade();	// ȸ����� ����
	int charge_money(int add_money); // �����ϱ�, ��� ����
	bool spend_money(int money); // ���� ��ȭ
	void print_change_money_list(int cnt);	//���� ��ȭ ���
};

class Node {
public:
	int color;	// RED and BLACK
	Node* left;	// left child
	Node* right;	// right child
	Node* parent;	// parent child
	Member* member;	// ȸ������

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
	int insert_member(int idx, char* name, char* phone, int grade, int money);	// ȸ�� ����
	bool insert_new_member(int idx, char* name, char* phone); // �ű�ȸ������
	pair<int, Member*> search(int idx);	// ȸ�� ã��
	void print_member(int depth, Member* member);	// ȸ������ ���

	Node* getRoot() {	// root�� ����
		return root;
	}

private:
	Node* root = NULL;

	int append_node(Node* node);	// idx�� �� �ڸ��� �θ� ��ȯ, idx �̹� ������ �� �ش� depth ��ȯ
	inline int getIdx(Node* node);	// node�� ���� memeber�� idx ��ȯ

									//insert
	bool doubleRed(Node* node);	// doubleRed���� �Ǻ�
	bool isBlack(Node* node);	// black node���� �Ǻ�
	Node* sibling(Node* node);	// node�� sibling�� Node ������ ��ȯ

	inline bool isLeft(Node* node);	// node�� �θ��� ���ʳ������ Ȯ��
	inline bool isRight(Node* node);	// node�� �θ��� ������ ������� Ȯ��
	int getDepth(Node* node);	// node�� depth�� ���Ѵ�.

	Node* restructure(Node* z);	// restructuring ����
	Node* recolor(Node* z);	// recoloring ����

};

class Ground {
public:
	int ground_idx[1000][1000] = { 0 };	// ���� ������ �Ǵ� ȸ����ȣ ����
	int ground_price[1000][1000] = { 0 };	// ���� ����
	bool buy_ground(Member* buyer, Member* seller, int x, int y, int price);	// buyer�� seller ���� ��ǥ x,y�� ���Ͽ� price ���ݿ� �ŷ��� ����
};

class Priority_queue {
private:
	pair<int, int> arr[SIZE] = { make_pair(0,0) };	// value : money, key : id
	int size = 0;	// arr �� element size
	int last_idx = -1;	// ���� ������ idx
	void sort();	// arr ����

public:
	pair<int, int> top();	// arr�� �ּ� value�� pair ��ȯ
	int top_value();	// arr�� �ּ� value�� value ��ȯ
	int top_key();	// arr�� �ּ� value�� key ��ȯ

	void push(int value, int key);	// arr�� value, key pair�� ����
	bool update(int value, int key);	//  arr�� key�� ���� value �� ����, ���� �� 0 ��ȯ
	void print();	// arr�� value, key ���

	bool isFull();	// arr�� �� �� ��� 1 ��ȯ
	bool isEmpty();	// arr�� �� ��� 1 ��ȯ

	pair<int, int> getLimit() {	// arr�� �ּ� value ������ index ��ȯ
		return arr[last_idx - 1];
	}
	void setTop(pair<int, int> p) {	// arr�� �ּ� value�� ���� pair�� setting
		arr[last_idx] = p;
	}
};

class Rich {
private:
	Priority_queue arr;	// ����ݾ� ���� 5���� �����ϴ� �ڷᱸ��

public:
	void print_rich();	// ����ݾ� ���� 5���� ���
	void new_rich(int money, int id);	// money, id�� arr�� �� �� �ִ��� Ȯ�� �� ����
	void increse_rich(int money, int id);	// ���� ���� �Ͽ��� ����� ��ƾ
	void decrese_rich(int money, int id, Node* root);	// ���� �����Ͽ��� ����� ��ƾ
};

//Function------------------------------------------------------------------------#

pair<int, int> compare(pair<int, int> a, pair<int, int> b) {	// money�� �� ũ�ų�, ���ٸ� idx�� �� ���� ���� ��ȯ
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

pair<int, int> travel(pair<int, int> limit, pair<int, int> max_val, Node* node) {	// Tree�� ��ȸ(Preorder ���)
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

pair<int, int> search_max(pair<int, int> limit, Node* root) {	// limit���ٴ� ���� �ִ밪�� root�� ���� ã��
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
		if (arr[last_idx].first == value && arr[last_idx].second < key)	// ���� ������ Ű ���� �� Ŭ ���
			return;
	}
	last_idx = (last_idx < SIZE - 1) ? (last_idx + 1) : last_idx;	// arr ������ ��쿡�� last_idx�� ������ index�� ����
	size = (size < SIZE) ? (size + 1) : size;
	arr[last_idx] = make_pair(value, key);
	sort();
}

bool Priority_queue::update(int value, int key) {	// ���ڿ� ���ϴ��� Ȯ��, ������ idx, ������ -1
	for (int i = 0; i < size; i++) {
		if (arr[i].second == key) {	// arr �� ������ ��� update
			arr[i].first = value;
			sort();
			return 1;
		}
	}
	return 0;	// �������� ���� ���
}

void Priority_queue::print() {
	if (size == 0)	// �ƹ��͵� ���� ���
		printf("Not found!\n");
	else {
		for (int i = 0; i < size; i++)
			printf("%d %d\n", arr[i].second, arr[i].first);
	}
}

void Priority_queue::sort() {	// value�� �������� �������� ����
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

void Rich::new_rich(int money, int id) { // �űԵ��
	if (arr.top_value() <= money)
		arr.push(money, id);
}

void Rich::increse_rich(int money, int id) {	// ���� ����������
	if (arr.top_value() > money)
		return;

	int idx = arr.update(money, id);	// id�� arr�� �������� ��� update

	if (idx == 0)	// �ȼ�������
		arr.push(money, id);
}

void Rich::decrese_rich(int money, int id, Node* root) {	// ���� ����������
	int idx = arr.update(money, id);	// arr �ȿ� id�� ������ ��� update
	if (idx) { 	// ��������
		if (arr.top_value() == money && arr.top_key() == id)	// money, id�� arr ���� �ּҰ��� ���
			arr.setTop(search_max(arr.getLimit(), root)); // money ���� ū �ִ밪�� �ִ��� ã��
	}
}

bool Ground::buy_ground(Member* buyer, Member* seller, int x, int y, int price) { // �� ������ ������ ���ι�ȣ ����, ������ -1 ����
	bool buy;
	int owner_idx = ground_idx[x][y], owner_price = ground_price[x][y];

	if (owner_idx == buyer->idx) {	// �̹� ���� ����
		buy = false;
	}
	else if (owner_idx != 0) {	// �̹� ������ ����.
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
	else if (price > buyer->money) {	// ���� ���µ� �� ����
		owner_idx = -1;
		buy = false;
	}
	else {	// ���� ���� �� ���
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
	change_money_list.push_back(make_pair(1, add_money));	// ����ݾ� ���� ����Ʈ�� �߰�

	setGrade();	// ��� �缳��
	return grade;
}

bool Member::spend_money(int use_money) {
	if (money >= use_money) {
		money -= use_money;
		change_money_list.push_back(make_pair(0, use_money));// ����ݾ� ���� ����Ʈ�� �߰�

		setGrade();	// ��� �缳��
		return 1;
	}
	else
		return 0;
}

void Member::print_change_money_list(int cnt) {
	if (change_money_list.size() == 0) {	// ����ݾ� ���� ������ ���� ��� 0 ��� �� ����
		printf("0\n");
		return;
	}

	for (int i = change_money_list.size() - 1; i >= 0; i--) {	// cnt ��ŭ ����ݾ� ���� ���� ���
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
	else if (node->color == RED && node->parent->color == RED)	// node�� �θ� ��� red�� ��
		return true;
	else
		return false;

}

bool RBTree::isBlack(Node* node) {
	if (node == LEAF || node->color == BLACK)	// node�� leaf node�̰ų� color�� black �϶�
		return true;
	else
		return false;
}

Node* RBTree::sibling(Node* node) {
	if (node->parent->right == node)	// ����� �θ��� �ݴ��� �ڽ� ��� ��ȯ
		return node->parent->left;
	else
		return node->parent->right;
}

int RBTree::append_node(Node* node) {
	if (root == NULL) {	// Ʈ���� ����� ��
		root = node;
		node->color = BLACK;	// root property ����
		return 0;
	}

	Node* temp = root;
	int depth = 0;

	//binary search tree
	while (temp != LEAF) {
		if (getIdx(node) < getIdx(temp)) { // left subtree
			if (temp->left == LEAF) {	// leaf node�� �޾���
				temp->left = node;
				node->parent = temp;
				break;
			}
			else
				temp = temp->left;
		}
		else if (getIdx(node) > getIdx(temp)) {// ������ subtree�� ũ�ų� ����
			if (temp->right == LEAF) {
				temp->right = node;
				node->parent = temp;
				break;
			}
			else
				temp = temp->right;
		}
		else // �̹� ���� ������ ���
			return -depth;
		depth++;
	}
	return depth + 1;
}

int RBTree::getDepth(Node* node) {
	int depth = 0;
	Node* temp = node;
	while (root != temp) {	// temp�� root�� �� ������ �ݺ�
		temp = temp->parent;
		depth++;
	}
	return depth;
}

int RBTree::insert_member(int idx, char* name, char* phone, int grade, int money) { // ȸ�� ����
																					// ����
	Member* new_member = new Member(idx, name, phone, grade, money);
	Node* new_node = new Node(new_member);

	int depth = append_node(new_node);//���� ������ ��ġ�� ã�� �޾���

	if (depth < 0)	// �̹� ȸ���� ����
		return depth;

	Node* z = new_node;

	//�������� ���
	while (doubleRed(z)) {	// doubleRed�� ��� (�θ�� �θ��Ǻθ� ����)
		if (isBlack(sibling(z->parent))) {	// ���̳�尡 black�� ���
			z = restructure(z);
			break;
		}
		else // ���̳�尡 red�� ���
			z = recolor(z);	// z�� �θ� ��ȯ�ް� while�� �ݺ�
	}
	return getDepth(new_node);	// depth�� �����Ͽ� ��ȯ
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


	//4���� ���̽�
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
	if (x != root) {	// �θ� ����
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

Node* RBTree::recolor(Node* z) {	// �θ�� red, �ڽĵ��� black���� ����
	Node* v = z->parent;
	Node* w = sibling(v);
	Node* x = v->parent;


	if (x != root)	// root�� ������ �����̿��� �ϹǷ�
		x->color = RED;

	v->color = BLACK;
	w->color = BLACK;

	return x;
}

bool RBTree::insert_new_member(int idx, char* name, char* phone) { // �ű�ȸ������
	int depth = insert_member(idx, name, phone, 0, 0);
	if (depth < 0) {	// ����
		printf("%d 0\n", -depth);
		return 0;
	}
	else {	// ����
		printf("%d 1\n", depth);
		return 1;
	}
}

pair<int, Member*> RBTree::search(int idx) {// RBTree ������ ȸ�� ã��
	int depth = 0;
	pair<int, Member*> result(0, NULL);
	if (root == NULL)
		return result;

	Node* temp = root;

	//binary search tree
	while (temp != LEAF) {
		if (idx < getIdx(temp)) { // left subtree
			if (temp->left == LEAF) // leaf�� ��� �� ã��
				return result;
			else
				temp = temp->left;
		}
		else if (idx > getIdx(temp)) // ������ subtree�� ũ�ų� ����
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

void RBTree::print_member(int depth, Member* member) {	// ã�� ����� ���� �������
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

	if (rfp = fopen(input_file, "r")) {	// ������ ����.
		while (fscanf(rfp, "%d %s %s %d %d %d %d", &id, name, phone, &x, &y, &grade, &money) != EOF) {	// ������ ���پ� �޾� �� instance �Լ� ����
			tree.insert_member(id, name, phone, grade, money);
			ground.ground_idx[x][y] = id;
			rich.new_rich(money, id);
		}
		fclose(rfp);	// ������ �ݴ´�.
	}
	else
		printf("error\n");
}

int main() {
	RBTree tree;
	Ground ground;
	Rich rich;

	char input_file[100] = { '\0' };	// �Է�����
	printf("�Է������� �̸��� �Է��ϼ��� : ");
	scanf("%s", input_file);

	read_file(input_file, tree, ground, rich);	//input_file�� �о� �� instance �Լ� ����

	Member* member = NULL;
	Member* buyer = NULL;
	Member* seller = NULL;
	pair<int, Member*> depth_and_member = make_pair(0, (Member*)NULL);	// depth�� member ������ �����ϴ� pair
	char name[20] = { '\0' };
	char phone[11] = { '\0' };
	int idx, change_grade, add_money, cnt, buy_money;
	char command;	// ��ɾ�

	while (1) {
		scanf("%c", &command);

		switch (command)
		{
		case 'I':	// ����
			int x, y;
			scanf("%d %s %s %d %d", &idx, name, phone, &x, &y);

			if (tree.insert_new_member(idx, name, phone)) {	// ȸ������ ����
				if (ground.ground_idx[x][y] == 0)
					ground.ground_idx[x][y] = idx;
			}
			break;

		case 'P':	// ȸ������ Ȯ��
			scanf("%d", &idx);

			depth_and_member = tree.search(idx);
			tree.print_member(depth_and_member.first, depth_and_member.second);
			break;

		case 'A':	// ����
			scanf("%d %d", &idx, &add_money);

			depth_and_member = tree.search(idx);	// depth�� member ������ ����
			if (depth_and_member.second == NULL) // member�� ��ã������
				printf("Not found!\n");
			else {	// member�� ã������
				change_grade = depth_and_member.second->charge_money(add_money);	// �ݾ� ����

				printf("%d %d\n", depth_and_member.first, change_grade);

				rich.increse_rich(depth_and_member.second->money, idx);	// ����ݾ� ���� 5�� ����
			}
			break;

		case 'F':	// �˻�
			rich.print_rich();	// ����ݾ� ���� 5�� ���
			break;

		case 'R':	// Ư�� ȸ���� �ֱ� �ݾ׺������� ��ȸ
			scanf("%d %d", &idx, &cnt);

			member = tree.search(idx).second;	// idx�� ���� member ������
			if (member == NULL)	// ���� ���
				printf("Not found!\n");
			else // ������ member�� ���� ���� �ݾ� �������� ���
				member->print_change_money_list(cnt);
			break;

		case 'B':	// �� ����
			scanf("%d %d %d %d", &idx, &x, &y, &buy_money);

			buyer = tree.search(idx).second;	// �� ��� ȸ���� member ����
			seller = tree.search(ground.ground_idx[x][y]).second; // �� �Ĵ� ȸ���� member ����

			if (ground.buy_ground(buyer, seller, x, y, buy_money)) {	// ���Ű� �����Ͽ��� ��
				rich.decrese_rich(buyer->money, buyer->idx, tree.getRoot());	// �������� ����ݾ� ����
				if (seller != NULL)	// �Ǹ��ڰ� ���� ���
					rich.increse_rich(seller->money, seller->idx);
			}

			break;
		}
		if (command == 'Q')	// ����
			break;
	}
}