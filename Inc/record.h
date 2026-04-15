/**
 * @file record.h
 * @brief 上机记录模块头文件
 */

#ifndef RECORD_H
#define RECORD_H



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "student.h"

int stu_build_record_file_path(char* path, int max_len);
void get_current_date(char* date);
void get_current_time(char* time_str);
int start_computer();
void query_records();
void stat_records();
#endif
