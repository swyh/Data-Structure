# Implementation of a Disk Based B+-Tree
## Implementation
- create : binary file 생성
- insert : key, value가 포함된 insert file을 받아 B+Tree 구조로 저장
- exact search : 입력한 key에 대한 value를 검색 후 출력
- range search : 입력한 key 범위에 대해 해당되는 key, value 출력
- print : B+Tree의 depth가 0 ~ 1의 node를 모두 출력

## How to compile and run
Visual Studio를 이용하여 main.cpp를 컴파일 뒤 실행파일 생성 후 실행할 수 있다. 실행파일이 생성되면 cmd창에서 실행파일 경로(Debug 폴더 내)로 이동한다. 아래 명령어를 통해 동작한다.
- create : [실행파일] c [btree binary file] [block_size]
- insert : [실행파일] i [btree binary file] [records data text file]
- exact search : [실행파일] s [btree binary file] [input text file] [output text file]
- range search : [실행파일] r [btree binary file] [input text file] [output text file]
- print : [실행파일] p [btree binary file] [output text file]
* output file은 경로가 주어지지 않을 경우 실행파일과 같은 폴더 내에 생성된다.
