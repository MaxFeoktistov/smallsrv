/*
 * Copyright (C) 1999-2022 Maksim Feoktistov.
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


#define __gnuc_va_list  va_list
#define __restrict
#define __const const
#define __need_timeval
//typedef void * ptrdiff_t;
#define _FILE_OFFSET_BITS  64
#define S_ADDR s_addr

#if defined __GNUC__  && __GNUC__ < 5
#include <features.h>
#endif

#undef __USE_FORTIFY_LEVEL
#define __USE_FORTIFY_LEVEL 0


#include <sys/types.h>



#if (!defined(ANDROID)) && !defined(LPC_ARM) && !defined(AT_ARM) && defined __GNUC__  && __GNUC__ < 5
#include <_G_config.h>
#endif

#if __USE_FORTIFY_LEVEL != 0
#error "Bad chk functions will be added to the file"  
#undef __USE_FORTIFY_LEVEL
#define __USE_FORTIFY_LEVEL 0
#endif

#include "mdef.h"
#include "mstring1.h"

#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include <sys/socket.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <sys/un.h>



#ifdef USE_POOL
#include <poll.h>
#endif

#ifdef FIX_EXCEPT
#include <setjmp.h>
#endif

#if __USE_FORTIFY_LEVEL != 0
#error "Bad _chk functions will be added to the file"  
#undef __USE_FORTIFY_LEVEL
#define __USE_FORTIFY_LEVEL 0
#endif

//   pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wunused-result"
#ifndef AT_ARM
#pragma GCC diagnostic ignored "-Wliteral-suffix"
#endif
//#pragma GCC diagnostic ignored "-fpermissive"
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"

#define CloseHandle  close
#define closesocket  close
#define ioctlsocket  ioctl
#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

extern "C"{

void Sleep(uint ms_time);
#if 1
#define THREADHANDLE pthread_t
#define FHANDLE int

inline pthread_t MyCreateThread( void * ( *ThreadFunction)(void *), void *par )
{
  pthread_t result;
//  pthread_attr_t __attr;
//  pthread_attr_init(&__attr);
//  pthread_attr_setscope ( &__attr, PTHREAD_SCOPE_PROCESS);
//  pthread_attr_setinheritsched( &__attr, PTHREAD_EXPLICIT_SCHED);
  pthread_create(&result,0 //&__attr
   ,ThreadFunction,par);
  //pthread_detach(result);
//  pthread_attr_destroy(&__attr);
  return result;
}
#define  CreateThread(SecuretyAttr,StackSize,ThreadFunction,ParamToThread,Flag,pid) MyCreateThread( (void * ( *)(void *)) ThreadFunction,ParamToThread)
#else
pthread_t MyCreateThread(int StackSize, void * ( *ThreadFunction)(void *), void *par );
#define  CreateThread(SecuretyAttr,StackSize,ThreadFunction,ParamToThread,Flag,pid) MyCreateThread(StackSize, (void * ( *)(void *)) ThreadFunction,ParamToThread)
#endif

#define  GetCurrentThreadId  pthread_self

#define _hwrite  write
#define _hread   read
#define _lopen(a,b)   open(a,(b)|O_CLOEXEC)
#define _llseek  lseek
#define _llseek64 lseek
#define _lcreat(a,b)  open(a, O_CREAT|O_WRONLY|O_TRUNC|O_CLOEXEC, 0600)
#define _lcreat2(a,b)  open(a, O_CREAT|O_WRONLY|O_TRUNC|O_CLOEXEC, 0600)
#define _lclose  close
#define OF_WRITE  (O_WRONLY|O_APPEND|O_CLOEXEC)
#ifdef x86_64
typedef void * HANDLE;
#else
typedef int HANDLE;
#endif

int IsAssess(char *fname,int uid,int gid,int type,char *uname=0);


#define WINAPI
#define DWORD ulong
//#define GetTickCount()  (time(0)<<10)
ulong GetTickCount();

int RESelect1(long tv_sec,long tv_usec,int s);
int RESelect2(long tv_sec,long tv_usec,int s,int s2);
#define RESelect(a,b,c,d) RESelect1(a,b,d)
#ifdef USE_POOL
int REPool(ulong tv_msec,int s,int pflg=POLLIN);
#endif

struct WIN32_FIND_DATA
{
  char dir[128];
  dirent //d,
   *pd;
  struct stat st;
//  inline ulong FileSizeHigh(){return (sizeof(st.st_size)>4)? ((ulong *)&st.st_size)[1]:0; }
  inline ulong&  FileSizeHigh(){return ((ulong *)&st.st_size)[1]; }
};

int FindNextFile(HANDLE h,WIN32_FIND_DATA *fd);
HANDLE FindFirstFile(char *p,WIN32_FIND_DATA *fd);

#define FindClose(h) if(h) closedir((DIR *)h);
#define cFileName pd->d_name
#define dwFileAttributes st.st_mode
#define nFileSizeLow st.st_size
#define nFileSizeHigh FileSizeHigh()
//#define nFileSizeHigh ( (sizeof(st.st_size)>4)? ((ulong *)&st.st_size)[1]:0 )
#define ftLastWriteTime  st.st_mtime
#define ftLastAccessTime st.st_atime
#define ftCreationTime   st.st_ctime

#define FileTime2time(a) (a)
#define  FILE_ATTRIBUTE_DIRECTORY S_IFDIR
#define  INVALID_HANDLE_VALUE ((HANDLE)-1)

//#define  GetTickCount() time(0)*1024

typedef  struct{ struct tm ttm;  /*char tmpbuf[64] ;*/ }  SYSTEMTIME;

