#pragma once
#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
#include <fstream>
#include <queue>
#include <string>
#include <cmath>

#include "minheap.h"

using namespace std;

class decompressor
{
public:
	vector<bool> bits_of_char[256];
	node* huffman_root;

	ifstream input_file_stream;
	ofstream output_file_stream;

	decompressor() = default;
	void decompress(const string& input_filename, const string& output_filename);
	void construct_huffman_tree();
	void assign_bits_to_char();
	void dfs(node* curr, vector<bool>& bits);
	void write_bits(const vector<bool>& bits_for_write);
	
};

inline void decompressor::construct_huffman_tree()
{
	int num_char = 0;
	input_file_stream >> num_char;
	input_file_stream.get();

	min_heap mh(0);
	char c;
	int fre;
	for (int i = 0; i < num_char; i++)
	{
		input_file_stream.get(c);
		input_file_stream.get();
		input_file_stream >> fre;
		input_file_stream.get();
		mh.push(new node(c, fre, nullptr, nullptr));
	}

	while (mh.size >= 2)
	{
		node* left = mh.pop();
		node* right = mh.pop();

		node* parent = new node(0, left->count + right->count, left, right);
		mh.push(parent);
	}

	huffman_root = mh.pop();

	if (huffman_root->left == nullptr && huffman_root->right == nullptr)
	{
		// 단일 글자
		huffman_root = new node(0, huffman_root->count, huffman_root, nullptr);
	}
}


inline void decompressor::dfs(node* curr, vector<bool>& bits) {
	if (curr->right == nullptr && curr->left == nullptr)
	{
		bits_of_char[curr->c] = bits;
		return;
	}

	if (curr->left != nullptr)
	{
		bits.push_back(0);
		dfs(curr->left, bits);
		bits.pop_back();
	}
	if (curr->right != nullptr)
	{
		bits.push_back(1);
		dfs(curr->right, bits);
		bits.pop_back();
	}
}

inline void decompressor::assign_bits_to_char()
{
	vector<bool> bits;
	dfs(huffman_root, bits);
}

inline void decompressor::write_bits(const vector<bool>& bits_for_write)
{
	node* curr = huffman_root;
	for (int i=0; i<bits_for_write.size(); i++)
	{
		if (bits_for_write[i] == false)
		{
			curr = curr->left;
		}
		else
		{
			curr = curr->right;
		}

		if (curr->left == nullptr && curr->right == nullptr)
		{
			output_file_stream << curr->c;
			curr = huffman_root;
		}
	}
}



inline void decompressor::decompress(const string& input_filename, const string& output_filename)
{
	input_file_stream.open(input_filename, ifstream::binary);
	output_file_stream.open(output_filename, ifstream::binary);

	construct_huffman_tree();
	assign_bits_to_char();


	vector<bool>bits_to_read;
	int num_bits;
	char c;
	input_file_stream >> num_bits;
	input_file_stream.get(c);

	for (int i = 0; i < ceil((double)num_bits/8);i++)
	{
		input_file_stream.get(c);

		unsigned char uc = c;
		for (int j = 0; j < 8 ; j++)
		{
			bits_to_read.push_back(uc / 128);
			uc <<= 1;
		}
	}

	while (bits_to_read.size() != num_bits)
	{
		bits_to_read.pop_back();
	}

	write_bits(bits_to_read);

	input_file_stream.close();
	output_file_stream.close();
}