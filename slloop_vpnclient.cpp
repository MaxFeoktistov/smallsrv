/*
 * Copyright (C) 1999-2025 Maksim Feoktistov.
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


timeval  TVal;
void  SignalHandler(int)
{int ll;
 if(is_no_exit)
 {is_no_exit=0;
  debug("\nBye...\n");
 /*

  if(unsave_limit>1)
  {unsave_limit=1;
   RelProt();
  }

  */
 }
};

void  SignalUSR(int)
{
 //RelProt();
 UDoneSepLog();
 signal(SIGUSR1,SignalUSR);
}


#if ! (defined(ANDROID) || defined(LPC_ARM)  || defined(AT_ARM) )
#include <execinfo.h>
#else
#define ucontext_t void
#endif
#include <sys/resource.h>

typedef void (*tsighandler)(int, siginfo_t*, void*);

//void  ErrHandler(int)
int pid_to_wait;
void signalChild(int , siginfo_t* info, ucontext_t* ptr)
{
  int status;
  if( /*info->si_code==CLD_EXITED && */ info->si_pid>0 )
  {
    waitpid(info->si_pid,(int *)&status,WNOHANG);
    //pid_to_wait=info->si_pid;
    SendEvent(pid_to_wait, info->si_pid);
  }


}
extern "C" ulong _etext,_edata;
#ifdef FIX_EXCEPT
jmp_buf  jmp_env;

#endif
int ext_cntr,ext_cntr2,ext_cntr3,ext_cnt4;
time_t last_ext_time;

#if  (!defined(NOTINTEL)) || defined(x86_64)
void DebugStack(u_long *esp1)
{
 u_long *esp=esp1;
 union{
 u_long *mesp;
 u_long mlesp;
 };
 if(ext_cntr) return;
 ext_cntr++;
 int i=0;
 u_long rez[6];
 u_long min;
// mesp=esp+128;
// mlesp=(((ulong)esp)+0xFFF) & ~0xFFF;
 mlesp=(((u_long)esp)|0xFFF) ;
// if(mlesp>max  )mlesp=max;
 min=(u_long) DebugStack;
 min &=~0xFFFFF;
 rez[0]=min;
 rez[1]=(u_long)&_edata;
 rez[2]=(u_long)&_etext;
 rez[3]=0;
 rez[4]=0;

 if( esp< (u_long *) &_edata )
 {
   debug("Invalid stask range %X-%X",esp,mesp);
   return;
 }

 while(esp<mesp)
 {
   if(*esp>min && *esp< (u_long)&_etext)
   {
     rez[i]=*esp;
     i++;
     if(i>=5)break;
   }
   esp++;
 }
#ifdef x86_64
 debug("In stack (%lX-%lX) found %u return address: %lX %lX %lX %lX %lX\n",esp1,mesp,i,rez[0],rez[1],rez[2],rez[3],rez[4]);
#else
 debug("In stack (%X-%X) found %u return address: %X %X %X %X %X\n",esp1,mesp,i,rez[0],rez[1],rez[2],rez[3],rez[4]);
#endif

 ext_cntr--;


};
#endif

