﻿#include <iostream>
#include <mysql.h>  // 추가: mysql.h 포함
#include<string>
#include <sstream> //stringstream 사용을 위한 헤더
#include <Windows.h>


using namespace std;

MYSQL Conn;           // MySQL 정보 담을 구조체
MYSQL* ConnPtr = NULL; // MySQL 핸들
MYSQL_RES* Result;     // 쿼리 성공시 결과를 담는 구조체 포인터
MYSQL_ROW Row;         // 쿼리 성공시 결과로 나온 행의 정보를 담는 구조체
int Stat;              // 쿼리 요청 후 결과 (성공, 실패)


int EndCode = 0;
/////////////////////////////////////////////////로그인 클래스//////////////////////////////////////////
class Login {
public:
    string Input_ID, Input_PW;
    int missing = 0, error = 0;
    int MemberNo = 0;
protected:
    void login();
};

void Login::login() {
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //하늘 색상 출력
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    cout << "\n\n\n";
    cout << "        ##        ##        ## ##    ####   ###   ##\n";
    cout << "        ##      ##   ##   ##   ##     ##      ##  ##\n";
    cout << "        ##      ##   ##   ##          ##     # ## ##\n";
    cout << "        ##      ##   ##   ##  ###     ##     ## ##\n";
    cout << "        ##      ##   ##   ##   ##     ##     ##  ##\n";
    cout << "        ##       ## ##     ##   ##    ##     ##  ##\n";
    cout << "        ### ###   ##        ## ##    ####   ###  ##\n";
    cout << "---------------로그인---------------" << "\n";
    while (1) {
        cout << "아이디를 입력하세요. >";
        cin >> Input_ID;
        cout << "비밀번호를 입력하세요. >";
        cin >> Input_PW;
        //아이디 비밀번호를 sql에서 확인하는 구문
        if (1) {
            cout << "환영합니다!";
            //MemberNo 변수에 사용자의 키 값을 삽입
            break;
        }
        else {
            missing += 1;
            cout << "비밀번호가 옳지않습니다.";
            cout << "현재 비밀번호" << missing << "회 오류입니다.";
            if (missing < 5)
            {
                error = 1;
                break;
            }
            cout << "로그인 5회 실패시 로그인이 제한됩니다.";
        }
    }

}
////////////////////////////////////////////////회원기입 클래스//////////////////////////////////////////
class SignUp {
public:
    string ID, PW, NAME;
    int PhoneNumber = 0;
    string AcccountNumber;
protected:
    void signup();
};

void SignUp::signup() {
    system("cls");
    cout << "---------------회원가입---------------" << "\n";
    cout << "이름을 입력하세요. (현재는 영문만 가능) >";
    cin >> NAME;
    while (1) {
        cout << "사용하실 아이디를 입력하세요. (영문 2자)";
        cin >> ID;
        char idcheck[] = "SELECT ID FROM customer_table";
        cout << idcheck << endl;
        if (idcheck[0] == NULL) {
            break;
        }
        cout << "이미 존재하는 아이디입니다!\n";
        break;
    }
    cout << "사용하실 비밀번호를 입력하세요. (영문 2자)";
    cin >> PW;
    while (1) {
        cout << "전화번호를 입력하세요.";
        cin >> PhoneNumber;
        char phonenumcheck[] = "SELECT PhoneNumber FROM customer_table";
        if (phonenumcheck[0] == NULL) {
            break;
        }
        cout << "이미 가입된 회원입니다!\n";
        break;
    }
    cout << "고객님의 계좌 번호는" << PhoneNumber << "0 입니다!\n";
    AcccountNumber = PhoneNumber;
    stringstream AcccountNumber;
    cout << "가입을 진심으로 환영합니다!!!\n";
    "INSERT INTO `cs_bank`.`customer_table` (`ID`, `Name`, `Phone`, `Password`, `ACCOUNTNUMBER`) VALUES(ID, NAME, PhoneNumber, PW, AcccountNumber)";
    int a;
    cin >> a;
}
////////////////////////////////스타트 클래스/////////////////////
class Start : public Login, public SignUp {
public:
    int StartMenuNum = 0;
    void start();
};


class User : public Start {
public:
    int UserMenuNum = 0;
    void UserFunction();
    //유저의 정보를 받아오는 내용
    string UserName;


    void deposit();
    void withdraw();
    void transfer();
    void AccountCheck();
    void TransferCheck();
    void displayCustomerTable();
};

void User::deposit() {
    int InputDeposit;
    cout << "입금 하실 금액을 입력하세요.";
    cin >> InputDeposit;

}

void User::withdraw() {

}

void User::transfer() {

}

void User::AccountCheck() {

}

void User::TransferCheck() {

}

void User::displayCustomerTable() {

}

