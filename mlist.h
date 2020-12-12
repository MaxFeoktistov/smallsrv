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


#ifdef SYSUNIX
#define MALLOC Malloc
#define REALLOC realloc
#else
extern "C" void *id_heap;
#define MALLOC(n) HeapAlloc(id_heap,HEAP_ZERO_MEMORY,n)
#define REALLOC(p,n)  HeapReAlloc(id_heap,HEAP_ZERO_MEMORY,p,n)

#endif

struct LimitCntr
{ ulong ip,first,cnt;
  int CheckLimit(ulong t1,ulong lst_tim);
};

// Template version workung too slow and required more memory, -- bad code has been generated.
// So, each  'template' has been replaced to 'define' ...
// And I don't recomend to use template in any case...

//template<class X,int ITEMS=1024>  struct MList
#define X LimitCntr
#define ITEMS 1024
struct MListCntr
{
//public:
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

//};
//struct MListCntr: public MList<LimitCntr,64>
//{
//
  X *Find(ulong x);
  void FreeOld(ulong told);
#undef X
#undef ITEMS
};


extern MListCntr ipcnts[10];
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

