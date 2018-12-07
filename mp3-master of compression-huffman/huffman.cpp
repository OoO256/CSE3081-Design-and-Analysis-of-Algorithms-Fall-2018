#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
#include <fstream>
#include <queue>

#include "compressor.h"
using namespace std;

int main() {
	
	compressor my_compressor;
	my_compressor.compress("test.txt", "re_test.txt");

	return 0;
}
