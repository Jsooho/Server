#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include <vector>
#include <sql.h>
#include <sqlext.h>


void Error(SQLHENV env, SQLHDBC dbc, SQLHSTMT stmt)
{
	SQLTCHAR buffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLTCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
	SQLINTEGER sqlcode;
	SQLSMALLINT length;
	while (SQLError(env, dbc, stmt,
		sqlstate, &sqlcode,
		buffer, SQL_MAX_MESSAGE_LENGTH + 1,
		&length) == SQL_SUCCESS)
	{
		printf("%s", buffer);
	}
}
//추가 삭제 수정 조회 = 추가완, 삭제완, 수정완,
//프로시저 2개이상
SQLHENV  hEnv;
SQLHDBC  hDbc;
SQLHSTMT hStmt = SQL_NULL_HSTMT;
SQLHSTMT g_stmtAccount = SQL_NULL_HSTMT;
SQLHSTMT g_stmtInfo = SQL_NULL_HSTMT;
SQLHSTMT g_stmtDel = SQL_NULL_HSTMT;
SQLHSTMT g_stmtUpdate = SQL_NULL_HSTMT;
SQLHSTMT g_stmtSelect = SQL_NULL_HSTMT;
SQLRETURN Ret;

struct dbBinding
{
	SQLSMALLINT type;
	SQLWCHAR*   valuePtr;
	SQLINTEGER  len;
	SQLLEN      etc;
};

SQLWCHAR id[10];
SQLWCHAR ps[10];
//======================================================================
void CreatePrepare()
{
	Ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &g_stmtAccount);

	SWORD sReturn = 0;
	SQLLEN cbRetParma = SQL_NTS;

	Ret = SQLBindParameter(g_stmtAccount, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER,
		0, 0, &sReturn, 0, &cbRetParma);
	Ret = SQLBindParameter(g_stmtAccount, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(id), 0, id, sizeof(id), NULL);
	Ret = SQLBindParameter(g_stmtAccount, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(ps), 0, ps, sizeof(ps), NULL);

	TCHAR call[] = L"{?=CALL AccountCreate(?,?)}"; //아이디생성
	Ret = SQLPrepare(g_stmtAccount, call, SQL_NTS);

}
void ExecutePrepare(const TCHAR* szid, const TCHAR* szps)
{
	memcpy(id, szid, sizeof(id));
	memcpy(ps, szps, sizeof(ps));
	
	Ret = SQLExecute(g_stmtAccount);
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		Error(hEnv, hDbc, g_stmtAccount);
		return;
	}
	while (SQLMoreResults(g_stmtAccount) != SQL_NO_DATA);
	SQLFreeStmt(g_stmtAccount, SQL_CLOSE);
	SQLCloseCursor(g_stmtAccount);
}
//======================================================================
void DeletePrepare()
{
	Ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &g_stmtDel);

	SWORD sReturn = 0;
	SQLLEN cbRetParma = SQL_NTS;

	Ret = SQLBindParameter(g_stmtDel, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER,
		0, 0, &sReturn, 0, &cbRetParma);
	Ret = SQLBindParameter(g_stmtDel, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(id), 0, id, sizeof(id), NULL);
	

	TCHAR call[] = L"{?=CALL DeleteUser(?)}"; //아이디삭제
	Ret = SQLPrepare(g_stmtDel, call, SQL_NTS);
}
void ExecuteDelPrepare(const TCHAR* szid)
{
	memcpy(id, szid, sizeof(id));

	Ret = SQLExecute(g_stmtDel);
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		Error(hEnv, hDbc, g_stmtDel);
		return;
	}
	while (SQLMoreResults(g_stmtDel) != SQL_NO_DATA);
	SQLFreeStmt(g_stmtDel, SQL_CLOSE);
	SQLCloseCursor(g_stmtDel);
}
//======================================================================
void UpDatePrepare()
{
	Ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &g_stmtUpdate);

	SWORD sReturn = 0;
	SQLLEN cbRetParma = SQL_NTS;

	Ret = SQLBindParameter(g_stmtUpdate, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER,
		0, 0, &sReturn, 0, &cbRetParma);
	Ret = SQLBindParameter(g_stmtUpdate, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(id), 0, id, sizeof(id), NULL);
	Ret = SQLBindParameter(g_stmtUpdate, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(ps), 0, ps, sizeof(ps), NULL);
	
	
	TCHAR call[] = L"{?=CALL UpDateUser(?,?)}"; //비번변경
	Ret = SQLPrepare(g_stmtUpdate, call, SQL_NTS);
}
void ExecuteUpDatePrepare(const TCHAR* szid, const TCHAR* szps)
{
	memcpy(id, szid, sizeof(id));
	memcpy(ps, szps, sizeof(ps));

	Ret = SQLExecute(g_stmtUpdate);
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		Error(hEnv, hDbc, g_stmtUpdate);
		return;
	}
	while (SQLMoreResults(g_stmtUpdate) != SQL_NO_DATA);
	SQLFreeStmt(g_stmtUpdate, SQL_CLOSE);
	SQLCloseCursor(g_stmtUpdate);
}
//======================================================================

