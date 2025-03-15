/*
 * Copyright (C) 1999-2021 Maksim Feoktistov.
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


#ifndef BVPRINTF_H
#define BVPRINTF_H

#include <stdarg.h>

extern "C"
{

#define BFR_LIM 0x800

#if defined(x86_64) || defined(AT_ARM) || defined(DJGPP) || A_64
#define V_CDECL
#else
#define V_CDECL  __attribute__((cdecl))
#endif

typedef int (*PrintFlush)(void *,char *,ulong);

struct BFILE
{
 void *par;
 char *bfr,*t;
 PrintFlush Flush;
 void  Init(void *p,PrintFlush f,char *b ){par=p; Flush=f; bfr=t=b;  }



#ifdef USEVALIST
 int   bvprintf(const char *fmt, va_list v);
#define mva_list va_list
#else
 int   bvprintf(const char *fmt, void *v) ;
#undef  mva_list
#define mva_list void **
#endif

 int bprintf(const char *fmt,...) V_CDECL
#ifdef BPRINTF_INLINE
 {
#ifdef USEVALIST
   va_list a;
   int r;

   va_start(a, fmt);
   r=bvprintf(fmt,a);
   va_end(a);
   return r;
#else
   return bvprintf(fmt,(void **) ((&fmt)+1 )) ;
#endif

 }
#endif
 ;
 int fflush()
 { int r;
   if( (r=t-bfr) )
   { r=Flush(par,bfr,r);
     t=bfr;
   }
   return r;
 }

};

int mvsprintfchk(char *t,char *et,const char *fmt, mva_list v);
int msprintfchk(char *t,char *et,const char *fmt,...);
int msprintf(char *t, const char *fmt,...);

}

#endif
