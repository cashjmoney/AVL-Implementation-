#include <iostream>
#include <string>
#include <sstream>
#include "AVL.h"
using namespace std;
/* Note: 
	1. You will have to comment main() when unit testing your code because catch uses its own main().
	2. You will submit this main.cpp file and any header files you have on Gradescope. 
*/
int num_commands;
string command_line = "";
string command;
bool initial = true;
AVLTree r;
vector<string> q;
int main()
{
	
	cin>> num_commands;
	std::cin.ignore();
	for(int i = 0; i < (num_commands); i++)
	{
		
			getline(cin,command);
			istringstream in(command);
			r.parse(in);	
		

	}
	return 0;
}
//Test 2 Done