#include "STL.h"
#include "func.h"

void four_byte_read(ifstream& fin, unsigned int& ptr) {	// 파일로 부터 4byte의 값을 ptr에 가져옴
	uint32_t a = 0;
	fin.read(reinterpret_cast<char *>(&a), sizeof(a));
	ptr = a;
}

void four_byte_write(ofstream& fout, const unsigned int ptr) {	// 해당 주소로 부터 4byte의 값을 16진수로 표현하여 파일에 씀
	uint32_t a = ptr;
	fout.write(reinterpret_cast<char *>(&a), sizeof(a));
}

void create_file(string file_name, unsigned int page_size) {	// 명령어 c를 통해 bin file을 생성한다.
	ofstream fout(file_name, ios::binary);
	four_byte_write(fout, page_size);	// write page_size
	four_byte_write(fout, 0);	// write RootBID
	four_byte_write(fout, 0);	// write Depth
	fout.close();
}
