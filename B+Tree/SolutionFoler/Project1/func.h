#ifndef func_H
#define func_H

#include "STL.h"

void four_byte_read(ifstream& fin, unsigned int& ptr);	// file���� 4byte�� ���� �о� ptr�� ����
void four_byte_write(ofstream& fout, const unsigned int ptr);	// ptr�� 4byte�� file�� ����.
void create_file(string file_name, unsigned int page_size);	// file_name���� binary file�� �����ϰ�, page_size�� write�Ѵ�. (+ write RootID = 0, Depth = 0)

struct Node {	// Node�� ����
	unsigned int block_id = 0;	// block ID
	vector<pair<unsigned int, unsigned int>> v;	// non-leaf node : key/address(BID) leaf node : key/value
	int v_size = 0;	// v�� �����ϴ� node�� ����
	unsigned int ptr = 0;// Non-leaf node : NextLevelBID, leaf node : NextBID
};

#endif