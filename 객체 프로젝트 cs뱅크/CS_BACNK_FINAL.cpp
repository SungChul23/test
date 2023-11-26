#include <iostream>
#include <mysql.h>  // 추가: mysql.h 포함
#include<string>
#include <sstream> //stringstream 사용을 위한 헤더
#include <Windows.h>
#include <cstdlib> // exit 함수를 사용하기 위한 헤더
#include <ctime> //랜덤을 위한 헤더

using namespace std;

MYSQL Conn;           // MySQL 정보 담을 구조체
MYSQL* ConnPtr = NULL; // MySQL 핸들
MYSQL_RES* Result;     // 쿼리 성공시 결과를 담는 구조체 포인터
MYSQL_ROW Row;         // 쿼리 성공시 결과로 나온 행의 정보를 담는 구조체
int Stat;              // 쿼리 요청 후 결과 (성공, 실패)
string MemberNo = "NULL";

void end() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // 하늘 색상 출력
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    cout << "           #### ##  ###  ##    ##     ###  ##  ##  ###           ##  ##    ## ##   ##  ###\n";
    cout << "           # ## ##   ##  ##     ##      ## ##  ##  ##            ##  ##   ##   ##  ##   ##\n";
    cout << "             ##      ##  ##   ## ##    # ## #  ## ##             ##  ##   ##   ##  ##   ##\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    cout << "             ##      ## ###   ##  ##   ## ##   ## ##              ## ##   ##   ##  ##   ##\n";
    cout << "             ##      ##  ##   ## ###   ##  ##  ## ###              ##     ##   ##  ##   ##\n";
    cout << "             ##      ##  ##   ##  ##   ##  ##  ##  ##              ##     ##   ##  ##   ##\n";
    cout << "            ####    ###  ##  ###  ##  ###  ##  ##  ###             ##      ## ##    ## ##\n";
    cout << "\n\n\n\n";
    // 흰색으로 설정하여 다음 출력이 흰색으로 나타나도록 함
    SetConsoleTextAttribute(hConsole, 15);
}

/////////////////////////////////////////////////로그인 클래스//////////////////////////////////////////
class Login {
public:
    string Input_ID, Input_PW;
    int missing = 0, error = 0;
protected:
    void login();
};

void Login::login()
{
    mysql_init(&Conn); // MySQL 정보 초기화

    // 데이터베이스와 연결
    ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "1q2w3e4r!", "cs_bank", 3306, (char*)NULL, 0);

    // 연결 결과 확인. null일 경우 실패
    if (ConnPtr == NULL) {
        fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
        return;
    }

    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //하늘 색상 출력
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    cout << "\n\n\n";
    cout << "                            ##        ##        ## ##    ####   ###   ##\n";
    cout << "                            ##      ##   ##   ##   ##     ##      ##  ##\n";
    cout << "                            ##      ##   ##   ##          ##     # ## ##\n";
    cout << "                            ##      ##   ##   ##  ###     ##     ## ##\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    cout << "                            ##      ##   ##   ##   ##     ##     ##  ##\n";
    cout << "                            ##       ## ##     ##   ##    ##     ##  ##\n";
    cout << "                            ### ###   ##        ## ##    ####   ###  ##\n";

    SetConsoleTextAttribute(hConsole, 15);
    cout << "\n\n\n\n";

    while (1)
    {
        cout << "\n";
        cout << "아이디를 입력하세요. >";
        cin >> Input_ID;
        cout << "비밀번호를 입력하세요. >";
        cin >> Input_PW;

        // SQL 쿼리를 준비
        string query = "SELECT * FROM customer_table WHERE ID = '" + Input_ID + "' AND password = '" + Input_PW + "'";

        // SQL 쿼리를 실행
        if (mysql_query(&Conn, query.c_str()) == 0) {
            MYSQL_RES* result = mysql_store_result(&Conn);
            if (result) {
                MYSQL_ROW row = mysql_fetch_row(result);
                if (row) {
                    // 결과 세트 해체
                    mysql_free_result(result);
                    // 로그인 성공 및 환영 구문
                    string whoLoginquery = "SELECT Name,NO FROM cs_bank.customer_table WHERE ID = '" + Input_ID + "'";
                    if (mysql_query(&Conn, whoLoginquery.c_str()) == 0) {
                        MYSQL_RES* result = mysql_store_result(&Conn);
                        if (result != NULL) {
                            MYSQL_ROW row = mysql_fetch_row(result);
                            if (row != NULL) {
                                cout << "환영합니다! " << row[0] << "님!";
                                MemberNo = row[1];
                            }
                        }
                        // 결과 세트 해체
                        mysql_free_result(result);
                    }

                    Sleep(1000);

                    return;
                }
                else {
                    // 비밀번호 실패 최대 5번 가능
                    missing += 1;
                    cout << "비밀번호가 옳지않습니다." << endl;
                    cout << "현재 비밀번호" << missing << "회 오류입니다." << endl;
                    if (missing < 5) {
                        continue;

                    }
                    cout << "로그인 5회 실패시 로그인이 제한됩니다." << endl;

                    //결과 세트 해체
                    mysql_free_result(result);

                    //프로그램 종료
                    cout << "\n";
                    cout << "사유 : 로그인 5회 실패" << endl;
                    cout << "\n\n\n\n";
                    end();
                    exit(0);

                }
                
            }
        }
        else {
            // 쿼리 실행중 오류 발생
            cout << "SQL query error: " << mysql_error(&Conn);
        }
    }

    // MYSQL과 연결 해체
    //mysql_close(&Conn); 연결을 해체하면 새로 받은 회원정보가 사라질까 ??
}




