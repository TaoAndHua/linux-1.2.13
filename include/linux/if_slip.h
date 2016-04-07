/*
 *	Swansea University Computer Society	NET3
 *	
 *	This file declares the constants of special use with the SLIP/CSLIP/
 *	KISS TNC driver.
 */
 
 /* SLIP��ȫ����Serial Line IP. ����һ���ڴ�����·�϶�IP���ݰ����з�װ�ļ���ʽ��
 * ��RFC 1055������ϸ������SLIP�����ڼ�ͥ��ÿ̨������������е�RS-232���ж˿ں�
 * ���ߵ��ƽ�����������硣
 *
 * �������������SLIP�����֡��ʽ:
 *
 * 1. IP���ݱ���һ������END(0xC0)�������ַ�������ͬʱ��Ϊ�˷�ֹ���ݱ�����֮ǰ
 * ��·�������������ݱ������ݣ������ʵ�������ݱ��Ŀ�ʼ��Ҳ����һ��END�ַ�
 * (�����·������������ôEND�ַ���������ݴ���ı��ġ�������ǰ�ı��ĵ���
 * ��ȷ�Ĵ��䣬��ǰһ���������ύ�ϲ�ʱ�����������ݺ����������������)
 *
 * 2. ���IP������ĳ���ַ�ΪEND����ô��Ҫ�������������ֽ�0xdb��0xdc��ȡ������
 * 0xdb�ַ�������SLIP��ESC�ַ�(����ͬ��ASCII�е�ESC�ַ�)��
 *
 * 3. ���IP������ĳ���ַ�ΪSLIP��ESC�ַ�����ô��Ҫ�������������ֽ�0xdb��0xdd��ȡ������
 */
 
#ifndef __LINUX_SLIP_H
#define __LINUX_SLIP_H

#define		SL_MODE_SLIP		0
#define		SL_MODE_CSLIP		1
#define 		SL_MODE_KISS		4

#define		SL_OPT_SIXBIT		2
#define		SL_OPT_ADAPTIVE		8


#endif
