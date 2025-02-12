/*
 * Copyright (C) 1999-2024 Maksim Feoktistov.
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


#ifndef SRV_H
#include "srv.h"
#endif
#ifdef TLSVPN
#include "vpn.h"
#endif


inline void InitMemCache()
{
#ifndef SYSUNIX
  if(! (hheap = HeapCreate(0,0x1000, cash_max+1024) ) )
  {err: cash_max=0; return; };
//  if(! (mcache=(CacheTag *)HeapAlloc(hheap,HEAP_ZERO_MEMORY,1024) ) )goto err;
#else
//  mcache=(CacheTag *)Malloc(1024);
#endif
};


#ifndef CD_VER

void Restart()
{
  char *t,*p;
  t=cmdline; //GetCommandLine();
  if(*t=='\"')
  {
    ++t;
    p=strchr(t,'\"');
    if(*p)*p=0;
  }
#ifndef SYSUNIX
  p=stristr(cmdline,".exe");
  if(p) {
    p[4]=0;
  }
#endif
  RestartServer(t,0);
}

void RestartServer(char *u,int cnt)
{int l,i;
  is_no_exit=0;

#ifdef _BSD_VA_LIST_
 if(dnstthr)pthread_kill(dnstthr,9);
#endif

  for(i=0;i<  MAX_SOCK  ; i++) if(soc_srv[i]>0){CloseSocket(soc_srv[i]); }

  Sleep(200);
  if(count_of_tr>cnt)
  {
    ++no_close_req;
    for(i=0;i<max_tsk;++i)
      if(rreq[i] > (Req *)1 && rreq[i]->s > 0) rreq[i]->Shutdown();
    dec_no_close_req();
  }

//  Sleep(0x20);
  Sleep(100);
#ifndef VPNCLIENT_ONLY
  if(total_dhcp_ip)SaveDHCP();
#endif // VPNCLIENT_ONLY
#ifndef SYSUNIX
  CloseService();

/*
  if(sesh)
  {
   l=0;
   HANDLE sh=0;
   SC_HANDLE sch=0;
   if((sch=OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS)))
    if((sh=OpenService(sch,"shttps",SERVICE_START)))
    { l=StartService(sh,1,(const char **)&u);
      CloseServiceHandle(sh);
    }
   if(!l){
    debug("Can't restart self as service %u %u %d",sch,sh,WSAGetLastError());
    MessageBox(0,"Can't restart self as service","Error",MB_OK);
   }
  }
  else
*/
  {PROCESS_INFORMATION pi;
   cbFwd.cb=sizeof(cbFwd);
   cbFwd.dwFlags= STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
   cbFwd.wShowWindow=SW_SHOWNORMAL;
   CreateProcess(0,u,&secat,&secat,1,NORMAL_PRIORITY_CLASS,0,0,&cbFwd,&pi);
  }
#else
  s_aflg|=AFL_RESTART;
  //system(__argv[0]);
#ifndef _BSD_VA_LIST_
  unsave_limit=1;
  //RelProt();
  DoneSepLog();
  sleep(1);
  if(!fork())
#else
#error "exec without fork"
  is_no_exit=0;
  usleep(500000);
  unsave_limit=1;
  //RelProt();
  DoneSepLog();
  sleep(1);
#endif
  { //fprintf(stderr, "Restart... (%d)\n",errno);
    execl(__argv[0],__argv[0],0);
//    fprintf(stderr, "Error, Can't exec %d %s\n",errno,strerror(errno));
    printf( "Error, Can't exec %d %s\n",errno,strerror(errno));
  }
//  sleep(1);
//  fprintf(stderr,"\r\n*** Restarting. Wait for clossing all incomming connections and terminating. (%d)\r\n",errno);
//  sleep(1);
#endif
/*
  Sleep(0x200);
  l=128;
  do
  { Sleep(0x100);
    if(--l<=0)break;
  }while(count_of_tr>cnt);
*/
  CloseServer();
};

#ifndef SYSUNIX

