#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
#include <fstream>
#include <queue>

#include "compressor.h"
#include "decompressor.h"
using namespace std;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	string input_filename = "a.txt";


	compressor my_compressor;
	my_compressor.compress(input_filename, "re_" + input_filename);

	decompressor my_decompressor;
	my_decompressor.decompress("re_" + input_filename, "re_"+("re_" + input_filename));

	return 0;
}