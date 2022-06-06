//파일입출력도 비동기 할수있다
#include<Windows.h>
#include<iostream>

using namespace std;



OVERLAPPED g_ReadOV = { 0, };//얼만큼 읽었다라는 것을 운영체제가 기입하고 처리해준다, 중간결과는 절대로 정확한값이 안떨어진다 
OVERLAPPED g_WriteOV = { 0, };
LARGE_INTEGER  filesize; // LARGE_INTEGER = LONGLONG 자료형
WCHAR* g_Buffer = NULL;

void main()
{
    setlocale(LC_ALL, "KOREAN"); //유니코드 콘솔창에 한글 출력 할때 사용

    // CREATE_ALWAYS = 새로운 파일을 만들되 만약 지정한 파일이 존재하면 기존 파일을 삭제하고 다시 만든다.

    HANDLE hWFile = CreateFile(L"test.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //파일 생성 , 반환값 : 실패(INVALID_HANDLE_VALUE)

    if (hWFile != NULL)
    {
        WCHAR Buffer[] = L"하이!";
        DWORD dwSize = sizeof(Buffer);
        DWORD dwWritten;
        BOOL Ret = WriteFile(hWFile, Buffer, dwSize, &dwWritten, NULL);
       // LPOVERLAPPED lpOverlapped = (마지막인자) 사용 유무에 따라 비동기를 결정할수있다.
        if (Ret == TRUE)
        {
            wcout << L"출력을 성공" << endl;
        }
    }

    CloseHandle(hWFile);

    HANDLE hFileRead = CreateFile(L"test.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //파일 생성 , 반환값 : 실패(INVALID_HANDLE_VALUE)

    if (hFileRead != NULL)
    {
        WCHAR Buffer[512] = { 0, };
        DWORD dwSize = sizeof(Buffer);
        DWORD dwRead;
        BOOL ret = ReadFile(hFileRead, Buffer, dwSize, &dwRead, NULL);
        //LPOVERLAPPED lpOverlapped = (마지막인자) 사용 유무에 따라 비동기를 결정할수있다.
        if (ret == TRUE)
        {
            wcout << Buffer << endl;
        }
    }

    CloseHandle(hFileRead);
    //===================================================================================================
    //비동기 입출력

    //HANDLE hFileAsync = CreateFile(L"yy.zip", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 
    //    FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, &g_ReadOV); //비동기 할때는 명시해줘야 한다 FILE_FLAG_OVERLAPPED
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
    //    // LPOVERLAPPED lpOverlapped = (마지막인자) 사용 유무에 따라 비동기를 결정할수있다.
    //    BOOL bPanding = FALSE;
    //    if (Return == FALSE)
    //    {
    //        DWORD Error = GetLastError();
    //        if (Error == ERROR_IO_PENDING) // 실패가 아니고 비동기 입출력이 완료 라는 의미
    //        {
    //            wcout << L"읽기 중" << endl;
    //            bPanding = TRUE;
    //        }
    //        else
    //        {
    //            wcout << L"읽기 실패" << endl;
    //            bPanding = FALSE;
    //        }
    //    }
    //    else
    //    {
    //        wcout << L"로드 완료" << endl;
    //    }
    //    //로드 중
    //    DWORD dwTrans = 0;
    //    BOOL  bret;
    //    while(bPanding)
    //    {
    //        //마지막 인자 Wait = TRUE면 동기 작업을 하게 되며 FALSE일 경우에 비동기 작업이 된다. 
    //        bret = GetOverlappedResult(hFileAsync, &g_ReadOV, &dwTrans, FALSE);//결과만 반환해줌 , 과정은 절대 알려주지않음  
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
    //                wcout << L"읽기 중" <<  endl;
    //                bPanding = TRUE;
    //            }
    //            else
    //            {
    //                wcout << L"읽기 실패" << endl;
    //            }
    //        }
    //    }
    //}

    //CloseHandle(hFileAsync);

    //wcout << "복사 합니다" << endl;
    //HANDLE hWFile = CreateFile(L"Copy.zip", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED |FILE_FLAG_WRITE_THROUGH, NULL); //파일 생성 , 반환값 : 실패(INVALID_HANDLE_VALUE)

    //if (hWFile != NULL)
    //{
    //    DWORD dwLen = filesize.QuadPart;
    //    DWORD dwWritten;
    //    BOOL Ret = WriteFile(hWFile, g_Buffer, dwLen, &dwWritten, &g_WriteOV);
    //   // LPOVERLAPPED lpOverlapped = (마지막인자) 사용 유무에 따라 비동기를 결정할수있다.
    //    BOOL bPanding = FALSE;
    //    if (Ret == FALSE)
    //    {
    //        DWORD Error = GetLastError();
    //        if (Error == ERROR_IO_PENDING) // 실패가 아니고 비동기 입출력이 완료 라는 의미
    //        {
    //            wcout << L"출력 중" << endl;
    //            bPanding = TRUE;
    //        }
    //        else
    //        {
    //            wcout << L"출력 실패" << endl;
    //            
    //        }
    //    }
    //    else
    //    {
    //        wcout << L"출력 완료" << endl;
    //        bPanding = FALSE;
    //    }
    //    
    //    DWORD dwTrans = 0;
    //    BOOL  bret;
    //    while (bPanding)
    //    {
    //        //마지막 인자 Wait = TRUE면 동기 작업을 하게 되며 FALSE일 경우에 비동기 작업이 된다. 
    //        bret = GetOverlappedResult(hFileAsync, &g_WriteOV, &dwTrans, FALSE);//결과만 반환해줌 , 과정은 절대 알려주지않음  

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
    //                wcout << L"출력 중..." << endl;
    //            }
    //            else
    //            {
    //                wcout << L"읽기 실패" << endl;
    //                bPanding = FALSE;
    //            }
    //        }
    //        Sleep(1000);
    //    }
    //}

    //CloseHandle(hWFile);
}
   
    
