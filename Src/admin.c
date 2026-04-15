/**
 * @brief 管理员功能
 */
#include "../Inc/admin.h"

Card stu_admin;
/**
 * @brief 输入学号，查找学生 
 * @param ID 学号
 * @return 0 表示找到，-1 表示未找到
 */
int findStudent(const char *ID)
{
    FILE *fp_find_stu = fopen(USER_DATA_FILE_PATH, "r");
    if (!fp_find_stu)
    {
        printf("无法打开学生数据文件。\n");
        return -1;
    }
    while(fread(&stu_admin,sizeof(Card),1,fp_find_stu) == 1)
    {
        if(feof(fp_find_stu))//文件末尾，账号不存在
        {
            printf("账号不存在, 将退回至主程序\n\n");
            return -1;
            break;
        }

        if(!strcmp(ID,stu_admin.ID))//找到账号
        {
            return 0;
            break;
        }     
   }

   fclose(fp_find_stu);
}   

/**
 * @brief 管理员充值功能
 * @return 0 表示充值成功，-1 表示充值失败
 */
int adminRecharge() {
    char ID[12];
    printf("请输入学号：\n");
    str_safe_read(ID, sizeof(ID));
    double amount;
    printf("\n========== 充值功能 ==========\n");
    if(findStudent(ID))
    {
        return -1;
    }
    printf("学生姓名：%s\n", stu_admin.name);
    printf("当前余额：%.2f 元\n", stu_admin.remain);
    printf("请输入充值金额：");
    scanf("%lf", &amount);   
    clean_stdin();
    if (amount <= 0) {
        printf("充值金额必须大于0！\n");
        printf("将退出至主程序\n");
        return -1;
    }   
    stu_admin.remain += amount;
    data_rewrite(ID);
    printf("充值成功！当前余额：%.2f 元\n", stu_admin.remain);

}

/**
 * @brief 管理员解冻/解挂功能
 * @return 0 表示操作成功，-1 表示操作失败
 */
int adminLostCard() {
    char ID[12];
    printf("请输入学号：\n");
    str_safe_read(ID, sizeof(ID));
    int choice;    
    printf("\n========== 挂失/解挂功能 ==========\n");
    if(findStudent(ID))
    {
        return -1;
    }
    printf("学生姓名：%s\n", stu_admin.name);
    switch(stu_admin.state)
    {
        case 0:
            printf("当前状态：正常\n");
            return 0;
            break;
        case 1:
            printf("当前状态：已挂失\n");
            printf("是否要解挂？(1-是, 0-否): ");
            scanf("%d", &choice);
            clean_stdin();
            if (choice == 1) {
                stu_admin.state = Normal;
                data_rewrite(ID);
                printf("解挂成功！该卡片已恢复正常使用。\n");
            }
            break;
        case Freeze:
            printf("当前状态：已冻结\n");
            printf("是否要解冻？(1-是, 0-否): ");
            scanf("%d", &choice);
            clean_stdin();
            if (choice == 1) {
                stu_admin.state = Normal;
                data_rewrite(ID);
                printf("解冻成功！该账户已恢复正常使用。\n");
                printf("请前去重置密码\n");
                adminResetPassword();
            }
            break;
        default:
            printf("状态异常，自动改为冻结\n");
            stu_admin.state = Freeze;
            data_rewrite(ID);
            printf("是否要解冻？(1-是, 0-否): ");
            scanf("%d", &choice);
            clean_stdin();
            if (choice == 1) {
                stu_admin.state = Normal;
                data_rewrite(ID);
                printf("解冻成功！该账户已恢复正常使用。\n");
                printf("请前去重置密码\n");
                adminResetPassword();
            }
            break;
    }
    
}
/**
 * @brief 密码重置
 * @return 0 表示重置成功，-1 表示重置失败
 */
int adminResetPassword() {
    printf("\n========== 重置密码功能 ==========\n");
    char ID[12];
    printf("请输入学号：\n");
    str_safe_read(ID, sizeof(ID));
    if(findStudent(ID))
    {
        return -1;
    }
    char newPassword[16];
    printf("学生姓名：%s\n", stu_admin.name);
    printf("当前密码：%s\n", stu_admin.password);
    printf("请输入新密码：");
    str_safe_read(newPassword, sizeof(newPassword));
    strcpy(stu_admin.password, newPassword);
    stu_admin.state = 0;           // 重置密码时自动解冻   
    data_rewrite(ID);
    printf("密码重置成功！新密码：%s\n", newPassword);
    printf("注意：账户已自动解冻。\n");
    return 0;
}

int data_rewrite(const char *ID)
{
    FILE *fp_rewrite_stu = fopen(USER_DATA_FILE_PATH, "r+");
    if (!fp_rewrite_stu)
    {
        printf("无法打开学生数据文件。\n");
        return -1;
    }
    while(1)
    {
        if(feof(fp_rewrite_stu))//文件末尾，账号不存在
        {
            printf("账号不存在, 将退回至主程序\n\n");
            return -1;
            break;
        }

        if(!strcmp(ID,stu_admin.ID))//找到账号
        {
            
            break;
        }  
        fread(&stu_admin,sizeof(Card),1,fp_rewrite_stu);      
        return 0;
   }

   //向前移动指针
   fseek(fp_rewrite_stu, -(long)sizeof(Card), SEEK_CUR);
   fwrite(&stu_admin, (size_t)sizeof(Card), 1, fp_rewrite_stu);

   fclose(fp_rewrite_stu);
    // 实现数据重写功能
    return 0;
}

// 管理员菜单
void adminMenu() {
    int choice;
    printf("\n========== 管理员菜单 ==========\n");
    printf("1. 充值\n");
    printf("2. 解冻/解挂\n");
    printf("3. 重置学生密码\n");
    printf("4. 退出登录\n");
    printf("================================\n");
    printf("请选择：");
    scanf("%d", &choice);
    clean_stdin();
    switch (choice) {
        case 1:
            adminRecharge();
            adminMenu();
            
            break;
        case 2:
            adminLostCard();
            adminMenu();
            break;
        case 3:
            adminResetPassword();
            adminMenu();
            break;
        case 4:
            printf("管理员已退出登录。\n");
            return;
        default:
            printf("无效选择！\n");
            adminMenu();
            break;
    }
    
}