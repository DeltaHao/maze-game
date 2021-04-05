#include "sgfhelper.h"
#include "SDL.h"

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

int GBKToUTF8(const char* gbk, int gbklen, char* utf8, int utf8len)
{
	if (!(gbk && utf8)) {
		return 0;
	}

	//转换为宽字符
	int bufWLen = MultiByteToWideChar(CP_ACP, 0, gbk, gbklen, NULL, 0);
	if (bufWLen == 0) {
		return -1;
	}

	WCHAR* bufW = malloc(sizeof(WCHAR) * bufWLen);
	if (!bufW) {
		return -2;
	}

	int ret = MultiByteToWideChar(CP_ACP, 0, gbk, gbklen, bufW, bufWLen);
	if (ret == 0) {
		free(bufW);
		return -3;
	}

	//转换为 UTF8
	int bufMLen = WideCharToMultiByte(CP_UTF8, 0, bufW, -1, NULL, 0, NULL, NULL);
	if (bufMLen == 0) {
		free(bufW);
		return -4;
	}

	CHAR* bufM = malloc(bufMLen);
	if (!bufM) {
		free(bufW);
		return -5;
	}

	ret = WideCharToMultiByte(CP_UTF8, 0, bufW, -1, bufM, bufMLen, NULL, NULL);
	if (ret == 0) {
		free(bufW);
		free(bufM);
		return -6;
	}

	//空间不够，不截取...
	if (bufMLen > utf8len) {
		free(bufW);
		free(bufM);
		return -7;
	}

	//复制
	memset(utf8, 0, utf8len);
	memcpy(utf8, bufM, bufMLen);

	free(bufW);
	free(bufM);

	return bufMLen;
}

int randEx(int x, int y)
{
	LARGE_INTEGER seed;
	QueryPerformanceFrequency(&seed);
	QueryPerformanceCounter(&seed);
	srand(seed.QuadPart);

	return rand()%y + x;
}

int isInArea(POINT p, SDL_Rect r) {
	return p.x > r.x && p.x < r.x + r.w - 1 && p.y > r.y && p.y < r.y + r.h;
}
