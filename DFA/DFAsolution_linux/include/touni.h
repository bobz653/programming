/**
 * ��ĳ��������ַ���ת��ΪUnicode
 * Xu Lubing
 */
#ifndef _TO_UNI_H
#define _TO_UNI_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ���ֽ���ת����Unicode
 * ����:
 *   src:    Դ�ַ�������������encָ��
 *   srcLen: src�ĳ���
 *   uni:    ���ת�����Unicode�Ŀռ�
 *   size:   uni�ĸ�����[�������㣺size >= srcLen]
 *   enc:    src�����룬֧�ֵ�������"GBK"/"Big5"/"UTF-8"/"8859_1"
 * ����:
 *   <0: ��֧������������
 *   =0: �����Ĳ���������
 *   >0: ��uni�еĿ��õ�Unicode�ַ�����
 *   [ע]��UTF-8ת��ΪUnicodeʱ��������ڷǷ�UTF-8�ַ�����Ӧ��UnicodeֵΪ"?"
 */
int bytesToUni(const char* src, int srcLen, unsigned short* uni, int size, const char* enc);

/**
 * ���ַ���תΪUnicode�ַ�����[����]��������������ҪUnicode������ĸ��ຯ��
 * ����:
 *   us:  ����Unicode���ĵ�ַ���������0��β���ռ������Էŵ���s��Ӧ��Unicode��������Խ�紦��
 *   s:   ��Ӧ��C������ͨ�ַ�������'\0'��β
 *   enc: s��Ӧ�����룬֧�ֵ�������"GBK"/"Big5"/"UTF-8"/"8859_1"����֧�ֵ������NULL����Ϊ"8859_1"
 * ����:
 *   ָ��us��ָ��
 * [ʹ�ý���]
 *   ���������Կ���Unicode�ַ����������������"ustring.h"��ĺ���ʹ��
 *   ��: #include "ustring.h"
 *       UCHAR u[1024];
 *       ustrlen(US(u, "�й�", "GBK"));          // ��Ӧ��ǰ�� strlen("�й�")
 *       ustrstr(unistr, US(u, "�й�", "GBK"));  // ��Ӧ��ǰ�� strstr(str, "�й�")
 *   ��Ȼ���һ�������ʹ�ã�Ϊ�˼���ת�����������Ա���ת����Ľ�������ʹ��
 *       US(u, "�й�", "GBK");
 *       ustrlen(u);
 *       ustrstr(unistr, u);
 */
unsigned short* US(unsigned short* us, const char* s, const char* enc);

#ifdef __cplusplus
}
#endif

#endif
