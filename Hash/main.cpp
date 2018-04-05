#include <iostream>
#include <fstream>
#include <string>
using namespace std;

static ofstream outputFile("output.txt"); // 출력파일을 정적공간에 선언하여 어디서든 접근 가능하도록 하였다.
static int txtType = 0;					  // tytType이 command.txt이면 1로 바꾼다. (insert의 probe 횟수를 출력할 수 있도록 한다.)

struct Student {		// 학생의 정보를 담고 있는 구조체
	int studentNumber;	// 학번
	string name;		// 이름
	string department;	// 학과
	int grade;			// 학년
	bool switchBit;		// 기본값은 0이고, 존재했던 값이 삭제된 자리면 1로 표시한다.
};
class Hash {			// Hash의 기능이 담겨져 있는 클래스
private:
	int N, M, size;		// N = 배열의 크기를 나타내는 소수, M = 2차 헤시함수에 사용되는 소수, size = 배열 원소의 수
	int probe;			// 수행한 탐사 횟수
	Student* arr;		// 학생 정보가 담긴 배열
public:
	Hash() {			// 생성자, 0으로 초기화
		N = 0;
		M = 0;
		size = 0;
	}
	bool empty();																	// element가 비었을때 삭제할 수 없음을 나타냄
	bool sizeFull();																// 배열이 가득찼을때 추가할 수 없음을 나타냄

	bool arrSizeSet(int N, int M);													// 클래스의 N, M을 설정하고,배열의 크기를 N으로 바꾼다.(범위 초과 경우 1 반환)
	int h1(int k);																	// 1차 헤시함수 : 처음 인덱스 위치를 지정하는 함수
	int h2(int k);																	// 2차 헤시함수 : 처음 지정된 인덱스에 다른 값이 있을 경우 실행되는 함수

	void insert(int studentNumber, string name, string department, int grade);		// 삽입 함수
	bool limiteLength(int studentNumber, string name, string department, int grade);// 입력받은 학생정보가 초기조건과 안맞을 경우 1을 반환하는 함수
	int insertIndexSet(int index, int studentNumber);								// h2 함수를 통해 인덱스를 재설정해주는 함수

	void print(int studentNumber);												    // 출력 함수
	void printIndexSet(int index, int studentNumber);								// 학번의 정보를 출력하거나 h2 함수를 통해 인덱스를 재설정해주는 함수

	void remove(int number);														// 삭제 함수
	void removeIndexSet(int index, int studentNumber);								// 학번의 정보를 삭제하거나 h2 함수를 통해 인덱스를 재설정해주는 함수
};

bool Hash::empty() {    // element가 비었을때 삭제할 수 없음을 나타냄
	if (size == 0) {
		outputFile << "삭제할 수 없음" << probe << endl;
		return 1;
	}
}
bool Hash::sizeFull() {	// 배열이 가득찼을때 추가할 수 없음을 나타냄
	if (size == N) {
		outputFile << "추가할 수 없음 " << probe << endl;
		return 1;
	}
}

bool Hash::arrSizeSet(int N, int M) { // 클래스의 N, M을 설정해주는 함수, 범위 초과 경우 1 반환
	bool exception = 0;				  // 예외 여부를 나타내는 변수(문제가 있을 때 1)
	if (N > 1 || N <= 30000) {		  // N이 30000이하의 소수일 때 설정
		for (int i = 2; i < N; i++)	  // N이 소수인지 판별한다.
			if (N%i == 0) {
				outputFile << "N 소수가 아님" << endl;
				exception = 1;		  // 소수가 아닐 경우 exception에 1을 넣는다.
				break;
			}
		this->N = N;
	}
	else{
		outputFile << "N 범위 초과" << endl;
		exception = 1;					  // 범위를 초과하면 1반환
	}

	if (M > 2 || M <= 1000) {			  // M이 1000이하의 소수일 때 설정
		this->M = M;				
		for (int i = 2; i < M; i++)		  // M이 소수인지 판별한다.
			if (M%i == 0) {
				outputFile << "M 소수가 아님" << endl;
				exception = 1;			  // 소수가 아닐 경우 exception에 1을 넣는다.
				break;
			}
	}
	else {
		outputFile << "M 범위 초과" << endl;
		exception = 1;					  // 범위를 초과하면 1반환
	}

	if (exception == 1)					  // 예외가 있을 경우 1 반환
		return 1;

	arr = new Student[N];				  // 배열을 student 구조체를 가리키는 포인터형으로 동적할당한다.
	for (int i = 0; i != N; i++) {		  // 배열의 각 원소를 NULL값으로 초기화한다.
		arr[i] = { NULL };
	}

	return 0;
}
int Hash::h1(int k) { // 1차 헤시함수 : 처음 인덱스 위치를 지정하는 함수
	int index = k % N;
	return index;
}
int Hash::h2(int k) { // 2차 헤시함수 : 처음 지정된 인덱스에 다른 값이 있을 경우 실행되는 함수
	int index = (M - k % M);
	return index;
}

