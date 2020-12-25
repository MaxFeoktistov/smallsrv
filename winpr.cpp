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
#include "mstring.h"
#include <windows.h>
#include "mdef.h"

extern "C"
{
//char *env=(char*)GetEnvironmentStrings();

#define CRREPL 0

#if 1
//ndef SLIB_H
uint  atoui(const char *a)
{
 register uint r=0,l;
 while( (l=*a-'0')<10 ){r=r*10+l;a++;};
 return r;
};
long long  atoui64(const char *a)
{
 long long r=0;
 int l;
 while( (l=*a-'0')<10 ){r=r*10+l;a++;};
 return r;
};
uint  atouix(const char *a)
{
 register uint r=0,l;
 while( (l=*a-  ( (*a>='a')?'a'-10:(*a>='A')?'A'-10:'0' )
        )<16 ){r=(r<<4)+l;a++;};
 return r;
};


uint atouisc(const char *&a)
{
 register uint r=0,l;
 while( (l=*a-'0')<10 ){r=r*10+l;a++;};
 return r;
};
#endif
#define atoi atoui

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

int vprintf(const char *fmt,void **v);

typedef int (*PrintFlush)(void *,char *,ulong);
struct FILE
{
 int   _flags;
 void* _fileno;
 PrintFlush Flush;
 PrintFlush Read;
 union{
 char *bfr;
 char  b[4];
 };
 char *pbfr;
};
#define REPLASE_LF 0x8

int NullFunc(){return 0;}
int FFlush(FILE *h,char *bfr,ulong i)
{WriteFile(h->_fileno,bfr,i,(ulong *)&i,0);  return i;}
int FRead(FILE *h,char *bfr,ulong i)
{ReadFile(h->_fileno,bfr,i,(ulong *)&i,0);  return i;}

FILE xstdout={REPLASE_LF,GetStdHandle((ulong)STD_OUTPUT_HANDLE),FFlush,(PrintFlush)NullFunc};
FILE xstderr={REPLASE_LF,GetStdHandle((ulong)STD_ERROR_HANDLE),FFlush,(PrintFlush)NullFunc};
FILE xstdin ={0,GetStdHandle((ulong)STD_INPUT_HANDLE),(PrintFlush)NullFunc,FRead};

#define stdout (&xstdout)
#define stderr (&xstderr)
#define stdin  (&xstdin)



int vfprintf(FILE *fl,const char *fmt,void **v)
{
 #define Flush(a,b,c)  (fl->Flush)(fl,b,c)

 char *t,*t1,*p,*pp;
 int i,ii,x,k,bl,bx;
 int state;
 char bfr[0x0F00],bb[64];
#define GEF 1
#if GEF
 register long  double dbl;
#endif
 int w,l;
 t=bfr;
 i=0;
 ii=0;

 while(*fmt)
 {
  if(*fmt=='%')
  {
   state=0; w=0; l=0;
  lb0:
    switch(*++fmt)
    {
     case '+':  state|=2;    goto lb0;
     case '-':  state|=1;    state&=~0x40; goto lb0;
     case '#':  state|=4;    goto lb0;
     case 'l':  state|=8;    goto lb0;
     case 'L':  state|=0x10; goto lb0;
     case 'h':  state|=0x20; goto lb0;
     case '0':  state|=0x40; state&=~1; goto lb0;
     case ' ':  state|=0x80; goto lb0;
     case '1':
     case '2':
     case '3':
     case '4':
     case '5':
     case '6':
     case '7':
     case '8':
     case '9':  l=atouis(fmt); fmt--;  goto lb0;
     case '.':  w=atouis(++fmt); state|=0x200;  fmt--; goto lb0;

     case 'u':  p=uitoa(bb,(ulong)(*v++) ); if(0){
     case 'i':
     case 'd':  if((x=(long)(*v++))<0){x=-x; state|=0x100; } p=uitoa(bb,x); if(0){
     case 'o':  p=uitoa(bb,(ulong)(*v++),010000000000,8); if(0){
     case 'X':  p=uitoa(bb,(ulong)(*v++),0x10000000,16,'A'); if(0){
     case 'x':  p=uitoa(bb,(ulong)(*v++),0x10000000,16,'a');
                }}}};
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
     case 'c':  *t++=(char)(*v++); if(0){
     case 's':
                if(!(p=(char*) *v++))p="";
                x=strlen(p);
                if(w && x>w) x=w;
                if(l<x)l=x;
               bl=(l-x)*((~state)&1);
               bx=bl+x;
               for(k=0;k<l;++k)
               {
#if CRREPL
                if( (h->_flags&REPLASE_LF) && *p=='\n')*t++='\r';
#endif

                *t++=(k<bl || k>=bx)?' ':*p++;
                if( (i=t-bfr)>0x800 )
                {
                 ii+=Flush(par,bfr,i);
                 t=bfr;
                }
               }

               // memset(t,' ',l);
               // memcpy(t+(l-x)*((~state)&1),p,x);
               // t+=l;
               }}
               ++fmt;
               break;

  #if GEF
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
                if(w>20)w=20;
                if(state&0x10)
                { dbl=*(long double*)v; v+=3;   }
                //else if(state&0x20)dbl=(float)DWORD_PTR(p);
                else {dbl=*(double *)v; v+=2;}
                if(dbl<0){dbl=-dbl; state|=0x100;};
              switch(*fmt++)
              {
               case 'F':
               case 'f':
                         if(dbl>1000000000){
               case 'G':
               case 'g': if( (dbl>10000) || (dbl<1e-4 )){
               case 'E':
               case 'e':  state|=0x400;
                          k=0;
                          while(dbl<1){dbl*=10; if(--k<=-304)break;}
                          while(dbl>=10){dbl/=10; if(++k>=304)break;}
                         }}
              };
                bl=(long) dbl;
                dbl-=bl;
                if(!(state&0x200))w=4;
                p=uitoa(bb,bl);
                x=strlen(p);
                if(l)
                {
                 memset(t,(state&0x40)?'0':' ',l);
                 if(state&0x400)l-=5;
                }
                i=((state>>8)|(state>>1))&0x1;

                if((l-=w+1+i+x)<0)l=0;
                if(i && (state&0x40)){i=0; *t++=(state&0x100)?'-':'+';}
                if(! (state&0x1) ){t+=l; l=0;}
                if(i)*t++=(state&0x100)?'-':'+';
                memcpy(t,p,x);
                t+=x;
                if(w)
                {*t++='.';
                 while(w--)
                 {
                  dbl*=10;
                  i=(int)dbl;
                  dbl-=i;
                  *t++=i+'0';
                 };
                }
                if(state&0x400)
                {
                 *t++='E'| (fmt[-1]&0x20);
                 *t++=(k<0)?k=-k,'-':'+';
                 uitoa(bb,k);
                 x=strlen(bb);
                 i=3 - ((state>>5)&0x1) + ((state>>4)&0x1);
                 DWORD_PTR(*t)=DWORD_PTR(bb[x-i]);
                 t+=i;
                }
                t+=l;
                break;
  #endif

     //case '%':
      default:
              goto lb1;

    }

  }
  else
  {
   lb1:
#if CRREPL
   if( *fmt=='\n' && (h->_flags&REPLASE_LF) )*t++='\r';
#endif
   *t++=*fmt++;
  }
  if( (i=t-bfr)>0x800 )
  {
   ii+=Flush(par,bfr,i);
   t=bfr;
  }
 }
 if( (i=t-bfr) )
 {
   ii+=Flush(par,bfr,i);
 }
 return ii;
#undef Flush
};


