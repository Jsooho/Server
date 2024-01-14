#include "Odbc.h"

void main()
{
	int iSelect = 0;
	bool m_bRun = true;
	setlocale(LC_ALL, "");
	Odbc odbc;
	odbc.Init();

	SQLWCHAR dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);
	std::wstring dbpath = dir;
	dbpath += L"\\test.dsn";

	odbc.Connect(1, L"test"/*dbpath.c_str()*/);
	

	while (m_bRun)
	{
		std::cout << "0번: 계정생성, 1번: 계정삭제, 2번: 비밀번호수정, 3번: 전체계정 조회, 4번: 로그인, 5번: 로그아웃, 6번: 종료" << std::endl;
		std::cin >> iSelect;
		system("cls");
		if (iSelect == 6)
		{
			break;
			m_bRun = false;
		}
		switch (iSelect)
		{
		case 0:
		{
			odbc.CreatePrepare();
			odbc.ExecutePrepare();
			
		}break;
		case 1:
		{
			odbc.DeletePrepare();
			odbc.ExecuteDelPrepare();
		}break;
		case 2:
		{
			odbc.UpDatePrepare();
			odbc.ExecuteUpDatePrepare();
		}break;
		case 3:
		{
			odbc.ExecTableInfo(L"Userlist");
		}break;
		case 4:
		{
			odbc.LoginPrepare();
			odbc.Login();
		}break;
		case 5:
		{
			odbc.LogoutPrepare();
			odbc.Logout();
		}break;
		}
	}
	odbc.Release();
}
