#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<string>
#include<tchar.h>
#include<sql.h>
#include<sqlext.h>
#include<iostream>

// [���� 341��]
//��Ŀ    = SQL�� ��Ʈ������ �Ű����� ��Ŀ�� ���Ե� �� �ִ�. "?" ���ڷ� ǥ�õǸ� 
//          SQLExecute()�� ȣ��� ��, ���ø����̼� ���� ���� ��ü�Ǿ�� �� ��ɹ��� ��ġ�� �����Ѵ�.
// SQLBindParam()�� ���ø����̼� ������ �� �Ű����� ��Ŀ�� ���ε�(����)���Ѽ� �ڷᰡ ���۵� �� �ڷ� ��ȯ�� ����Ǿ�� �ϴ����� ǥ���Ѵ�
// 

//Ʈ����� = a����z���� �۾��� �Ϸ�Ǿ� ����۾��� �����°��ε�, �߰��� ���������� �ѹ��ϴ� ��!

//ȯ���ڵ� = ȯ�濡 ����Ǵ� ������ ���� ����,��������,ȯ���� �Ӽ���� ���ῡ ���õ� �������� �ִ�
SQLHENV  g_hEnv; 

//�����ڵ� =������ �ҽ��� ����  �� �����ϴ� ���� ���� �ֵ� ����̴�. ���� ������ �ҽ��� ����̹��� ���� ������ �����ϰų� Ʈ����� ó������ ���ȴ�.
//�����ڵ�Ӽ� = �α��� Ÿ�� �ƿ� ����, ��Ŷ ũ��, �񵿱� �׼��� ���, Ʈ������ ��� ����( SQLSetConnectAttr() �Լ� ��� )
SQLHDBC  g_hDbc; 

//����ڵ� = ������ �ҽ��� ������ �� ���ϴ� �����͸� �׼����ϱ� ���� SQL ����� �����ϸ� �� ����°� SQL���� �Ķ���� ������ ������.
//����ڵ� �Ӽ� = Ŀ���� ����, ���� Ÿ�Ӿƿ���, ������� ���ڵ� �������� �Ӽ��� ���� ������ �� ������ SQLSetStmtAttr()�Լ��� ����Ѵ�.
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
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv) != SQL_SUCCESS) // ȯ���ڵ� �Ҵ�
	{
		return;
	}

	//ȯ��Ӽ� �ڵ�, �Ӽ��� ���� ����(����), �Ӽ� �� ����, �Ӽ� ���� ����
	if (SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER) != SQL_SUCCESS) //ȯ���ڵ� ����
	{
		Check();
		return;
	}
	
	
	if (SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc) != SQL_SUCCESS) //�����ڵ� �Ҵ�
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
	//���� �ڵ��� �Ҵ��ϸ� ���� ������ �ҽ��� ������ �� �ִ�.
	SQLRETURN ret = SQLDriverConnect(g_hDbc, NULL, InCon, _countof(InCon), NULL, 0, &cbOutLen, SQL_DRIVER_NOPROMPT);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return;
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt) != SQL_SUCCESS) //����ڵ� �Ҵ�
	{
		return;
	}
	SQLLEN lName, lPrice, lKorean;
	TCHAR Name[21];
	
	int Price;
	BOOL Korean;
	//���
	
	//ù��° �ʵ�� ������ ���ڵ带 �������ش�.
	ret = SQLBindCol(g_hStmt, 1,SQL_UNICODE, Name, sizeof(Name), &lName); 
	ret = SQLBindCol(g_hStmt, 2, SQL_C_ULONG, &Price, 0, &lPrice);
	ret = SQLBindCol(g_hStmt, 3, SQL_C_ULONG, &Korean, 0, &lKorean);

	//tblCigar�� ��絥����(*)�� ã�ƴ޶�
	TCHAR sql[MAX_PATH] = L"select * from tblCigar";
	
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << Name << " " << Price << " " << Korean << std::endl;
	}
	SQLCloseCursor(g_hStmt);

	TCHAR sql2[MAX_PATH] = { 0, }; // L"select name,price,korean from tblCigar ='%s'";
	//cigar���̺��� �̸��� ��̿����� ������ �ּ���!
	wsprintf(sql2, L"select name,price,korean from tblCigar where name='%s'", L"���"); 
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql2, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << Name << " " << Price << " " << Korean;
	}
	SQLCloseCursor(g_hStmt);

	TCHAR sql3[MAX_PATH] = { 0, }; // L"select name,price,korean from tblCigar ='%s'";
	//���̺� �߰����ּ���
	wsprintf(sql3, L"insert into tblCigar (name,price,korean) VALUE ('%s',%d,%d)", L"����", 4500, 1);
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