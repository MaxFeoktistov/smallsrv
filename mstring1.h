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


#ifndef _string_h_
#define _string_h_


#ifdef NOTINTEL
#include <string.h>

#define stristr strcasestr
#define stricmp strcasecmp

inline char *memchrc(void *a,int b,int c){return (char *) memchr(a,b,c); }
#define memchr memchrc

inline int strin(char *a, char *b)
{
 while(*b && *a==*b){a++; b++;  }
 return !*b ;
}


inline int striin(char *a, char *b)
{
 while(*b && ! (((*a ^ *b) & ~0x20)) ){a++; b++;}
 return !*b ;
}

#if (!defined(ANDROID)) && !defined(LPC_ARM) && !defined(AT_ARM) && !defined(x86_64)
inline void *  memcpy_back(void *s1, void *s2, size_t _n)
{
  while(_n--)
  { * ((char *)s1)--= * ((char *)s2) --;  }
  return s1;
}

inline unsigned int * memchr4(const void *_s,int _c,size_t _n)
{
 while(_n--)
 {
   if( *( int *)_s ==_c ) return (unsigned int *) _s;
   ((int *)_s)++;
 }
 return 0;
}
#else


inline void *  memcpy_back(char *s1, char *s2, size_t _n)
{
  while(_n--)
  { *s1--= *s2--;  }
  return s1;
}

inline unsigned int * memchr4(const unsigned int *_s,unsigned int _c,size_t _n)
{
 while(_n--)
 {
   if( *_s ==_c ) return (unsigned int *) _s;
   _s++;
 }
 return 0;
}



#endif

#define strinnc striin

#else
#include <sys/types.h>

#define _INC_STRING 1

#ifndef NULL
#define NULL 0
#endif
void tsttfnc();

#define CHECK_EDIESIECX    " test %%edi,%%edi ; jz 5f ; test %%esi,%%esi ; jz 5f ;  jecxz 5f ;"   
#define CHECK_ESIECX       " test %%esi,%%esi ; jz 5f ; jecxz 5f ;"   
#define CHECK_EDIECX       " test %%edi,%%edi ; jz 5f ; jecxz 5f ;"   

#define  ENDCHK   "\n5: \n"


