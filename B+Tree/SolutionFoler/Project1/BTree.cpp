#include "STL.h"
#include "func.h"
#include "Btree.h"

BTree::BTree(string file_name) {	// file_name�� file�� �ҷ��鿩 class�� ���� B+Tree ������ �����Ѵ�.
	this->file_name = file_name;

	ifstream fin(file_name, ios::binary | ios::in);

	four_byte_read(fin, block_size);
	four_byte_read(fin, root_id);
	four_byte_read(fin, depth);
	//cout << fin.tellg() << endl;

	block_size = (block_size - 4) / 8;	// �� �� �� �ִ� ������ ����

	if (block_size < 1)
		block_size = 0;

	//cout << "block_size : " << block_size << endl;
	//cout << "root_id : " << root_id << endl;
	//cout << "Depth : " << depth << endl;
}

Node BTree::get_block(unsigned int block_id, bool type) {
	ifstream fin(file_name, ios::binary | ios::in);

	struct Node node;
	node.block_id = block_id;
	node.v.resize(block_size);
	unsigned int move = 12 + ((block_id - 1) * ((block_size * 8) + 4));
	fin.seekg(move, ios::beg);

	if (type == NON_LEAF) {
		four_byte_read(fin, node.ptr);
		for (int i = 0; i < block_size; i++) {
			four_byte_read(fin, node.v[i].first);
			four_byte_read(fin, node.v[i].second);
		}
	}
	else {	// LEAF
		for (int i = 0; i < block_size; i++) {
			four_byte_read(fin, node.v[i].first);
			four_byte_read(fin, node.v[i].second);
		}
		four_byte_read(fin, node.ptr);
	}
	for (unsigned int i = 0; i < node.v.size(); i++) {
		if (node.v[i].first == 0)
			break;
		node.v_size++;
	}

	fin.close();
	return node;
}

void BTree::set_block(const Node& node, bool type) {
	ofstream fout(file_name, ios::binary | ios::out | ios::in);
	unsigned int move = 12 + ((node.block_id - 1) * ((block_size * 8) + 4));
	fout.seekp(move, ios::beg);

	if (type == NON_LEAF) {
		four_byte_write(fout, node.ptr);
		for (int i = 0; i < block_size; i++) {
			four_byte_write(fout, node.v[i].first);
			four_byte_write(fout, node.v[i].second);
		}
	}
	else {	// LEAF
		for (int i = 0; i < block_size; i++) {
			four_byte_write(fout, node.v[i].first);
			four_byte_write(fout, node.v[i].second);
		}
		four_byte_write(fout, node.ptr);
	}
	fout.close();
}


Node BTree::new_node() {
	struct Node node;
	node.block_id = ++new_node_num;
	node.v.resize(block_size);

	ofstream fout(file_name, ios::binary | ios::out | ios::in);
	fout.seekp(0, ios::end);

	if (block_size > 0)
		four_byte_write(fout, 0);

	for (unsigned int i = 0; i < block_size; i++) {	// �� ���(���) ����
		four_byte_write(fout, 0);
		four_byte_write(fout, 0);
	}

	fout.close();
	return node;
}

void BTree::insert_file(string inputfile) {
	ifstream fin(inputfile, ios::binary | ios::in);
	unsigned int key, value;

	//block insert
	unsigned int cnt = 0;

	while (!fin.eof()) {
		if (!(fin >> key >> value))
			break;

		insert(key, value);
	}

	//������ exit
	ofstream fout(file_name, ios::binary | ios::out | ios::in);
	fout.seekp(4, ios::beg);
	four_byte_write(fout, root_id);
	four_byte_write(fout, depth);
	fout.close();
	fin.close();
}