////////////////////////////////////////////////회원가입 클래스//////////////////////////////////////////
class SignUp {
public:
    string ID, PW, NAME, confirmPW; //confirmPW : 비밀번호를 한번더 치게 하기위한 변수
    string PhoneNumber;
    string AcccountNumber;
protected:
    void signup();
};

void SignUp::signup() {


    mysql_init(&Conn); // MySQL 정보 초기화

    // 데이터베이스와 연결
    MYSQL* ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "1q2w3e4r!", "cs_bank", 3306, (char*)NULL, 0);

    // 연결 결과 확인. null일 경우 실패
    if (ConnPtr == NULL) {
        fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
        return;
    }

    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // 하늘 색상 출력
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    std::cout << "\n\n\n";
    cout << "                ## ##     ####    ## ##   ###  ##           ##  ###  ### ##   \n";
    cout << "               ##   ##     ##    ##   ##    ## ##           ##   ##   ##  ##  \n";
    cout << "               ####        ##    ##        # ## #           ##   ##   ##  ##  \n";
    cout << "                #####      ##    ##  ###   ## ##            ##   ##   ##  ##  \n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    cout << "                   ###     ##    ##   ##   ##  ##           ##   ##   ## ##   \n";
    cout << "               ##   ##     ##    ##   ##   ##  ##           ##   ##   ##      \n";
    cout << "                ## ##     ####    ## ##   ###  ##            ## ##   ####     \n";
    cout << "\n\n\n\n";
    // 흰색으로 설정하여 다음 출력이 흰색으로 나타나도록 함
    SetConsoleTextAttribute(hConsole, 15);
    cout << "           ================================회원가입================================                  \n\n\n\n";

    while (1) {
        cout << "\n";
        cout << "사용하실 아이디를 입력하세요. (공백 없이 최대 50자) >> ";
        cin >> ID;

        // ID 중복 체크
        string idCheckQuery = "SELECT ID FROM customer_table WHERE ID = '" + ID + "'";
        mysql_query(&Conn, idCheckQuery.c_str());
        MYSQL_RES* idCheckResult = mysql_store_result(&Conn);

        if (mysql_num_rows(idCheckResult) == 0) {
            mysql_free_result(idCheckResult);
            break;
        }
        cout << "이미 존재하는 아이디 입니다!\n";
        mysql_free_result(idCheckResult);
    }

    cout << "\n";
    cout << "이름을 입력하세요. (영어로 입력해주세요) >> ";
    cin >> NAME;


    // 전화번호 입력
    cout << "\n";
    cout << "전화번호를 입력하세요. >> ";
    cin >> PhoneNumber;


    // 비밀번호 입력
    while (1) {
        cout << "\n";
        cout << "사용하실 비밀번호를 입력하세요. (숫자 4자리) >> ";
        cin >> PW;

        // 비밀번호 확인
        cout << "비밀번호를 다시 입력하세요. >> ";
        cin >> confirmPW;

        while (PW != confirmPW) {
            cout << "비밀번호가 일치하지 않습니다. 다시 입력해주세요.\n";

            // 다시 입력 받기
            cout << "\n";
            cout << "사용하실 비밀번호를 입력하세요. (숫자 4자리) >> ";
            cin >> PW;

            cout << "비밀번호를 다시 입력하세요. >> ";
            cin >> confirmPW;
        }

        cout << "비밀번호가 설정되었습니다!.\n";

        break; 
    }

    cout << "가입을 진심으로 환영합니다!!!\n";
    Sleep(3000);
    //가입 인원 판별 및 No값 삽입

    int No = 0;
    string AccountCountQuery = "SELECT COUNT(*) FROM customer_table";
    if (mysql_query(&Conn, AccountCountQuery.c_str()) != 0) {
        fprintf(stderr, "SQL 문 실행 오류: %s\n", mysql_error(&Conn));
    }
    else {
        MYSQL_RES* result = mysql_use_result(&Conn);
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row != NULL) {
            if (row != NULL) {
                No = atoi(row[0]); // atoi = 문자열을 숫자로
            }
            mysql_free_result(result);
        }
    }
    //가입 인원 +1 =No
    No += 1;
    // 회원 정보를 데이터베이스에 삽입하기 위한 SQL 쿼리 생성 및 실행
    string insertQuery = "INSERT INTO `cs_bank`.`customer_table` (`No`, `ID`, `Name`, `Phone`, `Password`) VALUES ('" + to_string(No) + "','" + ID + "', '" + NAME + "',  '" + PhoneNumber + "'  , '" + PW + "')";

    if (mysql_query(&Conn, insertQuery.c_str()) != 0) {
        fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
    }

}

