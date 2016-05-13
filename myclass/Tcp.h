#ifndef __TCP_H_
#define __TCP_H_

#include "stdlib.h"
#include "string.h"

#define __WINDOWS__ 1

#ifdef __WINDOWS__
	#ifndef _WINSOCKAPI_
	#define _WINSOCKAPI_
		#include "WinSock2.h"
	#endif
#endif

class TcpConnect
{
public:
	static void InitSocketLib();

public:
	TcpConnect(SOCKET sock = NULL);
	~TcpConnect();

	void InitSocket(SOCKET sock = NULL);

	bool setAddress(const char *addr);
	bool setAddress(unsigned long ip);
	const char *getIP(char *ipAddr);
	unsigned long getIP();

	inline int get_last(){ return m_ret; }

	inline void setPort(unsigned int port){ m_sockAddr.sin_port = htons((u_short)port); }
	inline unsigned int getPort(){ return ntohs(m_sockAddr.sin_port); }

	inline int send(const char *buf, int len, int flags = 0)
	{
		return ::send(m_socket, buf, len, flags);
	}
	inline int recv(char *buf, int len, int flags = 0)
	{
		return ::recv(m_socket, buf, len, flags);
	}

public:
	SOCKET m_socket;
	sockaddr_in m_sockAddr;
	hostent *m_host;
	int    m_ret;
};

class TcpClient : public TcpConnect
{
public:
	TcpClient();
	~TcpClient();

	inline bool connect();
	bool connect(const char *addr, unsigned int port);
};

class TcpServer : public TcpConnect
{
public:
	TcpServer();
	~TcpServer();

	bool bind(unsigned int port = 0);
	bool listen(int backlog = 5);
	TcpConnect* accept();
	TcpConnect* waitConnect(unsigned int port, int backlog = 5);

	bool m_binded;
	bool m_listened;
};


#endif
