/*
 * Copyright (C) 1999-2021 Maksim Feoktistov.
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

#ifdef SEPLOG

#undef debug
#undef AddToLog
#undef AddToLogDNS

#define debug(a...)  sepLog[7]->Ldebug(a)
#define AddToLog(a...)  sepLog[7]->LAddToLog(a)
#define AddToLogDNS(a...)  sepLog[7]->LAddToLogDNS(a)


#endif

#if defined(SYSUNIX) && (! defined(ARM) ) 
#include <netinet/ip.h>

#endif

#if (! defined(SYSUNIX)) && ! defined(IP_TTL)

#define IP_TOS 3
#define IP_TTL 4 

#endif

#ifdef IP_TOS

#ifndef IPTOS_LOWDELAY

/*
 * Definitions for IP type of service (ip_tos)
 */
#define	IPTOS_LOWDELAY		0x10
#define	IPTOS_THROUGHPUT	0x08
#define	IPTOS_RELIABILITY	0x04

/*
 * Definitions for IP precedence (also in ip_tos) (hopefully unused)
 */
#define	IPTOS_PREC_NETCONTROL		0xe0
#define	IPTOS_PREC_INTERNETCONTROL	0xc0
#define	IPTOS_PREC_CRITIC_ECP		0xa0
#define	IPTOS_PREC_FLASHOVERRIDE	0x80
#define	IPTOS_PREC_FLASH		0x60
#define	IPTOS_PREC_IMMEDIATE		0x40
#define	IPTOS_PREC_PRIORITY		0x20
#define	IPTOS_PREC_ROUTINE		0x10

#endif

#define IPTOS_DNS IPTOS_LOWDELAY|IPTOS_RELIABILITY

#endif

#define csh_flg (0x4000&s_flg)

#define dbg3(a) AddToLogDNS(a,WSAGetLastError(),(sockaddr_in *) &sa_client);
#define dbg6(a) AddToLogDNS(a,0,(sockaddr_in *) &sa);
#define dbg4(a) debug(a)
//pprot+=sprintf(pprot,"\r\n!%s\r\n",a);
//#define dbg5(a) pprot+=sprintf(pprot,"\r\n!%X\r\n",a);

#define DDEBUG(a)
// debug("%s:%u " a ,__FILE__,__LINE__  );
#ifdef SYSUNIX

#include "heap.cpp"

Heap *dnsheap;

//#define DEBUG_DNS_MEM
#ifdef  DEBUG_DNS_MEM
struct DBGMem
{
 uint l;   
 uint where;
};
void *DNS_ALLOC(uint a)
{
  union{  
  void *r;
  DBGMem *d;
  uchar *us;
  };
  if((r=dnsheap->Alloc(a+sizeof(DBGMem)+4)))
  {
   DWORD_PTR(us[a+sizeof(DBGMem)])= 0x5A5A55AA;
   d->l = a;
#ifndef x86_64   
   d->where = (&a)[-1];
#endif
    d++;    
  }
  return r;
    
}
void DNS_FREE(void *a)
{
  union{  
   DBGMem *d;
   uchar *us;
  };
  
  d=(DBGMem *)a;
  d--;
  if(DWORD_PTR(us[d->l+sizeof(DBGMem)]) != 0x5A5A55AA ) 
    debug("memory error: %X %u %X %X",a,d->l,d->where,us[d->l+sizeof(DBGMem)]);  
  dnsheap->Free(d);
    
}

#else
#define DNS_ALLOC(a) (dnsheap->Alloc(a))
//(Malloc(a))
#define DNS_FREE(a) (dnsheap->Free(a))
//(free(a))
#endif
#else

HANDLE wdnsheap;

#define DNS_ALLOC(a)  HeapAlloc(wdnsheap,HEAP_ZERO_MEMORY,a)
//(malloc(a))
#define DNS_FREE(a) HeapFree(wdnsheap,0,a)
//(free(a))
#endif

#define DOMSIZE 512

/*
 *  a resource record is made up of a compressed domain name followed by
 *  this structure.  All of these ints need to be byteswapped before use.
 */
struct rrpart {
    ushort rtype,          /* resource record type = DTYPEA */
           rclass;         /* RR class = DIN */
    ulong  ttl;            /* time-to-live, changed to 32 bits */
    ushort rdlength;       /* length of next field */
    uchar  rdata[DOMSIZE]; /* data field */
} __attribute__ ((packed));


#define  rtypeA   1
#define  rtypeNS  2
#define  rtypeCNAME 5
#define  rtypeMX 15
#define  rtypeTXT 16
#define  rtypeSPF 99
#define  rtypePTR 12
#define  rtypeSOA  6
#define  rtypeAAAA 28
#define  rtypeAXFR -4
#define  rtypeANY  -1
#define  rtypeCAA  257



//                     0 1 2 3 4  5  6 7  8  9  0   1  2   3 4  5 6
const uchar ArrTYPE[]={1,1,5,0,15,0,16,99, 2, 0, 2,  0, 0xC,0,28,0,6};
//                     A A CNM MX  TXT SPF NS   NSA     PTR       SOA


const uchar DNSSupportedTYpes[]={1,2,5,6,12,15,16,28,99};
//#define intel16(a)  (((a)>>8ul)|(((a)&0xFF)<<8ul))

#ifdef SYSUNIX
#define DMALLOC Malloc
#else
#define DMALLOC   malloc
#endif
//char dbg_bfr[0x1000];


struct NSRecord
{
  uchar l,l1,l2,l3;
  int type;
  ulong ttl,hash;
  char  *dt;
  union {
   char *dr;
   ulong ip;
  };

  char* Data()
  { return dr; //dt+l1;
  }
  char* AddRR(d_msg *dm,char *t,int wc);
  int Set(char *name,ulong hash,int type,ulong ttl,ulong ladv);
  void FreeRR();
  int isRecord(char * r);

} __attribute__ ((packed));


struct CheckPoint;




struct NSRecordArray
{
 int type;
 NSRecordArray *next;
 int cnt;

 ushort *hash_list_lo,*hash_list_hi,*hash_list_cnt;
 int need_rehash;
 char *smem;

 NSRecord  d[1];

 void   Save(char *fname);

 NSRecord  *SetRecord(char *name,ulong hash,int type,ulong ttl,ulong ladv);
 int AddRRfromReply(d_msg *pdm,char *t,char *et,int rtyp);
 int FillDomain(char *mem,char *dt,CheckPoint *cp);
 void Rehash();
 void UpdHash(ulong hash,ulong i);
 void DelHash(int k);
};

NSRecordArray  *arr_rr[2];
#define host_rr arr_rr[0]
#define cash_rr arr_rr[1]
NSRecord  *ProviderNS[2];

struct Secondary;

int sock_secondary;
time_t cur_time;
struct Secondary
{
 struct Secondary *next;
 NSRecordArray *parr ;
  int state;
  time_t next_chk;
  ulong lastserial;
  ulong refresh;
  char host[64];  
  sockaddr_in sa;
  char fname[512];
  
  int CheckSecondary();
  void LoadSecondary();
  void LoadFile();
};

d_msg  SOAreq;

struct Secondary *seconds;
struct timeval  tvlast_time, old_time;
#define last_time tvlast_time.tv_sec

long Dtime(timeval *t1,timeval  *t2)
{
  return  (t1->tv_sec-t2->tv_sec)*1000000 +  (t1->tv_usec-t2->tv_usec);
}


NSRecord  *FindRec(NSRecordArray  * thi, NSRecord  *first,ulong hash,char *nm);
NSRecord  *FastFindRec(NSRecordArray  * thi, NSRecord  *first,ulong hash,char *nm);
NSRecord  *FindFree(NSRecordArray  * thi,NSRecord  *first);
void DelDup(NSRecordArray  * thi,NSRecord  *rr);
void RemoveNS(NSRecordArray  * thi,char *bfx);



ulong MkName3(char *t)
{uint i=0,j=0,k=0;
 for(;*t;t++){i^=rol((int)(*t)|0x20,j); j+=*t|0x20; k++;};
 return  (i+j*k);
};

#define MkName MkName3



#ifdef USE_IPV6
inline ulong ADDR4(sockaddr_in6 *sa_ct)
{return  (sa_ct->sin6_family==AF_INET)?
  ((sockaddr_in *)sa_ct)->sin_addr. S_ADDR :
   sa_ct->sin6_addr.s6_addr32[3];
}


inline void SETADDR4(sockaddr_in6 *sa_c,ulong addr,ulong po)
{
#define sa_cc (*(sockaddr_in *)sa_c)
 if( s_flgs[2]&(1<<SDNS_IND))
 {memset(sa_c,0,sizeof(*sa_c));
  sa_c->sin6_addr.s6_addr32[2]=0xFFFF0000;
  sa_c->sin6_addr.s6_addr32[3]=addr;
  sa_cc.sin_family=AF_INET6;
 }
 else
 {sa_cc.sin_addr.s_addr=addr;
  sa_cc.sin_family=AF_INET;
  sa_cc.sin_port=po;
 }
#undef sa_cc
}

#else

inline ulong ADDR4(sockaddr_in6 *sa_ct){return sa_ct->sin_addr. S_ADDR ; }

inline void SETADDR4(sockaddr_in *sa_c,ulong addr,ulong po)
{sa_c->sin_addr.s_addr=addr;
 sa_c->sin_port=po;
 sa_c->sin_family=AF_INET;
}
#endif


int NSRecord::Set(char *name,ulong hsh,int typ,ulong ttl1,ulong ladv)
{
  l1=strlen(name)+1;
  l=l1+ladv+1;
  l2=ladv;
  ttl=ttl1;
  type=typ;
  hash=hsh;
  dr=dt+l1;
  return l;
}

void  NSRecord::FreeRR()
{
  if(dt)
  {
    DNS_FREE(dt);
    dt=0;
  }
  type=0;
};

NSRecord  *NSRecordArray::SetRecord(char *name,ulong hash,int type,ulong ttl,ulong ladv)
{
  NSRecord  *r;

  if((r=FindFree(this,0) ))
  {
    if(r->dt)
    {
      DelHash(hash);
      r->FreeRR();

    }
    r->Set(name,hash,type,ttl,ladv);
    r->dt=(char *) DNS_ALLOC(r->l+8);
    r->dr=r->dt+r->l1;
    memcpy(r->dt,name,r->l1);

    if(hash_list_lo)
    { UpdHash(hash,r-d);  }

  }
  return r;
}

//char errfile[]="00.dat";
int NSRecordArray::AddRRfromReply(d_msg *pdm,char *t,char *et,int rtyp)
{
 NSRecord  *rr,*rd;
 ulong  hsh,ttl,type,i,j,ret,r1;
 char *t1,*t2;
 char bfr[68];
 char bfr1[68];
 char bfr2[128];
 ret=0;
 r1=0;

    while(t<et)
    {
     t1=t;
     t=DecodeName(bfr,t,(char *)pdm );
     hsh=MkName(bfr);
     ttl=htonl(DWORD_PTR(t[4])); //TTL
     if(rtyp)ttl+=time(0)+20;



     if(t[1]<=0 || t[9]<0 )
     {
       //debug("Error %s %s\n%s",errfile,t1,dbg_bfr);
       /*
       i=creat(errfile,0644);
       write(i,pdm,et-(char*)pdm);
       close(i);
       errfile[0]++;
       // */
       break;
     }

  //  debug("Add record %s %d",bfr,t[1]);

      if(t[1]==rtyp && r1<pdm->ancount)++ret;
      ++r1;


      switch( j=htons(WORD_PTR(*t)) )
      {
        case 0x1:
         rr=SetRecord(bfr,hsh,1,ttl,4);
         //DWORD_PTR( * (rr->Data()) )
         rr->ip=DWORD_PTR(t[10]);
         break;
        case 6: // SOA
         t1=DecodeName(bfr1,t+10,(char *)pdm );
         t1=DecodeName(bfr2,t1,(char *)pdm );
         rr=SetRecord(bfr,hsh,6,ttl,(i=strlen(bfr1))+(strlen(bfr2))+24);
         j=sprintf(t2=rr->Data(),"%s %s",bfr1,bfr2);
         t2[i]=0;
         memcpy(t2+j+1,t1,20);
         break;
        case 2: //NS
        case 12: //PTR
        case 15:
        case 5:   // MX || CNAME
         DecodeName(bfr1,t+((t[1]==15)?12:10),(char *)pdm);
         rr=SetRecord(bfr,hsh,t[1],ttl,strlen(bfr1)+3);
         t1=rr->Data();
         if(t[1]==15)
         {
          WORD_PTR(*t1)=WORD_PTR(t[10]);
          t1+=2;
         }
         strcpy(t1,bfr1);
         break;
        default:
  //       debug("UT %u %u",t[1],t[9]);
         rr=SetRecord(bfr,hsh,j,ttl,((uchar *)t)[9]);
         memcpy(rr->Data(),t+10,((uchar *)t)[9]);
      }

      DelDup(this,rr);

      t+=((uchar *)t)[9]+10;
    }

    return ret;


}


int NSRecord::isRecord(char * nm)
{
 char *t;
  return (dt[0]=='*' && (t=stristr(nm,dt+1)) && ! (t[l1-2] ) ) ||
         !stricmp(nm,dt);
}

 NSRecord  * FindRec(NSRecordArray  * thi,NSRecord  *first,ulong hash,char *nm)
 {
   NSRecord  *last;
   char *t;
       
//   DDEBUG("")

   if(!thi) return 0;

//   DDEBUG("")

   if(thi->hash_list_lo)
   {
     return FastFindRec(thi,first,hash,nm);
   }

//   DDEBUG("")


   if(!first)first=thi->d;
   do{
    last=thi->d+thi->cnt;
    //debug("find2 %u %s %X %X",thi->cnt,nm,first,last);
    if( first<thi->d || first>=last )
    {
//     debug("find2 outside %u %s %X %X",thi->cnt,nm,first,last);
     if(!thi->next)return 0;
     thi=thi->next;
     if(first==last)first=thi->d;
    }
    else
    {
      while(first<last)
      {
       if(first->type && first->dt)
       {

    //if(nm[0]=='.' && first->type==rtypeNS && first->dt[0]=='.' )
    //  debug("root NS %X: %X %X |%s| |%s|",thi, hash,first->hash,nm,first->dt );
        //*
           
        if( (first->ttl<last_time) && first->ttl > (86400*60) )
        {

         DDEBUG("")

         thi->DelHash(first->hash);
         
         DDEBUG("")
         
         first->FreeRR();

         DDEBUG("")

        }
        else // */
        if( (  hash==first->hash  &&
                    !stricmp(nm,first->dt) )  ||
            (first->dt[0]=='*' && (t=stristr(nm,first->dt+1)) && ! (t[first->l1-2] ) ) ||
            (first->type==rtypeSOA && (t=stristr(nm,first->dt)) && ! (t[first->l1-1] ) )
          )
        {
          return first;
        }

       }
       first++;
      }
      //debug("not found %u %s %X %X",thi->cnt,nm,first,last);
      if(!thi->next)return 0;
      thi=thi->next;
      first=thi->d;
    }
   }while(1);
 }

void RemoveNS(NSRecordArray  * thi,char *nm)
{
   NSRecord  *last;
   NSRecord  *first;
   do{
    first=thi->d;
    last=thi->d+thi->cnt;
    //debug("find2 %u %s %X %X",thi->cnt,nm,first,last);
      while(first<last)
      {
       if(first->type==rtypeNS && !stricmp(nm,first->dr) )
       {
          thi->DelHash(first->hash);
          first->FreeRR();
         // debug("Found NS to del");
       }
       first++;
      }
   //   debug("not found %u %s %X %X",thi->cnt,nm,first,last);
     // if(!thi->next)return ;
   }while(thi=thi->next);
}



 void NSRecordArray::UpdHash(ulong hash,ulong i)
 {
   hash&=0x3ff;

   if( hash_list_lo[hash] > i )hash_list_lo[hash]=i;
   if( hash_list_hi[hash] <= i )hash_list_hi[hash]=i+1;
   hash_list_cnt[hash]++;

 }