void OkCfgWindow()
{
  CfgParam *cp;
  char  u[512],*t,*x;
  HWND  tw;
  int i,j,k,l,chk=0;

  int lis_no_exit=is_no_exit;
  for(k=0;ConfigParams[k].desc;++k)
  {
    cp = ConfigParams + k;
    if( cp->name )
    {
      if( cp->v )
      {
        GetDlgItemText(dwndc,(k<<3)+800,u,500 );
        if(cp->max)
        {
          if( (l=atoui(u))!=*(cp->v) )
          {
            if((cp->v== (uint *)&time_update) && ((!time_update) || !*(cp->v) ) )lis_no_exit=0;
            *(cp->v)=l;
            if(strstr(cp->name,"max" ) ) lis_no_exit=0;
            if(strstr(cp->name,"port") ) lis_no_exit=0;
            if(strstr(cp->name,"cache") )lis_no_exit=0;
          }
        }
        else
        {
          if( ((*cp->v)?strcmp(u,*(char**)(cp->v)):u[0] ))
          {
            if( (*cp->v) && (strlen(u)<=strlen(*(char**)cp->v)) )
            {
              if(*u)
                strcpy(*(char**)(cp->v),u);
              else
              {
                if(cp->v == (uint *)&cgi_detect )
                  (*(char**)(cp->v))= "\\cgi-bin\\";
                else if(   (cp->v != (uint *)&   def_dir)
                  && (cp->v != (uint *)&  out_path)
                  && (cp->v != (uint *)&  err_path)
                  && (cp->v != (uint *)& smtp_name)
                  && (cp->v != (uint *)& def_name )
                )*cp->v=0;
              }
            }
            else
            {
              //         is_no_exit=0;
              t=new char[i=strlen(u)+3];
              memcpy(t,u,i);
              FREE_IF_HEAP2((*(char**)(cp->v)));
              (*(char**)(cp->v))=t;
            }
          }
        }

        if(cp->fChange)cp->fChange(cp);
      }
      else if(cp->max)
      {
        if( IsDChk(dwndc,(k<<3)+805 ) )s_flgs[cp->min] |=cp->max;
        else s_flgs[cp->min]  &= ~cp->max;
      }
#if 0
      else if(ConfigParams[k+1].v && IsDChk(dwndc,(k<<3)+805 ) )
      {
      if( *ConfigParams[k+1].v && strstr(ConfigParams[k+1].name,"max")  )lis_no_exit=0;
      *ConfigParams[k+1].v=0; ++k;
    }
#endif
    }
  }
  t=new char[0x20000];

  x=SaveCfgFile(t);
  delete t;
  DestroyWindow(dwndc);
  if(!lis_no_exit)
  {
    debug( s_R_N____R );
    Sleep(500);
    is_no_exit=0;

    //RestartServer(x,0);
    Restart();
  }
  else
  {
     CheckValidCGIIdent();
     if( (s_flgs[2]&FL2_DOH) && doh_w<=0 )
     {
        CreatePipe((HANDLE *)&doh_r,(HANDLE *)&doh_w,&secat,0x400);
     }
  }
}
#endif
#else
void Restart()
{
 CloseServer();
}

#endif

int hLock,hcLock;
int FreeThreads()
{
  int i,j=-1;
  MyLock(hLock);
  for(i=0;i<max_tsk;++i)
  {
    if(hndls[i])
    {
      #ifdef SYSUNIX
      if( ((u_long)(rreq[i])) != 1 ) continue;
      pthread_detach((pthread_t) hndls[i]);
      rreq[i]=0;
      #else
      ulong r;
      if( (GetExitCodeThread(hndls[i],&r)) && r == STILL_ACTIVE) continue;
      CloseHandle(hndls[i]);
      #endif
      hndls[i]=0;
    }
    if(j<0)j=i;
  }
  if(j<0 && dos_protect_speed)
  {
    #define  TIME_TO_CHECK_TICK     (300*1024)
    ulong bytes_per_s = (dos_protect_speed<<10)/60;
    ulong tick = GetTickCount();
    ulong time_tick;
    int k;

    ++no_close_req;
    for(i=0; i<max_tsk; ++i)
    {
      if(rreq[i] > (Req *)1)
      {

        if( hndls[i] &&
          (time_tick = DTick(tick,rreq[i]->tmout)) > TIME_TO_CHECK_TICK &&
          rreq[i]->s != -1
        )
        {
          if( ( (u64) rreq[i]->Tin + (u64) rreq[i]->Tout ) < ( (time_tick * bytes_per_s )>>10 ) )
          {
            k = rreq[i]->flsrv[1]&MAX_SERV_MASK;
            #ifdef SEPLOG
            sepLog[k]->Ldebug("**Found DoS connection at thread %u (port: %u)\r\n",i,soc_port[k]);
            #else
            debug("**Found DoS connection at thread %u (port: %u) \r\n",i,soc_port[k]);
            #endif
            rreq[i]->Shutdown();
          }
        }
      }
    }
    dec_no_close_req();

  }
  MyUnlock(hLock);
  return j;
};