void signalSegv(int , siginfo_t* info, ucontext_t* ptr)
{
 int i;
 int ll;
 Req *r;
 time_t  tt;

 if(!ext_cntr3){
  ext_cntr3++;
  tt=time(0);
  if( tt<last_ext_time)
  {
     if(ext_cnt4>3) goto lbRestart;
  }
  else ext_cnt4=0;
  last_ext_time=tt+20;
  ext_cnt4++;

#ifdef FIX_EXCEPT

 unsave_limit=1;
 ll=GetCurrentThreadId();
#ifdef x86_64
 debug("\r\nException at %lX (%lX) pid=%d code: %X thread %d stack:\r\n", (long)info->si_addr, ptr?ptr->uc_mcontext.gregs[REG_RIP]:0l, info->si_pid, info->si_code, ll);
#elif  (!defined(NOTINTEL))
 debug("\r\nException at %X (%X) pid=%d code: %X thread %d stack:\r\n",info->si_addr,ptr?ptr->uc_mcontext.gregs[REG_EIP]:0,info->si_pid,info->si_code,ll);
#else
 debug("\r\nException at %X pid=%d code: %X thread %d stack:\r\n",info->si_addr,info->si_pid,info->si_code,ll);

#endif
// debug("\nExeption at %X\n last back in stack: %X %X\n",info->si_addr,i,ll);


 unsave_limit=0x3F00;
 for(i=0;i<max_tsk;++i)if( ((u_long)(r=rreq[i]))>1  && r->thread_id == ll )
 {
   debug("Found error thread %u %.96s",i , r->inf );
#if  defined(x86_64)
   if(ptr && ptr->uc_mcontext.gregs[REG_RSP]) DebugStack((u_long *) (ptr->uc_mcontext.gregs[REG_RSP]));
#elif !defined(NOTINTEL)
   if(ptr && ptr->uc_mcontext.gregs[REG_ESP]) DebugStack((u_long *) (ptr->uc_mcontext.gregs[REG_ESP]));

//   asm volatile("movl %%ebp, %%eax \n":"=&a" (ll) )  ;
//   DebugStack((ulong *) ll);

#endif

   ext_cntr3--;
   longjmp(r->jmp_env,1);
   return ;
  }


 debug("Not found error thread %X",ll);

#endif


 }

lbRestart:
 if(is_no_exit)
 {
  is_no_exit=0;
  s_aflg|=AFL_EXIT;
  unsave_limit=1;
#ifndef ARM

#endif

#if defined(x86_64)
  if( (!ext_cntr2) && ptr && ptr->uc_mcontext.gregs[REG_RSP]){ext_cntr2++; DebugStack((u_long *) (ptr->uc_mcontext.gregs[REG_RSP])); ext_cntr2=0; }
#elif  !defined(NOTINTEL)
  if( (!ext_cntr2) && ptr && ptr->uc_mcontext.gregs[REG_ESP]){ext_cntr2++; DebugStack((u_long *) (ptr->uc_mcontext.gregs[REG_ESP])); ext_cntr2=0; }
#endif
 }
 longjmp(jmp_env,1);

};

int last[8]={-1,-1,-1,-1,-1,-1};
struct rlimit  rlim={0x7FFFFFF,0x7FFFFFF};

