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

#ifdef SEPLOG

#undef debug
#undef AddToLog

#define debug(a...)  sepLog[VPN_LOG]->Ldebug(a)
#define AddToLog(a...)  sepLog[VPN_LOG]->LAddToLog(a)

#endif



//#include <stdio.h>

extern "C" {

int tuntap_fds[3] = {-1,-1,-1};
#define tun_fd tuntap_fds[0]
#define tap_fd tuntap_fds[1]

//const char * TUNTAPNames[3] = {"TUN","TAP","TAP"};
const char * TUNTAPNames[3] = {"tun","tap","tap"};
 
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
int  tuntap_number[3];
static int tuntap_flags[3] = {IFF_TUN, IFF_TAP|IFF_NO_PI, IFF_TAP|IFF_NO_PI };
char* tuntap_ipv4[3]= {"192.168.111.1","192.168.112.1"};
char* tuntap_ipv4nmask[3] = {"255.255.255.0","255.255.255.0"};
char* tuntap_ipv6[3];
uint tuntap_ipv6plen[3];
uint vpn_rescan_us=500000;
char* vpn_first_remote_ipc[2]={"192.168.111.20","192.168.112.20"};
uint vpn_first_remote_ip[2];
uint vpn_total_remote_ip[2] = {128,128} ; 
uint vpn_next_remote_ip[2]; 
//unsigned long long vpn_amask_remote_ip[2]; 
uint vpn_allocated_remote_ip;
char* vpn_gwc[2] = {"192.168.111.1","192.168.112.1"};
uint vpn_gw[2];
uint vpn_nmask[2];
char *vpn_dns[2] = {"192.168.111.1, 8.8.8.8, 4.4.4.4","192.168.112.1, 8.8.8.8, 4.4.4.4"};
char *vpn_scripts_up[3]; //={"","",""};
char *vpn_scripts_down[3];

long long vpn_mac[3];

unsigned long long VPNdroped;
unsigned long long VPNsendet;
unsigned long long VPNreceved;
unsigned long long VPNsendet_pkt;
unsigned long long VPNreceved_pkt;

int  rt_fd;


int  RunScript(char *cmd)
{
#ifdef VPN_LINUX
  int r = system(cmd);
  if(r == -1) debug("vpn: Can't run script '%s'\r\n", cmd);
  return r;
#else
  // TODO:
#endif  
}

int RunDownScript(int index, int ip)
{
  char cmd[300];
  if(vpn_scripts_down[index] && vpn_scripts_down[index][0])
  {
    sprintf(cmd, "%s %s%u %u.%u.%u.%u", vpn_scripts_down[index],
            TUNTAPNames[index], tuntap_number[index],
            ip>>24, (ip>>16)&0xFF, (ip>>8)&0xFF, ip&0xFF
    );

    DBGLS(cmd);

    return RunScript(cmd);
  }
  return 0;
};

int tun_alloc(int index)
{
#ifdef VPN_LINUX
    struct ifreq ifr;
    int fd, err;
    uint ip=0,m=0;
    uint aflag=0;
    int r;
    char *t;

    if( (fd = open(tundev, O_RDWR)) < 0 )
       return -1;
    tuntap_fds[index] = fd;

    memset(&ifr, 0, sizeof(ifr));

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags =  tuntap_flags[index]; //IFF_TUN;
    sprintf(ifr.ifr_name, "%s%u", TUNTAPNames[index], tuntap_number[index]);
    
    DBGLS(ifr.ifr_name);

    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) 
    {
      debug("Can't open tun device %s. %d %d %s", ifr.ifr_name, err, errno, strerror(errno));
      close(fd);
      return err;
    }

    if(rt_fd<=0) rt_fd = socket( PF_INET, SOCK_DGRAM,  IPPROTO_IP);

    
    if(tuntap_ipv4[index] && tuntap_ipv4nmask[index] && (ip=ConvertIP(t=tuntap_ipv4[index]) ) && (m=ConvertIP(t=tuntap_ipv4nmask[index]) ) )
    {

      DBGLS(ifr.ifr_name);

      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = ip;
      if( ioctl(rt_fd, SIOCSIFADDR, &ifr)  )
         debug("VPN: can't set IP %X for interface %s: %d %s\r\n", htonl(ip), ifr.ifr_name, errno, strerror(errno) );
   
      
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = m;
      if( ioctl(rt_fd, SIOCSIFNETMASK, &ifr) )
         debug("VPN: can't set netmask %X for interface %s: %d %s\r\n",m , ifr.ifr_name, errno, strerror(errno) );
   
      
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = ip | ~m;
      if( ioctl(rt_fd, SIOCSIFBRDADDR, &ifr) )
         debug("VPN: can't set broadcast address for interface %s: %d %s\r\n", ifr.ifr_name, errno, strerror(errno) );
   
      aflag |= IFF_BROADCAST;
    }
    //ifr.ifr_flags = IFF_UP | IFF_ALLMULTI | IFF_RUNNING | aflag;
    if( (r = ioctl(rt_fd, SIOCGIFFLAGS, &ifr) ) < 0 ) 
      debug("VPN: can't get flags interface %s: %d %d %s\r\n", ifr.ifr_name, r, errno, strerror(errno) );
    else aflag |= ifr.ifr_flags;
    
//    ifr.ifr_flags = IFF_UP | IFF_ALLMULTI | IFF_ECHO | aflag;
    ifr.ifr_flags = IFF_UP | IFF_ALLMULTI | IFF_DYNAMIC | IFF_AUTOMEDIA | aflag;
      
    if( (r=ioctl(rt_fd, SIOCSIFFLAGS, &ifr) ) )
      debug("VPN: can't UP %X interface %s: %d %d %s\r\n", ifr.ifr_flags, ifr.ifr_name, r, errno, strerror(errno) );
    
    if( tuntap_flags[index] & IFF_TAP )
    {
      if( (r=ioctl(rt_fd,  SIOCGIFHWADDR, &ifr) ) )
         debug("VPN: can't get hw address interface %s: %d %d %s\r\n", ifr.ifr_flags, ifr.ifr_name, r, errno, strerror(errno) );
      else vpn_mac[index] = DDWORD_PTR(ifr.ifr_hwaddr.sa_data[0]) & 0xFFFFffffFFLL;
    }
    
    // TODO: get IPv6
    
    if(vpn_scripts_up[index] && vpn_scripts_up[index][0] && index<2 ) 
    {
      char  cmd[512];
      sprintf(cmd, "%s %s%u %u.%u.%u.%u %u.%u.%u.%u ", vpn_scripts_up[index],
         TUNTAPNames[index], tuntap_number[index],
         ip>>24, (ip>>16)&0xFF, (ip>>8)&0xFF, ip&0xFF,
         m>>24, (m>>16)&0xFF, (m>>8)&0xFF, m&0xFF
      );

      DBGLS(cmd);

      RunScript(cmd);
    }
    
#else
    return -1;  
#endif
    FD_SET(fd, &VPNset);
    if(vpn_max_fd<fd)vpn_max_fd=fd;
    return fd;
}


