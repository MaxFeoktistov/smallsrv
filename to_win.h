/*
 * Copyright (C) 1999-2021 Maksim Feoktistov.
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

#ifndef MINGW64
#define _WINDOWS_
#else
#ifndef _X86_
#define _X86_
#endif
#define __CRT__NO_INLINE 1
#endif


//#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wnarrowing"
#pragma GCC diagnostic ignored "-Wliteral-suffix"


#ifdef  DJGPP 
typedef void* HKEY;
#endif 
#include <sys/types.h>
#include <ctype.h>
#include "mstring1.h"
#ifdef MINGW64
#include <winsock2.h>
#endif
#include <windows.h>


#define fd_set win_fd_set
typedef struct fd_set {
  int  fd_count;       /* how many are SET? */
  int  fd_array[64];   /* an array of SOCKETs */
}fd_set;

#ifdef MINGW64

//#include <winsock2.h>
#include <winsock_IPv6.h>

#undef DJGPP

#else // Not MINGW64

#include <winsock.h>

#ifdef MINGW
#include <winsock_IPv6.h>
#undef DJGPP
#endif





#endif

#ifdef  DJGPP 
extern "C" {
int PASCAL __WSAFDIsSet(int, fd_set *);
};
#endif

//#ifndef MINGW64


#include <shellapi.h>
#include <shlobj.h>
#include <commdlg.h>
#include <winsvc.h>

//#endif


#define sprintf wsprintf
#define vsprintf wvsprintf

#include "mdef.h"
#include "mrc.h"

#undef FD_ZERO
#undef FD_SET
#undef FD_CLR
#undef FD_ISSET
#define FD_ZERO(set) ((set)->fd_count=0)
#define FD_SET(fd,set) ((set)->fd_array[(set)->fd_count++]=fd)
#define FD_ISSET(fd, set) __WSAFDIsSet((fd),(set))
//#define FD_CLR(fd, set) 

