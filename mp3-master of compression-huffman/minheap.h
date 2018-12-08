#pragma once

#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
#include <fstream>
#include <queue>
using namespace std;

class node {
public:
	unsigned char c;
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

using data_type = node * ;
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
	if (tree.size() < size + 1)
	{
		tree.resize(size + 1);
	}
	// ������ ũ�Ⱑ �ʿ�̸��̸� resize�Ѵ�

	tree[size] = new_element;
	// ������ ���ҿ� e�� ����

	int curr = size;
	// ���� �� ������ ��ġ
	while (curr != 1)
	{
		if (
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

	if (size == 0)
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
	while (curr * 2 <= size)
	{
		if (curr * 2 + 1 <= size && *tree[curr * 2 + 1] < *tree[curr * 2])
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
		if (*tree[next] < *tree[curr]) {
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