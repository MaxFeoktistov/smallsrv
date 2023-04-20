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
#endif


//char *dns_server_for_mail=0;
struct d_msgt {
  ushort id,
         flags,
         qdcount,
         ancount,
         nscount,
         arcount;
}
 msg_reqmx={0x100,0x1,0x100,0,0,0};


char* CopyDName(char *t, char *s)
{
 char *tt,*t1,x;
 strcpy(t+1,s);
 tt=t;
 do{
    t1=strchr(tt+1,'.');
    if(!t1)
    {
     *tt=strlen(tt+1);
     break;
    }
    *tt=t1-tt-1;
    tt=t1;
   }while((t-tt)<255);
 return  tt + (*tt)+2;
};


void  MHinfo::InitHst(uchar *t1)
{
 char  b[80];
// uchar *t1;
 int   tl[8];
 int i,j,k;
 memset(ta,0,sizeof(ta)+20);
// t1=beg+4;
 for(j=i=0;j++ <7;++i)
 {
  t1=(uchar *)DecodeName(b //t2
    ,(char *) t1, (char *) beg);
//  debug("%s %X",b,DWORD_PTR(*t1));
  if(  DWORD_PTR(*t1)==0x01000F00 )
  {
   ta[i]=(char *) t1 +12;
   tl[i]=htons(WORD_PTR(t1[10]));
   FirstIP=t1=(uchar *)DecodeName(b,(char *) t1 +12, (char *) beg);
  }
  else
  {
    if(  DWORD_PTR(*t1)==0x01000100 || ! (t1[1]) ) break ;
    FirstIP=t1=(uchar *)DecodeName(b,(char *) t1 +10, (char *) beg);
    --i;
  }

//  debug("MH:%s",b);
  //if(!t1) break;
 }
 nhst_hst=i;
 LastIP=FirstIP;
 for(k=0; k<i; ++k)
   for(j=k+1;j<i;++j)
    if(tl[j]<tl[k])
    {xchg(ta[k],ta[j]);
     xchg(tl[k],tl[j]);
    }

}
uchar *MHinfo::GetNextMH(char *t)
{
 char  b[80];
 char *t1;
 //debug("==========%X %X %X",LastIP,ta[0],ta[1]);
 if(DWORD_PTR(LastIP)==1)
 {
  //debug("!!!!!! %X %X",ta[0],ta[1]);
  strcpy(t,ta[0]);
  LastIP=0;
  t1=((char *)(ta+1))-10;
  goto lbIP;
 }
tryNext: ;
 if(cur_hst>=nhst_hst) return 0;
 if( ((ulong) (ta[cur_hst]-(char *) beg)) >500ul )
 {
  // debug("MH error %X %X %u",ta[cur_hst],(char *) beg,cur_hst);
   return 0;
 }
 DecodeName(t,ta[cur_hst],(char *) beg);
 sip[0]=0;
 if(FirstIP)
 {
   if(!LastIP)LastIP=FirstIP;
//   t[68]=0;
   do
   {
    t1=DecodeName(b,(char *)LastIP,(char *) beg);
//debug("FMH |%s| %X %s",b,DWORD_PTR(*t1),t);
    if(WORD_PTR(*t1)!=0x0100) break;
    LastIP=(uchar *) (t1+10+t1[9]);
    if( DWORD_PTR(*t1)==0x01000100 &&
        ! stricmp(t,b)
      )
    {
 lbIP:
     sprintf(sip,"%u.%u.%u.%u",((uchar *)t1)[10],((uchar *)t1)[11],((uchar *)t1)[12],((uchar *)t1)[13]);
//     debug("MHIP:%s %s",t,sip);
     lIP=DWORD_PTR(t1[10]);
     ++cur_ip;
     return (uchar *)t;
    }
   }while(1);
   ++cur_hst;
   LastIP=FirstIP;
   if(cur_ip)
   {cur_ip=0;
    goto tryNext;
   }
 }
 else ++cur_hst;

 return (uchar *)t;
}

