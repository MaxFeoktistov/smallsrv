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


#define pprot  lpprot 
#define f_prot lf_prot
//#define pcnt   lpcnt  
#define b_prot lb_prot


void TLog::Init(int n){lpprot=lf_prot=loldprot=lb_prot; suffix=SrvNameSufix[n] ;  lb_prot[0]=0; lpcnt=0; msk=1<<n; llastday=0;};
void TLog::LAddToLog(char *t,int s,const char *fmt,...)
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
   if(t)
     pprot+= //sprintf(pprot,fmt+1,t)-2;
           msprintfchk(pprot,aabfr+256,fmt+1,t)-2;
   else
   {
#ifdef  USEVALIST
         //defined(ARM) || defined(x86_64)
      va_list v;
      va_start(v,fmt);
      pprot+=mvsprintfchk(pprot,aabfr+256,fmt+1,v)-2;
      va_end(v);
#else  
      pprot+=mvsprintfchk(pprot,aabfr+256,fmt+1,(
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

void InitSepLog()
{
 //TLog *p;
 int i;
  //gLog.Init("");
  if(sepLog[1])
  {
    if(sepLog[6] == &gLog &&  max_srv[0] && ((FL2_NOERROUT|FL2_DUBSTDERR) & s_flgs[2]) )
    {
        sepLog[6]=//new TLog;
            (TLog *) malloc(sizeof(TLog));
        sepLog[6]->Init(6); //SrvNameSufix[6]);
    }
        
    return ;
  }    
  for(i=0;i<ARRAY_SIZE(sepLog);i++) sepLog[i]=&gLog;
  
  if( FL2_SEPARATELOG & s_flgs[2]     )
  {
    //sepLog[0]=&gLog;
    //sepLog[5]=&gLog;
   // p=new TLog[];
    
    for(i=1;i<6;i++)
    {
     if(max_srv[i])
     {
       sepLog[i]=//new TLog;
            (TLog *) malloc(sizeof(TLog));
       sepLog[i]->Init(i);//SrvNameSufix[i]);
     }
     else
     {
       sepLog[i]=&gLog;
     }
    }
    
    if(dns_file)
    {
      sepLog[SDNS_IND]=sepLog[SDNS_IND+1]= //new TLog;
                    (TLog *) malloc(sizeof(TLog));
      sepLog[SDNS_IND]->Init(SDNS_IND); //SrvNameSufix[SDNS_IND]);
    }
    if(total_dhcp_ip)
    {
      sepLog[SDHCP_IND]=//new TLog;
            (TLog *) malloc(sizeof(TLog));
      sepLog[SDHCP_IND]->Init(SDHCP_IND) ; //SrvNameSufix[SDHCP_IND]);
    }

#ifdef TLSVPN
    if(max_srv[0] && ( (FL3_VPN_TUN | FL3_VPN_TAP | FL3_VPN_CLIENT) & s_flgs[3]) )
    {
      sepLog[VPN_LOG]=//new TLog;
      (TLog *) malloc(sizeof(TLog));
      sepLog[VPN_LOG]->Init(VPN_LOG); //SrvNameSufix[6]);
    }
#endif
  }
  
  if(max_srv[0] && ((FL2_NOERROUT|FL2_DUBSTDERR) & s_flgs[2]) )
  {
       sepLog[CGI_ERR_LOG]=//new TLog;
         (TLog *) malloc(sizeof(TLog));
       sepLog[CGI_ERR_LOG]->Init(6); //SrvNameSufix[6]);
  }
      
};
void DoneSepLog()
{
 int i;
   gLog.RelProt();
   for(i=1;i<ARRAY_SIZE(sepLog);i++) if(sepLog[i] != &gLog) sepLog[i]->RelProt();
}
void UDoneSepLog()
{
 int i;
   for(i=0;i<ARRAY_SIZE(sepLog);i++) if(logsigmsk& (1<<i) ) sepLog[i]->RelProt();
   logsigmsk=0;
}

void TLog::MkFileName(char *t,int n)
{
   char *p;
 //  sprintf(t,(n)?"%.240s%s%4.4u":"%.250s%s",flog,suffix,n); 
    sprintf(t,"%.250s%s",flog,suffix); 
    if(n)
    {
      p=strrchr(flog,'.');  
      if(p)
      {
       sprintf(t+(p-flog),"%4.4u%s",n,suffix); 
      }
      else
      {
        sprintf(t,"%.240s%4.4u%s",flog,n,suffix); 
      }    
    }    
    dprint("log filename: '%s'\n",t);
}

void TLog::GetProt(){
    int dead_lock_chk=512;
    while(mutex_pcnt && --dead_lock_chk>0)
#ifdef USE_FUTEX
      futex((int *)&mutex_pcnt,FUTEX_WAIT,mutex_pcnt,&timeout_50ms,0,0);  
#else      
      Sleep(50); 
#endif         
     MyLock(lpcnt);
     pcnt|=msk;
#ifdef SYSUNIX
     loldprot=lpprot;
#endif
};
void TLog::RelProt(SYSTEMTIME *stime)
{int l,ll;
 SYSTEMTIME lTime;
 FILETIME CrTime1,CrTime;
 char flogname[256];
 char ddir[280];
// char *t;
#ifdef SYSUNIX
 if(!(s_flg&FL_NOICON))printf("%s",loldprot);
#endif
#ifndef CD_VER
 if(flog &&  ( (l=(pprot-f_prot)) >= unsave_limit ) && l)
 {
  MkFileName(flogname,0);
#ifndef SYSUNIX
  if( (ll=_lopen(flogname,1))>0)
  {_llseek(ll,0,2);
#else
  struct stat stt;
  if(!llastday)
  {
    if( stat(flogname,&stt)>=0)
    {
       llastday= (localtime((time_t *) & (stt.st_ctime))->tm_mday);  
    }    
  }    
  if(puid!=geteuid()){
//printf("\nkill %d %d\n",puid,geteuid());
  logsigmsk|=msk;    
  kill(ppid,SIGUSR1); return;}

  if((ll=open(flogname,O_APPEND|O_WRONLY))>0)
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
     || (llastday!=stime->wDay) )
    {
     if(stime->wDay==lTime.wDay && lTime.wDay==1 )
       if(--lTime.ttm.tm_mon<0)lTime.ttm.tm_mon=11;
     lTime.wDay=llastday;
     llastday=stime->wDay;
#else
    ){
#endif
     //if(t=strrchr(flog,'.') )*t=0;
     //sprintf(flogname,"%.240s%s%2.2u%2.2u",flog,suffix,lTime.wMonth,lTime.wDay);
     MkFileName(flogname,(lTime.wMonth)*100+lTime.wDay);
#ifdef SYSUNIX
     if(stat(flogname,&stt)>=0 )
     {
       struct tm *ttm;
       char *tt;

       tt=strrchr(flogname,'/');
       ttm=localtime(&stt.st_ctime);
       if(tt)
       {
         *tt=0;  
         sprintf(ddir,"%.255s/%u",flogname,ttm->tm_year+1900);
         mkdir(ddir,0700);
         sprintf(ddir,"%.255s/%u/%.20s",flogname,ttm->tm_year+1900,tt+1);
         *tt='/';  
       }
       else
       {
         sprintf(ddir,"%u",ttm->tm_year+1900);
         mkdir(ddir,0700);
         sprintf(ddir,"%u/%.40s",ttm->tm_year+1900,flogname);
       }
       dprint("rename '%s' to '%s'\n",flogname,ddir);
       if(rename(flogname,ddir) )
       {
           if(!(s_flg&FL_NOICON))printf("Can't move '%s' to '%s'. %d %s\n",flogname,ddir,errno,strerror(errno));
           lpprot+=sprintf(lpprot,"Can't move '%s' to '%s'. %d %s\n",flogname,ddir,errno,strerror(errno));
       //    if(t)*t='.';
           
           goto lbRenErr;
       }
    }
#else

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES  ((ulong)-1)
#endif
     if(GetFileAttributes(flogname)!=INVALID_FILE_ATTRIBUTES )
     {
      char *tt;
      WIN32_FIND_DATA  fdt;
      SYSTEMTIME sTim2;
      HANDLE hh2;
      char ddir[280];

      hh2=FindFirstFile(flogname,&fdt);
      if(hh2!=INVALID_HANDLE_VALUE)
      {
         FindClose(hh2);     
         FileTimeToSystemTime(&fdt.ftCreationTime,&sTim2);
         
         
       tt=strrchr(flogname,'/');
       if(tt)
       {
         *tt=0;  
         sprintf(ddir,"%.255s/%u",flogname,sTim2.wYear);
         CreateDirectory(ddir,&secat);
         sprintf(ddir,"%.255s/%u/%.20s",flogname,sTim2.wYear,tt+1);
         *tt='/';  
       }
       else
       {
         sprintf(ddir,"%u",sTim2.wYear);
         CreateDirectory(ddir,&secat);
         sprintf(ddir,"%u/%.20s",sTim2.wYear,flogname);
       }
       if(!MoveFile(flogname,ddir) )
       {
   //        if(t)*t='.';
           goto lbRenErr;
       }
      }
     }         
#endif
   //  if(t)*t='.';
        
  //   sprintf(ddir,"%.250s%s",flog,suffix); 
     MkFileName(ddir,0);

     MoveFile(ddir,flogname);
#ifndef SYSUNIX
     if( (ll=_lcreat(ddir,0))>0)
     {
  lbOkOpenW:   
     
      _hwrite(ll,f_prot,l);
      GetSystemTimeAsFileTime(&CrTime);
      SetFileTime((HANDLE)ll,&CrTime,&CrTime,&CrTime);
#else
     if( (ll=creat(ddir,0600))>0)
     {
  lbOkOpenL:  
       _hwrite(ll,f_prot,l);
#endif
      _lclose(ll);
     }
    }
   }
  }
  else
  {
#ifdef SYSUNIX
    struct stat stt;  
    if(!(s_flg&FL_NOICON))printf("Can't open '%s'. %d %s\n",flogname,errno,strerror(errno));  
    if( stat(flogname,&stt)<0)
    {
      if( (ll=creat(flogname,0600))>0)
         goto lbOkOpenL;
    }    

#else
    if(GetFileAttributes(flogname)==INVALID_FILE_ATTRIBUTES )
    {
      if( (ll=_lcreat(flogname,0))>0)
         goto lbOkOpenW;
    }
     
#endif    
      
  }    
  
lbRenErr:;  
  f_prot=pprot;
 };
#endif
 if((pprot-b_prot)>=LOG_SIZE)
 {pprot-=0x1000;f_prot-=0x1000; memcpy(b_prot,b_prot+0x1000,pprot-b_prot);};

#ifndef SYSUNIX
 if(!wstate)ShowProt();
#endif
 pval++;
 pcnt&=~msk;
 MyUnlock(lpcnt);
      
};
void TLog::RelProt()
{SYSTEMTIME stime;
 GetLocalTime(&stime);
 RelProt(&stime);
};



extern "C" void TLog::Ldebug(const char *a,...)
{
#ifdef  USEVALIST
    //defined(ARM) || defined(x86_64)
 va_list v;
 va_start(v,a);
 Lvdebug(a,v);
 va_end(v);
#else  
 Lvdebug(a,( 
#ifdef _BSD_VA_LIST_
 _BSD_VA_LIST_
#else
 mva_list
#endif
   ) (void *) ((&a)+1)
 );
#endif
}
extern "C" void TLog::Lvdebug(const char *a,mva_list v)
{
 GetProt();
 DWORD_PTR(*pprot)=0x0A0D;
#ifdef SYSUNIX
//char * t;
//  t=pprot;
#endif
 pprot+=
 
/* 
#ifndef SYSUNIX
 wvsprintf
#else
 vsprintf
#endif
 (pprot+=2,a,v);
*/
 mvsprintfchk(pprot+=2,aabfr+256,a,v); 
 DWORD_PTR(*pprot)=0x0A0D;
 pprot+=2;
#ifdef SYSUNIX
// printf("%s",t);
#endif 
 RelProt();
}
/* Unused function
extern "C" void edebug(const char *a,...)
{
#ifdef  USEVALIST
    //defined(ARM) || defined(x86_64)
 va_list v;
 va_start(v,a);
 gLog.Lvdebug(a,v);
 va_end(v);
#else  
 gLog.Lvdebug(a,( 
#ifdef _BSD_VA_LIST_
 _BSD_VA_LIST_
#else
 va_list
#endif
 ) (void *) ((&a)+1)
 );
#endif
}
*/

extern "C" void tlsdebug(const char *a,...)
{
#ifdef  USEVALIST
    //defined(ARM) || defined(x86_64)
 va_list v;
 va_start(v,a);
 sepLog[5]->Lvdebug(a,v);
 va_end(v);
#else  
 sepLog[5]->Lvdebug(a,( 
#ifdef _BSD_VA_LIST_
 _BSD_VA_LIST_
#else
 mva_list
#endif
 ) (void *) ((&a)+1)
 );
#endif
}

TLog *GetLogS(int s)
{
union{    
  TSOCKADDR  sa;
  sockaddr_in sa4;
};
  int i,l=sizeof(sa);
  getsockname(s,(sockaddr *) &sa,&l);  
  for(i=0;i<N_LOG; i++)
  {
   if(sa4.sin_port == soc_port[i] )return sepLog[i];    
  } 
  return &gLog;
}

TLog *GetLogR(Req *r,int def)
{
  int l=def;   
    if(r)
    {
      l=r->flsrv[1];
      if(l>=N_LOG)l=def;
    }    
    return sepLog[l];       
};


#undef pprot  
#undef f_prot 
#undef pcnt   
#undef b_prot

