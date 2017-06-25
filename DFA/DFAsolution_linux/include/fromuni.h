/**
 * ��Unicodeת��Ϊĳ��������ַ���
 * Xu Lubing
 */
#ifndef _FROM_UNI_H
#define _FROM_UNI_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ��Unicodeת�����ֽ���
 * ����:
 *   uni:    unicode��
 *   uniLen: uni�ĳ���
 *   str:    ���ת������ַ����ռ䣬������encָ��
 *   size:   str�ĸ�������������size >= uniLen*ϵ��(GBK/Big5: 2; UTF-8: 3, 8859_1: 1)
 *   enc:    ת�������룬֧�ֵ�������"GBK"/"Big5"/"UTF-8"/"8859_1"
 * ����:
 *   <0: ��֧������������
 *   =0: �����Ĳ���������
 *   >0: ��str�еĿ��õ��ַ�����
 *   [ע]��Unicodeת��ΪGBK/Big5ʱ������޶�Ӧ�����룬��Ӧ��ֵΪ"?"
 */
int uniToBytes(const unsigned short* uni, int uniLen, char* str, int size, const char* enc);

#ifdef __cplusplus
}
#endif

#endif