int CrThreadFunc(TskSrv Fnc,Req *par)
{
  int i;
  MyLock(hcLock);
  if((i=FreeThreads())>=0)
  {
    par->ntsk = i;
    hndls[i]=
    (THREADHANDLE)
    CreateThread(&secat,0x5000,Fnc,par,0,&trd_id);
  }
  MyUnlock(hcLock);
  // debug("Crth %d) %X; %X %X %X %X %X %X ",i,hndls[i],hndls[0],rreq[0],hndls[1],rreq[1]);
  return i;
}

#ifndef VPNCLIENT_ONLY
int CrThread(uint fnc)
{
 int i;
 MyLock(hcLock);
 if((i=FreeThreads())>=0)
 { hndls[i]=
     (THREADHANDLE)
     CreateThread(&secat,0x5000,(TskSrv)SetServ,(void *)(long)(fnc|i),0,&trd_id);
 }
 MyUnlock(hcLock);
// debug("Crth %d) %X; %X %X %X %X %X %X ",i,hndls[i],hndls[0],rreq[0],hndls[1],rreq[1]);
 return i;
}
#endif // VPNCLIENT_ONLY

char * GetNextBindAddr(char *p, void *sa_server)
{
 char *t;
// debug("GNBA %s",p);
#ifdef USE_IPV6

  if( (t=strpbrk(p,".:")) && *t==':' )
  {
    ((sockaddr_in6 *)sa_server)->sin6_family=AF_INET6;
    p=IPv6Addr((ushort *) &(((sockaddr_in6 *)sa_server)->sin6_addr) ,p);
  }
  else
#endif
  {
    ((sockaddr_in *)sa_server)->sin_family=AF_INET;
    ((sockaddr_in *)sa_server)->sin_addr . s_addr = ConvertIP(p);
  }
//  debug("Next %s",p);

  if(p)
  {
    if(!*p) return 0;
    while( strchr(" ;,\t", *p))
    {p++;
     if(!*p) return 0;
    }
  }
//  debug("Nexttt %s",p);

  return p;

}

//int CreateSrv(int max_c,int flg, int &s,int s_port=80)
static int bind_wait_close;

