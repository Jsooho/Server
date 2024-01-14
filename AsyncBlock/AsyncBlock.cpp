#include<Windows.h>
#include<iostream>
#define BlockSize 2048
using namespace std;


void main()
{
    setlocale(LC_ALL, "KOREAN");

    OVERLAPPED g_ReadOV = { 0, };//��ŭ �о��ٶ�� ���� �ü���� �����ϰ� ó�����ش�, �߰������ ����� ��Ȯ�Ѱ��� �ȶ������� 
    OVERLAPPED g_WriteOV = { 0, };
    LARGE_INTEGER  filesize; // LARGE_INTEGER = LONGLONG �ڷ���
    WCHAR* g_Buffer = NULL;

    HANDLE hFileAsync = CreateFile(L"API_1_2.zip", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, &g_ReadOV); //�񵿱� �Ҷ��� �������� �Ѵ� FILE_FLAG_OVERLAPPED

    if (hFileAsync == INVALID_HANDLE_VALUE)
    {
        return;
    }

    HANDLE hFileCopy = CreateFile(L"Copy.zip", GENERIC_WRITE, 0, NULL, 
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

    if (hFileCopy == INVALID_HANDLE_VALUE)
    {
        return;
    }
    GetFileSizeEx(hFileAsync, &filesize);

    DWORD dwRead;
    DWORD dwWrite;
    DWORD dwTotal = 0;
    g_Buffer = new WCHAR[filesize.QuadPart];
    //g_Buffer = new WCHAR[BlockSize];
    for (int i = 0; ; i++)

    {
        ZeroMemory(g_Buffer, filesize.QuadPart);

        g_ReadOV.Offset = i * 2048;
        g_ReadOV.OffsetHigh = 0;
        BOOL bret = ReadFile(hFileAsync, g_Buffer, filesize.QuadPart, &dwRead, &g_ReadOV);
        if (bret == FALSE)
        {
           DWORD Error = GetLastError();
            if (Error == ERROR_IO_PENDING)
            {
                wcout << L"�д� ��" << endl;
            }
            else
            {
                wcout << L"�б� ����" << endl;
            }
        }
        else
        {
            wcout << L"�б� ����" << endl;
        }
        BOOL breturn = GetOverlappedResult(hFileAsync, &g_ReadOV, &dwRead, FALSE);
        if (breturn == TRUE)
        {
            wcout << L".";
        }
        else
        {
            DWORD Error = GetLastError();
            if (Error == ERROR_IO_INCOMPLETE)
            {
                wcout << L"�б� ��" << endl;
            }
            else
            {
                wcout << L"�б� ����" << endl;
            }
        }
        wcout << L"���� ����" << endl;
        g_WriteOV.Offset = i * 2048;
        g_WriteOV.OffsetHigh = 0;
        bret = WriteFile(hFileCopy, g_Buffer, dwRead, &dwWrite, &g_WriteOV);
        if (bret == FALSE)
        {
            DWORD Error = GetLastError();
            if (Error == ERROR_IO_PENDING)
            {
                wcout << L"��� ��" << endl;
            }
            else
            {
                wcout << L"��� ����" << endl;
            }
        }
        else
        {
            wcout << L"��� ����" << endl;
        }
        breturn = GetOverlappedResult(hFileCopy, &g_ReadOV, &dwWrite, FALSE);
        if (breturn == TRUE)
        {
            wcout << L".";
        }
        else
        {
            DWORD Error = GetLastError();
            if (Error == ERROR_IO_INCOMPLETE)
            {
                wcout << L"��� ��" << endl;
            }
            else
            {
                wcout << L"��� ����" << endl;
            }
        }
        dwTotal += dwWrite;

        if (filesize.LowPart == dwTotal)
        {
            wcout << L"���� �Ϸ�" << endl;
            break;
        }
        Sleep(1000);
       
    }
    CloseHandle(hFileAsync);
    CloseHandle(hFileCopy);
}
//    wcout << L"���� �մϴ�" << endl;
//    HANDLE hWFile = CreateFile(L"Copy.zip", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_WRITE_THROUGH, NULL); //���� ���� , ��ȯ�� : ����(INVALID_HANDLE_VALUE)
//
//    if (hWFile != NULL)
//    {
//        DWORD dwLen = filesize.QuadPart;
//        DWORD dwWritten;
//        BOOL Ret = WriteFile(hWFile, g_Buffer, dwLen, &dwWritten, &g_WriteOV);
//        // LPOVERLAPPED lpOverlapped = (����������) ��� ������ ���� �񵿱⸦ �����Ҽ��ִ�.
//        BOOL bPanding = FALSE;
//        if (Ret == FALSE)
//        {
//            DWORD Error = GetLastError();
//            if (Error == ERROR_IO_PENDING) // ���а� �ƴϰ� �񵿱� ������� �Ϸ� ��� �ǹ�
//            {
//                wcout << L"��� ��" << endl;
//                bPanding = TRUE;
//            }
//            else
//            {
//                wcout << L"��� ����" << endl;
//
//            }
//        }
//        else
//        {
//            wcout << L"��� �Ϸ�" << endl;
//            bPanding = FALSE;
//        }
//
//        DWORD dwTrans = 0;
//        BOOL  bret;
//        while (bPanding)
//        {
//            //������ ���� Wait = TRUE�� ���� �۾��� �ϰ� �Ǹ� FALSE�� ��쿡 �񵿱� �۾��� �ȴ�. 
//            bret = GetOverlappedResult(hFileAsync, &g_WriteOV, &dwTrans, FALSE);//����� ��ȯ���� , ������ ���� �˷���������  
//
//            if (bret == TRUE)
//            {
//                wcout << L"��� �Ϸ�" << endl;
//                bPanding = FALSE;
//            }
//            else
//            {
//                DWORD Error = GetLastError();
//                if (Error == ERROR_IO_INCOMPLETE)
//                {
//                    wcout << L"��� ��..." << endl;
//                }
//                else
//                {
//                    wcout << L"��� ����" << endl;
//                    bPanding = FALSE;
//                }
//            }
//            Sleep(1000);
//        }
//    }
//
//    CloseHandle(hWFile);
//}