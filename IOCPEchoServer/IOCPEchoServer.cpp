// IOCPEchoServer.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

///////////////////////////////////////////////////////////////////////
// ���� ����Ʈ
///////////////////////////////////////////////////////////////////////
list<CSESSION *> g_Session;
CRITICAL_SECTION g_ListCS;

///////////////////////////////////////////////////////////////////////
// ��������
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
	// ���� �ʱ�ȭ
	///////////////////////////////////////////////////////////////////
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	///////////////////////////////////////////////////////////////////
	// Completion Port ����
	///////////////////////////////////////////////////////////////////
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (g_hIOCP == NULL)
		return;

	///////////////////////////////////////////////////////////////////
	// socket ����
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
// ���� ���� �Լ�
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
// Send, Recv ó�� �Լ�
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

		// �����Ѱ� ó��(����, WhiteIP��)->�����Լ�(�������ʿ��� ó��)

		//-----------------------------------------------------------------------------
		// ���� ���
		//-----------------------------------------------------------------------------
		CSESSION *pSession = AddSession(clientSocket);

		//-----------------------------------------------------------------------------
		// IOCP ���
		//-----------------------------------------------------------------------------
		HANDLE result = CreateIoCompletionPort((HANDLE)clientSocket, g_hIOCP, (ULONG_PTR)pSession, 0);
		if (result == NULL)
		{
			wprintf(L"Error Code : %d, IOCP Registration Error\n", GetLastError());
		}

		//-----------------------------------------------------------------------------
		// Recv ���
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

