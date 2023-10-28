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

//tt eww

#ifndef SRV_H
#include "srv.h"
#include "gz.h"
#include "regular.h"
#endif

#ifndef ZEXPORT
#define ZEXPORT
#endif


typedef int (ZEXPORT * tgzfunc)(z_streamp strm, int flush);
//typedef int (ZEXPORT * tgzfunc3)(z_streamp strm, int flush,const char *version, int stream_size);
typedef int (ZEXPORT * tgzfunc3)(z_streamp strm, int  level, int  method,
                                      int windowBits, int memLevel,
                                      int strategy, const char *version,
                                      int stream_size);
typedef int (ZEXPORT * tgzfunc2)(z_streamp strm);
typedef int (ZEXPORT * tgzfunc4)(z_streamp strm,int,const char *version, int stream_size);
typedef int (ZEXPORT * transfunc)(void *x,void *b, int l);
typedef ulong (ZEXPORT * tfcrc)(ulong ,const void *b, int l);


tgzfunc  ZPack,ZUnpack;
tgzfunc3 ZPackInit;
tgzfunc2  ZPackEnd,ZUnpackEnd;
tgzfunc4 ZUnpackInit;
tfcrc    pcrc32;

char *zlib=
#ifndef SYSUNIX
"zlib-1.dll";
#else
"/usr/lib/libz.so";
#endif
char * NoGZSuf=".gif,.jpg,.jpeg,.gz,.zip,.rar,.arj,.jar";
uint  GZSizeLow=512;
#ifndef MINGW
#define HINSTANCE void *
//typedef  void *HINSTANCE;
#endif

HINSTANCE  hZlib;
int zlibstate;
#ifndef SYSUNIX
//HANDLE hZlib;
#else

#ifdef LPC_ARM
#include "nommc_dlfcn.h"
#else
#include <dlfcn.h>
#endif


#define LoadLibrary(a)   dlopen(a,2)
#define GetProcAddress(a,b)   dlsym(a,b)

#endif

const uchar gz_head[]=
{'C','o','n','t','e','n','t','-','E','n','c','o','d','i','n','g',':',' ','g','z','i','p',
'\r','\n','\r','\n',
 0x1f, 0x8b,
 Z_DEFLATED, 0 /*flags*/,
 0,0,0,0 /*time*/,
 0 /*xflags*/, 3
};
int InitZLib()
{
 if(zlibstate)return zlibstate;
 if(!(hZlib=LoadLibrary(zlib)))goto lberr;
 //if(!(ZPackInit=(tgzfunc)GetProcAddress(hZlib,"deflateInit")))
   ZPackInit=(tgzfunc3)GetProcAddress(hZlib,"deflateInit2_");
 //if(!(ZUnpackInit=(tgzfunc2)GetProcAddress(hZlib,"inflateInit")))
   ZUnpackInit=(tgzfunc4)GetProcAddress(hZlib,"inflateInit2_");

 if( (!ZPackInit) || (!ZUnpackInit) ||
   (!(ZPack=(tgzfunc)GetProcAddress(hZlib,"deflate"))) ||
   (!(ZUnpack=(tgzfunc)GetProcAddress(hZlib,"inflate"))) ||
   (!(ZPackEnd=(tgzfunc2)GetProcAddress(hZlib,"deflateEnd"))) ||
   (!(ZUnpackEnd=(tgzfunc2)GetProcAddress(hZlib,"inflateEnd"))) ||
   (!(pcrc32=(tfcrc)GetProcAddress(hZlib,"crc32")))
  )
  {
 lberr:
#ifdef SYSUNIX
   printf("Error loading Zlib library: %s\n",dlerror());
#else
   debug("Error loading Zlib library\n");

#endif
   --zlibstate;
   return -1;
  }
  ++zlibstate;
  debug("External Zlib library has been loaded...");
  return 1;
}

//*
#ifndef NOTINTEL
#define MCDECL  __attribute__ ((cdecl))
#else
#define MCDECL
typedef void* voidpf;
typedef uchar Bytef;

#endif

voidpf MCDECL alloc_f(voidpf opaque, uInt items, uInt size)
{
// debug("Malloc %u %u",items, size);
 return malloc(items * size);
};
void MCDECL free_f(voidpf opaque, voidpf address)
{
 free(address);
};
// */