int ChkWaitBind()
{
   if(!bind_wait_close)
   {
     debug("Wait 3 seconds for close old instance of server and try again...");
     Sleep(3000);
     bind_wait_close++;
     return 1;
   }
   return 0;

}
#ifndef VPNCLIENT_ONLY
int CreateSrv(int j)
{
 int i,s,k=0,jj=j%MAX_SERV;
 char  *pbnd=bind_a[jj];
#ifdef USE_IPV6
union{
  struct sockaddr_in6 sa_server6;
  struct sockaddr_in sa_server;
 };
#else
 struct sockaddr_in sa_server;
// debug("enter 1");
#endif
 do{
//   debug("loop  %X 1",pbnd);
   memset((char *) &sa_server,0,sizeof(
#ifdef USE_IPV6
   sa_server6
#else
   sa_server
#endif
  ) );
  if(pbnd)
    pbnd=GetNextBindAddr(pbnd,&sa_server);

//  debug("loop  %X 2",pbnd);

  sa_server.sin_port=htons(soc_port[jj]);// s_port);
#ifdef USE_IPV6
#ifdef SYSUNIX
  if(s_flgs[2]&(1<<jj) && sa_server.sin_family!=AF_INET)
#else
  if( (pbnd)? (sa_server6.sin6_family==AF_INET6) : (j>=8) )
#endif
  {if((s=socket(AF_INET6,SOCK_STREAM,IPPROTO_TCP //0
      ))<0)
   {
   elip4:
    debug("IPv6 socket error %d " SER " (port %u) ",WSAGetLastError() Xstrerror(errno),soc_port[j]);
//#ifndef SYSUNIX
    goto lip4;
//#else
//    return -1;
//#endif
   }
   sa_server6.sin6_family=AF_INET6;
   if(j>=8)++k;
  }
  else
#endif
  {
  lip4:
   if((s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1)
     die( sCOULD_NOT );
   sa_server.sin_family=AF_INET;
  }
#ifdef SYSUNIX
  fcntl(s, F_SETFD, fcntl(s, F_GETFD) | FD_CLOEXEC);
#endif


  setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char *)&one,sizeof(one));
//  debug("Sock %d %d",s,k);

  //sa_server.sin_addr.s_addr=0; //htonl(INADDR_ANY);

try_bind_again:
  if(bind(s,(struct sockaddr *) &sa_server,
#ifdef USE_IPV6
   (sa_server.sin_family==AF_INET)?sizeof(sa_server):sizeof(sa_server6)
#else
   sizeof(sa_server)
#endif
    ) == -1)
  {
   char addr6[64];
   IP2S(addr6,(sockaddr_in *)&sa_server);
   debug("Could not bind socket to %s:%u",addr6, htons(sa_server.sin_port));
#ifdef USE_IPV6
   if(sa_server.sin_family==AF_INET6)goto elip4;
#endif
   //shutdown( s, 2 );
   //closesocket( (int) s);
   if(ChkWaitBind())goto try_bind_again;
   /*
   if(!bind_wait_close)
   {
     debug("Wait 3 seconds for close old instance of server and try again...");
     Sleep(3000);
     bind_wait_close++;
     goto try_bind_again;
   }
   */

   CloseSocket(s);

   //die( sCOULD_NOT0 );
   goto lbCnt;
  }
  else if(bind_wait_close == 1)
  {
     bind_wait_close++;
     debug("Successfully bind to :%u (second time)", htons(sa_server.sin_port));
  }

  listen(s,3);

  soc_srv[jj+k*MAX_SERV]=s;

#ifdef SYSUNIX
 if(!s)soc_srv[jj+k*MAX_SERV]=dup(s);
#else
  CrThread((jj+k*MAX_SERV)<<16);
#endif
lbCnt:;
  }while(pbnd && ++k < MAX_ADAPT);
  return max_srv[jj];
}

char *srv_str[]=
{
"",
"Proxy",
"FTP",
"SMTP",
"POP3"
,"SSL"
,"Telnet"
};

#endif // VPNCLIENT_ONLY

