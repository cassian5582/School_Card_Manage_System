/**
 * @file login.h
 * @brief Login functionality for the school card management system.
 */

#ifndef LOGIN
#define LOGIN

#include <stdio.h>
#include "data_struct.h"
#include <string.h>
#include <windows.h>
#include "str_safe_read.h"
void Login_Menu_UI();
int admin_login();
int student_login();
int set_password(FILE** p_fp_user_data,Card* p_stu_read);
int password_check(FILE** p_fp_user_data,Card* p_stu_read,Card* p_stu_in);
int login();
#endif // LOGIN

