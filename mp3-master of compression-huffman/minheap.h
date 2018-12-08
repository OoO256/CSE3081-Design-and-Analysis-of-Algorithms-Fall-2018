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
	// min heap 을 구현할 클래스

public:
	int size;
	// heap의 원소 개수
	vector<data_type>tree;
	// min heap을 구현한 트리
	// 1부터 시작하고, i의 자식은 2i, 2i+1로 참조한다
	min_heap() : size(0) {};
	//기본생성자
	min_heap(const int size) : tree(size + 1), size(size) {};
	//크기가 주어진 생성자

	void push(const data_type e);
	data_type pop();

};

void min_heap::push(const data_type new_element) {
	// 트리에 원소를 추가하는 함수
	size++;
	if (tree.size() < size + 1)
	{
		tree.resize(size + 1);
	}
	// 벡터의 크기가 필요미만이면 resize한다

	tree[size] = new_element;
	// 마지막 원소에 e를 저장

	int curr = size;
	// 현제 새 원소의 위치
	while (curr != 1)
	{
		if (
			*tree[curr] < *tree[curr / 2])
			swap(tree[curr / 2], tree[curr]);
		else
			break;

		curr /= 2;
	}
	// 더 큰 부모가 없을 때까지 부모와 자식을 swap한다
}

data_type min_heap::pop() {
	// min heap에서 원소를 pop하는 함수

	if (size == 0)
		return nullptr;
	// 빈 트리인경우, 0을 출력한다

	const data_type top = tree[1];
	tree[1] = tree[size];
	size--;
	// root의 원소를 저장하고, 이를 마지막원소로 대체한다


	int curr = 1;
	// 현재 마지막 원소였던 원소가 있는 위치
	int next = curr;
	// 마지막 원소였던 원소가 다음으로 향할 위치
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

		// next는 curr의 두 자식 중 최소를 가리킨다.
		// next가 curr 보다 작으면 둘을 swap한다
		// 그렇지 않으면 반복문을 종료한다.
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