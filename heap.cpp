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

//g++ heap.cpp -DTESTMEM -o testt

#define   MEM_USED  0x80000000
//#define   DBG_SIGN 0x55aa1122
#include "mdef.h"
// #define   DBGMEM

struct  Heap
{
 ulong size;
 ulong d[0];

 void *Alloc(ulong s);
 void Free(void *p);
 int Compact();
 void Init(ulong s){size=s-2; d[0]=--s; d[s]=s|MEM_USED; };


void  TestOut();
void  CheckMem();

};

void memset4(ulong *t,ulong v,ulong l)
{
 register ulong *b=t;
 t+=l;
 while(b<t){*b++=v;}
}

 void *Heap::Alloc(ulong s)
 {
   ulong i,j;
   s=(s+
#ifdef DBGMEM
   11
#else
   7
#endif
     )>>2;

//   CheckMem();
   do{
    for(i=0;i<size;i+=d[i]&~MEM_USED)
    {
     if( d[i]>=s && ! (d[i]&MEM_USED) )
     {
      if(d[i]>s)
      {
        j=i+s;
        d[j]=d[i]-s;
        d[i]=s;
      }
      d[i]|=MEM_USED;
#ifdef DBGMEM
   d[i+s-1]=DBG_SIGN;
   memset4(d+i+1,0,s-2);
#else
   memset4(d+i+1,0,s-1);
#endif



      return d+i+1;
     }
    }
   }while(Compact()>=s);
   return 0;
 };

int Heap::Compact()
{
   ulong i,j,r;

   r=0;

   for(i=0;i<size;i+=d[i]&~MEM_USED)if(!(d[i]&MEM_USED))
   {
     j=i+d[i];
     while(!(d[j]&MEM_USED))
     {
       d[i]+=d[j];
       j+=d[j];
       if(j>=size)break;
     }
     if(r<d[i])r=d[i];
   }
   return r;
}


 void Heap::Free(void *p)
 {
   ulong i,j;
   i=((ulong *)p) - d;
   i--;
   if(i<size)
   {
     j=i+(d[i]&=~MEM_USED);
#ifdef DBGMEM
   if( d[j-1]!=  DBG_SIGN )
   {
     debug("Memory error: %u size %u addr %X %.128s",i,d[i]&~MEM_USED,d+i+1,d+i+1 );
     return ;
   }
#endif

     while(!(d[j]&MEM_USED))
     {
       d[i]+=d[j];
       j+=d[j];
       if(j>=size)break;
     }

   }
 };


 /*

  Init(100); ->

  size=100



*/

#ifdef DBGMEM


void  Heap::CheckMem()
{
 int i;
   for(i=0;i<size;i+=d[i]&~MEM_USED)
     if( (d[i]&MEM_USED) && d[i+d[(i&~MEM_USED)] -1 ] != DBG_SIGN )
   {
     //printf("%u -> %u %X\n",i,d[i]&~MEM_USED,d[i]);
     debug("Detected memory error: %u size %u addr %X %.128s",i,d[i]&~MEM_USED,d+i+1,d+i+1 );
   }
}

#endif

#ifdef TESTMEM

#include <stdio.h>


void  Heap::TestOut()
{
 int i;
printf("\n\nHeap out\n");
   for(i=0;i<size;i+=d[i]&~MEM_USED)
   {
     printf("%u -> %u %X\n",i,d[i]&~MEM_USED,d[i]);
   }
}


union{
 Heap hp;
 ulong bbb[200];
} u;

int main()
{
 ulong *a[5];
 int i,j;

  u.hp.Init(100);
  u.hp.TestOut();
  for(i=0;i<5;++i)
  {
    a[i]=(ulong *) u.hp.Alloc(5+i);
    u.hp.TestOut();
  }

  u.hp.Free(a[2]);
  u.hp.TestOut();
  u.hp.Free(a[3]);
  u.hp.TestOut();
  u.hp.Compact();
 // u.hp.Free(a[1]);
   u.hp.TestOut();
  a[4]=(ulong *) u.hp.Alloc(30);
    u.hp.TestOut();

      return 0;

}



#endif

