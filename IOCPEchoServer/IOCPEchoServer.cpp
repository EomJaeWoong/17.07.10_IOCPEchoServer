// IOCPEchoServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

///////////////////////////////////////////////////////////////////////
// 세션 리스트
///////////////////////////////////////////////////////////////////////
list<CSESSION *> g_Session;
CRITICAL_SECTION g_ListCS;

///////////////////////////////////////////////////////////////////////
// 리슨소켓
///////////////////////////////////////////////////////////////////////
SOCKET g_ListenSocket;

///////////////////////////////////////////////////////////////////////
// IOCP Handle
///////////////////////////////////////////////////////////////////////
HANDLE g_hIOCP;

///////////////////////////////////////////////////////////////////////
// 세션 관련 함수
///////////////////////////////////////////////////////////////////////
CSESSION *AddSession(SOCKET Socket)
{
	CSESSION *pSession = new CSESSION(Socket);

	EnterCriticalSection(&g_ListCS);
	g_Session.push_back(pSession);
	LeaveCriticalSection(&g_ListCS);

	return pSession;
}

void RemoveSession(SOCKET Socket)
{

}

///////////////////////////////////////////////////////////////////////
// Send, Recv 처리 함수
///////////////////////////////////////////////////////////////////////
void CompleteRecv(CSESSION *pSession, DWORD dwTransferred)
{

}

void CompleteSend(CSESSION *pSession, DWORD dwTransferred)
{

}

unsigned __stdcall AcceptThread(LPVOID acceptParam)
{
	SOCKADDR_IN clinetAddr;
	int addrlen;

	while (1)
	{
		SOCKET clientSocket = accept(g_ListenSocket, (SOCKADDR *)&clinetAddr, &addrlen);

		// 자잘한거 처리(점검, WhiteIP등)->가상함수(컨텐츠쪽에서 처리)

		//-----------------------------------------------------------------------------
		// 세션 등록
		//-----------------------------------------------------------------------------
		CSESSION *pSession = AddSession(clientSocket);

		//-----------------------------------------------------------------------------
		// IOCP 등록
		//-----------------------------------------------------------------------------
		HANDLE result = CreateIoCompletionPort((HANDLE)clientSocket, g_hIOCP, (ULONG_PTR)pSession, 0);
		if (result == NULL)
		{
			wprintf(L"Error Code : %d, IOCP Registration Error\n", GetLastError());
		}

		//-----------------------------------------------------------------------------
		// Recv 등록
		//-----------------------------------------------------------------------------
		pSession->PostRecv();
	}
}

unsigned __stdcall WorkerThread(LPVOID workerParam)
{

}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hAcceptTh, WorkerTh[3];


	return 0;
}

