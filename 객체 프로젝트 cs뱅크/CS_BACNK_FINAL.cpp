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
    string ID, PW, NAME;
    int PhoneNumber = 0;
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
    cout << "---------------회원가입---------------" << "\n";

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

        // 두 번 입력
        cout << "\n";
        cout << "비밀번호를 다시 입력하세요. >> ";
        string confirmPW;
        cin >> confirmPW;

        if (PW == confirmPW) {
            break;
        }
        cout << "비밀번호가 일치하지 않습니다. 다시 입력해주세요.\n";
    }

    cout << "가입을 진심으로 환영합니다!!!\n";
    Sleep(3000);
    //가입 인원 판별 및 No값 삽입
    int No=0;
    string AccountCountQuery = "SELECT COUNT(*) FROM customer_table";
    if (mysql_query(&Conn, AccountCountQuery.c_str()) != 0) {
        fprintf(stderr, "SQL 문 실행 오류: %s\n", mysql_error(&Conn));
    }
    else {
        MYSQL_RES* result = mysql_use_result(&Conn);
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row != NULL) {
            if (row != NULL) {
                No = atoi(row[0]);
            }
            mysql_free_result(result);
        }
    }
    //가입 인원 +1 =No
    No += 1;
    // 회원 정보를 데이터베이스에 삽입하기 위한 SQL 쿼리 생성 및 실행
    string insertQuery = "INSERT INTO `cs_bank`.`customer_table` (`No`, `ID`, `Name`, `Phone`, `Password`) VALUES ('" + to_string(No) + "','" + ID + "', '" + NAME + "',   '" + to_string(PhoneNumber) + "'  , '" + PW + "')";

    if (mysql_query(&Conn, insertQuery.c_str()) != 0) {
        fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
    }

    // MySQL 연결 해제
    //mysql_close(&Conn);
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
    string whoLoginquery = "SELECT ID,Name,Phone FROM cs_bank.customer_table WHERE No = '" + MemberNo + "'";
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
    cout << "본인 확인을 위해 전화번호를 입력하세요.\n";
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
        randomNumber= 10000000 + rand() % (99999999 - 10000000 + 1);
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
    string accountName="cs제일계좌";
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

}

void User::deposit() {
    int InputDeposit;
    cout << "입금 하실 금액을 입력하세요. >> ";
    cin >> InputDeposit;

    //SQL안전모드 해제
    string safeoff = "SET SQL_SAFE_UPDATES = 0;";
    mysql_query(&Conn, safeoff.c_str());

    string updateQuery = "UPDATE customer_table SET 잔액 = 잔액+'" + to_string(InputDeposit) + "' WHERE No = '" + MemberNo + "'";
    if (mysql_query(&Conn, updateQuery.c_str()) == 0) {
        cout << "업데이트가 성공적으로 수행되었습니다.";
        Sleep(3000);
    }
    else {
        cout << "업데이트를 수행하는 동안 오류가 발생했습니다.";
        Sleep(3000);
    }

    string safeon = "SET SQL_SAFE_UPDATES = 1;";
    mysql_query(&Conn, safeon.c_str());
}

void User::withdraw() {

}

void User::transfer() {

}





void User::checkmyInfo() {
    //UserId, UserName, UserPhone, UserPassword, UserAccountNumber
    cout << "ID :" << ID << endl;
    cout << "이름 :" << Name << endl;
    cout << "전화번호 :" << Phone << endl;
    cout << "비밀번호 :" << Password << endl;
    Sleep(3000);
}


void User::displayCustomerTable() {
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
        cout << "ID: " << Row[0] << ", Name: " << Row[1] << ", PhoneNumber: " << Row[2] << ", Password: " << Row[3] << ", ACCOUNTNUMBER: " << Row[4] << "\n";
    }
    Sleep(3000);
}

void User::UserFunction() {
    GetUserInfo();
    system("cls");
    cout << "---------------환영합니다!" << Name << "님!---------------" << "\n";
    cout << "0. 계좌 생성" <<endl;
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
        deposit();  break; //예금 입금
    case 2:
        withdraw();  break;//예금 출금
    case 3:
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
        }
        break;
    }



    case 7:
        cout << "프로그램을 종료합니다" << endl; // 프로그램 종료 
        exit(0);
    case 9:
        checkmyInfo(); break;
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
    system("mode con: cols=100 lines=50");

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
        cout << "               ##    ## ##    ##    ##     ## ##     ## ##   ### ##   ##                  \n";
        cout << "                ######   ######     ########  ##     ## ##    ## ##    ##                 \n";
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
    return 0;
}