#ifdef GSUNPACK
int GZObj::Get(void *t,int l)
{
 int r;
 z.next_out=(Bytef *)t;
 z.avail_out=l;
 while(z.avail_out>0)
 {
   if(z.avail_in)
   { if((f)(&z,tf?0:Z_FINISH) <0 )  return -3;
     break;
   }
   if(!tf)  break;
   if((r=(tf)(par,b,ZBOSIZE))>0)
   {
    z.next_in=(Bytef *)b;
    z.avail_in=r;
   }
   else
   {
    tf=0;
   }
 }
 return (l-z.avail_out);
}
int GZRcv(Req *th,void *b,int l)
{ return thi->Get(b,l) ;}

#endif


int Req::IGZU()
{
 if(zlibstate>0)
 {
  gz=(GZObj*)malloc(sizeof(GZObj));  //new GZObj;
  if(((GZObj *)gz)->InitUnpack((transfunc) Snd,this)>=0)
  {Snd=(tfSnd) GZSndSkip;
   return 1;
  }
  //delete gz;
  free(gz);
  gz=0;

 }
 return 0;

};



int GZObj::Put(const void *t,int l)
{
 int r,i;
 z.next_in=(Bytef *)t;
 z.avail_in=l;
 crc=(pcrc32)(crc,t,l);
 totallen+=l;
// debug("Put: %d %X",l,*(const ulong *)t);
 while(z.avail_in>0)
 {
  z.next_out=(Bytef *)b;
  z.avail_out=ZBOSIZE;

  if( (r=(f)(&z,Z_NO_FLUSH)) <0 )
  //if( (r=(ZPack)(&z,Z_NO_FLUSH)) <0 )
  {
   debug("ZLIB error %d %s",r,
//  z.next_in,  z.avail_in,  z.total_in, z.next_out,  z.avail_out,  z.total_out
  (z.msg)?z.msg:""
//  z.state, z.zalloc, z.zfree, z.opaque, z.data_type, z.adler, z.reserved
   );
   return -3;
  }
  //(f)(&z,0);
  i=ZBOSIZE-z.avail_out;
//  debug("GZIP OK %d %d",r,i);
  if(i>0)
  {
   if(h2>0)_hwrite(h2,b,i);
   if((r=(tf)(par,b,i))<=0)return r;
  }
 }
 return l;
}

#ifdef MINGW
typedef void*(*typealloc)(void*, uInt, uInt);
typedef void (*typefree)(void*, void*);
#endif

int GZObj::InitPack(transfunc tff,void *pp)
{int r;
 //memset(&z,0,sizeof(z));
 memset(this,0,sizeof(*this));
 tf=tff;
 par=pp;
 f=ZPack;
 fe=ZPackEnd;
#if (!defined(MIPS) ) && ! defined(LPC_ARM)
#ifdef MINGW
 z.zalloc =(typealloc) alloc_f;
 z.zfree = (typefree) free_f;
#else
 z.zalloc = alloc_f;
 z.zfree =  free_f;

#endif
#endif
 //z.opaque = 0;
 z.data_type =Z_UNKNOWN;
  if( (r=(ZPackInit)(&z,9, //Z_DEFAULT_COMPRESSION,
  Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY
  ,"1.1.4",sizeof(z)) ) )
  {
    debug("GZip Init error %d %s",r,(z.msg)?z.msg:"" ) ;
  };
 return r;
};

int GZObj::InitUnpack(transfunc tff,void *pp)
{
 int r;
 memset(this,0,sizeof(*this));
 tf=tff;
 par=pp;
 f=ZUnpack;
 fe=ZUnpackEnd;
#if (!defined(MIPS) ) && ! defined(LPC_ARM)
#ifdef MINGW
 z.zalloc =(typealloc) alloc_f;
 z.zfree = (typefree) free_f;
#else
 z.zalloc = alloc_f;
 z.zfree =  free_f;
#endif

 #endif
 //z.opaque = 0;
 z.data_type =Z_UNKNOWN;

 if((r=(ZUnpackInit)(&z,-MAX_WBITS,"1.1.4",sizeof(z))) )
 {
  debug("GZip Init error %d %s",r,(z.msg)?z.msg:"" ) ;
 };
 return r;

};

