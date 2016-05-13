
#include "mysql.h"

class MySql_RES;

class MySql
{
	static unsigned int COUNT;//对象实例计数
public:
	MySql();
	~MySql();

	//Connect to a server
	bool Connect(const char *host /*localhost if NULL*/, const char *user /*current user if NULL*/,
		const char *passwd, const char *db = NULL /* */, unsigned int port = 3306, const char *unix_sock = NULL, unsigned long flags = 0);
	//Perform a SQL
	bool Query(const char *stmt_str, unsigned long len = 0);
	//Get last error
	inline const char *Error(){ return mysql_error(&m_mysql); }
	//Get a result after perform a SQL
	inline MYSQL_RES* Result();
	//Check whether the connect is alived, if reConnect, the client will try to reconnect to the server
	bool CheckConnect(bool reConnect = true);
	//Close the connect
	inline void Close();
	//return the count of fields
	inline unsigned int FieldCount();
	//Get the most recently query info
	inline const char *Info(){ return mysql_info(&m_mysql); }
	//Return the rows num affected
	inline unsigned long AffectedRowNum(){ return mysql_affected_rows(&m_mysql); }

protected:
	MYSQL m_mysql;

private:
	void Init();
};

class MySql_RES
{
public:
	//MySql_RES(MYSQL_RES *res) :m_res(res){}
	MySql_RES(MySql *res);
	~MySql_RES();

	//Get the number of fields
	inline unsigned int FieldNum(){ return mysql_num_fields(m_res); }
	//Get a row data
	inline MYSQL_ROW FetchRow(){ return mysql_fetch_row(m_res); }
	//Get the data length of a field in a row
	inline unsigned long* FetchLengths(){ return mysql_fetch_lengths(m_res); }
	//No result set
	inline bool IsNull(){ return !m_res; }
	//Get the fields
	inline MYSQL_FIELD *Fields(){ return mysql_fetch_fields(m_res); }
	//Get a field by a index
	inline MYSQL_FIELD *Field(unsigned int index){ return mysql_fetch_field_direct(m_res, index); }
	//Get next field
	inline MYSQL_FIELD *FetchField(){ return mysql_fetch_field(m_res); }
	//inline unsigned long RowNum(){ mysql_num_rows(m_res); }
private:
	MYSQL_RES *m_res;
};
