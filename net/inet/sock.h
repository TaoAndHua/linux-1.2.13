/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the AF_INET socket handler.
 *
 * Version:	@(#)sock.h	1.0.4	05/13/93
 *
 * Authors:	Ross Biro, <bir7@leland.Stanford.Edu>
 *		Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *		Corey Minyard <wf-rch!minyard@relay.EU.net>
 *		Florian La Roche <flla@stud.uni-sb.de>
 *
 * Fixes:
 *		Alan Cox	:	Volatiles in skbuff pointers. See
 *					skbuff comments. May be overdone,
 *					better to prove they can be removed
 *					than the reverse.
 *		Alan Cox	:	Added a zapped field for tcp to note
 *					a socket is reset and must stay shut up
 *		Alan Cox	:	New fields for options
 *	Pauline Middelink	:	identd support
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#ifndef _SOCK_H
#define _SOCK_H

#include <linux/timer.h>
#include <linux/ip.h>		/* struct options */
#include <linux/tcp.h>		/* struct tcphdr */
#include <linux/config.h>

#include <linux/skbuff.h>	/* struct sk_buff */
#include "protocol.h"		/* struct inet_protocol */
#ifdef CONFIG_AX25
#include "ax25.h"
#endif
#ifdef CONFIG_IPX
#include "ipx.h"
#endif
#ifdef CONFIG_ATALK
#include <linux/atalk.h>
#endif

#include <linux/igmp.h>

#define SOCK_ARRAY_SIZE	256		/* Think big (also on some systems a byte is faster */


/*
 * This structure really needs to be cleaned up.
 * Most of it is for TCP, and not used by any of
 * the other protocols.
 */
 /* ��INET�㣬��ʾ�׽��� */
struct sock {
  struct options			*opt;			// IPѡ���(�յ����ݰ������ýṹ�������������ʹ��)
  volatile unsigned long		wmem_alloc;	// ��ǰ�ѷ���д�������Ĵ�С
  volatile unsigned long		rmem_alloc;		// ��ǰ�ѷ�����������Ĵ�С

  /* TCP�ɿ�����ʹ�õ����к�	*/
  unsigned long			write_seq;		// Ӧ�ó����´���TCP���ͻ�����д�������е�һ���ֽڵı��
  unsigned long			sent_seq;		// TCP���������ݵĵ�һ���ֽڱ�ţ���ӦTCP�ײ���"���к�"
  unsigned long			acked_seq;		// TCP�ȴ��������ݵĵ�һ���ֽڱ�ţ���ӦTCP�ײ���"ȷ�Ϻ�"
  unsigned long			copied_seq;		// copied_seq֮ǰ�����Ѿ����û������ȡ
  unsigned long			rcv_ack_seq;	// �ѱ�ȷ���ֽڵ���һ���ֽڱ�ţ�rcv_ack_seq=2312��ʾ2312֮ǰ���ֽ��Ѿ�����ȷ����
  unsigned long			window_seq;	// window_seq=sent_seq+window(�Զ�ͨ�洰�ڴ�С)����ʾwindow_seq֮ǰ���ֽ������ǿ��Է��͵ģ�֮�����Ϊ���ڴ�С�����ܷ���
  unsigned long			fin_seq;			// �ڽ��յ��Է�����FIN���ݰ��󣬼�¼�Ը����ݰ�����ȷ�ϵ�"ȷ�Ϻ�"

  /* TCP�������ݴ���	*/
  unsigned long			urg_seq;		// �������ݵ�������к�
  unsigned long			urg_data;		// ��־λ����Ϊ1ʱ��ʾ���յ���������

  /*
   * Not all are volatile, but some are, so we
   * might as well say they all are.
   */
  volatile char			inuse,			// inuse=1 ������������ʹ�ø�sock�ṹ����ǰ������Ҫ�ȴ����൱��sock�ṹ����
					dead,			// dead=1 ��ǰsock�ṹ�Ѿ������ͷ�״̬
					urginline,		// urginlin=1 �������ݱ�������ͨ���ݽ��д���
					intr,
					blog,			// blog=1 �׽��ִ��ڽ���״̬����ʱ�յ������ݱ���������
					done,
					reuse,
					keepopen,		// keepopen=1 ʹ�ñ��ʱ��
					linger,			// linger=1	�ر��׽���ʱ��������Ҫ�ȴ��׽��ֹرջ��߳�ʱ(lingertime)�󷵻�
					delay_acks,		// delay_acks=1 ��ʱӦ��
					destroy,			// destory=1 ��sock�ṹ�ȴ�����
					ack_timed,
					no_check,		// TCP_NO_CHECK
					zapped,			// In ax25 & ipx means not linked����TCP����Ϊ"��λ��־λ"��zapped=1��ʾ�յ���λ���ݰ�
					broadcast,
					nonagle;		// �Ƿ����TCP nagle�㷨
  unsigned long		lingertime;		// �׽��ֹر�ʱ���رղ�����lingertime����Ȼδ��ɣ��������ء�ֻ����linger��־λ��Чʱ������

  int					proc;			// ��sock�ṹ��������pid

  /* ����3���ֶΣ�����sock֮������	*/
  struct sock			*next;
  struct sock			*prev; 		/* Doubly linked chain.. */
  struct sock			*pair;		/* �μ�:inet_accept����
  								 * ����ϴ�inet_acceptʱ�����������ķ�ʽ���յģ��ڵȴ�����������ӵĹ����У����źŴ���ˣ�
  								 * ��Ҫ��֮ǰ����sk_prot->accept���յ�������newsk����pair�ֶ����ݴ档�ٴε���inet_acceptʱ
  								 * ���ȴ���pair�ֶ�
  								 */

  /* TCP�ط�����	*/
  struct sk_buff	* volatile send_head;	// �ط�����ͷָ��
  struct sk_buff	* volatile send_tail;	// �ط�����βָ��

  /* TCP�󱸽��ն��У���sock�ṹ��ĳ��������������ʱͨ���жϽ��յ������ݾͻ���ص�back_log������
  * ����back_log�����е�����ʵ���ϲ�û�б�ȷ�ϣ��ڴ�back_log�Ƶ�reveive_queue�����ݲ��㱻��ʽ���գ�����Ӧ��
  * ����receive_queue����ת��ʱ��������Ϊ�������ջ�������С���ޣ���ֱ�Ӷ�������,��ʹ�÷��Ͷ˳�ʱ�ط�
  */
  struct sk_buff_head	back_log;		// �Ѿ������浫��δ��ȷ�ϵ����ݣ�����˵��δ��TCPЭ�鴦�������
									// ���ݵ���ʱ����ǰsock����������ֱ�ӷŵ��󱸶�����
  struct sk_buff	    * partial;			// ���ڴ�����󳤶ȵĴ��������ݰ�.ĳЩ����»س��ֶ��partial buffer���μ�tcp_dequeue_partialע��
  struct timer_list		partial_timer;	// ��ʱ�������ڷ���partialָ������ݰ�����ֹ���ݻ���(�ȴ�)ʱ�����
  
  long				retransmits;		// ��¼�ط�����

  /* ���ݷ���/���ն��� 	*/
  struct sk_buff_head	write_queue,	// Ӧ�ó����Ѿ����͵�TCPЭ������Ŷӵ�����
					receive_queue;	// ��ǰ�����Ѿ����ղ�ȷ�Ϲ������ݣ������ǰ����ʱ�������ӣ���ôҲ����������ӵ��׽���(�μ�destroy_sock����)
					
  struct proto			*prot;			// ��������ʱ��INET���ô����Ľӿڼ���
  
  struct wait_queue	**sleep;			// ������sock�ϵĽ��̶��С�inet_release()�У����linger=1ʱ���п��ܵ���sock_release�Ľ��̾ͻ������ڸ�sleep�����ϣ��ȴ��رղ������
  
  unsigned long		daddr;			// �Զ˵�ַ
  unsigned long		saddr;			// ���ص�ַ
  unsigned short		max_unacked;	// ����ѽ���(received_queue)����δ����ȷ�ϵ������ֽ���
  unsigned short		window;			// �Զ�ͨ��Ĵ��ڴ�С
  unsigned short		bytes_rcv;		// ���������ѽ����ֽ�����������
/* mss is min(mtu, max_window) */
  unsigned short		mtu;			/* mss negotiated in the syn's 		���ӽ���ʱЭ�̵�����䵥Ԫ			*/
  volatile unsigned short	mss;       		/* current eff. mss - can change 	����ĳ��� = mtu - IP�ײ� - tcp�ײ�	*/
  volatile unsigned short	user_mss;  		/* mss requested by user in ioctl	�û�ָ����mssֵ							*/

  /* ��󴰿ڴ�С�ʹ��ڴ�Сǯ��ֵ	*/
  volatile unsigned short	max_window;	// �Զ�����ͨ�����󴰿ڴ�С  Nagle���и����������Ͷ��ۼ����ݴ���max_window>>1ʱ�����̷���
  unsigned long 		window_clamp;	
  
  unsigned short		num;			// ���ض˿ں�

  /* ����3���ֶ�����ӵ�������㷨	*/
  volatile unsigned short	cong_window;
  volatile unsigned short	cong_count;
  volatile unsigned short	ssthresh;

  volatile unsigned short	packets_out;	// �Ѿ�����������δ�õ�Ӧ������ݰ���Ŀ
  volatile unsigned short	shutdown;		// ���عرձ�־λ�����ڰ�رղ���

  /* 							*/
  volatile unsigned long	rtt;				// ����ʱ�����ֵ
  volatile unsigned long	mdev;			// mean deviation����RTTD,����ƫ��
  volatile unsigned long	rto;				// ͨ��rtt��mdev����ó����ӳ�ʱ��
  
/* currently backoff isn't used, but I'm maintaining it in case
 * we want to go back to a backoff formula that needs it
 */
  volatile unsigned short	backoff;			// �˱��㷨����ֵ(��ǰδʹ��)
  volatile short			err;				// ���������һ�γ�����Ϣ
  unsigned char		protocol;		// �����Э��ֵ
  volatile unsigned char	state;			// �׽���״̬����TCP_ESTABLISHED��inet_create()������ʼ��state=TCP_CLOSED
  volatile unsigned char	ack_backlog;	// �����δӦ�����ݰ���Ŀ
  unsigned char		max_ack_backlog; 	// �ѷ��͵�δ��ȷ�ϵ����ݱ���������(�μ�inet_create()�����У�Ӣ��ע��)
  
  unsigned char		priority;			// ���׽������ȼ�����Ӳ�����淢�����ݰ�ʱʹ��

  unsigned char		debug;
  unsigned short		rcvbuf;			// �����ջ�������С
  unsigned short		sndbuf;			// ����ͻ�������С
  unsigned short		type;			// �׽������ͣ���SOCK_STREAM
  
  unsigned char		localroute;		/* Route locally only ��Ϊ1ʱ��ʾֻʹ�ñ���·�ɣ�һ��Ŀ�Ķ�����ͬ����ʱʹ��	*/
#ifdef 0	/* ������IPЭ�� */
#ifdef CONFIG_IPX
  ipx_address			ipx_dest_addr;
  ipx_interface			*ipx_intrfc;
  unsigned short		ipx_port;
  unsigned short		ipx_type;
#endif
#ifdef CONFIG_AX25
/* Really we want to add a per protocol private area */
  ax25_address		ax25_source_addr,ax25_dest_addr;
  struct sk_buff *volatile ax25_retxq[8];
  char				ax25_state,ax25_vs,ax25_vr,ax25_lastrxnr,ax25_lasttxnr;
  char				ax25_condition;
  char				ax25_retxcnt;
  char				ax25_xx;
  char				ax25_retxqi;
  char				ax25_rrtimer;
  char				ax25_timer;
  unsigned char		ax25_n2;
  unsigned short		ax25_t1,ax25_t2,ax25_t3;
  ax25_digi			*ax25_digipeat;
#endif  
#ifdef CONFIG_ATALK
  struct atalk_sock		at;
#endif
#endif

  /* IP 'private area' or will be eventually */
  /* IPЭ���˽������	*/
  int				ip_ttl;				/* TTL setting */
  int				ip_tos;				/* TOS */
  
  struct tcphdr	dummy_th;			// ����TCP�ײ����������ݰ�ʱʹ�ø��ֶο��Կ��ٴ���TCPͷ��

  /* TCPЭ��ʹ�õĶ�ʱ��	*/
  struct timer_list	keepalive_timer;	/* TCP keepalive hack		���ʱ�� ��������̽��Է����ڴ�С����ֹ�Է��Ĵ���ͨ�涪ʧ*/
  struct timer_list	retransmit_timer;	/* TCP retransmit timer 	�ش���ʱ�� �������ݰ���ʱ	*/
  struct timer_list	ack_timer;			/* TCP delayed ack timer	�ӳ�ȷ�϶�ʱ��	 */

  /*	��־λ����ֶΣ����ڱ�ʾ������ͨ�ö�ʱ��timer�ĳ�ʱԭ��	*/
  int				ip_xmit_timeout;	/* Why the timeout is running */
  
#ifdef CONFIG_IP_MULTICAST  
  int				ip_mc_ttl;			/* Multicasting TTL */
  int				ip_mc_loop;			/* Loopback (not implemented yet) */
  char				ip_mc_name[MAX_ADDR_LEN];	/* Multicast device name */
  struct ip_mc_socklist		*ip_mc_list;			/* Group array */
#endif  

  /* This part is used for the timeout functions (timer.c). */
  int				timeout;		/* What are we waiting for? ��ʱԭ�� */
  struct timer_list	timer;		/* This is the TIME_WAIT/receive timer when we are doing IP */
  
  struct timeval	stamp;

  /* identd */
  struct socket	*socket;		// ָ���Ӧ��socket�ṹ
  
  /* Callbacks ��inet_create()�����и�ֵĬ��inet�ص�����	*/
  void			(*state_change)(struct sock *sk);			// default_callback1()
  void			(*data_ready)(struct sock *sk,int bytes);	// default_callback2()
  void			(*write_space)(struct sock *sk);			// default_callback3()
  void			(*error_report)(struct sock *sk);			// default_callback1()
};


/** 
 * Э��ص��þ���Ĵ����Э��Ľӿ�
 * 
 * �����Э�鶼�ᶨ��һ��struct proto
 * �ṹ�а���������ṩ�Ľӿڣ��Լ���ÿ�������Э���ר������
 * ��: tcp_prot / udp_prot / raw_prot
 */
struct proto {
  struct sk_buff *	(*wmalloc)(struct sock *sk,
						    unsigned long size, int force,
						    int priority);
  struct sk_buff *	(*rmalloc)(struct sock *sk,
						    unsigned long size, int force,
						    int priority);
  void			(*wfree)(struct sock *sk, struct sk_buff *skb,
						    unsigned long size);
  void			(*rfree)(struct sock *sk, struct sk_buff *skb,
						    unsigned long size);
  unsigned long	(*rspace)(struct sock *sk);
  unsigned long	(*wspace)(struct sock *sk);
  void			(*close)(struct sock *sk, int timeout);
  int				(*read)(struct sock *sk, unsigned char *to,
						int len, int nonblock, unsigned flags);
  int				(*write)(struct sock *sk, unsigned char *to,
						 int len, int nonblock, unsigned flags);
  int				(*sendto)(struct sock *sk,
				     	         unsigned char *from, int len, int noblock,
						  unsigned flags, struct sockaddr_in *usin,
						  int addr_len);
  int				(*recvfrom)(struct sock *sk,
						    unsigned char *from, int len, int noblock,
						    unsigned flags, struct sockaddr_in *usin,
						    int *addr_len);
  int				(*build_header)(struct sk_buff *skb,
							unsigned long saddr,
							unsigned long daddr,
							struct device **dev, int type,
							struct options *opt, int len, int tos, int ttl);
  int				(*connect)(struct sock *sk,
						  struct sockaddr_in *usin, int addr_len);
  struct sock *	(*accept) (struct sock *sk, int flags);
  void			(*queue_xmit)(struct sock *sk,
							      struct device *dev, struct sk_buff *skb,
							      int free);
  void			(*retransmit)(struct sock *sk, int all);
  void			(*write_wakeup)(struct sock *sk);
  void			(*read_wakeup)(struct sock *sk);
	  int			(*rcv)(struct sk_buff *buff, struct device *dev,
					       struct options *opt, unsigned long daddr,
					       unsigned short len, unsigned long saddr,
					       int redo, struct inet_protocol *protocol);
  int			(*select)(struct sock *sk, int which,
				  select_table *wait);
  int			(*ioctl)(struct sock *sk, int cmd,
				 unsigned long arg);
  int			(*init)(struct sock *sk);
  void		(*shutdown)(struct sock *sk, int how);
  int			(*setsockopt)(struct sock *sk, int level, int optname,
  						 char *optval, int optlen);
  int			(*getsockopt)(struct sock *sk, int level, int optname,
  						char *optval, int *option);  	 
  
  unsigned short	max_header;
  unsigned long	retransmits;
  struct sock *	sock_array[SOCK_ARRAY_SIZE];	// ���˿ں���֯ʹ�øô����Э���sock
  char			name[80];
  int				inuse, 			// ʹ�ø�Э���sock����
  				highestinuse;
};

#define TIME_WRITE	1
#define TIME_CLOSE	2
#define TIME_KEEPOPEN	3
#define TIME_DESTROY	4
#define TIME_DONE	5	/* used to absorb those last few packets */
#define TIME_PROBE0	6
#define SOCK_DESTROY_TIME 1000	/* about 10 seconds			*/

#define PROT_SOCK	1024	/* Sockets 0-1023 can't be bound too unless you are superuser */

#define SHUTDOWN_MASK	3
#define RCV_SHUTDOWN	1
#define SEND_SHUTDOWN	2


extern void			destroy_sock(struct sock *sk);
extern unsigned short		get_new_socknum(struct proto *, unsigned short);
extern void			put_sock(unsigned short, struct sock *); 
extern void			release_sock(struct sock *sk);
extern struct sock		*get_sock(struct proto *, unsigned short,
					  unsigned long, unsigned short,
					  unsigned long);
extern struct sock		*get_sock_mcast(struct sock *, unsigned short,
					  unsigned long, unsigned short,
					  unsigned long);
extern struct sock		*get_sock_raw(struct sock *, unsigned short,
					  unsigned long, unsigned long);

extern struct sk_buff		*sock_wmalloc(struct sock *sk,
					      unsigned long size, int force,
					      int priority);
extern struct sk_buff		*sock_rmalloc(struct sock *sk,
					      unsigned long size, int force,
					      int priority);
extern void			sock_wfree(struct sock *sk, struct sk_buff *skb,
					   unsigned long size);
extern void			sock_rfree(struct sock *sk, struct sk_buff *skb,
					   unsigned long size);
extern unsigned long		sock_rspace(struct sock *sk);
extern unsigned long		sock_wspace(struct sock *sk);

extern int			sock_setsockopt(struct sock *sk,int level,int op,char *optval,int optlen);

extern int			sock_getsockopt(struct sock *sk,int level,int op,char *optval,int *optlen);
extern struct sk_buff 		*sock_alloc_send_skb(struct sock *skb, unsigned long size, int noblock, int *errcode);
extern int			sock_queue_rcv_skb(struct sock *sk, struct sk_buff *skb);

/* declarations from timer.c */
extern struct sock *timer_base;

void delete_timer (struct sock *);
void reset_timer (struct sock *, int, unsigned long);
void net_timer (unsigned long);


#endif	/* _SOCK_H */
