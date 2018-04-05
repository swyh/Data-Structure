#include <iostream>
#include <fstream>
#include <string>
using namespace std;

static ofstream outputFile("output.txt"); // ��������� ���������� �����Ͽ� ��𼭵� ���� �����ϵ��� �Ͽ���.
static int txtType = 0;					  // tytType�� command.txt�̸� 1�� �ٲ۴�. (insert�� probe Ƚ���� ����� �� �ֵ��� �Ѵ�.)

struct Student {		// �л��� ������ ��� �ִ� ����ü
	int studentNumber;	// �й�
	string name;		// �̸�
	string department;	// �а�
	int grade;			// �г�
	bool switchBit;		// �⺻���� 0�̰�, �����ߴ� ���� ������ �ڸ��� 1�� ǥ���Ѵ�.
};
class Hash {			// Hash�� ����� ����� �ִ� Ŭ����
private:
	int N, M, size;		// N = �迭�� ũ�⸦ ��Ÿ���� �Ҽ�, M = 2�� ����Լ��� ���Ǵ� �Ҽ�, size = �迭 ������ ��
	int probe;			// ������ Ž�� Ƚ��
	Student* arr;		// �л� ������ ��� �迭
public:
	Hash() {			// ������, 0���� �ʱ�ȭ
		N = 0;
		M = 0;
		size = 0;
	}
	bool empty();																	// element�� ������� ������ �� ������ ��Ÿ��
	bool sizeFull();																// �迭�� ����á���� �߰��� �� ������ ��Ÿ��

	bool arrSizeSet(int N, int M);													// Ŭ������ N, M�� �����ϰ�,�迭�� ũ�⸦ N���� �ٲ۴�.(���� �ʰ� ��� 1 ��ȯ)
	int h1(int k);																	// 1�� ����Լ� : ó�� �ε��� ��ġ�� �����ϴ� �Լ�
	int h2(int k);																	// 2�� ����Լ� : ó�� ������ �ε����� �ٸ� ���� ���� ��� ����Ǵ� �Լ�

	void insert(int studentNumber, string name, string department, int grade);		// ���� �Լ�
	bool limiteLength(int studentNumber, string name, string department, int grade);// �Է¹��� �л������� �ʱ����ǰ� �ȸ��� ��� 1�� ��ȯ�ϴ� �Լ�
	int insertIndexSet(int index, int studentNumber);								// h2 �Լ��� ���� �ε����� �缳�����ִ� �Լ�

	void print(int studentNumber);												    // ��� �Լ�
	void printIndexSet(int index, int studentNumber);								// �й��� ������ ����ϰų� h2 �Լ��� ���� �ε����� �缳�����ִ� �Լ�

	void remove(int number);														// ���� �Լ�
	void removeIndexSet(int index, int studentNumber);								// �й��� ������ �����ϰų� h2 �Լ��� ���� �ε����� �缳�����ִ� �Լ�
};

bool Hash::empty() {    // element�� ������� ������ �� ������ ��Ÿ��
	if (size == 0) {
		outputFile << "������ �� ����" << probe << endl;
		return 1;
	}
}
bool Hash::sizeFull() {	// �迭�� ����á���� �߰��� �� ������ ��Ÿ��
	if (size == N) {
		outputFile << "�߰��� �� ���� " << probe << endl;
		return 1;
	}
}

