#include<Windows.h>
#include<iostream>
#define BlockSize (4096*4096)
#define MAX_WORKER_THREAD 3
using namespace std;
//IOCP는 OVERLAPPED 구조체를 상속해서 사용한다

OVERLAPPED g_ReadOV = { 0, };
OVERLAPPED g_WriteOV = { 0, };

//비동기 작업당 오버랩은 따로 있어야함, 비동기 오버랩당 구조체는 1개
DWORD      g_dwCurrentRead = 0;
DWORD      g_dwCurrentWrite = 0;
OVERLAPPED g_ReadOVArray[999999];
OVERLAPPED g_WriteOVArray[999999];

LARGE_INTEGER  filesize; // LARGE_INTEGER = LONGLONG 자료형
WCHAR* g_Buffer = NULL;
HANDLE g_hFileRead;
HANDLE g_hFileWrite;
HANDLE g_hWorkThread[MAX_WORKER_THREAD];

/*완료 포트 관리 객체*/
HANDLE g_hIOCP;
LARGE_INTEGER g_LargeRead;
LARGE_INTEGER g_LargeWrite;
HANDLE g_hEventEnd;

bool DispatchRead(DWORD transfer, OVERLAPPED* ov)
{
    g_LargeRead.QuadPart += transfer; //읽은 바이트 뒤에서부터 읽어라
    
    if (g_LargeRead.QuadPart == g_LargeWrite.QuadPart)
    {
        return true;
    }

    ++g_dwCurrentWrite;
    g_WriteOVArray[g_dwCurrentWrite].Offset = g_LargeWrite.LowPart;
    g_WriteOVArray[g_dwCurrentWrite].OffsetHigh = g_LargeWrite.HighPart;

    DWORD dwWrite;
    BOOL bret = WriteFile(g_hFileWrite, g_Buffer, transfer, &dwWrite, &g_WriteOVArray[g_dwCurrentWrite]);
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
    return true;
}

bool DispatchWrite(DWORD transfer, OVERLAPPED* ov)
{
    g_LargeWrite.QuadPart += transfer;

    ++g_dwCurrentRead;
    g_ReadOVArray[g_dwCurrentRead].Offset = g_LargeRead.LowPart;
    g_ReadOVArray[g_dwCurrentRead].OffsetHigh = g_LargeRead.HighPart;

    DWORD dwRead;
    BOOL ret = ReadFile(g_hFileRead, g_Buffer, BlockSize, &dwRead, &g_ReadOVArray[g_dwCurrentRead]);
    if (ret == FALSE)
    {
        DWORD Error = GetLastError();
        if (Error == ERROR_IO_PENDING)
        {
            wcout << L"읽는 중" << endl;
        }
        else
        {
            wcout << L"읽기 실패" << endl;
            return false;
        }
    }

    return true;
}

DWORD WINAPI WorkerThread(LPVOID lpThreadParameter)
{
    DWORD dwTransfer;//dwTransfer = 비동기 작업 결과
    ULONG_PTR KeyValue; //완료PORT를 통하여 작업을 나눌수 있다,  ULONG_PTR(32,64비트 다 써도됨) = 주소
    OVERLAPPED* pOverlapped; //비동기 작업한 오버렙 구조체도 넘겨준다
    //INFINITE = 데이터가 있으면 반환해준다
    
    //완료 큐에 데이터가 있으면 작업하게 만든다 , 큐에 데이터가 없으면 대기
    while (1)
    {
        if (WaitForSingleObject(g_hEventEnd, 1) == WAIT_OBJECT_0)
        {
            break;
        }

        BOOL bReturn = ::GetQueuedCompletionStatus(g_hIOCP, &dwTransfer, &KeyValue, &pOverlapped, 1);
        if (bReturn == TRUE)
        {
            //작업
            if (KeyValue == 1000)
            {
                DispatchRead(dwTransfer,pOverlapped);
            }
            if (KeyValue == 2000)
            {
                if (DispatchWrite(dwTransfer, pOverlapped))
                {
                    //::SetEvent(g_hEventEnd);
                    //break;
                }
            }
        }
        else
        {
            if(GetLastError() != WAIT_TIMEOUT)
            {
                ::SetEvent(g_hEventEnd);
                break;
            }
        }
    }
    return 1;
    
}
void main()
{
    setlocale(LC_ALL, "KOREAN");

    g_hEventEnd = ::CreateEvent(0, TRUE, FALSE, 0); //수동 이벤트

    g_hFileRead = CreateFile(L"API_1_2.zip", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, &g_ReadOV); //비동기 할때는 명시해줘야 한다 FILE_FLAG_OVERLAPPED

    if (g_hFileRead == INVALID_HANDLE_VALUE)
    {
        CloseHandle(g_hFileRead);
        return;
    }

    g_hFileWrite = CreateFile(L"Copy.zip", GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

    if (g_hFileWrite == INVALID_HANDLE_VALUE)
    {
        CloseHandle(g_hFileWrite);
        return;
    }

    // 비동기 입출력 작업결과 저장 큐 = 작업이 끝난게 여기에 결과를 저장
    g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0); //이게 완료되면 비동기 작업결과 저장하는 큐가 생성된다
    ::CreateIoCompletionPort(g_hFileRead, g_hIOCP, 1000, 0); //Read , IOCP핸들에 READ,WRITE 다 연결시켜준다 비동기 작업이 완료되면 바로 위에 큐에 저장
    ::CreateIoCompletionPort(g_hFileWrite, g_hIOCP, 2000, 0); //write , [1000,2000번이 완료PORT] 예) Read작업이끝나고 1000번을 리턴하면 Read가 끝난것

    GetFileSizeEx(g_hFileRead, &filesize);

    DWORD dwRead;
    DWORD dwWrite;
    DWORD dwTotal = 0;
    g_Buffer = new WCHAR[BlockSize];

    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        DWORD id;
        g_hWorkThread[iThread] = ::CreateThread(0, 0, WorkerThread, nullptr, 0, &id);
    }

    g_LargeRead.LowPart = 0;
    g_LargeRead.HighPart = 0;
    
    BOOL bret = ReadFile(g_hFileRead, g_Buffer, BlockSize, &dwRead, &g_ReadOVArray[g_dwCurrentRead++]);

    WaitForMultipleObjects(MAX_WORKER_THREAD, g_hWorkThread, TRUE, INFINITE);
    
    for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
    {
        CloseHandle(g_hWorkThread[iThread]);
    }

    CloseHandle(g_hEventEnd);
    CloseHandle(g_hIOCP);
    CloseHandle(g_hFileRead);
    CloseHandle(g_hFileWrite);
}

//IOCP 비동기 입출력
// 하나의 IOCP핸들을 통해 READ,WRITE를 계속 작업할수 있게 스레드를만들어서 사용한다