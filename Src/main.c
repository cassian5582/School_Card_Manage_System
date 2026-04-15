/**
 * @brief 模拟校园卡系统
 */
#include <windows.h>
#include "..\Inc\data_struct.h"
#include <stdio.h>
#include "..\Inc\login.h"
#include "..\Inc\student.h"
#include "..\Inc\admin.h"

int school_card_manage_system();

int main()
{
    SetConsoleOutputCP(CP_UTF8); // 设置控制台输出为 UTF-8
    SetConsoleCP(CP_UTF8);       // 设置控制台输入为 UTF-8
    
    school_card_manage_system();
    

}

int school_card_manage_system()
{
    switch(login())
    {
        case 0://退出
            printf("感谢使用校园卡系统，再见！\n");
            return 0;
            break;
        case 1://管理员
            adminMenu();
            school_card_manage_system();
            break;
        case 2://用户
            student();
            school_card_manage_system();
            break;
        default:
            printf("Error! \n");
            return 0;
            break;
    }
}