/*
 * Copyright (C) 1999-2020 Maksim Feoktistov.
 *
 * This file is part of Small HTTP server project.
 * Author: Maksim Feoktistov 
 *
 *
 * Small HTTP server is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * Small HTTP server is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/ 
 *
 * Contact addresses for Email:  support@smallsrv.com
 *
 * 
 */



extern struct DNS_FILEHEAD{
ulong dns_basetime;
int dns_incache;
}dns_filehead;



extern ulong *dns_ip,redirect_dns[3],*dip,*dcache,pval;
extern uchar *dtime;

#define dns_basetime dns_filehead.dns_basetime
#define dns_incache dns_filehead.dns_incache


struct d_msg{
 ushort id,flags;
 uchar t1,qdcount,
  t2,ancount,
  t3,nscount,
  t4,arcount;
 char buf[500];
} __attribute__ ((packed));

struct d_msgs{
 ushort id,flags;
 uchar t1,qdcount,
  t2,ancount,
  t3,nscount,
  t4,arcount;
 char buf[2];
} __attribute__ ((packed));
#define d_msgDOH 1


struct DHCPbase{
DHCPbase *next;
 union{
  ulong flg;
  uchar tt[4];
 };
 #define dhcpReleased 1
 #define dhcpDeleted  2
 #define dhcpTry      4

 union{
  ulong ip;
  uchar ipc[4];
 };
 char  domain[64];
 time_t t;
 union{
  ulong hw_typeLengh;
  uchar hw_t[2];
 };
 uchar  hw[16];

};

extern struct DHCPbase *dhcp_ar;

extern int dhcpd_s,dhcpc_s;
extern ulong first_dhcp_ip,total_dhcp_ip,last_dhcp_ip,allocated_dhcp_ip, gateway,netmask,dhcp_dns[3];
extern char *DhcpServerName;
extern char *first_dhcpc;
extern char *gatewayc;
extern char *netmaskc;
extern char *dhcp_dnsc;
extern char *dhcp_fname;


#ifdef SYSUNIX
void* zalloc(uint a);
#define ALLOC(a)  Malloc(a)
#else
#define ALLOC(a)  malloc(a)
#endif

#define FREE(a)   free(a)

void OptDHCP();
ulong WINAPI DHCPServ(void * fwrk);
int InitDHCP();
void SaveDHCP();
void DelDHCPIP(ulong ip,char *n,char *t);