void BTree::insert(unsigned int k, unsigned int p) {
	//cout << "[insert " << k << ", " << p << "]" << endl;
	Node l;	// ������ �� �� leaf node�� �ּҸ� ���� ��
	stack<unsigned int> parent;	// leaf node�� ã�� ������ ���İ��� ��� parent�� �ּ� ����

	//Tree emtpy ����
	if (root_id == 0) {	// tree�� ����ٸ� depth = 0
		l = new_node();
		root_id = l.block_id;
		//cout << "create	new root : " << l.block_id << endl;
	}
	else	// (�����͸� ������ leaf node block�� ã�� �۾�) block�� ���� �� k�� ������ block�� ã�´�.
		l = find_leafnode(k, parent);

	// leaf node�� ������ ����
	if (l.v_size < block_size) {	// ���� ������ ����Ҷ�
		//cout << "insert leaf node" << endl;
		insert_in_pair(l.v, k, p);	// node�� k,p ����
		l.v_size++;
		set_block(l, LEAF);
	}
	else {	//���� ���� ���� => ���ø�	// ���⼭���� �ڵ� ����
		//cout << "be full to block. execute split" << endl;

		Node l_;
		l_ = new_node();

		vector<pair<unsigned int, unsigned int>> t;
		t = l.v;	// vector copy
		t.resize(t.size() + 1);
		insert_in_pair(t, k, p);

		l.v = l_.v;	// ���� �ʱ�ȭ

		if (l.ptr != 0)
			l_.ptr = l.ptr;
		l.ptr = l_.block_id;	// leaf node������ ���� ��带 ����Ű�� ��

		int scope = ceil((double)(t.size()) / 2);	// ������ ������ ���� �ε��� block_size 4�̸� t.size = 5, �ε��� 3�� �������� ������ 0 1 2 / 3 4
													// split
		for (int i = 0; i < scope; i++)
			l.v[i] = t[i];
		l.v_size = scope;

		for (int i = scope; i < t.size(); i++)
			l_.v[i - (scope)] = t[i];
		l_.v_size = t.size() - scope;

		set_block(l, LEAF);
		set_block(l_, LEAF);
		// ���� ��忡 ������ l_�� key ���� �ִ´�.
		int small_key = l_.v[0].first;
		insert_in_parent(l.block_id, small_key, l_.block_id, parent);
	}
	//cout << "[insert exit " << k << ", " << p << "]" << endl;
}

void BTree::insert_in_parent(unsigned int n, unsigned int k_, unsigned int n_, stack<unsigned int>& parent) {	// ���Ե� node�� parent������ ����(non_leaf_node)
	if (n == root_id) {	// n�� ��Ʈ�� �� ���ο� �θ� ����
		depth++;
		Node r;
		r = new_node();
		//cout << "(parent)create new root : " << r.block_id << endl;
		root_id = r.block_id;
		// root�� �ڽ� block�� n, n_�� ����Ű���� �Ѵ�.
		r.ptr = n;
		r.v[0] = make_pair(k_, n_);
		set_block(r, NON_LEAF);
		// ���� �߰�

		return;
	}

	// ���� node�� parent node�� �ּ�
	if (parent.empty())
		return;

	Node p;
	p.block_id = parent.top();
	p = get_block(p.block_id, NON_LEAF);
	parent.pop();
	//cout << "add parent ---" << p.block_id << endl; 

	// p block(non_leaf_node)�� ���� ���� �ʾ��� ���
	if (p.v_size < block_size) {
		//cout << "�θ� ����" << endl;
		// non_leaf_node�� �� k_�� ���ο� ����� �ּ� n_�� �ִ´�.
		insert_in_pair(p.v, k_, n_);
		p.v_size++;
		set_block(p, NON_LEAF);
	}
	else {	// p�� �� �� ��� p�� split
		//cout << "parent split" << endl;
		Node p_;
		p_ = new_node();

		vector<pair<unsigned int, unsigned int>> t;
		t = p.v;
		t.resize(t.size() + 1);
		insert_in_pair(t, k_, n_);
		p.v_size++;

		p.v = p_.v;

		int scope = ceil((double)t.size() / 2) - 1;	// ������ ������ ���� �ε���, block_size 4�̸� t.size = 5(ptr�� �� 6���̹Ƿ� 3����), �ε��� 2�� �������� ������ 0 1 / 2 / 3 4 

		for (int i = 0; i < scope; i++)
			p.v[i] = t[i];
		p.v_size = scope;

		p_.ptr = t[scope].second;

		for (int i = scope + 1; i < t.size(); i++)
			p_.v[i - (scope + 1)] = t[i];
		p_.v_size = t.size() - scope;

		set_block(p, NON_LEAF);
		set_block(p_, NON_LEAF);
		// ���� ��忡 ������ l_�� key ���� �ִ´�(���)
		int small_key = t[scope].first;
		insert_in_parent(p.block_id, small_key, p_.block_id, parent);
	}
}

