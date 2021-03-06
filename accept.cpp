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

#ifndef STRING_CONST_H
#include "g4strc.h"
#endif

#ifndef SRV_H
#include "srv.h"
#endif

#define  SYN_TIME1 300
#define  SYN_TIME2 30
#define  SYN_FUNC() GetTickCount()

#ifndef SYSUNIX


#ifndef SELECT1
#ifdef MINGW
#undef fd_set 
#endif
int RESelect(long tv_sec,long tv_usec,int n,...)
{return select(0,(fd_set *)&n,0,0,(timeval*)&tv_sec);};
int WESelect(long tv_sec,long tv_usec,int n,...)
{return select(0,0,(fd_set *)&n,0,(timeval*)&tv_sec);};

#else


int RESelect1(long tv_sec,long tv_usec,int s)
{fd_set t;
 t.fd_count=1;
 t.fd_array[0]=s;
 return select(0,(fd_set *)&t,0,0,(timeval*)&tv_sec);
};
#define RESelect(a,b,c,d) RESelect1(a,b,d)
#endif
#endif


#ifdef USE_FUTEX
const  struct timespec timeout_10ms={0,10000000};
#endif
struct linger lngr={1,0};
int MutexEr;
int SrvChecker[16];
int SrvErr[16];


#if V_FULL
int (Req::*FWrk[])()=
{&Req::HttpReq,&Req::HttpReq,&Req::FTPReq,&Req::SMTPReq,&Req::POPReq,&Req::TLSReq
#ifdef TELNET    
    ,&Req::TReq
#endif    
};
#endif
int no_close_req=0;
int err_cnt;

