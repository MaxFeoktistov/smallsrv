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

#ifdef SYSUNIX
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

#define pprot  lpprot
#define f_prot lf_prot
//#define pcnt   lpcnt
#define b_prot lb_prot

#if defined(_BSD_VA_LIST_) && ! defined(USEVALIST)
#define  mva_list _BSD_VA_LIST_
#endif

//#define LOGDBG(a, b...) printf("%u:%s " a "\r\n", __LINE__, __func__, ## b);
#define LOGDBG(a, b...)

void TLog::Init(int n)
{
  lpprot=lf_prot=loldprot=lb_prot;
  //suffix=SrvNameSufix[n] ;
  idx = n;
  lb_prot[0]=0;
  lpcnt=0;
  msk=1<<n;
  llastday=0;
}
void TLog::FixPtr(char *old_base, int n)
{
  long d = lb_prot - old_base;
  lpprot += d;
  lf_prot += d;
  loldprot += d;
  //suffix=SrvNameSufix[n];
}

//void TLog::LAddToLog(char *t,int s,const char *fmt,...)
void TLog::LAddToLog(char *t,int s, TSOCKADDR *psa, const char *fmt,...)
{SYSTEMTIME stime;
  TSOCKADDR xsa[2];
  //TSOCKADDR lsa = xsa + 1;
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
     pprot+= //sprintf(pprot,fmt+1,t)-2;
           msprintfchk(pprot,aabfr+256,fmt+1,t)-2;
   else
   {
#ifdef  USEVALIST
      va_list v;
      va_start(v,fmt);
      pprot+=mvsprintfchk(pprot,aabfr+256,fmt+1,v)-2;
      va_end(v);
#else
      pprot+=mvsprintfchk(pprot,aabfr+256,fmt+1,(mva_list) (void *) ((&fmt)+1) )-2;
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


int InitShmem(int n_log, int new_only)
{
  int status = 0;
  int size = sizeof(SHMdata) + n_log * sizeof(TLog);

#ifdef SYSUNIX
  if( (!new_only) && (shm_id=shmget(shmkey,0, 0660)    )!=-1 ) {
    status = 1;
    LOGDBG("SHM found")
  }
  else {
    LOGDBG("SHM Not found")
    if((shm_id=shmget(shmkey, size, IPC_CREAT|IPC_EXCL|0660))==-1)
      return 0;
  }

  if ( (shm=(SHMdata *)shmat(shm_id,0,0)) != ((SHMdata *) -1) && shm )
#else
  if( (hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,                 // do not inherit the name
        shmkey)               // name of mapping object
      ) &&
      (shm = (SHMdata  *) MapViewOfFile(hMapFile, // handle to map object
                                           FILE_MAP_ALL_ACCESS,  // read/write permission
                                           0, 0, 0)
      )
  )
#endif
  {
    if(status)
    {
      if(shm->status & SHMST_RUN)
      {
        LOGDBG("SHM Run pid:%d cmd: %X ; reply: %X", shm->pid, shm->cmd, shm->reply)

        shm->reply = 0;
        shm->cmd = CMD_EXIT;
        if (shm->size == size)
          status |= SHMST_DONT_RM;
#ifdef SYSUNIX
        if(shm->pid > 0 && kill(shm->pid, SIGUSR2) !=-1)
        {
          int timeout = 30;
#ifdef USE_FUTEX
          futex((int *)&shm->reply,FUTEX_WAIT, 0,&timeout_50ms,0,0);
#endif
          do {
#ifdef USE_FUTEX
           futex((int *)&shm->status,FUTEX_WAIT, shm->status, &timeout_50ms,0,0);
#else
           usleep(50000);
#endif
           if (--timeout <= 0)
           {
             kill(shm->pid, SIGKILL);
             break;
           }
         } while(shm->reply == REPLY_INPROGRESS && (shm->status & SHMST_RUN));
       }
#else
    // TODO
#endif
      }

      if(!(shm->status & (SHMST_REMOVED|SHMST_EXIT) ))
      {
        SHMdata *old = shm->master_ptr;

        LOGDBG("shm: %lX old: %lX", (long)shm, (long)old)

        for(int i=0; i < shm->n_log; i++)
        {
          TLog *p;
          p = shm->sepLog + i;
          p->FixPtr(old->sepLog[i].lb_prot, i);

          LOGDBG("Save: %u old: %lX", i,  (long) old->sepLog[i].lb_prot)

          p->Save();
        }
      }

      if (shm->size != size)
      {
        shm->status = SHMST_REMOVED;
#ifdef SYSUNIX
        shmctl(shm_id, IPC_RMID, 0);
        shmdt(shm);
        shm = 0;
        if (new_only)
          return 0;
        usleep(1000);

        return InitShmem(n_log, 1);
#else
        DoneShm();
        if (new_only)
          return 0;
        return InitShmem(n_log, 1);
#endif
      }
   }

   shm->master_ptr = shm;
   shm->size = size;
   shm->vers = VERSION_CODE;  // Version of this header << 24 | Server version code
   shm->n_log = n_log;
#ifdef SYSUNIX
   shm->pid = getpid();
#endif
   shm->status = SHMST_START;

   return status;
  }
  return -1;
};

void  DoneShm()
{
  if(shm) {

#ifdef SYSUNIX
    if (! (shm->status & SHMST_DONT_RM) ) {
      shm->status = SHMST_REMOVED;
      shmctl(shm_id, IPC_RMID, 0);
    }
    else
      shm->status = SHMST_EXIT;

#ifdef USE_FUTEX
    futex((int *)&shm->status,FUTEX_WAKE,1,0,0,0);
#endif

    shmdt(shm);
#else
    shm->status = SHMST_REMOVED;
    UnmapViewOfFile(shm);
    CloseHandle(hMapFile);
#endif
    shm = 0;
    PreInitSepLog(&gLog);
  }
}



void PreInitSepLog(TLog *p)
{
  for(int i=0;i<ARRAY_SIZE(sepLog);i++) sepLog[i] = p;
}

void InitLogN(int i, int n)
{
  if(shm)
  {
    sepLog[i] = shm->sepLog + n;
  }
  else
  {
    sepLog[i] = (TLog *) malloc(sizeof(TLog));
    if(!sepLog[i])
    {
      sepLog[i] = &gLog;
      return;
    }
  }
  sepLog[i]->Init(i);
}

void InitSepLog()
{
 //TLog *p;
 int i;
 int j = 1;
  //gLog.Init("");
  if(sepLog[1])
  {
    if(sepLog[6] == sepLog[0] &&  max_srv[0] && ((FL2_NOERROUT|FL2_DUBSTDERR) & s_flgs[2]) )
    {
        sepLog[6]=//new TLog;
            (TLog *) malloc(sizeof(TLog));
        sepLog[6]->Init(6); //SrvNameSufix[6]);
    }

    return ;
  }
  PreInitSepLog(&gLog);

  if(FL3_SHMLOG & s_flgs[3])
  {
    int n_log = 1;
    if( FL2_SEPARATELOG & s_flgs[2]     )
    {
      for(i=1; i<6; i++)
        if(max_srv[i]) n_log++;
      if(dns_file) n_log++;
      if(total_dhcp_ip) n_log++;
      if(max_srv[0] && ( (FL3_VPN_TUN | FL3_VPN_TAP | FL3_VPN_CLIENT) & s_flgs[3]) ) n_log++;
    }
    if(max_srv[0] && ((FL2_NOERROUT|FL2_DUBSTDERR) & s_flgs[2]) )
       n_log++;

    InitShmem(n_log, 0);
    if(shm)
    {
      int n = gLog.lpprot - gLog.lb_prot;
      shm->sepLog[0].Init(0);
      PreInitSepLog(shm->sepLog);
      if(n)
      {
        memcpy(shm->sepLog[0].lb_prot, gLog.lb_prot, n);
        shm->sepLog[0].lpprot += n;
      }
      //sepLog[0] = shm->sepLog[0];
    }
  }


  if( FL2_SEPARATELOG & s_flgs[2])
  {

    for(i=1; i<6; i++)
    {
     if(max_srv[i])
       InitLogN(i, j++);
     else
       sepLog[i] = sepLog[0];
    }

    if(dns_file)
      InitLogN(SDNS_IND, j++);
    if(total_dhcp_ip)
      InitLogN(SDHCP_IND, j++);

#ifdef TLSVPN
    if(max_srv[0] && ( (FL3_VPN_TUN | FL3_VPN_TAP | FL3_VPN_CLIENT) & s_flgs[3]) )
      InitLogN(VPN_LOG, j++);
#endif
  }

  if(max_srv[0] && ((FL2_NOERROUT|FL2_DUBSTDERR) & s_flgs[2]))
      InitLogN(CGI_ERR_LOG, j++);

  if(shm) {
    shm->status = SHMST_RUN;
  }

};
void DoneSepLog()
{
  int i;

  sepLog[0]->RelProt();
  for(i=1;i<ARRAY_SIZE(sepLog);i++) if(sepLog[i] != sepLog[0]) sepLog[i]->RelProt();
  DoneShm();
}
void UDoneSepLog()
{
  int i;

  for(i=0;i<ARRAY_SIZE(sepLog);i++) if(logsigmsk& (1<<i) ) sepLog[i]->RelProt();
  logsigmsk=0;
}

void SaveAllLog()
{
  int i;

  sepLog[0]->Save();
  for(i=1; i<ARRAY_SIZE(sepLog); i++)
    if(sepLog[i] != sepLog[0])
      sepLog[i]->Save();
}

void TLog::MkFileName(char *t,int n)
{
   char *p;
   const char *suffix = SrvNameSufix[idx];
 //  sprintf(t,(n)?"%.240s%s%4.4u":"%.250s%s",flog,suffix,n);
    sprintf(t,"%.250s%s", flog, suffix);
    if(n)
    {
      p=strrchr(flog,'.');
      if(p)
      {
       sprintf(t+(p-flog),"%4.4u%s", n, suffix);
      }
      else
      {
        sprintf(t,"%.240s%4.4u%s", flog, n, suffix);
      }
    }
    dprint("log filename: '%s'\n", t);
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


int TLog::Save(SYSTEMTIME *stime)
{
  int l = lpprot - lf_prot;
  int ll;
  int tl;
  int r;
  SYSTEMTIME lTime;
  FILETIME CrTime1,CrTime;
  char flogname[256];
  char ddir[280];

  MkFileName(flogname,0);
  LOGDBG("%s l = %u", flogname, l)
#ifndef SYSUNIX
  if( (ll=_lopen(flogname,1))>0)
  {
    _llseek(ll,0,2);
#else
  struct stat stt;
  if(!llastday)
  {
    if( stat(flogname,&stt)>=0)
    {
      llastday= (localtime((time_t *) & (stt.st_ctime))->tm_mday);
    }
  }


  if((ll=open(flogname,O_APPEND|O_WRONLY))>0)
  {
#endif
    r = _hwrite(ll, lf_prot, l);
    LOGDBG("_hwrite(%d, %lX, %u ) : %d; errno %d %s", ll, (long)lf_prot, l, r, errno, strerror(errno))
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
      )
      {
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

           _hwrite(ll, lf_prot, l);
           GetSystemTimeAsFileTime(&CrTime);
           SetFileTime((HANDLE)ll,&CrTime,&CrTime,&CrTime);
           #else
           if( (ll=creat(ddir,0600))>0)
           {
             lbOkOpenL:
             _hwrite(ll, lf_prot, l);
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
    lf_prot=lpprot;
    return 0;
}

void TLog::RelProt(SYSTEMTIME *stime)
{
 int l,ll;
 int tl;
 //SYSTEMTIME lTime;
 //FILETIME CrTime1,CrTime;
 //char flogname[256];
 //char ddir[280];
// char *t;
#ifdef SYSUNIX
 if(!(s_flg&FL_NOICON))printf("%s",loldprot);
#endif
#ifndef CD_VER
 if(flog &&  ( (l=(pprot-f_prot)) >= unsave_limit ) && l)
 {
#ifdef SYSUNIX
   if(puid!=geteuid())
   {
     //printf("\nkill %d %d\n",puid,geteuid());
     logsigmsk|=msk;
     kill(ppid,SIGUSR1);
     return;
   }
#endif // CD_VER

   if(Save(stime))
     return;

 };
#endif // CD_VER

 if((tl = lpprot - lb_prot)>=LOG_SIZE)
 {
   pprot-=0x1000;
   f_prot-=0x1000;
   memcpy(b_prot,b_prot+0x1000,tl);
 };

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

int TLog::Save()
{SYSTEMTIME stime;
 GetLocalTime(&stime);
 return Save(&stime);
};



extern "C" void TLog::Ldebug(const char *a,...)
{
#ifdef  USEVALIST
 va_list v;
 va_start(v,a);
 Lvdebug(a,v);
 va_end(v);
#else
 Lvdebug(a,(mva_list) (void *) ((&a)+1));
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

extern "C" void tlsdebug(const char *a,...)
{
#ifdef  USEVALIST
 va_list v;
 va_start(v,a);
 sepLog[5]->Lvdebug(a,v);
 va_end(v);
#else
 sepLog[5]->Lvdebug(a,(mva_list) (void *) ((&a)+1));
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
  return sepLog[0];
}

TLog *GetLogR(Req *r,int def)
{
  int l=def;
    if(r)
    {
      l=r->flsrv[1] & MAX_SERV_MASK;
      if(l>=N_LOG)l=def;
    }
    return sepLog[l];
};


#undef pprot
#undef f_prot
#undef pcnt
#undef b_prot

