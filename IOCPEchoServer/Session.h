#ifndef __SESSION__H__
#define __SESSION__H__

class CSESSION
{
public :
	CSESSION(SOCKET Socket)
	{
		_Socket = Socket;

		memset(&_RecvOverlapped, 0, sizeof(OVERLAPPED));
		memset(&_SendOverlapped, 0, sizeof(OVERLAPPED));

		_RecvQ.ClearBuffer();
		_SendQ.ClearBuffer();

		_lIOCount = 0;

		_bSendFlag = false;
	}

	virtual ~CSESSION();

private :
	SOCKET _Socket;

	OVERLAPPED _RecvOverlapped;
	OVERLAPPED _SendOverlapped;

	CAyaStreamSQ _RecvQ;
	CAyaStreamSQ _SendQ;

	LONG64 _lIOCount;

	bool _bSendFlag;

public :
	void PostRecv()
	{
		int retval, iCount = 1;
		DWORD dwRecvSize, dwflag = 0;
		WSABUF wsabuf[2];

		wsabuf[0].buf = _RecvQ.GetWriteBufferPtr();
		wsabuf[0].len = _RecvQ.GetNotBrokenPutSize();

		if (_RecvQ.GetNotBrokenPutSize() != _RecvQ.GetFreeSize())
		{
			wsabuf[1].buf = _RecvQ.GetBufferPtr();
			wsabuf[1].len = _RecvQ.GetFreeSize() - _RecvQ.GetNotBrokenPutSize();
			iCount++;
		}

		InterlockedIncrement64((LONG64 *)&_lIOCount);
		retval = WSARecv(_Socket, wsabuf, iCount, &dwRecvSize, &dwflag, &_RecvOverlapped, NULL);
		if (retval == SOCKET_ERROR)
		{
			int iErrorCode = GetLastError();
			if (iErrorCode != WSA_IO_PENDING)
			{
				if (iErrorCode != 10054)
					wprintf(L"Error code : %d, RecvPost Error\n", iErrorCode);

				if (0 == InterlockedDecrement64((LONG64 *)&_lIOCount))
					//RemoveSession(_Socket);

				return;
			}
		}
	}

	void PostSend()
	{

	}
};

#endif