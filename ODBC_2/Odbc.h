#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include <vector>
#include <sql.h>
#include <sqlext.h>

struct ColumnInfo
{
	SQLUSMALLINT        icol;
	SQLWCHAR			szColName[20];
	SQLSMALLINT         BufferLength;
	SQLSMALLINT			NameLengthPtr;
	SQLSMALLINT			pfSqlType;
	SQLULEN				ColumnSizePtr;
	SQLSMALLINT			DecimalDigitsPtr;
	SQLSMALLINT			pfNullable;
};
struct TableInfo
{
	std::vector<ColumnInfo>	ColumnList;
	SQLSMALLINT 			iNumcol;
	std::wstring			szTableName;
};
struct Field
{
	std::wstring	szData;
	int				iDataType;
};
struct Record
{
	std::vector<Field>  record;
};
class Odbc
{

public:
	SQLHENV		m_hEnv;
	SQLHDBC		m_hDbc;
	SQLHSTMT	m_hStmt;
	SQLHSTMT	m_stmtAccount;
	SQLHSTMT	m_stmtDel;
	SQLHSTMT	m_stmtUpdate;
	SQLHSTMT	m_stmtLogin;
	SQLHSTMT	m_stmtLogout;
	std::vector<TableInfo>	m_TableList;
	std::vector<Record>     m_StringData;
	SQLWCHAR	id[10];
	SQLWCHAR	ps[10];
public:
	bool		Init();
	bool		Connect(int iType, const TCHAR* dsn);
	bool		Release();
	bool		ExecTableInfo(const TCHAR* szTableName);
	void		Check();
public:
	//생성=========================================================================
	void CreatePrepare();
	void ExecutePrepare();
	//삭제=========================================================================
	void DeletePrepare();
	void ExecuteDelPrepare();
	//수정=========================================================================
	void UpDatePrepare();
	void ExecuteUpDatePrepare();
	//로그인========================================================================
	void LoginPrepare();
	void Login();
	//로그아웃======================================================================
	void LogoutPrepare();
	void Logout();
	//==============================================================================
};

