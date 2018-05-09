#ifndef BTree_H
#define BTree_H

#include "STL.h"
#include "func.h"

#define NON_LEAF 0
#define LEAF 1

class BTree {
public:
	BTree(string file_name);	// B+tree 생성자, file의 blockSize, rootId, depth를 불러들어온다.
	Node BTree::get_block(unsigned int block_id, bool type);	// file을 read하여 해당 block_id에 대한 노드를 구조체에 담아 반환한다.
	void set_block(const Node& node, bool type);				// 노드 구조체를 file에 write한다.

	//insert
	Node new_node();	// 새로운 노드를 생성하고, block_id를 할당하여 반환
	void insert_file(string inputfile);
	void insert(unsigned int k, unsigned int p);	// key와 value의 pair를 B+Tree에 삽입한다.
	void insert_in_pair(vector<pair<unsigned int, unsigned int>>& v, unsigned int k, unsigned int p);	// 노드 내에 new node를 삽입하고 오름차순 정렬한다.
	void insert_in_parent(unsigned int n, unsigned int k, unsigned int n_, stack<unsigned int>& parent);	// 부모 노드에서 삽입이 필요하거나, split이 일어나는지 체크, 재귀적으로 동작
	Node find_leafnode(unsigned int k, stack<unsigned int>& parent);
	// new node가 들어갈 leaf node의 위치를 찾아 반환 (parant stack에 leafnode를 찾으며 지나온 부모의 주소를 저장한다=>부모에서 split이 일어날 수 있기 때문)


	//(과제 이외) 전체 출력 확인
	void print();


	//print B+tree structure
	void print_struct(string outputfile);	// depth <0>, <1>의 노드를 모두 출력
	void print_node(const Node& node, ofstream& fout);	// 해당 node 내에 있는 element 값을 모두 출력


	//search
	void search_file(string inputfile, string outputfile);	//	exact search file을 불러옴
	void search(int key, ofstream& fout); // exact seach

	void search_file_range(string inputfile, string outputfile);	// range serch file을 불러옴
	void search(int start_range, int end_range, ofstream& fout); // range search

	Node& search_leaf(Node& node, int key, int now_depth);	// key에 해당하는 leaf node를 반환
	int get_value(const Node& node, int key);	 // node 내에서 key값에 대한 value를 반환


private:
	string file_name;
	unsigned int block_size = 0;
	unsigned int root_id = 0;//루트의 block 번호
	unsigned int depth = 0;	// 트리의 깊이

	unsigned int new_node_num = 0;

};

#endif