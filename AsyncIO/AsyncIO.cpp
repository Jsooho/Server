//��������µ� �񵿱� �Ҽ��ִ�
#include<Windows.h>
#include<iostream>

using namespace std;



OVERLAPPED g_ReadOV = { 0, };//��ŭ �о��ٶ�� ���� �ü���� �����ϰ� ó�����ش�, �߰������ ����� ��Ȯ�Ѱ��� �ȶ������� 
OVERLAPPED g_WriteOV = { 0, };
LARGE_INTEGER  filesize; // LARGE_INTEGER = LONGLONG �ڷ���
WCHAR* g_Buffer = NULL;

void main()
{
    setlocale(LC_ALL, "KOREAN"); //�����ڵ� �ܼ�â�� �ѱ� ��� �Ҷ� ���

    // CREATE_ALWAYS = ���ο� ������ ����� ���� ������ ������ �����ϸ� ���� ������ �����ϰ� �ٽ� �����.

    HANDLE hWFile = CreateFile(L"test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //���� ���� , ��ȯ�� : ����(INVALID_HANDLE_VALUE)

    if (hWFile != NULL)
    {
        WCHAR Buffer[] = L"����!";
        DWORD dwSize = sizeof(Buffer);
        DWORD dwWritten;
        BOOL Ret = WriteFile(hWFile, Buffer, dwSize, &dwWritten, NULL);
       // LPOVERLAPPED lpOverlapped = (����������) ��� ������ ���� �񵿱⸦ �����Ҽ��ִ�.
        if (Ret == TRUE)
        {
            wcout << L"����� ����" << endl;
        }
    }

    CloseHandle(hWFile);

    HANDLE hFileRead = CreateFile(L"test.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //���� ���� , ��ȯ�� : ����(INVALID_HANDLE_VALUE)

    if (hFileRead != NULL)
    {
        WCHAR Buffer[512] = { 0, };
        DWORD dwSize = sizeof(Buffer);
        DWORD dwRead;
        BOOL ret = ReadFile(hFileRead, Buffer, dwSize, &dwRead, NULL);
        //LPOVERLAPPED lpOverlapped = (����������) ��� ������ ���� �񵿱⸦ �����Ҽ��ִ�.
        if (ret == TRUE)
        {
            wcout << Buffer << endl;
        }
    }

    CloseHandle(hFileRead);
    //===================================================================================================
    //�񵿱� �����

    //HANDLE hFileAsync = CreateFile(L"yy.zip", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 
    //    FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, &g_ReadOV); //�񵿱� �Ҷ��� �������� �Ѵ� FILE_FLAG_OVERLAPPED
    //
    //if (hFileAsync != INVALID_HANDLE_VALUE)
    //{
    //    //GetFileSize(hFile, &filesize);
    //    GetFileSizeEx(hFileAsync, &filesize);

    //    g_Buffer = new WCHAR[filesize.QuadPart];
    //    
    //    DWORD dwRead;
    //    DWORD dwWritten;
    //    BOOL Return = ReadFile(hFileAsync, g_Buffer, filesize.QuadPart, &dwRead, &g_ReadOV);
    //    // LPOVERLAPPED lpOverlapped = (����������) ��� ������ ���� �񵿱⸦ �����Ҽ��ִ�.
    //    BOOL bPanding = FALSE;
    //    if (Return == FALSE)
    //    {
    //        DWORD Error = GetLastError();
    //        if (Error == ERROR_IO_PENDING) // ���а� �ƴϰ� �񵿱� ������� �Ϸ� ��� �ǹ�
    //        {
    //            wcout << L"�б� ��" << endl;
    //            bPanding = TRUE;
    //        }
    //        else
    //        {
    //            wcout << L"�б� ����" << endl;
    //            bPanding = FALSE;
    //        }
    //    }
    //    else
    //    {
    //        wcout << L"�ε� �Ϸ�" << endl;
    //    }
    //    //�ε� ��
    //    DWORD dwTrans = 0;
    //    BOOL  bret;
    //    while(bPanding)
    //    {
    //        //������ ���� Wait = TRUE�� ���� �۾��� �ϰ� �Ǹ� FALSE�� ��쿡 �񵿱� �۾��� �ȴ�. 
    //        bret = GetOverlappedResult(hFileAsync, &g_ReadOV, &dwTrans, FALSE);//����� ��ȯ���� , ������ ���� �˷���������  
    //       
    //        if (bret == TRUE)
    //        {
    //            wcout << L"ok" << endl;
    //            bPanding = FALSE;
    //        }
    //        else
    //        {
    //            DWORD Error = GetLastError();
    //            if (Error == ERROR_IO_INCOMPLETE)
    //            {
    //                wcout << L"�б� ��" <<  endl;
    //                bPanding = TRUE;
    //            }
    //            else
    //            {
    //                wcout << L"�б� ����" << endl;
    //            }
    //        }
    //    }
    //}

    //CloseHandle(hFileAsync);

    //wcout << "���� �մϴ�" << endl;
    //HANDLE hWFile = CreateFile(L"Copy.zip", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED |FILE_FLAG_WRITE_THROUGH, NULL); //���� ���� , ��ȯ�� : ����(INVALID_HANDLE_VALUE)

    //if (hWFile != NULL)
    //{
    //    DWORD dwLen = filesize.QuadPart;
    //    DWORD dwWritten;
    //    BOOL Ret = WriteFile(hWFile, g_Buffer, dwLen, &dwWritten, &g_WriteOV);
    //   // LPOVERLAPPED lpOverlapped = (����������) ��� ������ ���� �񵿱⸦ �����Ҽ��ִ�.
    //    BOOL bPanding = FALSE;
    //    if (Ret == FALSE)
    //    {
    //        DWORD Error = GetLastError();
    //        if (Error == ERROR_IO_PENDING) // ���а� �ƴϰ� �񵿱� ������� �Ϸ� ��� �ǹ�
    //        {
    //            wcout << L"��� ��" << endl;
    //            bPanding = TRUE;
    //        }
    //        else
    //        {
    //            wcout << L"��� ����" << endl;
    //            
    //        }
    //    }
    //    else
    //    {
    //        wcout << L"��� �Ϸ�" << endl;
    //        bPanding = FALSE;
    //    }
    //    
    //    DWORD dwTrans = 0;
    //    BOOL  bret;
    //    while (bPanding)
    //    {
    //        //������ ���� Wait = TRUE�� ���� �۾��� �ϰ� �Ǹ� FALSE�� ��쿡 �񵿱� �۾��� �ȴ�. 
    //        bret = GetOverlappedResult(hFileAsync, &g_WriteOV, &dwTrans, FALSE);//����� ��ȯ���� , ������ ���� �˷���������  

    //        if (bret == TRUE)
    //        {
    //            wcout << L"ok" << endl;
    //            bPanding = FALSE;
    //        }
    //        else
    //        {
    //            DWORD Error = GetLastError();
    //            if (Error == ERROR_IO_INCOMPLETE)
    //            {
    //                wcout << L"��� ��..." << endl;
    //            }
    //            else
    //            {
    //                wcout << L"�б� ����" << endl;
    //                bPanding = FALSE;
    //            }
    //        }
    //        Sleep(1000);
    //    }
    //}

    //CloseHandle(hWFile);
}
   
    