inline char *  memchr(const void *_s, int _c, size_t _n)
{
  asm volatile(
      " test %%edi,%%edi ; jz 2f ; "
  " jecxz 2f;\n"
"       cld;  repne; scasb;\n"
"       je 1f;\n"
"       2: movl $1,%%edi;\n"
"       1: decl %%edi"
       :"=&D" (_s),"=&c" (_n),"=&a" (_c)
       :"0" (_s), "1"(_n),"2" (_c)
                   //  :"%edi","%ecx"
                     );
  return (char *)_s;
};
//--------------------------------------------------------------------
inline int     memcmp(const void *_s1, const void *_s2, size_t _n)
{
  asm volatile("jecxz 1f;\n"
"       cld;  repe;cmpsb;\n"
"       movzbl -1(%%edi),%%ecx\n"
"       movzbl -1(%%esi),%%eax\n"
"       subl  %%eax,%%ecx\n"
"       1:;"
       :"=&c" (_n),"=&D" (_s1),"=&S"(_s2)
                     :"0" (_n), "1" (_s1),"2" (_s2)
                     :"%eax"
                     //:"%edi","%esi","%ecx"
                     );
  return _n;
};
//--------------------
inline int memicmp(const void *_s1, const void *_s2, size_t _n)
{
  asm volatile("jecxz 1f;\n"
"       cld\n"
"       movb   $0,%%al\n"
"       jecxz  1f\n"
"       decl %%edi\n"
"   2:  incl %%edi\n"
"       lodsb\n"
"       xorb (%%edi),%%al\n"
"       loope 2b\n"
"       jecxz  1f\n"
"       xorb $0x20,%%al\n"
"       jz 2b\n"
"     1:\n"
"       movsbl %%al,%%eax\n"
"       "
       :"=&a" (_n),"=&D" (_s1),"=&S"(_s2)
       :"c" (_n), "1" (_s1),"2" (_s2)
                     //:"%edi","%esi","%ecx"
     );
  return _n;
};
//--------------------
inline void *  memcpy(void *_s1, const void *_s2, size_t _n)
{
  asm volatile(
      "test %%edi,%%edi ; jz 1f ; test %%esi,%%esi ; jz 1f ;  jecxz 1f ;"   
      "cld; rep; movsb;\n"
"1:                    ":
                     "=&D" (_s2),"=&S" (_s2),"=&c" (_n)
                     :"0" (_s1),"1" (_s2),"2" (_n)
                    // :"%edi","%esi","%ecx"
                     );
  return _s1;
};
inline void *  memcpy_back(void *_s1, const void *_s2, size_t _n)
{
  asm volatile(
           " test %%edi,%%edi ; jz 1f ; test %%esi,%%esi ; jz 1f ;  jecxz 1f ;"    
           "std; rep; movsb;\n"
           "cld ; \n"
"1:                    "
                     :"=&D" (_s1),"=&S" (_s2),"=&c" (_n)
                     :"0" (_s1),"1" (_s2),"2" (_n)
                    // :"%edi","%esi","%ecx"
                     );
  return _s1;
};
//-------------------
inline void *  memcpy_rl(void *_s1, const void *_s2, size_t _n)
{
  asm volatile("cld; rep; movsb;\n"
"                    ":"=&D" (_s1),"=&S" (_s2),"=&c" (_n)
                     :"0" (_s1),"1" (_s2),"2" (_n)
                    // :"%edi","%esi","%ecx"
                     );
  return _s1;
};
//#define memmove(s1,s2,n) memcpy(s1,s2,n)
//------------------
inline void *  memset(void *_s, int _c, size_t _n)
{
   asm volatile("test %%edi,%%edi ; jz 1f ;  jecxz 1f ; cld;  rep ; stosb;\n1: ;\n":"=&D" (_s),"=&c" (_n),"=&a" (_c)
       :"0" (_s),"1" (_n),"2" (_c)
//     :"%edi","%ecx"
   );
  return _s;
};
#define bzero(s,l) memset(s,0,l)
//-------------------
char *  strcat(char *_s1, const char *_s2);
char *  strchr(const char *_s, int _c);
//------
inline char *  rstrchr(const char *_s, int _c)
{
  asm volatile(
      " test %%edi,%%edi ; jz 5f ; "   
      "std\n"
"    2: scasb\n"
"       je  1f\n"
"       cmpb $0,1(%%edi)\n"
"       jne  2b\n"
"    5: movl $-1,%%edi\n"
"    1: incl %%edi\n"
"       cld;       "
       :"=&D" (_s),"=&a" (_c)
       :"0" (_s),"1" (_c)
//       :"%edi","%eax"
      );
  return (char *) _s;
};
//-------
int     strcmp(const char *_s1, const char *_s2);
//-----------
#define  strcmpnc stricmp
int     stricmp(const char *_s1, const char *_s2);
//-----------
#define  strcoll(s1,s2) strcmp(s1,s2)
//-----------
inline char *  strcpy(char *_s1, const char *_s2)
{
  asm volatile(
   " test %%edi,%%edi ; jz 5f ; test %%esi,%%esi ; jz 5f ;"      
      "cld;\n"
"     1:lodsb\n"
"       stosb\n"
"       orb %%al,%%al\n"
"       jnz 1b\n"
"5:\n"
       :"=&D" (_s1),"=&S" (_s2)
       :"0" (_s1),"1" (_s2)
       :"%eax"
//       :"%edi","%esi","%eax"
       );
  return _s1;
};
//-------------
inline char *  strcpy_rl(char *_s1, const char *_s2)
{
  asm volatile("cld;\n"
"     1:lodsb\n"
"       stosb\n"
"       orb %%al,%%al\n"
"       jnz 1b\n"
       :"=&D" (_s1),"=&S" (_s2)
       :"0" (_s1),"1" (_s2)
       :"%eax"
//       :"%edi","%esi","%eax"
     );
  return _s1;
};
//----------
inline size_t  strcspn(const char *_s1, const char *_s2)
{
 register char * res=(char *) _s1;
 while((*res) && (!strchr(_s2,*res)) )res++;
 return res-_s1;
};
//----------