void Hash::insert(int studentNumber, string name, string department, int grade) {		// 삽입
	probe = 0;

	if (sizeFull() == 1 || limiteLength(studentNumber, name, department, grade) == 1) {}// 크기가 꽉차거나 학생 정보가 조건과 안맞을 경우 함수 종료
	else {
		int index = h1(studentNumber);						// 삽입할 인덱스를 h1 함수를 통해 설정
		index = insertIndexSet(index, studentNumber);		// 삽입할 인덱스에 이미 값이 있는지 확인하고, 있다면 인덱스 h2 함수를 통해 재설정
		if (index != -1) {									// 인덱스가 -1이 아닐 경우 입력받은 학생의 정보를 해당 인덱스에 삽입한다.
			arr[index].studentNumber = studentNumber;
			arr[index].name = name;
			arr[index].department = department;
			arr[index].grade = grade;
			arr[index].switchBit = 0;						// 삭제된 자리가 아니기 때문에 0으로 초기화 시켜준다.
			size++;											// 사이즈를 1 추가한다.
		}
	}

}
bool Hash::limiteLength(int studentNumber, string name, string department, int grade) { // 입력받은 값이 초기조건과 안맞을 경우 1을 반환하는 함수
	if (studentNumber < 10000000 || studentNumber >= 100000000 || name.length() > 20    // 학번 8자리, 이름 20byte, 학과 20bybe, 학년 1~4 정수가 아닐경우
		|| department.length() > 20 || grade < 1 || grade > 4) {
		outputFile << "추가할 수 없음 " << probe << endl;								// 추가할 수 없음을 나타내고
		return 1;																		// 1 반환
	}
}
int Hash::insertIndexSet(int index, int studentNumber) {	  // h2 함수를 통해 인덱스를 재설정해주는 함수
	int temp = -1;						   					  // 반환할 인덱스의 값을 저장한다.
	bool stay = 0;											  // stay가 1이 되면 temp의 값을 더 이상 바꾸지 못하게 해준다.
	while (1) {
		probe++;											  // 반복될때마다 탐사횟수(probe)를 1 증가시킨다.
		if (arr[index].studentNumber == NULL) {				  // 인덱스가 비었고 
			if (stay == 0)									  // stay가 0이면 
				temp = index;								  // temp를 해당 index 값으로 설정한다.

			if (arr[index].switchBit == 1) {				  // 해당 인덱스가 값이 삭제된 자리면 다음 인덱스에 중복되는 값이 있을 수 있으므로 확인해 줘야한다.
				stay = 1;									  // stay를 1로 지정하여 다음 인덱스가 NULL이여도 인덱스 값이 변하지 못하게 한다.
				index = (index + h2(studentNumber)) % N;	  // 인덱스 값에 h2(k) 함수로 결정된 값을 더해 다음 인덱스로 설정 후 반복한다.
			}
			else {											  // 해당 인덱스가 값이 삭제된 자리가 아니면
				if (txtType == 1)							  // txtType이 command.txt일때
					outputFile << probe << endl;		  	  // 탐사횟수를 출력한다.
				break;										  // 반복문을 빠져나간다.
			}
		}
		else if (arr[index].studentNumber == studentNumber) { // 입력받은 학번과 같은 학번이 인덱스에 존재할 때
			outputFile << "추가할 수 없음 " << probe << endl; // 추가할 수 없음을 나타내고
			temp = -1;										  // temp를 -1로 초기화하고
			break;											  // 반복문을 빠져나간다.
		}
		else {												  // 인덱스에 값이 있다면
			index = (index + h2(studentNumber)) % N;		  // 인덱스 값에 h2(k) 함수로 결정된 값을 더해 재설정
		}
	}
	return temp; //temp의 값을 반환한다.
}

void Hash::print(int studentNumber) {		// 출력
	probe = 1;
	int index = h1(studentNumber);			// 출력할 인덱스를 h1 함수를 통해 설정
	printIndexSet(index, studentNumber);	// 인덱스의 값과 학번의 값을 넘겨 함수 실행 
}
void Hash::printIndexSet(int index, int studentNumber) {									// 학번의 정보를 출력하거나 h2 함수를 통해 인덱스를 재설정해주는 함수
	if (arr[index].switchBit == 0 && arr[index].studentNumber == studentNumber)				// 삭제된 자리가 아니고, 인덱스의 학번과 입력받은 학번이 같을 때
		outputFile << arr[index].studentNumber << " " <<									// 인덱스의 학생 정보와 탐사 횟수를 출력한다.
		arr[index].name << " " << arr[index].department << " " <<
		arr[index].grade << " " << probe << endl;
	else {																					// 그 외의 경우
		probe++;																			// 탐사횟수를 1 증가시키고
		index = (index + h2(studentNumber)) % N;											// 인덱스 값에 h2(k) 함수로 결정된 값을 더해 재설정
		if (N == probe || arr[index].switchBit == 0 && arr[index].studentNumber == NULL) {  // 탐사횟수가 N번째이거나, 삭제된 자리가 아니면서 해당 인덱스의 학번이 비었을 경우
			outputFile << "없음 " << probe << endl;											// 찾는 학번이 없음을 나타내고 탐사 횟수를 출력한다.
		}
		else																				// 그 외의 경우 
			printIndexSet(index, studentNumber);											// 수정된 인덱스의 값을 파라미터로 받은 함수를 호출한다.(재귀함수)
	}
}

