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

#define FRAME_SIZE (0x3fff-256)
#define CACHE_SIZE  0x80000
#define CASHE_SHIFT 6

inline void AddBits(uchar *t,int &c,int v,int n)
{
 DWORD_PTR(t[c>>3])|=v<<(c&7);
 c+=n;
};

int PackBits(uchar *t,uchar *s, int l)
{
 int c=0,i,n,j,m,msk;
 memset(t,0,l //*(CASHE_SHIFT+8)/16
 );
 for(i=0;i<l;++i)
 {
  j=(i/(FRAME_SIZE*2))*(FRAME_SIZE*2);
  if(j==i){n=0; m=0; msk=0;}
  if(!(i&511)) if(++m > msk){++n; msk=(msk<<1)|1;}
  AddBits(t,c,s[i],(i&1)?n:8);
 }
 return (c>>3)+5;
}

int LZEncode(ushort *t,uchar *s,int l)
{
 int i,j,k,n,xx;
 ulong c,cb,ch;
 uchar *cache;


  cache=new uchar[CACHE_SIZE];

 // t[0]=*s++;
 // t[1]=*s++;
  k=0;
  for(i=0;i<l;++i)
  {
    t[k++]=*s++;
  lbLp:

    //j=k-FRAME_SIZE;
    //if(j<0)j=0;

    j=(k/FRAME_SIZE)*FRAME_SIZE;
    if(j==k) memset(cache,0,CACHE_SIZE);

   if(k>=2)
   {
    ch=((t[k-1]+t[k-2])^(t[k-2]<<8))&(CACHE_SIZE-1);
    if( cache[ch] )
    {
     c=DWORD_PTR(t[k-2]);
     if( (n=t[k-2])<t[k-1] )n=t[k-1];
     if( (n-=256)<0 )n=0;
     if( (cb=((ulong)(cache[ch]-1))<<CASHE_SHIFT)>n )n=cb;
 #if 1
     xx=k-3;
     for(j+=n;j<xx;++j)
     {
      if(DWORD_PTR(t[j])==c)
      {
       t[--k-1]=(j%FRAME_SIZE)+256;
       goto lbLp;
      }
     }
 #else
     asm("
           movl  %%edi,%%edx
        0: scasl
           je   1f
           subl $2,%%edi
           loop 0b
           addl $4,%%edi
        1:
           leal  -4(%%edx),%%edi
           shrl  $1,%%edi
         ":"=D"(j)
          :"a"(c),"D"(t+j),"c"(k-3)
          :"%eax","%edi","%ecx","%edx"
        );
       if(j<(k-3))
       {
        t[--k-1]=(j%FRAME_SIZE)+256;
        goto lbLp;
       }

 #endif
    }
//    else
    cache[ch]=((k-2)>>CASHE_SHIFT)+1;
   }
  }

  j=PackBits(cache,(uchar *)t,k*2);
  memcpy(t+2,cache,j);
  DWORD_PTR(*t)=k;
  delete cache;
  return j;
//  return k<<1;
}
