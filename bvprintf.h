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


#ifndef BVPRINTF_H
#define BVPRINTF_H

#include <stdarg.h>

extern "C"
{

#define BFR_LIM 0x800


typedef int (*PrintFlush)(void *,char *,ulong);

struct BFILE
{
 void *par;
 char *bfr,*t;
 PrintFlush Flush;
 void  Init(void *p,PrintFlush f,char *b ){par=p; Flush=f; bfr=t=b;  }
 int   bvprintf(const char *fmt,
#ifdef USEVALIST
                va_list v
#else                
                void **v
#endif                
               );
 int   bprintf(const char *fmt,...)
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
     
 };
 int fflush()
 { int r;
   if( (r=t-bfr) )
   { r=Flush(par,bfr,r);
     t=bfr;
   }
   return r;
 }
 
};

int  mvsprintfchk(char *t,char *et,const char *fmt,
#ifdef USEVALIST
                va_list v
#else                
                void **v
#endif                
);
int msprintfchk(char *t,char *et,const char *fmt,...);
/*
inline int msprintfchk(char *t,char *et,const char *fmt,...)
{
#ifdef USEVALIST
   va_list a;
   int r;
   
   va_start(a, fmt);
   r=mvsprintfchk(t,et,fmt,a);
   va_end(a);
   return r;
#else     
   return mvsprintfchk(t,et,fmt,(void **) ((&fmt)+1 ) ) ;
#endif     
        
    
}

*/
}

#endif
