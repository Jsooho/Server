#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<string>
#include<tchar.h>
#include<sql.h>
#include<sqlext.h>
#include<iostream>

// [문서 341쪽]
//마커    = SQL문 스트링에는 매개변수 마커가 포함될 수 있다. "?" 문자로 표시되며 
//          SQLExecute()가 호출될 때, 어플리케이션 변수 값이 대체되어야 할 명령문의 위치를 지정한다.
// SQLBindParam()은 어플리케이션 변수를 각 매개변수 마커에 바인드(연관)시켜서 자료가 전송될 때 자료 변환이 수행되어야 하는지를 표시한다
// 

//트랜잭션 = a부터z까지 작업이 완료되야 모든작업이 끝나는것인데, 중간에 오류가나면 롤백하는 것!

//환경핸들 = 환경에 저장되는 정보는 현재 상태,진단정보,환경의 속성등과 연결에 관련된 정보들이 있다
SQLHENV  g_hEnv; 

//연결핸들 =데이터 소스에 연결  및 해제하는 것이 가장 주된 기능이다. 또한 데이터 소스나 드라이버에 관한 정보를 조사하거나 트랜잭션 처리에도 사용된다.
//연결핸들속성 = 로그인 타임 아웃 설정, 패킷 크기, 비동기 액세스 모드, 트랙젝션 모드 지정( SQLSetConnectAttr() 함수 사용 )
SQLHDBC  g_hDbc; 

//명령핸들 = 데이터 소스에 연결한 후 원하는 데이터를 액세스하기 위한 SQL 명령을 저장하며 그 결과셋과 SQL문의 파라미터 정보를 가진다.
//명령핸들 속성 = 커서의 종류, 쿼리 타임아웃값, 결과셋의 레코드 개수등의 속성을 갖고 지정할 수 있으며 SQLSetStmtAttr()함수를 사용한다.
SQLHSTMT g_hStmt; 

void Check()
{
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(g_hEnv, g_hDbc,
		g_hStmt,
		szSQLState,
		&iSQLCode,
		errorBuffer,
		sizeof(errorBuffer),
		&length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}

void main()
{
	setlocale(LC_ALL, "");
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv) != SQL_SUCCESS) // 환경핸들 할당
	{
		return;
	}

	//환경속성 핸들, 속성의 종류 지정(버전), 속성 값 지정, 속성 값의 길이
	if (SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER) != SQL_SUCCESS) //환경핸들 설정
	{
		Check();
		return;
	}
	
	
	if (SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc) != SQL_SUCCESS) //연결핸들 할당
	{
		return;
	}

	SQLWCHAR dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);

	std::wstring dbpath = dir;
	dbpath += L"\\cigarette.mdb";

	TCHAR InCon[256] = { 0, };
	_stprintf(InCon,
		_T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s;"), dbpath.c_str());

	SQLSMALLINT cbOutLen;
	//연결 핸들을 할당하면 실제 데이터 소스와 연결할 수 있다.
	SQLRETURN ret = SQLDriverConnect(g_hDbc, NULL, InCon, _countof(InCon), NULL, 0, &cbOutLen, SQL_DRIVER_NOPROMPT);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return;
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt) != SQL_SUCCESS) //명령핸들 할당
	{
		return;
	}
	SQLLEN lName, lPrice, lKorean;
	TCHAR Name[21];
	
	int Price;
	BOOL Korean;
	//결과
	
	//첫번째 필드로 각각의 레코드를 리턴해준다.
	ret = SQLBindCol(g_hStmt, 1,SQL_UNICODE, Name, sizeof(Name), &lName); 
	ret = SQLBindCol(g_hStmt, 2, SQL_C_ULONG, &Price, 0, &lPrice);
	ret = SQLBindCol(g_hStmt, 3, SQL_C_ULONG, &Korean, 0, &lKorean);

	//tblCigar에 모든데이터(*)를 찾아달라
	TCHAR sql[MAX_PATH] = L"select * from tblCigar";
	
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << Name << " " << Price << " " << Korean << std::endl;
	}
	SQLCloseCursor(g_hStmt);

	TCHAR sql2[MAX_PATH] = { 0, }; // L"select name,price,korean from tblCigar ='%s'";
	//cigar테이블에서 이름이 장미에대한 정보를 주세요!
	wsprintf(sql2, L"select name,price,korean from tblCigar where name='%s'", L"장미"); 
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql2, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << Name << " " << Price << " " << Korean;
	}
	SQLCloseCursor(g_hStmt);

	TCHAR sql3[MAX_PATH] = { 0, }; // L"select name,price,korean from tblCigar ='%s'";
	//테이블에 추가해주세요
	wsprintf(sql3, L"insert into tblCigar (name,price,korean) VALUE ('%s',%d,%d)", L"레종", 4500, 1);
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql3, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA) 
	{
		std::wcout << Name << " " << Price << " " << Korean;
	}
	SQLCloseCursor(g_hStmt);

	SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	SQLDisconnect(g_hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
	
}