int WINAPI SetServ(uint fnc)
{int clen,j,min_tick,min_tick_tsk;
 ulong tck,k;
 union{int i; ulong *pi;};
#define sa_client req.sa_c
 int l,rq_cnt,serv,serv2;
 Req req;

 memset(&req,0,sizeof(req) );

 serv=(serv2=fnc>>16)%MAX_SERV;
 req.ntsk=(fnc&0xFFF);
 runed[serv]++;
 req.timout=30;
// debug("Acc1 %d %d %X waited[serv2]=%u max_srv[serv]=%u runed[serv]=%u",serv,serv2,fnc,waited[serv2],max_srv[serv],runed[serv]);
#ifndef SYSUNIX
 while(is_no_exit){
  waited[serv2]++;
#else
  waited[serv]=max_srv[serv]-runed[serv];
#endif
  clen=sizeof(
#ifdef USE_IPV6
  sockaddr_in6
#else
  sockaddr_in
#endif
  );
  if((req.s=accept( soc_srv[serv2],(sockaddr *)&sa_client,&clen)) <0)
  {

   if(!is_no_exit)
#ifndef SYSUNIX
   break;
#else
   return 0;
#endif
   debug( "Error %d. Can't accept (port %u,%d,%d,%d,%X)" SER ,WSAGetLastError(),soc_port[serv],serv2,soc_srv[serv2],clen,sa_client.sin_addr. S_ADDR  Xstrerror(errno) );

   Sleep(10);
   if(MutexEr)return -1;
   MyLock(MutexEr);
   closesocket(soc_srv[serv2]);
   debug( "Reinit port %u",soc_port[serv]);

   if( (SrvErr[serv]++)>2 && SrvChecker[serv]>3 )
   {
    MyUnlock(MutexEr);
    Restart();
    return -1;
   }
   CreateSrv(serv2);
   MyUnlock(MutexEr);
   return -1;
  }
  SrvChecker[serv]++;
  SrvErr[serv]=0;
#ifdef SYSUNIX
  if(!req.s)req.s=dup(req.s);
#endif
  req.tmout=GetTickCount();
  req.bSpd=0;
  req.Tout=0;
  req.Tin=0;
  DWORD_PTR(req.inf[0])=0;
#ifdef FIX_EXCEPT
  req.thread_id=0;
#endif  
  rreq[req.ntsk]=&req;
  
  count_of_tr++;


#ifndef SYSUNIX
  if( (waited[serv2]<=1) && (max_srv[serv]>runed[serv]) )CrThread(fnc&0xFFFF0000);
  --waited[serv2];
#endif
//debug("Accept %d(port %d) thread:%u",serv,soc_port[serv],count_of_tr);
  req.fl=fnc&0xFFFF0000;
  req.Snd=(tfSnd) &JustSnd;
  req.Rcv=(tfRcv) &JustRcv;

  j=0;
#ifndef CD_VER
  LimitCntr *lip;
  if((lip=hack.Find(sa_client.sin_addr. S_ADDR )
     ) && (lip->cnt&0x0F)>=12)
  {req.HttpReturnError("DETECTED HACKER");
   i=-1;
   goto cnt;
  }
#else
  if(sa_client.sin_addr. S_ADDR == 0x7F000001)
  {
   req.HttpReturnError("Access deny");
   i=-1;
   goto cnt;

  }
#endif
  if( (req.IsInIPRange(ip_range)<=0 
      )   ||
     ( serv!=SMTP_N &&
       (req.IsInIPRange(serv<<1)<=0 
     ) )              ||
     req.IsProxyRange(serv<<1)
  )
  { req.HttpReturnError( sACCESS_DE );
//    setsockopt(req.s,SOL_SOCKET,SO_LINGER,(char *)&lngr,sizeof(lngr));
    i=-1;
    goto cnt;}
#ifndef CD_VER

  if(max_cln_host)
  {
   ++no_close_req;
   for(i=0;i<max_tsk;i++)
   if( ((u_long)(rreq[i])>1) && (rreq[i]->sa_c.sin_addr. S_ADDR==sa_client.sin_addr. S_ADDR ) )
   {if((++j)>=max_cln_host)
    {req.HttpReturnError( sTOO_MANY_ );
//     setsockopt(req.s,SOL_SOCKET,SO_LINGER,(char *)&lngr,sizeof(lngr));
     --no_close_req;
     i=-1;
     goto cnt;
    }
   }
   if(--no_close_req<0)no_close_req=0;
  }
#elif 0
  if(sa_client.sin_addr. S_ADDR !=0x0100007F)
  {req.HttpReturnError( sACCESS_DE0 ); goto cnt;}
#endif


#ifdef FIX_EXCEPT

 
 
 if(! setjmp( req.jmp_env ) ) { req.thread_id=GetCurrentThreadId();  }
 else 
 { 
     
     req.thread_id=0;  
     if(err_cnt++>3)
     {
         
      is_no_exit=0;
      s_aflg|=AFL_EXIT;
      StopSocket();
      unsave_limit=1;      
      sleep(3);   
      if( !vfork() )
         execl(__argv[0],__argv[0],0);
      exit(0);      
    
     }
     debug("Try to recovery after exception (%u)\n",serv);
     goto cnt;  
 }  
 #define  END_TRY(th)  (th)->thread_id=0;
 
#else
  #define  EXCEPT_TRY(th,x...) 
  #define  END_TRY(th)  
#endif
 
  
#if V_FULL
  i=(req.*FWrk[serv])();
#else
  req.HttpReq();
#endif

cnt:
  j=0;
  while(no_close_req>0)
  {
#ifdef USE_FUTEX
    if(++j>100)
    {
     no_close_req=0;
     break;
    }
    futex((int *)&no_close_req,FUTEX_WAIT,no_close_req,&timeout_10ms,0,0);  
#else      
    Sleep(1);  
    if(++j>1000)
    {
     no_close_req=0;
     break;
    }
#endif      
  }
  
  if(rreq[req.ntsk]==&req)
  {    
   rreq[req.ntsk]=0;
   
   count_of_tr--;
   if(count_of_tr<0)count_of_tr=0;
   
  }

/*
  shutdown(req.s,2);
  if(i<0 && runed[serv]>8 )
  {
   Sleep(128);
   setsockopt(req.s,SOL_SOCKET,SO_LINGER,(char *)&lngr,sizeof(lngr));
  }
  closesocket( req.s );
 */
   CloseSocket( req.s );

#ifndef CD_VER
  if(serv==1)proxy_chk.CheckProxy();
#endif

#ifndef SYSUNIX
  if(waited[serv2]>0)
  {
    //debug("cl %u %u %u",serv,serv2,waited[serv2] ) ;

    FreeThreads();
   runed[serv]--;
   break;
  }
 }
#else
 runed[serv]--;
 waited[serv]=max_srv[serv]-runed[serv];
 rreq[req.ntsk]=(Req *)1;
 pthread_exit(0);
#endif
 return 0;
#undef sa_client
};



int Req::HttpReturnError(char *err,int errcode)
{char out_buf[2124];
 int l;
 AddToLog(err,s,FmtShortErr);
 SendChk(out_buf,sprintf(out_buf,
 (l=fl&0xE0000)? //2,3,4
  "%u %s\r\n\r\n":"HTTP/1.0 %u Error\r\nContent-Type: text/html\r\n\r\n"
 "<hr><pre><font size=+2><b>\n%.512s\n</b></font></pre><hr><br>%450s",errcode,err,""));
// setsockopt(s,SOL_SOCKET,SO_LINGER,(char *)&lngr,sizeof(lngr));
 return 1;
};

