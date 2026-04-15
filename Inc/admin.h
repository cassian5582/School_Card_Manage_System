#ifndef ADMIN_H
#define ADMIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "str_safe_read.h"
#include "data_struct.h"

// 全局变量声明

// 函数声明
int findStudent(const char *ID);
void adminMenu();
int data_rewrite(const char *ID);
int adminResetPassword() ;
#endif