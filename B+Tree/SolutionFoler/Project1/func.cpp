#include "STL.h"
#include "func.h"

void four_byte_read(ifstream& fin, unsigned int& ptr) {	// ���Ϸ� ���� 4byte�� ���� ptr�� ������
	uint32_t a = 0;
	fin.read(reinterpret_cast<char *>(&a), sizeof(a));
	ptr = a;
}

void four_byte_write(ofstream& fout, const unsigned int ptr) {	// �ش� �ּҷ� ���� 4byte�� ���� 16������ ǥ���Ͽ� ���Ͽ� ��
	uint32_t a = ptr;
	fout.write(reinterpret_cast<char *>(&a), sizeof(a));
}

void create_file(string file_name, unsigned int page_size) {	// ��ɾ� c�� ���� bin file�� �����Ѵ�.
	ofstream fout(file_name, ios::binary);
	four_byte_write(fout, page_size);	// write page_size
	four_byte_write(fout, 0);	// write RootBID
	four_byte_write(fout, 0);	// write Depth
	fout.close();
}
