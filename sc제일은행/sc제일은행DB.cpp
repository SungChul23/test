#include<iostream>
#include<fstream>

using namespace std;

int main()
{
	ofstream fout;
	fout.open("file.txt");
	fout << 1 << ' '
		<< 2 << "\t"
		<< 3 << "\n"
		<< 4;






}