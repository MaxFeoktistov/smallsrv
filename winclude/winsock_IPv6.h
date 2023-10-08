/* WINSOCK.H--definitions to be used with the WINSOCK.DLL
 * Copyright (c) 1993-1996, Microsoft Corp. All rights reserved.
 *
 * This header file corresponds to version 1.1 of the Windows Sockets specification.
 *
 * This file includes parts which are Copyright (c) 1982-1986 Regents
 * of the University of California.  All rights reserved.  The
 * Berkeley Software License Agreement specifies the terms and
 * conditions for redistribution.
 *
 */

#ifndef _WINSOCKAPI_IPv6_
#define _WINSOCKAPI_IPv6_


typedef unsigned char   u_int8_t ;
typedef unsigned short  u_int16_t;
typedef unsigned long   u_int32_t;


/*
 * IPv6 address
 */
struct in6_addr {
        union {
                u_int8_t   __u6_addr8[16];
                u_int16_t  __u6_addr16[8];
                u_int32_t  __u6_addr32[4];
        } __u6_addr;                    /* 128-bit IP6 address */
};

#define s6_addr   __u6_addr.__u6_addr8
#define s6_addr8  __u6_addr.__u6_addr8
#define s6_addr16 __u6_addr.__u6_addr16
#define s6_addr32 __u6_addr.__u6_addr32

#define INET6_ADDRSTRLEN        46

/*
 * Socket address for IPv6
 */
#ifndef _XOPEN_SOURCE
#define SIN6_LEN
#endif

struct sockaddr_in6 {
    short   sin6_family;        /* AF_INET6 */
    u_short sin6_port;          /* Transport level port number */
    u_long  sin6_flowinfo;      /* IPv6 flow information */
    struct in6_addr sin6_addr;  /* IPv6 address */
    u_long sin6_scope_id;       /* set of interfaces for a scope */
};


/*
 * Local definition for masks
 */
#ifdef _KERNEL  /*XXX nonstandard*/
#define IN6MASK0        {{{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }}}
#define IN6MASK32       {{{ 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, \
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}}
#define IN6MASK64       {{{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}}
#define IN6MASK96       {{{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
                            0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 }}}
#define IN6MASK128      {{{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }}}
#endif

#ifdef _KERNEL
extern const struct sockaddr_in6 sa6_any;

extern const struct in6_addr in6mask0;
extern const struct in6_addr in6mask32;
extern const struct in6_addr in6mask64;
extern const struct in6_addr in6mask96;
extern const struct in6_addr in6mask128;
#endif /* _KERNEL */

/*
 * Macros started with IPV6_ADDR is KAME local
 */
#ifdef _KERNEL  /*XXX nonstandard*/
#if BYTE_ORDER == BIG_ENDIAN
#define IPV6_ADDR_INT32_ONE     1
#define IPV6_ADDR_INT32_TWO     2
#define IPV6_ADDR_INT32_MNL     0xff010000
#define IPV6_ADDR_INT32_MLL     0xff020000
#define IPV6_ADDR_INT32_SMP     0x0000ffff
#define IPV6_ADDR_INT16_ULL     0xfe80
#define IPV6_ADDR_INT16_USL     0xfec0
#define IPV6_ADDR_INT16_MLL     0xff02
#elif BYTE_ORDER == LITTLE_ENDIAN
#define IPV6_ADDR_INT32_ONE     0x01000000
#define IPV6_ADDR_INT32_TWO     0x02000000
#define IPV6_ADDR_INT32_MNL     0x000001ff
#define IPV6_ADDR_INT32_MLL     0x000002ff
#define IPV6_ADDR_INT32_SMP     0xffff0000
#define IPV6_ADDR_INT16_ULL     0x80fe
#define IPV6_ADDR_INT16_USL     0xc0fe
#define IPV6_ADDR_INT16_MLL     0x02ff
#endif
#endif

/*
 * Definition of some useful macros to handle IP6 addresses
 */
#define IN6ADDR_ANY_INIT \
        {{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}}
#define IN6ADDR_LOOPBACK_INIT \
        {{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
#define IN6ADDR_NODELOCAL_ALLNODES_INIT \
        {{{ 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
#define IN6ADDR_LINKLOCAL_ALLNODES_INIT \
        {{{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}
#define IN6ADDR_LINKLOCAL_ALLROUTERS_INIT \
        {{{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 }}}

extern const struct in6_addr in6addr_any;
extern const struct in6_addr in6addr_loopback;
extern const struct in6_addr in6addr_nodelocal_allnodes;
extern const struct in6_addr in6addr_linklocal_allnodes;
extern const struct in6_addr in6addr_linklocal_allrouters;



// Options to use with [gs]etsockopt at the IPPROTO_TCP level.
#define TCP_OFFLOAD_NO_PREFERENCE	0
#define	TCP_OFFLOAD_NOT_PREFERRED	1
#define TCP_OFFLOAD_PREFERRED		2

#define TCP_EXPEDITED_1122  	 0x0002
#define TCP_KEEPALIVE       	 3
#define TCP_MAXSEG          	 4
#define TCP_MAXRT           	 5
#define TCP_STDURG          	 6
#define TCP_NOURG           	 7
#define TCP_ATMARK          	 8
#define TCP_NOSYNRETRIES    	 9
#define TCP_TIMESTAMPS      	 10
#define TCP_OFFLOAD_PREFERENCE	 11
#define TCP_CONGESTION_ALGORITHM 12
#define TCP_DELAY_FIN_ACK        13
#define TCP_MAXRTMS              14
#define TCP_FASTOPEN             15
#define TCP_KEEPCNT              16
#define TCP_KEEPIDLE             TCP_KEEPALIVE
#define TCP_KEEPINTVL            17



#endif  /* _WINSOCKAPI_ */
