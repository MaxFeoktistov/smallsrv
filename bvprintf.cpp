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

//gcc -c -O2 -IE:\DJGPP\INCLUDE\WIN32 winpr.cpp



#include "mstring1.h"
#include "srv.h"
#include "mdef.h"

#include "bvprintf.h"
#ifndef PF_LONG_LONG
#define PF_LONG_LONG 1
#endif

extern "C"
{


#define CRREPL 0

/*
uint  atoui(char *a)
{
 register uint r=0,l;
 while( (l=*a-'0')<10 ){r=r*10+l;a++;};
 return r;
};
uint  atouix(char *a)
{
 register uint r=0,l;
 while( (l=*a-  ( (*a>='a')?'a'-10:(*a>='A')?'A'-10:'0' )
        )<16 ){r=(r<<4)+l;a++;};
 return r;
};
uint atouis(const char *&a)
{
 register uint r=0,l;
 while( (l=*a-'0')<10 ){r=r*10+l;a++;};
 return r;
};
#define atoi atoui
*/

char * uitoa(char *t,ulong a,ulong d=1000000000,ulong o=10,char b='a')
{
 ulong x;
 char *r=0;
 while(d)
 {
  x=a/d;
  a=a%d;
  if(x && !r)r=t;
  if( (x+='0')>'9' )x+=b-'0'-10;
  *t++=x;
  d/=o;
 };
 *t=0;
 if(!r)return t-1;
 return r;
};
#ifdef  PF_LONG_LONG
/*
  (a*2^32 + aa)/b
  a/b = (a/n*n)/b=(a/n) / (b/n)
 */

u64 my_mulldiv(u64 a, u64 b, u64 *rem)
{
  u64 q = 0;
  u64 r = 0;
  int i;
  /*
  union {
    uint  v32[2];
    u64   v64;
  };
  v64 = a;
#ifndef  NOTINTEL
  if(v32[1] < b)
  {

  }
#endif
*/
  for(i=63; i>=0; i--)
  {
    r <<= 1;
    r |= (a>>i)&1;
    if(r >= b) {
      r -= b;
      q |= (1ull << i);
    }
  };
  if(rem) *rem = r;
  return q;

}

static char * my_ulltoa(char *t,unsigned long long a,unsigned long long d=1000000000000000000ll,ulong o=10,char b='a')
{
  uint x;
  char *r=0;
  while(d)
  {

    //x=a/d;
    //a=a%d;
    x = my_mulldiv(a, d, &a);

    if(x && !r)r=t;
    if( (x+='0')>'9' )x+=b-'0'-10;
    *t++=x;
    //d/=o;
    d = my_mulldiv(d, o, 0);
  };
  *t=0;
  if(!r)return t-1;
  return r;
};

#endif

#ifndef BPRINTF_INLINE

int BFILE::bprintf(const char *fmt,...)
{
#ifdef USEVALIST
   va_list a;
   int r;

   va_start(a, fmt);
   r=bvprintf(fmt,a);
   va_end(a);
   return r;
#else
   return bvprintf(fmt,(void *) ((&fmt)+1)) ;
#endif

} ;
#endif


inline void * mvarg(void **v, int size)
{
  void *r = *v;
  * (char **)v += size;
//  DWORD_PTR(*v) += size;
  return r;
}

int BFILE::bvprintf(const char *fmt, void *v)
{
#undef mva_arg
#define mva_arg(a, t) (*(t *)a ++)
//#define mva_arg(a,t) (*(t*) mvarg((void **) &a, (sizeof(t)+3)&~3 ) )

 char *t1,*p,*pp;
#ifdef  PF_LONG_LONG
  union{
   long long lx;
   int x;
  };
 int i,ii,k,bl,bx;
#else
 int i,ii,x,k,bl,bx;

#endif
 int state;
 char bb[64];
// long  double dbl;
 int w,l;
// t=bfr;
 i=0;
 ii=0;

 if(fmt) while(*fmt)
 {
  if(*fmt=='%')
  {
   state=0; w=0; l=0;
  lb0:
    switch(*++fmt)
    {
     case  0:  break;
     case '+':  state|=2;    goto lb0;
     case '-':  state|=1;    state&=~0x40; goto lb0;
     case '#':  state|=4;    goto lb0;
     case 'l':
#if __SIZEOF_LONG__ != 8
                state|=
#ifdef  PF_LONG_LONG
         (state&8)?0x10:
#endif
         8;
         goto lb0;
#endif
     case 'L':  state|=0x10; goto lb0;
     case 'h':  state|=0x20; goto lb0;
     case '0':  state|=0x40; state&=~2; goto lb0;
     case ' ':  state|=0x80; goto lb0;
     case '1':
     case '2':
     case '3':
     case '4':
     case '5':
     case '6':
     case '7':
     case '8':
     case '9':  l=atouisc( fmt); fmt--; goto lb0;
     case '.':  w=atouisc( ++fmt); state|=0x200;  fmt--; goto lb0;
#ifdef  PF_LONG_LONG
     case 'u':  p=(state&0x10)?my_ulltoa(bb,mva_arg(v, u64) ):uitoa(bb,mva_arg(v,uint) ); if(0){
     case 'i':
     case 'd':  if(state&0x10)
                {
                    if((lx=mva_arg(v, u64))<0){lx=-lx; state|=0x100; } p=my_ulltoa(bb,lx);
                }
                else
                {
                    if((x=mva_arg(v,int))<0){x=-x; state|=0x100; } p=uitoa(bb,x);
                }
                if(0){
     case 'o':  p=(state&0x10)?my_ulltoa(bb,mva_arg(v,u64),01000000000000000000000ll,8 ):uitoa(bb,mva_arg(v,uint),010000000000,8   ); if(0){
     case 'X':  p=(state&0x10)?my_ulltoa(bb,mva_arg(v,u64),0x1000000000000000ll,16,'A' ):uitoa(bb,mva_arg(v,uint),0x10000000,16,'A'); if(0){
     case 'x':  p=(state&0x10)?my_ulltoa(bb,mva_arg(v,u64),0x1000000000000000ll,16,'a' ):uitoa(bb,mva_arg(v,uint),0x10000000,16,'a');
                }}}};
#else
     case 'u':  p=uitoa(bb, (*(uint *)v++) ); if(0){
     case 'i':
     case 'd':  if((x=(* (uint *) v++))<0){x=-x; state|=0x100; } p=uitoa(bb,x); if(0){
     case 'o':  p=uitoa(bb,(*(uint *)v++),010000000000,8); if(0){
     case 'X':  p=uitoa(bb,(*(uint *)v++),0x10000000,16,'A'); if(0){
     case 'x':  p=uitoa(bb,(*(uint *)v++),0x10000000,16,'a');
                }}}};
#endif
                x=strlen(p);
                if(l<w)l=w;
                if(x>=w)
                {w=x;
                 if(state&0x102)w++;
                }
                if(x>=l)
                {l=x;
                 if(state&0x102)l++;
                }
                if(l)
                {memset(t,(state&0x40)?'0':' ',l);
                 if(w)memset(t+(l-w)*((~state)&1),'0',w);
                }
                memcpy(t+(l-x)*((~state)&1)+(state&0x102!=0) ,p,x);
                if(state&0x102)t[l-w]=(state&0x100)?'-':'+';
              t+=l;
              if(0){
     case 's':
                p= * (char**)v;
                DWORD_PTR(v) += sizeof(char**);
                if(!p )p="";
                if(p<(char *)MIN_PTR)
                {
                  dprint("bvprintf s error arg=%X at %.32s\n",p,fmt-1);
                  p="";
                }
                x=strlen(p);
                if(w && x>w) x=w;
                if(l<x)l=x;
               bl=(l-x)*((~state)&1);
               bx=bl+x;
               for(k=0;k<l;++k)
               {
#if CRREPL
                if(*p=='\n')*t++='\r';
#endif

                if(k<bl || k>=bx) {
                  *t++=' ';
                }
                else {
                  if(!*p) break;
                  *t++ = *p++;
                }
                if( (i=t-bfr)> BFR_LIM )
                {
                  ii += x = Flush(par,bfr,i);
                  t=bfr;
                  if(i != x)
                    goto exLoop;
                }
               }

               // memset(t,' ',l);
               // memcpy(t+(l-x)*((~state)&1),p,x);
               // t+=l;
               }
               ++fmt;
               break;

  #if 0
       case 'p':
                if((p=(char*) *v++ ))
                   DWORD_PTR(*p)=ii+(t-bfr);
                ++fmt;
                break;
       case 'G':
       case 'g':
       case 'E':
       case 'e':
       case 'f':
                p=(char*) *v++;
                if(state&0x10)dbl=*(long double *)p;
                else if(state&0x20)dbl=(float )p;
                else dbl=*(double *)p;
                if(dbl<0){dbl=-dbl; state|=0x100;};
              switch(*fmt++)
              {
               case 'G':
               case 'g': if( (dbl>10000) || (dbl<1e-4 )){
               case 'E':
               case 'e':  state|=0x400;
                          k=0;
                          while(dbl<1){dbl*=10; if(--k<=-304)break;}
                          while(dbl>=10){dbl/=10; if(++k>=304)break;}
                         }
              };
                bl=(long) dbl;
                dbl-=bl;
                if(!(state&0x200))w=4;
                i=w;
                while(i--)dbl*=10;
                bx=(long) dbl;

  #endif

      default:
              goto lb1;

    }

  }
  else
  {
   lb1:
#if CRREPL
   if(*fmt=='\n')*t++='\r';
#endif
   *t++=*fmt++;
  }
  if( (i=t-bfr)>BFR_LIM )
  {
    ii += x = Flush(par,bfr,i);
    t=bfr;
    if(i != x)
      break;
  }
 }

exLoop:
 /*
 if( (i=t-bfr) )
 {
   ii+=Flush(par,bfr,i);
   t=bfr;
 }
 return ii;
// */
 return ii+(t-bfr);

};


#ifdef TEST_BUILD
//gcc -m32 -g bvprintf.cpp -o o/bvprintf -DTEST_BUILD -DPF_LONG_LONG -DSYSUNIX
//-DUSEVALIST
//-DBPRINTF_INLINE
//#include <unistd.h>
//#include <stdlib.h>
#include <stdlib.h>

int TPrintFlush(void *,char *s,ulong l)
{
  return write(1,s,l);
}

uint atouisc(const char *&a)
{register uint r=0,l,n=0;
 while( (l=*a-'0')<10 ){r=r*10+l;a++; if(n++>11)break; };
 return r;
};

int main()
{
  BFILE bf;
  char  b[BFR_LIM+0x100];
  bf.Init(0,(PrintFlush)TPrintFlush,b);
  bf.bprintf(
    "u:%u\n"
    "X:%X\n"
    "o:%o\n"
    ,12345678
    ,0x12345678
    ,012345670
    );

  bf.bprintf(
    "llu:%llu\n"
    "llX:%llX\n"
    "llo:%llo\n"
    ,12345678901234567
    ,0x123456789ABCDEF
    ,01234567012345670
    );
  bf.fflush();
  return 0;
}

#endif



};