////////////////////////////////스타트 클래스/////////////////////
class Start : public Login, public SignUp {
public:
    int StartMenuNum = 0;
    void start();
};


class User : public Start {
private:
    string ID, Name, Phone, Password;
public:
    int UserMenuNum = 0;
    void UserFunction();
    //유저의 정보를 받아오는 내용
    void GetUserInfo();
    //계좌 개설
    void OpenAccount();
    //계좌 여부 확인
    void IsHaveAccount();
    //입금
    void deposit();
    //출금
    void withdraw();
    //계좌이체
    void transfer();
    //관리자 - 모든 테이블 확인
    void displayCustomerTable();
    //사용자 - 본인 정보 확인
    void checkmyInfo();

};
void User::GetUserInfo() {
    string whoLoginquery = "SELECT ID,Name,Phone,Password FROM cs_bank.customer_table WHERE No = '" + MemberNo + "'";
    if (mysql_query(&Conn, whoLoginquery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(&Conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                ID = row[0];
                Name = row[1];
                Phone = row[2];
                Password = row[3];
            }
        }
        // 결과 세트 해체
        mysql_free_result(result);
    }
}

void User::OpenAccount() {
    string CheckPhone;
    string agree;
    cout << "계좌 개설 페이지입니다.\n";
    cout << "본인 확인을 위해 전화번호를 입력하세요. >> ";
    cin >> CheckPhone;

    if (Phone != CheckPhone) {
        cout << "본인 확인에 실패했습니다.\n";
        cout << "관리자에게 문의바랍니다. \n";
        Sleep(2000);
        UserFunction();
    }
    cout << "본인 확인이 되었습니다.\n";

    cout << "보이스피싱 등 전기 통신 금융사기가 급증함에 따라, 고객님의 금융사기 피해를 사전에 예방하고 소중한 자산을 보호하고자 합니다.";
    cout << "계좌 개설 목적이 본인 사용이 맞으신가요?\n< Yes / NO >\n";
    cin >> agree;
    if (agree == "No" || agree == "NO") {
        cout << "관리자에게 문의바랍니다. \n";
        Sleep(2000);
        UserFunction();
    }

    int randomNumber = 0;

    while (1) {
        //계좌번호 랜덤 생성
        srand(time(NULL));
        randomNumber = 10000000 + rand() % (99999999 - 10000000 + 1);
        //계좌번호 중복 확인
        string AccountCheckQuery = "SELECT ID FROM account_table WHERE ID = '" + to_string(randomNumber) + "'";
        mysql_query(&Conn, AccountCheckQuery.c_str());
        MYSQL_RES* idCheckResult = mysql_store_result(&Conn);
        if (mysql_num_rows(idCheckResult) == 0) {
            mysql_free_result(idCheckResult);
            cout << "고객님의 계좌번호는" << randomNumber << "입니다.\n";
            break;
        }
    }
    Sleep(3000);
    string AccountCountQuery = "SELECT COUNT(*) FROM account_table";
    int accountCount;
    if (mysql_query(&Conn, AccountCountQuery.c_str()) != 0) {
        fprintf(stderr, "SQL 문 실행 오류: %s\n", mysql_error(&Conn));
    }
    else {
        MYSQL_RES* result = mysql_use_result(&Conn);
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row != NULL) {
            if (row != NULL) {
                accountCount = atoi(row[0]);
            }
            mysql_free_result(result);
        }
    }

    //계좌 개수 +1
    accountCount += 1;
    //계좌 이름
    string accountName = "cs제일계좌";
    accountName += to_string(accountCount);
    // 회원 정보를 데이터베이스에 삽입하기 위한 SQL 쿼리 생성 및 실행
    string insertQuery = "INSERT INTO `cs_bank`.`account_table` (`ID`, `AccountName`, `AccountNumber`, `Balance`) VALUES ('" + ID + "', '" + accountName + "',   '" + to_string(randomNumber) + "'  , '" + to_string(0) + "')";
    if (mysql_query(&Conn, insertQuery.c_str()) != 0) {
        fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
    }

    cout << "계좌 개설을 축하합니다";
    Sleep(3000);
}