void CloseVPNClient(int i)
{

  SecClose((OpenSSLConnection*) vpn_list[i]->Adv);
  CloseSocket(vpn_list[i]->s);
  MyLock(vpn_mutex);
  free(vpn_list[i]);
  vpn_count --;
  if(i != vpn_count)
  {
    vpn_list[i] = vpn_list[vpn_count];
  }
  MyUnlock(vpn_mutex);
}

//////////////////////////////////////////////
int IsVPN_IP_Free(uint ip)
{
  int i;
  for(i=0; i<vpn_count; i++)
    if(vpn_list[i]->ipv4 == ip) return 0;
  return 1;
}
///////////////////
int Req::InsertVPNclient()
{
  VPNclient *cl;
  char *rnd, *pwdcode;
  int l;
  User *tuser;
  char *t;
  char *p;
  int isTap;
  ulong ip;
  long long mac;
  

  DBGL("");

  if(vpn_count >= vpn_max)
  {
    debug("Too many VPN clients. Can't insert new\r\n");
    return -1;
  }
  DBGL("");

   if( ( ! (s_flgs[3] & FL3_VPN_PUBLIC) )  && ! (tuser=ChUser(UserHTTP)) )
   {
  lbLogout:

      DBGL("");

#ifdef WITHMD5
       SendDigestAuthReq(loc); 
#else
       Send(AuthErr, strlen(AuthErr)  ) ;// sizeof(AuthErr)-1); 
#endif
    return -1;
   }

   DBGL("");

   isTap = 0;
   if( (t =  GetVar(http_var,"tap") ) ) isTap++;
   else t =  GetVar(http_var,"tun");
   if( (!t) || tuntap_fds[isTap] == -1 )
   {
     HttpReturnError("Error. Selected VPN type disabled...");
     return -1; 
   }

   DBGLS(t);

//   cl = new VPNclient;
   cl = (VPNclient *) malloc(sizeof(VPNclient));
   if(!cl) return -1;
   memcpy(cl, this, sizeof(Req) );
   memcpy(&cl->tls, Adv, sizeof(OpenSSLConnection) );
   cl->Adv = &cl->tls;
   cl->tls.CallbackParam = cl;
   SecUpdateCB(&cl->tls);

   cl->fl = F_VPNTUN << isTap;
   cl->tun_index = isTap;
   
   ip = strtoul(t, &p, 16);
   if( (!ip) || (vpn_total_remote_ip[isTap] && ! IsVPN_IP_Free(ip)) )
   {
     if(vpn_first_remote_ip[isTap] && vpn_total_remote_ip[isTap]) 
     {
       if(vpn_next_remote_ip[isTap] < vpn_first_remote_ip[isTap]) vpn_next_remote_ip[isTap] = vpn_first_remote_ip[isTap];
       ip = vpn_next_remote_ip[isTap];
       do{
         if(IsVPN_IP_Free(ip)) 
         {
           vpn_next_remote_ip[isTap] = ip;
           goto found_free_ip;
         }
         
         #ifdef BIG_ENDIAN
         ip++;
         if( (ip-vpn_first_remote_ip[isTap]) >= vpn_total_remote_ip[isTap] ) ip = vpn_first_remote_ip[isTap];
         #else
         ip = htonl(ip);
         ip++;
         if( (ip-htonl(vpn_first_remote_ip[isTap])) >= vpn_total_remote_ip[isTap] ) ip = vpn_first_remote_ip[isTap];
         else ip = htonl(ip);
         #endif         
         
       } while( ip != vpn_next_remote_ip[isTap] );
       ip = 0;
       debug("Not found free IP for %s\r\n", TUNTAPNames[isTap]);
     found_free_ip:;  
     }
   }
   t=loc + 1024;
   l = sprintf(t,"HTTP/1.0 200\r\n");
   if(ip) {
       l += sprintf(t + l,"ip: %X\r\n"
                         "mask: %X\r\n"
                         "gw: %X\r\n"
       ,ip, vpn_nmask[isTap], vpn_gw[isTap]);
       if(vpn_dns[isTap] && vpn_dns[isTap][0]) {
         l += sprintf(t + l,"dns: %s\r\n", vpn_dns[isTap]);
       cl->ipv4 = ip;
       cl->ipv4bcast = ip| ~vpn_nmask[isTap];  
       cl->fl |= F_VPN_IPSET;
      }
   }
   l += sprintf(t + l,"\r\n");
   cl->Send(t,l);
   if(p && (mac = strtoll(SkipSpace(p) , 0, 16)) ) 
   {
     cl->macl = mac;
     cl->fl |= F_VPN_MACSET;
     DBGLS(p);
   }

   DBGL("Ok");

   vpn_list[vpn_count++] = cl;
   fl = F_KEEP_ALIVE | F_VPNANY;
   s = -1;
   Adv = 0;
   return 0;
}