void Hash::remove(int studentNumber) {   // 삭제
	if (empty() == 1) {}				 // 배열이 비었을 경우 함수를 종료한다.
	else {
		probe = 1;

		int index = h1(studentNumber);  // 삭제할 인덱스를 h1 함수를 통해 설정
		removeIndexSet(index, studentNumber);
	}
}
void Hash::removeIndexSet(int index, int studentNumber) { // 학번의 정보를 삭제하거나 h2 함수를 통해 인덱스를 재설정해주는 함수
	if (arr[index].studentNumber == studentNumber) {	  // 인덱스의 한번과 학번이 일치하면
		arr[index] = { NULL };							  // 인덱스를 NULL값으로 초기화시킨다.
		arr[index].switchBit = 1;						  // 삭제된 자리임을 표시해준다.
		size--;											  // 배열의 원소 개수 -1
		outputFile << probe << endl;					  // 프로브 횟수 출력
	}
	else {																				     // 그 외의 경우
		probe++;																			 // 프로브 횟수 +1
		index = (index + h2(studentNumber)) % N;											 // 인덱스 값에 h2(k) 함수로 결정된 값을 더해 재설정
		if (N == probe || (arr[index].switchBit == 0 && arr[index].studentNumber == NULL)) { // 탐사횟수가 N번째이거나, 삭제된 자리가 아니면서 해당 인덱스의 학번이 비었을 경우
			outputFile << "삭제할 수 없음 ";												 // 찾는 학번이 없어 삭제할 수 없음을 나타내고 탐사 횟수를 출력한다.
			outputFile << probe << endl;
		}
		else {
			removeIndexSet(index, studentNumber); // 수정된 인덱스의 값을 파라미터로 받은 함수를 호출한다.(재귀함수)
		}
	}
}

int main() {
	int N, M;            // N = 배열의 크기를 나타내는 소수, M = 2차 헤시함수에 사용되는 소수
	bool arrExcess;		 // 배열의 크기가 정해진 범위를 넘었을 경우 실행을 중지해주도록 한다.

	int studentNumber;	 // 학번
	string name;		 // 이름
	string department;	 // 학과
	int grade;			 // 학년

	Hash hash;					   // 헤시클래스 생성
	string inputName, commandName; // inputName : 초기 입력 파일 이름, commandName : 질의 입력 파일 이름

	cout << "초기 입력 파일명 : ";
	cin >> inputName;
	cout << "질의 입력 파일명 : ";
	cin >> commandName;

	ifstream inputFile(inputName);  // 입력파일을 쓸 수 있도록 한다.
	int count = 0;

	if (!inputFile.is_open()) {		// 파일명과 일치하는 파일이 없을 경우
		cout << "파일이 존재하지 않습니다." << endl;
	}
	else {
		if (!inputFile.eof()) {
			count++;
			inputFile >> N >> M;
			arrExcess = hash.arrSizeSet(N, M);									// arrSizeSet 함수에서 범위를 초과했을 경우 1을 반환한다.
			if (arrExcess == 0){												// N, M이 범위를 초과하지 않았을 때만 실행
				while (!inputFile.eof()) {
					inputFile >> studentNumber >> name >> department >> grade;  // 초기입력파일의 학생 정보들을 받는다.
					hash.insert(studentNumber, name, department, grade);	    // insert 함수를 이용하여 헤시클래스에 학생의 정보를 넣는다.
			}
			}
		}
	}

	ifstream commandFile(commandName); // 입력파일을 쓸 수 있도록 한다.
	int count2 = 0;
	char kind;	// 입력 받을 기능의 종류

	if (!commandFile.is_open()) {
		cout << "파일이 존재하지 않습니다." << endl;
	}
	else {
		txtType = 1;	// txtType을 1로 바꿔줘 insert에서 탐사 횟수가 출력될 수 있게 한다.

		while (arrExcess == 0 && !commandFile.eof()) {
			count2++;
			commandFile >> kind;
			if (kind == 's') {		 // 탐색
				commandFile >> studentNumber;
				hash.print(studentNumber); // 해당 학번의 정보를 탐색한다.
			}
			else if (kind == 'i') {  // 추가
				commandFile >> studentNumber >> name >> department >> grade;
				hash.insert(studentNumber, name, department, grade); // 해당 학생의 정보를 삽입한다.
			}
			else if (kind == 'd') {  // 삭제
				commandFile >> studentNumber;
				hash.remove(studentNumber); // 해당 학생의 학번을 삭제한다.
			}
			else
				break;
		}
	}

	return 0;
}