bool SQLSuccess(SQLRETURN ret)
{
	return(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}

void GetDBInfo()
{
	WCHAR szVer[20] = { 0, };
	SQLSMALLINT cch = 0;
	Ret = SQLGetInfo(hDbc, SQL_DRIVER_ODBC_VER, szVer, _countof(szVer), &cch); //->드라이버 확인
	int iOdbcMajor;
	int iOdbcMinor;

	if (SQL_SUCCEEDED(Ret))
	{

	}

	//조회 
	SQLWCHAR dbName[1024] = { 0, };
	SQLWCHAR dbmsVer[1024] = { 0, };
	SQLWCHAR dbmsName[1024] = { 0, };
	SQLWCHAR UserName[1024] = { 0, };

	Ret = SQLGetInfo(hDbc, SQL_DATABASE_NAME, dbName, _countof(dbName), &cch);
	if (SQL_SUCCEEDED(Ret)) {}

	Ret = SQLGetInfo(hDbc, SQL_DBMS_NAME, dbmsName, _countof(dbmsName), &cch);
	if (SQL_SUCCEEDED(Ret)) {}

	Ret = SQLGetInfo(hDbc, SQL_DBMS_VER, dbmsVer, _countof(dbmsVer), &cch);
	if (SQL_SUCCEEDED(Ret)) {}

	Ret = SQLGetInfo(hDbc, SQL_USER_NAME, UserName, _countof(UserName), &cch);
	if (SQL_SUCCEEDED(Ret)) {}

	Ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &g_stmtInfo);
	dbBinding dbResult[5];

	for (int i = 0; i < 5; i++)
	{
		dbResult[i].type = SQL_UNICODE;
		dbResult[i].len = 1024 + 1;
		dbResult[i].valuePtr = new WCHAR[dbResult[i].len];

		SQLBindCol(
			hStmt,
			i + 1, 
			dbResult[i].type, 
			(SQLPOINTER)dbResult[i].valuePtr, 
			dbResult[i].len, 
			&dbResult[i].etc);
	}
	Error(hEnv, hDbc, g_stmtInfo);
	Ret = SQLTables(g_stmtInfo, dbName, SQL_NTS, UserName, SQL_NTS,
		     (SQLWCHAR*)SQL_ALL_TABLE_TYPES, 
		     SQL_NTS, (SQLWCHAR*)L"'TABLE'", SQL_NTS);
	for (Ret = SQLFetch(g_stmtInfo); SQLSuccess(Ret); Ret = SQLFetch(g_stmtInfo))
	{
		std::wstring szData = (WCHAR*)dbResult[0].valuePtr;
		szData += L" ";
		szData += (WCHAR*)dbResult[1].valuePtr;
		szData += L" ";
		szData += (WCHAR*)dbResult[2].valuePtr;
		szData += L" ";
		szData += (WCHAR*)dbResult[3].valuePtr;
		std::wcout << szData.c_str() << std::endl;
	}
	SQLCloseCursor(g_stmtInfo);
	SQLFreeStmt(g_stmtInfo, SQL_UNBIND);

	for (int i = 0; i < 5; i++)
	{
		delete dbResult[i].valuePtr;
	}
}

