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


#include "arhbb.cpp"

#define FRAME4  0x100
#define FRAME_2  0x6000
#define MIN_SEK2 3
#define MIN_SEK  5


#define BLK_LIMIT  0x80000

struct BLZHead
{ int l,x,b,w,d,o;
  ulong chk;
//  int xa,ba,wa,da,oa;
};


#ifdef ENCODE


uchar Csh[BLK_LIMIT*3];
uchar *Cash[0x10010];
//ulong  CIND[0x10000];
uchar  TDat[BLK_LIMIT];
uchar  DDat[BLK_LIMIT/4+20];
uchar  WDat[BLK_LIMIT/3+20];
uchar  BDat[BLK_LIMIT/2+20];
uchar  ODat[BLK_LIMIT];


#undef debug
//#define debug printf
#define debug(a,b...)

#ifdef __GNUC__

inline ulong M3(uchar *a,int i)
{
 asm volatile(
  "lea (%%eax,%%eax,2),%%eax\n"
"   movl (%%edx,%%eax,1),%%eax\n"
"   andl $0xFFffFF,%%eax\n"
"  ":"=&a" (i),"=&d" (a)
   :"0" (i), "1"(a)
 );
 return i;
}
inline void SetM3(uchar *a,int i,ulong v)
{
 asm volatile(
  "lea (%%eax,%%eax,2),%%eax\n"
"   orl %%ecx,(%%edx,%%eax,1)\n"
"  ":"=&a" (i),"=&d" (a),"=&c" (v)
   :"0" (i), "1"(a) , "2"(v)
 );
}

#else
inline ulong M3(uchar *a,ulong i){return DWORD_PTR(a[i*3]); }
inline void  SetM3(uchar *a,ulong i,ulong v){ DWORD_PTR(a[i*3])|=v; }

#endif
void InitCache(uchar *s,int l)
{int i,j,x,jj;
 memset(Csh,0,sizeof(Csh));
 memset(Cash,0,sizeof(Cash));
 --l;
 for(i=0;i<l;++i)
 { DWORD_PTR(Cash[1l+WORD_PTR(s[i])])++;
 }
 Cash[0]=Csh;
 for(i=1;i<0x10000;++i)Cash[i]=Cash[i-1]+4+(3*(ulong)(Cash[i]));
#define CIND  ((ulong *)TDat)
memset(CIND,0,0x40000);
 for(i=0;i<l;++i)
 {
  j=WORD_PTR(s[i]);
  SetM3(Cash[j],CIND[j]++,i);
 }
#undef CIND
 memset(DDat,0,sizeof(DDat));
 memset(WDat,0,sizeof(WDat));
 memset(BDat,0,sizeof(BDat));
 memset(ODat,0,sizeof(ODat));
 memset(TDat,0,sizeof(TDat));

}



int fbest(uchar *s1,uchar *s,uchar *e,int &n)
{
uchar *t,*p,*r, *cc;
 int bst,i,l,ll,j,je,k,rr;

  bst=0;
  ll=e-s;
//  rr=0;
  if(! (cc=Cash[WORD_PTR(*s) ] ) )return 0;
  je=s-s1;
  while( (k=DWORD_PTR(*cc)&0xFFffFF)<je && k )
  {
   t=s1+k;
   for(i=2;i<ll;++i) if(t[i]!=s[i])break;
   if(i>=bst
  //    || (bst==MIN_SEK && i==(MIN_SEK-1) && (s-t)<FRAME_2 )
    ||  (i==(bst-1) && (s-t)<0x8000 && rr>0x8000 )
   )
   {

    bst=i;
    r=t;
    rr=s-r;
    if(bst>=256)break;
   }
   cc+=3;
  }
  if(bst>=256)bst=256;
  i=s-r;
  if(bst>=MIN_SEK
//     || (bst>=3 && i<0x4000)
     || (bst>=MIN_SEK2 && i<FRAME_2)
     || (bst>=2 && i<FRAME4))
  {
//   n=r-s1;
   n=i;
   return bst;
  }
  return 0;
}



