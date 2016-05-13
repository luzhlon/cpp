// Relayer.cpp : �������̨Ӧ�ó������ڵ㡣
// Relay Э����м̷����� ʵ��

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
	void start(TcpConnect* ser);//��������Ӧ�����ӣ���ʼ����
};

//ת������
void relay(TcpConnect *from, TcpConnect *to)
{
	char buf[1024 * 50] = { 0 };//50k��ջ�ռ�
	while (true)
	{
		//int len = from->recv(buf, sizeof(buf));
		int len = from->recv(buf, 32);
		printf("Relayed recved: %s | %d\n", buf, len);
		if (len <= 0) //���from�����Ѿ��Ͽ�
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

//�ӷ�������ͻ��˴������ݵ��߳�
class thread_server2client : public thread_win
{
	Connect *m_con;
public:
	thread_server2client(Connect *con) { m_con = con; }
protected:
	virtual void run()
	{
		puts("server2client prepared");
		m_con->server->Send("ok", 2);//��Ӧ�ɹ�,�Է�׼����������
		relay(m_con->server, m_con->client);
	}
};

//�ӿͻ�����������������ݵ��߳�
class thread_client2server : public thread_win
{
	Connect *m_con;
public:
	thread_client2server(Connect *con) { m_con = con; }
protected:
	virtual void run()
	{
		puts("client2server prepared");
		m_con->client->Send("ok", 2);//���߿ͻ��ˣ����ӽ����ɹ�,׼����������
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
	if (!ser)//��û�д˷�����
	{
		char *reason = "No thus server.";
		client->send("no", 2);//���߿ͻ��ˣ����ӷ�����ʧ��
		client->send(reason, strlen(reason));//ʧ��ԭ��
		return NULL;
	}
	Connect *con = new Connect(client);
	set_connect(name, con);//�������Ӽ�������
	ser->Send(name, strlen(name));//���߷���������������,���������ӵ�����
}

void handle_server(TcpConnect *ser)
{
	char buf[PRO_SIZE] = { 0 };
	printf("new server:%s,%u\n", ser->getIP(NULL), ser->getPort());
	ser->Recv(buf, PRO_SIZE);//����������
	printf("Server:%s\n", buf);
	set_server(buf, ser);//������������������������
	//�Է���ʼ���ϵĵȴ�(�ͻ��˵���������)
}

void handle_client(TcpConnect *cli)
{
	char ser[PRO_SIZE] = { 0 };
	char con[PRO_SIZE] = { 0 };
	printf("new client connect:%s,%u\n", cli->getIP(NULL), cli->getPort());
	cli->Recv(ser, PRO_SIZE);//Ҫ���ӵķ���������
	printf("To Connect Server:%s\n", ser);
	cli->Recv(con, PRO_SIZE);//���� �� ����
	printf("Connect:%s\n", con);
	new_connect(cli, con, ser);//�½�����
	//�Է���ʼ�ȴ��������ӵĽ��
}

void handle_respond(TcpConnect *rep)
{
	char con[PRO_SIZE] = { 0 };
	printf("new respond connect:%s,%u\n", rep->getIP(NULL), rep->getPort());
	rep->Recv(con, PRO_SIZE);//��Ӧ�����ӵ�����
	Connect *conn = get_connect(con);
	if (!conn) rep->Send("no", 2); //û�д����ӣ����߷�������Ӧʧ��
	conn->start(rep);
}

class thread_connect : public thread_win
{
	TcpConnect *m_con;
public:
	thread_connect(TcpConnect *con) { m_con = con; }
	virtual void run();
};


void thread_connect::run()//����������ӵ��߳�
{
	char buf[PRO_SIZE] = { 0 };
	printf("new connection:%s,%u\n", m_con->getIP(NULL), m_con->getPort());
	while (true)
	{
		int len = m_con->Recv(buf, PRO_SIZE);
		if (len < 0) //���ӶϿ�ʱ
		{
			delete m_con;
			break;
		}
		string str(buf);
		if (str== "server")
			handle_server(m_con);//��Ϊ���������Ӵ���
		else if (str=="client")
			handle_client(m_con);//��Ϊ�ͻ������Ӵ���
		else if (str=="respond")
			handle_respond(m_con);//��Ϊ��������Ӧ�ͻ��˵����Ӵ���
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

