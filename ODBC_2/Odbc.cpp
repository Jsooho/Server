#include "Odbc.h"

bool Odbc::Init()
{
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv) != SQL_SUCCESS)
	{
		return false;
	}
	if (SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		return false;
	}
	if (SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc) != SQL_SUCCESS)
	{
		return false;
	}

	return true;
}
void Odbc::Check()
{
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(m_hEnv, m_hDbc,
		m_hStmt,
		szSQLState,
		&iSQLCode,
		errorBuffer,
		sizeof(errorBuffer),
		&length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}

bool Odbc::Connect(int iType, const TCHAR* dsn)
{
	
	SQLTCHAR OutCon[255];
	SQLSMALLINT cbOutCon;
	TCHAR InCon[256] = { 0, };
	//_stprintf(InCon, 
	//	_T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s;"), dbpath.c_str());
	int iSize = sizeof(OutCon);
	SQLSMALLINT cbOutLen;
	SQLRETURN ret;
	switch (iType)
	{
	case 0: {
		_stprintf(InCon, _T("%s"),
			_T("Driver={SQL Server};Server=directx.kr;Address=192.168.0.44,1433;Network=dbmssocn;Database=GameDb;Uid=sa;Pwd=kgca!@;"));
		ret = SQLDriverConnect(m_hDbc, NULL,
			InCon, _countof(InCon),
			OutCon, _countof(OutCon),
			&cbOutLen, SQL_DRIVER_NOPROMPT);
	}break;

	case 1: {
		_stprintf(InCon, _T("Dsn=%s"), dsn);
		ret = SQLConnect(m_hDbc,
			(SQLTCHAR*)dsn, SQL_NTS,
			(SQLTCHAR*)L"sa", SQL_NTS,
			(SQLTCHAR*)L"kgca!@", SQL_NTS);
	}break;
	case 2: {
		_stprintf(InCon, _T("FileDsn=%s"), dsn);
		ret = SQLDriverConnect(m_hDbc, NULL,
			InCon, _countof(InCon),
			OutCon, _countof(OutCon),
			&cbOutLen, SQL_DRIVER_NOPROMPT);
	}break;
		// access 대화상자 버전
	case 3: {
		HWND hWnd = GetDesktopWindow();
		SQLSMALLINT len;
		ret = SQLDriverConnect(m_hDbc, hWnd,
			(SQLWCHAR*)L"Driver={Microsoft Access Driver (*.mdb, *.accdb)}", SQL_NTS,
			(SQLWCHAR*)InCon, _countof(InCon),
			&len, SQL_DRIVER_PROMPT);
	}break;
		// SQL Server 대화상자 버전
	case 4: {
		HWND hWnd = GetDesktopWindow();
		SQLSMALLINT len;
		ret = SQLDriverConnect(m_hDbc, hWnd,
			(SQLWCHAR*)L"Driver={SQL Server}", SQL_NTS,
			(SQLWCHAR*)InCon, _countof(InCon),
			&len, SQL_DRIVER_PROMPT); // NOPROMPT = 코딩용 , PROMPT = 연결용
	}break;
	};
	
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return false;
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt) != SQL_SUCCESS)
	{
		return false;
	}
	return true;
}