void GZObj::End()
{int r,l;
 do{
  z.next_out=(Bytef *)b;
  z.avail_out=ZBOSIZE;
  r=(f)(&z,Z_FINISH);
  if((l=ZBOSIZE-z.avail_out)>0 )
  {
   if(h2>0)_hwrite(h2,b,l);
   if((tf)(par,b,l)<=0)break;
  }
//  debug("FZ finish %u %d",l,r);
 }while(r==Z_OK);
 if(f==ZPack)
 {
  //crc=htonl(crc);
  //totallen =htonl(totallen);
  (tf)(par,&crc,8);
  if(h2>0)_hwrite(h2,(char *)&crc,8);
 }
 if(h2>0)
 {_lclose(h2);
  h2=0;
 }
 (fe)(&z);
}

#define thi  ((GZObj *)(th->gz))
int GZSnd(Req *th,const void *b,int l)
{ return thi->Put(b,l) ; }

int GZObj::ChuInit(char *x,int ll)
{
 char *t;
 int i,j,r;
   t=x;
// debug("ChuInit: chul=%d(%X) |%.8s|",chul,chul,x);
   if(chul<=0)
   {chul=0;
    x=SkipSpace(x);
   }
   r=x-t;
   t=x;
   i=atouix(x);
   if((chustate&ChukeWaitLF) && chul)
   { for(j=0; ((uchar)(x[j]-'0'))<=9 ||
              ((uchar)((x[j]|0x20)-'a'))<=5  ; ++j ) chul<<=4; }
   chul+=i;
   if(!(x=memchr(x,'\n',ll)))
   {
    chustate|=ChukeWaitLF;
    return ll;
   }
   chustate&=~ChukeWaitLF;
   ++x;
//   debug("ChuInit2: chul=%d(%X) %X",chul,chul,DWORD_PTR(*x));
   return (x-t)+r;
};

int GZSnd2(Req *th,const void *b,int l)
{int i,j,k;
 if(thi->lflg)
 {
  if(l>8)thi->Put(thi->last,thi->lflg);
  else
  {
   if((j=thi->lflg+l)>8)
   {
    i=8-thi->lflg;
    if(i)
    {if(i>l)i=l;
     memcpy(thi->last+thi->lflg,b,i);
     DWORD_PTR(b)+=i;
     l-=i;
     thi->lflg+=i;
    }
    thi->Put(thi->last,k=j-8);
    if(k<8)
    {memcpy(thi->last,thi->last+i,8-k);
     thi->lflg-=k;
    }
   }
   if(l)memcpy(thi->last+thi->lflg,b,l);
   thi->lflg+=l;
   return l+i;
  }
 }
 i=l;
 if(l>8)
 {i=thi->Put(b,j=l-8);
  DWORD_PTR(b)+=j;
  l=8;
 }
 thi->lflg=l;
 DWORD_PTR(thi->last[0])=*(const ulong *)b;
 DWORD_PTR(thi->last[4])=((const ulong *)b)[1];
 return i;
}