void NSRecordArray::Rehash()
{
   ulong i,j;

   memset(hash_list_hi,0,0x1000);
   for(i=0; i<0x400 ;  ++i )hash_list_lo[i]=cnt;

   for(i=0;i<cnt;++i)
     if(d[i].type)
     { UpdHash(d[i].hash,i);
     }

   need_rehash=0;
}

void NSRecordArray::DelHash(int k)
{
 if(hash_list_lo)
  if(hash_list_cnt[k&=0x3FF])
  {
     if(! -- hash_list_cnt[k] )
     {
       hash_list_lo[k]=cnt;
       hash_list_hi[k]=0;
     }
     else need_rehash++;
  }
  else need_rehash=1024;
}

 NSRecord  * FastFindRec(NSRecordArray  * thi,NSRecord  *first,ulong hash,char *nm)
 {
   NSRecord  *last;
   char *t;
   int i,j,k;

   if(thi->need_rehash>128)thi->Rehash();

   i=hash&0x3FF;
   if(!first)
   {
     first=thi->d + thi->hash_list_lo[i] ;
   }
   do{
    last=thi->d + thi->hash_list_hi[i]  ; //thi->cnt;
    //debug("find2 %u %s %X %X",thi->cnt,nm,first,last);
    if( first<thi->d || first>=last )
    {
//     debug("find2 outside %u %s %X %X",thi->cnt,nm,first,last);
     if(!thi->next)return 0;
     thi=thi->next;
     if(first==last)first=thi->d;
    }
    else
    {
      j=0;
      while(first<last)
      {
       if(first->type)
       {
      //   printf("|%s| %u\n",first->dt,first->type);

        if( (first->ttl<last_time) && first->ttl > (86400*60) )
        {

          DDEBUG("")
          thi->DelHash(first->hash);
          first->FreeRR();

          DDEBUG("")

        }
        else if( ( hash==first->hash  &&
             !stricmp(nm,first->dt) ) )
        {
          return first;
        }

       }
       first++;
      }
   //   debug("not found %u %s %X %X",thi->cnt,nm,first,last);
      if(!thi->next)return 0;
      thi=thi->next;
      first=thi->d;
    }
   }while(1);
 }


void  DelDup(NSRecordArray  * thi,NSRecord  *rr)
 {
   NSRecord  *last,*first;
   char *t;
   register ulong hash;
   int i,j,k;
   if( (! thi->hash_list_lo)  || ! rr->l ) return;

   hash=rr->hash;
   i=hash&0x3FF;
   first=thi->d+ thi->hash_list_lo[i] ;

   do{
    last=thi->d+thi->hash_list_hi[i] ;  //thi->cnt;

    //debug("find2 %u %s %X %X",thi->cnt,nm,first,last);
    if( first<thi->d || first>=last )
    {
     if(!thi->next)return ;
     thi=thi->next;
     if(first==last)first=thi->d + thi->hash_list_lo[i];
    }
    else
    {
      while(first<last)
      {
       if(hash==first->hash  && first->type && first!=rr && first->l == rr->l && (!memcmp(first->dt,rr->dt,rr->l))
          &&  (first->type!=rtypeA || first->ip==rr->ip)
         )
       {
         if(first->ttl > rr->ttl ) rr->ttl =first->ttl;
         thi->DelHash(first->hash);
         first->FreeRR();
         return ;
       }
       first++;
      }
      if(!thi->next)return ;
      thi=thi->next;
      first=thi->d;
    }
   }while(1);
 }



 NSRecord  * FindFree(NSRecordArray  * thi,NSRecord  *first)
 {
   NSRecord  *last,*best=0;
   ulong  best_ttl=0xFFFFFFFF;
   if(!first)first=thi->d;
   do{
    last=thi->d+thi->cnt;
    if( first<thi->d || first>=last )
    {
     if(!thi->next)return best;
     thi=thi->next;
    }
    else
    {
      best=first;
      while(first<last)
      {
        if( (!first->type) || (first->ttl < last_time && thi==cash_rr ) )return first;
        if(first->ttl<best->ttl)
        {
          best_ttl=first->ttl;
          best=first;
        }
        first++;
      }
      if(!thi->next)return best;
      thi=thi->next;
      first=thi->d;
    }
   }while(1);
 };


char *hsfile;
int nsmut;

char DefaultSOA[]={
0,0,0,1, // SERIAL
0,0,0xA8,0xC0,//REFRESH=43200
0,0,0x70,0x80,//RETRY=28800
0,0x36,0xEE,0x80,//EXPIRE
0,1,0x51,0x80 //MINIMUM
};
int DNS_CACHE_SIZE;
ulong DNS_TTL=86400;
//0xC0A80000;


const char REQURSION_CALL[]="(recursion)";
char NSTypes[]={1,2,5,6,12,15,16,28,-4,-1,99};
char *NSTypesTXT[]={"A","NS","CNAME","SOA","PTR","MX","TXT","AAAA","AXFR","ANY","SPF"};

struct CheckPoint;
struct CheckPoint
{
 ulong interval,next_chk;
 ulong oldIP,newIP;
 int type;
 char *check;
};

CheckPoint *DRList;
ulong check_state;

struct DNSLogRepeat
{
 char name[128];
 union{
  TSOCKADDR   sa;
  sockaddr_in sa4;
 };
 time_t first,last;
 int cnt;
 
 int cmp(const char *t,TSOCKADDR  *sa);
 void set(const char *t,TSOCKADDR  *sa);
 void out();
};

#define REPEAT_Q_SIZE 24
#undef printf
DNSLogRepeat  logrepeat[REPEAT_Q_SIZE+1];
time_t log_flash_time;
int log_flash_pos;

int DNSLogRepeat::cmp(const char *t,TSOCKADDR  *psa){
 if(!cnt)return -1;
 
//  dprint("cmp |%s| |%s|\r\n",t,name);
#ifdef USE_IPV6
 //if(psa->sin6_port != sa.sin6_port) return -1;
 if(((sockaddr_in6 *)psa)->sin6_family==AF_INET6)
 {
   if( memcmp(psa->sin6_addr.s6_addr16,sa.sin6_addr.s6_addr16,sizeof(sa.sin6_addr.s6_addr16) ) )  return -1;
 }
 else
 {
   if( ((sockaddr_in *)psa)->sin_addr.s_addr != sa4.sin_addr.s_addr  ) return -1;
 }
#else
 //if( psa->sin_port != sa.sin_port ) return -1;
 if( psa->sin_addr.s_addr != sa.sin_addr.s_addr  ) return -1;
#endif
 
 
 return stricmp(name,t);
    
};

void DNSLogRepeat::set(const char *t,TSOCKADDR  *psa)
{
  // DBGLINE
  memcpy(&sa,psa,sizeof(sa));
  sprintf(name,"%.92s",t);
  first=cur_time;
  last=cur_time;
  cnt=1;
}


void DNSLogRepeat::out()
{
  char  bt[80];
  DBGLINE
  if(cnt>1)
  {
   DBGLINE
   sprintf(bt,"repeated %u times in the %us (:%2.2u:%2.2u - %2.2u:%2.2u)",cnt,last-first, (first/60)%60,first%60,(last/60)%60,last%60 );
   AddToLogDNS(name,-3,&sa,bt); 
  } 
  cnt=0;
}


#ifdef SEPLOG

#define pprot  lpprot 
#define f_prot lf_prot
#define pcnt   lpcnt  
#define b_prot lb_prot

void TLog::LAddToLogDNS(const char *t,int n,TSOCKADDR  *sa,char *ad)
#else
void AddToLogDNS(const char *t,int n,TSOCKADDR *sa ,char *ad) //="")
#endif
{
 int ll,l,p1,p2,ltbr,i;
 char *ttt;
 char tbfr[128]; 
 SYSTEMTIME stime;
 
 
  if(n==257) ttt="CAA"; 
 else if((ttt=memchr(NSTypes,n,sizeof(NSTypes))))
  ttt=NSTypesTXT[ttt-NSTypes];
 else ttt="";
 l='>';
 p2=n?53:67;
 if(//t==REQURSION_CALL ||
   t[0]=='(')
 {
   l-=2;
 }

 if(n==-3)
 {
  ltbr=sprintf(tbfr,"%s %.64s",t,ad);
 }
 else
 {
 
  ltbr=sprintf(tbfr,"%c%s %s(%d)%.64s",l,t,ttt,n,ad);

  if(t!=REQURSION_CALL)
   for(i=0;i<REPEAT_Q_SIZE; i++)if( ! logrepeat[i].cmp(tbfr,sa) )
   {
     logrepeat[i].cnt++;
     logrepeat[i].last=cur_time;
     if( (!log_flash_time) || log_flash_pos==i ){log_flash_time=cur_time+20; log_flash_pos=i; }
     return;
   }
 }
 
#ifdef USE_IPV6
 char addr6[64];
 IP2S(addr6,(sockaddr_in *)sa);
 p1=htons((ushort)sa->sin6_port);
#else
 p1=htons((ushort)sa->sin_port);
#endif
 

 GetLocalTime(&stime);
 GetProt();
 pprot+=sprintf(pprot,//FmtBasic,
 "\r\n!-%c%2.2u/%2.2u %2.2u:%2.2u:%2.2u ["
#ifdef USE_IPV6
 "%s"
#else
 "%u.%u.%u.%u"
#endif
 ":%u%c%u] (t%d %u) ",
   l,
     stime.wDay,stime.wMonth,stime.wHour,stime.wMinute,stime.wSecond,
#ifdef USE_IPV6
 addr6,
#else
#ifndef SYSUNIX
     sa->sin_addr.S_un.S_un_b.s_b1,  sa->sin_addr.S_un.S_un_b.s_b2,
     sa->sin_addr.S_un.S_un_b.s_b3,  sa->sin_addr.S_un.S_un_b.s_b4,
#else
     sa->sin_addr.s_addr&0xFF, BYTE_PTR(sa->sin_addr.s_addr,1),
     BYTE_PTR(sa->sin_addr.s_addr,2), BYTE_PTR(sa->sin_addr.s_addr,3),
#endif
#endif
     p1,l,p2,count_of_tr,pval,t);
//     pprot+=sprintf(pprot,"%c%s %s(%d)%s",l,t,ttt,n,ad);
     memcpy(pprot,tbfr,ltbr+1);
     pprot+=ltbr;
     RelProt(&stime);
     if(  t!=REQURSION_CALL && n!=-3 )
     {
       time_t tb;
       if(!logrepeat[0].cnt)
       {
         i=0;
       }
       else
       {
         tb=logrepeat[0].last;
         l=0;
         for(i=1;i<REPEAT_Q_SIZE; i++)
         {
           if(!logrepeat[0].cnt)goto lbFoundFree;
           if(tb>logrepeat[i].last) { tb=logrepeat[i].last; l=i; }

         }
         i=l;
         logrepeat[i].out();
       }
    lbFoundFree:   
   // dprint("Set %u %s\n",i,tbfr);
       logrepeat[i].set(tbfr,sa);
     }
};

void FlashRepeatLog()
{
 int i;
 time_t  dtime=cur_time-32;
 time_t  mindtime=cur_time-3600;
   log_flash_time=0;
   for(i=0;i<REPEAT_Q_SIZE; i++)
   {
    if( logrepeat[i].cnt )
    {
     if(logrepeat[i].last<dtime ||logrepeat[i].first < mindtime  )  
     {
       if(logrepeat[i].cnt>1)
       {
         logrepeat[i].out();
       }
       logrepeat[i].cnt=0;
     }
     else
       if(logrepeat[i].cnt>1)log_flash_time=cur_time+20;
    } 
   }
   
}

inline void ChkFlashRepeatLog(){if(log_flash_time && log_flash_time<cur_time)FlashRepeatLog(); }

#define MAX_REPL_HST 32
#define MAX_REPL_NSHST 10
#define MAX_ALIST_HST 20

struct AList
{
  int n;
  ulong a[MAX_ALIST_HST];
  int AddIP(ulong ip);
  int Add(NSRecord *r){return AddIP(r->ip); };
  void Del(ulong c);
  int Find(ulong c);
  void Init(){n=0; }
};

int AList::AddIP(ulong ip)
{
  if(n<MAX_ALIST_HST)
  {
    a[n++]=ip; // DWORD_PTR(* (r->Data()) );
  }
  return n<MAX_ALIST_HST;
}

void AList::Del(ulong b)
{
 int i;
   for(i=0; i<n ; ++i)
   {
     if(a[i]==b)
     {
      a[i]=a[--n];
     }

   }

};

int AList::Find(ulong b)
{
  int i;
   for(i=0; i<n ; ++i)
   {
     if(a[i]==b)return 1;
   }
   return 0;
}

struct NSRecordLst;

struct DNSReq
{
 ushort cnt;
 ushort state;
#define dnsstWAIT_REPLY 1
#define dnsstWAIT_NS    2
#define dnsstWAIT_CNAME 4
#define dnsstLINK       8

#define dnsstLNK_CNAME   0x40
#define dnsstLNK_NS      0x20
#define dnsstUSED        0x10

#define dnsstUSOA       0x100
#define dnsstEDNS       0x200



 int  s_in[5];
 struct
#ifdef USE_IPV6
  sockaddr_in6
#else
 sockaddr_in
#endif
      sa_cl[5];
 struct timeval stim,ptim;
 ushort in_id[6];
 Req *doh_ptra[5];
#define doh_ptr doh_ptra[0]
 
 ulong  hsh;
 AList alist;
 AList repled;
 
 uchar cur,ttcnt,rcnt,lnk,aa,a2,a3;
 int  typ;
 int  l;
 ushort ll;
 union {
  char dmmx[80];
  ushort us_id;
  struct d_msgs dmms;
 };


  int NextSend(int rs);
  int ReReq(int rs,char *name,int rtyp=rtypeA);
  int  FindNSResend(int rs,NSRecordLst *ns,char *bfx,int min=0);
  void SendReply(d_msg  *pdm);
  void SendErrReply(d_msg  *pdm,char *);
  void FreeDNSReq();
};

#define MAX_DNS_REQ  96
DNSReq   dreq[MAX_DNS_REQ+1];
ulong cdreq;
ushort dns_id;

int MxNextSend;
int DNSReq::NextSend(int rs)
{

  sockaddr_in nsa_cl;
  char  bfx[128];
  char *t;
  int i;


  memset(&nsa_cl,0,sizeof(nsa_cl));


  nsa_cl.sin_port=0x3500;
  nsa_cl.sin_family=AF_INET;

  if( (!typ) || ttcnt > 24 || !alist.n )
  {
    debug("Error typ=%u %u %u %d %d",typ,l,alist.n,rs,ttcnt);
    return alist.n;
  }

  for(i=0;i<alist.n ; ++i)
  {

    if(cur>=alist.n)cur=0;
    nsa_cl.sin_addr.s_addr=alist.a[cur++];
    if(!repled.Find(nsa_cl.sin_addr.s_addr) )
    {


  MyLock(MxNextSend);



   if(!nsa_cl.sin_addr.s_addr)
   {
     debug("Error resend Bad addres %u %u",cur,alist.n);
   }
   else
   {
     ((d_msg *)(dmmx))->  arcount =0;
     sendto(rs,dmmx, l,0,(sockaddr *) &nsa_cl, sizeof(sockaddr_in) );
     state|=dnsstWAIT_REPLY;
   }
   if(cur >= alist.n) cur=0;
   ttcnt++;

   MyUnlock(MxNextSend);

   gettimeofday(&ptim,0);
   t=DecodeName(bfx,((d_msg *)(dmmx))->buf,dmmx );
   AddToLogDNS(REQURSION_CALL,t[1],(sockaddr_in6 *) &nsa_cl,bfx);

      return 1;
    }
  }

debug("No unrepled NS");
  return 0;
}


void DNSReq::SendErrReply(d_msg  *pdm,char *bfx)
{
     pdm->flags = 0x0380;
     pdm->ancount=0;
     AddToLogDNS("(Name not found...)",typ,&sa_cl[0],bfx);
     SendReply(pdm);
}

const uchar edns_record[]=
{
  0,
  0, 0x29,
  0x2, 0,
  0, 0, 0, 0, 
  0, 0                                                                                                       
};