bool Hash::arrSizeSet(int N, int M) { // Ŭ������ N, M�� �������ִ� �Լ�, ���� �ʰ� ��� 1 ��ȯ
	bool exception = 0;				  // ���� ���θ� ��Ÿ���� ����(������ ���� �� 1)
	if (N > 1 || N <= 30000) {		  // N�� 30000������ �Ҽ��� �� ����
		for (int i = 2; i < N; i++)	  // N�� �Ҽ����� �Ǻ��Ѵ�.
			if (N%i == 0) {
				outputFile << "N �Ҽ��� �ƴ�" << endl;
				exception = 1;		  // �Ҽ��� �ƴ� ��� exception�� 1�� �ִ´�.
				break;
			}
		this->N = N;
	}
	else{
		outputFile << "N ���� �ʰ�" << endl;
		exception = 1;					  // ������ �ʰ��ϸ� 1��ȯ
	}

	if (M > 2 || M <= 1000) {			  // M�� 1000������ �Ҽ��� �� ����
		this->M = M;				
		for (int i = 2; i < M; i++)		  // M�� �Ҽ����� �Ǻ��Ѵ�.
			if (M%i == 0) {
				outputFile << "M �Ҽ��� �ƴ�" << endl;
				exception = 1;			  // �Ҽ��� �ƴ� ��� exception�� 1�� �ִ´�.
				break;
			}
	}
	else {
		outputFile << "M ���� �ʰ�" << endl;
		exception = 1;					  // ������ �ʰ��ϸ� 1��ȯ
	}

	if (exception == 1)					  // ���ܰ� ���� ��� 1 ��ȯ
		return 1;

	arr = new Student[N];				  // �迭�� student ����ü�� ����Ű�� ������������ �����Ҵ��Ѵ�.
	for (int i = 0; i != N; i++) {		  // �迭�� �� ���Ҹ� NULL������ �ʱ�ȭ�Ѵ�.
		arr[i] = { NULL };
	}

	return 0;
}
int Hash::h1(int k) { // 1�� ����Լ� : ó�� �ε��� ��ġ�� �����ϴ� �Լ�
	int index = k % N;
	return index;
}
int Hash::h2(int k) { // 2�� ����Լ� : ó�� ������ �ε����� �ٸ� ���� ���� ��� ����Ǵ� �Լ�
	int index = (M - k % M);
	return index;
}

void Hash::insert(int studentNumber, string name, string department, int grade) {		// ����
	probe = 0;

	if (sizeFull() == 1 || limiteLength(studentNumber, name, department, grade) == 1) {}// ũ�Ⱑ �����ų� �л� ������ ���ǰ� �ȸ��� ��� �Լ� ����
	else {
		int index = h1(studentNumber);						// ������ �ε����� h1 �Լ��� ���� ����
		index = insertIndexSet(index, studentNumber);		// ������ �ε����� �̹� ���� �ִ��� Ȯ���ϰ�, �ִٸ� �ε��� h2 �Լ��� ���� �缳��
		if (index != -1) {									// �ε����� -1�� �ƴ� ��� �Է¹��� �л��� ������ �ش� �ε����� �����Ѵ�.
			arr[index].studentNumber = studentNumber;
			arr[index].name = name;
			arr[index].department = department;
			arr[index].grade = grade;
			arr[index].switchBit = 0;						// ������ �ڸ��� �ƴϱ� ������ 0���� �ʱ�ȭ �����ش�.
			size++;											// ����� 1 �߰��Ѵ�.
		}
	}

}
bool Hash::limiteLength(int studentNumber, string name, string department, int grade) { // �Է¹��� ���� �ʱ����ǰ� �ȸ��� ��� 1�� ��ȯ�ϴ� �Լ�
	if (studentNumber < 10000000 || studentNumber >= 100000000 || name.length() > 20    // �й� 8�ڸ�, �̸� 20byte, �а� 20bybe, �г� 1~4 ������ �ƴҰ��
		|| department.length() > 20 || grade < 1 || grade > 4) {
		outputFile << "�߰��� �� ���� " << probe << endl;								// �߰��� �� ������ ��Ÿ����
		return 1;																		// 1 ��ȯ
	}
}
int Hash::insertIndexSet(int index, int studentNumber) {	  // h2 �Լ��� ���� �ε����� �缳�����ִ� �Լ�
	int temp = -1;						   					  // ��ȯ�� �ε����� ���� �����Ѵ�.
	bool stay = 0;											  // stay�� 1�� �Ǹ� temp�� ���� �� �̻� �ٲ��� ���ϰ� ���ش�.
	while (1) {
		probe++;											  // �ݺ��ɶ����� Ž��Ƚ��(probe)�� 1 ������Ų��.
		if (arr[index].studentNumber == NULL) {				  // �ε����� ����� 
			if (stay == 0)									  // stay�� 0�̸� 
				temp = index;								  // temp�� �ش� index ������ �����Ѵ�.

			if (arr[index].switchBit == 1) {				  // �ش� �ε����� ���� ������ �ڸ��� ���� �ε����� �ߺ��Ǵ� ���� ���� �� �����Ƿ� Ȯ���� ����Ѵ�.
				stay = 1;									  // stay�� 1�� �����Ͽ� ���� �ε����� NULL�̿��� �ε��� ���� ������ ���ϰ� �Ѵ�.
				index = (index + h2(studentNumber)) % N;	  // �ε��� ���� h2(k) �Լ��� ������ ���� ���� ���� �ε����� ���� �� �ݺ��Ѵ�.
			}
			else {											  // �ش� �ε����� ���� ������ �ڸ��� �ƴϸ�
				if (txtType == 1)							  // txtType�� command.txt�϶�
					outputFile << probe << endl;		  	  // Ž��Ƚ���� ����Ѵ�.
				break;										  // �ݺ����� ����������.
			}
		}
		else if (arr[index].studentNumber == studentNumber) { // �Է¹��� �й��� ���� �й��� �ε����� ������ ��
			outputFile << "�߰��� �� ���� " << probe << endl; // �߰��� �� ������ ��Ÿ����
			temp = -1;										  // temp�� -1�� �ʱ�ȭ�ϰ�
			break;											  // �ݺ����� ����������.
		}
		else {												  // �ε����� ���� �ִٸ�
			index = (index + h2(studentNumber)) % N;		  // �ε��� ���� h2(k) �Լ��� ������ ���� ���� �缳��
		}
	}
	return temp; //temp�� ���� ��ȯ�Ѵ�.
}