void User::UserFunction() {
    system("cls");
    cout << "---------------환영합니다!" << UserName << "님!---------------" << "\n";
    cout << "1. 예금 입금" << endl;
    cout << "2. 예금 출금" << endl;
    cout << "3. 계좌 이체" << endl;
    cout << "4. 계좌 정보 조회" << endl;
    cout << "5. 이체 내역 확인" << endl;
    cout << "6. 로그아웃" << endl;
    cout << "7. 관리자 기능" << endl;
    cout << "8. 프로그램 종료" << endl;
    
    cin >> UserMenuNum;

    switch (UserMenuNum) {
    case 1:
        deposit();  break;
    case 2:
        withdraw();  break;
    case 3:
        transfer(); break;
    case 4:
        AccountCheck(); break;
    case 5:
        TransferCheck(); break;
    case 7:
        displayCustomerTable(); break; // 사용자에 대한 모든 정보가 들어있는 테이블 호출
    case 8:
        EndCode = 1; break;
    default:
        system("cls");
        cout << "잘못된 입력입니다. 다시 입력해주세요.\n";
        UserFunction();
    }
}

void Start::start() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << "\n";
    cout << "                                 □□□□□□□□□□□□□□□□□□\n";
    cout << "                                 □                                □\n";
    cout << "                                 □                                □\n";
    cout << "                                 □        ";
    SetConsoleTextAttribute(hConsole, 12);
    cout << "기능을 입력하세요";
    SetConsoleTextAttribute(hConsole, 15);
    cout << "       □\n";
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
    cin >> StartMenuNum;
    switch (StartMenuNum) {
    case 1:
        login();  break;
    case 2:
        signup();  break;
    case 3:
        EndCode = 1; break;
    default:
        system("cls");
        cout << "잘못된 입력입니다. 다시 입력해주세요.\n";
        start();
    }
    if (error = 1) {
        cout << "에러 코드 1";
        cout << "로그인에 5회 실패했습니다.";
        cout << "프로그램이 종료됩니다";
        EndCode = 1;
    }
    if (MemberNo != 0) {
        //로그인 완료
    }
}

// 이 함수 메인에서 끌어냈음
void displayCustomerTable()
{
    // 쿼리 요청
    const char* query = "SELECT * FROM customer_table";
    Stat = mysql_query(ConnPtr, query);

    if (Stat != 0) {
        fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
        return;
    }

    Result = mysql_store_result(ConnPtr);

    // 결과 출력
    cout << "Customer Table:\n";
    while ((Row = mysql_fetch_row(Result)) != NULL)
    {
        cout << "ID: " << Row[0] << ", Name: " << Row[1] << ", Phone: " << Row[2] << ", Password: " << Row[3] << ", ACCOUNTNUMBER: " << Row[4] << "\n";
    }

   
}

//////////////////////////////////////MYSQL 연결 + MAIN 시작//////////////////////////////////////

int main() {
   
    mysql_init(&Conn); // MySQL 정보 초기화

    // 데이터베이스와 연결
    ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "1q2w3e4r!", "cs_bank", 3306, (char*)NULL, 0);

    // 연결 결과 확인. null일 경우 실패
    if (ConnPtr == NULL) {
        fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
        return 1;
    }

    //콘솔창 크기 및 색상 불러오기
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("mode con: cols=100 lines=35");

    //////////////////////////////////////은행 구문 시작//////////////////////////////////////
    while (1)
    {
        //하늘 색상 출력
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        cout << "\n\n\n";
        cout << " 	         	 ######   ######     ########     ###    ##    ## ##    ##                 \n";
        cout << " 	         	##    ## ##    ##    ##     ##   ## ##   ###   ## ##   ##                  \n";
        cout << " 	         	##       ##          ##     ##  ##   ##  ####  ## ##  ##                   \n";
        cout << " 	         	##        ######     ########  ##     ## ## ## ## #####                    \n";
        cout << " 	         	##             ##    ##     ## ######### ##  #### ##  ##                   \n";
        //초록 색상 출력
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        cout << " 	        	##    ## ##    ##    ##     ## ##     ## ##   ### ##   ##                  \n";
        cout << " 	        	 ######   ######     ########  ##     ## ##    ## ##    ##                 \n";
        cout << "\n\n\n\n";
        //검정 색상 출력
        SetConsoleTextAttribute(hConsole, 15);
        cout << "                                  CS제일은행에 오신것을 환영합니다!                                 \n\n\n\n";
        Start customer;
        customer.start();
        if (customer.MemberNo == 0) {
            customer.start();

        }
        else {
            User user;
            user.UserFunction();
        }
        if (EndCode == 1) {
            cout << "프로그램이 종료 됩니다!";
            return 0;
        }

    }







    mysql_free_result(Result);// MySQL C API에서 사용한 메모리를 해제하는 함수
    mysql_close(ConnPtr); // MySQL 데이터베이스 연결을 닫는 함수
    return 0;
}