void main()
{

	
	setlocale(LC_ALL, "");
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) // 환경핸들 할당
	{
		return;
	}
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, 0/*SQL_IS_INTEGER */ ) != SQL_SUCCESS) //환경핸들 설정
	{
		return;
	}
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) //연결핸들 할당
	{
		return;
	}
	

	//HWND hWnd = GetDesktopWindow();
	SQLSMALLINT len;
	SQLRETURN ret;
	TCHAR InCon[512] = { 0, };

	ret = SQLConnect(hDbc, (SQLWCHAR*)L"test", 
		SQL_NTS, (SQLWCHAR*)L"sa", SQL_NTS, (SQLWCHAR*)L"kgca!@", SQL_NTS);
		

	/*ret = SQLDriverConnect(hDbc, hWnd,
		(SQLWCHAR*)L"Driver={SQL Server}", SQL_NTS,
		(SQLWCHAR*)InCon, _countof(InCon),
		&len, SQL_DRIVER_PROMPT);*/
	
	CreatePrepare();
	ExecutePrepare(L"zzzzx", L"cxcxc");
	ExecutePrepare(L"zzx", L"cxxc");
	DeletePrepare();
	ExecuteDelPrepare(L"zzzzx");
	UpDatePrepare();
	ExecuteUpDatePrepare(L"zzx", L"xxx");
	ExecuteUpDatePrepare(L"dsf", L"xxxx");



	

	

	//GetDBInfo();

	/// <summary>
	///
	//SQLHSTMT hStmt1 = SQL_NULL_HSTMT;
	//SQLRETURN retcode;
	//retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt1);

	//SWORD sReturn1 = 0;
	//SQLLEN cbRetParam1 = SQL_NTS;
	//retcode = SQLBindParameter(hStmt1, 1, SQL_PARAM_OUTPUT,
	//	SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sReturn1, 0, &cbRetParam1);

	//
	//retcode = SQLBindParameter(hStmt1, 2, SQL_PARAM_INPUT,
	//	SQL_C_WCHAR, SQL_WVARCHAR, sizeof(id), 0, id, sizeof(id), NULL);

	///*SQLWCHAR outps[10] = { 0, };
	//retcode = SQLBindParameter(hStmt1, 3, SQL_PARAM_OUTPUT,
	//	SQL_C_WCHAR, SQL_WVARCHAR, sizeof(outps), 0, outps, sizeof(outps), NULL);*/

	///*SQLWCHAR inps[10] = L"sssd";
	//retcode = SQLBindParameter(hStmt1, 3, SQL_PARAM_INPUT,
	//	SQL_C_WCHAR, SQL_WVARCHAR, sizeof(inps), 0, inps, sizeof(inps), NULL);*/

	//TCHAR callsp1[] = L"{?=call GameUserInfo(?)}"; //패스워드 확인
	////TCHAR callsp1[] = L"{?=call LoginUser(?)}"; // 로그인
	////TCHAR callsp1[] = L"{?=call LogoutUser(?)}"; // 로그아웃
	//retcode = SQLPrepare(hStmt1, callsp1, SQL_NTS);
	////retcode = SQLExecDirect(hstmt,callsp1, SQL_NTS);
	//retcode = SQLExecute(hStmt1);
	//if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	//{
	//	Error(hEnv,hDbc,hStmt1);
	//	return;
	//}
	//while (SQLMoreResults(hStmt1) != SQL_NO_DATA)
	//{}
	//SQLFreeStmt(hStmt1, SQL_UNBIND);
	//SQLFreeStmt(hStmt1, SQL_RESET_PARAMS);
	//SQLCloseCursor(hStmt1);
	/// </summary>
	return;
}