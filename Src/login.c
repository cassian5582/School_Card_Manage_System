/**
 * @file login.c
 * @brief 登录模块设计
 */
#include "..\Inc\login.h"

Card stu_cur;


 /**
  * @brief 开始菜单UI
  * @param 无
  * @retval 无
  */
void Login_Menu_UI()
{
   printf("\n=====欢迎使用模拟校园卡系统=====\n");
   printf("-00 退出系统\n");
   printf("-01 管理员登录\n");
   printf("-02 学生登录\n");
   printf("==============================\n");
   printf("请输入 0~2 选择\n");
}

/**
 * @brief 管理员登录
 * @param 无
 * @retval 1表示登录成功，0表示登录失败
 */
int admin_login()
{
   char admin_password[]= ADMIN_PASSWORD;//管理员登录密码
   char admin_password_input[10];
   //ui
   printf("\n================管理员登录========================\n");
   printf("请输入密码\n");
   if(!str_safe_read(admin_password_input, sizeof(admin_password_input)) && 
      !strcmp(admin_password_input, admin_password))
      return 1;
   else
   {
      int i = 4;
      while(--i)
      {
         printf("密码错误，请重新输入密码\n");
         printf("剩余%d次机会\n",i);
         if(!str_safe_read(admin_password_input, sizeof(admin_password_input)) && 
            !strcmp(admin_password_input, admin_password))
         {
            return 1;
            break;
               
         }
      }
      if(!i)
      {
         printf("机会耗尽，将退回至主程序\n");
         return 0;
      }
      
   }

}
/**
 * @brief 学生账号检查
 * @param p_fp_user_data 学生账号数据文件指针的二级指针
 * @param p_stu_read 读取的学生信息结构体指针
 * @param p_stu_in 用户输入的学生信息结构体指针 
 * @retval 0表示账号存在且状态正常，1表示账号不存在,-1表示状态异常
 */
int stu_account_check(FILE** p_fp_user_data,Card* p_stu_read,Card* p_stu_in)
{
   //检查账号是否存在
   int account_exist_flag = 0;
   while(1)
   {
      if(feof(*p_fp_user_data))//文件末尾，账号不存在
      {
         printf("账号不存在, 将退回至主程序\n\n");
         return 1;
         break;
      }

      if(!strcmp(p_stu_read->ID,p_stu_in->ID))//找到账号
      {
         account_exist_flag = 1;
         break;
      }  
      fread(p_stu_read,sizeof(Card),1,*p_fp_user_data);      
   }
   //判断账号状态
   if(account_exist_flag)
   {
      if(p_stu_read->state == Normal)
         return 0;
      else if(p_stu_read->state == Loss)
      {
         printf("账号已挂失, 请联系管理员\n\n");
         return -1;
      }
      else if(p_stu_read->state == Freeze)
      {
         printf("账号已冻结, 请联系管理员\n\n");
         return -1;
      }
         
   }
  
}
/**
 * @brief 学生初次登录设置密码
 * @param p_fp_user_data 学生账号数据文件指针的二级指针
 * @param p_stu_read 读取的学生信息结构体指针 
 * @retval 1: 设置成功 0: 设置失败
 */
int set_password(FILE** p_fp_user_data,Card* p_stu_read)
{
   char read_password[16];
   if(p_stu_read->password[0] == '\0')
   {
      printf("初次登录,请设置密码,14字符以内\n");
      while (1) {
         printf("请输入新密码: ");
         if (!str_safe_read(read_password, sizeof(read_password)) && strlen(read_password)!=0)
         {
            strncpy(p_stu_read->password, read_password, sizeof(p_stu_read->password));
            p_stu_read->password[sizeof(p_stu_read->password)-1] = '\0';
            // 写回文件
            fseek(*p_fp_user_data, (long)-sizeof(Card), SEEK_CUR);
            fwrite(p_stu_read, sizeof(Card), 1, *p_fp_user_data);
            fflush(*p_fp_user_data);
            printf("密码设置成功！\n");
            return 1;
         } 
         else 
         {
            static int set_password_cnt =4;
            printf("\n密码设置失败\n");
            if(--set_password_cnt)
            {
               printf("还剩%d次机会\n",set_password_cnt);
               return set_password(p_fp_user_data,p_stu_read);
            }
            else 
               return 0;
         }
      }
   }
}
/**
 * @brief 密码检查
 * @param p_fp_user_data 学生账号数据文件指针的二级指针
 * @param p_stu_read 读取的学生信息结构体指针
 * @param p_stu_in 用户输入的学生信息结构体指针
 * @retval 0-密码正确；1-密码错误，账号冻结
 */
