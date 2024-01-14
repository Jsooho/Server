#include<Windows.h>
#include<iostream>
#define BlockSize (4096*4096)
#define MAX_WORKER_THREAD 3
using namespace std;
//IOCP�� OVERLAPPED ����ü�� ����ؼ� ����Ѵ�

OVERLAPPED g_ReadOV = { 0, };
OVERLAPPED g_WriteOV = { 0, };

//�񵿱� �۾��� �������� ���� �־����, �񵿱� �������� ����ü�� 1��
DWORD      g_dwCurrentRead = 0;
DWORD      g_dwCurrentWrite = 0;
OVERLAPPED g_ReadOVArray[999999];
OVERLAPPED g_WriteOVArray[999999];

LARGE_INTEGER  filesize; // LARGE_INTEGER = LONGLONG �ڷ���
WCHAR* g_Buffer = NULL;
HANDLE g_hFileRead;
HANDLE g_hFileWrite;
HANDLE g_hWorkThread[MAX_WORKER_THREAD];

/*�Ϸ� ��Ʈ ���� ��ü*/
HANDLE g_hIOCP;
LARGE_INTEGER g_LargeRead;
LARGE_INTEGER g_LargeWrite;
HANDLE g_hEventEnd;

bool DispatchRead(DWORD transfer, OVERLAPPED* ov)
{
    g_LargeRead.QuadPart += transfer; //���� ����Ʈ �ڿ������� �о��
    
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
            wcout << L"��� ��" << endl;
        }
        else
        {
            wcout << L"��� ����" << endl;
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
            wcout << L"�д� ��" << endl;
        }
        else
        {
            wcout << L"�б� ����" << endl;
            return false;
        }
    }

    return true;
}

DWORD WINAPI WorkerThread(LPVOID lpThreadParameter)
{
    DWORD dwTransfer;//dwTransfer = �񵿱� �۾� ���
    ULONG_PTR KeyValue; //�Ϸ�PORT�� ���Ͽ� �۾��� ������ �ִ�,  ULONG_PTR(32,64��Ʈ �� �ᵵ��) = �ּ�
    OVERLAPPED* pOverlapped; //�񵿱� �۾��� ������ ����ü�� �Ѱ��ش�
    //INFINITE = �����Ͱ� ������ ��ȯ���ش�
    
    //�Ϸ� ť�� �����Ͱ� ������ �۾��ϰ� ����� , ť�� �����Ͱ� ������ ���
    while (1)
    {
        if (WaitForSingleObject(g_hEventEnd, 1) == WAIT_OBJECT_0)
        {
            break;
        }

        BOOL bReturn = ::GetQueuedCompletionStatus(g_hIOCP, &dwTransfer, &KeyValue, &pOverlapped, 1);
        if (bReturn == TRUE)
        {
            //�۾�
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

    g_hEventEnd = ::CreateEvent(0, TRUE, FALSE, 0); //���� �̺�Ʈ

    g_hFileRead = CreateFile(L"API_1_2.zip", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, &g_ReadOV); //�񵿱� �Ҷ��� �������� �Ѵ� FILE_FLAG_OVERLAPPED

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

    // �񵿱� ����� �۾���� ���� ť = �۾��� ������ ���⿡ ����� ����
    g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0); //�̰� �Ϸ�Ǹ� �񵿱� �۾���� �����ϴ� ť�� �����ȴ�
    ::CreateIoCompletionPort(g_hFileRead, g_hIOCP, 1000, 0); //Read , IOCP�ڵ鿡 READ,WRITE �� ��������ش� �񵿱� �۾��� �Ϸ�Ǹ� �ٷ� ���� ť�� ����
    ::CreateIoCompletionPort(g_hFileWrite, g_hIOCP, 2000, 0); //write , [1000,2000���� �Ϸ�PORT] ��) Read�۾��̳����� 1000���� �����ϸ� Read�� ������

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

//IOCP �񵿱� �����
// �ϳ��� IOCP�ڵ��� ���� READ,WRITE�� ��� �۾��Ҽ� �ְ� �����带���� ����Ѵ