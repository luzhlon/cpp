// Relayer.cpp : 定义控制台应用程序的入口点。
// Relay 协议的中继服务器 实现

#include "stdafx.h"
#include "LuaAPI.h"
#include "Tcp.h"
#include "thread_win.h"
#include <string>

using namespace std;

#define SERVERS "_SERVERS"
#define CONNECTS "_CONNECTS"
#define PRO_SIZE 32

LuaAPI lua;

class Connect
{
	friend class thread_server2client;
	friend class thread_client2server;
public:
	TcpConnect *server;
	TcpConnect *client;
	Connect(TcpConnect* cli)
	{
		server = NULL;
		client = cli;
	}
	void start(TcpConnect* ser);//服务器响应了连接，开始工作
};

//转发函数
void relay(TcpConnect *from, TcpConnect *to)
{
	char buf[1024 * 50] = { 0 };//50k的栈空间
	while (true)
	{
		//int len = from->recv(buf, sizeof(buf));
		int len = from->recv(buf, 32);
		printf("Relayed recved: %s | %d\n", buf, len);
		if (len <= 0) //如果from连接已经断开
		{
			puts("Relay:recv failure.");
			goto E;
		}
		int n = to->send(buf, len);
		printf("Relayed sended: %s | %d\n", buf, n);
		//printf("%x -> %x Relayed %d bytes\n", from, to, n);
		buf[len] = 0;
		printf("%x -> %x Relayed %s | %d\n", from, to, buf, len);
		if (n < 0)
		{
			puts("Relay:send failure.");
			goto E;
		}
	}
E:
	return;
	//delete from;
	//delete to;
}

//从服务器向客户端传输数据的线程
class thread_server2client : public thread_win
{
	Connect *m_con;
public:
	thread_server2client(Connect *con) { m_con = con; }
protected:
	virtual void run()
	{
		puts("server2client prepared");
		m_con->server->Send("ok", 2);//响应成功,对方准备传输数据
		relay(m_con->server, m_con->client);
	}
};

//从客户端向服务器传输数据的线程
class thread_client2server : public thread_win
{
	Connect *m_con;
public:
	thread_client2server(Connect *con) { m_con = con; }
protected:
	virtual void run()
	{
		puts("client2server prepared");
		m_con->client->Send("ok", 2);//告诉客户端，连接建立成功,准备传输数据
		relay(m_con->client, m_con->server);
	}
};

void Connect::start(TcpConnect* ser)
{
	printf("Connect start\n");
	server = ser;
	thread_client2server *c2s = new thread_client2server(this);
	c2s->start();
	thread_server2client *s2c = new thread_server2client(this);
	s2c->start();
}

void set_server(const char *server, TcpConnect* ser)
{
	lua.getglobal(SERVERS);
	lua.pushlightuserdata(ser);
	lua.setfield(-2, server);
	lua.pop();
}

TcpConnect *get_server(const char *server)
{
	lua.getglobal(SERVERS);
	lua.getfield(-1, server);
	TcpConnect *ser = (TcpConnect *)lua.touserdata(-1);
	lua.pop(2);
	return ser;
}

void set_connect(const char *conn, Connect* con)
{
	lua.getglobal(SERVERS);
	lua.pushlightuserdata(con);
	lua.setfield(-2, conn);
	lua.pop();
}

Connect *get_connect(const char *conn)
{
	lua.getglobal(SERVERS);
	lua.getfield(-1, conn);
	Connect *con = (Connect *)lua.touserdata(-1);
	lua.pop(2);
	return con;
}

Connect *new_connect(TcpConnect* client, const char *name, const char *server)
{
	TcpConnect *ser = get_server(server);
	if (!ser)//若没有此服务器
	{
		char *reason = "No thus server.";
		client->send("no", 2);//告诉客户端，连接服务器失败
		client->send(reason, strlen(reason));//失败原因
		return NULL;
	}
	Connect *con = new Connect(client);
	set_connect(name, con);//关联连接及其名字
	ser->Send(name, strlen(name));//告诉服务器有连接请求,内容是连接的名字
}

void handle_server(TcpConnect *ser)
{
	char buf[PRO_SIZE] = { 0 };
	printf("new server:%s,%u\n", ser->getIP(NULL), ser->getPort());
	ser->Recv(buf, PRO_SIZE);//服务器名字
	printf("Server:%s\n", buf);
	set_server(buf, ser);//将服务器名字与对象关联起来
	//对方开始不断的等待(客户端的连接请求)
}

void handle_client(TcpConnect *cli)
{
	char ser[PRO_SIZE] = { 0 };
	char con[PRO_SIZE] = { 0 };
	printf("new client connect:%s,%u\n", cli->getIP(NULL), cli->getPort());
	cli->Recv(ser, PRO_SIZE);//要连接的服务器名字
	printf("To Connect Server:%s\n", ser);
	cli->Recv(con, PRO_SIZE);//连接 的 名称
	printf("Connect:%s\n", con);
	new_connect(cli, con, ser);//新建连接
	//对方开始等待请求连接的结果
}

void handle_respond(TcpConnect *rep)
{
	char con[PRO_SIZE] = { 0 };
	printf("new respond connect:%s,%u\n", rep->getIP(NULL), rep->getPort());
	rep->Recv(con, PRO_SIZE);//响应的连接的名字
	Connect *conn = get_connect(con);
	if (!conn) rep->Send("no", 2); //没有此连接，告诉服务器响应失败
	conn->start(rep);
}

class thread_connect : public thread_win
{
	TcpConnect *m_con;
public:
	thread_connect(TcpConnect *con) { m_con = con; }
	virtual void run();
};


void thread_connect::run()//处理各种连接的线程
{
	char buf[PRO_SIZE] = { 0 };
	printf("new connection:%s,%u\n", m_con->getIP(NULL), m_con->getPort());
	while (true)
	{
		int len = m_con->Recv(buf, PRO_SIZE);
		if (len < 0) //连接断开时
		{
			delete m_con;
			break;
		}
		string str(buf);
		if (str== "server")
			handle_server(m_con);//作为服务器连接处理
		else if (str=="client")
			handle_client(m_con);//作为客户端连接处理
		else if (str=="respond")
			handle_respond(m_con);//作为服务器响应客户端的连接处理
	}
}

void work()
{
	TcpServer server;
	while (true)
	{
		TcpConnect* con = server.waitConnect(4320);
		if (con)
		{
			thread_connect * thread = new thread_connect(con);
			thread->start();
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	lua = *LuaAPI::NewStdState();
	lua.newtable();
	lua.setglobal(SERVERS);
	lua.newtable();
	lua.setglobal(CONNECTS);
	work();
	return 0;
}