void DNSReq::SendReply(d_msg  *pdm)
{
  char *t;
  int j;
  int s;
    j=0;
    if( (state&dnsstEDNS) && l<(512-sizeof(edns_record) )  )
    {
      memcpy( ((char *)pdm)+l,edns_record,sizeof(edns_record) ) ;
      l+=sizeof(edns_record);
      pdm->arcount++;
      state&=~dnsstEDNS;
    }    
    do{
      t=(char *)pdm;
      s=s_in[j];
      if(ll)  
      {t-=2;
       //debug("DNS TCP reply %d %d %X",s,ssudp,th.l);
       WORD_PTR(*t)=htons(l);
       l+=2;
      }
      pdm->id=in_id[j];
   //   debug("SendReply to %X id=%X l=%u s=%u sudp2=%d",ADDR4(sa_cl), pdm->id,l,s,sudp2);
      if(doh_ptra[j])
      {
        memcpy(doh_ptra[j]->pst,t,l);
        doh_ptra[j]->dirlen=l;
#ifdef USE_FUTEX
     futex((int *)&doh_ptra[j]->dirlen,FUTEX_WAKE,1,0,0,0);  
#endif          
        doh_ptra[j]=0;
//        debug("DoH send %d",l);
        

      }    
      else    
      {    
#ifdef _BSD_VA_LIST_
      if(ll) //s==stcp) // !=ssudp) // sdns)
        BSend(s,t,l);  else
#endif
      sendto(s,t,l,0,(sockaddr *) (sa_cl+j),
#ifdef USE_IPV6
      sa_cl[j].sin6_family==AF_INET6?sizeof(sockaddr_in6):
#endif
      sizeof(sockaddr_in) );
      }

     }while(++j < cnt && (!ll) );

   FreeDNSReq();

}


char* ConvertDomain(char *t,d_msg *dm)
{
DDEBUG("")
 register union{
  int i;
  char *r;
 };
 char *s;
 int j;
 if(!(t[1])){*t++=0; return t;}
 i=1;
 s=t;
 while(t[i])
 {
  if(t[i] == '.')
  {
   *t=i-1;
   t+=i;
   i=0;
  }
  ++i;
 }
 *t=i-1;
 t+=i+1;
// Pask
 while(*s)
 {
  for(r=dm->buf;r<s && (j=r-(char*)dm)<256 ; ++r)
  {
   if(!stricmp(r,s) )
   {
    *s=0xC0;
    s[1]=j;
    return s+2;
   }
  }
  s+=*s+1;
 }
 return t;
}

//--------


int IsNotMeta(char *t)
{
 int i;
 for(i=-1;t[i]=='\\'; i-- );
 return (i&1);
}


char *GetWorld(char * &s)
{register char *t=s;
 int dq=0;    
 while(*t==' '|| *t=='\t' ) ++t;
 s=strpbrk(t," \t;,#\r\n");
 while(s && (*s==';' || *s=='#' ) && !IsNotMeta(s)){ s=strpbrk(s+1," \t;,#\r\n"); dq++; }
 if(!s){s=t+strlen(t);}
 if(dq)
 {
   char *p,*r;
   p=t;
   r=t;
   while(t<s)
   {
     if(*t=='\\')
     {
       t++;
     }
   //  else if( t[1]!=';' && t[1]!='#' )  
   //  {
       *p++=*t;
   //  }
     t++;
   }    
        
   return r;    
 }    
 return t;
}

char* IPv6Addr(ushort *t, char *s)
{int i,j,k,n;
 ushort r1[12];
 char *p;
 j=0;
 i=0;
 k=0;
 while(*s && i<8)
 {
  if(*s==':')
  {
   if(s[1]==':' )
   { s+=2; t[i]=0; k=++i;
//     debug("+A %u %.25s ",i,s);
     continue;}
   ++s;
  }
  if(*s<'0' || (*s|0x20)>'f')break;
  n=strtoul(s,&p,16);
  if(s==p)break;
  if(*p=='.' && i)
  {n=ConvertIP(s);  //inet_addr(s);
   DWORD_PTR(t[i])=n;
   i+=2;
   break;
  }
  s=p;
  t[i++]=htons(n);
 }
 if(i<8 && k)
 {n=i-1;
  for(j=7;j>n;--j)
  {t[j]=t[n];
   if(n>=k)--n;
  }
 }
 return s;
}



char * NSRecord::AddRR(d_msg *dm,char *t,int wc)
{
 char *p,*s,*x,*z;
 CheckPoint *cp;

 int i,j;
 ulong tt;
DDEBUG("")
 //debug("AddRR %s %u ttl=%u",dt,type,ttl);
 if(!dt)return t;

 if(wc)
 {
  if(wc==2)
  {sprintf(t+1,"%u.%u.%u.%u.IN-ADDR.ARPA",
    ((uchar *)dt)[l1+3],((uchar *)dt)[l1+2],((uchar *)dt)[l1+1],((uchar *)dt)[l1]


    );
  }
  else
  {
    strcpy(t+1,dt);
    if( WORD_PTR(*dt)=='.' )
    {
      t[1]=0;

    }
  }
  t=ConvertDomain(t,dm);
 }
 else
 {
  DWORD_PTR(*t)=0xCC0;
  t+=2;
 }

 DWORD_PTR(*t)=0x1000000;  //class IN
 t[1]= type;
 t[0]= type>>8;  
 tt=ttl;
 if(tt>(86400*60) ) tt-=last_time;
 if(tt>(86400*60) ) tt=60;
 DWORD_PTR(t[4])=htonl(tt); 
 WORD_PTR(t[8]) =0x400;  // size

 p=t+9;
 if(wc==2)
 {t[1]=0xC;
  strcpy(t+11,dt);
  t=ConvertDomain(t+10,dm);
  goto exlp;
 }
 if(type==rtypeA)
 {
  DWORD_PTR(t[10])= ip; //DWORD_PTR(dt[l1])  ;
  for(cp=DRList; cp && cp->check ; cp++ )
    if( (cp->type & 0x80)  &&
         cp->oldIP == DWORD_PTR(t[10]) )
    {
     DWORD_PTR(t[10])=cp->newIP;
     tt=cp->next_chk-last_time+5;
     if(tt>86400)tt=32;
     DWORD_PTR(t[4])=htonl(tt);
    }
  return t+14;
 }

   
 t+=10;
 z=Data();
 if(type==rtypeMX)
 {
  WORD_PTR(*t)=WORD_PTR(*z);
  t+=2;
  z+=2;
 }
 if(type==rtypeMX || type==rtypeNS || type==rtypeCNAME || type==rtypeSOA  || type==rtypePTR )
 {
   i=sprintf(t+1,"%.64s",z);
   t=ConvertDomain(t,dm);
   if(type==rtypeSOA )
   {
    i+=sprintf(t+1,"%.64s",z+i+1);

//debug("AddSOA %s %s %X %X",z,t+1,DWORD_PTR(z[i+2]),DWORD_PTR(z[i+6]) );  // debug

    t=ConvertDomain(t,dm);
    memcpy(t,z+i+2,20);
    DWORD_PTR(*t)^=check_state;
    t+=20;
   }
 }
 else
 {
   memcpy(t,z,l2);
   if(type==rtypeAAAA && DWORD_PTR(t[l2-8])==0xFFFF0000 )     //!  Little indian 
   {
    for(cp=DRList; cp && cp->check ; cp++ )
      if( (cp->type & 0x80)  &&
          cp->oldIP == DWORD_PTR(t[l2-4]) )
      {
        DWORD_PTR(t[l2-4])=cp->newIP;
        tt=cp->next_chk-last_time+5;
        if(tt>86400)tt=32;
        DWORD_PTR(t[-6])=htonl(tt);
      }

   }
   t+=l2;
 }

exlp:
 *p=t-p-1;
 return t;
};




char* DecodeName(char *tt,char *s,char *dm)
{
 char *r=0;
 register char *t=tt;
 uint i,n,j;

// char *dbg_p=dbg_bfr;
 i=s-(char *)dm;
 n=0;

 if(*s)
 {while( (j=(*(uchar *)s) ) )
  {
   if( j>=0xC0 )
   {

    if(!r)r=s+2;
    j=( (j&0x3F)<<8) + ((uchar *)s)[1];
   // dbg_p+=sprintf(dbg_p,"%X %X %X %X %X\n",*(uchar *)s, ((uchar *)s)[1],j,i,r-dm);
    if(j>=i || ++n>=36 )break;
    s=((char *)dm)+j;
   }
   else
   {
    if( ( (t-tt) + j) >= 63)
    {
 //     dbg_p+=sprintf(dbg_p,">63 %X %u %.8s",j,t-tt,s+1);
      break;
    }
    memcpy(t,s+1,j);
    t+=j+1;
    t[-1]='.';
    s+=1+j;
    if( (t-tt)>63 ) break;
   }
  }
  t[-1]=0;
 }
 else
 {
  *t=0;
 }
 return (r)?r:(s+1);
}

int ChkThread;

struct linger dnschklng ={1,2};

ulong CheckRemoteDown(void * )
{
  CheckPoint *cp,*ocp;
  ulong tt,i;
  char *p;
  int s,port,r;
  Secondary *sec;
  ChkThread++;
//  debug("shutd %X %X %X\n",SHUT_RDWR,SHUT_RD,SHUT_WR) ;
  do{
    Sleep(1000);
    tt=time(0);
    i=0;
    if(DRList) for(cp=DRList; cp->check ; cp++ )
    {
      if(cp->next_chk<tt)
      {
        port=80;
        if( (p=strchr(cp->check,':')))
        {
          *p=0;
          port=atoui(p+1);
        }
        s=call_socket(cp->check,port);
        if(p)*p=':';
        if( s > 0l )
        {
          /*
          struct linger dnschklng;
           dnschklng.l_onoff=1;
           dnschklng.l_linger=1;
          */ 
          setsockopt(s,SOL_SOCKET,SO_LINGER,
#ifndef SYSUNIX
          (char *)
#endif                     
                     &dnschklng,sizeof(dnschklng));
          r=shutdown(s,2);
          closesocket(s);
          //CloseSocket(s);
          if(!r)
          {
           cp->type &= ~0x80;
           check_state&=~(1<<i);
           debug("DNS: Check %s passed",cp->check);
          }
          else
          {
            cp->type |= 0x80;
            check_state|=(1<<i);
            debug("DNS: Check %s failed  (shutdown %d) " SER ,cp->check, errno Xstrerror(errno));
          }
        }
        else
        {
          cp->type |= 0x80;
          check_state|=(1<<i);
          debug("DNS: Check %s failed",cp->check);
        }
        cp->next_chk=time(0)+cp->interval;
        ++i;
      }

    }

    cur_time = tt;
    for(sec=seconds; sec ; sec=sec->next)
    {
      //if( sec->next_chk < tt )
      {
        if( sec->CheckSecondary()>0 ) sec->LoadSecondary();
      }
    }
  }while(is_no_exit);
  return 0;
}


struct NSRecordLst
{
  int n;
  NSRecord  *hst[MAX_REPL_HST];
  void Add(NSRecord *r){if(n<MAX_REPL_HST){hst[n++]=r; } }
  char *Put(d_msg  *pdm,char *t,char *et);
  char *PutAdv(NSRecordArray *a,d_msg  *pdm,char *t,char *et);
  int  UpdateAList(NSRecordArray *a, AList *alist);
  void UpdateNS(char *bfx,int first); //,int speedflg);
  void Del(int i);

};

void NSRecordLst::Del(int i)
{
  if(n)
  {
    --n;
    if(i<n)
    {
       hst[i]=hst[n];
    }
  }
};


struct FndRec
{
 NSRecord  *psoa;
 //NSRecord  *rphst[MAX_REPL_HST];
 //NSRecord  *nshst[MAX_REPL_NSHST];
 //NSRecord  *cname[5];
 NSRecordLst rp;
 NSRecordLst ns;
 NSRecordLst cname;


 //int  nns,nrpl,cn_cnt;

 int Find(NSRecordArray *a, char *nm, ulong hash, long type);
 void Init(){ns.n=0; rp.n=0; cname.n=0; psoa=0;}
 char* AddFound(NSRecordArray *a,d_msg  *pdm,char *t,char *et);

};

int FndRec::Find(NSRecordArray *a, char *nm, ulong hash, long type)
{
 int ret=0;
 NSRecord *f=0;

// debug("find %s %X %X",nm,  hash,  type);
 do{

   f=FindRec(a,f,hash,nm);
   if(!f)break;
   //debug("found record %s %X %X",f->dt,  f->type,type);

   if(type==f->type || type==rtypeANY )
   {
    ++ret;
    //if(nrepl<MAX_REPL_HST)rphst[nrepl++]=f;
    rp.Add(f);
   }
   else if(f->type==rtypeNS && type<256)
   {
     //if(nss<MAX_REPL_NSHST)nshst[nns++]=f;
     ns.Add(f);
   }
   else if(f->type==rtypeCNAME) // && type==rtypeA)
   {
     //if(cn_cnt<5)cname[cn_cnt++]=f;
     cname.Add(f);
     //if( type==rtypeA || type == rtypeAAAA)
       ++ret;
   }

   if(f->type==rtypeSOA)
   {
     psoa=f;
     if(type==rtypeAXFR || type==rtypeSOA) return 1;
   }

   ++f;
 }while( ns.n<MAX_REPL_HST || rp.n<MAX_REPL_HST );
 //(nrepl<MAX_REPL_HST) || (nns<MAX_REPL_NSHST) );

 return ret;

};

char *NSRecordLst::Put(d_msg  *pdm,char *t,char *et)
{
  char *lt;
  int j,w=0,k=0,ww=1;
  do{
   for(j=0; j<n; ++j)
   {
    if( w>=0 && hst[j]->dt[0] =='*'  )
    {
      if(k) continue;   
      w++;
      continue;      
    }
    t=hst[j]->AddRR(pdm,lt=t,ww);
    if(t>=et)
    {
      return lt;
    }
    pdm->ancount++;
    k++;
   }
   if(w<=0 || k)break;
   w=-1;
   ww=0;
  }while(1);

  return t;

};



int NSRecordLst::UpdateAList(NSRecordArray *a, AList *alist)
{
 NSRecord *f;
 int j;
 char *t;
 ulong hash;
 ulong r=0;

    for(j=0; j<n; ++j)if(hst[j]->type!=rtypeA )
    {
        f=0;
        t=hst[j]->Data()+2*(hst[j]->type==rtypeMX);
        hash=MkName(t);
        while( (f=FindRec(a,f,hash,t) ) )
        {
          if(f->type==rtypeA )
          {
           if(! (alist->Add(f)) ) return alist->n;
           r|=1<<j;
          }
          ++f;
        }
    }
 return r;
}

char *ROOTSERVERNAME=".";
ulong ROOTSERVERHASH;

void NSRecordLst::UpdateNS(char *tt,int fst)
{
 NSRecord *f=0;
 int j,on;
 char *t;
 ulong hash;
 //j=speedflg&1;
 on=n;
 if( (s_flg&FL_DNSUPLVL) && !(n+fst)  )
 {
    //f=0;
    if(ProviderNS[0])Add(ProviderNS[0]);
    if(ProviderNS[1])Add(ProviderNS[1]);
   // if(f)++f;
    //while( (f=FindRec(host_rr,f,ROOTSERVERHASH,ROOTSERVERNAME) ) )
    //{ if(f->type==rtypeNS){ Add(f); if(n>=3)break; }  ++f;}
 }
// else if((s_flg&FL_DNSUPLVL)) debug("Up NS %u %u %s",n,fst,tt);

 
 do{

   hash=MkName(tt);
   for(j=0; j<2; ++j)
   {
    f=0;
    while( (f=FindRec(arr_rr[j],f,hash,tt) ) )
    {
       if(f->type==rtypeNS && stricmp(tt,f->Data() ) )
       {
         Add(f);
// !temp         
//         debug("Found NS %s %u",f->Data(),n);
         if(n>=MAX_REPL_HST)return ;
       }
       ++f;
    }
//*
    if(n!=on)
    {
     /*
     if( (!j) && (s_flg&FL_DNSUPLVL) && !on )
     {
       hst[0]=hst[--n];
       if(--n > 1) hst[1]=hst[n];
     }
      */
     return ;
    }
 //   */
   }
   if(tt==ROOTSERVERNAME )return;
   if( (tt=strchr(tt,'.') ) )
   {
    ++tt;
   }
   else tt=ROOTSERVERNAME;
   // j=(speedflg>>2)&1;
  }while(1);
};