//#define strerror( _errcode)  ""
#ifdef __cplusplus
extern "C" {
#endif    
 char * strerror(int errnum);
#ifdef __cplusplus
}
#endif
//----------
inline size_t  strlen(const char *_s)
{
 int _x;
  asm volatile("cld;\n"
"       xorb %%al,%%al;\n"
"       xorl %%ecx,%%ecx;\n"
"       test %%edi,%%edi ; jz 5f \n"   
"       decl %%ecx;\n"
"       repne; scasb;\n"
"       incl %%ecx;\n"
"       notl %%ecx ;\n"
"5: \n"
       :"=c" (_s),"=&D" (_x)
       :"1" (_s)
       :"%eax"
 //      :"%eax","%edi"
  );
  return (int)_s;
};
//--------
inline char *  strncat(char *_s1, const char *_s2, size_t _n)
{
  asm volatile(
      " test %%edi,%%edi ; jz 5f ; test %%esi,%%esi ; jz 5f  ;\n"   
        "cld; movl $-1,%%ecx;\n"
"       xor %%al,%%al;\n"
"       repne; scasb;\n"
"       decl %%edi;\n"
"       lea  1(%%edx),%%ecx;\n"
"      1:\n"
"       lodsb;\n"
"       stosb;\n"
"       orb %%al,%%al;\n"
"       loopnz 1b\n"
"       movb $0,-1(%%edi)\n"
"5:     \n    "
       :"=&D" (_s2),"=&S" (_s2),"=&d" (_n)
       :"0" (_s1),"1" (_s2),"2" (_n)
       : //"%edi",%esi",
       "%ecx", "%eax"

       );
  return _s1;
};
//----------
inline char *  strncat_rl(char *_s1, const char *_s2, size_t _n)
{
  asm volatile("cld; movl $-1,%%ecx;\n"
"       xor %%al,%%al;\n"
"       repne; scasb;\n"
"       decl %%edi;\n"
"       lea  1(%%edx),%%ecx;;\n"
"      1:\n"
"       lodsb;\n"
"       stosb;\n"
"       orb %%al,%%al;\n"
"       loopnz 1b\n"
"       movb $0,-1(%%edi) "
       :"=&D" (_s1),"=&S" (_s2),"=&d" (_n)
       :"0" (_s1),"1" (_s2),"2" (_n)
       : //"%edi","%esi",
       "%eax","%ecx"
       );
  return _s1;
};
//--------------------------------------------------------------------
inline int  strncmp(const char *_s1, const char *_s2, size_t _n)
{
  asm volatile(
      " test %%edi,%%edi ; jz 1f ; test %%esi,%%esi ; jz 1f ; "   
      " cld; jecxz 1f;\n"
"    3: lodsb; scasb; jne 2f;\n"
"       orb %%al,%%al\n"
"       loopnz 3b;\n"
"       jecxz  1f;\n"
"    2: movl -1(%%edi),%%ecx\n"
"       subl -1(%%esi),%%ecx\n"
"    1:;"
    :"=&c" (_n),"=&D" (_s1),"=&S" (_s2)
    :"1" (_s1),"2" (_s2),"0" (_n)
    : //"%edi","%esi",
    "%eax"
    //,"%ecx"
);
  return _n;
};
//--------------------------------------------------------------------
inline char *  strncpy(char *_s1, const char *_s2, size_t _n)
{
  asm volatile(
  "jecxz 5f \n"   
  "cld; incl %%ecx ;\n"
"     1:lodsb\n"
"       stosb\n"
"       orb %%al,%%al\n"
"       loopnz 1b;\n"
"     2:movb $0,-1(%%edi) \n"
"5: \n"
   :"=&D" (_s2),"=&S" (_s2),"=&c" (_n)
   :"0" (_s1),"1" (_s2),"2" (_n)
   ://"%edi","%ecx","%esi",
   "%eax"
   );
  return _s1;
};
//--------------------------------------------------------------------
inline char *  strncpy_rl(char *_s1, const char *_s2, size_t _n)
{
  asm volatile("cld; jecxz 2f;\n"
"     1:lodsb\n"
"       stosb\n"
"       orb %%al,%%al\n"
"       loopnz 1b;\n"
"       2:\n"
      :"=&D" (_s1),"=&S" (_s2),"=&c" (_n)
      :"0" (_s1),"1" (_s2),"2" (_n)
      ://"%edi","%ecx","%esi",
     "%eax"
  );
  return _s1;
};
//--------------------------------------------------------------------
char *  strpbrk(const char *_s1, const char *_s2);
char *  strrchr(const char *_s, int _c);
//--------------------------------------------------------------------
inline size_t  strspn(const char *_s1, const char *_s2)
{
 register char * res=(char *) _s1;
 while((*res) && (strchr(_s2,*res)) )res++;
 return res-_s1;
};
//--------------------------------------------------------------------
inline int strin(const char *_s1, const char *_s2)
{
  asm volatile("cld; movl $1,%%ecx;\n"
"    3: lodsb;\n"
"       orb %%al,%%al\n"
"       jz 1f;\n"
"       scasb;\n"
"       je 3b;\n"
"       decl   %%ecx\n"
"    1:;\n"
     :"=c" (_s1),"=&D" (_s2),"=&S" (_s2)
     :"1" (_s1),"2" (_s2)
     ://"%edi","%esi",
     "%eax"
     );
  return (int)_s1;
};
//--------*/
#define striin strinnc
int strinnc(const char *_s1, const char *_s2);
//--------*/
inline int strin1(const char *_s1, const char *_s2)
{
  asm volatile("cld; movl $-1,%%ecx;\n"
"       repe; cmpsb;\n"
"       xor   %%ecx,%%ecx\n"
"       cmpb  %%cl,-1(%%esi)\n"
"       sete  %%cl;\n"
"    1:;\n"
    :"=c" (_s1),"=&D" (_s2),"=&S" (_s2)
    :"1" (_s1),"2" (_s2)
    ://"%edi","%esi",
    "%eax"
);
  return (int)_s1;
};
//-------