#ifdef ARM
inline void FileTimeToSystemTime(void *tx,SYSTEMTIME *tt)
{
 //memcpy(tt,gmtime(tx),sizeof(*tt));
  gmtime_r((time_t *) tx,(struct tm *)tt);
}
#else
inline void FileTimeToSystemTime(time_t *tx,SYSTEMTIME *tt)
{
 //memcpy(tt,gmtime(tx),sizeof(*tt));
  gmtime_r(tx,(struct tm *)tt);
}

#endif

inline void GetSystemTime(SYSTEMTIME *t)
{time_t x=time(0);
  //memcpy(t,gmtime(&x),sizeof(*t));
  gmtime_r(&x,(struct tm *)t);
};
inline void GetLocalTime(SYSTEMTIME *t)
{time_t x=time(0);
 localtime_r(&x,(struct tm *)t);
 //memcpy(t,localtime(&x),sizeof(*t));
};
#define GetSystemTimeAsFileTime(a) *a=time(0)

#define  FILETIME time_t
#ifdef x86_64
void GetFileTime(int h,FILETIME *a,FILETIME *b,FILETIME *c);
#else
//void GetFileTime(HANDLE h,FILETIME *a,FILETIME *b,FILETIME *c);
void GetFileTime(int h,FILETIME *a,FILETIME *b,FILETIME *c);
#endif

#ifdef _BSD_VA_LIST_
#define FileTimeToLocalFileTime(a,b) (*(b)=*(a))

#define s6_addr8  __u6_addr.__u6_addr8
#define s6_addr16 __u6_addr.__u6_addr16
#define s6_addr32 __u6_addr.__u6_addr32

#define atoll(a) strtoll(a,0,10)
#else
#define FileTimeToLocalFileTime(a,b) (*(b)=*(a) - (timezone))
#endif

//#define MoveFile(a,b) rename(a,b)
inline int MoveFile(char *a,char *b){ return !rename(a,b); }
#define DeleteFile(a) remove(a)

char * Malloc(int c);
#define HeapAlloc(a,b,c) Malloc(c)
#define HeapFree(a,b,c) free(c)
#define HeapCompact(a,b)



inline void* HeapReAlloc1(void *c,int d)
{ register void *t;
  if(!(t=realloc(c,d)))
  {if(!(t=malloc(d))) return 0;
   memcpy(t,c,d);
   free(c);
  };
  return t;
}
#define HeapReAlloc(a,b,c,d) HeapReAlloc1(c,d)



