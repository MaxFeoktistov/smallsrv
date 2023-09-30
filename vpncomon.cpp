/*
 * Copyright (C) 1999-2023 Maksim Feoktistov.
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

#include "vpn.h"





//#include <stdio.h>

extern "C" {

const char * TUNTAPNames[] = {"TUN","TAP"};
 
#define MCAST_DETECT  0xE0
#define MCAST_MASK    0xF0

#define BROADCAST_ANY 0xFFFFffff


VPNclient **vpn_list;

fd_set VPNset;
int vpn_max_fd;
int vpn_max;
int vpn_count;
//volatile 
int vpn_mutex;
char *tundev="/dev/net/tun";
int  tuntap_number[2];
static const int tuntap_flags[2] = {IFF_TUN, IFF_TAP|IFF_NO_PI };
uint vpn_rescan_us=500000;
unsigned long long VPNdroped;
unsigned long long VPNsendet;
unsigned long long VPNreceved;


int tun_alloc(char dev_number, uint flags)
{
#ifdef VPN_LINUX
    struct ifreq ifr;
    int fd, err;

    if( (fd = open(tundev, O_RDWR)) < 0 )
       return -1;

    memset(&ifr, 0, sizeof(ifr));

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags =  flags; //IFF_TUN;
    sprintf(ifr.ifr_name, ( (flags & (IFF_TUN|IFF_TAP) ) == IFF_TUN)?"tun%u":"tap%u" , dev_number);

    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) 
    {
      debug("Can't open tun device %s. %d %d %s", ifr.ifr_name, err, errno, strerror(errno));
      close(fd);
      return err;
    }
#else
    return -1;  
#endif
    FD_SET(fd, &VPNset);
    if(vpn_max_fd<fd)vpn_max_fd=fd;
    return fd;
}


int Req::InsertVPNclient()
{
  VPNclient *cl;
  
  if(vpn_count >= vpn_max)
  {
    debug("Too many VPN clients. Can't insert new\r\n");
    return -1;
  }  
  cl = new VPNclient;
  if(!cl) return -1;
  memcpy(cl, this, sizeof(Req) );
  return 0;
}


int VPNclient::RecvPkt()
{
  int l,ll;
  //HANDLE h;
  if(pos_pkt<2) l=Recv(pkt + pos_pkt, MAX_MTU - pos_pkt);
  if(l<=0) return -1;
  pos_pkt += l;
  if(pos_pkt < 2) return 0;
  if(pkt_len >= MAX_MTU) {
    debug("VPN sinhronisation error\r\n");
    return -1;
  }
  while(pos_pkt >= (ll = pkt_len+2))
  {
    if( (l=write((fl & F_VPNTUN) ? tun_fd : tap_fd, pkt + 2, pkt_len)) != pkt_len )
    {
       debug("TUN write %d error %d %d %s\r\n",pkt_len, l, errno, strerror(errno));
       return -1;
    }
    if(pos_pkt > ll)
    {
      pos_pkt -= ll;
      memcpy(pkt, pkt+ll, pos_pkt);
    }
    else pos_pkt = 0;
  }
  return 0;
}

int ReInitTUNTAP(char *reason, int i)
{
  debug("VPN Error %s for %s %d %d %s\r\n", reason, TUNTAPNames[i], errno, strerror(errno));
  close(tuntap_fds[i]);
  return tuntap_fds[i] = tun_alloc(tuntap_number[i], tuntap_flags[i]);
}

int VPN_Thread(void *) 
{
  fd_set set;
  fd_set er_set;
  timeval  TVal;
  int    i,j,l,k,r;
  union {
    short pkt_len;
    uchar pkt[MAX_MTU];
  };
  
  while(is_no_exit) {
    memcpy(&set, &VPNset, sizeof(set) );
    memcpy(&er_set,&set,sizeof(er_set));
    TVal.tv_sec=0;
    TVal.tv_usec=vpn_rescan_us;
    j=select(vpn_max_fd+1,&set,0,&er_set,&TVal);
    if(j>0) {

      for(i=0; i<2; i++) 
        if(tuntap_fds[i] > 0 && FD_ISSET(tuntap_fds[i], &set) )
        {
          if( (pkt_len=read(tuntap_fds[i],pkt+2,MAX_MTU)) <= 0 ) {
              ReInitTUNTAP("read",i);
          }
          else
          {
             l = 0;
             for(k=0; k<vpn_count; k++) 
             {
               r = vpn_list[k]->SendIsUs(pkt,i);
               if( r < 0 ) CloseVPNClient(k);
               else if(r > 0)
               {
                 l++;
                 if(r == PKT_US) break;
               }
             }
             if(!l) VPNdroped ++;  //TODO: bytes stat 
             else VPNsendet += l;
          }
          j--; 
        }
        else  if( FD_ISSET(tuntap_fds[i], &er_set) )
        {
          ReInitTUNTAP("set",i);
          j--; 
        }
         
        
      for(i=0; i<vpn_count && j>0; i++) 
        if( FD_ISSET(vpn_list[i]->s, &set) )
        {
          if(vpn_list[i]->RecvPkt()<0)  CloseVPNClient(i);
          j--;
        }
        else  if( FD_ISSET(vpn_list[i]->s, &er_set) )
        {
          CloseVPNClient(i);
          j--;
        }
    }
  }

  return 0;
}

void  CloseTunTap()
{
  for(int i=0; i<2; i++)
     if(tuntap_fds[i] >= 0) {
     close(tuntap_fds[i]);
     tuntap_fds[i] = -1;
  }
}

int VPN_Init()
{
  
  if(USE_TUN) tun_fd = tun_alloc(tuntap_number[0], IFF_TUN);
  if(USE_TAP) tap_fd = tun_alloc(tuntap_number[1], IFF_TAP);
  if(tun_fd < 0 && tap_fd < 0) return -1;
  
  vpn_list = (VPNclient **) malloc( sizeof(VPNclient *) * (vpn_max+1) );
  if(!vpn_list) {
    CloseTunTap();
    return -1;
  }
  
  return CreateThread(&secat,0x5000,(TskSrv)VPN_Thread, (void *)0, 0, &trd_id);
}

int VPNclient::SendIsUs(uchar *pktl, int tuntap)
{
  
  if(tuntap)
  { // TUN
    if(fl & F_VPNTUN)
    {
#define pkt ((VPN_TUNPacket *) pktl)
      switch(pkt->tun_proto)
      {
        case ETHERTYPE_IP:
             if(pkt->ip4.daddr != ipv4 && (pkt->ip4.daddr & MCAST_MASK) != MCAST_DETECT &&  
               pkt->ip4.daddr != ipv4bcast && 
               pkt->ip4.daddr != BROADCAST_ANY 
             )  break;
        case ETHERTYPE_ARP:
        case ETHERTYPE_REVARP:
             Send(pktl, pkt->len);
             break;
          
        case ETHERTYPE_IPV6:
             if( 
                memcmp( & pkt->ip6.ip6_dst, &ipv6[0], sizeof(pkt->ip6.ip6_dst) ) &&
                memcmp( & pkt->ip6.ip6_dst, &ipv6[1], sizeof(pkt->ip6.ip6_dst) ) &&
#if __BYTE_ORDER__  !=  __ORDER_LITTLE_ENDIAN__ 
                WORD_PTR(pkt->ip6.ip6_dst.s6_addr[0]) != 0xFF02  // Multicast
#else                
                WORD_PTR(pkt->ip6.ip6_dst.s6_addr[0]) != 0x20FF  // Multicast
#endif                
             ) break;
             return Send(pktl, pkt->len);  
        
      }
#undef  pkt
    }
  }
  else
  { //TAP
    if(fl & F_VPNTAP)
    {
#define pkt ((VPN_TAPPacket *) pktl)
      if(  (!memcmp(pkt->eth.ether_dhost, &macb, 6) ) ||
           //(!memcmp(pkt->ether_dhost, &NullLongLong, 6) ) ||
           //(!memcmp(pkt->ether_dhost, &BCAST_MAC, 6) ) ||
           pkt->eth.ether_dhost[0] & 0x1   // multycast
      ) return Send(pktl, pkt->len);
#undef  pkt
    }
  }
  return 1;
}

void VPN_Done()
{
  int i;
  MyLock(vpn_mutex);
  
  for(i=0; i<vpn_count; i++)
  {
    CloseSocket(vpn_list[i]->s);
  }
  vpn_count = 0;
  MyUnlock(vpn_mutex);

  CloseTunTap();
  free(vpn_list);
  
}

}
