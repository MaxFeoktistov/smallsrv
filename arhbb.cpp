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


//#ifndef FNCATRIBUTE
#undef FNCATRIBUTE
#define  FNCATRIBUTE
//#endif

#ifdef PROGR
#define DECODE
#define ENCODE
#ifdef __GNUC__
#include "mstring.h"
#include "winpr.cpp"
#else
#define no8087
#include <windows.h>
#include <string.h>
#include <stdio.h>

#endif

#endif
#include "mdef.h"


#ifdef __GNUC__
//#define debug printf
#define debug(a,b...)

inline int FNCATRIBUTE IMULIDIV(int a,int b,int c)
{
  asm volatile(
     "imull %1\n"
"      idivl %2\n"
"     ":"=&a"(a)
      : "0"(a),"g"(b),"g"(c)
      : "%edx"
     );
  return a;
};
inline int FNCATRIBUTE MULDIV(uint a,uint b,uint c)
{
  asm volatile(
     "mull %2\n"
"      divl %3\n"
"     ":"=&a" (a)
      : "0"(a), "m" (b), "m" (c)
      :"%edx"
      );
  return a;
};
#else

#pragma pack(1)
#define __attribute__(a)
//#define debug printf
inline void debug(char *a,...){}

inline int MULDIV(uint a,uint b,uint c)
{
 register int r;
  _asm{
     mov eax,a
     mul b
     div c
     mov r,eax
   };
 return r;
};

#endif

struct UnlimitedRange
{ulong  BlkSize;
 ulong  FreqOffs;
 ulong  SrcSize;
 ulong  BaseDivider;
 ulong CountOfBytes;
 ulong CountOfBits;
 ushort FreqBits;
 ushort FreqShift;

 ulong  Numerator[1];
 void   Init(int *s,int Base);
 inline void FNCATRIBUTE  AddBit(uint) ;
 inline uint FNCATRIBUTE  GetBit(uint) ;
 ulong FNCATRIBUTE Decode(uchar *s,int step);
 ulong FNCATRIBUTE Encode(uchar *s,int l,int step);
} __attribute__ ((packed));


inline void FNCATRIBUTE  UnlimitedRange::AddBit(uint b)
{
  if( ! (CountOfBits=(CountOfBits+1)&31) )Numerator[++CountOfBytes]=0;
  Numerator[CountOfBytes]|=b<<(31-CountOfBits);
};

inline uint FNCATRIBUTE  UnlimitedRange::GetBit(uint n)
{  return  ((Numerator[n>>5])>>(31-(0x1Ful&n)))&1;
};


#ifdef ENCODE

