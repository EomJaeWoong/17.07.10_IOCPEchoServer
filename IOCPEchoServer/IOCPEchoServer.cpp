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