extern "C" int main(int argc, char *argv[] )
{
  int ll;
union{
 fd_set set;
 struct stat stt;
 int tmp[2];
};
fd_set er_set;
 int s,i,j,k,kk;
 char *t,*p;

 __argv=argv;

#ifdef FIX_EXCEPT


   if(setjmp(jmp_env))
   {

    sleep(3);
    if( !vfork() )
     execl(__argv[0],__argv[0],0);
     exit(0);
   }

#endif
 cmdline=argv[0];

#ifdef SEPLOG
 gLog.Init(0);//"");
 PreInitSepLog();
#endif

 if(argc>1 )
 {
   i=DWORD_PTR(argv[1][0]);
   if(! (i&0xFF0000) ) i&=0xFFFF;
   switch(i)
   {
       case 0x00762D x4CHAR("-v") :
       case 0x00762D2D x4CHAR("--v") :
       case 0x65762D2D x4CHAR("--ve") :
           printf("%s\n", sSMALL_HTT);
           exit(0);
       case  0x003F2D x4CHAR("-?"):
       case  0x003F2D2D x4CHAR("--?"):
#if defined(CONFIG_CONFIG) && ! defined(CONFIG_CURRENT_DIR)
        if( LoadLangCfg(CONFIG_CONFIG "shs_lang.cfg" ) < 0 )
#endif
          LoadLangCfg( "shs_lang.cfg" );

       case  0x00682D x4CHAR("-h"):
       case  0x00682D2D x4CHAR("--h"):
       case 0x65682D2D x4CHAR("--he"):
          PrintHelp();
          exit(0);
       case 0x00632D2D x4CHAR("--c"):
        if(argc>2)
        {
            if(!PrepCfg( argv[2] ) )
            {
                if(argv[2][0]!='.' && (t=strrchr(argv[2],'/' )) )
                {
                   *t=0;
                    chdir(argv[2]);
                   *t='/';
                }
                goto lbSkipCfg;
            }


        }

   }
 }

#if defined(CONFIG_CONFIG) && ! defined(CONFIG_CURRENT_DIR)
  if(! PrepCfg( CONFIG_CONFIG "vpnclient.cfg"))
  {
    chdir(CONFIG_CONFIG);
  }
  else
#endif
    //if( (p=stristr(t=argv[0],".ex") ) )
    {
      char *tt;

      t=argv[0];
      //ll = p - t + 1;
      ll = strlen(t) ;
      if(*t == '"') {
        t++;
        ll--;
      }
      tt= (char *) malloc(ll + 8);
      if(!tt) return 1;
      sprintf(tt, "%s.cfg", t, ll);
      //DWORD_PTR(tt[ll])=0x676663 x4CHAR(".cfg");
      if(PrepCfg(tt)) {
        free(tt);
        if(PrepCfg("vpnclient.cfg") )
          if( ! PrepCfg("/etc/shttp/vpnclient.cfg") )
          {
            chdir("/etc/shttp");
          };
      }
    }
 lbSkipCfg:
  InitParam( (char*)argv );
#if defined(CONFIG_CONFIG) && ! defined(CONFIG_CURRENT_DIR)
  if( LoadLangCfg(CONFIG_CONFIG "shs_lang.cfg" ) < 0 )
#endif
    LoadLangCfg("shs_lang.cfg" );

  if(s_flg&FL_HIDE)
  {// Daimonize
    #ifndef LPC_ARM
    if( ( ll=fork() ) )
    { exit(0);
    }
    #else
    daemon(1,1);
    #endif
    if( setsid()<0 )
    {
      printf("Running as daimon\n");
    }
    pipe(tmp);
  }
  puid=getuid();
  ppid=getpid();
  signal(SIGTERM  ,SignalHandler);
  signal(SIGINT   ,SignalHandler);
  signal(SIGQUIT  ,SignalHandler);
  signal(SIGABRT  ,SignalHandler);
  signal(SIGUSR1  ,SignalUSR);
  signal(SIGPIPE  ,SIG_IGN);

  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_sigaction = (tsighandler) signalSegv;
  action.sa_flags = SA_SIGINFO;
  sigaction(SIGSEGV, &action, NULL);

  action.sa_sigaction = (tsighandler) signalChild;
  sigaction(SIGCHLD, &action, NULL);

  //if()
  {
    if(setrlimit(RLIMIT_CORE,  &rlim))
      debug("Cant set ulimit %d %s\n",errno,strerror(errno) )  ;
  }

  if(s_flg&FL_HIDE){s_flg|=FL_NOICON;}
  if(flog)
  {
    struct tm *stm;
    if(stat(flog,&stt)<0)
    {
      if((ll=creat(flog,0600))>=0)
      {
        fstat(ll,&stt);
        _lclose(ll);
      }
    }
    stm = localtime( (time_t *) & (stt.st_ctime));
    if(stm) lastday = stm->tm_mday;
  }
#ifdef  TLSWODLL
  SetPriority(tls_priority);
#else
  if((!TLSLibrary) || !InitSecDLL())
  {
    pprot+=sprintf(pprot,"**Error. Can't load TLS/SSL library\r\n");
  }
  else
#endif
  if(InitLib((TFprintf) &tlsdebug,
              (TFtransfer) (&JustSnd),
              (TFtransfer) (&JustRcv),
              CApath,CAfile,s_cert_file, s_key_file) )
  {
    DBGL("Start")
    VPNClient(0);
    return 0;
  }

  return 1;
};

