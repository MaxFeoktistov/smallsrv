/**
 *
 *  This file is part of Small HTTP server project.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program (see the file COPYING included with this
 *  distribution); if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef NETHDR_H
#define NETHDR_H

#include <winsock_IPv6.h>

#ifndef PACKED
#define PACKED __attribute__((packed))
#endif


#define	ETHER_ADDR_LEN		6

struct	ether_header {
	u_int8_t ether_dhost[ETHER_ADDR_LEN];
	u_int8_t ether_shost[ETHER_ADDR_LEN];
	u_int16_t ether_type;
} PACKED;

#define	ETHERTYPE_IP	0x0800
#define	ETHERTYPE_ARP   0x0806
#define	ETHERTYPE_RARP   0x8035
#define	ETHERTYPE_8021Q_TAG  0x8100
#define	ETHERTYPE_IPV6  0x86DD
#define	ETHERTYPE_FLOW_CONTROL  0x8808


/* File for little endian, windows */
struct iphdr {
        u_int8_t    ihl:4,
                    version:4;
         u_int8_t   tos;
         u_int16_t  tot_len;
         u_int16_t  id;
         u_int16_t  frag_off;
         u_int8_t   ttl;
         u_int8_t   protocol;
         u_int16_t  check;
         u_int32_t  saddr;
         u_int32_t  daddr;
         /*The options start here. */
} PACKED;


struct ip6_hdr
{
  union
  {
    struct ip6_hdrctl
    {
      u_int32_t ip6_un1_flow;   /* 4 bits version, 8 bits TC,
      20 bits flow-ID */
      u_int16_t ip6_un1_plen;   /* payload length */
      u_int8_t  ip6_un1_nxt;    /* next header */
      u_int8_t  ip6_un1_hlim;   /* hop limit */
    } ip6_un1;
    u_int8_t ip6_un2_vfc;       /* 4 bits version, top 4 bits tclass */
  } ip6_ctlun;
  struct in6_addr ip6_src;      /* source address */
  struct in6_addr ip6_dst;      /* destination address */
} PACKED;

#endif