void BTree::insert_in_pair(vector<pair<unsigned int, unsigned int>>& v, unsigned int k, unsigned int p) {
	for (int i = 0; i < v.size(); i++) {
		if (v[i].first == 0) {
			v[i].first = k;
			v[i].second = p;
			sort(v.begin(), v.begin() + i + 1);
			break;
		}
	}
}

Node BTree::find_leafnode(unsigned int k, stack<unsigned int>& parent) {	// k�� ������(Ȥ�� �����ϴ�) block�� ã����
	//cout << "find_leafnode" << endl;
	unsigned int depth = 0;
	Node node;

	if (this->depth == 0) {
		node = get_block(root_id, LEAF);
		return node;
	}
	else
		node = get_block(root_id, NON_LEAF);

	while (depth < this->depth) {	// leaf ��带 ã�´�.
		//cout << "non-leaf : " << node.block_id << endl;

		unsigned int temp = node.block_id;

		for (vector<pair<unsigned int, unsigned int>>::iterator iter = node.v.begin(); iter != node.v.end(); iter++) {
			if (k < iter->first) {	// �̵��ؾ��� �κ�
				parent.push(node.block_id);	// parent stack �ȿ� ����(�θ� split�� �Ͼ �� �θ��� �ּҸ� �����ؾ��ϹǷ�)

				// �ڽ� ���� �̵�
				if (iter == node.v.begin())	//ó�� �������϶�
					node.block_id = node.ptr;
				else
					node.block_id = (iter - 1)->second;	//cout << (iter - 1)->first << " move." << endl;

				break;
			}
		}

		if (node.block_id == temp) {	// ã�� ���Ͽ��� �� (���� ū ���� ����)
			parent.push(node.block_id);	// �ּ� ����
			node.block_id = node.v[node.v_size - 1].second;	// k�� ���� non_leaf_node�� ��� ������ Ŭ �� ������ ����Ʈ�� �̵�
		}

		depth++;			// ���� ����

		if (depth < this->depth)
			node = get_block(node.block_id, NON_LEAF);
		else if (depth == this->depth)
			node = get_block(node.block_id, LEAF);
	}

	return node;
}


// (�����̿�) ��� ��带 depth�� ������ ���
void BTree::print() {
	ifstream fin(file_name, ios::binary | ios::in);

	Node node;
	if (depth == 0) {
		node = get_block(root_id, LEAF);
		return;
	}

	vector<queue<int>> q;
	int level = 0;
	q.resize(level + 2);
	q[level].push(root_id);

	//pirnt nonleaf node (use BFS)
	while (!q[level].empty()) {
		cout << "<" << level << ">" << endl;
		while (!q[level].empty()) {
			node = get_block(q[level].front(), NON_LEAF);
			q[level].pop();

			int node_ptr = node.ptr;
			if (node_ptr != 0 && level != depth)
				q[level + 1].push(node_ptr);

			cout << "(" << node.block_id << ":";
			for (int i = 0; i < node.v_size; i++) {
				cout << node.v[i].first;
				if (i != node.v_size - 1)
					cout << ", ";

				if (node.v[i].second != 0 && level != depth)
					q[level + 1].push(node.v[i].second);
			}
			cout << ") ";
		}
		cout << endl;
		level++;
		q.resize(level + 2);

		//print leaf node
		if (level == depth) {
			cout << "<" << level << ">" << endl;
			while (!q[level].empty()) {
				node = get_block(q[level].front(), LEAF);
				q[level].pop();
				cout << "(" << node.block_id << ":";
				for (int i = 0; i < node.v_size; i++) {
					cout << node.v[i].first;
					if (i != node.v_size - 1)
						cout << ", ";
				}
				cout << ")";
			}
			cout << endl;
			break;
		}
	}
}