int LZZPk(uchar *t,uchar *s0,int l)
{
 register uchar *s;
 int i,j,n; //,b,w,d,x,o;
 BLZHead bh;
 uchar *e;
 s=s0;
 e=s+l;

 InitCache(s,l);
#define b bh.b
#define w bh.w
#define d bh.d
#define x bh.x
#define o bh.o
 b=0;
 w=0;
 d=0;
 x=0;
 o=1;
 bh.l=l;
 bh.chk=0;
 for(i=0;i<l;++i){bh.chk+=((ulong)(s[i]))<<((i&15)<<2);}
 ODat[0]=*s++;



 while(s<e)
 {
  j=fbest(s0,s,e,n);
  if(j<2){ ODat[o++]=*s++;  TDat[x++]=0; }
  else
  {TDat[x++]=j-1;
   BDat[b++]=n;
   if(j>=MIN_SEK2)
   {
    if(j<MIN_SEK || (s-s0)<0x10000 || n<=0x7FFF ){ WDat[w++]=n>>8;}
    else
    { WDat[w++]=(n>>8)|0x80;
      DDat[d++]=n>>15;
    }

/*
    if(j<MIN_SEK || (s-s0)<FRAME_2 ){ WDat[w++]=n>>8;}
    else {
      if(n<0x8000 || (s-s0)<0x10000 )BDat[b++]=n>>8;
      else
      { BDat[b++]=(n>>8)|0x80;
        DDat[d++]=n>>15;
      }
    }
*/

   }
/*
   else if(j<MIN_SEK){ WDat[w++]=n;}
   else {  DDat[d++]=n; }
*/
   s+=j;
  }

 }

 debug("b=%u  w=%u  d=%u  x=%u  o=%u \n ",b,w,d,x,o);

 s=t+sizeof(BLZHead);
 s+=((UnlimitedRange*)s)->Encode(TDat,x,1);
 s+=((UnlimitedRange*)s)->Encode(ODat,o,1);
 if(b){
  s+=((UnlimitedRange*)s)->Encode(((uchar *)BDat),b,1);
//  s+=((UnlimitedRange*)s)->Encode(((uchar *)BDat)+1,b,1);
 }
 if(w){
  s+=((UnlimitedRange*)s)->Encode(((uchar *)WDat),w,1);
//  s+=((UnlimitedRange*)s)->Encode(((uchar *)WDat)+1,w*2,2);
 }
 if(d){
  s+=((UnlimitedRange*)s)->Encode(((uchar *)DDat),d,1);
//  s+=((UnlimitedRange*)s)->Encode(((uchar *)DDat)+1,d*4,4);
//  if(l>0x10000)s+=((UnlimitedRange*)s)->Encode(((uchar *)DDat)+2,d*4,4);
 }
 memcpy(t,&bh,sizeof(bh));
 return s-t;
#undef b
#undef w
#undef d
#undef x
#undef o

}

#endif

#ifdef DECODE


int FNCATRIBUTE LZZUnpk(uchar *t,uchar *s0)
{
#define pbh  ((BLZHead *)s0)
 register uchar *s;
 int j,n;
 uchar  *b0,*px,*po,*pb,*e;
#if 0
 ushort *pw;
 ulong  *pd;
 px=b0=
#ifndef MALLOC
   new uchar[pbh->w*2+pbh->b+pbh->d*4+pbh->o+pbh->x+0x200];
#else
   (uchar*) MALLOC(pbh->w*2+pbh->b+pbh->d*4+pbh->o+pbh->x+0x200);
#endif
 po=px+pbh->x+32;
 pb=po+pbh->o+32;
 pw=(ushort *)(pb+pbh->b+32);
 pd=(ulong *)(pw+pbh->w+32);
 memset(pd,0,pbh->d*4);

 s=s0+sizeof(BLZHead);
 ((UnlimitedRange*)s)->Decode(px,1);
 s+=((UnlimitedRange*)s)->BlkSize;
 ((UnlimitedRange*)s)->Decode(po,1);
 s+=((UnlimitedRange*)s)->BlkSize;
 if(pbh->b){
  ((UnlimitedRange*)s)->Decode(((uchar *)pb),1);
  s+=((UnlimitedRange*)s)->BlkSize;
 }
 if(pbh->w){
  ((UnlimitedRange*)s)->Decode(((uchar *)pw),2);
  s+=((UnlimitedRange*)s)->BlkSize;
  ((UnlimitedRange*)s)->Decode(((uchar *)pw)+1,2);
  s+=((UnlimitedRange*)s)->BlkSize;
 }
 if(pbh->d){
  ((UnlimitedRange*)s)->Decode(((uchar *)pd),4);
  s+=((UnlimitedRange*)s)->BlkSize;
  ((UnlimitedRange*)s)->Decode(((uchar *)pd)+1,4);
  s+=((UnlimitedRange*)s)->BlkSize;
  if(pbh->l>0x10000)
  {
   ((UnlimitedRange*)s)->Decode(((uchar *)pd)+2,4);
   s+=((UnlimitedRange*)s)->BlkSize;
  }
 }

 e=px+pbh->x;
 s=t;
 *s++=*po++;
 while(px<e)
 {

  if( !(j=*px++) ){
   *s++=*po++;
  }
  else
  {++j;
   if(j<MIN_SEK2)n=*pb++;
   else if(j<MIN_SEK)n=*pw++;
   else n=*pd++;
   memcpy(s,s-n,j);
   s+=j;
  }
 }
#else
 uchar *pw;
 uchar *pd;
 px=b0=
#ifndef MALLOC
   new uchar[pbh->w+pbh->b+pbh->d+pbh->o+pbh->x+0x200];
#else
   (uchar*) MALLOC(pbh->w+pbh->b+pbh->d+pbh->o+pbh->x+0x200);
#endif
 po=px+pbh->x+32;
 pb=po+pbh->o+32;
 pw=pb+pbh->b+32;
 pd=pw+pbh->w+32;

 s=s0+sizeof(BLZHead);
 ((UnlimitedRange*)s)->Decode(px,1);
 s+=((UnlimitedRange*)s)->BlkSize;
 ((UnlimitedRange*)s)->Decode(po,1);
 s+=((UnlimitedRange*)s)->BlkSize;
 if(pbh->b){
  ((UnlimitedRange*)s)->Decode(((uchar *)pb),1);
  s+=((UnlimitedRange*)s)->BlkSize;
 }
 if(pbh->w){
  ((UnlimitedRange*)s)->Decode(((uchar *)pw),1);
  s+=((UnlimitedRange*)s)->BlkSize;
 }
 if(pbh->d){
  ((UnlimitedRange*)s)->Decode(((uchar *)pd),1);
  s+=((UnlimitedRange*)s)->BlkSize;
 }

 e=px+pbh->x;
 s=t;
 *s++=*po++;
 while(px<e)
 {

  if( !(j=*px++) ){
   *s++=*po++;
  }
  else
  {++j;
   n=*pb++;
   if(j>=MIN_SEK2)
   {
    n|=(*pw++)<<8;
    if( !(j<MIN_SEK || (s-t)<0x10000 || n<=0x7FFF ))
    { n=(n&0x7FFF)|(*pd++)<<15;
    }
   }
   memcpy(s,s-n,j);
   s+=j;
  }
 }
#endif

 ulong chk=0;
 int i;
 for(i=0;i<pbh->l;++i){chk+=((ulong)(t[i]))<<((i&15)<<2);}
#ifndef MALLOC
  delete b0;
#else
  FREE(b0);
#endif
 if(chk!=pbh->chk)return 0;
 return pbh->l;
#undef pbh
}