inline int printf(const char *fmt,...)
{ return vfprintf(stdout,fmt,(void **)((&fmt)+1)); }
inline int fprintf(FILE *h,const char *fmt,...)
{ return vfprintf(h,fmt,(void **)((&fmt)+1)); }
inline int vprintf(const char *fmt,void **v)
{ return vfprintf(stdout,fmt,v); }

#if 0

int SFlush(FILE *f,char *bfr,ulong i)
{
 memcpy(f->h,bfr,i);
 ((char *)(f->h))[i]=0;
 return i;
};

int mvsprintf(char *h,const char *fmt,void **v)
{FILE f;
 f->h=h;
 f->Flush=SFlush;
 return vfprintf(f,fmt,v);
}
inline int msprintf(char *h,const char *fmt,...)
{ return mvsprintf(h,fmt,(void **)((&fmt)+1));}

#endif
inline int fread(void *fmt,int x,int y,FILE *f)
{ return (f->Read)(f,(char *)fmt,x*y); };
inline int fwrite(void *fmt,int x,int y,FILE *f)
{ return (f->Flush)(f,(char *)fmt,x*y); };
FILE * fopen(char *n,char *tp)
{
 FILE *r,tr;
 int t,t1;
 r=new FILE;

 r->Read = (PrintFlush)NullFunc;
 r->Flush = (PrintFlush)NullFunc;
 t=0;

 if(*tp=='r' || tp[1]=='+')
 {t1=OPEN_EXISTING;
  t=GENERIC_READ;
  r->Read =FRead;
 }

 if(*tp=='w' || *tp=='a' || tp[1]=='+')
 {t1=(*tp=='w')?CREATE_ALWAYS:OPEN_ALWAYS;
  t|=GENERIC_WRITE;
  r->Flush=FFlush;
 }
 if( (r->_fileno=CreateFile(n,t,FILE_SHARE_READ,0,t1,FILE_ATTRIBUTE_NORMAL,0)) == INVALID_HANDLE_VALUE )
 {delete r;
  return 0;
 }
 if(*tp=='a')
 {
  SetFilePointer(r->_fileno,0,0,FILE_END);
 }
 return r;
};
inline int fclose(FILE *f)
{ CloseHandle(f->_fileno); delete f; };
inline int fseek(FILE *f,ulong pos,int t)
{ return SetFilePointer(f->_fileno,pos,0,t);}
inline int ftell(FILE *f)
{ return SetFilePointer(f->_fileno,0,0,FILE_CURRENT);}


//int puts(char *bfr){return WriteFile(stdout,bfr,strlen(bfr),0,0);}
inline int putsn(const char *bfr,int i){return WriteFile(xstdout._fileno,(void *)bfr,i,(ulong *)&i,0);}
inline int fputc(int a,FILE *f){return WriteFile(f->_fileno,&a,1,0,0);}
/*
char *getenv(const char *s)
{
  char *t;
  int l;
  l=strlen(s);
  for(t=env;WORD_PTR(*t);++t)
  {
   if(t==env || !t[-1])
   {
    if( (!memcmp(t,s,l)) &&
        t[l]=='='
      )return t+l+1;
   }
  }
  return 0;
};
*/

};