void User::IsHaveAccount() {
    string CountAccount="";
    string IsHaveAccount = "SELECT COUNT(*) FROM account_table WHERE ID = '" + ID + "'";
    if (mysql_query(&Conn, IsHaveAccount.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(&Conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                CountAccount = row[0];
            }
        }
        mysql_free_result(result);
    }
    if (CountAccount == "0") {
        cout << "계좌가 없습니다!\n계좌 생성 먼저 해주세요.";
        Sleep(3000);
        UserFunction();
    }
}

void User::deposit() {

    // 모든 계좌 출력 
    string AllAccount = "SELECT AccountName, AccountNumber, Balance FROM account_table WHERE ID = '" + ID + "'";
    mysql_query(&Conn, AllAccount.c_str());
    MYSQL_RES* result = mysql_store_result(&Conn);

    if (result == NULL) {
        fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
        return;
    }


    // 결과 출력
    int i = 1;
    cout << Name << "님의 계좌 목록\n\n";
    cout << "No       계좌 이름           계좌번호            잔액\n";
    while ((Row = mysql_fetch_row(result)) != NULL)
    {
        cout << i << "        " << Row[0] << "         " << Row[1] << "            " << Row[2] << endl;
        i++;
    }

    cout << "\n";
    

    int DepositAccount;
    cout << "입금 하실 계좌를 입력하세요. >> ";
    cin >> DepositAccount;

    string AccountName;
    string DepositQuery = "SELECT AccountName FROM account_table WHERE AccountNumber = '" + to_string(DepositAccount) + "'";
    if (mysql_query(&Conn, DepositQuery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(&Conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                AccountName = row[0];
            }
        }
        mysql_free_result(result);
    }
    else {
        cout << "계좌가 존재하지 않습니다.\n 계좌 번호 재확인 바랍니다!";
        Sleep(3000);
        UserFunction();
    }
    cout << "입금하시는 계좌명이 "<<AccountName<<"이 맞으신가요? <Yes/No>";
    string AccountNameCheck;
    cin >> AccountNameCheck;
    if (AccountNameCheck == "No" || AccountNameCheck == "NO" || AccountNameCheck=="N" || AccountNameCheck == "n") {
        cout << "계좌 번호 재확인 바랍니다!";
        Sleep(3000);
        UserFunction();
    }

    int InputDeposit;
    cout << "입금 하실 금액을 입력하세요. >> ";
    cin >> InputDeposit;

    string updateQuery = "UPDATE account_table SET Balance = Balance+'" + to_string(InputDeposit) + "' WHERE AccountNumber = '" + to_string(DepositAccount) + "'";
    if (mysql_query(&Conn, updateQuery.c_str()) == 0) {
        cout << "성공적으로 입금이 완료되었습니다.";
        
        Sleep(3000);
    }
    else {
        cout << "업데이트를 수행하는 동안 오류가 발생했습니다.";
        Sleep(3000);
    }

}

