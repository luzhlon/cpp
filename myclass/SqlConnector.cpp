
#include "assert.h"
#include "SqlConnector.h"

unsigned int MySql::COUNT = 0;

MySql::MySql()
{
	Init();
}

MySql::~MySql()
{
	mysql_close(&m_mysql);
	if (!COUNT)//���һ������
	{
		mysql_library_end();
	}
	COUNT--;//��������Լ�1
}

void MySql::Init()
{
	if (!COUNT)//��һ������
	{
		assert(mysql_library_init(0, NULL, NULL) == 0);
	}
	assert(mysql_init(&m_mysql));
	//ʹ��TCPЭ��
	unsigned int proto = MYSQL_PROTOCOL_TCP;
	mysql_options(&m_mysql, MYSQL_OPT_PROTOCOL, &proto);

	COUNT++;//��������Լ�1
}

bool MySql::Connect(const char *host /*localhost if NULL*/, const char *user /*current user if NULL*/,
		const char *passwd, const char *db , unsigned int port, const char *unix_sock, unsigned long flags)
{
	return mysql_real_connect(&m_mysql, host, user, passwd, db, port, unix_sock, flags);
}

void MySql::Close()
{
	return mysql_close(&m_mysql);
}

bool MySql::Query(const char *stmt_str, unsigned long len)
{
	bool ret = false;
	if (len)
	{
		ret = 0 == mysql_real_query(&m_mysql, stmt_str, len);
	}
	else
	{
		ret = 0 == mysql_query(&m_mysql, stmt_str);
	}
	return ret;
}

MYSQL_RES* MySql::Result()
{
	return mysql_use_result(&m_mysql);
}

unsigned int MySql::FieldCount()
{
	return mysql_field_count(&m_mysql);
}

bool MySql::CheckConnect(bool reConnect)
{
	mysql_options(&m_mysql, MYSQL_OPT_RECONNECT, &reConnect);//�����Ƿ���������
	return mysql_ping(&m_mysql) == 0;
}

MySql_RES::MySql_RES(MySql *mysql)
{
	m_res = mysql->Result();
}

MySql_RES::~MySql_RES()
{
}
