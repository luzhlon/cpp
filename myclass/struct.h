
#include "wx/wx.h"

struct data_struct
{
	wxInt16       len;//数据包的长度
	wxInt16       msg;//消息数值
	unsigned char data[8];//数据
};