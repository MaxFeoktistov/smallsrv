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




#ifdef NOTINTEL


inline void XCHG(void *a,void *b)
{
 long c;
 c=*(long *)a;
 *(long *)a=*(long *)b;
 *(long *)b=c;
}
#define xchg(a,b)  XCHG(&(a),&(b))

inline int MULDIV(uint a,uint b,uint c)
{
  return  (long long )a * (long long) b / c ;
}

inline int IMULDIV(int a,int b,int c)
{
  return  (long long )a * (long long) b / c ;
}

inline ulong rol(ulong a,int b){
  b&=0x1F;
  return (a<<b)|(a>>(32-b));
}


inline ulong ror(ulong a,int b){
  b&=0x1F;
  return (a>>b)|(a<<(32-b));
}


#ifdef __cplusplus                      

inline ulong DIVMODLL(ulong a,ulong b,ulong d,
                      ulong &m)
{
  union {
    ulong l[2];
    long long ll;
  } ;
  l[0]=a;  l[1]=b;
  m= ll%d;
  return ll/d;
}
#endif                      


#else
inline int intel16(int a)
{
  asm volatile("xchgb %%ah,%%al":"=&a" (a)
                 :"0" (a)
//                 :"%eax"
  );
  return a;
}
inline int intel32(int a)
{
  asm volatile("xchgb %%ah,%%al\n"
"                roll  $16,%%eax\n"
"                xchgb %%ah,%%al\n"
"               ":"=&a" (a)
                :"0" (a)
//                 :"%eax"
               );
  return a;
}

inline void XCHG(void *a,void *b)
{
  asm volatile("movl  (%%eax),%%ecx\n"
"                xchgl (%%edx),%%ecx\n"
"                movl  %%ecx, (%%eax)\n"
"               ":"=&a" (a),"=&d" (b)
                :"0" (a),"1" (b)
                :"%ecx"
              );
};

inline int MULDIV(uint a,uint b,uint c)
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
inline int IMULDIV(int a,int b,int c)
{
  asm volatile(
     "imull %2\n"
"      idivl %3\n"
"     ":"=&a" (a)
      : "0"(a), "m" (b), "m" (c)
      :"%edx"
      );
  return a;
};
#define xchg(a,b)  XCHG(&a,&b)
#undef htonl
#undef htons
#define htonl intel32
#define htons intel16

inline ulong rol(ulong a,int b){
 asm volatile("\n"
"   roll  %%cl,%%eax\n"
"":"=&a"(a),"=&c"(b)
 :"0"(a),"1"(b)
 );
 return a;
};
inline ulong ror(ulong a,int b){
 asm volatile("\n"
"   rorl  %%cl,%%eax\n"
"":"=&a"(a),"=&c"(b)
 :"0"(a),"1"(b)
 );
 return a;
}

#ifdef __cplusplus
inline ulong DIVMODLL(ulong a,ulong b,ulong d,ulong &m)
#else
inline ulong DIVMODLL(ulong a,ulong b,ulong d,ulong *m)
#endif
{ asm volatile("\n"
"     divl %%ecx\n"
"   ":"=&a" (a), "=&d" (m) , "=&c" (d)
    :"0" (a), "1"(b), "2"(d)
   );
 return a;
}

#endif
