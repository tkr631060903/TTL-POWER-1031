#ifndef _AES_H
#define _AES_H


// ��bitΪ��λ����Կ���ȣ�ֻ��Ϊ 128��192 �� 256 ����
#define AES_KEY_LENGTH	128

// �ӽ���ģʽ
#define AES_MODE_ECB	0				// �������뱾ģʽ��һ��ģʽ��
#define AES_MODE_CBC	1				// �����������ģʽ
#define AES_MODE		AES_MODE_CBC


///
//	��������	aes_init
//	������		��ʼ�����ڴ�ִ����չ��Կ������
//	���������	pKey -- ԭʼ��Կ���䳤�ȱ���Ϊ AES_KEY_LENGTH/8 �ֽڡ�
//	���������	�ޡ�
//	����ֵ��	�ޡ�
///
void aes_init(const void *pKey);

//
//	��������	aes_encrypt
//	������		��������
//	���������	pPlainText	-- ���ģ�������ܵ����ݣ��䳤��ΪnDataLen�ֽڡ�
//				nDataLen	-- ���ݳ��ȣ����ֽ�Ϊ��λ������ΪAES_KEY_LENGTH/8����������
//				pIV			-- ��ʼ�����������ʹ��ECBģʽ������ΪNULL��
//	���������	pCipherText	-- ���ģ��������ļ��ܺ�����ݣ�������pPlainText��ͬ��
//	����ֵ��	�ޡ�
//
void aes_encrypt(const unsigned char *pPlainText, unsigned char *pCipherText, 
				 unsigned int nDataLen, const unsigned char *pIV);

//
//	��������	aes_decrypt
//	������		��������
//	���������	pCipherText -- ���ģ�������ܵ����ݣ��䳤��ΪnDataLen�ֽڡ�
//				nDataLen	-- ���ݳ��ȣ����ֽ�Ϊ��λ������ΪAES_KEY_LENGTH/8����������
//				pIV			-- ��ʼ�����������ʹ��ECBģʽ������ΪNULL��
//	���������	pPlainText  -- ���ģ��������Ľ��ܺ�����ݣ�������pCipherText��ͬ��
//	����ֵ��	�ޡ�
//
void aes_decrypt( const unsigned char *pCipherText,unsigned char *pPlainText, 
				 unsigned int nDataLen, const unsigned char *pIV);
#endif  /* _AES_H */