int InitApplication()
{
#ifndef SYSUNIX
 WNDCLASS wc;
 WSADATA tmp;
 RECT rc;
#endif

 SYSTEMTIME  tm;
 int i,j;
 char *t,*p;

 int l;
#ifndef SYSUNIX

 if( WSAStartup(0x202,&tmp) )
  WSAStartup(0x2,&tmp) ;
#endif

 tmSpd=GetTickCount();

 max_tsk =max_cln+max_ftp+max_pop+max_smtp+max_prx+max_ssl+8;
 hndls=(THREADHANDLE*) (
// saddr=(
//        tmout=(ulong *) (  (wsock=new int[max_tsk*4+20]
//                           )  + max_tsk)
//       ) + max_tsk
  (rreq=(Req **) (new void *[max_tsk*2+32]
  )             ) ) + max_tsk+16;

#ifndef SYSUNIX
// wc.cbSize=sizeof(wc);
 //GetClassInfo(0,"STATIC",&wc);
 wc.style = CS_HREDRAW | CS_VREDRAW;
 wc.lpfnWndProc =  DefProc;
 wc.cbClsExtra = 0;
 wc.cbWndExtra = 0;
 wc.hInstance = hinstance;
/*
 t=(char *)icn;
 if((GetVersion() & 0x80000000))
 {
  t=(char *)icn2;
  DWORD_PTR(icn_ch[0])=0x04011010;
 }
*/
 wc.hIcon=hicon=
#ifndef RICON
 CreateIcon(hinstance,
// 16,16,1,4,(uchar *)b_prot,icn);
// 32,32,1,4,(uchar *)b_prot,icn);
//  32,32,1,1,(uchar *)b_prot,icn);
//  icn_ch[0],icn_ch[1],icn_ch[2],icn_ch[3],(uchar *)b_prot,(uchar *)t);
  icn[128],icn[129],icn[130],icn[131],(uchar *)b_prot+4096,(uchar *)icn);
// (uchar *)icn,icn);
#else
 LoadIcon(hinstance,MAKEINTRESOURCE(502));
#endif

 wc.hCursor = LoadCursor( NULL, IDC_ARROW );
 wc.hbrBackground =
   //GetStockObject(LTGRAY_BRUSH);
   (HBRUSH) (COLOR_BTNFACE+1) ;
   //( COLOR_WINDOW );
 wc.lpszMenuName = NULL;
 wc.lpszClassName = "FMFROM";
// wc.cbSize=sizeof(wc);
// wc.hIconSm=0;

 if( RegisterClass( &wc ) )
 {

  leenv=arstrlen(eenv=GetEnvironmentStrings());

  hProcess=GetCurrentProcess();
#endif // SYSUNIX

#ifdef SEPLOG
#define  pprot  gLog.lpprot
#define  GetProt() gLog.GetProt()
#define  RelProt() gLog.RelProt()
  InitSepLog();
#endif


  GetProt();
  pprot+=sprintf(pprot,"%s\r\n\r\n",about);
  i=0;
#ifndef VPNCLIENT_ONLY

  if(max_srv[0])
  {
   pprot+=sprintf(pprot,
#ifdef RUS
  "HTTP "
  "Стартовый каталог=%s по умолчанию=%s "
#else
   sROOT_DIR_
#endif
   ,def_dir,def_name);
  }
  for(j=0;j<7;++j)
   if(j!=5 && max_srv[j])
   {i+= max_srv[j]= CreateSrv(j);
    if(max_srv[j])
    pprot+=sprintf(pprot,
#ifdef RUS
"%s порт=%u Количество подключений=%d\r\n"
#else
 s_S_PORT__
#endif
    ,srv_str[j],soc_port[j],max_srv[j]);
    if(s_flgs[2]&(1<<j))
#ifndef SYSUNIX
      if( bind_a[j] || CreateSrv(j+MAX_SERV)>0)
#endif
         pprot+=sprintf(pprot-2, " (+IPv6)\r\n" )-2;
   }

   if(max_prx)
   {if(cash_max=cash_max_kb*1024)InitMemCache();
   }
#if V_FULL
   if(max_smtp)
   {HSmtpSendThrd=(THREADHANDLE)CreateThread(&secat,0x2000,(TskSrv) SMTPcl,0,0,&trd_id);
#ifndef SYSUNIX
    mnu2[10].flg=0;
#endif
    pprot+=sprintf(pprot,
#ifdef RUS
"Имя почтового домена=\"%s\" Папки: исходящие=\"%s\" отправленные=\"%s\" ошибки=\"%s\"\r\n"
#else
 sMAIN_DOMA
#endif
   ,smtp_name,out_path,sent_path?sent_path:"",err_path);
   }

#endif // V_FULL

// debug("*** %X %X ***",s_flgs[1],FL1_SMTPTLS&s_flgs[1]);

//#ifndef VPNCLIENT_ONLY
 if(max_srv[5] || (FL1_SMTPTLS&s_flgs[1]) || (s_flgs[2]&FL2_FTPTLS) || (vpn_remote_host && vpn_remote_host[0]) )
#endif // VPNCLIENT_ONLY
 {
   //pprot+=sprintf(pprot,"Loading TLS/SSL library...\r\n");
   #ifdef SYSUNIX
   RelProt();
   oldprot=pprot;
   #endif
#ifdef  TLSWODLL
   SetPriority(tls_priority);
#else
   if((!TLSLibrary) || !InitSecDLL())
   {
     pprot+=sprintf(pprot,"**Error. Can't load TLS/SSL library\r\n");
   }
   else
#endif
   {
    // pprot+=sprintf(pprot,"Load TLS/SSL library initting...\r\n");

     if(
       InitLib((TFprintf) &tlsdebug, (TFtransfer) (&JustSnd), (TFtransfer) (&JustRcv),
               //   InitLib((TFprintf) &debug, (TFtransfer) (&Req::JustSend), (TFtransfer) (&Req::JustRecv),
               //   InitLib((TFprintf) &debug, dynamic_cast<TFtransfer>(&Req::JustSend), (TFtransfer)  static_cast<void *>(&Req::JustRecv),
               CApath,CAfile,s_cert_file, s_key_file)
     )
     {
       pprot+=sprintf(pprot,"TLS/SSL library loadded\r\n");
       #ifndef VPNCLIENT_ONLY
       if(FL1_SMTPTLS&s_flgs[1])s_aflg|=AFL_TLS;
       if(max_srv[5])
       {
         i+= max_srv[5]=CreateSrv(5);
         pprot+=sprintf(pprot,
                        #ifdef RUS
                        "%s порт=%u Количество подключений=%d\r\n"
                        #else
                        s_S_PORT__
                        #endif
                        ,"TLS/SSL",soc_port[5],max_srv[5]);
         if(s_flgs[2]&(1<<5))
           #ifndef SYSUNIX
           if(CreateSrv(5+MAX_SERV)>0)
           #endif
             pprot+=sprintf(pprot-2, " (+IPv6)\r\n" )-2;

       }
       #endif // VPNCLIENT_ONLY

       //   else printf("Tls server dont using %X\n",s_flgs[2]);
     }
     else
     {
       pprot+=sprintf(pprot,"Cant init TLS/SSL library\r\n");
     }
   }
     #ifdef SYSUNIX
     if(oldprot!=pprot)RelProt();
     GetProt();
     #endif
 }
 afterTLS:
 sprintf(sprt80 ,"%u",http_port);
 sprintf(sprt443,"%u",ssl_port);

#if 0
 //ndef SYSUNIX
  if(time_update && dns_user && sd)
  {struct sockaddr_in sa;
    getsockname(sd,( sockaddr *)&sa,&(i=sizeof(sockaddr) ) );
    CreateThread(&secat,0x1000,SetDNSCall,(void *)0,0,&trd_id);
    pprot+=sprintf(pprot,
#ifdef RUS
    "Динамический DNS включен. URL=\r\n");
#else
     sDINAMIC_D );
#endif
    pprot+=sprintf(pprot,dns_user,
#ifndef SYSUNIX
     sa.sin_addr.S_un.S_un_b.s_b1,sa.sin_addr.S_un.S_un_b.s_b2,
     sa.sin_addr.S_un.S_un_b.s_b3,sa.sin_addr.S_un.S_un_b.s_b4);
#else
     sa.sin_addr.s_addr&0xFF, BYTE_PTR(sa.sin_addr.s_addr,1),
     BYTE_PTR(sa.sin_addr.s_addr,2), BYTE_PTR(sa.sin_addr.s_addr,3));
#endif
    DWORD_PTR(*pprot)= 0x0A0D;
    pprot+=2;
  };