int password_check(FILE** p_fp_user_data,Card* p_stu_read,Card* p_stu_in)
{
   printf("请输入密码\n");
   if(!str_safe_read(p_stu_in->password, sizeof(p_stu_in->password)) && 
      !strcmp(p_stu_read->password, p_stu_in->password))
   {
      printf("登录成功，欢迎%s同学\n",p_stu_read->name);
      return 0;
   }
   else
   {
      int i = 4;
      while(--i)
      {
         printf("密码错误，请重新输入密码\n");
         printf("剩余%d次机会,机会耗尽账号将被冻结\n",i);
         if(!str_safe_read(p_stu_in->password, sizeof(p_stu_in->password)) && 
            !strcmp(p_stu_read->password, p_stu_in->password))
         {
            printf("登录成功，欢迎%s同学\n",p_stu_read->name);
            return 0; 
            break; 
         }
      }
      if(!i)
      {
         printf("机会耗尽，账号将被冻结，将退回至主程序\n");
         //账号状态修改为冻结
         p_stu_read->state = Freeze;
         fseek(*p_fp_user_data,(long)-sizeof(Card),SEEK_CUR);
         fwrite(p_stu_read,sizeof(Card),1,*p_fp_user_data);
         fflush(*p_fp_user_data);
         return 1;
      }
   }
}

/**
 * @brief 学生登录
 * @param 无
 * @retval 1: 登录成功; 0: 登录失败 
*/ 
int student_login()
{
   FILE *fp_user_data = fopen(USER_DATA_FILE_PATH,"r+");
   if(!fp_user_data) return 0;//文件打开失败，返回登录界面
   else
   {
      Card stu_read;//读取学生信息
      Card stu_in;//用户输入信息
      fread(&stu_read,sizeof(Card),1,fp_user_data);
      printf("\n================学生登录=======================\n");
      printf("请输入学号\n");
      if(str_safe_read(stu_in.ID, sizeof(stu_in.ID)))
      {
         printf("学号输入失败，将退回至主程序\n");
         return 0;
      }
      //账号检查
      int account_check_result = 
         stu_account_check(&fp_user_data,&stu_read,&stu_in);
      if(!account_check_result)
      {
         //账号存在且状态正常，继续检查密码
         //新用户设置密码，并判断是否设置成功
         int a= set_password(&fp_user_data,&stu_read);
         if(!a)
            return 0;
         else
         {
            int password_check_flag =
               password_check(&fp_user_data,&stu_read,&stu_in);
            if(!password_check_flag)//密码正确
            {
               memcpy(&stu_cur, &stu_read, sizeof(Card));
               return 1;
            }
            else //错误
               return 0;
         }
         
      }
      else if(account_check_result == 1)
      {
         //账号不存在，返回登录界面
         return 0;
      }
      else if(account_check_result == -1)
      {
         //账号状态异常，返回登录界面
         return 0;
      }

      
   }

   fclose(fp_user_data);
   fp_user_data = NULL;
   return 0;
}

/**
 * @brief 登录系统
 * @param 无
 * @retval 0: 退出系统; 1: 管理员登录成功; 2: 学生登陆成功
 */
int login()
{
   Login_Menu_UI();//显示UI
   int mode_select=-1;
   if(scanf("%d",&mode_select) != 1)
   {
      clean_stdin();
      printf("输入非法，请输入数字\n");
      return login();
   }
   clean_stdin();
   switch(mode_select)
   {
      //管理员登录
      case 1:
         if(admin_login())//登录成功
         {
            return 1;
         }
         else //登录失败，重新进入系统
            login();
         break;
      //学生登录
      case 2:
         if(student_login())//登录成功
         {
            return 2;
         }
         else //登录失败，重新进入系统
            login();
         break;
      //退出系统
      case 0:
         return 0;
         break;
      //无效值，重新进入系统
      default:
         printf("请输入规定值\n");
         login();
         break;


   }
}