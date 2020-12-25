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


#include "mstring1.h"


#define NULL 0
#ifdef MINGWDLL
#define  CDECL  _cdecl
#else
#define CDECL
#endif


char * CDECL  strcat(char *_s1, const char *_s2)
{
    asm volatile("cld;\n"
"       movl $-1,%%ecx;\n"
"       xor %%al,%%al;\n"
"       repne;scasb;\n"
"       decl %%edi;\n"
"      1:\n"
"       lodsb\n"
"       stosb\n"
"       orb %%al,%%al\n"
"       jnz 1b"
       :"=&D" (_s2),"=&S" (_s2)
       :"0" (_s1),"1" (_s2)
       :"%ecx","%eax"
       //"%edi","%ecx","%esi","%eax"
       );
  return _s1;
};
//--------------------------------------------------------------------
char * CDECL strchr(const char *_s, int _c)
{
  asm volatile("cld;\n"
"    2: scasb;\n"
"       je  1f\n"
"       cmpb $0,-1(%%edi)\n"
"       jne  2b\n"
"       movl  $1,%%edi;\n"
"    1: decl %%edi;"
      :"=&D" (_s),"=&a" (_c)
      :"0" (_s),"1" (_c)
//    :"%edi","%eax"
    );
  return (char *) _s;
};
//------
int   CDECL  strcmp(const char *_s1, const char *_s2)
{
  asm volatile("cld;\n"
"    3: lodsb; scasb; jne 2f;\n"
"       orb %%al,%%al\n"
"       jnz 3b;\n"
"    2: movb -1(%%edi),%%al\n"
"       subb -1(%%esi),%%al\n"
"       movsbl  %%al,%%eax\n"
"    1:;"
    :"=a" (_s1),"=&D" (_s2),"=&S" (_s2)
    :"1" (_s1),"2" (_s2)
   // :"%edi","%esi","%eax"
    );
  return (int)_s1;
};
//--------------------------------------------------------------------
#define  strcmpnc stricmp
int  CDECL   stricmp(const char *_s1, const char *_s2)
{
  asm volatile("cld;\n"
"    3: lodsb; scasb; jne 2f;\n"
"       orb %%al,%%al\n"
"       jnz 3b;\n"
"    2: xor $0x20,%%al\n"
"       cmp -1(%%edi),%%al\n"
"       je  3b\n"
"       xor $0x20,%%al\n"
"       movb -1(%%edi),%%al\n"
"       subb -1(%%esi),%%al\n"
"       movsbl  %%al,%%eax\n"
"    1:;"
    :"=a" (_s1),"=&D" (_s2),"=&S" (_s2)
    :"1" (_s1),"2" (_s2)
//    :"%edi","%esi","%eax"
    );
  return (int)_s1;
};
//--------------------------------------------------------------------
#define  strcoll(s1,s2) strcmp(s1,s2)
//--------------------------------------------------------------------
char * CDECL strpbrk(const char *_s1, const char *_s2)
{
   asm volatile("cld;\n"
"       movl %%edx,%%edi\n"
"       xorl %%eax,%%eax;\n"
"       movl $-1,%%ecx;\n"
"       repne; scasb;\n"
"       lea  1(%%ecx),%%ebx\n"
"       notl %%ebx;\n"
"    3:\n"
"       lodsb\n"
"     //  shrdl $32,%%eax,%%edi\n"
"       or   %%al,%%al\n"
"       jnz   2f\n"
"       xorl  %%esi,%%esi\n"
"       jmp   1f\n"
"    2:\n"
"       movl %%edx,%%edi\n"
"       movl %%ebx,%%ecx\n"
"       repne; scasb;\n"
"       jne  3b\n"
"       decl %%esi;\n"
"    1:"
    :"=&S" (_s1),"=&d" (_s2)
    :"0"(_s1),"1" (_s2)
    :"%ecx","%ebx","%edi","%eax"
  );
  return (char *)_s1;

}
//--------------------------------------------------------------------
char * CDECL strrchr(const char *_s, int _c)
{
  asm volatile("cld;\n"
"       xorb %%ah,%%ah;\n"
"       xorl %%edx,%%edx;\n"
"    2: cmp  (%%edi),%%ah;\n"
"       je  3f;\n"
"       scasb;\n"
"       jne 2b;\n"
"       lea -1(%%edi),%%edx;\n"
"       jmp 2b;\n"
"    3:"
    :"=d" (_s),"=&D" (_c),"=&a" (_c)
    :"1" (_s),"2" (_c)
  //  :"%edi","%eax"
);
  return (char *)_s;
};
//--------------------------------------------------------------------
int CDECL strinnc(const char *_s1, const char *_s2)
{
  asm volatile("cld; movl $1,%%ecx;\n"
"    3: lodsb;\n"
"       orb %%al,%%al\n"
"       jz 1f;\n"
"       scasb; je 3b;\n"
"       xorb $0x20,%%al\n"
"       cmpb -1(%%edi),%%al\n"
"       je  3b\n"
"       decl   %%ecx\n"
"    1:;"
    :"=c" (_s1),"=&D" (_s2),"=&S" (_s2)
    :"1" (_s1),"2" (_s2)
    ://"%edi","%esi",
    "%eax"
    );
  return (int)_s1;
};
//--------*/
char * CDECL strstr(const char *_s1, const char *_s2)
{
 asm volatile("cld\n"
"    2: movl %%edx,%%esi\n"
"       movl %%ecx,%%edi\n"
"    3: lodsb\n"
"       orb  %%al,%%al\n"
"       jz   1f\n"
"       scasb\n"
"       je  3b\n"
"       incl %%ecx\n"
"       cmpb $0,(%%ecx)\n"
"       jne  2b\n"
"       xorl %%ecx,%%ecx\n"
"    1:\n"
"    ":"=&c"(_s1),"=&d"(_s2)
     :"0"(_s1),"1"(_s2)
     :"%edi","%esi","%eax"
  );
 return (char *) _s1;
}

#define  strstrnc stristr
char * CDECL stristr(const char *_s1, const char *_s2)
{
 asm volatile("cld\n"
"    2: movl %%edx,%%esi\n"
"       movl %%ecx,%%edi\n"
"    3: lodsb\n"
"       orb  %%al,%%al\n"
"       jz   1f\n"
"       scasb\n"
"       je  3b\n"
"       xorb $0x20,%%al\n"
"       cmpb -1(%%edi),%%al\n"
"       je  3b\n"
"\n"
"       incl %%ecx\n"
"       cmpb $0,(%%ecx)\n"
"       jne  2b\n"
"       xorl %%ecx,%%ecx\n"
"    1:\n"
"    ":"=&c"(_s1),"=&d"(_s2)
     :"0"(_s1),"1"(_s2)
     :"%edi","%esi","%eax" );
 return (char *) _s1;
}


