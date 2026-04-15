/**
 * @file str_safe_read.h
 * @brief 安全读取字符串的函数实现
 */
#ifndef STR_SAFE_READ_H
#define STR_SAFE_READ_H 
#include <stdio.h>
#include <string.h>
void clean_stdin();
int str_safe_read(char* str, int max_len);
#endif