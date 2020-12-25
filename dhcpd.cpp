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

#ifndef SRV_H
#include "srv.h"
#include "mstring1.h"
#endif

#include "dhcp.h"

struct DHCPbase *dhcp_ar,*last_dhcp_loaded;
int DHCPMutex, sizeSize,dhcp_need_save;



ulong first_dhcp_ip,total_dhcp_ip,last_dhcp_ip,allocated_dhcp_ip, gateway,netmask,dhcp_dns[3];
char *DhcpServerName="SmallSrv.local";
char *dhcp_addr="192.168.0.1";
char *first_dhcpc="192.168.0.20";
char *gatewayc="192.168.0.1";
char *netmaskc="255.255.255.0";
char *dhcp_dnsc="192.168.0.1";
char *dhcp_fname="dhcp";
char *dhcp_bcast="255.255.255.255";
#define DHCP_SAVE_INTERVAL (3600*12)

static union
{
 ulong next_dhcp_ip;
 uchar ndi[4];
};

int isIpFree(ulong i)
{DHCPbase *d;
 for(d=dhcp_ar;d;d=d->next)if(d->ip==i) return 0;
 return 1;
}

ulong NextDHCPIP()
{
 int i,k,j;
 DHCPbase *d;
 j=0;
 ulong ip,ip2;

 ip=htonl(next_dhcp_ip);
 ip2=htonl(first_dhcp_ip);
 do{
     ++ip;
     if( (k=ip&0xFF) == 0xFF || !k)
     { ip=(ip&0xFFFFFF00)+0x101;
     }
    //debug("ip=%X last_dhcp_ip=%X %X ",ip,last_dhcp_ip,first_dhcp_ip);
    if(ip>last_dhcp_ip || ip<ip2 )ip=ip2;
    next_dhcp_ip=htonl(ip);
  for(d=dhcp_ar;d;d=d->next)if(d->ip==next_dhcp_ip)
  {
   if( (d->flg&dhcpReleased) && allocated_dhcp_ip>=(total_dhcp_ip-1) )
   {
    DelDHCPIP(next_dhcp_ip=d->ip,0,0);
    return next_dhcp_ip;
   }
   goto lbExlp;
  }
  return next_dhcp_ip;
lbExlp:;
 }while(++j<total_dhcp_ip);
}

void SaveDHCP()
{
 int h;
 DHCPbase *d;
 if(dhcp_need_save)
 {    
        if((h=_lcreat(dhcp_fname,0))>0)
        {
          for(d=dhcp_ar;d;d=d->next)if(!(d->flg&dhcpDeleted) )_hwrite(h,(char *)d,sizeof(*d));
         _lclose(h);
        }
    dhcp_need_save=0;     
 }
}

#define DHCP_TBL_HEAD "<h2>DHCP</h2>" \
   "<table border=1 bgcolor=#C0E0F0><tr align=center bgcolor=#C0C0FF>"  \
   "<td><b>IP</b></td>"        \
   "<td><b>HW type</b></td>"   \
   "<td><b>HW address</b></td>" \
   "<td><b>name</b></td><td><b>Last ask</b></td><td> . </td></tr> "
