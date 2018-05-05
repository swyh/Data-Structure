Implementation of a Disk Based B+-Tree
1. Implementation
- B+Tree에서의 Disk를 기반으로 한 create, insert, exact search, range search, print를 구현하였다.

2. Explanation of my Implementation
struct node : node(block)의 BID를 저장하고, (page_size - 4) / 8개의 key를 저장할 수 있는 node vector, 실제 node를 저장한 갯수인 v_size가 존재한다. 또한 Non-leaf node에서는 NextLevelBID, leaf node에서는 NextBID로 사용되는 ptr 변수를 갖고 있다.
binary file에 입출력을 위한 함수
get_block : BID를 이용하여 binary file 내의 node의 위치를 찾아 node 구조체로 반환한다.
set_block : node 구조체에 담긴 정보를 binary file 내에 형식에 맞게 write 한다.
B+tree 함수
- create : binary file 생성하고 입력 받은 page_size와 RootBID(=0), Depth(=0)를 write한다.
- insert : insert text file 읽어서 한 줄씩 key, value값을 B+Tree 노드의 형식에 맞게 bin file에 write한다. 이 때 insert 함수를 호출한다. 이 함수에서는 root BID가 0이 아니라면, fine_leafnode 함수를 호출하여 해당 key 값이 들어갈 leaf node를 찾는다. 이 과정에서 지나간 parent node들은 parent stack에 쌓는다. leaf node가 가득 차지 않았다면, key, value를 삽입 후 함수는 종료된다.
삽입 되어야할 leaf node가 가득 찼을 경우, node를 복사한 t 배열에 key 값을 삽입 후 정렬한다. 그리고 split을 발생시킨다. t 배열의 n개의 key는 n/2의 올림을 기준으로 나눈다. (예를 들어, 5개의 노드가 있으면 왼쪽 node에 3개, 오른쪽 node에 2개)
이후, insert_in_parent 함수를 호출시켜 새로 생성된 node의 key값을 parent node에 삽입하는 작업을 한다. (이 때 parent node의 BID는 parent stack을 이용하여 찾을 수 있다) 만약 parent node가 가득 찼다면, split을 발생시킨다. 새로운 node가 생성되었기 때문에 parent node에 key값을 삽입해야 한다. 이는 재귀적으로 일어나므로 insert_in_parent 함수를 호출한다.
- exact search : search_file 함수를 통해 input.txt file을 읽는다. 각 줄에 있는 key 값을 읽어서 search 함수를 실행한다. 먼저 get_block 함수를 통해 root node를 불러들인다. search_leaf 함수를 통해서 root부터 key 값을 비교해가며, depth를 내려가면서 해당되는 leaf node를 찾는다. 찾은 leaf node에서 key 값을 찾고 그에 따른 value를 output file에 write한다.
- range search : serach_file_range 함수를 통해 input.txt file을 읽는다. 각 줄의 start, end 값을 받아 search 함수를 실행한다. start가 속할 수 있는 leaf node를 search_leaf 함수를 통해 찾는다. 찾은 leaf node에서 end key 값보다 작거나 NULL이 나올 때까지 next node를 탐색하며 value를 output file에 write 한다. 다음 node로 넘어갈때는 node 구조체 ptr 변수에 저장된 next nod BID를 참조한다.
- print : print_struct 함수를 이용하여 depth를 활용하여 depth가 1일 때까지의 key값을 모두 출력한다.

3. How to compile and run
Visual Studio를 이용하여 main.cpp를 컴파일 뒤 실행파일 생성 후 실행할 수 있다. 실행파일이 생성되면 cmd창에서 실행파일 경로(Debug 폴더 내)로 이동한다. 아래 명령어를 통해 동작한다.
- create : [실행파일] c [btree binary file] [block_size]
- insert : [실행파일] i [btree binary file] [records data text file]
- exact search : [실행파일] s [btree binary file] [input text file] [output text file]
- range search : [실행파일] r [btree binary file] [input text file] [output text file]
- print : [실행파일] p [btree binary file] [output text file]
* output file은 경로가 주어지지 않을 경우 실행파일과 같은 폴더 내에 생성된다.