void Hash::print(int studentNumber) {		// ���
	probe = 1;
	int index = h1(studentNumber);			// ����� �ε����� h1 �Լ��� ���� ����
	printIndexSet(index, studentNumber);	// �ε����� ���� �й��� ���� �Ѱ� �Լ� ���� 
}
void Hash::printIndexSet(int index, int studentNumber) {									// �й��� ������ ����ϰų� h2 �Լ��� ���� �ε����� �缳�����ִ� �Լ�
	if (arr[index].switchBit == 0 && arr[index].studentNumber == studentNumber)				// ������ �ڸ��� �ƴϰ�, �ε����� �й��� �Է¹��� �й��� ���� ��
		outputFile << arr[index].studentNumber << " " <<									// �ε����� �л� ������ Ž�� Ƚ���� ����Ѵ�.
		arr[index].name << " " << arr[index].department << " " <<
		arr[index].grade << " " << probe << endl;
	else {																					// �� ���� ���
		probe++;																			// Ž��Ƚ���� 1 ������Ű��
		index = (index + h2(studentNumber)) % N;											// �ε��� ���� h2(k) �Լ��� ������ ���� ���� �缳��
		if (N == probe || arr[index].switchBit == 0 && arr[index].studentNumber == NULL) {  // Ž��Ƚ���� N��°�̰ų�, ������ �ڸ��� �ƴϸ鼭 �ش� �ε����� �й��� ����� ���
			outputFile << "���� " << probe << endl;											// ã�� �й��� ������ ��Ÿ���� Ž�� Ƚ���� ����Ѵ�.
		}
		else																				// �� ���� ��� 
			printIndexSet(index, studentNumber);											// ������ �ε����� ���� �Ķ���ͷ� ���� �Լ��� ȣ���Ѵ�.(����Լ�)
	}
}

