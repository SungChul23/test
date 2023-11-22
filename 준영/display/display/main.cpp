#include <iostream>
#include <windows.h>
#include <stdlib.h>
using namespace std;

class Login {
public:
	string Input_ID, Input_PW;
	int missing = 0, error = 0;
	int MemberNo = 0;
protected:
	void login() {}
};


int main() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("mode con: cols=100 lines=35");
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
	SetConsoleTextAttribute(hConsole, 15);
	cout << "                                  CS제일은행에 오신것을 환영합니다!                                 \n\n\n\n";
	cout << "                                 □□□□□□□□□□□□□□□□□□\n";
	cout << "                                 □                                □\n";
	cout << "                                 □        ";
	SetConsoleTextAttribute(hConsole, 12);
	cout << "기능을 입력하세요";
	SetConsoleTextAttribute(hConsole, 15);
	cout<<"       □\n";
	cout << "                                 □                                □\n";
	cout << "                                 □                                □\n";
	cout << "                                 □           1. 로그인            □\n";
	cout << "                                 □                                □\n";
	cout << "                                 □          2. 회원가입           □\n";
	cout << "                                 □                                □\n";
	cout << "                                 □            3. 종료             □\n";
	cout << "                                 □                                □\n";
	cout << "                                 □                                □\n";
	cout << "                                 □□□□□□□□□□□□□□□□□□\n";
}

