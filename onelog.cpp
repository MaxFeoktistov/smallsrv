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
#if defined(USEVALIST)
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
 (pprot+=2,a,
#if defined(USEVALIST)
v
#else
#ifdef _BSD_VA_LIST_
 ( _BSD_VA_LIST_)
#else
 //void *
 (va_list)
#endif
  ((&a)+1)
#endif
 );
 DWORD_PTR(*pprot)=0x0A0D;
 pprot+=2;
#ifdef SYSUNIX
// printf("%s",t);
#endif
 RelProt();
}



void AddToLog(char *t,int s, TSOCKADDR *psa, const char *fmt,...)
{SYSTEMTIME stime;
 TSOCKADDR xsa[2];

#define sa  (*(sockaddr_in *)psa)
#define san (*(sockaddr_in *)(xsa+1))
 int ll,l;
 l=sizeof(xsa[0]);
 char *x,*y;

// printf("s=%d \n",s);
 if(s>0){
   if(!psa) {
     getpeername(s,(sockaddr*) (xsa),&l);
     l=sizeof(xsa[0]);
   }
   getsockname(s,(sockaddr *) (xsa+1),&l);
 }
 else
 {
   san.sin_port = htons(-s);
 }
 if(!psa) psa = xsa;

 GetLocalTime(&stime);
#ifdef USE_IPV6
 char addr6[64];
 IP2S(addr6,(sockaddr_in *)psa);
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
   if(t)
     pprot+=sprintf(pprot,fmt+1,t)-2;
   else
   {
#ifdef  USEVALIST
      va_list v;
      va_start(v,fmt);
      pprot+=mvsprintfchk(pprot,b_prot+LOG_SIZE,fmt+1,v)-2;
      va_end(v);
#else
      pprot+=mvsprintfchk(pprot,b_prot+LOG_SIZE,fmt+1,(
#ifdef _BSD_VA_LIST_
 _BSD_VA_LIST_
#else
 mva_list
#endif
   ) (void *) ((&fmt)+1)
 )-2;
#endif

   }
   if(trim_log_lines)pprot=TrimLogLines(x)-2;
   if(FL1_DELPAS&s_flgs[1])
   {for(l=0;DLST[l];++l)if((y=stristr(x,DLST[l])))
    {y+=strlen(DLST[l]); while(*((uchar *)y)>0xD) *y++='*';}
   }
 RelProt(&stime);
#undef sa
#undef san
};


