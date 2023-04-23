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
#ifdef  DJGPP 
#include "dgpp_quote.h"
#else
#define DJGPP_QUOTE
#endif
//#include "mstring.h"

inline void XCHG_s(char *a, char *b,int l )
{
#ifdef DJGPP
  asm volatile(
  DJGPP_QUOTE jecxz 2f;
   subl %%edi,%%edx
   cld
   1:
   movb  (%%edi),%%al
   xchgb %%al,(%%edi,%%edx,1)
   stosb
   loop 1b
   2: DJGPP_QUOTE
   :"=&D" (a),"=&c" (l),"=&d" (b)
   :"0" (a), "1"(l),"2" (b)
  );
#else
  asm volatile(
  " jecxz 2f \n"
   " subl %%edi,%%edx\n"
   " cld\n"
   "1:  \n"
   " movb  (%%edi),%%al\n"
   " xchgb %%al,(%%edi,%%edx,1)\n"
   " stosb\n"
   " loop 1b\n"
   " 2:\n"
   :"=&D" (a),"=&c" (l),"=&d" (b)
   :"0" (a), "1"(l),"2" (b)
  );
#endif

}

#ifndef DJGPP

typedef unsigned  int   size_t     ;

#endif

extern "C" {
void    qsort(void *base, size_t nelem, size_t size,
              int (*cmp)(const void *e1, const void *e2))
{
 int i,j,k;
 long *pos;
 char *tmp,*p;
 k=nelem-1;
#define E(a) (((char*)base)+(a)*size)
 for(i=0;i<k;++i)
 {
  for(j=i+1;j<nelem;++j)
  {
    if(cmp(E(i),E(j))>0)XCHG_s(E(i),E(j),size);
  }
 }
};

}