void Hash::remove(int studentNumber) {   // ����
	if (empty() == 1) {}				 // �迭�� ����� ��� �Լ��� �����Ѵ�.
	else {
		probe = 1;

		int index = h1(studentNumber);  // ������ �ε����� h1 �Լ��� ���� ����
		removeIndexSet(index, studentNumber);
	}
}
void Hash::removeIndexSet(int index, int studentNumber) { // �й��� ������ �����ϰų� h2 �Լ��� ���� �ε����� �缳�����ִ� �Լ�
	if (arr[index].studentNumber == studentNumber) {	  // �ε����� �ѹ��� �й��� ��ġ�ϸ�
		arr[index] = { NULL };							  // �ε����� NULL������ �ʱ�ȭ��Ų��.
		arr[index].switchBit = 1;						  // ������ �ڸ����� ǥ�����ش�.
		size--;											  // �迭�� ���� ���� -1
		outputFile << probe << endl;					  // ���κ� Ƚ�� ���
	}
	else {																				     // �� ���� ���
		probe++;																			 // ���κ� Ƚ�� +1
		index = (index + h2(studentNumber)) % N;											 // �ε��� ���� h2(k) �Լ��� ������ ���� ���� �缳��
		if (N == probe || (arr[index].switchBit == 0 && arr[index].studentNumber == NULL)) { // Ž��Ƚ���� N��°�̰ų�, ������ �ڸ��� �ƴϸ鼭 �ش� �ε����� �й��� ����� ���
			outputFile << "������ �� ���� ";												 // ã�� �й��� ���� ������ �� ������ ��Ÿ���� Ž�� Ƚ���� ����Ѵ�.
			outputFile << probe << endl;
		}
		else {
			removeIndexSet(index, studentNumber); // ������ �ε����� ���� �Ķ���ͷ� ���� �Լ��� ȣ���Ѵ�.(����Լ�)
		}
	}
}

int main() {
	int N, M;            // N = �迭�� ũ�⸦ ��Ÿ���� �Ҽ�, M = 2�� ����Լ��� ���Ǵ� �Ҽ�
	bool arrExcess;		 // �迭�� ũ�Ⱑ ������ ������ �Ѿ��� ��� ������ �������ֵ��� �Ѵ�.

	int studentNumber;	 // �й�
	string name;		 // �̸�
	string department;	 // �а�
	int grade;			 // �г�

	Hash hash;					   // ���Ŭ���� ����
	string inputName, commandName; // inputName : �ʱ� �Է� ���� �̸�, commandName : ���� �Է� ���� �̸�

	cout << "�ʱ� �Է� ���ϸ� : ";
	cin >> inputName;
	cout << "���� �Է� ���ϸ� : ";
	cin >> commandName;

	ifstream inputFile(inputName);  // �Է������� �� �� �ֵ��� �Ѵ�.
	int count = 0;

	if (!inputFile.is_open()) {		// ���ϸ�� ��ġ�ϴ� ������ ���� ���
		cout << "������ �������� �ʽ��ϴ�." << endl;
	}
	else {
		if (!inputFile.eof()) {
			count++;
			inputFile >> N >> M;
			arrExcess = hash.arrSizeSet(N, M);									// arrSizeSet �Լ����� ������ �ʰ����� ��� 1�� ��ȯ�Ѵ�.
			if (arrExcess == 0){												// N, M�� ������ �ʰ����� �ʾ��� ���� ����
				while (!inputFile.eof()) {
					inputFile >> studentNumber >> name >> department >> grade;  // �ʱ��Է������� �л� �������� �޴´�.
					hash.insert(studentNumber, name, department, grade);	    // insert �Լ��� �̿��Ͽ� ���Ŭ������ �л��� ������ �ִ´�.
			}
			}
		}
	}

	ifstream commandFile(commandName); // �Է������� �� �� �ֵ��� �Ѵ�.
	int count2 = 0;
	char kind;	// �Է� ���� ����� ����

	if (!commandFile.is_open()) {
		cout << "������ �������� �ʽ��ϴ�." << endl;
	}
	else {
		txtType = 1;	// txtType�� 1�� �ٲ��� insert���� Ž�� Ƚ���� ��µ� �� �ְ� �Ѵ�.

		while (arrExcess == 0 && !commandFile.eof()) {
			count2++;
			commandFile >> kind;
			if (kind == 's') {		 // Ž��
				commandFile >> studentNumber;
				hash.print(studentNumber); // �ش� �й��� ������ Ž���Ѵ�.
			}
			else if (kind == 'i') {  // �߰�
				commandFile >> studentNumber >> name >> department >> grade;
				hash.insert(studentNumber, name, department, grade); // �ش� �л��� ������ �����Ѵ�.
			}
			else if (kind == 'd') {  // ����
				commandFile >> studentNumber;
				hash.remove(studentNumber); // �ش� �л��� �й��� �����Ѵ�.
			}
			else
				break;
		}
	}

	return 0;
}