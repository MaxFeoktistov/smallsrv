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



#include <stdarg.h>


#include "mstring1.h"
//#include "mstring.h"
#include "srv.h"
#include "mdef.h"

#include "bvprintf.h"

extern "C"
{

struct SprintfPars
{
  char *t;
  ulong l;
};

int SPrintChkFlush(SprintfPars *th,char *t,ulong l)
{
 if(th->l < l)l=th->l;
 if(l>0)
 {
   memcpy(th->t,t,l);
   th->l-=l;
   th->t+=l;
 }
 return l;
};

int  mvsprintfchk(char *t,char *et,const char *fmt,
    #ifdef USEVALIST
                va_list v
#else
                void **v
#endif
)
{
  SprintfPars th;
  BFILE bf;
  int r;
  char  b[BFR_LIM*2];
  th.t=t;
  th.l=et-t-1;
  bf.Init(&th,(PrintFlush)SPrintChkFlush,b);
  r=bf.bvprintf(fmt,v);
  bf.fflush();
  *(th.t)=0;
  return r;
}

int msprintfchk(char *t,char *et,const char *fmt,...)
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

int msprintf(char *t, const char *fmt,...)
{
#ifdef USEVALIST
   va_list a;
   int r;

   va_start(a, fmt);
   r = mvsprintfchk(t, t + 0x10000, fmt, a);
   va_end(a);
   return r;
#else
   return mvsprintfchk(t, t + 0x10000, fmt, (void **) ((&fmt)+1 ) ) ;
#endif
}

}