extern "C" {

int RESelect1(long tv_sec,long tv_usec,int s);
#ifdef SELECT1
#define RESelect(a,b,c,d) RESelect1(a,b,d)
//#define RESelect(a,b,c,d,e) RESelect2(a,b,d,e)
int RESelect2(long tv_sec,long tv_usec,int s1,int s2);

#else
int RESelect(long tv_sec,long tv_usec,int n,...);

inline int RESelect2(long tv_sec,long tv_usec,int s1,int s2)
{ int r=RESelect(tv_sec,tv_usec,2,s1,s2);
  if(r<=0)return r;
  return (RESelect(0,0,1,s1)>0)?s1:s2;
}

#endif


#ifdef SERVICE
extern SERVICE_STATUS_HANDLE sesh;
extern SERVICE_STATUS ServiceStatus;
#endif
extern mrc_obj dlg2[];
extern mrc_mnu mnu2[];
extern //const 
       mrc_mnu mnu3[];
extern mrc_mnu mnu4[];
extern const char *url_desc[];
extern char fnamebuf[512],hstflt[];
extern OPENFILENAME ofn;
extern BROWSEINFO binf;
extern HMENU hmnu,hmmnu;
extern HWND dwnd2,dwndc;

extern SECURITY_ATTRIBUTES secat;
extern NOTIFYICONDATA nid;
int MyCreateProcess(char *p,void *env,char *loc,STARTUPINFO *cb,PROCESS_INFORMATION *pi);
#define wnd_name (nid.szTip)

int WINAPI lopen (char *f,ulong t);
int WINAPI lcreat(char *f);
int WINAPI lread (int h,char *b,int n);
int WINAPI lwrite(int h,char *b,int n);
int WINAPI llseek(int h,int n,int t);

#define _lclose(h) CloseHandle((HANDLE)(h))
#define FileSize(h) GetFileSize((HANDLE)(h),0)

#define _lopen lopen
#define _lcreat(a,b) lcreat(a)
#define _lcreat2(a,b) lcreat2(a)
int WINAPI lcreat2(char *f);
#define _hread lread
#define _lread lread
#define _lwrite lwrite
#define _hwrite lwrite
#define _llseek llseek


long long _llseek64(int h,long long p, int t );


int WINAPI SetServ(uint fnc);
ulong WINAPI SMTPcl(void *);

void SelectInList(int n);
void* CurInList(int n,char *bfr);
typedef void* (*ListNext)(void *s,char *t);
void ReinitListBox(int n,ListNext fln,void *lst);
int IsDChk(HWND hwnd,int d);
int GetDlgItemTextLen(HWND hwnd,int d);
long CALLBACK DefProc(HWND hwnd,UINT msg,UINT wparam,LONG lparam);

//extern HANDLE hinstance;

//extern HINSTANCE hinstance;

extern HANDLE hheap;
extern HWND mwnd,ewnd;
extern STARTUPINFO cbFwd;
extern HANDLE htrd,hProcess;
extern ulong trd_id;
extern HICON  hicon;

#define SOCKLEN_T


#define FORLINUX
#define S_ADDR S_un.S_addr
int utf2unicode(uchar *s,ushort *cm);
uchar *   unicode2utf(uchar *utf8_buf,ushort *ucs4_buf, int l);



extern ulong _PerSecond1E7;
// ulong _PerSecond1E7=10000000;
//inline
#ifdef  DJGPP 

struct timeval
{ ulong tv_sec,tv_usec;};

#endif

inline ulong FileTime2time(FILETIME  &a)
{
 ulong r,t;
 
#ifdef  DJGPP 
 asm volatile(" subl  $0xFDE04000 ,%%eax
       sbbl  $0x14F373B ,%%edx
       cmpl  __PerSecond1E7,%%edx
       jae   1f
       divl __PerSecond1E7
       imul $51,%%edx
       shrl $9,%%edx
      1:
     ":"=&a"(r),"=&d"(t)
      :"0"(a.dwLowDateTime),"1"(a.dwHighDateTime)
    );
#else
 
 asm volatile(" subl  $0xFDE04000 ,%%eax \n"
      " sbbl  $0x14F373B ,%%edx \n"
      "  cmpl  __PerSecond1E7,%%edx \n"
      "  jae   1f \n"
      "  divl __PerSecond1E7 \n"
      "  imul $51,%%edx \n"
      "  shrl $9,%%edx \n"
      "1: \n"
      :"=&a"(r),"=&d"(t)
      :"0"(a.dwLowDateTime),"1"(a.dwHighDateTime)
    );

#endif 
    return r;
}


inline
void gettimeofday(struct timeval *x,...)
{
 SYSTEMTIME  stime;
 GetLocalTime(&stime);
 SystemTimeToFileTime(&stime, (FILETIME *)x);

//debug("TIME %X %X",x->tv_sec,x->tv_usec);
#ifdef  DJGPP 
 asm volatile(" subl  $0xFDE04000 ,%%eax
       sbbl  $0x14F373B ,%%edx
       cmpl  __PerSecond1E7,%%edx
       jae   1f
       divl __PerSecond1E7
       imul $51,%%edx
       shrl $9,%%edx
      1:
     ":"=&a"(x->tv_sec),"=&d"(x->tv_usec)
      :"0"(x->tv_sec),"1"(x->tv_usec)
    );
#else
 
 asm volatile(" subl  $0xFDE04000 ,%%eax \n"
    " sbbl  $0x14F373B ,%%edx\n"
    "        cmpl  __PerSecond1E7,%%edx\n"
    "        jae   1f\n"
    "        divl __PerSecond1E7\n"
    "        imul $51,%%edx\n"
    "        shrl $9,%%edx\n"
    "       1:\n"
     :"=&a"(x->tv_sec),"=&d"(x->tv_usec)
      :"0"(x->tv_sec),"1"(x->tv_usec)
    );
#endif 

}

inline
ulong  time(ulong *y)
{
 struct timeval x;
 gettimeofday(&x);
 if(y)*y=x.tv_sec;
 return x.tv_sec;
}

#define THREADHANDLE HANDLE
#define FHANDLE HANDLE

#define FSLUSH '\\'
#define FSLUSHS "\\"
#define socklen_t int

//#define SER ""
//#define Xstrerror(a)
char * strerror(int ErrorCode);
#define SER "%s"
#define Xstrerror(a)  ,strerror(a)
#undef errno
#define errno WSAGetLastError()
#define sdns6 (soc_srv[6+9])
#define sdnst6 (soc_srv[7+9])


#define MNU_CGI   10
#define MNU_VHOST 13
#define  FNCATRIBUTE

int GetFileAttributes_UTF(char *a);
HANDLE FindFirstFile_UTF(char  *   a, LPWIN32_FIND_DATA lpFindFileData);
int WINAPI lopen_UTF (char *f,ulong t);
int FindNextFile_UTF(HANDLE a, LPWIN32_FIND_DATA lpFindFileData);

#ifdef MINGW
extern void *id_heap;
inline void * WMALLOC(int n){return HeapAlloc(id_heap,HEAP_ZERO_MEMORY,n);}
inline void * WREALLOC(void *p,int n){return  HeapReAlloc(id_heap,HEAP_ZERO_MEMORY,p,n); }
inline void   WFREE(void *p) { HeapFree(id_heap,0,p); }
#undef  malloc
#undef  Malloc
#undef  free
#define malloc WMALLOC
#define Malloc WMALLOC
#define free WFREE
inline void * operator new[](unsigned int s)
{
  return WMALLOC(s);
}
inline void operator delete[](void *a)
{
  WFREE(a);
}

inline void operator delete(void *a)
{
  WFREE(a);
}

inline void operator delete(void *a,unsigned int)
{
  WFREE(a);
}


#define atoll atoll1
long long atoll(const char *a);

#endif

extern char dprbuf[256];
extern int hstdout;
#define dprint(a...)  
//_hwrite(hstdout,dprbuf,wsprintf(dprbuf,a )) ; Sleep(10);
#define DBGLINE  
//dprint("%s:%u\r\n",__FILE__,__LINE__); 


#define MIN_PTR (void *)0x40000

#ifdef DJGPP
#define Malloc malloc
#endif
}