#endif //DDNS

#ifndef VPNCLIENT_ONLY
  if((dns_file) && InitDnsSrv() )
  { pprot+=sprintf(pprot,
#ifdef RUS
"DNS включен. Загружено %u хостов.\r\n"
#else
 sDNS_IS_EN
#endif
,count_dns );
//#endif
#ifndef SYSUNIX
   mnu2[11].flg=0;
#endif
  }
#if V_FULL
  if(total_dhcp_ip && InitDHCP()>0)
  {
   CreateThread(&secat,0x1000,DHCPServ,(void *)0,0,&trd_id);
   pprot+=sprintf(pprot,
#ifdef RUS
"DHCP включен. %u IP для выделения .\r\n"
#else
"DHCP enabled. %u IPs for allocate\r\n"
#endif
,total_dhcp_ip );
  }
#endif // V_FILL
#endif // VPNCLIENT_ONLY

#ifdef TLSVPN
#ifndef VPNCLIENT_ONLY
  if( max_srv[SRV_SSL] )
  {
    if(VPN_Init()!=-1)
    {
      pprot+=sprintf(pprot, "TLS VPN enabled\r\n" );
    }
  }
#endif // VPNCLIENT_ONLY

#if defined(SYSUNIX) || !defined(VPNCLIENT_ONLY)
  if(vpn_remote_host && vpn_remote_host[0])
  {
    CreateThread(&secat,(0x5000 + sizeof(VPNclient) + MAX_MTU + 0xFFF)& ~0xFFF ,VPNClient,(void *)0,0,&trd_id);
    pprot+=sprintf(pprot, "TLS VPN client started\r\n" );
  }