void User::withdraw() {

    // 모든 계좌 출력 
    string AllAccount = "SELECT AccountName, AccountNumber, Balance FROM account_table WHERE ID = '" + ID + "'";
    mysql_query(&Conn, AllAccount.c_str());
    MYSQL_RES* result = mysql_store_result(&Conn);

    if (result == NULL) {
        fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
        return;
    }


    // 결과 출력
    int i = 1;
    cout << Name << "님의 계좌 목록\n\n";
    cout << "No       계좌 이름           계좌번호            잔액\n";
    while ((Row = mysql_fetch_row(result)) != NULL)
    {
        cout << i << "        " << Row[0] << "         " << Row[1] << "            " << Row[2] << endl;
        i++;
    }

    cout << "\n";
    
    int WithdrawAccount;
    cout << "출금 하실 계좌 번호를 입력하세요. >> ";
    cin >> WithdrawAccount;

    //출금하는 계좌명 불러오는 내용
    string AccountName;
    string AccountNameQuery = "SELECT AccountName FROM account_table WHERE AccountNumber = '" + to_string(WithdrawAccount) + "'";
    if (mysql_query(&Conn, AccountNameQuery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(&Conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                AccountName = row[0];
            }
        }
    }

    //출금하는 계좌가 본인 명의인지 확인하는 내용
    string AccountID;
    string AccountIDQuery = "SELECT ID FROM account_table WHERE AccountNumber = '" + to_string(WithdrawAccount) + "'";
    if (mysql_query(&Conn, AccountIDQuery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(&Conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                AccountID = row[0];
            }
        }
    }
    else {
        cout << "계좌가 존재하지 않습니다.\n 계좌 번호 재확인 바랍니다!";
        Sleep(3000);
        UserFunction();
    }

    //만약 로그인된 아이디와 입력받은 계좌번호의 아이디가 다를 경우
    if (AccountID != ID) {
        cout << "출금하려는 계좌가 본인 명의가 아닙니다.\n 계좌 확인을 다시 해주세요.";
        Sleep(3000);
        UserFunction();
    }

    cout << "출금하시는 계좌명이 " << AccountName << "이 맞으신가요? <Yes/No>";
    string AccountNameCheck;
    cin >> AccountNameCheck;
    if (AccountNameCheck == "No" || AccountNameCheck == "NO") {
        cout << "계좌 번호 재확인 바랍니다!";
        Sleep(3000);
        UserFunction();
    }

    string checkPW;
    cout << "사용자 비밀번호를 입력하세요. >>";
    cin >> checkPW;
    //사용자 비밀번호와 입력 비밀번호가 다를 경우
    if (checkPW != Password) {
        cout << "비밀번호 재확인 바랍니다!";
        Sleep(3000);
        UserFunction();
    }

    int InputWithdraw;
    cout << "출금 하실 금액을 입력하세요. >> ";
    cin >> InputWithdraw;

    //잔액이 부족한지 확인하는 쿼리
    string NowAccountBalnce;
    string IsBalanceQuery = "SELECT Balance FROM account_table WHERE AccountNumber = '" + to_string(WithdrawAccount) + "'";
    if (mysql_query(&Conn, IsBalanceQuery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(&Conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                NowAccountBalnce = row[0];
            }
        }
        mysql_free_result(result);
    }
    
    //만약 출금하려는 금액이 잔액보다 클 경우
    if (stoi(NowAccountBalnce) < InputWithdraw) {
        cout << "잔액이 부족합니다!";
        Sleep(3000);
        UserFunction();
    }

    //출금 쿼리
    string updateQuery = "UPDATE account_table SET Balance = Balance-'" + to_string(InputWithdraw) + "' WHERE AccountNumber = '" + to_string(WithdrawAccount) + "'";
    

    if (mysql_query(&Conn, updateQuery.c_str()) == 0) {
        cout << "성공적으로 출금이 완료되었습니다.\n";
    }
    else {
        cout << "업데이트를 수행하는 동안 오류가 발생했습니다.";
        Sleep(3000);
    }

    //출금 후 잔액 확인 쿼리
    string BalanceQuery = "SELECT Balance FROM account_table WHERE AccountNumber = '" + to_string(WithdrawAccount) + "'";
    string AccountBalnce;

    if (mysql_query(&Conn, BalanceQuery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(&Conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                AccountBalnce = row[0];
            }
        }
        mysql_free_result(result);
    }
    cout << "잔액은 " << AccountBalnce << "입니다.";
    Sleep(3000);
}