void BTree::print_struct(string outputfile) {
	ifstream fin(file_name, ios::binary | ios::in);
	ofstream fout(outputfile, ios::out);

	int kind;
	Node node;

	if (depth == 0)
		kind = LEAF;
	else
		kind = NON_LEAF;

	fout << "< 0 >" << endl;
	node = get_block(root_id, kind);
	print_node(node, fout);
	fout << endl;

	if (depth == 0)	// depth�� 0�̸� �Լ� ����
		return;
	else if (depth == 1)	// ���� Ž���� node�� leaf node
		kind = LEAF;
	else	// non leaf node
		kind = NON_LEAF;


	fout << "< 1 >" << endl;
	Node temp = get_block(node.ptr, kind);
	print_node(temp, fout);
	fout << ", ";
	for (int i = 0; i < node.v_size; i++) {
		Node temp = get_block((node.v[i].second), kind);	// non_leaf, leaf ������� leaf��� ������
		print_node(temp, fout);
		if (i != node.v_size - 1)
			fout << ", ";
	}
	fout << endl;
}

void BTree::print_node(const Node& node, ofstream& fout) {	// ��忡 �ִ� ������ print
	for (int i = 0; i < node.v_size; i++) {
		fout << node.v[i].first;
		if (i != node.v_size - 1)
			fout << ", ";
	}
}


void BTree::search_file(string inputfile, string outputfile) {
	ifstream fin(inputfile, ios::binary | ios::in);
	ofstream fout(outputfile, ios::out);
	int key;

	while (!fin.eof()) {	// problem : �������� ���� ������ 2�� ��	
		//system("pause");
		if (!(fin >> key))
			break;

		search(key, fout);
	}
}

void BTree::search(int key, ofstream& fout) {
	ifstream fin(file_name, ios::binary | ios::in);

	Node node;

	if (depth == 0) {
		node = get_block(root_id, LEAF);
		fout << key << "\t" << get_value(node, key) << endl;
		return;
	}

	node = get_block(root_id, NON_LEAF);
	node = search_leaf(node, key, depth);
	fout << key << "\t" << get_value(node, key) << endl;

}

void BTree::search(int start, int end, ofstream& fout) {
	ifstream fin(file_name, ios::binary | ios::in);

	Node node;

	if (depth == 0) {
		node = get_block(root_id, LEAF);

		for (int i = 0; i < node.v_size; i++) {
			if (start <= node.v[i].first && node.v[i].first <= end)
				fout << node.v[i].first << "\t" << node.v[i].second << endl;
		}
		return;
	}


	node = get_block(root_id, NON_LEAF);
	node = search_leaf(node, start, depth);

	bool comma = 0;
	while (1) {
		for (int i = 0; i < node.v_size; i++) {
			if (start > node.v[i].first)
				continue;
			else if (node.v[i].first > end)
				return;
			else {
				if (comma)
					fout << " , ";

				fout << node.v[i].first << '\t' << node.v[i].second;
				comma = 1;
			}
		}
		if (node.ptr == NULL) {
			return;
		}
		else
			node = get_block(node.ptr, LEAF);
	}

}

void BTree::search_file_range(string inputfile, string outputfile) {
	ifstream fin(inputfile, ios::binary | ios::in);
	ofstream fout(outputfile, ios::out);

	int start, end;

	while (!fin.eof()) {	// problem : �������� ���� ������ 2�� ��	
							//system("pause");
		if (!(fin >> start >> end))
			break;

		search(start, end, fout);
		fout << endl;
	}
}

Node& BTree::search_leaf(Node& node, int key, int now_depth) {
	for (int i = 0; i < node.v_size; i++) {
		if (key < node.v[i].first) {
			//cout << node.v[i].first << " -> ";
			int type;

			if (now_depth - 1 == 0)
				type = LEAF;
			else
				type = NON_LEAF;

			if (i == 0)
				node = get_block(node.ptr, type);
			else
				node = get_block(node.v[i - 1].second, type);

			if (now_depth - 1 == 0)
				return node;
			else
				return search_leaf(node, key, now_depth - 1);
		}
	}

	// ������ ���
	int type;

	if (now_depth - 1 == 0)
		type = LEAF;
	else
		type = NON_LEAF;

	node = get_block(node.v[node.v_size - 1].second, type);

	if (now_depth - 1 == 0)
		return node;
	else
		return search_leaf(node, key, now_depth - 1);

}

int BTree::get_value(const Node& node, int key) {
	for (int i = 0; i < node.v_size; i++) {
		if (node.v[i].first == key)
			return node.v[i].second;
	}
	return -1;
}