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
	void PostRecv();
	void PostSend();
};

#endif