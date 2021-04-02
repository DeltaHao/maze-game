#pragma once

#include <Windows.h>
#include <tchar.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SDL_RWops SDL_RWops;

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

int GBKToUTF8(const char* gbk, int gbklen, char* utf8, int utf8len);
//���������
int randEx(int x, int y);
#ifdef __cplusplus
}
#endif