char *  strstr(const char *_s1, const char *_s2);
#define  strstrnc stristr
char *  stristr(const char *_s1, const char *_s2);

#define  strxfrm  strncpy

inline unsigned char * memfnd4(unsigned char *s,unsigned long a, int c)
{
 asm volatile("\n"
"     cld\n"
"   1:\n"
"     scasl\n"
"     je  2f\n"
"     subl $3,%%edi\n"
"     loop 1b\n"
"     movl $4,%%edi\n"
"   2:\n"
"     subl $4,%%edi\n"
"     ":"=&D"(s),"=&a"(a),"=&c"(c)
      :"0"(s),"1"(a),"2"(c)
    );
  return s;
};
//----
inline int arstrlen(const char *_s)
{int _x;
 asm volatile("cld;\n"
"   xorb %%al,%%al;\n"
"   xorl %%ecx,%%ecx;\n"
"   decl %%ecx;\n"
" 1:repne; scasb;\n"
"   cmpb %%al,(%%edi)\n"
"   jne  1b\n"
"   incl %%ecx;\n"
"   notl %%ecx;\n"
"   incl %%ecx;\n"
" ":"=c" (_s) ,"=&D" (_x)
  :"1" (_s)
  :"%eax"
 );
 return (int)_s;
};
inline unsigned int * memchr4(const void *_s,int _c,size_t _n)
{asm volatile("jecxz 2f;\n"
"  cld; repne;scasl;\n"
"  je 1f;\n"
"  2: movl $4,%%edi;\n"
"  1: subl $4,%%edi":"=&D" (_s),"=&c" (_n),"=&a" (_c)
   :"0" (_s),"1" (_n),"2" (_c)
 );
 return (unsigned int *)_s;
};


#endif

#ifndef _STRING_H
#define _STRING_H
#endif
#define _STRING_H_  1
#define __STRING_H  1


#endif