#define wSecond    ttm.tm_sec
#define wDayOfWeek ttm.tm_wday
#define wDay       ttm.tm_mday
#define wMonth     ttm.tm_mon+1
#define wYear      ttm.tm_year+1900
#define wHour      ttm.tm_hour
#define wMinute    ttm.tm_min





#define secat 0770
#define CreateDirectory(a,b) mkdir(a,0770)
#define SetCurrentDirectory chdir
#define RemoveDirectory  rmdir

int FileSize(int i);
#define GetFileSize(a,b) FileSize(a)
int GetFileAttributes(char *);

//#define  MessageBox(a,b,c,d) fprintf(stderr,"%s > %s\n",b,c);
#define  MessageBox(a,b,c,d) printf("%s > %s\n",b,c);
#define  GetCommandLine() __argv[0]

#define Shell_NotifyIcon(a,b)   ;
#define DestroyWindow(mwnd)
#define DestroyIcon(hicon)
#define WSAStartup(a,b)
#define WSACleanup()
#define ExitProcess exit

int SetServ(uint fnc);
ulong SMTPcl(void *);

}

inline void * operator new(size_t c){return Malloc(c);}
inline void * operator new[](size_t c){return Malloc(c);}
inline void   operator delete(void *t){free(t);}
inline void   operator delete(void *t,uint x){free(t);}
inline void   operator delete(void *t,unsigned long x){free(t);}
inline void   operator delete[](void *t){free(t);}


//#define debug2(a,b...)  fprintf(stderr,a,##b );
//#define SOCKLEN_T
//(socklen_t *)

#define getpeername(a,b,c) getpeername(a,b,(socklen_t *) (c))
#define getsockname(a,b,c) getsockname(a,b,(socklen_t *) (c))
#define accept(a,b,c) accept(a,b,(socklen_t *) (c))
#define recvfrom(a,b,c,d,e,f) recvfrom(a,b,c,d,e,(socklen_t *) (f))
#undef send
#define send(a,b,c,d) send(a,(void *)(b),c,d)



#define WSAGetLastError() errno
#define SetEndOfFile(fs)
extern char **__argv;

//#define debug(a,b...) {GetProt(); pprot+=sprintf(pprot,a, ## b); printf(a, ## b);  RelProt();}
//#define debug(a,b...)  printf(a, ## b)

//#define SYSUNIX
#define FSLUSH '/'
#define FSLUSHS "/"

#define FORLINUX " for UNIX"
//s_FOR_UNIX

extern char *oldprot;
#define SER "%s"
#define Xstrerror(a)  ,strerror(a)
#define sdns6 (soc_srv[6])
#define sdnst6 (soc_srv[7])
#ifdef _BSD_VA_LIST_
extern pthread_t dnstthr;
#define  FNCATRIBUTE
#else
#define  FNCATRIBUTE __attribute__ ((section (".stext")))
#endif

#ifdef LPC_ARM
#undef fork
#define fork vfork
#endif


#define dprint(a...) 
//printf(a)
#define DBGLINE   
//dprint("%s:%u\r\n",__FILE__,__LINE__);

#define MIN_PTR 0x80000


#ifdef USE_FUTEX

#include <sys/syscall.h>

#if  defined(ARM9)  || defined(AT_ARM)
// #include <linux/futex.h> 
// #define STRING2(x) #x
// #define STRING(x) STRING2(x)
// #pragma message "SYS_futex = " STRING(SYS_futex)
// #pragma message "FUTEX_WAIT = " STRING(FUTEX_WAIT)   

#ifndef SYS_futex
#define SYS_futex 240
#endif

#ifndef FUTEX_WAIT
#define FUTEX_WAIT  0
#define FUTEX_WAKE  1
#endif

#pragma GCC diagnostic ignored "-Wnarrowing"

#else

 #include <linux/futex.h>

#endif
inline int futex(int *uaddr, int futex_op, int val,
             const struct timespec *timeout, int *uaddr2, int val3)
{
   return syscall(SYS_futex, uaddr, futex_op, val,
                   timeout, uaddr, val3);
}

#endif
