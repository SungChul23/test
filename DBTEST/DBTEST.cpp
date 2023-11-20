#include<iostream>
#include<fstream>

using namespace std;

int main()
{
	ofstream fout;
	fout.open("file.txt");
	fout <<8<<2<<1;

	ifstream fin;
	fin.open("file.txt");
	int number[3];
	fin >> number[0]>> number[1]>> number[2];

	for (int i = 0; i < 4; i++)
	{
		cout << number[i] << endl;
	}

}