ulong tmSpd;
int   SpdMut;
int Req::SleepSpeed()
{ulong uspd;
 if(!(uspd=ipspeed[flsrv[1]&7]))return 0;
 Req *r;
 ulong sum=0,j;
 ulong tt;
 long x,i,y;
 ulong z;
 //tmSpd=GetTickCount();
 MyLock(SpdMut);
 tt=GetTickCount();
 z=tt-tmSpd;
 if(z>1024 || !tmSpd)tmSpd=tt;
 MyUnlock(SpdMut);
 j=0;
 ++no_close_req;
 for(i=0;i<max_tsk;++i)
  if( (u_long)(r=rreq[i])>1 )
  {x=tt-r->tmout;
   if(x<0) x=-x;
   //if(x>0)
   {
    y=r->Tout;
    if(x>0x4000)
    {
     y-=r->bSpd;
     if(y<0)y=0; else
     if(z>1024)
     {r->bSpd+=MULDIV(z,y,0x4000+z);
      if((y=r->Tout-r->bSpd)<0)y=0;
     }
    }
    if(r->sa_c.sin_addr.s_addr==sa_c.sin_addr.s_addr)sum+=y;
    else if(y>0x400)++j;
   }
  }
  --no_close_req;
 sum>>=8;
 if((x=sum-uspd)>0 && j>=ipspdusr[flsrv[1]&7]){ Sleep(MULDIV(x,0x4000,uspd)); return 1;}
 return 0;
}


int BSend(int s,char *b,int l)
{int i;
 char *bb=b;
#ifndef SPECSEND
 while(l>0)
 {if((i=send(s,bb,l,0))<=0) return i;
  l-=i;
  DWORD_PTR(bb)+=i;
 }
#else
 ioctlsocket(s,FIONBIO,(ulong *) &one);
 while(l>0)
 {if(WESelect(120,0,1,s)<=0)break;  //return -1;
  if((i=send(s,bb,l,0))<=0) return i;
  l-=i;
  DWORD_PTR(bb)+=i;
 }
 ioctlsocket(s,FIONBIO,(ulong *) &NullLong);
#endif

 return bb-b;
}

int JustSnd(Req *th,const void *b,int l)
{int r;
 char chu[16];
 th->SleepSpeed();
 if(th->fl & F_CHUNKED )
 {
   if( (r=BSend(th->s,chu,sprintf(chu,"%X\r\n",l) ))<=0 ) return r;
   th->Tout+=r;
 }
 if((r=BSend(th->s,(char *)b,l))>0)
 {
    th->Tout+=r;
    if( th->fl & F_CHUNKED )
    {
        BSend(th->s,"\r\n",2);
        th->Tout+=2;
      //  debug("Chu Send %X %.5s",l,b);
    }
 }
 return r;
};
int JustRcv(Req *th,void *b,int l)
{int r;
 if(RESelect(th->timout,0,1,th->s)<=0)return -1;
 if((r=recv(th->s,(char *)b,l,0))>0 )th->Tin+=r;
 return r;
};


int Req::IsProxyRange(int a)
{
 if(a==proxy_range || !max_prx)return 0;
 Req *r;
 int i;
 ++no_close_req;
 for(i=0;i<max_tsk;++i)
 {
  if( ((u_long)(r=rreq[i]))>1 && (r->fl&0xF0000)==F_PROXY &&
     r->trn &&
#ifdef x86_64
     ((ulong)(u_long)(r->req_var))==sa_c.sin_addr. S_ADDR &&
     ((ulong)(u_long)(r->pst))==sa_c.sin_port
#else
     ((ulong)(r->req_var))==sa_c.sin_addr. S_ADDR &&
     ((ulong)(r->pst))==sa_c.sin_port
#endif
  )
  {i=IsInIPRange(a);
   --no_close_req;
   return i<=0;
  }
 }
 --no_close_req;
 return 0;
};

int FndLimit(int lst,LimitCntr **ip, LimitCntr **net, ulong i)
{ulong x;
#ifndef CD_VER
 *ip=0;
 *net=0;
 if(!(ltime[lst]))return 0;
 LimitCntr *lip,*lnet;
 if(!ipcnts[lst].n)ipcnts[lst].Push();
 if(ipcnts[lst].d[0].CheckLimit(limit[lst],ltime[lst]))
 {
 lErrLim:
   Sleep(0x4000);
 //  Send("421 limit overflow\r\n",sizeof("421 limit overflow\r\n")-1);
   return 1;
 }
 x=time(0)-ltime[lst];
 if(!(lip= ipcnts[lst].Find(i))) 
 { ipcnts[lst].FreeOld(x); lip=ipcnts[lst].Push(); lip->ip=i;}
 i&=0xFFFFFF; 
 if(!(lnet=ipcnts[lst].Find(i)))
 { lnet=ipcnts[lst].Push(); lnet->ip=i;}
 *ip =lip ;
 *net=lnet;
 if(lnet->CheckLimit(net_limit[lst],ltime[lst])) goto lErrLim;
 if(lip->CheckLimit(ip_limit[lst],ltime[lst])) goto lErrLim;
#endif
 return 0;
}


void CloseSocket(int s)
{
#undef shutdown
 shutdown(s,3);
 closesocket(s);

}