bool Odbc::ExecTableInfo(const TCHAR* szTableName)
{
	TableInfo table;
	table.szTableName = szTableName;
	std::wstring sql = L"select * from ";
	sql += szTableName;
	SQLRETURN ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}

	SQLSMALLINT iNumCols;
	SQLNumResultCols(m_hStmt, &table.iNumcol);
	for (int iCols = 1; iCols <= table.iNumcol; iCols++)
	{
		ColumnInfo column;
		column.icol = iCols;
		int iSize = _countof(column.szColName);
		SQLDescribeCol(m_hStmt,
			iCols,
			column.szColName,
			iSize,
			&column.NameLengthPtr,
			&column.pfSqlType,// 데이터형
			&column.ColumnSizePtr,
			&column.DecimalDigitsPtr, // 10진수 자리수
			&column.pfNullable);// NULL 허용여부
		table.ColumnList.push_back(column);
	}

	SQLLEN lTemp;
	TCHAR szData[100][21] = { 0, };
	int   iData[100];
	Record rData;
	for (int iBind = 0; iBind < table.ColumnList.size(); iBind++)
	{
		switch (table.ColumnList[iBind].pfSqlType)
		{
		case SQL_TYPE_TIMESTAMP:
		{
			Field data;
			data.iDataType = SQL_UNICODE;
			ret = SQLBindCol(m_hStmt, iBind + 1,
				SQL_TYPE_TIMESTAMP,
				&szData[iBind],
				0,
				&lTemp);
			if (ret != SQL_SUCCESS)
			{
				Check();
				return false;
			}
			rData.record.push_back(data);
		}break;
		case SQL_WCHAR:
		case SQL_WVARCHAR: {
			Field data;
			data.iDataType = SQL_UNICODE;
			ret = SQLBindCol(m_hStmt, iBind + 1,
				SQL_UNICODE,
				szData[iBind],
				sizeof(szData[iBind]),
				&lTemp);
			if (ret != SQL_SUCCESS)
			{
				Check();
				return false;
			}
			rData.record.push_back(data);
		}break;
		case SQL_INTEGER: {
			Field data;
			data.iDataType = SQL_INTEGER;
			ret = SQLBindCol(m_hStmt, iBind + 1,
				SQL_INTEGER,
				&iData[iBind],
				0,
				&lTemp);
			if (ret != SQL_SUCCESS)
			{
				Check();
				return false;
			}
			rData.record.push_back(data);
		}break;
		case -7: {
			Field data;
			data.iDataType = SQL_C_ULONG;
			ret = SQLBindCol(m_hStmt, iBind + 1,
				SQL_C_ULONG,
				&iData[iBind],
				0,
				&lTemp);
			if (ret != SQL_SUCCESS)
			{
				Check();
				return false;
			}
			rData.record.push_back(data);
		};
		}
	}
	// SQLFetch() 결과 집합에서 다음 데이터 행 집합을 가져오고 바인딩된 모든 열에 대한 데이터를 반환
	while (SQLFetch(m_hStmt) != SQL_NO_DATA)
	{
		for (int iCol = 0; iCol < table.ColumnList.size(); iCol++)
		{
			rData.record[iCol].iDataType = rData.record[iCol].iDataType;
			if (rData.record[iCol].iDataType == SQL_UNICODE)
			{
				rData.record[iCol].szData = szData[iCol];
			}
			else
			{
				rData.record[iCol].szData = std::to_wstring(iData[iCol]);
			}
		}
		m_StringData.push_back(rData);
		std::wcout << L"ID:" << rData.record[1].szData <<L" ";
		std::wcout << L"PassWord:" << rData.record[2].szData << std::endl;
	}
	SQLCloseCursor(m_hStmt);
	m_TableList.push_back(table);
	
	return true;
}

