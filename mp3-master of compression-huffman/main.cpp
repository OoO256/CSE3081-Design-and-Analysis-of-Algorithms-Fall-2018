#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
#include <fstream>
#include <queue>
#include <cstring>

#include "compressor.h"
#include "decompressor.h"
using namespace std;

int main(int argc, char **argv) {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	string input_filename = argv[2];

	if (!strcmp(argv[1], "-c"))
	{
		compressor my_compressor;
		my_compressor.compress(input_filename, input_filename + ".zz");
	}
	else if (!strcmp(argv[1], "-d"))
	{
		decompressor my_decompressor;
		my_decompressor.decompress(input_filename, input_filename + ".yy");
	}
	else
	{
		cout << "Not Suported Option!\n";
	}


	return 0;
}