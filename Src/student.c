/**
 * @file student.c
 * @brief 学生模块实现
 */
#include "../Inc/student.h"

extern Card stu_cur;
int duration_min;
/**
 * @brief 检查当前是否有学生登录
 * @param 无
 * @retval 1: 已登录；0: 未登录
 */
int stu_is_login()
{
    if (stu_cur.ID[0] == '\0')
    {
        printf("当前无学生登录，请先完成登录\n");
        return 0;
    }
    return 1;
}


/**
 * @brief 将当前学生余额同步写回用户数据文件
 * @param 无
 * @retval 0: 写回成功；1: 写回失败
 */
int stu_sync_remain_to_file()
{
    FILE* fp_user_data = fopen(USER_DATA_FILE_PATH, "r+b");
    if (!fp_user_data)
    {
        printf("用户数据文件打开失败，无法更新余额\n");
        return 1;
    }

    Card stu_read;
    while (fread(&stu_read, sizeof(Card), 1, fp_user_data) == 1)
    {
        if (!strcmp(stu_read.ID, stu_cur.ID))
        {
            stu_read.remain = stu_cur.remain;
            fseek(fp_user_data, (long)-sizeof(Card), SEEK_CUR);
            fwrite(&stu_read, sizeof(Card), 1, fp_user_data);
            fflush(fp_user_data);
            fclose(fp_user_data);
            return 0;
        }
    }

    fclose(fp_user_data);
    printf("未找到当前学生账号，余额更新失败\n");
    return 1;
}


/**
 * @brief 显示学生系统UI
 * @param 无
 * @retval 无
 */
void stu_UI()
{
    printf("\n================欢迎来到学生系统==================\n");
    printf(" 00- 退出学生系统\n");
    printf(" 01- 本次上机时长选择\n");
    printf(" 02- 余额查询\n");
    printf(" 03- 上机记录查询\n");
    printf(" 04- 上机时间、费用统计\n");
    printf("===================================================\n");
    printf("请输入 0~4 选择\n");
}

/**
 * @brief 本次上机时长选择模块
 * @param 无
 * @retval 0: 成功或主动返回；1: 执行失败
 */
int stu_duration_select()
{
    if (!stu_is_login())
        return 1;

    int mode_select = -1;
    duration_min = 0;
    double cur_fee = 0.0;
    double backup_remain = 0.0;

    printf("\n================本次上机时长选择==================\n");
    printf(" 00- 返回\n");
    printf(" 01- 30分钟\n");
    printf(" 02- 60分钟\n");
    printf(" 03- 90分钟\n");
    printf(" 04- 120分钟\n");
    printf(" 05- 自定义时长（单位：分钟）\n");
    printf("计费标准：%.2f 元/小时\n", PRICE_PER_HOUR);
    printf("请输入 0~5 选择\n");

    if (scanf("%d", &mode_select) != 1)
    {
        clean_stdin();
        printf("输入非法，请输入数字\n");
        return 1;
    }
    clean_stdin();

    switch (mode_select)
    {
        case 0:
            return 0;
        case 1:
            duration_min = 30;
            break;
        case 2:
            duration_min = 60;
            break;
        case 3:
            duration_min = 90;
            break;
        case 4:
            duration_min = 120;
            break;
        case 5:
            printf("请输入本次上机时长（分钟，1~720）\n");
            if (scanf("%d", &duration_min) != 1)
            {
                clean_stdin();
                printf("输入非法，请输入整数\n");
                return 1;
            }
            clean_stdin();
            if (duration_min <= 0 || duration_min > 720)
            {
                printf("时长超出范围，设置失败\n");
                return 1;
            }
            break;
        default:
            printf("请输入规定范围内的菜单选项\n");
            return 1;
    }

    cur_fee = (double)duration_min * PRICE_PER_HOUR / 60.0;
    if (stu_cur.remain < cur_fee)
    {
        printf("余额不足，本次上机需要 %.2f 元，当前余额 %.2f 元\n", cur_fee, stu_cur.remain);
        return 1;
    }

    backup_remain = stu_cur.remain;
    stu_cur.remain -= cur_fee;

    if (stu_sync_remain_to_file())
    {
        stu_cur.remain = backup_remain;
        return 1;
    }

    if (start_computer())
    {
        /* 记录写入失败时进行余额回滚 */
        stu_cur.remain = backup_remain;
        stu_sync_remain_to_file();
        printf("上机记录写入失败，已取消本次扣费\n");
        return 1;
    }

    printf("时长设置成功：%d 分钟，本次费用 %.2f 元，当前余额 %.2f 元\n",
           duration_min, cur_fee, stu_cur.remain);
    return 0;
}

/**
 * @brief 余额查询模块
 * @param 无
 * @retval 无
 */
void stu_remain_query()
{
    printf("\n================余额查询==================\n");
    if (!stu_is_login())
        return;

    printf("学号：%s\n", stu_cur.ID);
    printf("姓名：%s\n", stu_cur.name);
    printf("当前余额：%.2f 元\n", stu_cur.remain);
}

/**
 * @brief 学生功能调度入口
 * @param 无
 * @retval 0: 退出学生系统
 */
int student()
{
    stu_UI();

    int mode_select = -1;
    if (scanf("%d", &mode_select) != 1)
    {
        clean_stdin();
        printf("请输入规定值\n");
        return student();
    }
    clean_stdin();

    switch (mode_select)
    {
        case 0:
            printf("感谢使用学生系统，再见！\n");
            return 0;
        case 1:
            stu_duration_select();
            return student();
        case 2:
            stu_remain_query();
            return student();
        case 3:
            query_records();
            return student();
        case 4:
            stat_records();
            return student();
        default:
            printf("请输入规定值\n");
            return student();
    }
}