//inline uchar * GetNextMH(uchar *beg) //,uchar *t1,char *t2)
//{ return  ((MHinfo *)beg)->GetNextMH((char *)beg+512);}

int  MHinfo::CheckIP(ulong ip)
{
 struct hostent *hp;
 ulong **p;

  while(GetNextMH((char *) name) )if(sip[0])
  {
   if(lIP ==ip )return 1;
  }
  else
  {
    if( (hp=gethostbyname((char *) name)) )
    {
     for( p=(ulong **)  hp->h_addr_list; *p ; ++p )if(*p[0]==ip)return 1;
    }
  }
  return 0;
}

//inline int CheckMHIP(uchar *beg,ulong ip) { return  ((MHinfo *)beg)->CheckIP(ip);}

//---------
char* GetMailHost(char *adr, d_msg  *dmm,int jchk)
{
 int dns_s;
 int addr_dns;
 struct sockaddr_in sa;
 //d_msg  dm;
#define dm  (*dmm)
 char *t,*t1,*t2;
 int i;
 int h;
 int j=4;
// if(dns_s<=0)
 {
  if( (!dns_server_for_mail) ||
   (dns_s= socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
         return 0;
  }
#ifdef SYSUNIX
  fcntl(dns_s, F_SETFD, fcntl(dns_s, F_GETFD) | FD_CLOEXEC);
#endif
  addr_dns=ConvertIP(t=dns_server_for_mail);
 }

 memset((char *)&sa, 0, sizeof(sa));
 sa.sin_addr.s_addr=addr_dns;
 sa.sin_family=AF_INET;
 sa.sin_port=0x3500;  //htons(53);
 ++msg_reqmx.id;
 memcpy(&dm,&msg_reqmx,sizeof(msg_reqmx));
 t=CopyDName(dm.buf, adr );
 WORD_PTR(*t)= 0x0F00;     //type MX
 WORD_PTR(t[2])=0x0100;   //class IN

 do{
  if( (i=sendto(dns_s,(char *)&dm,t-(char *)&dm+4,0,(sockaddr *)&sa ,sizeof(sockaddr) )) <=0)
  { 
    //dbg("SMTP DNS error...");
    return 0;
  }
  i=-1;
  if(RESelect(2+j,0,1,dns_s)>0)
  {
   i=recvfrom(dns_s,(char *)dmm,512,0,(sockaddr *)&sa, &(i=sizeof(sockaddr)));
   break;
  }
 }while(--j>0);

 CloseSocket(dns_s);
//debug("MAILIP: %d %X %u %X %X adr %X",i,dmm->flags,jchk,s_flgs[1]&FL1_MHST,t+4,adr);
 if( i<=0 )
 {
   //dbg2("Resv error...");
  return 0;
 }
 //debug("x %X,",(dmm->flags&0xF00));
 if((dmm->flags&0xF00) || ! (dmm->ancount) )
 {struct hostent *hp;
  //debug("|%s| %X %u",adr,!(s_flgs[1]&FL1_MHST),jchk);

  if( (jchk)   ||
      ((s_flgs[1]&FL1_MHST)==0ul)
      ||  !(hp=gethostbyname(adr)) )
      return 0;
        //||
        ;

   //debug("hp=%X %X",hp,hp?*(ulong *)(hp->h_addr):0);
  // if( !(hp)  )return 0;

  memset(((MHinfo *)dmm)->ta,0,sizeof(((MHinfo *)dmm)->ta)+20);
  ((MHinfo *)dmm)->ta[0]=adr;
  DWORD_PTR(((MHinfo *)dmm)->ta[1])=*(ulong *)(hp->h_addr);
  DWORD_PTR(((MHinfo *)dmm)->LastIP)=1;
 }
 else if(!jchk)
 {
  DWORD_PTR( ((char*)dmm)[i] )=0;
 // ((char*)dmm)[0]=0;
  ((MHinfo *)dmm)->InitHst((uchar *)t+4);
 }
 return t+4;
};
