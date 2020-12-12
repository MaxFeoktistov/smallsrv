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



#ifndef GZ_H
#define GZ_H


#if defined(MIPS) || defined(LPC_ARM)
#include "uclinux/zlib.h"
#else
#include <zlib.h>
#endif
#define ZBOSIZE 0x10000
#ifndef EXTERN
#define EXTERN extern
#endif

typedef int (* tgzfunc)(z_streamp strm, int flush);
typedef int (* tgzfunc2)(z_streamp strm);
typedef int (* tgzfunc3)(
//z_streamp strm, int flush,const char *version, int stream_size
z_streamp strm, int  level, int  method,
                                      int windowBits, int memLevel,
                                      int strategy, const char *version,
                                      int stream_size
);
typedef int (* tgzfunc4)(z_streamp strm,int,const char *version, int stream_size);

typedef int (* transfunc)(void *x,void *b, int l);

EXTERN tgzfunc  ZPack,ZUnpack;
EXTERN tgzfunc3 ZPackInit;
EXTERN tgzfunc2  ZPackEnd,ZUnpackEnd;
EXTERN tgzfunc4 ZUnpackInit;

EXTERN char *zlib;

#ifndef MINGW
#define HINSTANCE void *
//typedef  void *HINSTANCE; 
#endif

EXTERN HINSTANCE  hZlib;
EXTERN int zlibstate;
EXTERN char * NoGZSuf;
EXTERN uint  GZSizeLow;


struct GZObj
{
 z_stream z;
 tgzfunc  f;
 tgzfunc2  fe;
 transfunc tf;
 void *par;
 int   h2;
 ulong crc,totallen;
 char b[ZBOSIZE+8];
 char last[8];
 int  lflg;
 int  chustate,chul;
#define UseChuke  1
#define ChukeWaitLF  2
#define InChuke  4


 int Get(void *t,int l);
 int Put(const void *t,int l);
 void End();
 int InitPack(transfunc tff,void *pp);
 int InitUnpack(transfunc tff,void *pp);
 int ChuInit(char *x,int ll);
 //int InitUnpack(transfunc tff,void *pp){tf=tff; par=pp; f=ZUnpack; fe=ZUnpackEnd; (ZUnpackInit)(&z,"1",sizeof(z)); };
};

int GZSndSkip(Req *th,uchar *b,int l);

int InitZLib();
EXTERN const char gz_head[10+sizeof("Content-Encoding: gzip\r\n\r\n")-1 ];
#endif