char *NSRecordLst::PutAdv(NSRecordArray *a,d_msg  *pdm,char *t,char *et)
{
 char *lt;
 NSRecord *f;
 char *tt;
 ulong hash,j;


    for(j=0; j<n; ++j)
    {
      if(hst[j]->type==rtypeCNAME ||hst[j]->type==rtypeNS || hst[j]->type==rtypeMX )
      {
        f=0;
        tt=hst[j]->Data()+2*(hst[j]->type==rtypeMX);
        hash=MkName(tt);
        while( (f=FindRec(a,f,hash,tt) ) )
        {
          if( (f->type==rtypeA || f->type==rtypeAAAA ) && f->dt[0] !='*'  )
          {
            t=f->AddRR(pdm,lt=t,1);
            if(t>=et) return lt;
            if(t>lt)pdm->arcount++;
          }
          ++f;
        }
      }
    }
    return t;
}
char*  FndRec::AddFound(NSRecordArray *a,d_msg  *pdm,char *t,char *et)
{
 int j,edns;
 char *r;
    edns=pdm->arcount;
    if(edns==1)
    {
      et-= sizeof(edns_record); 
    }
    pdm->ancount=0;
    pdm->nscount=0;
    pdm->arcount=0;
    t= cname.Put(pdm,t,et);
    t= rp.Put(pdm,t,et);
    j=pdm->ancount;
    t= ns.Put(pdm,t,et);
    pdm->nscount=pdm->ancount-j;
    pdm->ancount=j;
    t= rp.PutAdv(a,pdm,t,et);
    //t= cname.PutAdv(a,pdm,t,et);
//    return ns.PutAdv(a,pdm,t,et);
    r=ns.PutAdv(a,pdm,t,et);
  /*  
    if(edns==1)
    {
      memcpy(r,edns_record,sizeof(edns_record))  ;
      r+=sizeof(edns_record);
      pdm->arcount++;
      
    }
    */
    return r;
}

int rndnscntr;
int DNSReq::FindNSResend(int rs, NSRecordLst *ns,char *bfx,int min)
{
  int i,j;
  char *t;
  ulong vns;
       if((!ns->n) && !(state&dnsstUSOA) ) ns->UpdateNS(bfx,ttcnt);
       if(!ns->n) return 0;

       alist.Init();
       vns =ns->UpdateAList(host_rr,& alist);
       vns|=ns->UpdateAList(cash_rr,& alist);
       //if(ttcnt)--ttcnt;
      // ttcnt=0;
       //memcpy(pdreq,th,sizeof(th));
       ++ttcnt;
       if(ProviderNS[0])cur=0;
       if(alist.n<=min || !NextSend(rs) )
       {
        if( (state&dnsstUSOA) ) return 0;
        if(ttcnt>20 || (tvlast_time.tv_sec-stim.tv_sec)>3 )return 0;
        state|=dnsstWAIT_NS;
        state&=~dnsstWAIT_REPLY;
        j=0;
        do{ i=rndnscntr++ % ns->n;
        }while( (vns&(1<<i) ) && ++j< ns->n );
        if(! ReReq(rs,ns->hst[i]->Data()) )
        {
          if(state&dnsstLINK)
          {
           //state=0;
           state&=~dnsstWAIT_NS;
           return 0;
          }
          ns->n=0;
          if( (t=strrchr(bfx,'.') ) )++t;
          else t=ROOTSERVERNAME;
          ns->UpdateNS(t,1);


          ns->UpdateAList(host_rr,& alist);
          ns->UpdateAList(cash_rr,& alist);
          if(!alist.n)return 0;
          state|=dnsstWAIT_REPLY;
          NextSend(rs);

        };
       }
  return 1;
}

void DNSReq::FreeDNSReq()
{
  int i;  
  if(state)
  {
 //   debug("Free req %u",cdreq);
    state=0;
    if(cdreq)--cdreq;
    for(i=0; i<cnt ; i++ )
      if(doh_ptra[i])
      {
         doh_ptra[i]->dirlen=-1;  
         doh_ptra[i]=0;
      }

  }
}

int CheckDNSDoS(sockaddr_in * sa_c);

const DNSReq* lastDNSReq=dreq+MAX_DNS_REQ;
DNSReq* GetFreeReq(DNSReq* th,ulong hsh)
{
   //dreq[MAX_DNS_REQ
 ulong k,bj,i,x;
 DNSReq* r=0;
 DNSReq* pdreq;

 k=last_time-5;

     if(cdreq>=MAX_DNS_REQ)
     {

      debug("Lot of DNS requests wait reply");
      x=0;
     // for(j=0;j<MAX_DNS_REQ;++j)

      for(pdreq=dreq;pdreq<lastDNSReq;pdreq++)
          if(pdreq->state)
      {
        if(k > pdreq->stim.tv_sec )
        {
         --cdreq;
         pdreq->cnt=0;
         pdreq->state=0;
        }
        else ++x;
      }
      cdreq=x;

     }

// debug("Redir2 %s",bfx);

//     hshx=MkName(bfx);
     x=0;
     k-=10;
//     for(j=0;j<MAX_DNS_REQ;++j)
     for(pdreq=dreq;pdreq<lastDNSReq;pdreq++)
         if(pdreq->state)
     {
      if( th!=pdreq )
      {
       if(pdreq->stim.tv_sec<k)
       {
  // debug("req timeout %d",j);
        pdreq->cnt =0;
        pdreq->state=0;
        if(cdreq)--cdreq;
        continue;
       }
       //if(pdreq->stim.tv_sec<(th.stim.tv_sec-5))continue;


       if( pdreq->hsh==hsh && ( pdreq->typ==th->typ))
       {
        if(pdreq->us_id== //pdm->id
                     th->in_id[0]
            && th->l==pdreq->l)
        {
         //if( ((sockaddr_in *)&sa_client)->sin_port==0x3500  )
         {
          for(i=0;i<pdreq->alist.n;++i)
           if(ADDR4( & th->sa_cl[0])
                //.sin_addr.s_addr
           == pdreq->alist.a[i])
           { // Loop detected
             debug("DNS loop");
             if(! -- (pdreq->alist.n))
             {
               //th->dmmx.flags = 0x0380;
               //th->SendReply(&th->dmmx);
               return 0;
             }
             pdreq->alist.a[i]=pdreq->alist.a[ pdreq->alist.n ]  ;
           }
         }

        }
        debug("DNS already try");

        for(i=0;i<pdreq->cnt ; ++i)
        {
          if(
            pdreq->in_id[i]==  th->in_id[0] //pdm->id
             &&
            ADDR4( & th->sa_cl[0]) == ADDR4( & pdreq->sa_cl[i] ) &&
             ((sockaddr_in *)& th->sa_cl[0])->sin_port == ((sockaddr_in *)& pdreq->sa_cl[i])->sin_port
            ) return 0;

        }
        debug("DNS another id");

        if(pdreq->cnt >= 4)return 0;

        k=pdreq->cnt++;
        pdreq->in_id[k]=     //pdm->id;
        pdreq->s_in[k]=th->s_in[0] ; //s;
        pdreq->doh_ptra[k] = th->doh_ptra[0] ;
        //if(th->doh_ptr)pdreq->s_in[k]=(int)()
        //if(s==ssudp) //sdns)
        memcpy(pdreq->sa_cl+k,th->sa_cl,sizeof(th->sa_cl[0]));
        //else{
        //  DWORD_PTR(pdreq->sa_cl[k])=(ulong)-1;
        //  ((ulong *) &(pdreq->sa_cl[k]))[1]=s;
        //}
        return 0;
       }




       for(i=0;i<pdreq->cnt ; ++i)
         if(ADDR4( th->sa_cl ) == ADDR4(  pdreq->sa_cl+i ) )
         {
           if(++x >= (MAX_DNS_REQ/2) )
           {
             debug("DNS DoS dettected");
             if(dns_dos_limit)CheckDNSDoS( (sockaddr_in*) (th->sa_cl) );
             return 0;
           }
           break;
         }
      }
     }
     else if(!r)r=pdreq;



    if(r)
    {
     memcpy(r,th, sizeof(*r));
     r->hsh=hsh;
     r->us_id=((++dns_id)<<8) | (r-dreq) ;
     //r->ttcnt=0;
     r->stim.tv_sec=tvlast_time.tv_sec;
     r->stim.tv_usec=tvlast_time.tv_usec;
     r->state|=dnsstUSED;
     r->repled.Init();
   //  debug("Alloc req %X",r->us_id);
     r->dmms.arcount=0;  // :-[
     DWORD_PTR(r->dmms.t3)=0;
     cdreq++;
    }

    return r;


}

DNSReq *FindReqById(ulong id)
{
  int i;
  if( (i=id&0xFF) < MAX_DNS_REQ  && dreq[i].us_id==id )return dreq+i;
  //for(i=0; i< MAX_DNS_REQ ; ++i) if(dreq[i].us_id==id)return dreq+i;
//  if(i<MAX_DNS_REQ)
//    debug("Not found req %u %X %X %X",i,dreq[i].us_id,id,dreq[i].state);

  return 0;
}

int DNSReq::ReReq(int rs,char *name,int rtyp)
{
  DNSReq *nxt;
  NSRecordLst  lst;


     debug("DNS Reqursion get %s %u %u type:%u",name,rcnt,ttcnt,rtyp);
     if( rcnt>3 || ttcnt>24 ) return 0;

     if((nxt=GetFreeReq(this,MkName(name) ) ) )
     {

//       nxt->rcnt++;
       rcnt++;   
       nxt->rcnt=rcnt;
       nxt->lnk=this - dreq;
       //debug("DNS Reqursion get %s %u link=%u %X %X",name,rcnt,nxt->lnk, nxt,this);

       nxt->state=dnsstWAIT_REPLY|dnsstLINK| ((state<<4)&(dnsstLNK_CNAME|dnsstLNK_NS) );

       strcpy(nxt->dmmx+13,name);
       nxt->l= ConvertDomain(nxt->dmmx+12,(d_msg *) (nxt->dmmx))-nxt->dmmx;
        //th.l= CopyDName(dmm.buf,bfr)-th.dmmx;
//       DWORD_PTR(nxt->dmmx[nxt->l])=0x1000100;
       DWORD_PTR(nxt->dmmx[nxt->l])=0x1000000 | htons(rtyp) ;
       nxt->l+=4;
       nxt->typ=rtyp; //rtypeA;
       lst.n=0;
       if(! (nxt->FindNSResend(rs,&lst,name)) )
       {
         nxt->FreeDNSReq();
         //state=0;
         return 0;
       };
       return 1;
     }

    return 0;
}

int ReinitDNSSocket(int k,int tcp=0);

int CheckBadDNSName(char *b)
{
  char *p;
  int i;
  i=0;
  for(p=b; *p ;++p)
  {
    if(*p=='.')
    {
      ++i;
      if(i>10)return 1;
    }
  }

  return 0;
}

int CheckDNSDoS(sockaddr_in * sa_c)
{
  LimitCntr *lip;
   if(!(lip=dnsDoS.Find(sa_c->sin_addr. S_ADDR)))
   {
     lip=dnsDoS.Push(); lip->ip= sa_c->sin_addr. S_ADDR; 
     lip->first=last_time+180;
  //   debug("DNS ANY");
   }
   if(lip->cnt++ > dns_dos_limit)return 1;
       
   if(lip->first<last_time)
   {
     lip->cnt=0;  
     lip->first=last_time+180;
   }
   else 
     lip->first++;  

   return 0; 
}