#endif

#endif // TLSVPN

#ifndef SYSUNIX
#ifndef VPNCLIENT_ONLY
  if(maxKeepAlive) CreateThread(&secat,0x2000,(TskSrv) KeepAliveThread,0,0,&trd_id);
#endif // VPNCLIENT_ONLY

  mwnd=CreateWindowEx( 0 /*((s_flg&0x10)>>4)*WS_EX_TOOLWINDOW*/ ,"FMFROM",wnd_name,
                       DS_3DLOOK|WS_CLIPCHILDREN|WS_CAPTION|WS_POPUP|WS_SYSMENU|WS_MINIMIZEBOX|WS_DLGFRAME|((wstate=FL_HIDE&s_flg)?0:WS_VISIBLE),
                       0,0,480,480,0,0, hinstance, 0);
  GetClientRect(mwnd,&rc);

  hmnu=MkMnu(mnu2,CreatePopupMenu);
  /*
  mnu3[0].id=(ulong)hmnu;
  hmmnu=MkMnu(mnu4,CreateMenu);
  */
#ifdef VPNCLIENT_ONLY

        CreateWindowEx(0,"BUTTON", "VPN Connect",
                        WS_BORDER|WS_CHILD|WS_VISIBLE, 4, 4,132,20, mwnd,(HMENU) 98 ,hinstance, 0);

        CreateWindowEx(0,"BUTTON", "Settings",
                        WS_BORDER|WS_CHILD|WS_VISIBLE, 144,4,132,20, mwnd,(HMENU) 126 ,hinstance, 0);

        CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC", "In: 0  Out:0",
                        WS_BORDER|WS_CHILD|WS_VISIBLE, 282, 2, rc.right - 282,20, mwnd,(HMENU) 97 ,hinstance, 0);

   rc.top += 28;
   rc.bottom -= 28;
#endif // VPNCLIENT_ONLY
  ewnd=CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT",b_prot,
        WS_BORDER|WS_CHILD|WS_VISIBLE|ES_MULTILINE|
        ES_READONLY|WS_VSCROLL,
        rc.left,rc.top,rc.right,rc.bottom,
        mwnd,(HMENU) //hmmnu
         101
        ,hinstance, 0);
  InsertMenu(GetSystemMenu(mwnd,0),0,MF_POPUP|MF_BYPOSITION|MF_HILITE,(uint)hmnu,"&Server");

  nid.hWnd=mwnd; nid.hIcon=hicon;
  if(! (s_flg&FL_NOICON) )s_aflg|=!Shell_NotifyIcon(NIM_ADD,&nid);
  SendMessage(ewnd,WM_SETFONT,(ulong)GetStockObject(17),1);
#if defined(CD_VER) && (! defined(SYSUNIX)) && ! defined(SPECIAL)

// char bb[40];
// sprintf(bb+30,":%u",http_port);
// if(http_port==80)bb[30]=0;
// sprintf(bb,"http://127.0.0.1%s/",bb+30);
// ShellExecute(0,"open",bb,0,0,SW_SHOWNORMAL);
#endif
  RelProt();
  return 1;
 };
 return 0;
#else
 pprot+=sprintf(pprot,"Started.. \n\n");
 RelProt();
 return 1;
#endif
}

#undef pprot
#undef RelProt
#undef GetProt

