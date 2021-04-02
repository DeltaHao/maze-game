#pragma once

#include <Windows.h>
#include <tchar.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SDL_RWops SDL_RWops;

/*********************************************************************************
 *									编码转换
 *********************************************************************************/


 /**
  * GBK 转 UTF8
  * 参数：
  *		gbk 需要转换的 GBK 编码字符串指针
  *		gbklen 字符串长度（需要计算结束字符 strlen(gbk) + 1)，推荐填写 -1，自动计算长度
  *		utf8 转换的结果指针
  *		utf8len 转换结果的空间大小
  *
  * 返回：
  *		返回转换后字符串的长度 （> 0)
  */

int GBKToUTF8(const char* gbk, int gbklen, char* utf8, int utf8len);
//生成随机数
int randEx(int x, int y);
#ifdef __cplusplus
}
#endif