
#include "Tcp.h"

TcpConnect::TcpConnect(SOCKET sock)
{
	InitSocket(sock);
}

TcpConnect::~TcpConnect()
{
	closesocket(m_socket);
}

//传入一个IP地址字符串或域名
bool TcpConnect::setAddress(const char *addr)
{
	unsigned long ip = inet_addr(addr);
	if (ip == INADDR_NONE)
	{
		m_host = gethostbyname(addr);
		if (!m_host->h_addr_list)
			return false;
		ip = (unsigned long)m_host->h_addr_list[0];
		if (!ip)
			return false;
	}
	return setAddress(ip);
}
//设置要bind或connect的地址 //传入一个long型的IP
bool TcpConnect::setAddress(unsigned long ip)
{
	m_sockAddr.sin_addr.S_un.S_addr = ip;
	return true;
}
//获取IP地址字符串
const char * TcpConnect::getIP(char *ipAddr)
{
	char *ip = inet_ntoa(m_sockAddr.sin_addr);
	if (!ip) return false;
	if(ipAddr) strcpy(ipAddr, ip);
	return ip;
}
//获取IP地址
unsigned long TcpConnect::getIP()
{
	return m_sockAddr.sin_addr.S_un.S_addr;
}

void TcpConnect::InitSocket(SOCKET sock)
{
	InitSocketLib();
	m_socket = sock ? sock : socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	m_sockAddr.sin_family = AF_INET;
	m_sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
}

TcpClient::TcpClient()
{
}

TcpClient::~TcpClient()
{
}

bool TcpClient::connect(const char *addr, unsigned int port)
{
	setAddress(addr);
	setPort(port);
	return connect();
}

bool TcpClient::connect()
{
	m_ret = ::connect(m_socket, (const sockaddr *)&m_sockAddr, sizeof(m_sockAddr));
	return !m_ret;
}

TcpServer::TcpServer()
: m_binded(false), m_listened(false)
{
}

TcpServer::~TcpServer()
{
}

TcpConnect* TcpServer::accept()
{
	sockaddr_in remoteAddr;
	int addrLen = sizeof(remoteAddr);//!!!!!addrlen  in out !!!!!!!!1
	SOCKET remoteSock = ::accept(m_socket, (sockaddr *)&remoteAddr, &addrLen);
	TcpConnect *connect = new TcpConnect(remoteSock);
	memcpy(&connect->m_sockAddr, &remoteAddr, sizeof(remoteAddr));
	if (remoteSock == INVALID_SOCKET)
	{
		delete connect;
		return NULL;
	}
	else
		return connect;
	//return (remoteSock == INVALID_SOCKET) ? (delete connect, NULL) : connect;
}

bool TcpServer::bind(unsigned int port)
{
	if (m_binded) return true;
	if (port) m_sockAddr.sin_port = htons(port);
	m_ret = ::bind(m_socket, (const sockaddr *)&m_sockAddr, sizeof(m_sockAddr));
	m_binded = !m_ret;
	return m_binded;
}

bool TcpServer::listen(int backlog)
{
	if (m_listened) return true;
	m_ret = ::listen(m_socket, backlog);
	m_listened = !m_ret;
	return m_listened;
}

TcpConnect* TcpServer::waitConnect(unsigned int port, int backlog)
{
	if(bind(port) && listen(backlog))
		return accept();
	return NULL;
}

void TcpConnect::InitSocketLib()
{
#ifdef __WINDOWS__
	static WSADATA *wsData = NULL;
	if (wsData) return;
	wsData = new WSADATA;
	WSAStartup(MAKEWORD(2,2), wsData);
#endif
}