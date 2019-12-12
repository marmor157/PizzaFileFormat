#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{

	vector<string> msg{"Hello", "C++", "World", "as", "VS Code!"};

	for (const string &word : msg)
	{
		cout << word << " ";
	}
	cout << endl;
}