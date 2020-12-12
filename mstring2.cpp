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

/* by M. Feoktistov (C) 1999  */



#include "mstring1.h"


#define NULL 0

char *  strcat(char *_s1, const char *_s2)
{
    asm volatile("cld;
       movl $-1,%%ecx;
       xor %%al,%%al;
       repne;scasb;
       decl %%edi;
      1:
       lodsb
       stosb
       orb %%al,%%al
       jnz 1b"
       :"=&D" (_s2),"=&S" (_s2)
       :"0" (_s1),"1" (_s2)
       :"%ecx","%eax"
       //"%edi","%ecx","%esi","%eax"
       );
  return _s1;
};
//------
char *  strrchr(const char *_s, int _c)
{
  asm volatile("cld;
       xorb %%ah,%%ah;
       xorl %%edx,%%edx;
    2: cmp  (%%edi),%%ah;
       je  3f;
       scasb;
       jne 2b;
       lea -1(%%edi),%%edx;
       jmp 2b;
    3:"
    :"=d" (_s),"=&D" (_c),"=&a" (_c)
    :"1" (_s),"2" (_c)
  //  :"%edi","%eax"
);
  return (char *)_s;
};
//--------*/
char *  strstr(const char *_s1, const char *_s2)
{
 asm volatile("cld
    2: movl %%edx,%%esi
       movl %%ecx,%%edi
    3: lodsb
       orb  %%al,%%al
       jz   1f
       scasb
       je  3b
       incl %%ecx
       cmpb $0,(%%ecx)
       jne  2b
       xorl %%ecx,%%ecx
    1:
    ":"=&c"(_s1),"=&d"(_s2)
     :"0"(_s1),"1"(_s2)
     :"%edi","%esi","%eax"
  );
 return (char *) _s1;
}