void User::transfer() {

    string YourAccountName;
    int YourAccount;
    cout << "이체 받으실 계좌 번호를 입력하세요. >> ";
    cin >> YourAccount;

    //이체(입금) 계좌명 불러오는 쿼리
    string SendAccountName;
    string YourAccountNameQuery = "SELECT AccountName FROM account_table WHERE AccountNumber = '" + to_string(YourAccount) + "'";
    if (mysql_query(&Conn, YourAccountNameQuery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(&Conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                YourAccountName = row[0];
            }
        }
    }
    else {
        cout << "계좌가 존재하지 않습니다.\n 계좌 번호 재확인 바랍니다!";
        Sleep(3000);
        UserFunction();
    }

    cout << "이체 받으실 계좌명의 계좌명이 " << YourAccountName << "이 맞으신가요? <Yes/No>";
    string YourAccountNameCheck;
    cin >> YourAccountNameCheck;
    if (YourAccountNameCheck == "No" || YourAccountNameCheck == "NO" || YourAccountNameCheck == "N" || YourAccountNameCheck == "n") {
        cout << "계좌 번호 재확인 바랍니다!";
        Sleep(3000);
        UserFunction();
    }

    //이체(출금) - 본인 모든 계좌 출력
    string MyAllAccount = "SELECT AccountName, AccountNumber, Balance FROM account_table WHERE ID = '" + ID + "'";
    mysql_query(&Conn, MyAllAccount.c_str());
    MYSQL_RES* result = mysql_store_result(&Conn);

    if (result == NULL) {
        fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
        return;
    }

    int i = 1;
    cout << "\nNo       계좌 이름           계좌번호            잔액\n";
    while ((Row = mysql_fetch_row(result)) != NULL)
    {
        cout << i << "        " << Row[0] << "         " << Row[1] << "            " << Row[2] << endl;
        i++;
    }
    cout << "\n";

    //이체(출금) - 계좌 선택

    int MyAccountNumber;
    cout << "이체 하실 계좌 번호를 입력하세요. >> ";
    cin >> MyAccountNumber;

    //이체(출금) 계좌명 불러오는 쿼리
    string MyAccountName;
    string AccountNameQuery = "SELECT AccountName FROM account_table WHERE AccountNumber = '" + to_string(MyAccountNumber) + "'";
    if (mysql_query(&Conn, AccountNameQuery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(&Conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                MyAccountName = row[0];
            }
        }
    }

    //계좌가 본인 명의인지 확인하는 쿼리
    string AccountID;
    string AccountIDQuery = "SELECT ID FROM account_table WHERE AccountNumber = '" + to_string(MyAccountNumber) + "'";
    if (mysql_query(&Conn, AccountIDQuery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(&Conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                AccountID = row[0];
            }
        }
    }
    else {
        cout << "계좌가 존재하지 않습니다.\n 계좌 번호 재확인 바랍니다!";
        Sleep(3000);
        UserFunction();
    }

    //만약 로그인된 아이디와 입력받은 계좌번호의 아이디가 다를 경우
    if (AccountID != ID) {
        cout << "이체하실 계좌가 본인 명의가 아닙니다.\n 계좌 확인을 다시 해주세요.";
        Sleep(3000);
        UserFunction();
    }

    cout << "이체하시는 계좌명이 " << MyAccountName << "이 맞으신가요? <Yes/No>";
    string AccountNameCheck;
    cin >> AccountNameCheck;
    if (AccountNameCheck == "No" || AccountNameCheck == "NO") {
        cout << "계좌 번호 재확인 바랍니다!";
        Sleep(3000);
        UserFunction();
    }


    string checkPW;
    cout << "사용자 비밀번호를 입력하세요. >>";
    cin >> checkPW;
    //사용자 비밀번호와 입력 비밀번호가 다를 경우
    if (checkPW != Password) {
        cout << "비밀번호 재확인 바랍니다!";
        Sleep(3000);
        UserFunction();
    }

    int InputTransfer;
    cout << "이체 하실 금액을 입력하세요. >> ";
    cin >> InputTransfer;

    //잔액이 부족한지 확인하는 쿼리
    string MyAccountBalnce;
    string MyBalanceQuery = "SELECT Balance FROM account_table WHERE AccountNumber = '" + to_string(MyAccountNumber) + "'";
    if (mysql_query(&Conn, MyBalanceQuery.c_str()) == 0) {
        MYSQL_RES* result = mysql_store_result(&Conn);
        if (result != NULL) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != NULL) {
                MyAccountBalnce = row[0];
            }
        }
        mysql_free_result(result);
    }

    //만약 출금하려는 금액이 잔액보다 클 경우
    if (stoi(MyAccountBalnce) < InputTransfer) {
        cout << "잔액이 부족합니다!";
        Sleep(3000);
        UserFunction();
    }

    //출금 내용
    string WithdrawAccountQuery = "UPDATE account_table SET Balance = Balance-'" + to_string(InputTransfer) + "' WHERE AccountNumber = '" + to_string(MyAccountNumber) + "'";


    if (mysql_query(&Conn, WithdrawAccountQuery.c_str()) == 0) {
        cout << "정상 실행 완료...\n";
    }
    else {
        cout << "업데이트를 수행하는 동안 오류가 발생했습니다.";
        Sleep(3000);
    }

    //이체 완료
    string updateQuery = "UPDATE account_table SET Balance = Balance+'" + to_string(InputTransfer) + "' WHERE AccountNumber = '" + to_string(YourAccount) + "'";
    if (mysql_query(&Conn, updateQuery.c_str()) == 0) {
        cout << "성공적으로 이체가 완료되었습니다.";
        Sleep(3000);
    }
    else {
        cout << "업데이트를 수행하는 동안 오류가 발생했습니다.";
        Sleep(3000);
    }


}


