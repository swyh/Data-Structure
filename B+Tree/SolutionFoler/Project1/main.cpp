#include <string>
#include "STL.h"
#include "func.h"
#include "Btree.h"

int main(int argc, char* argv[])
{
	if (argc == 1)
		return 0;

	char command = argv[1][0];
	//BTree myBtree = new BTree();
	BTree* btree;
	switch (command)
	{
	case 'c':
		// create index file
		create_file(argv[2], atoi(argv[3])); //[btree binary file] [page_size] 입력
		break;
	case 'i':
		btree = new BTree(argv[2]);	// 생성자 매개변수 : file name
		btree->insert_file(argv[3]);
		// insert records from [records data file], ex) records.txt
		//open_file(argv[1]);
		break;
	case 's':
		btree = new BTree(argv[2]);
		btree->search_file(argv[3], argv[4]);
		// search keys in [input file] and print results to [output file]
		break;
	case 'r':
		btree = new BTree(argv[2]);
		btree->search_file_range(argv[3], argv[4]);
		// search keys in [input file] and print results to [output file]
		break;
	case 'p':
		btree = new BTree(argv[2]);	// 생성자 매개변수 : file name
		btree->print_struct(argv[3]);
		// print B+-Tree structure to [output file]
		break;
	//case 'z':	// B+tree 전체구조 print 
	//	btree = new BTree(argv[2]);	// 생성자 매개변수 : file name
	//	btree->print();
	//	break;
	}
}