ulong FNCATRIBUTE UnlimitedRange::Encode(uchar *os,int l,int step)
{
 //register
 uchar *s;
 ulong c,i,j,r,rng,low,high,simb,range;
 ulong sum_freq[258];
 ulong max_freq;
 s=os;
 low=0;
 high=0x7FFFffff;
 CountOfBytes=0;
 CountOfBits=0;
 SrcSize=BaseDivider=l/step;
 FreqShift=0;
// if(BaseDivider<700) goto lbJustCopy;


 memset(sum_freq,0,sizeof(sum_freq));
 memset(Numerator,0,SrcSize+1024);

 max_freq=0;
 for(i=0;i<l;i+=step)
 { ++ (sum_freq[s[i]+1]);
 }
 for(i=0;i<256;i++)if(sum_freq[i]>max_freq)max_freq=sum_freq[i];
 j=0;
 while(BaseDivider>0x8000
       || max_freq>0x2000
      )
 {
  j=(j<<1)|1;
  BaseDivider=(BaseDivider+1)>>1;
  max_freq=(max_freq+1)>>1;
  ++FreqShift;
 }

 sum_freq[0]=(sum_freq[0]+j)>>FreqShift;
 for(i=1;i<=256;++i)
 {
  if(j)sum_freq[i]=(sum_freq[i]+j)>>FreqShift;
  sum_freq[i]+=sum_freq[i-1];
 }
 BaseDivider=sum_freq[256];
 debug("ZZZZ: %u %u %u| %u %u\n",l,BaseDivider,max_freq,sum_freq[0],sum_freq[1]);

 for(;l>0;l-=step)
 {
  range=high-low-1;
  simb=*s; s+=step;
//  high=low + MULDIV(sum_freq[simb+1],range,BaseDivider); // -1;
  low+=MULDIV(sum_freq[simb],range,BaseDivider);
  high=low + MULDIV(sum_freq[simb+1]-sum_freq[simb]-1,range,BaseDivider); // -1;
  while(!((low^high)&0x40000000) )
//  while((high-low)<BaseDivider  )
  { AddBit((low>>30)&1);
    low<<=1;  high=((high<<1)+1); };
//    low<<=1; low&=0x7FFFffff; high=((high<<1)+1)&0x7FFFffff; };
 }
 low+=(high+1-low)/2;
 while(low){AddBit((low>>30)&1);  low<<=1;}

 c=1;
 while(max_freq>>c)++c;
 FreqBits=c;
 FreqOffs=CountOfBytes*32+CountOfBits+1;
 debug("c=%u max_freq=%u BaseDivider=%u\n",c,max_freq,BaseDivider);

// for(i=0;i<=256;++i)debug("%X,",sum_freq[i]);

 for(i=1;i<=256;++i)
 { r=sum_freq[i]-sum_freq[i-1];
//  debug("%u|",sum_freq[i-1]);
//*
  if( (j=BaseDivider-sum_freq[i-1])<max_freq)
  {c=1;
   while(j>>c)++c;
//   debug("%u-%u(%u) ",i,c,j);

  }
  if(((int)j)<=0)
  {
//   debug("Total c=%d i=%u FreqOffs=%u\n",c,i,FreqOffs);
   break;
  }
// */
   debug("%2.2x ",r);
   for(j=0;j<c;++j)AddBit((r>>j)&1);
 }
 debug("\n\n");
 i=CountOfBytes*4;
 if(i>=SrcSize)
 {
//lbJustCopy:
  register uchar *t,*te;
  s=os;
  FreqOffs=0;
  te=(t=(uchar*)&BaseDivider) + SrcSize;
  while(t<te){*t++=*s; s+=step;}
  i=SrcSize-offset(UnlimitedRange,BaseDivider);
 }

 return BlkSize=i+sizeof(*this)+4;

};
#endif
//---------

#ifdef DECODE
ulong FNCATRIBUTE UnlimitedRange::Decode(uchar *t,int step)
{
 ulong c,i,j,r,low,high,n,range,k,max_freq,x;
// static
 ulong sum_freq[258];
 if(!FreqOffs)
 {k=0;
  j=SrcSize*step;
  register uchar *kk=(uchar *)&BaseDivider;
  for(i=0;i<j;i+=step)t[i]= *kk++;
 }
 else
 {

// sum_freq[0]=0;
 max_freq=1<<(c=FreqBits);
 n=FreqOffs;
 memset(sum_freq,0,sizeof(sum_freq));
 for(i=1;i<=256;++i)
 {
//*
  if( (j=BaseDivider-sum_freq[i-1])<max_freq)
  {c=1;
   while(j>>c)++c;
  }
  if(((int)j)<=0)
  {
//   debug("Total c=%d i=%u FreqOffs=%u\n",c,i,FreqOffs);
   sum_freq[i]=sum_freq[i-1];
   continue;
   //break;
  }
// */
   r=0;
   for(j=0;j<c;++j)r|=GetBit(n++)<<j;
   debug("%2.2x ",r);
   sum_freq[i]=r+sum_freq[i-1];
//   debug("%x=%x,",i,r);
 };
 debug("\n\n");
// for(i=0;i<=256;++i)debug("%X,",sum_freq[i]);

 low=0;
 high=0x7FFFffff;
 n=32;
 c=Numerator[0];

 for(j=SrcSize;j>0;j--)
 {
  range=high-low-1;// +1;
  r=MULDIV(c+1 ,BaseDivider,range);
  for(i=1; (r>=sum_freq[i])&&(i<256);++i);
//  for(i=1; (c>=MULDIV(sum_freq[i],range,BaseDivider) )&&(i<256);++i);
  *t=--i; t+=step;
//  high=low + MULDIV(sum_freq[i+1],range,BaseDivider); // -1;
  low+=r=MULDIV(sum_freq[i],range,BaseDivider);
  high=low + MULDIV(sum_freq[i+1]-sum_freq[i]-1,range,BaseDivider); // -1;
  c-=r;
  while( !((low^high)&0x40000000) )
//  while((high-low)<BaseDivider  )
  { c=((c<<1)|GetBit(n++))&0x7FFFffff;   low=low<<1; high=(high<<1)+1;

  };
 }
 }
 return SrcSize;
}

#endif
////////////////////////////////////////
