#include <iostream>
#include <windows.h>
#include <stdlib.h>
using namespace std;

int main() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("mode con: cols=100 lines=30");
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	
	cout << "\n\n\n";
	cout << " 	         	 ######   ######     ########     ###    ##    ## ##    ##                 \n";
	cout << " 	         	##    ## ##    ##    ##     ##   ## ##   ###   ## ##   ##                  \n";
	cout << " 	         	##       ##          ##     ##  ##   ##  ####  ## ##  ##                   \n";
	cout << " 	         	##        ######     ########  ##     ## ## ## ## #####                    \n";
	cout << " 	         	##             ##    ##     ## ######### ##  #### ##  ##                   \n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << " 	        	##    ## ##    ##    ##     ## ##     ## ##   ### ##   ##                  \n";
	cout << " 	        	 ######   ######     ########  ##     ## ##    ## ##    ##                 \n";
	cout << "\n\n\n\n";
	cout << "                                  CS제일은행에 오신것을 환영합니다!                                 \n";
	cout << "                                                                                                    \n";

}

