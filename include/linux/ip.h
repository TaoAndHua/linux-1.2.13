/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the IP protocol.
 *
 * Version:	@(#)ip.h	1.0.2	04/28/93
 *
 * Authors:	Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#ifndef _LINUX_IP_H
#define _LINUX_IP_H
#include <asm/byteorder.h>

/* �������ѡ�������ֶ�ֵ */
#define IPOPT_END	0
#define IPOPT_NOOP	1
#define IPOPT_SEC	130
#define IPOPT_LSRR	131
#define IPOPT_SSRR	137
#define IPOPT_RR		7
#define IPOPT_SID	136
#define IPOPT_TIMESTAMP	68


#define MAXTTL		255

/* ʱ���ѡ�����ݲ��� */
struct timestamp {
	__u8	len;                                          // ѡ���
	__u8	ptr;						      // ָ��
	union {
#if defined(LITTLE_ENDIAN_BITFIELD)
	__u8	flags:4,					      // ��־�ֶΣ����ֶ�ָ�������ʱ�������
		overflow:4;					      // ����ֶ�
#elif defined(BIG_ENDIAN_BITFIELD)
	__u8	overflow:4,
		flags:4;
#else
#error	"Please fix <asm/byteorder.h>"
#endif						
	__u8	full_char;
	} x;
	__u32	data[9];					      // ��ֻ��¼ʱ���������£�������9��ʱ�������ռ�
};


#define MAX_ROUTE	16

struct route {
  char		route_size;					// ѡ����ֶ�
  char		pointer;						// ָ���ֶ�
  unsigned long route[MAX_ROUTE];			// ·�����ݣ���ȻMAX_ROUTE����Ϊ16����ʵ��
  										// ���õ�·�����ݿռ��Ϊ37�ֽ�
};

/* 
 * �����յ���IP���ݱ�����ѡ��ʱ��
 * �ں˽��������ݱ��о����IPѡ���ʼ���ýṹ��
 * �Ա��ں���������Ĵ���
 */
struct options {
  struct route		record_route;
  struct route		loose_route;
  struct route		strict_route;
  struct timestamp	tstamp;
  unsigned short	security;
  unsigned short	compartment;
  unsigned short	handling;
  unsigned short	stream;
  unsigned		tcc;
};


struct iphdr {
#if defined(LITTLE_ENDIAN_BITFIELD)
	__u8	ihl:4,
		version:4;
#elif defined (BIG_ENDIAN_BITFIELD)
	__u8	version:4,
  		ihl:4;                      // �ײ����ȣ����ֶ���4�ֽ�Ϊ��λ�����ײ��������ɴ�15 * 4 = 60�ֽڣ����й̶�����20�ֽ�
#else
#error	"Please fix <asm/byteorder.h>"
#endif
	__u8	tos;
	__u16	tot_len;
	__u16	id;
	__u16	frag_off;
	__u8	ttl;
	__u8	protocol;
	__u16	check;
	__u32	saddr;
	__u32	daddr;
	/*The options start here. */
};


#endif	/* _LINUX_IP_H */
