#include "sgfhelper.h"
#include "SDL.h"

/*********************************************************************************
 *									����ת��
 *********************************************************************************/


/**
 * GBK ת UTF8
 * ������
 *		gbk ��Ҫת���� GBK �����ַ���ָ��
 *		gbklen �ַ������ȣ���Ҫ��������ַ� strlen(gbk) + 1)���Ƽ���д -1���Զ����㳤��
 *		utf8 ת���Ľ��ָ��
 *		utf8len ת������Ŀռ��С
 *
 * ���أ�
 *		����ת�����ַ����ĳ��� ��> 0)
 */

int GBKToUTF8(const char* gbk, int gbklen, char* utf8, int utf8len)
{
	if (!(gbk && utf8)) {
		return 0;
	}

	//ת��Ϊ���ַ�
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

	//ת��Ϊ UTF8
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

	//�ռ䲻��������ȡ...
	if (bufMLen > utf8len) {
		free(bufW);
		free(bufM);
		return -7;
	}

	//����
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