int Req::ShowDHCP(char *b)
{
 DHCPbase *d;
 char *p;
 int l,i;
 char addr[52];
 
#ifdef  SYSUNIX
 struct tm *ttm;
#else
 SYSTEMTIME stm;
 union {
 FILETIME   ft;
 long long  lft;
 };
#endif 
 
 p=b+sprintf(b,DHCP_TBL_HEAD);
 for(d=dhcp_ar;d;d=d->next)
 {

  if(!(d->flg &dhcpDeleted))
  {
    l=d->hw_t[1];
    if(l>16) l=16;
    for(i=0;i<l; ++i)sprintf(addr+i*3,"%2.2X-",d->hw[i]);
#ifdef  SYSUNIX
    ttm=localtime(&d->t);
#else
    lft=(ulong long)d->t *10000000LL + 0xFDE04000LL ;
    FileTimeToSystemTime(&ft,&stm);
#endif    
    p+=sprintf(p,
     "<tr>"
     "<td>%u.%u.%u.%u</td>"
     "<td>%u</td>"
     "<td>%s</td>"
     "<td>%s %s</td>"
     "<td>%2.2u.%2.2u.%2.2u %2.2u:%2.2u:%2.2u</td>"
     "<td>%s"
     "<font size=2 class=f2>"
     "<form method=GET action=/$_admin_$dhcp>"
     "<input type=hidden name=a value=%u>"
     "<input type=submit name=s value=Release> "
     "<input type=text name=d maxlength=17 size=12 value=\"%u.%u.%u.%u\">"
     "<input type=submit name=s value=Change>"
     "</font>"
     "</form>"
     "<!-- . --></td></tr>"
     ,d->ipc[0],d->ipc[1],d->ipc[2],d->ipc[3]
     ,d->hw_t[0]
     ,addr
     ,d->domain
     ,(d->flg&dhcpReleased)? "(released)" :""
#ifdef  SYSUNIX
     ,ttm->tm_mday,ttm->tm_mon+1,ttm->tm_year%100,ttm->tm_hour,ttm->tm_min,ttm->tm_sec
#else
      ,stm.wDay,stm.wMonth,stm.wYear,stm.wHour,stm.wMinute,stm.wSecond
#endif
      ,(s_flg&FL_RADMIN)?"":"<!-- "
     ,d->ip
     ,d->ipc[0],d->ipc[1],d->ipc[2],d->ipc[3]
    );
    if( (l=p-b)>0x1000 ){if(Send(b,l)<=0) return -1; p=b; }
  }
 };

 p+=sprintf(p,"</table>");
 return Send(b,p-b);
}
void DelDHCPIP(ulong ip,char *n,char *t)
{
 DHCPbase *d,*pd;
 char *p;
 int l,i;
 pd=0;
 for(d=dhcp_ar;d;d=d->next)
 {
  if(d->ip==ip)
  {
   if(t && *t=='C' && n )
   { d->ip=ConvertIP(n);
   }
   else
   {
    MyLock(DHCPMutex);
    if(!pd)
    {
     dhcp_ar=dhcp_ar->next;
    }
    else
    {pd->next=d->next;
    }
    if(d>last_dhcp_loaded )
       FREE(d);

    if(allocated_dhcp_ip)allocated_dhcp_ip--;
    dhcp_need_save++;
    MyUnlock(DHCPMutex);
   }
   return;
  }
  pd=d;
 }
}

void LoadDHCP()
{
 int h,l,i,n;
 if((h=_lopen(dhcp_fname,0))>0)
 {
  if((l=FileSize(h))>0 && ! (l%sizeof(DHCPbase)) )
  {
   dhcp_ar=(DHCPbase*) ALLOC(l);
   sizeSize=l;
   _hread(h,(char *) dhcp_ar,l);
   n=l/sizeof(DHCPbase);
   for(i=0;i<n;i++)dhcp_ar[i].next=dhcp_ar+i+1;
   dhcp_ar[n-1].next=0;
   last_dhcp_loaded=dhcp_ar+i+1;
   allocated_dhcp_ip=n;
  };
  _lclose(h);
 }
}




int UDPSrvSock(int port,char *adapter)
{
 int s;
 struct sockaddr_in sa_server;
 memset((char *) &sa_server,0,sizeof(sa_server));
 sa_server.sin_family=AF_INET;

 if( (s = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1 )
 {   debug("could not get socket"); return 0;}
  setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char *)&one,sizeof(one));
  setsockopt(s,SOL_SOCKET,SO_BROADCAST,(char *)&one,sizeof(one));
  sa_server.sin_addr.s_addr=ConvertIP(adapter);  //htonl(INADDR_ANY);
  sa_server.sin_port=htons(port);
 if(bind(s,(struct sockaddr *) &sa_server, sizeof(sa_server) )
   )
   {
     debug("Error. Could not bind socket to port %u. (%d)" SER  ,port, WSAGetLastError() Xstrerror(errno));
     shutdown( s, 2 );
     closesocket( (int) s);
     return 0;
   }
   setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char *)&one,sizeof(one));
   setsockopt(s,SOL_SOCKET,SO_BROADCAST,(char *)&one,sizeof(one));
 return s;
}

void OptDHCP()
{
 char *t;
 int i;
 ulong ip;
 first_dhcp_ip   = ConvertIP(t=first_dhcpc);
 gateway      = ConvertIP(t=gatewayc   );
 netmask      = ConvertIP(t=netmaskc   );
 t=dhcp_dnsc;
 for(i=0;i<3; ++i)
 { dhcp_dns[i]   = ConvertIP(t);
   t=SkipSpace(t);
   if(*t!=',')break;
   ++t;
   t=SkipSpace(t);
 }
 last_dhcp_ip=htonl(first_dhcp_ip) + total_dhcp_ip + (total_dhcp_ip/128) ;
 NextDHCPIP();

}

