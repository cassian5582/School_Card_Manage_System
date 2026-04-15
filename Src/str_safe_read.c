/**
 * @file str_safe_read.c
 * @brief 安全读取字符串的函数实现   
 */
#include "..\Inc\str_safe_read.h"

/**
 * @brief 清空 stdin 中残留的字符，防止它们自动作为下一次输入的读取内容
 * @param 无
 */
void clean_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // 循环读取直到遇到换行符或文件结束符
    }
}

/**
 * @brief 安全读取字符串的函数实现
 * @param str 存储输入字符串的缓冲区指针
 * @param max_len 缓冲区的最大长度（包括末尾的 null 字符）
 * @retval 0: 成功读取字符串；1: 发生溢出；-1: 读取失败
 */
int str_safe_read(char* str, int max_len)
{
    if (str == NULL || max_len <= 1)
    {
        printf("\n[Error] 读取输入失败！\n");
        return -1;
    }
    if (fgets(str, max_len, stdin) != NULL)
    {
        size_t len = strlen(str);
        // 去除末尾换行符
        if (len > 0 && str[len - 1] == '\n')
            str[len - 1] = '\0';
        else if (len == max_len - 1)
        {
            // 输入溢出，清空剩余输入
            clean_stdin();
            printf("\n[Warning] 发生溢出！输入过长，部分内容被截断。\n");
            return 1;
        }
        return 0;
    }
    else
    {
        printf("\n[Error] 读取输入失败！\n");
        return -1;
    }
}
