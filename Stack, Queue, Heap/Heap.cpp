#include <iostream>
using namespace std;

#define SIZE	100

class Heap {
private:
	int* arr;
	int max_size = 0;
	int last = 1;
public:

	Heap() {
		arr = new int[SIZE];
		max_size = SIZE;
	}

	Heap(int N) {
		arr = new int[N];
		max_size = N;
	}

	~Heap() {
		delete[] arr;
	}

	bool empty() {
		if (last == 1)
			return 1;
		else
			return 0;
	}

	bool full() {
		if (last == max_size)
			return 1;
		else
			return 0;
	}

	void insert(int n) {
		if (full())
			return;

		arr[last] = n;

		int i = last;

		while (arr[i / 2] > arr[i]) {	// Up heap
			int temp = arr[i];
			arr[i] = arr[i / 2];
			arr[i / 2] = temp;

			i /= 2;
		}

		last++;
	}

	void print() {
		for (int i = 1; i < last; i++) {
			cout << arr[i] << " ";
		}
		cout << endl;
	}


	int erase_min() {
		if (empty())
			return -1;

		int min_val = arr[1];

		arr[1] = arr[last - 1];
		last--;

		int i = 1;

		while (i * 2 < last) {
			int temp = arr[i];
			int left = i * 2;
			int right = i * 2 + 1;
			int min_idx;

			if (i * 2 + 1 >= last || arr[left] < arr[right])
				min_idx = left;
			else
				min_idx = right;

			if (arr[min_idx] < arr[i]) {
				arr[i] = arr[min_idx];
				arr[min_idx] = temp;
				i = min_idx;
			}
			else
				break;
		}

		return min_val;
	}
};

int main() {
	Heap heap;

	heap.insert(3);
	heap.insert(19);
	heap.insert(14);
	heap.insert(1);
	heap.insert(8);

	heap.print();

	while (!heap.empty()) {
		cout << heap.erase_min() << endl;
	}
}