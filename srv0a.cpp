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


 #ifndef STRING_CONST_H
 #include "g4strc.h"
 #endif

#ifdef ARM
#include <stdarg.h>

#endif

#ifndef SRV_H
#include "srv.h"
#endif

#include "srvars.cpp"

#ifndef SYSUNIX

int WINAPI lopen (char *f,ulong t){ return (int)CreateFile(f,(0xB0000000ul<<t)&0xC0000000,FILE_SHARE_READ,&secat,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);}
int WINAPI lcreat(char *f){return (int) CreateFile(f,GENERIC_WRITE,0,&secat,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,0);}
int WINAPI lcreat2(char *f){return (int) CreateFile(f,GENERIC_WRITE,FILE_SHARE_READ,&secat,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,0);}
int WINAPI lread (int h,char *b,int n){return ReadFile((HANDLE)h,b,n,(ulong *)&n,0)*n;}
int WINAPI lwrite(int h,char *b,int n){return WriteFile((HANDLE)h,b,n,(ulong *)&n,0)*n;}
int WINAPI llseek(int h,int n,int t){return SetFilePointer((HANDLE)h,n,0,t);}

#ifdef SERVICE
#include <winsvc.h>

SERVICE_STATUS_HANDLE sesh;
SERVICE_STATUS ServiceStatus=
{SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,
SERVICE_RUNNING,
SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN|SERVICE_ACCEPT_PAUSE_CONTINUE,
0,0,0,0
};
void WINAPI NThandler(DWORD fdwControl)
{if(fdwControl==SERVICE_CONTROL_STOP || fdwControl==SERVICE_CONTROL_SHUTDOWN)
 {ServiceStatus.dwCurrentState=SERVICE_STOP_PENDING;
  s_aflg|=AFL_EX2;
  PostMessage(mwnd,WM_CLOSE,0,0);
 }
 else if(fdwControl==SERVICE_CONTROL_PAUSE || fdwControl==SERVICE_CONTROL_CONTINUE)
 {if(fdwControl==SERVICE_CONTROL_PAUSE)ServiceStatus.dwCurrentState=SERVICE_PAUSED;
  PostMessage(mwnd,WM_USER,WM_LBUTTONUP,WM_LBUTTONUP);
 }
 SetServiceStatus(sesh,&ServiceStatus);
};
#endif
#endif

#ifdef USE_FUTEX
const struct timespec timeout_50ms={0,50000000};
#endif
void MyLock(volatile int &x){int a=(int) GetCurrentThreadId();
   int dead_lock_chk=128;
#if 1
#ifdef SYSUNIX
  // usleep(10);
  sched_yield();
#else
   Sleep(1);
#endif

   do{ while(x && x!=a && --dead_lock_chk>0)
#ifdef USE_FUTEX
    futex((int *)&x,FUTEX_WAIT,x,&timeout_50ms,0,0);  
#else      
    Sleep(50); 
#endif       
     x=a;
//   Sleep(0);
#ifdef SYSUNIX
   //usleep(100);
   sched_yield();
#else
   Sleep(1);
#endif
   }while(x!=a);
 //Sleep(0); do{ while(x){ Sleep(30);}  if(++x==1)break; --x; }while(1);
#else
 int b;
 while(x!=a){
  while(x)
  {
       Sleep(20); 
       if(x==a)goto ex2;
       if(--dead_lock_chk<0){ x=a; goto ex2; }  
  }
  if((b=InterlockedExchange((long *)&x,a)))x=b;
 }
 ex2:;
#endif
}

void MyUnlockOwn(volatile int &x){
  if(x==(int) GetCurrentThreadId())
  {
      x=0;
#ifdef USE_FUTEX
      futex((int *)&x,FUTEX_WAKE,1,0,0,0);  
#endif    
  }    
}

