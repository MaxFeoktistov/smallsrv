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

#ifndef MSTR_H
#define MSTR_H

#undef uint  
#undef ushort
#undef uchar 
#undef ulong
#undef u_int
#undef u_short
#undef u_char 
#undef u_long 

#define uint   unsigned int
#define ushort unsigned short
#define uchar  unsigned char

#ifdef x86_64
#define ulong unsigned int
#else
#define ulong unsigned long
#endif

#define u_int   unsigned int
#define u_short unsigned short
#define u_char  unsigned char
#define u_long  unsigned long

#undef LOWORD
#undef HIWORD
#define MIN(a,b)  ( ((a)<(b))?(a):(b) )
#define MAX(a,b)  ( ((a)>(b))?(a):(b) )
#define LOWORD(a) ( (a)&0xFFFFul)
#define HIWORD(a) ((ulong)(a)>>16)
#define MAKE_DWORD(a,b) (((ulong)(a)<<16)|(b))
#define MAKE_DWORD1(a,b) (((ulong)(a)<<16)|((b)&0xFFFF))

#if __SIZEOF_INT__ == 4 &&  __SIZEOF_POINTER__ == 8
//sizeof(int) == 4

#define LODWORD(a) ((uint) ((ulong long) (a)) )
#define HIDWORD(a) ((uint) (((ulong long)(a))>>32) )

#endif

#ifdef x86_64
//#define PACKED __attribute__((packed, aligned(1)))
//#define PACKED __attribute__((__packed__))
#define PACKED __attribute__((packed))

#else
#define PACKED __attribute__((packed))

#endif

#if defined(AT_ARM) 

#ifdef __cplusplus


//#define DWORD_PTR(a) ( (uint)(uchar * )(&(a))[1]<<8ul)|( (uint)(uchar * )(&(a))[2]<<16ul)|((uint) (uchar * )(&(a))[3]<<24ul)|(* (uchar * )(&(a)) )

//#define WORD_PTR(a) ( (uint)(uchar * )(&(a))[1]<<8ul) | (* (uchar * )(&(a)) )


struct OffsetBugW
{
 uchar *t;
 OffsetBugW(void *c){t=(uchar *) c;}
 OffsetBugW & operator=(short c)
 {*t=*(uchar *)(&c);
  t[1]=((uchar *)(&c))[1];
  return *this;
 }
 operator ushort()
 {
   return
       (((uint)(t[0])) ) |
       (((uint)(t[1])) <<8  ) ;
 }
};
struct OffsetBugD
{
 uchar *t;
 OffsetBugD(void *c){t=(uchar *) c;}
 OffsetBugD & operator=(int c)
 {
/*   
   *t=*(uchar *)(&c);
  t[1]=((uchar *)(&c))[1];
  t[2]=((uchar *)(&c))[2];
  t[3]=((uchar *)(&c))[3];
  */
  *t=c;
  t[1]=c>>8;
  t[2]=c>>16;
  t[3]=c>>24;
  return *this;
 }
 operator int()
 { return
       (((uint)(t[0]))      ) |
       (((uint)(t[1])) <<8  ) |
       (((uint)(t[2])) <<16 ) |
       (((uint)(t[3])) <<24 ) ;
 }
 OffsetBugD & operator+=(int c)
 {c+=  (((uint)(t[0]))      ) |
       (((uint)(t[1])) <<8  ) |
       (((uint)(t[2])) <<16 ) |
       (((uint)(t[3])) <<24 );
  *t=c;
  t[1]=c>>8;
  t[2]=c>>16;
  t[3]=c>>24;
  return *this;
 }

 OffsetBugD & operator^=(int c)
 {
  *t^=(c);
  t[1]^=c>>8;
  t[2]^=c>>16;
  t[3]^=c>>24;
  return *this;
 }

};

#define WORD_PTR(a)   ((OffsetBugW)( & (a) ))
#define DWORD_PTR(a)  ((OffsetBugD)( &  (a) ))

#else  // ! __cplusplus
inline ulong fDWORD_PTR(uchar *t)
{ return
       (((uint)(t[0]))      ) |
       (((uint)(t[1])) <<8  ) |
       (((uint)(t[2])) <<16 ) |
       (((uint)(t[3])) <<24 ) ;
}

inline ushort fWORD_PTR(uchar *t)
{ return
       (((uint)(t[0]))      ) |
       (((uint)(t[1])) <<8  ) ;
}
#define WORD_PTR(a)  fWORD_PTR((uchar *)  & (a))  
#define DWORD_PTR(a) fDWORD_PTR((uchar *) & (a)) 

#endif  // __cplusplus

//dfsfs
//#define WORD_PTR(a)    ((* (ushort *)(&(a)) )

#else


#define DDWORD_PTR(a)   (* (unsigned long long *)(&(a)) )
#define DWORD_PTR(a)   (* (ulong *)(&(a)) )
#define WORD_PTR(a)    (* (ushort *)(&(a)) )
#define WORD_PTRn(a,n) (* ((ushort *)(&(a)) + n) )
#define HI_WORD_PTR(a) (* ((ushort *)(&(a)) + 1) )
#define CONST4CHAR(a0,a1,a2,a3)  (const ulong ) ( (  (ulong)(a0)     )|\
                                                  ( ((ulong)(a1))<<8 )|\
                                                  ( ((ulong)(a2))<<16)|\
                                                  ( ((ulong)(a3))<<24) )
#define C4CHAR(a)                (const ulong ) ( (  (ulong)(a[0])     )|\
                                                  ( ((ulong)(a[1]))<<8 )|\
                                                  ( ((ulong)(a[2]))<<16)|\
                                                  ( ((ulong)(a[3]))<<24) )
#define CONST2CHAR(a0,a1)  (const ushort) ( (  (ulong)(a0)     )|\
                                            ( ((ulong)(a1))<<8 )


#endif

#define BYTE_PTR(a,n)  (* ((uchar *)(&(a))+n) )
#define x4CHAR(a)
#define xC4CHAR(a)
#define offset(a,b)  ((long)&(((a *)0)->b))

#endif