void  PrepareDefAReply(char *t)
{
    WORD_PTR(*t)    =0xCC0; //
    DWORD_PTR(t[2]) =DWORD_PTR(t[-4]);  // PTR
    DWORD_PTR(t[6]) =DNS_TTL;  
    WORD_PTR(t[10]) =0x400; // size
}
ulong WINAPI SetDNSServ(void * fwrk)
{
 int s,ss, //ssudp,
  sudp2=-1,
  stcp=0;
// ssudp=soc_srv[6 + 9*(((int)fwrk)/9 )] ;
     //sdns;
 //if(((int)fwrk)&1)
 ss=soc_srv[6 + ( 
#ifdef x86_64 
 *(uchar *) &fwrk
#else
  0xFF &(uint)fwrk
#endif  
) ] ;  //sdnst;
#ifdef USE_IPV6
union{
  struct sockaddr_in6 sa_server6;
  struct sockaddr_in sa_server;
 };
#else
 struct sockaddr_in sa_server;
#endif

 sockaddr_in *psa_client;
// sockaddr_in s_clt;

 ulong stt;
// int sck[20];
// ulong     mxnsnip[MAX_REPL_NSHST];
 ulong crp,cap,cns,l1,l2,xhst,ttl,cname_hsh,errcounter=0;
 char *ptmp=0,*t,*t1,*arpa,*et,*p,*lst,*ptyp,*pcn,*pns;

 NSRecord *hst,*hst1;
 FndRec   fnd;
// char *tolog;
 d_msg  *pdm;
 DNSReq *pdreq,*pdreq2;
// uchar     mxnsoff[32];
 union {
  DNSReq th;
//  char   bb[sizeof(DNSReq)+768+280+78+32];
  char   bb[1600];
 };
 fd_set set;
 fd_set er_set;
 timeval tval;
// char bfx[280];
// char bfr[78];
// char cnam_bfr2[65];
#define bfx  (bb + sizeof(DNSReq)+512 )
//(bb+968)
#define bfr (bb+1048)

 int i,j,k, //n_i,n_ii,n_x,n_j,n_k,nadvhst,x,cnamef
  soik,soi;
 union{
 ulong z;
 uchar bff[4];
 };
 /*
 ushort  max_size[2];
 ulong nm1[34];
 ulong nm2[34];
 ulong nm3[34];
 char  tp[34];
 */
 int isTCP;

 isTCP=(
#ifdef x86_64 
 *(uint *) &fwrk
#else
  (uint)fwrk
#endif  
         )%9;
#define mxnsoff ((uchar *)tp)
#define mxnsnip nm1


#define hshx      (th.hsh)
#define sa_client th.sa_cl[0]
//#define nscnt     th.cnt_ns
//#define l         (th.l)
//#define stime     (th.stim.tv_sec)
#define dmmc      (th.dmmx)
#define dmm       (*(struct d_msg *)& th.dmmx)
#define dip       (pns_hash->a)

 memset(&th,0,sizeof(th));
// memset(&s_clt,0,sizeof(s_clt));
// s_clt.sin_port==0x3500;
//DDEBUG("Enter")
 th.cnt=1;
 th.state=dnsstWAIT_REPLY|dnsstUSED;
 s=0;

 MyUnlock(nsmut);
 while(1){
 MyUnlockOwn(nsmut);
 if(ptmp){
   FREE(ptmp);
   ptmp=0;

 }
 th.ll=0;
 pdreq=&th;
 FD_ZERO(&set);
 FD_ZERO(&er_set);
 j=0;
 pdm= &dmm;

  
 if( isTCP )
 {
  i=sizeof(sa_client);
  if(!stcp)
#undef accept
   while( (stcp=accept(ss,(sockaddr *)&sa_client,(socklen_t *) &i)) < 0)
   {

     if(!is_no_exit)return 0;
     Sleep(500);
     ReinitDNSSocket( (
#ifdef x86_64 
 *(uchar *) &fwrk
#else
  0xFF &(uint)fwrk
#endif  
    )/9,1);
     ss=soc_srv[6 + 
#ifdef x86_64 
  *(uchar *) &fwrk
#else
  (0xFF &(uint)fwrk)
#endif  
      ] ; 
     
   }
   s=stcp;

//  dbg3("Accept");
  if( ( RESelect(5,300000,1,s ) <=0 ) || recv(s,(char *) &th.ll,2,0)!=2)goto lbEnd2;
  //FD_SET(stcp,&set);
  //j=stcp;
  th.s_in[0]=s;
  //FD_SET(s,&set);
  //j=k;
  soik=0;
  soi=0;
// debug("TCP 2 %d",s);
  goto lb_tcpudp;

 }
 else
 {
  s=ss; //sdns;

  for(soik=0;(k=soc_srv[6+soik*9])>0 && soik<9 ; ++soik )
  {
    FD_SET(k,&set);
    //FD_SET(k,&er_set);
    if(j<k)j=k;
  }


  if(sudp2>0)
  {
    FD_SET(sudp2,&set);
    if(j<sudp2)j=sudp2;
  }
  else --soik;
  if(s_flgs[2]&FL2_DOH)
  {
    FD_SET(doh_r,&set);   
    if(j<doh_r)j=doh_r;
  }    

 }
 th.s_in[0]=s;


 tval.tv_sec=(!cdreq)*3;
 tval.tv_usec=250000;

// debug("j=%u soik=%u fwrk=%u",j,soik,fwrk);
 if(
/*
     (
      (sudp2<=0)?
        (i=RESelect(  (!cdreq)*3,250000,1,s ))  :
        (s=RESelect2( (!cdreq)*3,250000,s,sudp2) )
*/
     (k=select(
#ifdef SYSUNIX
            j+1
#else
           0
#endif
            ,
#ifdef MINGW
#undef fd_set
        (fd_set *)    
#endif
                &set,0,0 //&er_set
               ,&tval))  >0
   )
 {    
 for(soi=0; soi<=soik  ; soi++ )
 {

  s=( isTCP )? stcp : (soi==soik && sudp2>0)? sudp2 : soc_srv[6+soi*9];

//debug("%u soi=%d",s,soi);
/*  if(FD_ISSET(s,&er_set) )
  {
    j=4;
    getsockopt(s,SOL_SOCKET,SO_ERROR,(char *)&i,(socklen_t*)&j);
    debug("DNS socket error %u(%u) %d %u",soi,soik,i,s);
    Sleep(300);
  }
  else*/ 
      if( 
#ifdef MINGW
          
          FD_ISSET(s, (fd_set *) &set) 
#else          
          FD_ISSET(s,  &set) 

#endif         
      )
  {


//debug("ISSET %u",s);
lb_tcpudp:
  th.s_in[0]=s;

 //t=(char *)
  psa_client=(sockaddr_in *) &sa_client;
//  dbg3("d 1");
  i=
#ifdef USE_IPV6
  (s_flgs[2]&(1<<SDNS_IND))?sizeof(sockaddr_in6):
#endif
  sizeof(sockaddr_in);
//#ifndef   MSG_TRUNC
//#warning "MSG_TRUNC undefined"
//#define MSG_TRUNC 0
//#endif
 // memset(&sa_client,0,i);
  if( (th.l=recvfrom(s,dmmc,600,0,(sockaddr *)&sa_client,&i) ) > 0 )
  {
    th.doh_ptr=0;
 lbDOH:   
 
//   DDEBUG("DD")

 
   DWORD_PTR(dmmc[th.l])=0;
// DEBUG:   
 
//   DWORD_PTR(dmmc[th.l+4])=DWORD_PTR(dmmc[th.l-6]);
   
   
//   dbg3("d 2");
//   tolog="";
   th.state=dnsstWAIT_REPLY;
   t=DecodeName(bfx,dmm.buf,(char *)pdm )+4;
   xhst=MkName(bfx);

 //debug("3 %d %d %s",s,th.l,bfx);
   
  // last_time=time(0);
   gettimeofday(&tvlast_time,0);
   MyLock(nsmut);
   if(!(dmm.flags & 0x80))  //QUERY ?
   { //Query
//    dbg3("d 3");
   // nadvhst=0;
    th.typ=i=t[-3]|(t[-4]<<8) ;
    th.cnt=1;
    th.in_id[0]=dmm.id;
    if(dmm.qdcount>1)
    {
      debug("dmm.qdcount = %u",dmm.qdcount);  
    }
    if(dmm.arcount)th.state|=dnsstEDNS;
    
    if(DNS_DoS_hosts[0] && bfx[0] && stristr(DNS_DoS_hosts,bfx))
    {
      AddToLogDNS(bfx,i,&sa_client,"DoS detected" );  
      continue;
    }
//     else
//     {
//       debug("Debug DoS: \"%s\" \"%s\"\r\n",bfx,DNS_DoS_hosts)  ;
//     }    
    
    if(    dns_dos_limit && 
        //  (th.typ==rtypeANY || th.typ==rtypeAXFR)
           (! memchr((void *)DNSSupportedTYpes,th.typ,sizeof(DNSSupportedTYpes)  ) )
           && CheckDNSDoS((sockaddr_in *) &sa_client) )
    {
      AddToLogDNS("DoS detected",i,&sa_client,(char *) ((isTCP)?" TCP":(th.doh_ptr)?"DoH":"") );  
      continue;
    }    
    AddToLogDNS(bfx,i,&sa_client,(char *) ((isTCP)?" TCP":(th.doh_ptr)?"DoH":"") );
    th.hsh=xhst; //=MkName(bfx);
    l1=strlen(bfx);
    DDEBUG("")
    if(l1>64)
    {
      debug("Bad size of name %s",bfx);
      continue;
    }
    if(CheckBadDNSName(bfx))
    {
      debug("Bad DNS name %s",bfx);
      continue;
    }
    DDEBUG("")
  /*  
    WORD_PTR(*t)    =0xCC0; //
    DWORD_PTR(t[2]) =DWORD_PTR(t[-4]);  // PTR
    DWORD_PTR(t[6]) =DNS_TTL;  
    WORD_PTR(t[10]) =0x400; // size
*/


    if( dnsblname && dnsblname[0] && i==1 &&  (t1=stristr(bfx,dnsblname)) && ! t1[strlen(dnsblname) ] )
    {
      PrepareDefAReply(t);  
      if(!ipcnts[1].Find( htonl(ConvertIP(t1=bfx)) ))
      {
       dmm.flags = 0x0385;
       goto lbSend;
      }
      DWORD_PTR(t[12]) = 0x0200007F ;
      t+=16;
      ++dmm.ancount;
      dmm.flags = 0x0085;
      goto lbSend;
    }

    DDEBUG("")

    if((arpa=stristr(bfx,".IN-ADDR.ARPA")))
    {
     z=htonl(ConvertIP(t1=bfx));
     if(*t1=='.' )
     {if(i==1)
      {
       PrepareDefAReply(t);  
       DWORD_PTR(t[12]) = z ; // DWORD_PTR(bfx[270]);
       t+=16;
       ++dmm.ancount;
       dmm.flags = 0x0085;
       goto lbSend;
      }
     }
     else arpa=0;
    };


    DDEBUG("")

    if(! bfx[0] )
    {
      PrepareDefAReply(t);  
      DWORD_PTR(bfx[0])='.';
      th.hsh=xhst=ROOTSERVERHASH;
 //     debug("root NS request %u %X %X %s",th.typ,xhst,ROOTSERVERHASH,bfx);
    }

    DDEBUG("")

    fnd.Init();

    DDEBUG("")
    et=((char *)dmmc)+512;
    if( fnd.Find(host_rr,bfx,xhst,th.typ) )
    {
      PrepareDefAReply(t);  

      switch(th.typ)
      {
        case rtypeAXFR:
           if( s==stcp) // !=ssudp) //sdns)
           { ptmp=(char *) ALLOC(0x10100);
             memcpy(ptmp+2,(char *)dmmc,th.l);
             t=ptmp+2+(t-dmmc);
             pdm=(d_msg  *)(ptmp+2);
             et=ptmp+0x10000;
           }
        case rtypeSOA:
              if(!fnd.psoa) goto lbErrSend;
              hst=fnd.psoa;
              do
              {
               t=hst->AddRR(pdm,t,1);
               pdm->ancount++;
               if(i==rtypeSOA) goto lbSend;

               if(t>=et)
               { pdm->flags = 0x0087; //! 0x0087;
                 goto lbSend;
               }

              }while( (++hst)->type && hst->type!=rtypeSOA );
              t=fnd.psoa->AddRR(pdm,t,1);
              pdm->ancount++;
              break;

        default:
              t=fnd.AddFound(host_rr,pdm,t,et);
      }
      pdm->flags = 0x0085; //! 0x0085;
      goto lbSend;

    }
    else
    {
      if(fnd.psoa )
      {
        if(fnd.ns.n)
        {

          for(j=0; j<fnd.ns.n ; ++j )
          {
            while(fnd.ns.n && fnd.ns.hst[j]->isRecord(fnd.psoa->dt ) )
            {
              fnd.ns.Del(j);
            }
          }
        }
        if( !fnd.ns.n )
        {
         pdm->flags = 0x0385;
        //t=fnd.psoa->AddRR(pdm,t,1);
        //pdm->nscount++;
         goto lbErrSend2;
        }
        pdreq->state|=dnsstUSOA;
      }




      DDEBUG("Redirect")
      
      
lbRedirect:

      
     if( (!dns_cach_size) || (! (dmm.flags&0x1) ) || ( (s_flgs[2]&FL2_DNSNO6) && th.typ == 28) ||
         (isTCP) ||
         (
           ( //s!=ssudp //sdns
             //||
            // !IsInIPRange(dns_range,htonl(sa_client.sin_addr.S_ADDR))
             IsInIPRR(dns_range,(sockaddr_in *) &sa_client)<=0
           ) && ! (stt&2)
         ) ||
         (!strchr(bfx,'.'))
       )
     {
      // debug("req disable: %d %X %X %X",(!dns_cach_size) , (! (dmm.flags&0x1) ) , ( (s_flgs[2]&FL2_DNSNO6)),! (stt&2) );
  lbErrSend:
      pdm->flags = 0x0380;
  lbErrSend2:
     if(fnd.psoa)
     {t=fnd.psoa->AddRR(pdm,t,1);
      pdm->nscount++;
      pdm->flags &= 0x00FF;
     }
     for(j=0; j<fnd.ns.n  ; ++j )
     {
       t=fnd.ns.hst[j]->AddRR(pdm,t,1);
       if(t >= et)break;
       pdm->nscount++;
     }
     pdm->ancount=0;
     AddToLogDNS("(Name not found...)",th.typ,&sa_client,bfx);



  lbSend:

    pdreq->l=t-(char *)pdm;
//debug("send: %u %X %X",pdreq->l,t,(char *)pdm);
    pdreq->SendReply(pdm);
    lbEnd:
      if(s==stcp  // !=ssudp //sdns
         && ! pdm->ancount )
      {
  lbEnd2:
       //shutdown(s,2);
       CloseSocket(s);
       if(s==stcp)stcp=0;
       s=0;
      }


     continue;
    }

 //     debug("Reqursion");


       if(sudp2>0) s=sudp2 ;
       else if( bind_a[SDNS_IND] ) //|| sdns!=s )
       {
        if((sudp2=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1 )
        {
          debug("Could not get socket for resend ");
          return 0;
        }
      //  i=512;
      //  setsockopt(sudp2,SOL_SOCKET,SO_RCVBUF,(char *)&i,4 );
         (*(sockaddr_in *) (th.sa_cl+1)).sin_addr.s_addr=0;
        (*(sockaddr_in *) (th.sa_cl+1)).sin_port=0;
        (*(sockaddr_in *) (th.sa_cl+1)).sin_family=AF_INET;

        bind(sudp2,(struct sockaddr *) (th.sa_cl+1),sizeof(sockaddr_in) );
        s=sudp2;
#ifdef IP_TTL
     i=240;
     setsockopt(sudp2,IPPROTO_IP,IP_TTL,(char *)&i,4 );
#ifdef SYSUNIX      
     i=IPTOS_DNS;
     setsockopt(sudp2,IPPROTO_IP,IP_TOS,(char *)&i,4 );
#endif     
#else
#warning  IP_TTL not defined     
#endif     
       }





  lbFindCashe:
  //   debug("Find in cashe |%s| %u",bfx,pdreq->typ);
     if( fnd.Find(cash_rr,bfx,xhst,pdreq->typ) )
     {
       for(j=0; j<fnd.cname.n ; ++j )if(fnd.cname.hst[j]->type==rtypeCNAME)
          fnd.Find(cash_rr,fnd.cname.hst[j]->Data(),MkName(fnd.cname.hst[j]->Data()),pdreq->typ);
       if( fnd.rp.n )
       {
         if(!dmm.ancount)AddToLogDNS("(From cache)",th.typ,&sa_client,bfx);
         t=fnd.AddFound(cash_rr,pdm,t,et);
         dmm.flags =0x8081;
         goto lbSend;
       }

       if(fnd.cname.n)
       {
         pdreq->state|=dnsstWAIT_CNAME;
         pdreq->state&=~dnsstWAIT_REPLY;
         //if( !(pdreq=GetFreeReq(&th,MkName(t1)) ) )continue;
         t1=fnd.cname.hst[fnd.cname.n-1]->Data();
         if(pdreq==&th)if( !(pdreq=GetFreeReq(&th,hshx)) ) continue;
         pdreq->ttcnt++;
         pdreq->ReReq(s,t1,pdreq->typ);
       }
     }
  /*   
     else if(fnd.cname.n)
     {
         pdreq->state|=dnsstWAIT_CNAME;
         pdreq->state&=~dnsstWAIT_REPLY;
         //if( !(pdreq=GetFreeReq(&th,MkName(t1)) ) )continue;
         t1=fnd.cname.hst[fnd.cname.n-1]->Data();
         if(pdreq==&th)if( !(pdreq=GetFreeReq(&th,hshx)) ) continue;
         pdreq->ReReq(s,t1,pdreq->typ);
        
     } */
     else  
     {
       
         /*
        if(pdreq->typ != rtypeCNAME && pdreq->typ != rtypeA && fnd.Find(cash_rr,bfx,xhst,) )
        {
            
        }  
        */
        
  //    debug("Not found in cashe: %u %u %u",fnd.rp.n, fnd.cname.n,fnd.ns.n);
       pdreq->state|=dnsstWAIT_REPLY;
       if(pdreq==&th)if(!(pdreq=GetFreeReq(&th,th.hsh)))continue;
       /*
       if(fnd.cname.n)
       {
           
         fnd.ns.n=0;
//         if(! pdreq->FindNSResend(s,& fnd.ns,fnd.cname.hst[fnd.cname.n-1]->Data()) )goto lbErrSend;
         sprintf(bfx,"%.64s",fnd.cname.hst[fnd.cname.n-1]->Data());
       }
       else
           */
       {    
         if(s_flg&FL_DNSUPLVL && ! (pdreq->state&dnsstUSOA) )fnd.ns.n=0;
       }  
       if(! pdreq->FindNSResend(s,& fnd.ns,bfx) )goto lbErrSend;
       //pdreq->NextSend(s);
     }
    }

    DDEBUG("")

   }
   else
   { //Reply


    debug("DNS Reply %s %u %u %u %X %X", bfx,dmm.ancount,dmm.nscount,dmm.arcount,dmm.flags,th.us_id);
  //  MyLock(nsmut);
      if((pdreq=FindReqById(th.us_id)))
      {

       if(pdreq->state)
       {
   // debug("Add to cash time=%u",cur_time );
        j=cash_rr->AddRRfromReply(pdm,t, ( (char *)pdm+th.l),pdreq->typ );
        if(!dmm.ancount)pdreq->repled.AddIP(ADDR4(& sa_client ) );

//    debug("pdreq found %u %u %s",pdreq->state,j,bfx);

  //  debug("End add to cash time=%u",time(0) );

        if( (!(dmm.ancount))  &&  (!(dmm.flags&0x4 ) ) && (dmm.flags&0xF00)!=0x300
          )
        {
          if(dmm.nscount)
          {
           fnd.ns.n=0;
           pdreq->FindNSResend(s,& fnd.ns,bfx);
          }
          else
          {
            pdreq->alist.Del( ADDR4(&sa_client) );
            if(pdreq->alist.n)pdreq->NextSend( (sudp2>0)?sudp2:s );
            else
            {
             if( (dmm.flags&0xF00)==0x300 || dmm.flags&4 ) goto lbNS_notfound;
             goto lbEndReReq;
            }
          }

        }
        else
        {
    lbEndReReq:
           i=pdreq->state;
           //pdreq->state=0;
           if((i& (dnsstWAIT_REPLY|dnsstLINK) )==dnsstWAIT_REPLY )
           {
        //debug("Send reply 1");
             if( (!j) && /*(! (dmm.flags&0x4 )) && */ (dmm.flags&0xF00)!=0x300 )
             {
               memcpy(pdm,pdreq->dmmx,pdreq->l);
               t=((char*)pdm)+pdreq->l;
               fnd.Init();
               goto lbFindCashe;
             }
             pdreq->l=th.l;
             pdreq->SendReply(pdm);
           }
           else if( (i&(dnsstLNK_CNAME|dnsstLNK_NS))==dnsstLNK_CNAME )
           {
             // CNAME

             if(! pdreq->state&dnsstLINK ) // ! pdreq->lnk)
             {
               debug("Error link ; state=%X",pdreq->state);
               pdreq->FreeDNSReq();
             }
             else
             {
              while(pdreq->state&dnsstLINK)
              {
               pdreq->FreeDNSReq(); //state=0;
               pdreq= dreq+pdreq->lnk;
              }
              DecodeName(bfx,((d_msg *)(pdreq->dmmx))->buf,pdreq->dmmx );
              //memcpy(th,pdreq,sizeof(DNSReq));
              //pdreq->state=0;
              j=dmm.ancount;
              memcpy(pdm,pdreq->dmmx,pdreq->l);
              t=((char*)pdm)+pdreq->l;
              if(j)
              {
                fnd.Init();
                xhst=pdreq->hsh;
                goto lbFindCashe;
              }

             // if(pdreq->state)

              goto lbEndReReq;
             }
           }
           else if(i&dnsstLNK_NS)
           {
        //   debug("End NS re req %u %u ancount=%u",pdreq->lnk,pdreq-dreq,dmm.ancount);
            pdreq->FreeDNSReq(); // state=0;
            pdreq= dreq+pdreq->lnk;
            pdreq->state|=dnsstWAIT_REPLY;
            pdreq->state&=~dnsstWAIT_NS;
            if(dmm.ancount)
            {
             DecodeName(bfx,((d_msg *)(pdreq->dmmx))->buf,pdreq->dmmx );
          // debug("dmm.ancount =%u %s",dmm.ancount,bfx);
             fnd.ns.n=0;
             pdreq->FindNSResend(s,& fnd.ns,bfx);
            }
            else
            {

         lbNS_notfound:
              debug("Remove NS %s",bfx);
              RemoveNS(cash_rr,bfx);
              while(pdreq->state&dnsstLINK ) //pdreq->lnk)
              {
               pdreq->FreeDNSReq(); //state=0;
               pdreq= dreq+pdreq->lnk;
              }

             DecodeName(bfx,((d_msg *)(pdreq->dmmx))->buf,pdreq->dmmx );
          // debug("dmm.ancount =%u %s",dmm.ancount,bfx);
             fnd.ns.n=0;
             if(!(pdreq->FindNSResend(s,& fnd.ns,bfx)) )
                pdreq->SendErrReply(pdm,bfx);
            }
           }
           else
           {
             debug("Strange state %X ",i);

           }



        }

       }
      }
      else   {    debug("Id not found %X %s",th.us_id,bfx);      }

   }


  

  } // recvfrom
  else
  {

    if( isTCP)
    {
      CloseSocket(stcp);
      stcp=0;

    }
    else
    {
     Sleep(300);
    

     j=4;
     getsockopt(s,SOL_SOCKET,SO_ERROR,(char *) &i,(socklen_t*)&j);
     debug("Bad message received %u %u %u %d %d %d",s,soi,soik,i,th.l,fwrk);
     if(s==sudp2)
     {
      closesocket(sudp2);
      sudp2=0;
     }
     else  // if(! ( isTCP) )
     {
      ReinitDNSSocket(soi);
     }
    }


  }
  
  } // IS_SET
 } // for Select

   if( (s_flgs[2]&FL2_DOH) && FD_ISSET(doh_r, (fd_set *) &set)   )
   {
      s =soc_srv[SDNS_IND];
#ifdef   SYSUNIX    
      FD_CLR(doh_r,(fd_set *) &set); 
#else      
      for(int iji=0; iji<set.fd_count ; iji++ ) if(set.fd_array[iji]==doh_r){set.fd_array[iji]=0; break;  } ; 
#endif      
      _hread(doh_r,(char *) &th.doh_ptr,sizeof(th.doh_ptr) );
      th.l=th.doh_ptr->postsize;
      memcpy(dmmc,th.doh_ptr->pst,th.l);
      memcpy(&sa_client,& (th.doh_ptr->sa_c),sizeof(sa_client) );
     // i=sizeof(sa_client);
     // getpeername(th.doh_ptr->s,(sockaddr*) &sa_client,&i);
      
      goto lbDOH;
   }

 }// Select
 else
 {
   if( ! isTCP )ChkFlashRepeatLog();
 }

 if(!is_no_exit)return 0;
//lb_cnt_loop:
 if(cdreq>0 && (  (!bind_a[SDNS_IND]) || sudp2>0 )
 )
 {

  gettimeofday(&tvlast_time,0);
  if( Dtime(&tvlast_time,&old_time) >= 100000 )
  {


   MyLock(nsmut);
   ttl=DnsTms*1000;
   s=sdns;
   if(sudp2>0)s=sudp2;

   i=0;
   for(pdreq=dreq;pdreq<(dreq+MAX_DNS_REQ);pdreq++)if(pdreq->state)
   {
    if( (pdreq->state&dnsstWAIT_REPLY) && (j=Dtime(&tvlast_time, & pdreq->ptim )) >= (int)ttl )
    {
       if(pdreq->alist.n==1 && pdreq->ttcnt>2 )
       {
         fnd.ns.n=0;
         DecodeName(bfx,((d_msg *)(pdreq->dmmx))->buf,pdreq->dmmx );
         pdreq->FindNSResend(s,& fnd.ns,bfx,1);
         continue;
       }
       else if( pdreq->ttcnt > (pdreq->alist.n*2 + 3)  //&& (pdreq->ttcnt*DnsTms)>2000
           //|| j > 5000000
         )
       {

          debug("Deed by overtime %u %u",pdreq->ttcnt,pdreq->alist.n);
          pdreq2=pdreq;
          while(pdreq2->state&dnsstLINK ) //pdreq2->lnk)
          {
            pdreq2->FreeDNSReq(); //state=0;
            pdreq2= dreq+pdreq->lnk;
          }
         //DecodeName(bfx,((d_msg *)(pdreq2->dmmx))->buf, pdreq2->dmmx );
         //pdreq2->SendErrReply( &dmm,bfx );
          pdreq2->FreeDNSReq();
       }
       else
       {
          if(j<5000000)
            pdreq->NextSend(s);
          //else debug("j=%d",j);
       }
    }

    if(++i>=cdreq)break;
   }

   gettimeofday(&old_time,0);
  }
 }

 } //while

}


///////////////////////////////////////

inline int IsEndOfLine(char t)
{
 return 
#ifdef x86_64 
      NULL !=
#else
 (int)
#endif 
    strchr("\r\n;#",t);
}

int NSRecordArray::FillDomain(char *mem,char *dt,CheckPoint *cp)
{

  char *old_name="";
 // char *mem=0;
//  int  cntif=0;
//  int ni=0;
  char *t,*t1,*t2,*t3,*t4;
  ulong x,j,k,ttl,typ,mmi=0;
  Secondary **pseconds = &seconds;
  Secondary *sec ;
  NSRecord *prr=d;
  while(*pseconds)pseconds = & ((*pseconds)->next);

  /*
  if(arr)
  { mem=(char *) & (arr->d[arr->cnt]) ;   }
  */
  
  if(mem)smem=mem;
    
   for(t=dt;t && *t;)
   {

    prr->dt=mem+mmi;

    switch(*t)
    {
    case '$':
      if(striin(t+1,"ORIGIN ") )
      {
       old_name=GetWorld(t+=6);
       if(!t)
       {
        //cnt=d-prr;
        //return mmi;
        goto exLD;
       }
       k=*t!='\n';
       *t++=0;
       if(k)break;
      }
      else if( DWORD_PTR(t[1]) == 0x204C5454 x4CHAR("TTL ") )
      { if( (DNS_TTL= atoui(GetWorld(t+=4)))<3 )DNS_TTL=3;
        if(DNS_TTL>259200)DNS_TTL=259200;
        //DNS_TTL=htonl(DNS_TTL);
      }
      else if( striin(t+1,"IF_DOWN ") )
      {
        t3=GetWorld(t+=sizeof("IF_DOWN"));
        if(cp && *t>'\n')
        {
         cp->check=mem+mmi;
         *t++=0;
         if(cp->interval=atoui(GetWorld(t)))
         {
          if(cp->interval<3)cp->interval=3;
          t1=GetWorld(t);
          cp->oldIP=ConvertIP(t1);
          t=GetWorld(t1);
          if(*t!='=')goto lbEchk;
          t1=GetWorld(++t);
          cp->newIP=ConvertIP(t1);
          x=strlen(t3);
          if(mem)
          {
            strcpy(mem+mmi,t3);
          }
          mmi+=x+2;
          cp++;
         }
         else
         {
          lbEchk:
            debug("Error in IF_DOWN_CHANGE %.20s",t);
         }

       }


      }
      else if( striin(t+1,"SLAVE ") ) // SLAVE domain ip [file]
      {
        t3=GetWorld(t+=sizeof("SLAVE "));
        if( *t>'\n')
        {
         
          if(mem)
          {
            *t++=0;
            t1=GetWorld(t);
            *pseconds=sec=(Secondary *) (mem+mmi);
            pseconds=& (sec->next);
            sec->next=0;
            strncpy(sec->host,t3,sizeof(sec->host)-1);
            sec->sa.sin_addr.s_addr=ConvertIP(t1);
            sec->sa.sin_family=AF_INET;
            sec->sa.sin_port=(53<<8);  //htons((ushort)53);
            if(*t>'\n')
            {
              t1=GetWorld(++t);
              *t++=0;
              strncpy(sec->fname,t1,sizeof(sec->fname)-1);
            }
            
            // Remove the domain with the same hostname from the list (if exist)
            for(Secondary **tsecond=&seconds; *tsecond ; tsecond=& ((*tsecond)->next) ) 
                if(*tsecond!=sec && ! stricmp((*tsecond)->host,sec->host) )
                {
                  *tsecond=(*tsecond)->next;  
                  break;
                }

        
          }
          mmi+=sizeof(Secondary) ;
         }
         else
         {
            debug("Error in SLAVE %.20s",t);
         }

       }


      

    case ';':
    case '#': 
              if( IsNotMeta(t) )  
              {     
               if(!(t=strchr(t,'\n')))
               {
                 goto exLD;
                 //cnt=d-prr; return mmi;
               } //  break;
    case ' ':
    case '@':
    case '\n':
    case '\r':
    case '\t': ++t; break;
              }
    default:
      t1=GetWorld(t);
      if( IsEndOfLine(*t) && IsNotMeta(t) )break;

      *t++=0;
    //  debug("Before %.63s %s j=%X\r\n",t1,old_name,j);

      j=
#ifdef x86_64 
      NULL !=
#else
      (int)
#endif      
      strchr(t1,'.');
      if(t[-2]=='.' && t1<(t-3) )t[-1]=0;

      if( (!(k=anychars(t1)||WORD_PTR(*t1)=='.' )) && j )
      {
    //   debug("%.12s\r\n",t1);
       //(*Cur_hsts= (HSTRecord  *) ((t2=GetWorld(t))-6))->src=(char *)ConvertIP(t1);
       //(*Cur_hsts)->typ=typAA;

       t2=GetWorld(t);
       if(t)
       {if(t[-1]=='.' && t2<(t-2))t[-1]=0;
        *t++=0;
       }
       prr->Set(t2,MkName(t2),rtypeA,DNS_TTL,0);
       prr->ip=ConvertIP(t1);
       if(mem)
       {  strcpy(mem+mmi,t2);   }
       mmi+=prr->l+1;
       prr++;

    //   debug(">%s %s %X %X %X\r\n",t2,(*Cur_hsts)->name,t2,(*Cur_hsts)->name,(*Cur_hsts)->src );
      }else
      {
       if(!k)
       {
         if( (x=atoui(t1) ) )
         {
          ttl=x;
         }
         break;
       }
//       debug("Before %.63s %s j=%X\r\n",t1,old_name,j);
       if(j)
       { old_name=t1;
         if(t[-1]=='.')t[-1]=0;
         if(t[-2]=='.')t[-2]=0;
         if(!*t1)
         {
           old_name=ROOTSERVERNAME;
         //  debug("Root server %.20s",t);
         }
         break;
       }




       x=DWORD_PTR(*t1);

       if( (x|0xFF002020) == 0xFF006E69 ) //IN
       {
        //t1=GetWorld(t);
        //if( IsEndOfLine(*t) )break;
        //*t++=0;
        break;
       }


//       debug("After IN found %.63s %X %s %X\r\n",t1,x,old_name,j);

       //(*Cur_hsts= (HSTRecord  *) ((t2=GetWorld(t))-6))->src=old_name;

       t2=GetWorld(t);

       if(t)
       {
        j=*t;
        *t++=0;
       }


       if(mem)
       {
         strcpy(mem+mmi,old_name);
       }

       if( (x|0xFFFF0020)==0xFFFF0061 ) //A
       {
       // debug("typ A found |%.63s| |%.63s| %.63s\r\n",t1,t2,old_name);
         prr->Set(old_name,MkName(old_name),rtypeA,ttl?ttl:DNS_TTL,0);
         prr->ip=ConvertIP(t2);
       }
       else if( (x|0xFF002020)==0xFF00786D  ) //MX
       {
        if( anychars(t2) ) x=1;
        else
        {
         x=atoui(t2);
         t2=GetWorld(t);
         if(t)
         {
           if(t[-1] == '.')t[-1] =0;
           *t++=0;
         }
        }
        prr->Set(old_name,MkName(old_name),rtypeMX,ttl?ttl:DNS_TTL,2+strlen(t2)+1);

         if(mem)
         {
            DWORD_PTR(prr->dr[0])=htons(x);
            strcpy(prr->dr+2,t2);
         }

//        debug("typ MX found |%.63s| |%.63s| %.63s\r\n",t1,t2,old_name);
       }
       else if( (x|0x20202020)==0x61616161 ) //AAAA
       {
         prr->Set(old_name,MkName(old_name),rtypeAAAA,ttl?ttl:DNS_TTL, 16);
         if(mem)
         {
            IPv6Addr((ushort *)prr->dr,t2);
         }
       }
       else if( (x|0x202020)==0x616F73 x4CHAR("soa")  ) //SOA
       {


        t3=GetWorld(t);


        if( (!t) || ! (t4=strchr(t,')') ) )
        {
         debug("Error in hosts file in SOA %s",old_name);
         //cnt=d-prr;
         //return mmi;
         goto exLD;
        }

        while(*t!='(' ){*t++=0;}
        *t++=0;
        *t4=0;
        x=strlen(t2);
        prr->Set(old_name,MkName(old_name),rtypeSOA,ttl?ttl:DNS_TTL, 24+x+strlen(t3) );
        if(mem)
        {
          j=sprintf(prr->dr,"%s %s",t2,t3);
          prr->dr[x]=0;
          ++j;
          memcpy(prr->dr+j,DefaultSOA,20);

          for(x=0; x<5 ; ++x)
          {

            do{
              t2=GetWorld(t);
              if(*t2>='0')break;
              ++t;
            }while(t);

            DWORD_PTR(prr->dr[j])=htonl(atoui(t2) ) ;

        // debug("LdSoa %u) %X |%.15s|",x,DWORD_PTR(prr->dr[j]),t2);
            j+=4;
            while(t && (*t>'9' || *t<'0' ) )
            {
              if( (*t==';' || *t=='#') && IsNotMeta(t) )
               while(*t && *t!='\n')++t;
              ++t;
            }
            if( (t>=t4) || !*t)break;
          }
        }
        t=t4+1;
       }
       else if((x|0x202020)==0x00616163 x4CHAR("caa")  )           
       {
           typ=  rtypeCAA;
           t2=GetWorld(t);
            if( anychars(t2) ) x=0;
            else if(t)
            {
             x=atoui(t2);
             t2=GetWorld(t);
                          
            }
            if(!t)
            { 
           lbErrCAA:     
              debug("Error in hosts file in CAA %s",old_name);
              goto exLD;
            }
            j=t-t2;
            *t++=0;
            t3=SkipSpace(t);
            if(!(t=strchr_meta(t3,'\n'))) goto lbErrCAA;
            t4=t;   
            if(t[-1]=='\r')
            {
                t[-1]=0;
                t4--;
            }

            if(*t3=='\"')
            {  
                t3++;
                t4=strchr_meta(t,'\"');
                if(!t4)
                {
                  t4=t;
                  if(!t4[-1])t4--;
                }
            }    
            
            *t++=0;
            k=(t4-t3);
            if(k<=0)goto lbErrCAA;
           prr->Set(old_name,MkName(old_name),typ,ttl?ttl:DNS_TTL,2 + j + k );
           if(mem)
           {
             prr->dr[0]=x;
             prr->dr[1]=j;
             memcpy(prr->dr+2,t2,j);
             memcpy(prr->dr+2+j,t3,k);
           } 
       }
       else if((x|0x20202020)== 0x45505954 x4CHAR("TYPE")  )
       {
         typ= atoui(t1+4);
         if(typ>0 && typ<=0xFFFF && typ!= rtypeAXFR  && typ!=rtypeANY && WORD_PTR(*t2) == 0x235C x4CHAR("\#") )
         {
           t3=GetWorld(t);
           k=atoui(t3);
           prr->Set(old_name,MkName(old_name),typ,ttl?ttl:DNS_TTL,k);
              x='\n';
              for(j=0;j<k;j++)
              {
                
                if(*t=='\r')t++;
                if(*t==x)break;
                if(*t=='\n')t++;
                  
                t4=GetWorld(t);
                if(*t4=='\r')t4++;
                if(*t4==x)break;
                if(*t4=='\n')
                {
                  t++;
                }
                
                if(*t4 == '(')
                {
                  x=')'; 
                  t4=GetWorld(t);
                }
                if(*t4 == '\n')
                
                if(mem)
                {
                  prr->dr[j]=atouix(t4);
                }
           
              }
           

         
         }
         else 
           debug("DNS: Bad RR %u %.32s...\r\n",typ, t1);
       
       }
       else
       {
         if( (x|0xFF002020)==0xFF00736E  ) //NS
         {

           typ=rtypeNS;
	   if(mem && old_name==ROOTSERVERNAME && (s_flg&FL_DNSUPLVL) )
	   {
	      if(! (ProviderNS[0]) )ProviderNS[0]=prr;
              else if(! (ProviderNS[1]) )ProviderNS[1]=prr;
	   }
//        debug("typ NS found |%.63s| |%.63s| %.63s\r\n",t1,t2,old_name);
//         debug("typ NS found |%.63s| |%.63s| \r\n",(*Cur_hsts)->name,(*Cur_hsts)->src);
         }
         else if( (x|0x00202020)==0x727470 ) //PTR
         {
           typ=rtypePTR;
//         debug("typ PTR found |%.63s| |%.63s| \r\n",(*Cur_hsts)->name,(*Cur_hsts)->src);
         }
         else if( (x|0x202020)==0x747874   //! 0x22747874
	          x4CHAR("txt") || (x|0x202020)==0x00667073 x4CHAR("spf")   ) //TXT
         {
           typ=((x|0x202020)==0x00667073 x4CHAR("spf") )?rtypeSPF:rtypeTXT;
           if(j>'\n')
           {
             t[-1]=j;
             if((t=strchr_meta(t,'\n')))
             {*t++=0;
               if(t[-2]=='\r')t[-2]=0;
             }
           }

           t2--;
           //*t2=t-t2;
           *t2=strlen(t2+1);
           //debug("typ TXT found |%.63s| |%.63s| %u \r\n",t2,t2+1,*t2);
         }
         else if( (x|0x20202020)==0x6D616E63 x4CHAR("cnam")  ) //CNAME
         {
//        debug("typ CNAME found |%.63s| |%.63s| %.63s\r\n",t1,t2,old_name);
           typ=rtypeCNAME;
         }else  break;

         prr->Set(old_name,MkName(old_name),typ,ttl?ttl:DNS_TTL, strlen(t2) );
         if(mem)
         {
            strcpy(prr->dr,t2);
         }
        }

         mmi+=prr->l+1;
         ttl=0;
         prr++;
         break;

      }
    }
   }

exLD:
   if(cp) cp->check=0;
   cnt=prr-d;
   return  mmi;



}


#undef hshx
#undef sa_client
#undef cnt_ns
#undef nscnt
#undef stime

#undef dip
#undef bfr
#undef bfx
#undef dmm
#undef dmmc



void  SaveDNS()
{


}

int LoadDomain(char *file)
{

 register char *t;
 int s,i,k,j,l,kk,x;
 char *hostfile,*t1,*t2;
 CheckPoint *cp;

 NSRecordArray  *arr,*trr;

 FILETIME tft;

// DDEBUG("Init")

 if(file)
 {
// DDEBUG("I")
  if( (s=_lopen(file,0) ) <0 )
  {
   er1:
    MessageBox(0,"Unable to open hosts file",file,MB_OK|MB_ICONSTOP);
    return 0;
  }
   j=FileSize(s); //_llseek(s,0,2); _llseek(s,0,0);
   if(! (hostfile= (char *)DMALLOC(j*2+40) ) )goto er1;
   _hread(s,hostfile,j);
   _lclose(s);
   hostfile[j]=0;
   memcpy(hostfile+8+j,hostfile,j+1);

//   hostfile[j]=0;
   i=1;
   for(t=hostfile;*t;++t)if(*t=='\n') ++i;
// DDEBUG("I2")
  // Cur_hsts=xhsts=new HSTRecord *[i+10];
   k=0;
   t=hostfile;
   do{
     if(!(t=strstr(t,"$IF_") ) )break;
     ++k;
     ++t;
   }while(1);

   arr=(NSRecordArray *) DMALLOC(sizeof(NSRecordArray) +  (i+2)* sizeof(NSRecord)  +       sizeof(CheckPoint)*(k+3)  );

   if(k)
    DRList= (CheckPoint *) ( ((char *)arr) + sizeof(NSRecordArray) +  (i+2)* sizeof(NSRecord) );

   //if(k) cp=DRList = new CheckPoint[k+8];
   ProviderNS[0]=0;
   ProviderNS[1]=0;

   i=arr->FillDomain(0,hostfile+8+j,DRList);
   //printf("Load1 %u bytes need arr->cnt = %u\n",i,arr->cnt);

   t=(char*)DMALLOC(i+16);
   arr->FillDomain(t,hostfile,DRList);

   //if(ProviderNS[0])   Find

   //printf("Load2 %u bytes need arr->cnt = %u\n",i,arr->cnt);



 //  DDEBUG("IE")
   MyLock(nsmut);
   //Cur_hsts=hsts;
   trr=host_rr;
   host_rr=arr;

   if(trr)free(trr);
   if(hsfile) free(hsfile);
   hsfile=t;

   MyUnlock(nsmut);

   free(hostfile);
   count_dns=arr->cnt;

   if(ProviderNS[0])debug("Provder's DNS: %s %s", ProviderNS[0]->dr,ProviderNS[1]?ProviderNS[1]->dr:"");
   else debug("No providers servers...");
   
   
   Secondary *sec;
   i=0;
   for(sec=seconds ; sec ; sec=sec->next)
   { 
    // debug("DNS:%u) Secondary %s\r\n",++i,sec->host);
     sec->LoadFile();
   }

 
   return 1;
 }
// DDEBUG("I.")

 return 0;
}

void LoadDomainM()
{
 int i;
 if(count_dns)
 {
   if(LoadDomain(dns_file))
   {
     debug("DNS: Load host definetion file. %u hosts now are loaded",count_dns );
     
     if((DRList || seconds) && !ChkThread)
       CreateThread(&secat,0x8000,(TskSrv)CheckRemoteDown,(void *)0,0,(ulong *)&i);
   }
   else
   {
     debug(
       "Error loading host definetion file.");
   }
 }
}








////////////////////////////////////////////

#ifdef USE_IPV6



union sockaddr_in46{
  struct sockaddr_in6 sa_server6;
  struct sockaddr_in sa_server;
 } dns_bindaddr[10];

#endif


int ReinitDNSSocket(int k,int tcp)
{
  int s;
  int n;
  n=6+k*9;

  if(tcp)++n;
  s=soc_srv[n];
  if(s>0 && dns_bindaddr[k].sa_server.sin_port)
  {
    soc_srv[n]=0;
    CloseSocket(s);
    Sleep(100);
    if((s=
          (tcp)?    socket(dns_bindaddr[k].sa_server.sin_family,SOCK_STREAM,IPPROTO_TCP)
                  : socket(dns_bindaddr[k].sa_server.sin_family,SOCK_DGRAM,IPPROTO_UDP))<0)
    {
     debug("IP socket error %d " SER ,WSAGetLastError() Xstrerror(errno));
     return -1;
    }
    setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char *)&one,sizeof(one));
try_bind_again:   
    if(
     bind(s,(struct sockaddr *) & dns_bindaddr[k].sa_server,
#ifdef USE_IPV6
   (dns_bindaddr[k].sa_server.sin_family==AF_INET)?sizeof(sockaddr_in):sizeof(sockaddr_in6)
#else
   sizeof(sa_server)
#endif
     )
   )
   {
     debug("Error. Could not bind socket. (%d)" SER ,WSAGetLastError() Xstrerror(errno));

     if(ChkWaitBind())goto try_bind_again;
     
     closesocket( (int) s);
     return -2;
   }
#ifdef IP_TTL
    
    if(!tcp)
    {
     int i;      
     i=240;
     setsockopt(s,IPPROTO_IP,IP_TTL,(char *)&i,4 );
#ifdef SYSUNIX      
     
     i=IPTOS_DNS;
     setsockopt(s,IPPROTO_IP,IP_TOS,(char *)&i,4 );
#endif  
    }    
#endif    
   if(tcp)
   {
     listen(s,3);
   }

   soc_srv[n]=s;
  }

 return 1;
}