#ifndef SYSUNIX
ushort wb_prot[LOG_SIZE+0x1080];
void ShowProt()
{
 if( (s_flgs[2]&FL2_UTF)  && utf2unicode((uchar *)b_prot,wb_prot)>0)    
 {
    SetWindowTextW(ewnd,(LPCWSTR)wb_prot); 
 }    
 else SetWindowText(ewnd,b_prot);
 SendMessage(ewnd,EM_LINESCROLL,0,SendMessage(ewnd,EM_GETLINECOUNT,0,0)-4);
};

char err_msg[260];
char* strerror(int ErrorCode)
{
 FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK, 0, ErrorCode, 0,(LPSTR)err_msg, 256, 0);
 return err_msg;
}
#else
int ppid,puid,lastday;
#endif

void xdie(char *er)
{char buf1[200];
 sprintf(buf1,"%s\r\n %X",er,WSAGetLastError());
 MessageBox(0,buf1,"Error",MB_OK);
 return;
}

void RelProt(SYSTEMTIME *stime)
{int l,ll;
 SYSTEMTIME lTime;
 FILETIME CrTime1,CrTime;
 char *t;
#ifdef SYSUNIX
 if(!(s_flg&FL_NOICON))printf("%s",oldprot);
#endif
#ifndef CD_VER
 if(flog &&  ( (l=(pprot-f_prot)) >= unsave_limit ) )
 {
#ifndef SYSUNIX
  if( (ll=_lopen(flog,1))>0)
  {_llseek(ll,0,2);
#else
  if(puid!=geteuid()){
//printf("\nkill %d %d\n",puid,geteuid());
  kill(ppid,SIGUSR1); return;}

  if((ll=open(flog,O_APPEND|O_WRONLY))>0)
  {
#endif
   _hwrite(ll,f_prot,l);
   GetFileTime((FHANDLE) ll,&CrTime1,0,0 );
   _lclose(ll);
   if(s_flg&FL_LOGDAY)
   {
#ifndef SYSUNIX
    FileTimeToLocalFileTime(&CrTime1,&CrTime);
    FileTimeToSystemTime(&CrTime,&lTime);
#else
    memcpy(&lTime,localtime((const time_t *) &CrTime1),sizeof(lTime));
#endif
//  printf("stime->wDay %u == lTime.wDay %u ;stime->wHour =%u, lTime.wHour=%u ",stime->wDay,lTime.wDay,stime->wHour, lTime.wHour );
    if( (stime->wDay!=lTime.wDay)
#ifdef SYSUNIX
     || (lastday!=stime->wDay) )
    {struct stat stt;
     if(stime->wDay==lTime.wDay && lTime.wDay==1 )
      if(--lTime.ttm.tm_mon<0)lTime.ttm.tm_mon=11;
     lTime.wDay=lastday;
     lastday=stime->wDay;
#else
    ){
#endif
     if(t=strrchr(flog,'.') )*t=0;
     sprintf(pprot+16,"%.255s%2.2u%2.2u",flog,lTime.wMonth,lTime.wDay);
#ifdef SYSUNIX
    if(stat(pprot+16,&stt)>=0 ){
       struct tm *ttm;
       char *tt;
       char ddir[280];

       tt=strrchr(pprot+16,'/');
       ttm=localtime(&stt.st_ctime);
       if(tt)
       {
         *tt=0;  
         sprintf(ddir,"%.255s/%u",pprot+16,ttm->tm_year+1900);
         mkdir(ddir,0700);
         sprintf(ddir,"%.255s/%u/%.20s",pprot+16,ttm->tm_year+1900,tt+1);
         *tt='/';  
       }
       else
       {
         sprintf(ddir,"%u",ttm->tm_year+1900);
         mkdir(ddir,0700);
         sprintf(ddir,"%u/%.20s",ttm->tm_year+1900,pprot+16);
       }
       if(rename(pprot+16,ddir) )
       {
           if(t)*t='.';
           goto lbRenErr;
       }
    }
#else

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES  ((ulong)-1)
#endif
     if(GetFileAttributes(pprot+16)!=INVALID_FILE_ATTRIBUTES )
     {
      char *tt;
      WIN32_FIND_DATA  fdt;
      SYSTEMTIME sTim2;
      HANDLE hh2;
      char ddir[280];

      hh2=FindFirstFile(pprot+16,&fdt);
      if(hh2!=INVALID_HANDLE_VALUE)
      {
         FindClose(hh2);     
         FileTimeToSystemTime(&fdt.ftCreationTime,&sTim2);
         
         
       tt=strrchr(pprot+16,'/');
       if(tt)
       {
         *tt=0;  
         sprintf(ddir,"%.255s/%u",pprot+16,sTim2.wYear);
         CreateDirectory(ddir,&secat);
         sprintf(ddir,"%.255s/%u/%.20s",pprot+16,sTim2.wYear,tt+1);
         *tt='/';  
       }
       else
       {
         sprintf(ddir,"%u",sTim2.wYear);
         CreateDirectory(ddir,&secat);
         sprintf(ddir,"%u/%.20s",sTim2.wYear,pprot+16);
       }
       if(!MoveFile(pprot+16,ddir) )
       {
           if(t)*t='.';
           goto lbRenErr;
       }
      }
     }         
#endif
     if(t)*t='.';
        
     MoveFile(flog,pprot+16);
#ifndef SYSUNIX
     if( (ll=_lcreat(flog,0))>0)
     {_hwrite(ll,f_prot,l);
      GetSystemTimeAsFileTime(&CrTime);
      SetFileTime((HANDLE)ll,&CrTime,&CrTime,&CrTime);
#else
     if( (ll=creat(flog,0600))>0)
     {_hwrite(ll,f_prot,l);
#endif
      _lclose(ll);
     }
    }
   }
  }
lbRenErr:;  
  f_prot=pprot;
 };
#endif
 if((pprot-b_prot)>LOG_SIZE)
 {pprot-=0x1000;f_prot-=0x1000; memcpy(b_prot,b_prot+0x1000,pprot-b_prot);};

#ifndef SYSUNIX
 if(!wstate)ShowProt();
#endif
 pval++;
 MyUnlock(pcnt);
};
void RelProt()
{SYSTEMTIME stime;
 GetLocalTime(&stime);
 RelProt(&stime);
};

extern "C" void debug(const char *a,...)
{
#if defined(ARM) || defined(x86_64)
va_list v;

 va_start(v,a);
#endif



GetProt();
 DWORD_PTR(*pprot)=0x0A0D;
#ifdef SYSUNIX
//char * t;
//  t=pprot;
#endif
 pprot+=
#ifndef SYSUNIX
 wvsprintf
#else


 vsprintf
#endif
 (pprot+=2,a,(
#if  defined(ARM) || defined(x86_64)
v)
#else
#ifdef _BSD_VA_LIST_
 _BSD_VA_LIST_
#else
 //void *
 va_list
#endif
 ) ((&a)+1)
#endif
 );
 DWORD_PTR(*pprot)=0x0A0D;
 pprot+=2;
#ifdef SYSUNIX
// printf("%s",t);
#endif 
 RelProt();
}

void dbgf(char *er,int s)
{char buf1[200];
 sprintf(buf1,"Error: %s\r\n",er);
 AddToLog(buf1,s);
}

const char FmtBasicC[]=">Keep-Alive\r\n%.4000s";
const char FmtBasic[] =">\r\n%.4000s";
const char FmtShort []=">>%.256s\r\n";
const char FmtShortErr []=">>%.256s\r\n";
const char FmtShortR[]="}>%.256s\r\n";
const char FmtShrt [] ="<<%.256s\r\n";
const char FmtShrtR[] ="<<%.256s\r\n";


char *DLST[]={"\nAuthorization:","\nProxy-Authorization:",">PASS ","/$_admin_$user?n=",0};

char *oldprot=b_prot;

#ifdef USE_IPV6
void IP2S(char *addr6,sockaddr_in* xsa)
{
#define XAR  (((sockaddr_in6 *)xsa)->sin6_addr.s6_addr16)
 if(((sockaddr_in6 *)xsa)->sin6_family==AF_INET6)
 {
  if(
     ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr32[0]==0 &&
     ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr32[1]==0 &&
     (
      ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr32[2]==0xFFFF0000
       //|| ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr16[2]==0
     )
  )
   sprintf(addr6,"%u.%u.%u.%u",
    ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr[12],
    ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr[13],
    ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr[14],
    ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr[15]);
  else
  {
   sprintf(addr6,"%X:%X:%X:%X:%X:%X:%X:%X-",
    htons(XAR[0]),htons(XAR[1]),htons(XAR[2]),htons(XAR[3]),htons(XAR[4]),
    htons(XAR[5]),htons(XAR[6]),htons(XAR[7]));
  }
 }
 else
   sprintf(addr6,"%u.%u.%u.%u",
#ifndef SYSUNIX
   xsa->sin_addr.S_un.S_un_b.s_b1, xsa->sin_addr.S_un.S_un_b.s_b2,
   xsa->sin_addr.S_un.S_un_b.s_b3, xsa->sin_addr.S_un.S_un_b.s_b4
#else
   xsa->sin_addr.s_addr&0xFF ,  BYTE_PTR(xsa->sin_addr.s_addr,1),
   BYTE_PTR(xsa->sin_addr.s_addr,2),BYTE_PTR(xsa->sin_addr.s_addr,3)
#endif
   );
}
#endif

char *TrimLogLines(char *s)
{
 int i=0,mtrim_log_lines;   
 char *t=s,*e;

 if(trim_log_lines<32)trim_log_lines=32;
 mtrim_log_lines=trim_log_lines+3;   
 while(*s)
 {
   if(*s=='\r' || *s=='\n')
   {
     i=0;    
   }    
   if(i<mtrim_log_lines)  
     *t++=*s;    
   else 
       if(i==trim_log_lines) WORD_PTR(t[i-2])=0x2e2e;
   
   i++;
   s++;
 }    
 *t=0;
 return t;
}

void AddToLog(char *t,int s,const char *fmt)
{SYSTEMTIME stime;
#ifdef USE_IPV6
 struct sockaddr_in6
#else
 struct sockaddr_in
#endif
  xsa[2];
#define sa  (*(sockaddr_in *)xsa)
#define san (*(sockaddr_in *)(xsa+1))
 int ll,l;
 l=sizeof(xsa[0]);
 char *x,*y;

// printf("s=%d \n",s);
 if(s){
  getpeername(s,(sockaddr*) (xsa),&l);
  getsockname(s,(sockaddr *) (xsa+1),&l);
 }else memset(&sa,0,sizeof(xsa));

 GetLocalTime(&stime);
#ifdef USE_IPV6
 char addr6[64];
 IP2S(addr6,(sockaddr_in *)xsa);
#endif
 GetProt();
 if(FmtShortErr == fmt)
 {
   DWORD_PTR(*pprot)=0x21215E x4CHAR("^!!");
   pprot+=3;
 }
 pprot+=sprintf(pprot,
 "\r\n!-%c%2.2u/%2.2u %2.2u:%2.2u:%2.2u ["
#ifdef USE_IPV6
  "%s"
#else
  "%u.%u.%u.%u"
#endif

 ":%u%c%u] (t%d %d) "
 ,fmt[0],
  stime.wDay,stime.wMonth,stime.wHour,stime.wMinute,stime.wSecond,
#ifndef USE_IPV6
#ifndef SYSUNIX
   sa.sin_addr.S_un.S_un_b.s_b1,  sa.sin_addr.S_un.S_un_b.s_b2,
   sa.sin_addr.S_un.S_un_b.s_b3,  sa.sin_addr.S_un.S_un_b.s_b4,
#else
   sa.sin_addr.s_addr&0xFF ,  BYTE_PTR(sa.sin_addr.s_addr,1),
   BYTE_PTR(sa.sin_addr.s_addr,2),BYTE_PTR(sa.sin_addr.s_addr,3),
#endif
#else
   addr6,
#endif
   htons((ushort)sa.sin_port),fmt[0],
   htons((ushort)san.sin_port),
   count_of_tr //s
   ,pval);
   x=pprot;
   pprot+=sprintf(pprot,fmt+1,t)-2;
   if(trim_log_lines)pprot=TrimLogLines(x)-2;
   if(FL1_DELPAS&s_flgs[1])
   {for(l=0;DLST[l];++l)if((y=stristr(x,DLST[l])))
    {y+=strlen(DLST[l]); while(*((uchar *)y)>0xD) *y++='*';}
   }
 RelProt(&stime);
#undef sa
#undef san
};

#ifdef SERVICE
void CloseService()
{if(sesh && ServiceStatus.dwCurrentState!=SERVICE_STOPPED)
 {ServiceStatus.dwCurrentState=SERVICE_STOPPED;
  SetServiceStatus(sesh,&ServiceStatus);
 }
};
#else
#define CloseService()
#endif

/*
void CloseSocket(int s){shutdown(s,2); closesocket(s);}

#undef closesocket
#define shutdown(a,b)
#define closesocket(s) CloseSocket(s)
*/

void StopSocket()
{int i;
 for(i=0;i<
 /*
#if defined(USE_IPV6) && ! defined(SYSUNIX)
    16
#else
    8
#endif
  */
   MAX_SOCK  ; i++) if(soc_srv[i]>0){CloseSocket(soc_srv[i]); soc_srv[i]=0; }

      
}
void CloseServer()
{int i;
#ifdef SYSUNIX
 if(s_aflg&AFL_EXIT){Sleep(3000); }
 s_aflg|=AFL_EXIT;
#else  
  if(! (s_flg&FL_NOICON) )Shell_NotifyIcon(NIM_DELETE,&nid);
#endif
 is_no_exit=0;
 unsave_limit=1;
 RelProt();
 DestroyWindow(mwnd);
 DestroyIcon(
#ifndef DJGPP     
     (HICON__ *) 
#endif     
     hicon);
#if V_FULL
/*
 if( //dns_incache &&  dcache
    dnscachefile && pns_hash &&(i=_lcreat(dnscachefile,0))>0)
 {//_hwrite(i,(char *)&dns_filehead,sizeof(dns_filehead));
  //_hwrite(i,(char *)dip   ,dns_incache *4);
  //_hwrite(i,(char *)dcache,dns_incache *4);
  //_hwrite(i,(char *)dtime ,dns_incache);
  pns_hash->sign=0x1290475;
  _hwrite(i,(char *)pns_hash ,sizeof(NS_hash)+dns_cach_size);
  _lclose(i);
 }
 */
// SaveDNS();
#endif
 CloseService();
 StopSocket();

 Sleep(500);

 WSACleanup();
#if V_FULL
 if(total_dhcp_ip)SaveDHCP();
#endif
 ExitProcess(0);
}


#ifndef SYSUNIX
#include "mrc.cpp"
#endif


#ifdef ONEFILE



#include "mstring1.cpp"

#define MAINSTR 1

#undef LF
#define LF "\n"

#ifndef STRING_CONST_H_t2
#include "g4s1.hh"
#include "g4strc.h"
#include "g4strcwm.h"
#include "g4strhtm.hh"
//#include "strc.h"
#endif


#include "conf.cpp"

#include "icfgjs.cpp"
//#include "icfghtm.cpp"


#ifndef RUS
#include "t2icfg.cpp"
//#include "icfg.cpp"
#else
#include "icfgrus.cpp"
#endif

#ifndef SYSUNIX
//#if V_FULL
//#include "ddns.cpp"
//#endif
#include "qsort.cpp"

#include "srvdat.cpp"
#include "fwnd.cpp"
#else
#include "to_linux.cpp"
#ifndef FREEVER
#include "reghtm.cpp"
#endif
#endif


#ifndef CD_VER
#include "musers.cpp"
#endif
#include "srv_ssi.cpp"
//#include "qsort.cpp"
#include "stat.cpp"
#include "accept.cpp"
#include "gz.cpp"
#include "req.cpp"


#if V_FULL
#include "adminr.cpp"
#include "proxy.cpp"
#include "dhcpd.cpp"
#include "dnsd.cpp"
#include "mailip.cpp"
#else
#include "strvar.cpp"
#include "icfghtm.cpp"
#endif
#include "srv_cgi.cpp"
#if V_FULL
#include "smptps.cpp"
#include "smtpcl.cpp"
#include "ftpd.cpp"
#include "tlsm.cpp"
#endif
#include "restart.cpp"
#include "isapi.cpp"
#include "wmbx.cpp"
#include "bvprintf.cpp"
#include "msprintfchk.cpp"

#endif

void PrintHelp()
{
#ifndef SYSUNIX
 int h=(int)GetStdHandle((ulong)STD_OUTPUT_HANDLE);
  
#define printf(a...)  _hwrite(h,b_prot,wsprintf(b_prot,a))
 
#endif     
     printf("%s\nCommand line:\n%s [--v][--c config_file|@config_file][--h|?] [{Params }]\n--v print version and exit\n--c config_file use directed config file instead default\n@config_file  use directed config file after default\n--h,--? print this help and exit\nParams the same as the params in config file:\n",sSMALL_HTT,
      cmdline     
    //        argv[0] 
           );
      for(CfgParam *cp=ConfigParams;cp->desc /*&& cp->name*/;++cp)
       if(!cp->name)
       {
           printf("%s:\n",cp->desc);
       }
       else
       {
            if( (cp->min==255 || cp->min==256) && cp->v )
            {   
                printf("%s=path - %s\n",cp->name,cp->desc); 
                
            }
            else     if(cp->v )
            {
               if(cp->adv) 
               {
                   printf("%s=value - ",cp->name);
                   printf(cp->desc,cp->adv);
                   printf("\n");
               }
               else 
                   printf("%s=value - %s\n",cp->name,cp->desc);
            }
            else
               printf("%s - %s\n",cp->name,cp->desc);
       }    
#ifndef SYSUNIX
#undef printf
#endif   
    
}

#ifndef SYSUNIX

#ifdef SERVICE
//int WINAPI RMain(void *);
extern "C" int RMain(void *);

void WINAPI ServiceStart (DWORD argc, LPTSTR *argv)
{if(!cmdline)cmdline=argc?argv[0]:(char *)"http.exe";
 CreateThread(&secat,0x5000,(TskSrv)RMain,0,0,&trd_id);
 if((sesh=RegisterServiceCtrlHandler("shttps",NThandler)))
 {SetServiceStatus(sesh, &ServiceStatus);
  debug( sSTART_AS_ );
 }
};
SERVICE_TABLE_ENTRY  DispatchTable[]={{"shttps", ServiceStart},{0,0} };
#endif

#ifdef MINGW
extern "C"  int WINAPI WinMain1( HINSTANCE hinst, HANDLE prev_inst, LPSTR cmline, int cmdshow )
#else
int PASCAL WinMain( HINSTANCE hinst, HANDLE prev_inst, LPSTR cmline, int cmdshow )
#endif
{hinstance=hinst;
 cmdline=cmline;
#ifdef SERVICE
 if( (! (s_aflg=(GetVersion() & 0x80000000)) ) )
   //NTSERVISE)
 {DWORD_PTR(b_prot[0])=255;
  GetUserName(b_prot+4, (ulong *) b_prot);
  if((!(b_prot[0]))
    || stristr(b_prot+4,"SYSTEM")
    || stristr(cmline," service")
  )
  {NTSERVISE=1;
   if(StartServiceCtrlDispatcher(DispatchTable))return 0;
   debug( sRUN_AS_AP );
  }
 }
 return RMain(0);
}

//int WINAPI RMain(void *)

extern "C" int RMain(void *)
{
#endif
 MSG msg;
 char *t,*p,*pp;
// union{
 int ll;
// ulong ull;
// };
 if(strstr(cmdline," --v") )
 {
   _hwrite((int)GetStdHandle((ulong)STD_OUTPUT_HANDLE),sSMALL_HTT "\n",sizeof(sSMALL_HTT)+1);
   return 0;  
 }    
 if( ( (t=strstr(cmdline," --h") ) || (t=strstr(cmdline," --?") ) ) && ! (t[4])  )
 {
     *t=0;
     PrintHelp();
     return 0;  
 }    
 if( (t=strstr(cmdline," --c ") ) )
 {
   t=SkipSpace(t+5); 
   ll=' ';
   if(*t=='\"')
   {
     t++;
     ll='\"';
   }
   if((p=strchr(t,ll)))*p=0;
   PrepCfg(t);  
   if(t[1]==':' || t[2]==':' && (pp=strrchr(t,'\\')) )
   {
    *pp=0;
    SetCurrentDirectory(t);
    *pp='\\';
//    LoadLangCfg("shs_lang.cfg" );
   }
   if(p)*p=ll;
   LoadLangCfg("shs_lang.cfg" );
 }
 else if( (p=stristr(t=cmdline,".exe") ) )
 {if(*t=='\"')++t;
  ll=DWORD_PTR(p[1]);
  DWORD_PTR(p[1])=0x676663 x4CHAR("cfg");
  PrepCfg(t);
  LoadLangCfg("shs_lang.cfg" );
  DWORD_PTR(p[1])=ll;
 }
 InitParam(cmdline);
#if (! defined(CD_VER) ) && (! defined(FREEVER) )
 CheckCode((uchar *)user_name,chk_code3a,0);
#endif
 if(InitApplication()<=0)return 0;
#if  (! defined(FREEVER) )
 if(CheckDate(cmdline)){CloseService();  ExitProcess(0);} ;
#endif 
#if (! defined(CD_VER) ) 
#if (! defined(SYSUNIX) )  && (! defined(FREEVER) )
 if(userList && userList->next && userList->next->next &&
   user_name!=(about+sizeof(ABOUT_STR)+14) )
    MessageBox(0,sNOW_THE_PROGRAM,"Warning",MB_ICONSTOP|MB_OK);
#endif
 if(flog){ if( (ll=_lopen(flog,1))<0)ll=_lcreat(flog,0);
 _lclose(ll); }
#endif
#ifdef CD_VER1
 sprintf(b_prot+0x3000,"%.128s",def_dir);
 if(cddir.d.DirChkLngh|dirchk.DirChkSumX )
 {
  CalcDirChkSum(b_prot+0x3000);
  if(
    dirchk.DirChkLngh!=cddir.d.DirChkLngh ||
    dirchk.DirChkSumX!=cddir.d.DirChkSumX ||
    dirchk.DirChkSum2X!=cddir.d.DirChkSum2X
  )CloseServer();
 }
#endif


 while(GetMessage( &msg, NULL, 0, 0 ))
 {if( ( (!dwnd2) || !IsDialogMessage(dwnd2,&msg) ) )
  {if( dwndc && IsDialogMessage(dwndc,&msg) )
   {
    if(msg.message!=WM_KEYDOWN)continue;
    msg.hwnd=dwndc;
   }
   //else
       if( (!dwndc) && msg.message == WM_LBUTTONDBLCLK)
   {
      DefProc(mwnd, WM_LBUTTONDBLCLK,msg.wParam, msg.lParam) ;
      continue;
   } 
  // else
   {    
     TranslateMessage( &msg );
     DispatchMessage( &msg );
   }
  }
  if(s_aflg&AFL_ICO)
  {if(((ulong)((ulong)ll-GetTickCount()))>0x1000ul)
   {if(Shell_NotifyIcon(NIM_ADD,&nid))s_aflg&=~AFL_ICO;
    ll=GetTickCount();
   }
  }
 };

 CloseServer();
 return 0;
};
#else
char **__argv;
#include "slloop.cpp"


#endif

