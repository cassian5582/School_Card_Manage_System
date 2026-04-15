/**
 * @file record.c
 * @brief 上机记录模块实现
 */
#define _CRT_SECURE_NO_WARNINGS
#include "../Inc/record.h"
extern int duration_min;
extern Card stu_cur;
/**
 * @brief 拼接当前学生上机记录文件路径
 * @param path 存放路径字符串的缓冲区
 * @param max_len 缓冲区最大长度
 * @retval 0: 成功；1: 失败
 */
int stu_build_record_file_path(char* path, int max_len)
{
    if (path == NULL || max_len <= 0)
        return 1;

    if (!stu_is_login())
        return 1;

    if (snprintf(path, (size_t)max_len, USER_RECORD_FILE_FMT, stu_cur.ID) >= max_len)
        return 1;

    return 0;
}


/**
 * @brief 获取当前日期
 * @param date 输出日期字符串缓冲区，格式：YYYY-MM-DD
 * @retval 无
 */
void get_current_date(char* date)
{
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	sprintf(date, "%04d-%02d-%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
}

/**
 * @brief 获取当前时间
 * @param time_str 输出时间字符串缓冲区，格式：HH:MM
 * @retval 无
 */
void get_current_time(char* time_str)
{
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	sprintf(time_str, "%02d:%02d", tm->tm_hour, tm->tm_min);
}


/**
 * @brief 开始上机，写入一条“上机中”状态的记录
 * @param 无
 * @retval 0: 成功；1: 失败
 */
int start_computer()
{

	struct record r;
	printf("请输入上机地点：\n");
	str_safe_read(r.address, sizeof(r.address));
	memcpy(r.ID, stu_cur.ID, sizeof(r.ID));

	// 记录上机开始时刻
	get_current_date(r.Date);
	get_current_time(r.start_time);
	r.duration = duration_min;
    char path[256];
	if (stu_build_record_file_path(path, sizeof(path)) != 0)
	{
		printf("构建记录文件路径失败\n");
		return 1;
	}
	FILE* fp = fopen(path, "a");
	if (fp == NULL)
	{
		printf("文件打开失败\n");
		return 1;
	}

	// 追加写入记录文件
	fprintf(fp, DRCORD_STRUCT, r.ID, r.Date, r.start_time, r.duration, r.address);
	fclose(fp);
	return 0;
}


/**
 * @brief 查询并显示所有上机记录
 * @param 无
 * @retval 无
 */
void query_records()
{
	printf("\n===上机记录===\n");
    char path[256];
    if (stu_build_record_file_path(path, sizeof(path)) != 0)
	{
		printf("构建记录文件路径失败\n");
		return ;
	}
	FILE* fp = fopen(path, "r");
	if (fp == NULL)
	{
		printf("暂无上机记录\n");
		return;
	}

	struct record r;
	int count = 0;
	while (fscanf(fp, DRCORD_STRUCT, r.ID, r.Date, r.start_time, &r.duration, r.address) != EOF)
	{
		printf("卡号：%s 日期：%s 时间：%s 时长：%d 地点：%s\n",
			r.ID, r.Date, r.start_time, r.duration, r.address);
		count++;
	}
	fclose(fp);

	if (count == 0)
		printf("暂无上机记录\n");
}

/**
 * @brief 统计已结束记录的总上机时长与总费用
 * @param 无
 * @retval 无
 */
void stat_records()
{
	printf("\n===统计总上机时间和费用===\n");

	int total_min = 0;
	double total_cost = 0.0;
    char path[256];
    if (stu_build_record_file_path(path, sizeof(path)) != 0)
	{
		printf("构建记录文件路径失败\n");
		return ;
	}
	FILE* fp = fopen(path, "r");
	if (fp == NULL)
	{
		printf("暂无上机记录\n");
		return ;
	}

	struct record r;
	while (fscanf(fp, DRCORD_STRUCT, r.ID, r.Date, r.start_time, &r.duration, r.address) != EOF)
	{
		total_min += r.duration;
		
	}
	fclose(fp);

	total_cost = total_min / 60.0 * PRICE_PER_HOUR;
	printf("总上机时间：%d 分钟\n", total_min);
	printf("总费用：%.2f 元\n", total_cost);
}

