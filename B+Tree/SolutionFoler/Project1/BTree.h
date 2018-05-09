#ifndef BTree_H
#define BTree_H

#include "STL.h"
#include "func.h"

#define NON_LEAF 0
#define LEAF 1

class BTree {
public:
	BTree(string file_name);	// B+tree ������, file�� blockSize, rootId, depth�� �ҷ����´�.
	Node BTree::get_block(unsigned int block_id, bool type);	// file�� read�Ͽ� �ش� block_id�� ���� ��带 ����ü�� ��� ��ȯ�Ѵ�.
	void set_block(const Node& node, bool type);				// ��� ����ü�� file�� write�Ѵ�.

	//insert
	Node new_node();	// ���ο� ��带 �����ϰ�, block_id�� �Ҵ��Ͽ� ��ȯ
	void insert_file(string inputfile);
	void insert(unsigned int k, unsigned int p);	// key�� value�� pair�� B+Tree�� �����Ѵ�.
	void insert_in_pair(vector<pair<unsigned int, unsigned int>>& v, unsigned int k, unsigned int p);	// ��� ���� new node�� �����ϰ� �������� �����Ѵ�.
	void insert_in_parent(unsigned int n, unsigned int k, unsigned int n_, stack<unsigned int>& parent);	// �θ� ��忡�� ������ �ʿ��ϰų�, split�� �Ͼ���� üũ, ��������� ����
	Node find_leafnode(unsigned int k, stack<unsigned int>& parent);
	// new node�� �� leaf node�� ��ġ�� ã�� ��ȯ (parant stack�� leafnode�� ã���� ������ �θ��� �ּҸ� �����Ѵ�=>�θ𿡼� split�� �Ͼ �� �ֱ� ����)


	//(���� �̿�) ��ü ��� Ȯ��
	void print();


	//print B+tree structure
	void print_struct(string outputfile);	// depth <0>, <1>�� ��带 ��� ���
	void print_node(const Node& node, ofstream& fout);	// �ش� node ���� �ִ� element ���� ��� ���


	//search
	void search_file(string inputfile, string outputfile);	//	exact search file�� �ҷ���
	void search(int key, ofstream& fout); // exact seach

	void search_file_range(string inputfile, string outputfile);	// range serch file�� �ҷ���
	void search(int start_range, int end_range, ofstream& fout); // range search

	Node& search_leaf(Node& node, int key, int now_depth);	// key�� �ش��ϴ� leaf node�� ��ȯ
	int get_value(const Node& node, int key);	 // node ������ key���� ���� value�� ��ȯ


private:
	string file_name;
	unsigned int block_size = 0;
	unsigned int root_id = 0;//��Ʈ�� block ��ȣ
	unsigned int depth = 0;	// Ʈ���� ����

	unsigned int new_node_num = 0;

};

#endif