int InitDHCP()
{
  if((dhcpd_so=UDPSrvSock(67,dhcp_addr))<0)
  {
   debug("Could not get secondary socket for DHCP");

  };
  if((dhcpd_s=UDPSrvSock(67,"0.0.0.0")) < 0)dhcpd_s=dhcpd_so;
  if(dhcpd_s>0)
  { LoadDHCP();
    OptDHCP();
  }

  return dhcpd_s;
}

uchar DHCP_opt[]={
0x63 ,0x82 ,0x53 ,0x63,
0x35 ,0x01 ,0x05 ,
0x3A ,0x04 ,0x1F ,0xFF ,0xFF ,0xFE,
0x3B ,0x04 ,0x1F ,0xFF ,0xFF ,0xFF,
0x33 ,0x04 ,0xFF ,0xFF ,0xFF ,0xFF
,0x36 ,0x04 
};

ulong next_dhcp_save;
ulong WINAPI DHCPServ(void * fwrk)
{
 dhcpPacket pkt;
 struct sockaddr_in  sa_client;
 int l,i,j,edu,s;
 ulong k,k2,tim;
 DHCPbase *pb,*pbrl, **ppb;
 uchar *t;
 char *hhh;
  while(is_no_exit)if( (s=RESelect2(3,3,dhcpd_so,dhcpd_s))>0 && (l=recvfrom(s,(char *)&pkt,sizeof(pkt),0,(sockaddr *)&sa_client,&(i=sizeof(sa_client))))>0 )
  {
   if( pkt.op==1 && pkt.hlen<=16
   )
   {

   /*
    i=_lcreat("dhcptst",0);
    _lwrite(i,(char *)&pkt,l);
    _lclose(i);
  // */
    pkt.op=2; 
    ppb=&dhcp_ar;
    pbrl=0;
    tim=time(0);
    MyLock(DHCPMutex);
    for(pb=dhcp_ar;pb;pb=pb->next)
    {
     if( (pb->hw_typeLengh==WORD_PTR(pkt.htype)) &&
          !memcmp(pb->hw,&pkt.chaddr,pkt.hlen) )
     {
      if(FL1_DHCPLO&s_flgs[1]){  if(pkt.ciaddr ) pb->ip=pkt.ciaddr; }
      else if(!pb->ip) goto lbFound2;
      goto lbFound;
     }
     if((!pbrl) && (pb->flg&0x4) &&  pb->t <tim )pbrl=pb;

     ppb=&(pb->next);
    }

    if(allocated_dhcp_ip < total_dhcp_ip)
    {
     if(pbrl)
     {
       pb=pbrl;
       next_dhcp_ip=pb->ip;
     }
     else
     {
      *ppb=pb=(DHCPbase *) ALLOC(sizeof(DHCPbase));
      ++allocated_dhcp_ip;
     }

     pb->hw_typeLengh=WORD_PTR(pkt.htype);


     memcpy(pb->hw,&pkt.chaddr,pkt.hlen);
     if(pkt.ciaddr &&
        (k=htonl(pkt.ciaddr) )>= (k2=htonl(first_dhcp_ip)) &&
        k<(k2+total_dhcp_ip) &&
        isIpFree(pkt.ciaddr)
       )
     {pb->ip=pkt.ciaddr;
     }
     else
      if(!(FL1_DHCPLO&s_flgs[1]) )
      {
lbFound2:
       pb->ip=next_dhcp_ip;
       NextDHCPIP();
      }



lbFound:
    // debug("ready to ret %X %X %X",pb->ip,next_dhcp_ip,first_dhcp_ip);
     pkt.yiaddr=pb->ip;
     //pkt.ciaddr=pb->ip;
     //pkt.giaddr=gateway;
     //pkt.siaddr=gateway;
     pkt.secs=0;
    pb->flg=0;
    DHCP_opt[6]=5;
//    pkt.ciaddr=0;

    edu=0;
    if(DWORD_PTR(pkt.vend[0])==0x63538263 )
    {
     for(t=pkt.vend+4; *t!=0xFF && t<(pkt.vend+128) ; t+=*t?t[1]+2:1 )
     {
     // debug("*%X %X %s",*t,t[1],t+2 );

      if(*t==50)
      {if(DHCP_opt[6]==5 && DWORD_PTR(t[2])!=pkt.yiaddr )DHCP_opt[6]=6;
       if(!pb->ip)pb->ip=DWORD_PTR(t[2]);
 //      debug("!!! %X",DWORD_PTR(t[2]));
      }
      if(*t==53)
      {


       switch(edu=t[2])
       {
        case 3:
        if(pkt.ciaddr && pkt.ciaddr!=pkt.yiaddr)
        {DHCP_opt[6]=6;      }
        else
        {
          pb->t=tim+86400;
          pb->flg&= ~dhcpTry;
        }

        break;
        case 4:
        case 7: pb->flg|=dhcpReleased; if(0){
        case 1: pb->t=tim+3; pb->flg|= dhcpTry; } DHCP_opt[6]=2;
       }
//       debug("REQ %u %u",t[2],pkt.ciaddr);

      }
      if(*t==12)
      {
       i=t[1];
       if(i>63)i=63;
       strncpy(pb->domain,(char *)t+2,i);
       pb->domain[i]=0;
       //break;
      }
     }
    }
    AddToLogDNS("DHCP",0,(sockaddr_in6 *) &sa_client,pb->domain);

     //DWORD_PTR(pkt.vend[0])=0;
     //DWORD_PTR(pkt.vend[4])=0x050135;
     memcpy(pkt.vend,DHCP_opt,sizeof(DHCP_opt) );
     l=sizeof(DHCP_opt);
     //if(edu) pkt.vend[4+2]=2;
    // WORD_PTR(pkt.vend[l])=0x436;
    DWORD_PTR(pkt.vend[l])=inet_addr(hhh=dhcp_addr);  //dhcp_ip;
    l+=4;
     WORD_PTR(pkt.vend[l])=0x401;
     DWORD_PTR(pkt.vend[l+2])=netmask;
     l+=6;


     if(gateway)
     {WORD_PTR(pkt.vend[l])=0x403;
      DWORD_PTR(pkt.vend[l+2])=gateway;
      l+=6;
      //WORD_PTR(pkt.vend[l])=0x420;
      //DWORD_PTR(pkt.vend[l+2])=gateway;
      //l+=6;
     }
     if(dhcp_dns[0])
     {
 //     debug("DHCP dns %u",l );
      WORD_PTR(pkt.vend[l])=0x6;
      i=l+1;
      l+=2;
      j=0;
      while(dhcp_dns[j])
      { DWORD_PTR(pkt.vend[l])=dhcp_dns[j];
        pkt.vend[i]+=4;
        l+=4;
        ++j;
      }
     }
     if(DhcpServerName)
     {
      sprintf(pkt.sname,"%.63s",DhcpServerName);
//      debug("DHCP dns %s %u",DhcpServerName,l );

      j=sprintf((char *) (pkt.vend+l+2),"%.63s",DhcpServerName);
      pkt.vend[l]=15;
      pkt.vend[l+1]=j+1;

      l+=j+3;
     }

     pb->t=time(0);

     dhcp_need_save++;

     pkt.vend[l++]=0xFF;
     DWORD_PTR(sa_client.sin_addr.s_addr)=inet_addr(hhh=dhcp_bcast);
    pkt.flags=0; 
    l+=offset(dhcpPacket,vend);
   // debug("DHCP %u offs %u",l,offset(dhcpPacket,vend));
    /*
    i=creat("dhcptst2",0666);
    write(i,(char *)&pkt,l);
    close(i);
    // */
     if(!(FL1_DHCPLO&s_flgs[1]) )
       sendto((dhcpd_so>0)? dhcpd_so:dhcpd_s,(char *)&pkt,l,0,(sockaddr*)&sa_client,sizeof(sa_client) );
    }
    else
     AddToLogDNS("DHCP",0,(sockaddr_in6 *) &sa_client,"Error. No more addresses");

    MyUnlock(DHCPMutex);
    if(!next_dhcp_save)next_dhcp_save=tim + DHCP_SAVE_INTERVAL;
    else if(next_dhcp_save<tim)
    {
       SaveDHCP(); 
       next_dhcp_save=tim + DHCP_SAVE_INTERVAL; 
    }    
    
        

   }
  }
  return 0;
}
