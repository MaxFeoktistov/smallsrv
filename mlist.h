/*
 * Copyright (C) 1999-2022 Maksim Feoktistov.
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


#ifdef SYSUNIX
#define MALLOC Malloc
#define REALLOC realloc
#else
extern "C" void *id_heap;
#define MALLOC(n) HeapAlloc(id_heap,HEAP_ZERO_MEMORY,n)
#define REALLOC(p,n)  HeapReAlloc(id_heap,HEAP_ZERO_MEMORY,p,n)

#endif

struct LimitBase
{
  ulong first,cnt;  
  int CheckLimit(ulong t1,ulong lst_tim);
    
};

struct LimitCntr : public LimitBase
{ 
    
  ulong  ip;
};

struct IPv6c
{
    
 union{
   in6_addr ip;
   unsigned long long s6_addr64[2];
 };
// */
 
 int operator==(in6_addr &sip){return !memcmp(&ip,&sip,16); }  
 void Set(unsigned long long *addr64)
 {
    s6_addr64[0] =  addr64[0];
    s6_addr64[1] =  addr64[1];
 }
 void Set(in6_addr &sa){Set((unsigned long long *) sa.s6_addr32 );}
 void Set(in6_addr *sa){Set((unsigned long long *) sa->s6_addr32 );}
};
struct LimitCntrIPv6: public LimitBase
{ 
  IPv6c ip;
  
  void Set(in6_addr &sa){ip.Set((u_long long *) sa.s6_addr32 );}
  void Set(in6_addr *sa){ip.Set((u_long long *) sa->s6_addr32 );}
};

// Template version workung too slow and required more memory, -- bad code has been generated.
// So, each  'template' has been replaced to 'define' ...
// And I don't recomend to use template in any case...


#ifdef WITHOUTTEMPL

#define X LimitCntr
#define ITEMS 1024
struct MListCntr

#else

// Upd. In modern version of gcc template without virtual member functions working better, just like defines.  
// Ok. Now I try to use it carefull... 

extern "C++" template<class X,int ITEMS=1024>  struct MList
#endif
{
public:
  ulong l,n;
  X *d;
  X *Push()
  {X *r;
   if(n>=l)
   {l+=ITEMS;
    d=(X *)
      ((d)?REALLOC(d,sizeof(X)*l): MALLOC(sizeof(X)*l));
   }
   memset(r=d+n++,0,sizeof(X));
   return r;
  };
  void Pop(){--n;}
//  void FreeAll(){X *r,*e; for(e=(r=d)+n;r<e;++r)r->Free(); n=0;}
  void Del(X *x) {--n; if(n)memcpy(x,d+n,sizeof(X)); }

#ifndef WITHOUTTEMPL
  template <class Y> X *FindT(Y x)  {X *r,*e;  for(e=(r=d)+n;r<e;++r)if(r->ip==x)return r;  return 0;}  ;
#endif  
  void FreeOldT(ulong told){ X *r,*e;  for(e=(r=d)+n;r<e;)if(r->first<told){Del(r); --e;} else ++r;} ;
  
};

/*
#ifndef WITHOUTTEMPL
template<class X,int ITEMS=1024>   X * MList<X,ITEMS>::Find(ulong x)  
  {X *r,*e;  for(e=(r=d)+n;r<e;++r)if(r->ip==x)return r;  return 0;}
template<class X,int ITEMS=1024>   void MList<X,ITEMS>::FreeOld(ulong told) 
  { X *r,*e;  for(e=(r=d)+n;r<e;)if(r->first<told){Del(r); --e;} else ++r;};
#endif
*/


#ifdef WITHOUTTEMPL
  
#undef X
#undef ITEMS


extern MListCntr ipcnts[10];
#else
struct MListCntr: public MList<LimitCntr,1024> 
{
  LimitCntr *Find(ulong x);
  void FreeOld(ulong told); 
};

struct MListCntrIPv6: public MList<LimitCntrIPv6,1024> 
{
  LimitCntrIPv6 *Find(in6_addr &x);
  void FreeOld(ulong told); 
};

extern MListCntr ipcnts[10];
extern MListCntrIPv6 ipv6cnts[10];

#define hack6 ipv6cnts[5]
#define dns6DoS ipv6cnts[8]

#endif

#define hack ipcnts[5]
#define dnsDoS ipcnts[8]
extern ulong ltime[10],ip_limit[10],limit[10],net_limit[10];

#define smtp_ip_limit ip_limit[0]
#define smtp_net_limit net_limit[0]
#define smtp_ltime ltime[0]
#define smtp_limit limit[0]


#define prxy_ip_limit  ip_limit[2]
#define prxy_net_limit net_limit[2]
#define prxy_ltime     ltime[2]
#define prxy_limit     limit[2]

#define ftpi_ip_limit  ip_limit[3]
#define ftpi_net_limit net_limit[3]
#define ftpi_ltime     ltime[3]
#define ftpi_limit     limit[3]

#define ftpo_ip_limit  ip_limit[4]
#define ftpo_net_limit net_limit[4]
#define ftpo_ltime     ltime[4]
#define ftpo_limit     limit[4]


#define http_ip_limit  ip_limit[7]
#define http_net_limit net_limit[7]
#define http_ltime     ltime[7]
#define http_limit     limit[7]

LimitBase * AddToList(int i,sockaddr_in *sa,int c=0);



