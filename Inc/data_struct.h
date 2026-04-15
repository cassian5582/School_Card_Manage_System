/**
 * @file data_struct.h
 * @brief 定义存储数据的结构
 */

#ifndef DATA_STRUCT
#define DATA_STRUCT 

#define DRCORD_STRUCT "ID: %s\tData: %s\tstart_time: %s\tduration: %d\taddress: %s\t\n"
#define USER_DATA_FILE_PATH "..\\data\\user.txt"
#define USER_RECORD_FILE_FMT "..\\log\\%s.txt"
#define ADMIN_PASSWORD "123456"
#define PRICE_PER_HOUR (1.0)


typedef enum
{
 Normal=0,Loss=1 ,Freeze=-1   
}CardState;

//学生卡信息格式
typedef struct 
{
char ID[12];//学号
char password[16];//密码
char name[10];//姓名
double remain;//余额
CardState state; //状态 :正常 0、挂失 1、冻结-1
}Card;
        

//上机记录结构体
struct record
{
	char ID[12];                  // 卡号
	char Date[11];                // 上机日期，格式：YYYY-MM-DD
	char start_time[6];           // 开始时间，格式：HH:MM
	int duration;                 // 上机用时，单位：分钟
	char address[50];             // 上机地点
};

#endif
