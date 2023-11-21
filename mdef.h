/*
 * Copyright (C) 1999-2023 Maksim Feoktistov.
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

#if __SIZEOF_INT__ == 4 &&  __SIZEOF_POINTER__ == 8

#ifndef A_64
#define A_64 1
#endif

#endif

#if defined(__BYTE_ORDER__) &&  __BYTE_ORDER__  ==  __ORDER_BIG_ENDIAN__
#ifndef BIG_ENDIAN
#define BIG_ENDIAN 1
#endif
#else
#undef BIG_ENDIAN
#endif


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

#ifdef A_64
#define ulong unsigned int
#else
#define ulong unsigned long
#endif

#define u_int   unsigned int
#define u_short unsigned short
#define u_char  unsigned char
#define u_long  unsigned long

typedef unsigned int       u32;
typedef unsigned short     u16;
typedef unsigned char      u8 ;
typedef unsigned long long u64;

typedef int       i32;
typedef short     i16;
typedef char      i8 ;
typedef long long i64;

#undef LOWORD
#undef HIWORD

#ifdef BIG_ENDIAN

#define HIWORD(a) ( (a)&0xFFFFul)
#define LOWORD(a) ((ulong)(a)>>16)
#define MAKE_DWORD(b,a) (((ulong)(a)<<16)|(b))
#define MAKE_DWORD1(b,a) (((ulong)(a)<<16)|((b)&0xFFFF))

#else

#define LOWORD(a) ( (a)&0xFFFFul)
#define HIWORD(a) ((ulong)(a)>>16)
#define MAKE_DWORD(a,b) (((ulong)(a)<<16)|(b))
#define MAKE_DWORD1(a,b) (((ulong)(a)<<16)|((b)&0xFFFF))

#endif

#define MIN(a,b)  ( ((a)<(b))?(a):(b) )
#define MAX(a,b)  ( ((a)>(b))?(a):(b) )



#ifdef A_64
#define LODWORD(a) ((uint) ((ulong long) (a)) )
#define HIDWORD(a) ((uint) (((ulong long)(a))>>32) )

//#define PACKED __attribute__((packed, aligned(1)))
//#define PACKED __attribute__((__packed__))
#define PACKED __attribute__((packed))

#else
#define PACKED __attribute__((packed))

#endif



#if defined(AT_ARM) || defined(ARCH_REQ_ALIGN)

#ifdef __cplusplus

struct OffsetBugW
{
 uchar *t;
 OffsetBugW(void *c){t=(uchar *) c;}
 OffsetBugW & operator=(short c)
 {
   *t=*(uchar *)(&c);
   t[1]=((uchar *)(&c))[1];
   return *this;
 }
 operator ushort()
 {
#ifdef BIG_ENDIAN
   return
       (((uint)(t[1])) ) |
       (((uint)(t[0])) <<8  ) ;
#else
   return
       (((uint)(t[0])) ) |
       (((uint)(t[1])) <<8  ) ;
#endif
 }
};
struct OffsetBugD
{
  uchar *t;
  OffsetBugD(void *c){t=(uchar *) c;}
  OffsetBugD & operator=(int c)
  {
#ifdef BIG_ENDIAN
    *t=c>>24;
    t[1]=c>>16;
    t[2]=c>>8;
    t[3]=c;
#else
    *t=c;
    t[1]=c>>8;
    t[2]=c>>16;
    t[3]=c>>24;
#endif
  return *this;
 }
 operator int()
 {
#ifdef BIG_ENDIAN
   return
       (((uint)(t[3]))      ) |
       (((uint)(t[2])) <<8  ) |
       (((uint)(t[1])) <<16 ) |
       (((uint)(t[0])) <<24 ) ;
#else
   return
       (((uint)(t[0]))      ) |
       (((uint)(t[1])) <<8  ) |
       (((uint)(t[2])) <<16 ) |
       (((uint)(t[3])) <<24 ) ;
#endif
 }
 OffsetBugD & operator+=(int c)
 {
#ifdef BIG_ENDIAN
    c+= (((uint)(t[3]))      ) |
        (((uint)(t[2])) <<8  ) |
        (((uint)(t[1])) <<16 ) |
        (((uint)(t[0])) <<24 );
    *t=c>>24;
    t[1]=c>>16;
    t[2]=c>>8;
    t[3]=c;
#else
    c+= (((uint)(t[0]))      ) |
        (((uint)(t[1])) <<8  ) |
        (((uint)(t[2])) <<16 ) |
        (((uint)(t[3])) <<24 );
    *t=c;
    t[1]=c>>8;
    t[2]=c>>16;
    t[3]=c>>24;
#endif
  return *this;
 }

 OffsetBugD & operator^=(int c)
 {
#ifdef BIG_ENDIAN
    *t^=c>>24;
    t[1]^=c>>16;
    t[2]^=c>>8;
    t[3]^=c;
#else
    *t^=(c);
    t[1]^=c>>8;
    t[2]^=c>>16;
    t[3]^=c>>24;
#endif
    return *this;
 }

};

#define WORD_PTR(a)   ((OffsetBugW)( (void *) & (a) ))
#define DWORD_PTR(a)  ((OffsetBugD)( (void *) & (a) ))

#else  // ! __cplusplus

#ifdef BIG_ENDIAN

inline ulong fDWORD_PTR(uchar *t)
{ return
       (((uint)(t[3]))      ) |
       (((uint)(t[2])) <<8  ) |
       (((uint)(t[1])) <<16 ) |
       (((uint)(t[0])) <<24 ) ;
}

inline ushort fWORD_PTR(uchar *t)
{ return
       (((uint)(t[1]))      ) |
       (((uint)(t[0])) <<8  ) ;
}

#else  // LITTLE_ENDIAN

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
#endif
#define WORD_PTR(a)  fWORD_PTR((uchar *)  & (a))
#define DWORD_PTR(a) fDWORD_PTR((uchar *) & (a))

#endif  // __cplusplus


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

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)  (sizeof(a)/sizeof(a[0]) )
#endif