typedef int (*tfGZSnd2)(Req *th,const void *b,int l);
int GZSndChu(Req *th,uchar *x,int ll)
{
 char *t;
 int i,j;
 int r=0;
 //x[ll]=0;
// debug("Chu: chul=%d(%X) %d",thi->chul,thi->chul,ll);
 while(ll>0)
 {
   if(thi->chul<=0 || (thi->chustate&ChukeWaitLF) )
   {
    i=thi->ChuInit((char *)x,ll);
    x+=i; ll-=i; r+=i;
   // debug("++%X %u",x,ll);
   }
   else
   {
    i=ll;
    if(thi->chul<ll)i=thi->chul;
    if((j=(//thi->chustate&InChuke
        th->Snd!=( tfSnd) GZSndChu
          )?(GZSndSkip(th,x,i)):
            (GZSnd2(th,x,i))
       )<0) return j;
    r+=i;
    thi->chul-=i;
    ll-=i;
    x+=i;
   }
 }
 return r;
}
int GZSndSkip(Req *th,uchar *b,int l)
{
 char *p,*pp;
 int r,i,j,defl;
  defl=0;
  r=0;
//  debug("GSU0 -- %X %d",DWORD_PTR(*b),l);
  if( (!(th->fl&F_SKIPHEAD)
      )  &&
      ((p=strstr((char *)b,"\r\n\r\n"))||
       (pp=strstr((char *)b,"\n\n"))
      )
    )
  {if(p)p+=4; else p=pp+2;
   j=i=p-(char*)b;
   p[-1]=0;
   if((pp=stristr((char *)b,"Content-Encoding:")) )
   {defl=
#ifdef A_64
      NULL !=
#else
       (int)
#endif
       stristr(pp," deflate");
    i=DelStr(pp,(char *)b,i);
   }
   if((pp=stristr((char *)b,"Content-Length:")) ) i=DelStr(pp,(char *)b,i);
   if((pp=stristr((char *)b,"Transfer-Encoding: chunked")) )
   {i=DelStr(pp,(char *)b,i);
    thi->chustate=UseChuke;
   }
   ((char *)b)[i-1]='\n';
   r=thi->tf(th,(char *)b,i);
   th->fl|=F_SKIPHEAD;
   if((l-=j)<=0)return r;
   b=(uchar *)p;
  }
//  debug("GSU01 -- %X %d",DWORD_PTR(*b),l);

  if(th->fl&F_SKIPHEAD)
  {
   if((thi->chustate&(UseChuke|InChuke))==UseChuke)
   {// i=thi->ChuInit(b,l);
    // b+=i; l-=i; r+=i;
    thi->chustate|=InChuke;
    r=GZSndChu(th,b,l);
    thi->chustate&=~InChuke;
    return r;
   }
//   debug("GSU01A -- %X %d",DWORD_PTR(*b),l);
   if(!(thi->totallen))
   {
/*
    while((l>=3) && *(ushort *)b != 0x8B1f )
    { if(defl)
      { //debug(" Deflate...");
       goto cntLp;
      }
      ++b; --l; //debug(" Find.");
    }
*/
     if((l>=3) && *(ushort *)b != 0x8B1f ) goto cntLp;
     if(l<3)
     { //debug("Debug <4");
      return r;
     }
   // debug(" Found.");
    DWORD_PTR(b)+=3;
    thi->crc=*b++;
    l-=4;
    thi->totallen=4;
    if(l<=0)return r+4;
   }
#define HEAD_CRC     0x02 /* bit 1 set: header CRC present */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
   if(thi->totallen<10)
   { i=10-thi->totallen;
     if(i>l){ thi->totallen+=l;

      return r+l;
     }
     //if( (b[i-1]&0xF0) && b[i-2]==2 ){--i;}
     l-=i;
     DWORD_PTR(b)+=i;
     r+=i;
     thi->totallen=10;

   }
//   debug("GSU02 -- %X %X %d",DWORD_PTR(*b),thi->crc ,l);
   if(thi->crc & EXTRA_FIELD)
   {
    if(thi->crc&0xFFFF00) i=thi->crc>>8;
    else
    {i=*(ushort *)b;
     l-=2;
     DWORD_PTR(b)+=2;
    }
    if(l<i)
    {thi->crc =thi->crc|((i-l)<<8);
     return r+l;
    }
    l-=i;
    r+=i;
    DWORD_PTR(b)+=i;
    thi->crc&=(~EXTRA_FIELD)&0xFF;
   }
   while(thi->crc&(ORIG_NAME|COMMENT))
   {
    do{ if(l<=0)return r;
      l--; ++r; }while( *b++ );
    if(thi->crc&ORIG_NAME)thi->crc&=~ORIG_NAME;
    else thi->crc&=~COMMENT;
   }
   if(l<=0)return r;
   if(thi->crc&HEAD_CRC)
   {
    --l;
    r++;
    thi->crc&=~HEAD_CRC;
    if(!l)
    { thi->totallen=9;
      return r;
    }
    --l;
    r++;
    DWORD_PTR(b)+=2;
   }
  cntLp:
//   debug("GSU1 -- %X %d",DWORD_PTR(*b),l);
   th->Snd=(thi->chustate)?(tfGZSnd2)GZSndChu:GZSnd2;
   i=GZSnd2(th,b,l);
   if(i<0)return i;
   return r+i;
  }
  else  return thi->tf(th,b,l);
  return r;
}

#undef thi
#undef  HEAD_CRC
#undef  EXTRA_FIELD
#undef  ORIG_NAME
#undef  COMMENT

//struct Req;
//int JustSnd(Req *th,const void *b,int l);
//int JustRcv(Req *th,void *b,int l);

int Req::IGZP()
{
 if(zlibstate>0)
 {
//  debug("IGZP");
  gz=(GZObj *) malloc(sizeof(GZObj));
  if(((GZObj *)gz)->InitPack((transfunc) Snd
     ,this)>=0)
  {//Snd= GZSnd;
   return 1;
  }
  free(gz);
  gz=0;
 }
 return 0;
};
void Req::GZEnd()
{
 if(gz)
 {
  DBGL("")
  ((GZObj *)gz)->End();
  free(gz);
  gz=0;
 }
};