void User::checkmyInfo() {

    cout << "ID :" << ID << endl;
    cout << "이름 :" << Name << endl;
    cout << "전화번호 :" << Phone << endl;
    
    // 모든 계좌 출력 
   string AllAccount = "SELECT AccountName, AccountNumber, Balance FROM account_table WHERE ID = '" +ID + "'";
   mysql_query(&Conn, AllAccount.c_str());
   MYSQL_RES* result = mysql_store_result(&Conn);

    if (result == NULL) {
        fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
        return;
    }


    // 결과 출력
    int i = 1;
    cout << Name<<"님의 계좌 목록\n\n";
    cout << "No       계좌 이름           계좌번호            잔액\n";
    while ((Row = mysql_fetch_row(result)) != NULL)
    {
        cout << i << "        " << Row[0] <<"         " << Row[1]<<"            " << Row[2] << endl;
        i++;
    }
    Sleep(3000);
}


void User::displayCustomerTable() {
    // 고객 테이블 출력 쿼리 요청
    const char* customerquery = "SELECT * FROM customer_table";
    Stat = mysql_query(ConnPtr, customerquery);

    if (Stat != 0) {
        fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
        return;
    }

    Result = mysql_store_result(ConnPtr);

    // 결과 출력
    cout << "Customer Table:\n";
    cout << "No    ID        이름           전화번호            비밀번호\n";
    while ((Row = mysql_fetch_row(Result)) != NULL)
    {
        
        cout << Row[0]<<"     "<< Row[1]<<"     " << Row[2]<<"        "<< Row[3]<<"            " << Row[4] << "\n";
    }

    cout << "\n";
    
    // 계좌 테이블 출력 쿼리 요청
    const char* accountquery = "SELECT * FROM account_table";
    Stat = mysql_query(ConnPtr, accountquery);

    if (Stat != 0) {
        fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
        return;
    }

    Result = mysql_store_result(ConnPtr);

    // 결과 출력
    cout << "Account Table:\n";
    cout << "ID        계좌명        계좌번호       잔액\n";
    while ((Row = mysql_fetch_row(Result)) != NULL)
    {
        
        cout << Row[0]<<"    " << Row[1]<<"     "<< Row[2]<<"       " << Row[3] << "\n";
    }
    Sleep(3000);
}

