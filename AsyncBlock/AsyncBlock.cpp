#include<Windows.h>
#include<iostream>
#define BlockSize 2048
using namespace std;


void main()
{
    setlocale(LC_ALL, "KOREAN");

    OVERLAPPED g_ReadOV = { 0, };//얼만큼 읽었다라는 것을 운영체제가 기입하고 처리해준다, 중간결과는 절대로 정확한값이 안떨어진다 
    OVERLAPPED g_WriteOV = { 0, };
    LARGE_INTEGER  filesize; // LARGE_INTEGER = LONGLONG 자료형
    WCHAR* g_Buffer = NULL;

    HANDLE hFileAsync = CreateFile(L"API_1_2.zip", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, &g_ReadOV); //비동기 할때는 명시해줘야 한다 FILE_FLAG_OVERLAPPED

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
                wcout << L"읽는 중" << endl;
            }
            else
            {
                wcout << L"읽기 실패" << endl;
            }
        }
        else
        {
            wcout << L"읽기 성공" << endl;
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
                wcout << L"읽기 중" << endl;
            }
            else
            {
                wcout << L"읽기 실패" << endl;
            }
        }
        wcout << L"복사 시작" << endl;
        g_WriteOV.Offset = i * 2048;
        g_WriteOV.OffsetHigh = 0;
        bret = WriteFile(hFileCopy, g_Buffer, dwRead, &dwWrite, &g_WriteOV);
        if (bret == FALSE)
        {
            DWORD Error = GetLastError();
            if (Error == ERROR_IO_PENDING)
            {
                wcout << L"출력 중" << endl;
            }
            else
            {
                wcout << L"출력 실패" << endl;
            }
        }
        else
        {
            wcout << L"출력 성공" << endl;
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
                wcout << L"출력 중" << endl;
            }
            else
            {
                wcout << L"출력 실패" << endl;
            }
        }
        dwTotal += dwWrite;

        if (filesize.LowPart == dwTotal)
        {
            wcout << L"복사 완료" << endl;
            break;
        }
        Sleep(1000);
       
    }
    CloseHandle(hFileAsync);
    CloseHandle(hFileCopy);
}
//    wcout << L"복사 합니다" << endl;
//    HANDLE hWFile = CreateFile(L"Copy.zip", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_WRITE_THROUGH, NULL); //파일 생성 , 반환값 : 실패(INVALID_HANDLE_VALUE)
//
//    if (hWFile != NULL)
//    {
//        DWORD dwLen = filesize.QuadPart;
//        DWORD dwWritten;
//        BOOL Ret = WriteFile(hWFile, g_Buffer, dwLen, &dwWritten, &g_WriteOV);
//        // LPOVERLAPPED lpOverlapped = (마지막인자) 사용 유무에 따라 비동기를 결정할수있다.
//        BOOL bPanding = FALSE;
//        if (Ret == FALSE)
//        {
//            DWORD Error = GetLastError();
//            if (Error == ERROR_IO_PENDING) // 실패가 아니고 비동기 입출력이 완료 라는 의미
//            {
//                wcout << L"출력 중" << endl;
//                bPanding = TRUE;
//            }
//            else
//            {
//                wcout << L"출력 실패" << endl;
//
//            }
//        }
//        else
//        {
//            wcout << L"출력 완료" << endl;
//            bPanding = FALSE;
//        }
//
//        DWORD dwTrans = 0;
//        BOOL  bret;
//        while (bPanding)
//        {
//            //마지막 인자 Wait = TRUE면 동기 작업을 하게 되며 FALSE일 경우에 비동기 작업이 된다. 
//            bret = GetOverlappedResult(hFileAsync, &g_WriteOV, &dwTrans, FALSE);//결과만 반환해줌 , 과정은 절대 알려주지않음  
//
//            if (bret == TRUE)
//            {
//                wcout << L"출력 완료" << endl;
//                bPanding = FALSE;
//            }
//            else
//            {
//                DWORD Error = GetLastError();
//                if (Error == ERROR_IO_INCOMPLETE)
//                {
//                    wcout << L"출력 중..." << endl;
//                }
//                else
//                {
//                    wcout << L"출력 실패" << endl;
//                    bPanding = FALSE;
//                }
//            }
//            Sleep(1000);
//        }
//    }
//
//    CloseHandle(hWFile);
//}