int VPNclient::RecvPkt()
{
  int l,ll;
  //HANDLE h;
  DBGL("recv");
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
    if(fl & F_VPNTUN)
    {
      if( (! (fl & F_VPN_IPSET) ) && tunpkt.tun_proto == ETHERTYPE_IP && tunpkt.ip4.saddr)
      {
        ipv4 = tunpkt.ip4.saddr;
        fl |= F_VPN_IPSET;
      }
      if( (! (fl & F_VPN_IP6SET) ) && tunpkt.tun_proto == ETHERTYPE_IPV6 && WORD_PTR(tunpkt.ip6.ip6_src.s6_addr[0]) == 0xfe80 )
      {
        memcpy(& ipv6[0], tappkt.ip6.ip6_src.s6_addr, sizeof(ipv6[0]) );
        fl |= F_VPN_IP6SET;
      }
      if( (! (fl & F_VPN_IP6SET2) ) && tunpkt.tun_proto == ETHERTYPE_IPV6 && 
          WORD_PTR(tunpkt.ip6.ip6_src.s6_addr[0]) < 0xfe00 && 
          WORD_PTR(tunpkt.ip6.ip6_src.s6_addr[0]) & ~0x40  )
      {
        memcpy(& ipv6[1], tappkt.ip6.ip6_src.s6_addr, sizeof(ipv6[0]) );
        fl |= F_VPN_IP6SET2;
      }
    }
    else if(fl & F_VPNTAP)
    {
      if(! (fl & F_VPN_MACSET) ) 
      {
        memcpy(&macb, &tappkt.eth.ether_shost, 6);
        fl |= F_VPN_MACSET;
      }
      if( (! (fl & F_VPN_IPSET) ) && tappkt.eth.ether_type == ETHERTYPE_IP && tappkt.ip4.saddr )
      {
        ipv4 = tunpkt.ip4.saddr;
        fl |= F_VPN_MACSET;
      }
      if( (! (fl & F_VPN_IP6SET) ) && tappkt.eth.ether_type == ETHERTYPE_IPV6 && WORD_PTR(tappkt.ip6.ip6_src.s6_addr[0]) == 
#ifdef BIG_ENDIAN
        0xFE80
#else        
        0x80fe // LE machine!!! 
#endif        
      ) 
      {
        memcpy(& ipv6[0], tappkt.ip6.ip6_src.s6_addr, sizeof(ipv6[0]) );
        fl |= F_VPN_IP6SET;
      }
      if( (! (fl & F_VPN_IP6SET2) ) && tappkt.eth.ether_type == ETHERTYPE_IPV6 && 
          tappkt.ip6.ip6_src.s6_addr[0] < 0xfe && 
          WORD_PTR(tappkt.ip6.ip6_src.s6_addr[0]) & 
#ifdef BIG_ENDIAN
          ~0x40
#else
          ~0x4000
#endif
        ) 
      {
        memcpy(& ipv6[1], tappkt.ip6.ip6_src.s6_addr, sizeof(ipv6[0]) );
        fl |= F_VPN_IP6SET2;
      }
    }
    if( (l=write(//(fl & F_VPNTUN) ? tun_fd : tap_fd, 
                 tuntap_fds[tun_index], pkt + 2, pkt_len)) != pkt_len )
    {
       debug("TUN write %d error %d %d %s\r\n",pkt_len, l, errno, strerror(errno));
       return -1;
    }
    VPNreceved += l;
    VPNreceved_pkt ++;

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
  return tuntap_fds[i] = tun_alloc(i);
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
  
  while(is_no_exit) 
  {
    memcpy(&set, &VPNset, sizeof(set) );
    memcpy(&er_set,&set,sizeof(er_set));
    TVal.tv_sec=0;
    TVal.tv_usec=vpn_rescan_us;
    j=select(vpn_max_fd+1,&set,0,&er_set,&TVal);
    if(j>0) 
    {

      DBGL("");

      for(i=0; i<2; i++) 
        if(tuntap_fds[i] > 0 && FD_ISSET(tuntap_fds[i], &set) )
        {
          if( (pkt_len=read(tuntap_fds[i],pkt+2,MAX_MTU)) <= 0 ) 
          {
              ReInitTUNTAP("read",i);
          }
          else
          {

            DBGLA("%u read %u %u",TUNTAPNames[i], pkt_len,vpn_count);

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
             else {
               VPNsendet += l;
               VPNsendet_pkt ++;
             }
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
        else if( FD_ISSET(vpn_list[i]->s, &er_set) )
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
  int  i;
  char *t;
  for(i=0; i<2; i++)
  {
    if(vpn_first_remote_ipc[i] && vpn_first_remote_ipc[i][0])
      vpn_next_remote_ip[i] = vpn_first_remote_ip[i] = ConvertIP(t=vpn_first_remote_ipc[i]);
    if(vpn_gwc[i] && vpn_gwc[i][0])
      vpn_gw[i] = ConvertIP(t=vpn_gwc[i]);
    if(tuntap_ipv4nmask[i] && tuntap_ipv4nmask[i][0])
      vpn_nmask[i] = ConvertIP(t=tuntap_ipv4nmask[i]);
  }

  if(USE_TUN) tun_fd = tun_alloc(0);
  if(USE_TAP) tap_fd = tun_alloc(1);
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
  int ret = PKT_NOT_US;
  if(tuntap)
  { // TUN
    if(fl & F_VPNTUN)
    {
#define pkt ((VPN_TUNPacket *) pktl)
      DBGLA("tun pkt: %X", pkt->tun_proto);
      switch(pkt->tun_proto)
      {
        case ETHERTYPE_IP:
             if(pkt->ip4.daddr == ipv4) ret = PKT_US;
             else if( (pkt->ip4.daddr & MCAST_MASK) == MCAST_DETECT ||
               pkt->ip4.daddr == ipv4bcast ||
               pkt->ip4.daddr == BROADCAST_ANY 
             ) ret = PKT_BCAST;
             else  break;
             if(0){
        case ETHERTYPE_ARP:
        case ETHERTYPE_REVARP:
             ret = PKT_BCAST;
             if(0) {
        case ETHERTYPE_IPV6:
             if( 
                (!memcmp( & pkt->ip6.ip6_dst, &ipv6[0], sizeof(pkt->ip6.ip6_dst) ) ) ||
                (!memcmp( & pkt->ip6.ip6_dst, &ipv6[1], sizeof(pkt->ip6.ip6_dst) ) ) 
               ) ret = PKT_US;
             else if(  
#if __BYTE_ORDER__  !=  __ORDER_LITTLE_ENDIAN__ 
                WORD_PTR(pkt->ip6.ip6_dst.s6_addr[0]) == 0xFF02  // Multicast
#else                
                WORD_PTR(pkt->ip6.ip6_dst.s6_addr[0]) == 0x20FF  // Multicast
#endif                
             ) ret = PKT_BCAST;
             else  break;
             }}
             if(Send(pktl, pkt->len + 2)<=0) ret = -1;

        
      }
#undef  pkt
    }
    // TODO: else Tun/Tap gate
  }
  else
  { //TAP
    if(fl & F_VPNTAP)
    {


#define pkt ((VPN_TAPPacket *) pktl)
      DBGLA("tap pkt: %X %llX", pkt->eth.ether_type, DDWORD_PTR(pkt->eth.ether_dhost[0]) & 0xFFFFffffFFLL );
      if(  (!memcmp(pkt->eth.ether_dhost, &macb, 6) ) ) ret = PKT_US;
      else if(
           //(!memcmp(pkt->ether_dhost, &NullLongLong, 6) ) ||
           //(!memcmp(pkt->ether_dhost, &BCAST_MAC, 6) ) ||
           pkt->eth.ether_dhost[0] & 0x1   // multycast
      )  ret = PKT_BCAST;
      else return ret;
      if(Send(pktl, pkt->len + 2)<=0) ret = -1;
#undef  pkt
    }
    // TODO: else Tun/Tap gate
  }
  return ret;
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

char *vpn_remote_host;
int vpn_client_port=443;
char * vpn_passw="", *vpn_user="";
char vpn_notice[48];
char vpn_opaque[48];
char vpn_realm[32];
uint  vpn_client_ip;
long long vpn_client_mac;

void addRoute(uint dstip, uint dstmsk, uint gw)
{
    //int rt_fd = socket( PF_INET, SOCK_DGRAM,  IPPROTO_IP);
    if(rt_fd<=0) rt_fd = socket( PF_INET, SOCK_DGRAM,  IPPROTO_IP);

    struct rtentry route;
    memset( &route, 0, sizeof( route ) );
    
    ((struct sockaddr_in*) &route.rt_gateway)->sin_family = AF_INET;
    ((struct sockaddr_in*) &route.rt_gateway)->sin_addr.s_addr = gw;
    
    ((struct sockaddr_in*) &route.rt_dst)->sin_family = AF_INET;
    ((struct sockaddr_in*) &route.rt_dst)->sin_addr.s_addr = dstip;
    
    ((struct sockaddr_in*) &route.rt_genmask)->sin_family = AF_INET;
    ((struct sockaddr_in*) &route.rt_genmask)->sin_addr.s_addr = dstmsk;
    route.rt_flags = RTF_UP | RTF_GATEWAY;
    
    route.rt_metric = 0;

    if (ioctl( rt_fd, SIOCADDRT, &route ) < 0)
    {
        debug("ioctl failed and returned errno %s \n", strerror(errno));
    }
    //close( fd );
}

int split(char *src, char *separators, char **result, int max_result)
{
  int ret = 0;
  while(*src && ret<max_result)
  {
    *result=src;
    //while( ! strchr(separators, *src ) ) { if(!*src) goto ex2loop; src++ ; }
    if( (!(src = strpbrk(src, separators)) ) || ! *src )  goto ex2loop;
    *src++ = 0;
    while( strchr(separators, *src ) ) { if(!*src) goto ex2loop; src++ ; }
    result++;
    ret++;
  }
 ex2loop: 
  return ret;
}

uint GetDefaultRoute()
{
#ifdef VPN_LINUX
  uint ret = 0;
  int fd, l, nlines, n,i, g;
  char *bfr;
  char *lines[32];
  char *words[16];
  enum {Iface=0,   Destination,     Gateway,         Flags,   RefCnt,  Use,     Metric,  Mask,            MTU,     Window,  IRTT };
#define MAX_ROUTE_SIZE 0x10000 
  bfr=(char *)malloc(MAX_ROUTE_SIZE);
  fd=open("/proc/net/route",O_RDONLY);
  if(fd > 0) 
  {
    l = read(fd,bfr,MAX_ROUTE_SIZE-1);
    close(fd);
    if(l>0)
    {
      bfr[l]=0;
      nlines = split(bfr,"\r\n",lines,31);
      for(i=1; i< nlines; i++)
      {
        n = split(lines[i]," \t",words,15);
        if(n >= Mask)
        {
          if( (!atouix(words[Destination])) && (!atouix(words[Mask])) && (g=atouix(words[Mask])) && words[Destination][0]=='0' )
          {
            ret = g;
            goto ex_ret;
          }
        }
      }
    }
  }
 ex_ret:
  free(bfr);
  return ret;
#else
  //TODO
#endif
}


int FindCopyVar(char *s, char *name, char *t, int l)
{
  char *v = PrFinVar(s,name);
  char *e;
  int ll;
  if(!v) return 0;
  e=strpbrk(v,"\t\r\n \",");
  if(!e) return 0;
  ll = e-v;
  if(l <= ll) ll = l - 1;
  memcpy(t,v,ll);
  t[ll] = 0;
  return 1;
};

void ChkEmpty(char *p)
{
  if(! *p )
    sprintf(p,"%X", Rnd() );
}

int VPNclient::ClientConnect(OpenSSLConnection *x)
{
  char bfr[2048];
  int  r;
  char *t;
  uint n,i;
  int  l;
  struct ifreq  ifr;
  char *dgtvars[10];
  uint HA1[6];
  char HA2Hex[40];
  int  tryes = 0;
#define TRYES_REQUEST_BASIC 0x8000
  uint m=0xFFFFFF;

  
  if(!vpn_remote_host) return -5;
  if( (!PSecConnect) || PSecConnect == SecConnectAbcent) 
  {
    debug("Your version of 'seclib' library doesn't support functions required for VPN client. Please update it...\r\n");
    return -4;
  }
  timout = 60;

 // DBGL("")

agayn1:  
  s=call_socket(vpn_remote_host, vpn_client_port);
  if(s <= 0) 
  {
    debug("VPN client: Can't connect to %s...\r\n", vpn_remote_host);
    return -1;
  }
  
 // DBGL("")

  if(! TLSBegin(x,tbtAnon) )
  {
    debug("VPN client: Can't setup TLS to %s...\r\n", vpn_remote_host);
    CloseSocket(s);
    return -1;
  }

//  DBGL("")
  l = 1024;
  if(tryes & TRYES_REQUEST_BASIC ) 
  {
    l += sprintf(bfr + l,"Basic ");
    l = Encode64(bfr + l, bfr + 1400, sprintf(bfr+1400, "%s:%s",vpn_user, vpn_passw ) ) - bfr;
  }
  else
  {
    l += sprintf(bfr + l,"Digest ");
    memset(dgtvars, 0, sizeof(dgtvars));
    dgtvars[digtVar_username] = vpn_user;
    dgtvars[digtVar_nonce   ] = vpn_notice;
    dgtvars[digtVar_uri     ] = vpn_name;
    dgtvars[digtVar_opaque  ] = vpn_opaque;
    ChkEmpty(vpn_notice);
    ChkEmpty(vpn_opaque);
    
    //dgtvars[digtVar_qop] = 0;
    
    //CalkHA1(vpn_user, vpn_passw, (uchar *)HA1);
    ConvPwdMD5L4(HA1, vpn_user, vpn_passw, vpn_realm);
    CalkPwdMD5D(dgtvars, HA1, "GET", HA2Hex);
    dgtvars[digtVar_response] = HA2Hex;
    for(i=0; digetvars[i]; i++) if(dgtvars[i])
      l += sprintf(bfr+l,"%c%s=\"%s\"", (i)?',':' ', digetvars[i], dgtvars[i]);
    /*
     * l=sprintf(bfr+1024,"%X", r);
     * GenAPOP_dgst(vpn_passw, bfr+1124,bfr+1024,l);
     */
  }
  l=sprintf(bfr,"GET %s HTTP/1.1\r\n"
  //  "User: %s\r\n"
  //  "Dg: %X\r\n"
  //  "Code: %s\r\n"
    "Authorization: %s\r\n"
    "Host: %s\r\n"
    "%s: %X %llX\r\n"
    "Connection: keep-alive\r\n\r\n", vpn_name,
    //vpn_user, r, bfr+1124, 
    bfr+1024, vpn_remote_host,
    TUNTAPNames[tun_index], vpn_client_ip, vpn_mac[tun_index]
  );

  DBGLA("ClSend:%u: %s",l,bfr);

  if(Send(bfr,l) != l )
  {
 err_close:
    debug("TLS Connection error\n");
    SecClose(x);
    CloseSocket(s);
    return -3;
  };
  if( ! REPool(30000,s) ) {
    debug("VPN Connection timeout\r\n");
    goto err_close;   
  }
  if( (l=Recv(bfr,sizeof(bfr)-1) ) <= 0 ) 
  {
     DBGL("Recv error")
     goto err_close;   
  }

  bfr[l] = 0;

  DBGLA("ClRecv: l=%d\r\n%s", l, bfr)

  if( (r=atoi(bfr+9)) != 200)
  {

    debug("VPN client: server return error %d %.64s...\r\n", r, bfr);

    if(r == 401) 
    {
      http_var=(char **) (bfr + 1024);
      rq = bfr;
      prepare_HTTP_var();

      DBGL("")

      if( (t=GetVar(http_var,"WWW_Authenticate") ) )
      {
        DBGLS(t)
        if(strin(t, "Basic ") )
        {
          if(tryes) goto err_close;   
          tryes = TRYES_REQUEST_BASIC;
          DBGLS("Basic")
          goto agayn1;
        }

        FindCopyVar(t,"realm",  vpn_realm , sizeof(vpn_realm ));
        FindCopyVar(t,"nonce",  vpn_notice, sizeof(vpn_notice));
        FindCopyVar(t,"opaque", vpn_opaque, sizeof(vpn_opaque));
        
        DBGLA(" %s %s %s", vpn_realm, vpn_notice, vpn_opaque)
        
        if(!tryes) {
          SecClose(x);
          CloseSocket(s);
          tryes ++ ;
          goto agayn1;
        }
      }
    }
    goto err_close;   
  }
  
  http_var=(char **) (bfr + 1024);
  rq = bfr;
  prepare_HTTP_var();
  
  memset(&ifr, 0, sizeof(ifr) );
  //tun_index = (vpn.fl == F_VPNTAP)?0:1;
  DBGL("")

  sprintf(ifr.ifr_name,"%s%u",TUNTAPNames[tun_index], tuntap_number[tun_index]);
  t=GetVar(http_var,"ip");
  if(t && t[0])
  {
    DBGLS(t)
    n=atouix(t); //ConvertIP(t);
    DBGLA("%X" , n)

    
    //if(ipv4 != n)
    {
      ipv4 = n;
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = ipv4;
      ioctl(rt_fd, SIOCSIFADDR, &ifr);
      
      t = GetVar(http_var,"nmask");
      if(t && t[0])
      {
        n=atouix(t); //ConvertIP(t);
        if(n)
        {
          m = n;
          ipv4bcast = ipv4 | ~n;
          ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
          ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = n;
          ioctl(tuntap_fds[tun_index], SIOCSIFNETMASK, &ifr);
        }
      }
    }
  }
  t=GetVar(http_var, "gw");
  // TODO: set route
  if(t && t[0]) n = atouix(t);
  if( vpn_scripts_up[2] && vpn_scripts_up[2][0] )
  {
    char *dns=GetVar(http_var,"dns");
    sprintf(bfr, "%s %s%u %u.%u.%u.%u %u.%u.%u.%u %u.%u.%u.%u '%s'", vpn_scripts_up[2],
            TUNTAPNames[2], tuntap_number[2],
            ipv4>>24, (ipv4>>16)&0xFF, (ipv4>>8)&0xFF, ipv4&0xFF,
            m>>24, (m>>16)&0xFF, (m>>8)&0xFF, m&0xFF,
            n>>24, (n>>16)&0xFF, (n>>8)&0xFF, n&0xFF
            ,t,(dns)?dns:""
    );
    DBGLS(bfr);
    RunScript(bfr);
  }
  DBGL("ConnectOk\n");
  return 1;
}

int VPNClient(void *)
{
  VPNclient         vpn;
  timeval  TVal;

  int r;
  int vpn_client_fd;
  int max_fd;
  fd_set set, er_set;
  union {
    short pkt_len;
    uchar pkt[MAX_MTU];
  };
  
  DBGL("")

  //vpn.tun_index=0;
  //if(TAP_SEPARATE) 
  {
    vpn.tun_index=2;
    if(TAP_CLIENT){
      vpn.fl = F_VPNTAP;
      tuntap_flags[2] = IFF_TAP|IFF_NO_PI;
      TUNTAPNames[2] = "tap";
    }
    else
    {
      vpn.fl = F_VPNTUN;
      tuntap_flags[2] = IFF_TUN;
      TUNTAPNames[2] = "tun";
    }
  }
  
  /*
  else if(TAP_CLIENT)
  {
    vpn.tun_index++ ;
    vpn.fl = F_VPNTAP;
  }
  else  
    vpn.fl = F_VPNTUN;
  */
  
  if(tuntap_fds[vpn.tun_index] < 0 ) tuntap_fds[vpn.tun_index] = tun_alloc(vpn.tun_index);
  if(tuntap_fds[vpn.tun_index] < 0 ) {
    return -1;
  }

  DBGL("")

  vpn_client_fd = tuntap_fds[vpn.tun_index];
  
  //vpn_client_fd = tun_alloc(tuntap_client_number, (TUN_CLIENT)? IFF_TUN : IFF_TAP);
  //if(vpn_client_fd < 0) {
  //  return -1;
  //}
  while(is_no_exit)
  {
    if( (r=vpn.ClientConnect(&vpn.tls)) < 0 ) 
    {

      DBGL("")

      if(r < -4)
      {

        DBGL("client exit");

        return -1;
      }
      Sleep(60000);
    }
    else
    {
      FD_ZERO(&set);
      FD_ZERO(&er_set);
      max_fd = vpn_client_fd;
      if(vpn_client_fd < vpn.s) max_fd = vpn.s;
       
      while(is_no_exit)
      {
        FD_SET(vpn_client_fd, &set);
        FD_SET(vpn.s, &set);
        FD_SET(vpn_client_fd, &er_set);
        FD_SET(vpn.s, &er_set);
        TVal.tv_sec=1;
        TVal.tv_usec=500000;
        if( (select(max_fd+1,&set,0,&er_set,&TVal))>0 )
        {
          if(FD_ISSET(vpn.s, &er_set)) goto err_s;
          if(FD_ISSET(vpn_client_fd, &er_set)) goto err_vpn;
          
          if(FD_ISSET(vpn_client_fd, &set))
          {
            if( (pkt_len=read(vpn_client_fd,pkt+2,MAX_MTU)) <= 0 ) 
            {
              //debug("VPN client error %d %s\r\n", errno, strerror(errno));
          err_vpn:    
              vpn_client_fd=ReInitTUNTAP("read",vpn.tun_index);
              if(vpn_client_fd < 0)
                return -1;
              max_fd = vpn_client_fd;
              if(vpn_client_fd < vpn.s) max_fd = vpn.s;
            }
            else {
              vpn.Send(&pkt, pkt_len+2);
            }
          }
          if(FD_ISSET(vpn.s, &set))
          {
            if(vpn.RecvPkt()<0)  
            {
            err_s:  
              SecClose(&vpn.tls);
              CloseSocket(vpn.s);
              RunDownScript(vpn.tun_index, vpn.ipv4);
              break;
            }
          }
        }
      }
    }
  }
  
  
  return 0;
}

}
