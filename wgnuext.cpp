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

//gcc -c -S -O2 -fpack-struct wgnuext.cpp

struct ExtBlock
{
 int  Sign;
 void *beg;
 void *end;
};


struct SEHRec
{
 void *r1;
 void *esp;
 void *r2;
 void *ecept_list;
 void *handler;
 void *b;
 int Sign;

  inline void BegTry(void *hndlr);
  inline void EndTry();
};


int  pSign=-1;
inline void SEHRec::BegTry(void *hndlr)
{
 int t;
  asm volatile("  movl  %%esp,(%%eax)\n"
  //   movl  %%esp,8(%%eax)
     "     addl  $8,%%eax\n"
     "     movl  %%fs:0,%%ecx\n"
     "     movl  %%ecx,(%%eax)\n"
     "     movl  %%eax,%%fs:0\n"
                 :"=&a" (t)
                 :"0" (&esp)
                 ://"%eax",
                  "%ecx"
                 );
  ecept_list=0;
  handler=hndlr;
  b=&pSign;
  Sign=0;
};

inline void SEHRec::EndTry()
{Sign=-1;
  asm volatile("  movl  %%eax,%%fs:0 \n"
                 :"=&a" (ecept_list)
                 :"0" (ecept_list)
//                 :"%eax"
                 );

}

inline int except()
{
  asm volatile(
//   movl 4(%%esp),%%eax\n"
     "   movl 8(%%esp),%%ecx\n"
     "   leal 0x10(%%ecx),%%ebp\n"
                :
                :
                :"%ecx");
};

#if 0
int TstPrg(char *a, int b)
{
 SEHRec  stry;

 while(*a++)
 {
  stry.BegTry( &&lbErr);

  if(!b)break;
  --b;
 }
lbErr:
 return b;
};

#endif

