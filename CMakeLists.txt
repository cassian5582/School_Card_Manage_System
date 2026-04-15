cmake_minimum_required(VERSION 3.10)
project(School_Card_Manage C)

# 指定 C 标准为 C11
set(CMAKE_C_STANDARD 11)
# 设置头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/Inc)

# ========== 解决中文乱码问题 ==========
# 设置源文件编码为 UTF-8
if(MSVC)
    # 对于 Visual Studio，设置源文件使用 UTF-8 编码
    # /utf-8 选项告诉编译器将源文件和执行字符集都视为 UTF-8
    add_compile_options(/utf-8)
    # 设置执行字符集为 UTF-8（可选，/utf-8 已包含此功能）
    add_compile_definitions(_UTF8_SOURCE)
endif()

# 收集源文件
set(SOURCES
        Src/main.c
        Src/login.c
        Src/str_safe_read.c
        Src/student.c
        Src/record.c
        Src/admin.c
)
# 收集头文件
set(HEADERS
        Inc/login.h
        Inc/data_struct.h
        Inc/str_safe_read.h
        Inc/student.h
        Inc/record.h
        Inc/admin.h
)

# 生成可执行文件
add_executable(School_Card_Manage ${SOURCES} ${HEADERS})