#endif


#ifdef PROGR
#ifdef __GNUC__
int PASCAL WinMain( HANDLE hinst, HANDLE prev_inst, LPSTR cmdline, int cmdshow )
{
 int argc;
 char *argv[5];
 argc=0;
 goto  ll1;
 while(*cmdline){
  if( *cmdline!=' ' )
  {
   if(!cmdline[-1])
   {
   ll1:
    argv[argc++]=cmdline;
    if(argc>4)break;
   }
  }
  else *cmdline=0;
  *cmdline++;
 };
#else
int main( int argc, char *argv[])
{
#endif
 int din,dout,i,k,b,n,c,v;
 ulong  j=0,tk;
 int fo,fi;
 char *bbuf,*tbuf,*ttbuf;

  if( argc<=2 )
  {
    printf("Command line:\n%s src tgr [key]",argv[0]);
    return -1;
  }

  if( ! (fi = _lopen(argv[1],0)))
  {
    printf("Unable to open %s",argv[1]);
    return -1;
  }




  k=_llseek(fi,0,2);
  _llseek(fi,0,0);
  tbuf=ttbuf=(bbuf=new char[(k+2048)*20])+k+32;
  i=_hread(fi,bbuf,k);
  _lclose(fi);
  j=0;
  tk=GetTickCount();
#if 0
//  j=((UnlimitedRange*)tbuf)->Encode((uchar*)bbuf,i,1);
//  ((UnlimitedRange*)tbuf)->Decode(Csh,1);
  j=LZZPk((uchar *)tbuf,(uchar *)bbuf,i);
  k=LZZUnpk(Csh,(uchar *)tbuf);
  memcpy(ttbuf,Csh,i);
  printf("Pack size: %u  %u%% result:%d %d %d\n",j,j*100/i,memcmp(Csh,bbuf,i),i,k);
  j=i;
 // return 0;
#else
  if(argc>=4)
  {
   j=
   //((LZHead *)bbuf)->Decode((uchar*)tbuf);
   LZZUnpk((uchar *)tbuf,(uchar *)bbuf);
 /*
   do{
    j+=((UnlimitedRange*)bbuf)->Decode((uchar*)tbuf+j);
    i-=n=((UnlimitedRange*)bbuf)->CountOfBytes*4+sizeof(UnlimitedRange)+4;
    bbuf+=n;
   // printf("j=%X n=%X i=%X\n",j,n,k-i);
    //tbuf+=c;
   }while( i>0 );
 */
  }
  else
  {
   j=
    //((LZHead *)tbuf)->Encode((uchar*)bbuf,i);
    LZZPk((uchar *)tbuf,(uchar *)bbuf,i);
/*
   if( (n=i/0x7FFF) )n=i/n;
   else n=i;
   while(i>0)
   {
    j+=c=((UnlimitedRange*)tbuf)->Encode((uchar*)bbuf,n);
    tbuf+=c;
    bbuf+=n;
    i-=n;
  //  printf("j=%X c=%X n=%X i=%X\n",j,c,n,i);
    if( (n+0x4000)>i)n=i;
   }
*/
  }
  printf("Ok %u%%  (%u ms)\r\n",j*100/k,GetTickCount()-tk);
#endif
  fo = _lcreat(argv[2],0);
  if(fo<=0)
  {
    printf("\n**Error. Unable to create %s\n",argv[2]);
    return -1;
  }
  _hwrite(fo,ttbuf,j);
  _lclose(fo);

  return 0;
}

#endif
