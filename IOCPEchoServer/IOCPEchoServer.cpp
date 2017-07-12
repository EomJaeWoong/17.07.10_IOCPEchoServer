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


void Initial(WCHAR *wOpenIP, int iPort, int iWorkerThdNum, bool bNagle, int iMaxConnection)
{
	int retval;

	///////////////////////////////////////////////////////////////////
	// 윈속 초기화
	///////////////////////////////////////////////////////////////////
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	///////////////////////////////////////////////////////////////////
	// Completion Port 생성
	///////////////////////////////////////////////////////////////////
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (g_hIOCP == NULL)
		return;

	///////////////////////////////////////////////////////////////////
	// socket 생성
	///////////////////////////////////////////////////////////////////
	g_ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (g_ListenSocket == INVALID_SOCKET)
		return;

	///////////////////////////////////////////////////////////////////
	//bind
	///////////////////////////////////////////////////////////////////
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(iPort);
	InetPton(AF_INET, wOpenIP, &serverAddr.sin_addr);
	retval = bind(g_ListenSocket, (SOCKADDR *)&serverAddr, sizeof(SOCKADDR_IN));
	if (retval == SOCKET_ERROR)
		return;

	///////////////////////////////////////////////////////////////////
	//listen
	///////////////////////////////////////////////////////////////////
	retval = listen(g_ListenSocket, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return;
}

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