void Odbc::CreatePrepare()
{
	SQLRETURN Ret;
	Ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_stmtAccount);

	SWORD sReturn = 0;
	SQLLEN cbRetParma = SQL_NTS;

	Ret = SQLBindParameter(m_stmtAccount, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER,
		0, 0, &sReturn, 0, &cbRetParma);
	Ret = SQLBindParameter(m_stmtAccount, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(id), 0, id, sizeof(id), NULL);
	Ret = SQLBindParameter(m_stmtAccount, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(ps), 0, ps, sizeof(ps), NULL);

	TCHAR call[] = L"{?=CALL AccountCreate(?,?)}"; //아이디생성
	Ret = SQLPrepare(m_stmtAccount, call, SQL_NTS);

}
void Odbc::ExecutePrepare()
{
	SQLRETURN Ret;
	SQLWCHAR ID[10] = { 0, };
	std::wcout << L"생성 ID:" << std::endl;
	std::wcin >> ID;
	SQLWCHAR PS[10] = { 0, };
	std::wcout << L"생성 PS:" << std::endl;
	std::wcin >> PS;
	memcpy(id, ID, sizeof(id));
	memcpy(ps, PS, sizeof(ps));

	Ret = SQLExecute(m_stmtAccount);
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return;
	}
	while (SQLMoreResults(m_stmtAccount) != SQL_NO_DATA);
	SQLFreeStmt(m_stmtAccount, SQL_CLOSE);
	SQLCloseCursor(m_stmtAccount);
}
//======================================================================
void Odbc::DeletePrepare()
{
	SQLRETURN Ret;
	Ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_stmtDel);

	SWORD sReturn = 0;
	SQLLEN cbRetParma = SQL_NTS;

	Ret = SQLBindParameter(m_stmtDel, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER,
		0, 0, &sReturn, 0, &cbRetParma);
	Ret = SQLBindParameter(m_stmtDel, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(id), 0, id, sizeof(id), NULL);


	TCHAR call[] = L"{?=CALL DeleteUser(?)}"; //아이디삭제
	Ret = SQLPrepare(m_stmtDel, call, SQL_NTS);
}
void Odbc::ExecuteDelPrepare()
{
	SQLRETURN Ret;
	SQLWCHAR ID[10] = { 0, };
	std::wcout << L"삭제 ID:" << std::endl;
	std::wcin >> ID;
	memcpy(id, ID, sizeof(id));
	
	Ret = SQLExecute(m_stmtDel);
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return;
	}
	while (SQLMoreResults(m_stmtDel) != SQL_NO_DATA);
	SQLFreeStmt(m_stmtDel, SQL_CLOSE);
	SQLCloseCursor(m_stmtDel);
}
//======================================================================
void Odbc::UpDatePrepare()
{
	SQLRETURN Ret;
	Ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_stmtUpdate);

	SWORD sReturn = 0;
	SQLLEN cbRetParma = SQL_NTS;

	Ret = SQLBindParameter(m_stmtUpdate, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER,
		0, 0, &sReturn, 0, &cbRetParma);
	Ret = SQLBindParameter(m_stmtUpdate, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(id), 0, id, sizeof(id), NULL);
	Ret = SQLBindParameter(m_stmtUpdate, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(ps), 0, ps, sizeof(ps), NULL);


	TCHAR call[] = L"{?=CALL UpDateUser(?,?)}"; //비번변경
	Ret = SQLPrepare(m_stmtUpdate, call, SQL_NTS);
}
void Odbc::ExecuteUpDatePrepare()
{
	SQLRETURN Ret;
	SQLWCHAR ID[10] = { 0, };
	std::wcout << L"비밀번호 수정할 ID:" << std::endl;
	std::wcin >> ID;
	SQLWCHAR UpdatePS[10] = { 0, };
	std::wcout << L"수정할 비밀번호 PS:" << std::endl;
	std::wcin >> UpdatePS;
	memcpy(id, ID, sizeof(id));
	memcpy(ps, UpdatePS, sizeof(ps));

	Ret = SQLExecute(m_stmtUpdate);
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return;
	}
	while (SQLMoreResults(m_stmtUpdate) != SQL_NO_DATA);
	SQLFreeStmt(m_stmtUpdate, SQL_CLOSE);
	SQLCloseCursor(m_stmtUpdate);
}
//======================================================================
void Odbc::LoginPrepare()
{
	SQLRETURN Ret;
	Ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_stmtLogin);

	SWORD sReturn = 0;
	SQLLEN cbRetParma = SQL_NTS;

	Ret = SQLBindParameter(m_stmtLogin, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER,
		0, 0, &sReturn, 0, &cbRetParma);
	Ret = SQLBindParameter(m_stmtLogin, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(id), 0, id, sizeof(id), NULL);


	TCHAR call[] = L"{?=CALL LoginUser(?)}"; //아이디삭제
	Ret = SQLPrepare(m_stmtLogin, call, SQL_NTS);
}
void Odbc::Login()
{
	SQLRETURN Ret;
	SQLWCHAR ID[10] = { 0, };
	std::wcout << L"로그인 ID:" << std::endl;
	std::wcin >> ID;
	memcpy(id, ID, sizeof(id));

	Ret = SQLExecute(m_stmtLogin);
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return;
	}
	while (SQLMoreResults(m_stmtLogin) != SQL_NO_DATA);
	SQLFreeStmt(m_stmtLogin, SQL_CLOSE);
	SQLCloseCursor(m_stmtLogin);
	std::wcout << id <<L"로그인 되었습니다." << std::endl;
}
void Odbc::LogoutPrepare()
{
	SQLRETURN Ret;
	Ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_stmtLogout);

	SWORD sReturn = 0;
	SQLLEN cbRetParma = SQL_NTS;

	Ret = SQLBindParameter(m_stmtLogout, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER,
		0, 0, &sReturn, 0, &cbRetParma);
	Ret = SQLBindParameter(m_stmtLogout, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
		sizeof(id), 0, id, sizeof(id), NULL);


	TCHAR call[] = L"{?=CALL LogoutUser(?)}"; //아이디삭제
	Ret = SQLPrepare(m_stmtLogout, call, SQL_NTS);
}
void Odbc::Logout()
{
	SQLRETURN Ret;
	SQLWCHAR ID[10] = { 0, };
	std::wcout << L"로그아웃 ID:" << std::endl;
	std::wcin >> ID;
	memcpy(id, ID, sizeof(id));

	Ret = SQLExecute(m_stmtLogout);
	if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return;
	}
	while (SQLMoreResults(m_stmtLogout) != SQL_NO_DATA);
	SQLFreeStmt(m_stmtLogout, SQL_CLOSE);
	SQLCloseCursor(m_stmtLogout);
	std::wcout << id << L"로그아웃 되었습니다." << std::endl;
}
bool Odbc::Release()
{
	SQLCloseCursor(m_hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	SQLDisconnect(m_hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
	return true;
}