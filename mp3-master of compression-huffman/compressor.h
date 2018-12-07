#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
#include <fstream>
#include <queue>
#include <string>

#include "minheap.h"

using namespace std;



class compressor
{
public:

	int char_count[256];
	vector<bool> bits_of_char[256];
	node* huffman_root;

	ifstream input_file_stream;
	ofstream output_file_stream;

	compressor() { fill(char_count, char_count+256, 0); };
	void compress(const string input_filename, const string output_filename);
	void construct_huffman_tree();
	void write_huffman_tree();
	void assign_bits_to_char();
	void dfs(node* curr, vector<bool>& bits);
	void write_bits(vector<bool>& bits_for_write);

};


void compressor::construct_huffman_tree() {
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

	huffman_root = mh.pop();
}


void compressor::dfs(node* curr, vector<bool>& bits) {
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

void compressor::assign_bits_to_char()
{
	vector<bool> bits;
	dfs(huffman_root, bits);
}

void compressor::write_huffman_tree() {

	int num_char = 0;

	for(int i = 0; i < 256; i++)
	{
		num_char += char_count[i] != 0;
	}

	output_file_stream << num_char;



	for(int i = 0; i < 256; i++)
	{
		if(char_count[i] != 0)
		{
			output_file_stream << char(i) << char_count[i];
		}
	}
}

void compressor::write_bits(vector<bool>& bits_for_write) {
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
		output_file_stream << to_write;
	}
}

void compressor::compress(const string input_filename, const string output_filename)
{
	input_file_stream.open(input_filename, ifstream::in);
	output_file_stream.open(output_filename, ifstream::out);

	construct_huffman_tree();
	write_huffman_tree();

	assign_bits_to_char();

	input_file_stream.close();
	input_file_stream.open(input_filename, ifstream::in);

	char c;
	vector<bool>bits_for_write;
	int num_bits = 0;
	while(input_file_stream.get(c))
	{
		for(auto i : bits_of_char[c])
		{
			bits_for_write.push_back(i);
			num_bits++;
		}
	}
	output_file_stream << num_bits;
	write_bits(bits_for_write);
}