#if defined(USE_IPV6) && ! defined(SYSUNIX)

int InitDnsSrv2(int is_ip6);
int InitDnsSrv()
{
 if(!LoadDomain(dns_file))return 0;

 if( s_flgs[2]&(1<<SDNS_IND) && ! (bind_a[SDNS_IND]) )
 {
  MyLock(nsmut);
//  debug("Try to bind IPv6...");
  if(InitDnsSrv2(1)>0)
  {sdns6=sdns;
   sdnst6=sdnst;
 //  debug("Sucsess IPv6  wait mutex...");

   do{ Sleep(300); }while(nsmut);

 //  debug("Sucsess IPv6  ready...");
  }
  else MyUnlock(nsmut);
 }
 return InitDnsSrv2(0);
}
int InitDnsSrv2(int is_ip6)
{

#else
#define is_ip6 (s_flgs[2]&(1<<SDNS_IND))
int InitDnsSrv()
{
 if(!LoadDomain(dns_file))return 0;
#endif

 ROOTSERVERHASH=MkName(ROOTSERVERNAME);
// debug("ROOTSERVERHASH %u",ROOTSERVERHASH);

#ifdef USE_IPV6
union{
  struct sockaddr_in6 sa_server6;
  struct sockaddr_in sa_server;
 };
#else
 struct sockaddr_in sa_server;
#endif
 int i,sdn,sdnt=0,k=0,kk;
 char  *pbnd=bind_a[SDNS_IND];


 if( //s_flg&FL_DNSREDIR &&
    dns_cach_size )
 {
   if(dns_cach_size>0x1000)dns_cach_size=0x1000;
   if(dns_cach_size<0x100)dns_cach_size=0x100;

   cash_rr = (NSRecordArray *) DMALLOC( (i=dns_cach_size * sizeof(NSRecord) + sizeof(NSRecordArray))  + 1028*6 );
   cash_rr->hash_list_lo = (ushort *)  ( ((char *)cash_rr) +i ) ;
   cash_rr->hash_list_hi = cash_rr->hash_list_lo+0x400;
   cash_rr->hash_list_cnt = cash_rr->hash_list_hi+0x400;
   cash_rr->cnt=dns_cach_size;
   cash_rr->Rehash();
#ifdef SYSUNIX

   dnsheap=(Heap *) DMALLOC(dns_cach_size*128+sizeof(Heap) + 32);
   dnsheap->Init(dns_cach_size*32);

#else
//   wdnsheap=HeapCreate(0,0x1000,dns_cach_size*512);
//   wdnsheap=HeapCreate(0,0,dns_cach_size*512 + 1024*8 );
   wdnsheap=HeapCreate(0,0,0x1000*1024 );
   if(!wdnsheap)
   {
       debug("Heap allocate error. Using default heap");
       wdnsheap=GetProcessHeap();
   }    

#endif


/*
  pns_hash = (NS_hash *)  new uchar[sizeof(NS_hash)+dns_cach_size];
  pns_hash->hshbtime=time(0);
  if(dnscachefile && (i=_lopen(dnscachefile,0))>0)
  {
   _hread(i,(char *)pns_hash,4);
   if(pns_hash->sign==0x1290475)
   {
     _hread(i,((char *)pns_hash)+4,sizeof(NS_hash)+dns_cach_size-4);
   }
   _lclose(i);


//   debug("In cashe %u dns_basetime=%X tft.dwHighDateTime=%X\r\n",dns_incache,dns_basetime,tft.dwHighDateTime);

  }
  */

 }
 if(s_flgs[2]&FL2_DOH) 
 {
#ifdef SYSUNIX      
   pipe(doh_pipe);  
#else
  CreatePipe((HANDLE *)&doh_r,(HANDLE *)&doh_w,&secat,0x100*sizeof(void *));
#endif   
 }

 do{

   memset((char *) &sa_server,0,sizeof(
#ifdef USE_IPV6
   sa_server6
#else
   sa_server
#endif
    )) ;
  sa_server.sin_family=AF_INET;
  if(pbnd)
    pbnd=GetNextBindAddr(pbnd,&sa_server);


#ifdef USE_IPV6
  if( bind_a[SDNS_IND]? sa_server6.sin6_family==AF_INET6 :is_ip6 )// s_flgs[2]&(1<<6))
  {
   debug("Try to bind IPv6...  " );
      
   if((sdn=socket(AF_INET6,SOCK_DGRAM,IPPROTO_UDP))<0)
   {
    debug("IPv6 socket error %d " SER ,WSAGetLastError() Xstrerror(errno));
//#ifndef SYSUNIX
    goto lip4;
//#else
//    return -1;
//#endif

   }
   debug("+IPv6" );
   sa_server6.sin6_family=AF_INET6;
  }
  else
#endif
  {
  lip4:
   debug("Try to bind IPv4...  " ); 
   if( (sdn = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1 )
   {   dbg4("could not get socket"); return 0;}
  }
  setsockopt(sdn,SOL_SOCKET,SO_REUSEADDR,(char *)&one,sizeof(one));

//   i=512;
//   setsockopt(sdn,SOL_SOCKET,SO_RCVBUF,(char *)&i,4 );

/*
   i=512;
   setsockopt(sdns,SOL_SOCKET,SO_RCVBUF,(char *)&i,4 );
   i=512;
   setsockopt(sdns,SOL_SOCKET,SO_SNDBUF,(char *)&i,4 );
*/
     
   //sa_server.sin_addr.s_addr=0;  //htonl(INADDR_ANY);
   sa_server.sin_port=0x3500;  //htons(53);
try_bind_again:   
   if(
     bind(sdn,(struct sockaddr *) &sa_server,
#ifdef USE_IPV6
   (sa_server.sin_family==AF_INET)?sizeof(sa_server):sizeof(sa_server6)
#else
   sizeof(sa_server)
#endif
     )
   )
   {

   err:
     debug("Error. Could not bind socket. (%d)" SER ,WSAGetLastError() Xstrerror(errno));

     if(ChkWaitBind())goto try_bind_again;

     //shutdown( sdn, 2 );
     //closesocket( (int) sdn);
      CloseSocket(sdn);
     //shutdown( sdnt, 2 );
     //closesocket( (int) sdnt);
     soc_srv[6+k*9]=0;
     if(pbnd) continue; 
     if(k)break;
     if(sdnt) CloseSocket(sdnt);


     count_dns=0;
     return 0;
      
   }

#ifdef IP_TTL

     i=240;
     setsockopt(sdn,IPPROTO_IP,IP_TTL,(char *)&i,4 );
#ifdef SYSUNIX      
     
     i=IPTOS_DNS;
     setsockopt(sdn,IPPROTO_IP,IP_TOS,(char *)&i,4 );
#endif     
#endif     

  soc_srv[6+k*9]=sdn;
   memcpy(dns_bindaddr+k,&sa_server,sizeof(sa_server6));


  kk++;
  

//  CreateThread(&secat,0x8000,(TskSrv)SetDNSServ,(void *)(k*9),0,(ulong *)&i);
// DDEBUG("-")
  if(s_flg&FL_DNSTCP)
  {
#ifdef USE_IPV6
   if(bind_a[SDNS_IND]? sa_server6.sin6_family==AF_INET6 : is_ip6) //s_flgs[2]&(1<<6))
   {if((sdnt=socket(AF_INET6,SOCK_STREAM,IPPROTO_TCP //0
                   ))<0)
    {
     debug("IPv6 socket error %d " SER ,WSAGetLastError() Xstrerror(errno));
//#ifndef SYSUNIX
     goto lip4a;
//#else
//     return -1;
//#endif
    }
   }
   else
#endif
   {
   lip4a:
    sdnt = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
   }
   setsockopt(sdnt,SOL_SOCKET,SO_REUSEADDR,(char *)&one,sizeof(one));
try_bind_again2:   
   if(bind(sdnt,(struct sockaddr *) &sa_server,
#ifdef USE_IPV6
   (sa_server.sin_family==AF_INET)?sizeof(sa_server):sizeof(sa_server6)
#else
   sizeof(sa_server)
#endif
   ))
   {    
       if(ChkWaitBind())goto try_bind_again2;
       goto err;
   }    
   listen(sdnt,3);

   soc_srv[7+k*9]=sdnt;

#ifdef _BSD_VA_LIST_
  dnstthr=
#endif
   CreateThread(&secat,0x8000,(TskSrv)SetDNSServ,(void *)(k*9+1),0,(ulong *)&i);
  }
//  DDEBUG("3")
lb_cnt_adaptor:;
 }while(pbnd && ++k < MAX_ADAPT);

 if(kk)
 {
  CreateThread(&secat,0x8000,(TskSrv)SetDNSServ,(void *)(0),0,(ulong *)&i);
  if(DRList)
   CreateThread(&secat,0x8000,(TskSrv)CheckRemoteDown,(void *)0,0,(ulong *)&i);
 }
  return 1;
//   s_flg|=FL_DNS;

}


int Secondary::CheckSecondary()
{
 char *p,*ep;
 int i;
#ifdef USE_IPV6
    sockaddr_in6
#else
   sockaddr_in
#endif
     lsa;
 union{
  uchar bb[512+16];
  d_msg dbb;
 };
 char bfr[68];
 
   if(next_chk>cur_time)return 0; 
   if( //(!next_chk) || 
       (!parr)  || !lastserial )return 1; 
   if(sock_secondary<=0)
   {
     sock_secondary=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)  ;
     if(sock_secondary<=0)return 0;
     SOAreq.flags=1;
     SOAreq.qdcount=1;

   } 

   next_chk=cur_time+30;
   SOAreq.id++;
   strcpy(SOAreq.buf+1,host);
   p=ConvertDomain(SOAreq.buf, &SOAreq);
   DWORD_PTR(*p)=0x01000600;  //type A IN
   if( (sendto(sock_secondary,(char *)&SOAreq,p+4-(char *)&SOAreq,0,(sockaddr *)&sa,sizeof(sockaddr) )) <=0)
   {
    debug("DNS: Error check primary DNS server for %s (cant send request) %d %s",host,errno,strerror(errno));
    closesocket(sock_secondary);
    sock_secondary=0;
    return 0;
   }
   do{
    dbb.id=0;   
    if( RESelect(5,300000,1,sock_secondary)<=0  || (i=recvfrom(sock_secondary,(char *)bb,512,0,(sockaddr *)&lsa ,(socklen_t *) (void *) &(i=sizeof(sockaddr)) ))<=0 )
    {
      debug("DNS: Error check primary DNS server for %s (cant recv reply) %d %s",host,errno,strerror(errno));
      closesocket(sock_secondary);
      sock_secondary=0;
      return 0;
    }
   }while(SOAreq.id != dbb.id ); 
   bb[i]=0;
   
   next_chk=cur_time+refresh;
   
   
   p=DecodeName(bfr,dbb.buf,(char *)bb );
   ep=(char *)bb+i;
   p+=4;
   for(i=0;i<dbb.ancount && p<ep ;i++){
     p=DecodeName(bfr,p,(char *)bb );
     if(p>ep)break;
     
     if(p[1]==6)
     {    
      p=DecodeName(bfr,p+10,(char *)bb );
      p=DecodeName(bfr,p,(char *)bb );
    

      debug("DNS: Check primary DNS server for %s Ok serial:%u",host,htonl(DWORD_PTR(*p)) );
   
      if(lastserial!=DWORD_PTR(*p) ) return 1;
      return 0;
     } 
     p+=10+p[9] ;  //((rrpart *)p)
   }

   debug("DNS: Error check primary DNS server for %s (bad reply)",host );
      
   return 0;
};


void Secondary::LoadSecondary()
{
 char *p,*ep,*m=0,*t;
 int i,s,j;
 NSRecordArray  *rr,*rr1;
 union{
      char *bb;
      d_msg *dbb;
 };
 
 char bfr[68];
 

   next_chk=cur_time+60;

#ifndef SYSUNIX
   debug("DNS: try to load domain %s from  %u.%u.%u.%u:%u",host,
     sa.sin_addr.S_un.S_un_b.s_b1,  sa.sin_addr.S_un.S_un_b.s_b2,
     sa.sin_addr.S_un.S_un_b.s_b3,  sa.sin_addr.S_un.S_un_b.s_b4
       , htons( (ushort)sa.sin_port )
   );
#else
   debug("DNS: try to load domain %s from  %u.%u.%u.%u:%u",host,
     sa.sin_addr.s_addr&0xFF, BYTE_PTR(sa.sin_addr.s_addr,1),
     BYTE_PTR(sa.sin_addr.s_addr,2), BYTE_PTR(sa.sin_addr.s_addr,3)
       , htons( (ushort)sa.sin_port )
   );
#endif
 
   if(
       (s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0){
     //    strcpy(prot,"call to socket failed");
         debug("DNS: Cant get socket %d %s",errno,strerror(errno));
         return ;
   };
   if(connect(s,(struct sockaddr *)&sa,sizeof(sa)) < 0) {
         debug("DNS: Cant connect %d %s",errno,strerror(errno));
        // shutdown(s, 2);
         closesocket(s);
         return ;
   }
   m=new char[0x10100];
   if(!m)
   {
         debug("No enought memory %d %s",errno,strerror(errno));
         return ;
   }    
   bb=m+2;
   memcpy(bb,&SOAreq,16);
   strcpy(dbb->buf+1,host);
   p=ConvertDomain(dbb->buf,dbb);
   DWORD_PTR(*p)=0x0100FC00;  //type AXFR IN
   i=p+4-bb;
   m[1]=i;
   if( (send(s,m,i+2,0) ) <=0)
   {
    debug("DNS: Error load zone for %s (cant send request) %d %s",host,errno,strerror(errno));
    goto  err_exit;
   }
   
   
   p=m;
   j=0;
   ep=p+0x10000;
   do{
    if( (i=XRecv(s,p,ep-p,0,PRXTimeout)) <= 0 )
    {
      debug("DNS: Error load zone for %s (cant recv reply) %d %s",host,errno,strerror(errno));
      goto err_exit;
    }
    p+=i;
    j+=i;
   }while( p<ep && j<htons(WORD_PTR(*m) ) );
   
   *p=0;
   if(dbb->ancount <= 2)
   {
   err_reply2:    
      debug("DNS: Error load zone for %s (bad reply ancount:%u)",host,dbb->ancount);
      goto err_exit;
   }    
   
   p=DecodeName(bfr,dbb->buf,(char *)bb );
   p+=4;
   t=DecodeName(bfr,p,(char *)bb );
   if(t[1]!=6)//SOA
       goto err_reply2;
   
   
   t=DecodeName(bfr,p+10,(char *)bb );
   t=DecodeName(bfr,t,(char *)bb );
   lastserial=DWORD_PTR(t[0]);
   refresh=htonl(DWORD_PTR(t[4]));
   if(refresh<60)refresh=60;
   next_chk=cur_time+refresh;

   debug("DNS: Load primary DNS server for %s Ok.  %u records. Serial:%u. Refresh set to %u\r\n",host,dbb->ancount,lastserial, refresh);
   
   
   rr = (NSRecordArray *) DMALLOC( i=(dbb->ancount + 3) *  sizeof(NSRecord) + sizeof(NSRecordArray)  //+ 1028*6 
        );
   memset(rr,0,i);
   /*
   rr->hash_list_lo = (ushort *)  ( ((char *)rr) +i ) ;
   rr->hash_list_hi = cash_rr->hash_list_lo+0x400;
   rr->hash_list_cnt = cash_rr->hash_list_hi+dbb->ancount;
   rr->cnt=dbb->ancount;
   rr->Rehash();
   */
   
   rr->cnt=dbb->ancount+1;

   rr->AddRRfromReply(dbb,p,ep ,0 );

   rr->next=0;
   if(!parr)
   {
     for(rr1=host_rr; rr1->next ; rr1 = rr1->next) ;   
   }    
   else 
   {
     for(rr1=host_rr; rr1->next  ; rr1 = rr1->next)
     {
       if( rr1->next == parr)
       {
         rr->next = parr->next;
         rr1->next = rr;
         Sleep(100); // be sure that nobody dont use this more
         if(parr->smem)free(parr->smem);
         free(parr);
         parr=rr;
         break;  
       }    
     }      
   }
   rr1->next = rr;
   if(fname[0])rr->Save(fname);

   


   
 err_exit:   
    delete m;
    shutdown(s,2);
    closesocket(s);
};

 void  NSRecordArray::Save(char *fname)
 {
   int i;
   int h;
   char *old=0;
   char *m,*t,*t3;
   ulong x,y;
   ushort *pxar;
//   if(! (f=fopen(fname,"w")) )return ;
   if( (h=_lcreat(fname,0)) <0 )return ;
   m= (char *) malloc(0x8000);

  if(m)
  {
   
   t=m;
   *t++='\n';
   for(i=0; i<cnt ; ++i )if(d[i].type)
   {
     if((x=t-m)>0x7000)
     {
      _hwrite(h,m,x);
      t=m;  
     }
     if( (!old) || stricmp(old,d[i].dt)   )
     {
       t+=sprintf(t,"\n %s %u IN ",d[i].dt,d[i].ttl);
       old=d[i].dt;
            
     }
     else
      t+=sprintf(t,"\n   %u IN ",d[i].ttl);
     switch(d[i].type)
     {
   case 1: //A
        x=d[i].ip;
        t+=sprintf(t,"A %u.%u.%u.%u",BYTE_PTR(x,0),BYTE_PTR(x,1),BYTE_PTR(x,2),BYTE_PTR(x,3));
        break;
   case 28: //AAAA
        pxar=(ushort *) (d[i].dr);
        t+=sprintf(t,"AAAA %X:%X:%X:%X:%X:%X:%X:%X",
             htons(pxar[0]),htons(pxar[1]),htons(pxar[2]),htons(pxar[3]),htons(pxar[4]),
             htons(pxar[5]),htons(pxar[6]),htons(pxar[7]) );
        break;
   case 3:                    //MD
   case 8:                    //MG
   case 9: t3="";      if(0){ //MR
   case 2: t3="NS";    if(0){ //NS
   case 5: t3="CNAME"; if(0){ //CNAME
   case 12:t3="PTR";   if(0){ //PTR
   case 16: t3="TXT";   if(0){ 
   case 99:t3="SPF";          
   }}}}}
        t+=sprintf(t,"%s %.255s",t3,d[i].dr);
        break;
   case 15:     //MX
        t+=sprintf(t,"MX %u %.63s",htons(WORD_PTR(d[i].dr[0]  )),d[i].dr+2);
        break;
   case 257:     //CAA
        t3=d[i].dr;
        x=t3[1];
        t+=sprintf(t,"CAA %u ",*t3);
        strncpy(t,t3+2,x);
        t+=x;
        t+=sprintf(t," %.63s",t3+2+x);
        break;
//   case 13: //HINFO
//        t+=sprintf(t,"HINFO %.63s %.63s\n",t2+12,t2+13+strlen(t2+12));
//        break;
   case 6: //SOA
        t+=x=sprintf(t,"SOA %.63s",d[i].dr);
        t+=y=sprintf(t," %.63s (",d[i].dr+x-3);
        t3=d[0].dr+x+y-3-2;
        t+=sprintf(t," %u ; SERIAL\n          %u ; REFRESH\n          %u ;  RETRY\n          %u ;  EXPIRE\n          %u ;  MINIMUM\n             )"
         ,htonl(DWORD_PTR(t3[0]))
         ,htonl(DWORD_PTR(t3[4]))
         ,htonl(DWORD_PTR(t3[8]))
         ,htonl(DWORD_PTR(t3[12]))
         ,htonl(DWORD_PTR(t3[16]))
        );
        break;
    default:
          t+=sprintf(t," TYPE%u  \\# %u ",d[i].type,y=d[i].l2);
          t3=d[i].dr;
          for(x=0;x<y;x++) t+=sprintf(t," %x",((uchar *)t3)[x]  );
       
     }
   }
   
  *t++='\n';
   _hwrite(h,m,t-m);
   free(m);
  }
  _lclose(h);
 };

void Secondary::LoadFile()
{

 register char *t;
 int s,i,k,j,l,kk,x;
 char *hostfile,*t1,*t2;

 NSRecordArray  *arr,*trr;

 FILETIME tft;


 if(fname)
 {
 // debug("DNS: Try to load %s\r\n",fname);
  if( (s=_lopen(fname,0) ) <0 )
  {
   er1:
    return ;
  }
   j=FileSize(s); //_llseek(s,0,2); _llseek(s,0,0);
   
   if(! (hostfile= (char *)DMALLOC(j*2+40) ) )goto er1;
   _hread(s,hostfile,j);
   _lclose(s);
   hostfile[j]=0;
   memcpy(hostfile+8+j,hostfile,j+1);

//   hostfile[j]=0;
   i=1;
   for(t=hostfile;*t;++t)if(*t=='\n') ++i;
  // Cur_hsts=xhsts=new HSTRecord *[i+10];
   arr=(NSRecordArray *) DMALLOC(sizeof(NSRecordArray) +  (i+3)* sizeof(NSRecord)   );
   if(!arr) 
   {
     debug("DNS: No enought memory to load %s\r\n",fname);
     return ;
   }
   i=arr->FillDomain(0,hostfile+8+j,0);
   //printf("Load1 %u bytes need arr->cnt = %u\n",i,arr->cnt);
   t=(char*)DMALLOC(i+16);
   arr->FillDomain(t,hostfile,0);

   //if(ProviderNS[0])   Find

   //printf("Load2 %u bytes need arr->cnt = %u\n",i,arr->cnt);
 
   free(hostfile);
   if( arr->d[0].type == 6 ) //SOA
   {
    // t2=GetWorld(t3=);
     t1= arr->d[0].dr ;
     t1+=strlen(t1)+1;
     t1+=strlen(t1)+1;
     lastserial= DWORD_PTR(t1[0]);
     refresh =  htonl(DWORD_PTR(t1[4])) ;
     if(refresh<20)refresh=20;
     if(refresh>86400)refresh=86400;
     parr = arr;
     debug("DNS: domain %s loaded from file %s refresh set to %u\r\n",host,fname,refresh);
   } 
   else
   {
     if(arr->smem)free(arr->smem);
     free(arr);
     debug("DNS: Bad domain file %s\r\n",fname);
   }
 //  debug("...\r\n");
  }
 
};

#undef MkName

#undef pprot 
#undef f_prot
#undef pcnt  
#undef b_prot