void User::UserFunction() {
    GetUserInfo();
    system("cls");
    
    cout << "---------------환영합니다!" << Name << "님!---------------" << "\n";
    cout << "0. 계좌 생성" << endl;
    cout << "1. 예금 입금" << endl;
    cout << "2. 예금 출금" << endl;
    cout << "3. 계좌 이체" << endl;
    cout << "4. 내 정보 보기 " << endl;
    cout << "5. 로그아웃" << endl;
    cout << "6. 관리자 기능" << endl;
    cout << "7. 프로그램 종료" << endl;
    cout << "\n";
    cout << "원하시는 메뉴를 입력해 주세요. >> ";
    cin >> UserMenuNum;

    switch (UserMenuNum) {
    case 0:
        OpenAccount(); break;//계좌 개설
    case 1:
        IsHaveAccount();
        deposit();  break; //예금 입금
    case 2:
        IsHaveAccount();
        withdraw();  break;//예금 출금
    case 3:
        IsHaveAccount();
        transfer(); break;//계좌 이체
    case 4:
        checkmyInfo(); break;//내 정보 보기
    case 5:
        login(); break; // 로그아웃
    case 6: //관리자 기능
    {
        string adminPassword;
        cout << "관리자 비밀번호를 입력하세요: ";
        cin >> adminPassword;

        // 입력된 비밀번호를 기대하는 관리자 비밀번호와 비교
        if (adminPassword == "1q2w3e4r!") {
            displayCustomerTable();  // 관리자 기능 함수 호출
        }
        else {
            cout << "잘못된 비밀번호입니다. 관리자 권한이 없습니다.\n";
            Sleep(1000);
        }
        break;
    }
    case 7:
        cout << "프로그램을 종료합니다" << endl; // 프로그램 종료
        end();
        exit(0);

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
        cout << "프로그램을 종료합니다" << endl;
        end();
        exit(0);
    default:
        system("cls");
        cout << "잘못된 입력입니다. 다시 입력해주세요.\n";
        start();
    }

    if (MemberNo != "NULL") {
        //로그인 완료
    }
}


//////////////////////////////////////MYSQL 연결 + MAIN 시작//////////////////////////////////////

int main() {
    system("chcp 65001");
    system("title CS BANK");
    User user; //User 클래스의 객체 선언
    Start customer; //Start 클래스의 객체 선언

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
    system("mode con: cols=100 lines=40");

    //////////////////////////////////////은행 구문 시작//////////////////////////////////////
    while (1)
    {
        system("cls");
        //하늘 색상 출력
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        cout << "\n\n\n";
        cout << "                 ######   ######     ########     ###    ##    ## ##    ##                 \n";
        cout << "                ##    ## ##    ##    ##     ##   ## ##   ###   ## ##   ##                  \n";
        cout << "                ##       ##          ##     ##  ##   ##  ####  ## ##  ##                   \n";
        cout << "                ##        ######     ########  ##     ## ## ## ## #####                    \n";
        cout << "                ##             ##    ##     ## ######### ##  #### ##  ##                   \n";
        //초록 색상 출력
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        cout << "                ##    ## ##    ##    ##     ## ##     ## ##   ### ##   ##                  \n";
        cout << "                 ######   ######     ########  ##     ## ##    ## ##    ##                 \n";
        cout << "\n\n\n\n";
        //검정 색상 출력
        SetConsoleTextAttribute(hConsole, 15);
        cout << "                                  CS제일은행에 오신것을 환영합니다!                                 \n\n\n\n";



        customer.start();
        while (1) {
            if (MemberNo != "NULL") {
                user.UserFunction();
            }
            else {
                break;
            }
        }


    }

    mysql_free_result(Result);// MySQL C API에서 사용한 메모리를 해제하는 함수
    mysql_close(ConnPtr); // MySQL 데이터베이스 연결을 닫는 함수
    end();
    return 0;
}
