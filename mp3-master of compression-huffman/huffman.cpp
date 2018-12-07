#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
#include <fstream>
#include <queue>
using namespace std;

class node {
public:
	char c;
	int count;
	node* left;
	node* right;

	node() = default;
	node(const char c, const int count, node* l, node* r) : c(c), count(count), left(l), right(r) {};

	bool operator < (const node& rhs) const {
		if (this->count != rhs.count) 
			return this->count < rhs.count;
		else
			return this->c < rhs.c;
	}
};

using data_type = node*;
class min_heap
{
	// min heap �� ������ Ŭ����

public:
	int size;
	// heap�� ���� ����
	vector<data_type>tree;
	// min heap�� ������ Ʈ��
	// 1���� �����ϰ�, i�� �ڽ��� 2i, 2i+1�� �����Ѵ�
	min_heap() : size(0) {};
	//�⺻������
	min_heap(const int size) : tree(size + 1), size(size) {};
	//ũ�Ⱑ �־��� ������

	void push(const data_type e);
	data_type pop();

};

void min_heap::push(const data_type new_element) {
	// Ʈ���� ���Ҹ� �߰��ϴ� �Լ�
	size++;
	if(tree.size() < size + 1)
	{
		tree.resize(size + 1);
	}
	// ������ ũ�Ⱑ �ʿ�̸��̸� resize�Ѵ�

	tree[size] = new_element;
	// ������ ���ҿ� e�� ����

	int curr = size;
	// ���� �� ������ ��ġ
	while(curr != 1)
	{
		if(
			*tree[curr] < *tree[curr / 2])
			swap(tree[curr / 2], tree[curr]);
		else
			break;

		curr /= 2;
	}
	// �� ū �θ� ���� ������ �θ�� �ڽ��� swap�Ѵ�
}

data_type min_heap::pop() {
	// min heap���� ���Ҹ� pop�ϴ� �Լ�

	if(size == 0)
		return nullptr;
	// �� Ʈ���ΰ��, 0�� ����Ѵ�

	const data_type top = tree[1];
	tree[1] = tree[size];
	size--;
	// root�� ���Ҹ� �����ϰ�, �̸� ���������ҷ� ��ü�Ѵ�


	int curr = 1;
	// ���� ������ ���ҿ��� ���Ұ� �ִ� ��ġ
	int next = curr;
	// ������ ���ҿ��� ���Ұ� �������� ���� ��ġ
	while(curr * 2 <= size)
	{
		if(curr * 2 + 1 <= size && *tree[curr * 2 + 1] < *tree[curr * 2])
		{
			next = curr * 2 + 1;
		}
		else
		{
			next = curr * 2;
		}

		// next�� curr�� �� �ڽ� �� �ּҸ� ����Ų��.
		// next�� curr ���� ������ ���� swap�Ѵ�
		// �׷��� ������ �ݺ����� �����Ѵ�.
		if(*tree[next] < *tree[curr]) {
			swap(tree[curr], tree[next]);
			curr = next;
		}
		else
		{
			break;
		}

	}
	return top;
}




int char_count[256];
vector<bool> bits_of_char[256];
node* root;

void dfs(node* curr, vector<bool>& bits) {
	if(curr->right == nullptr && curr->left == nullptr)
	{
		bits_of_char[curr->c] = bits;
		return;
	}

	if(curr->left != nullptr)
	{
		bits.push_back(0);
		dfs(curr->left, bits);
		bits.pop_back();
	}
	if(curr->right != nullptr)
	{
		bits.push_back(1);
		dfs(curr->right, bits);
		bits.pop_back();
	}
}

void push_huffman_tree(FILE * output_file_stream, int char_count[256]) {

	int num_char = 0;

	for(int i = 0; i < 256; i++)
	{
		num_char += char_count[i] != 0;
	}

	fprintf(output_file_stream, "%d", num_char);



	for(int i = 0; i < 256; i++)
	{
		if(char_count[i] != 0)
		{
			fprintf(output_file_stream, "%c", i);
			fprintf(output_file_stream, "%d", char_count[i]);
		}

	}
}

void write_bits(FILE *output_file_stream, vector<bool> bits_for_write) {
	while(bits_for_write.size() % 8 != 0)
		bits_for_write.push_back(0);


	for(int i = 0; i + 7 < bits_for_write.size(); i+= 8)
	{
		unsigned char to_write = 0;
		for(int j = 0; j < 8; j++)
		{
			to_write <<= 1;
			to_write |= bits_for_write[i+j];
		}
		fprintf(output_file_stream, "%c", to_write);
	}
}


int main() {
	ifstream input_file_stream("test.txt", ifstream::in);
	FILE* output_file_stream = fopen("re_test.txt", "w");

	char c;
	while(input_file_stream.get(c))
	{
		char_count[c]++;
	}


	min_heap mh(0);
	for(int i = 0; i < 256; i++)
	{
		if(char_count[i] != 0)
			mh.push(new node(i, char_count[i], nullptr, nullptr));
	}

	while(mh.size >= 2)
	{
		node* left = mh.pop();
		node* right = mh.pop();

		node* parent = new node(0, left->count + right->count, left, right);
		mh.push(parent);
	}

	root = mh.pop();

	vector<bool> bits;
	dfs(root, bits);

	input_file_stream.close();

	input_file_stream.open("test.txt", ifstream::in);

	vector<bool>bits_for_write;

	push_huffman_tree(output_file_stream, char_count);


	int num_bits = 0;
	while(input_file_stream.get(c))
	{
		for(auto i : bits_of_char[c])
		{
			bits_for_write.push_back(i);
			num_bits++;
		}
	}

	fprintf(output_file_stream, "%d", num_bits);

	write_bits(output_file_stream, bits_for_write);

	return 0;
}
