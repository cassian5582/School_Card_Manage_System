/**
 * @file student.h
 * @brief 学生模块设计
 */

#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <string.h>
#include "data_struct.h"
#include "str_safe_read.h"
#include "login.h"
#include <time.h>
#include "record.h"

int stu_is_login();
void stu_UI();
int student();
int stu_duration_select();
void stu_remain_query();
void stu_time_query();
void stu_time_statistics();
void stu_fee_query();

#endif // STUDENT_H
