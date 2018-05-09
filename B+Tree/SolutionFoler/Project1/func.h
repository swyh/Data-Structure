#ifndef func_H
#define func_H

#include "STL.h"

void four_byte_read(ifstream& fin, unsigned int& ptr);	// file에서 4byte의 값을 읽어 ptr에 담음
void four_byte_write(ofstream& fout, const unsigned int ptr);	// ptr의 4byte를 file에 쓴다.
void create_file(string file_name, unsigned int page_size);	// file_name으로 binary file을 생성하고, page_size를 write한다. (+ write RootID = 0, Depth = 0)

struct Node {	// Node의 구조
	unsigned int block_id = 0;	// block ID
	vector<pair<unsigned int, unsigned int>> v;	// non-leaf node : key/address(BID) leaf node : key/value
	int v_size = 0;	// v에 존재하는 node의 개수
	unsigned int ptr = 0;// Non-leaf node : NextLevelBID, leaf node : NextBID
};

#endif