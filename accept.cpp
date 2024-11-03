/*
 * Copyright (C) 1999-2023 Maksim Feoktistov.
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

#ifdef MINGW
#undef fd_set
#define win_fd_set fd_set
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
int close_wait;
int err_cnt;

void no_close_wait()
{
  int j=0;
  while(no_close_req>0)
  {
#ifdef USE_FUTEX
    if(++j>100)
    {
     no_close_req=0;
     break;
    }
    close_wait++;
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
}

void dec_no_close_req()
{
#ifdef USE_FUTEX
  if( --no_close_req <= 0)
  {
    no_close_req = 0;
    if(close_wait)
    {
      close_wait = 0;
      futex((int *)&no_close_req,FUTEX_WAKE,1,0,0,0);
    }
  }
#else
  if(--no_close_req<0)no_close_req = 0;
#endif
}

void SetKeepAliveSock(int s)
{
  int v;
  setsockopt(s,SOL_SOCKET,SO_KEEPALIVE,(char *)&one,sizeof(int));
  if(keepalive_idle)
  {int v = 3;
    setsockopt(s, IPPROTO_TCP, TCP_KEEPIDLE,(char *)&keepalive_idle,sizeof(int));
    setsockopt(s, IPPROTO_TCP, TCP_KEEPINTVL,(char *)&keepalive_idle,sizeof(int));
    setsockopt(s, IPPROTO_TCP, TCP_KEEPCNT,(char *)&v,sizeof(int));
  }
}


void AddKeepAlive(Req* preq)
{
  MyLock(KeepAliveMutex);
  KeepAliveList[KeepAliveCount] = preq;
  KeepAliveCount++;
  maxKeepAliveSet.Set(preq->s);
  MyUnlock(KeepAliveMutex);
  SetKeepAliveSock(preq->s);
}

void RemoveKeepAlive(int i)
{
  Req *preq;
  int unlock = MyLock(KeepAliveMutex);
  preq = KeepAliveList[i];
  maxKeepAliveSet.Clear(preq->s);
  if(KeepAliveCount)
  {
    KeepAliveCount --;
    if(i != KeepAliveCount)
      KeepAliveList[i] = KeepAliveList[KeepAliveCount] ;
  }
  if(unlock) MyUnlock(KeepAliveMutex);
}

void Req::Close()
{
  if(s != -1)
  {
    if(Snd==&TLSSend)
    {
      SecClose((OpenSSLConnection*)Adv);
      Snd=(tfSnd) &JustSnd;
    }
    CloseSocket(s);
    s = -1;
  }
};


void DeleteKeepAlive(Req* preq)
{
  preq->Close();
  DBGLA("free %lX", (long)preq)
  free(preq);
}

void RemoveAndDelKeepAlive(int i)
{
  Req *preq;
  int unlock = MyLock(KeepAliveMutex);
  preq = KeepAliveList[i];
  RemoveKeepAlive(i);
  if(unlock) MyUnlock(KeepAliveMutex);
  DeleteKeepAlive(preq);
}

// Function return -1, if remove anything, index of oldest if not
int RemoveExpired()
{
  int r=0,i;
  int deleted=0;
  int fix_max_expired=0;
  int fix_max_fd=0;
  int max=KeepAliveList[0]->KeepAliveExpired;
  time_t tt = time(0);
  DBGL("rm exp")
  MyLock(KeepAliveMutex);
  for(i=0; i<KeepAliveCount; i++)
  {
    Req *preq;
    preq = KeepAliveList[i];
    if(preq->KeepAliveExpired < tt) {

      if(keep_alive_max_fd == preq->s) fix_max_fd++;
      FD_CLR(preq->s, &KeepAliveSet);
      if(KeepAliveCount)
      {
        KeepAliveCount --;
        if(i != KeepAliveCount)
          KeepAliveList[i] == KeepAliveList[KeepAliveCount] ;
      }
      DeleteKeepAlive(preq);
      deleted++;
    }
    if(fix_max_expired < preq->KeepAliveExpired )
    {
      fix_max_expired = preq->KeepAliveExpired;
      r = i;
    }
  }
  if(fix_max_fd) maxKeepAliveSet.Fix();
  MyUnlock(KeepAliveMutex);
  return deleted? -1 : r;
}
void RemoveOldestIfNeed()
{
  if(KeepAliveCount >= maxKeepAlive)
  {
    int oldest;
    oldest = RemoveExpired();
    if(oldest >= 0)
    {
      RemoveAndDelKeepAlive(oldest);
    }
  }
}


int TryToAddKeepAlive(Req *req)
{
  Req *preq;
  int l = sizeof(Req);
  if(req->s == -1) return 1;
  DBGL("add keep alive required");
  if( !KeepAliveList ) goto lbCanT;
 // debug("KeepAlive connection %d\n", req->s);
  RemoveOldestIfNeed();
  if(req->Snd==&TLSSend)
    l = sizeof(Req) + sizeof(OpenSSLConnection);
  if(KeepAliveCount < maxKeepAlive && (preq = (Req *) malloc(l) ))
  {
    memcpy(preq, req, offset(Req, loc));
    memset(&preq->loc, 0, sizeof(Req) - offset(Req, loc));
    if(req->Snd == &TLSSend)
    {
      OpenSSLConnection *o;
      o = (OpenSSLConnection *) (preq + 1);
      memcpy(o, req->Adv, sizeof(OpenSSLConnection) );
      o->CallbackParam = preq;
      preq->Adv = o;
      SecUpdateCB(o);
    }
    AddKeepAlive(preq);
  }
  else
  {
  lbCanT:
    //if(req->Snd==&TLSSend)SecClose((OpenSSLConnection*) req->Adv);
    req->fl &= ~F_KEEP_ALIVE;
    return 0;
  }
  req->s = -1;
  req->Adv = 0;
  return 1;
}

#ifdef FIX_EXCEPT
int SetFixExept(Req *preq)
{
  if(! setjmp( preq->jmp_env ) ) { preq->thread_id=GetCurrentThreadId(); return 0; }
  preq->thread_id=0;
  if(err_cnt++>3)
  {
    is_no_exit = 0;
    s_aflg |= AFL_EXIT;
    StopSocket();
    unsave_limit=1;
    sleep(3);
    if( !vfork() )
      execl(__argv[0],__argv[0],0);
    exit(0);
  }
  debug("Try to recovery after exception (%u)\n", preq->flsrv[1]& MAX_SERV_MASK);
  return -1;
}
#endif

int WINAPI SetServ(uint fnc)
{int clen,j,min_tick,min_tick_tsk;
 ulong tck,k;
 union{int i; ulong *pi;};
#define sa_client req.sa_c
 int l,rq_cnt,serv,serv2;
 Req req;

 memset(&req,0,sizeof(req) );

 serv=(serv2=fnc>>16) % MAX_SERV;
 req.ntsk=(fnc&0xFFF);
 runed[serv]++;
 req.timout=30;
// debug("Acc1 %d %d %X waited[serv2]=%u max_srv[serv]=%u runed[serv]=%u",serv,serv2,fnc,waited[serv2],max_srv[serv],runed[serv]);
#ifndef SYSUNIX
 while(is_no_exit){
  DBGLA("fnc = %X serv:%u", fnc, serv)
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
  fcntl(req.s, F_SETFD, fcntl(req.s, F_GETFD) | FD_CLOEXEC);
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
  req.fl = serv << 16; // fnc&0xFFFF0000;

  DBGLA("fnc:%X fl:%X port:%u", fnc, req.fl, soc_port[serv])

  req.Snd=(tfSnd) &JustSnd;
  req.Rcv=(tfRcv) &JustRcv;



  j=0;
#ifndef CD_VER
#ifdef USE_IPV6
  union{
    LimitCntr *lip;
    LimitCntrIPv6 *lip6;
  };
#else
  LimitCntr *lip;
#endif

  if( (lip) && (lip->cnt&0x0F)>=12)
  {req.HttpReturnError("DETECTED HACKER");
   i=-1;
   goto cnt;
  }
#else // CD_VER
  if( IPv4addr(&sa_client) //sa_client.sin_addr. S_ADDR
      == 0x7F000001)
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
    {
      k =   IPv4addr(&sa_client);
      for(i=0;i<max_tsk;i++)  if( ((u_long)(rreq[i])>1) &&
        //       (rreq[i]->sa_c.sin_addr. S_ADDR == sa_client.sin_addr. S_ADDR )
        (
          #ifdef USE_IPV6
          (k == 1)? ( IsIPv6(& rreq[i]->sa_c) && !memcmp( &rreq[i]->sa_c6.sin6_addr, & req.sa_c6.sin6_addr, 16 ) ) :
          #endif
          ( IPv4addr(& rreq[i]->sa_c) == k )
        )
      )
      {
        if((++j)>=max_cln_host)
        {
          req.HttpReturnError( sTOO_MANY_ );
          //     setsockopt(req.s,SOL_SOCKET,SO_LINGER,(char *)&lngr,sizeof(lngr));
          dec_no_close_req();
          i=-1;
          goto cnt;
        }
      }
      if(KeepAliveCount)
      {
        MyLock(KeepAliveMutex);
        for(i=0; i<KeepAliveCount; )
        {
          Req *preq;
          preq = KeepAliveList[i];
          if (
            #ifdef USE_IPV6
            (k == 1)? ( IsIPv6(& preq->sa_c) && !memcmp( &preq->sa_c6.sin6_addr, & req.sa_c6.sin6_addr, 16 ) ) :
            #endif
            ( IPv4addr(&preq->sa_c) == k )
          )
          {
            ++j;
            if(j>3)RemoveAndDelKeepAlive(i);
          }
          else { i++; }
        }
        MyUnlock(KeepAliveMutex);
      }
    }
    dec_no_close_req();
  }
#elif 0
  if(sa_client.sin_addr. S_ADDR !=0x0100007F)
  {req.HttpReturnError( sACCESS_DE0 ); goto cnt;}
#endif


#ifdef FIX_EXCEPT


 if(SetFixExept(&req) ) goto cnt;
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
  if( (req.fl & (F_KEEP_ALIVE|F_VPNANY) ) == F_KEEP_ALIVE && req.s != -1)
    TryToAddKeepAlive(&req);
    DBGLA("Add KeepAlive fnc:%X fl:%X ", fnc, req.fl)

cnt:
  no_close_wait();

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
  //if( req.s != -1 &&  ! (req.fl & F_KEEP_ALIVE) ) CloseSocket( req.s );
  if( ! (req.fl & F_KEEP_ALIVE) ) req.Close();

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

int WINAPI KeepAliveWike(Req *preq)
{
  int serv=preq->flsrv[1] & MAX_SERV_MASK;
  int ntsk = preq->ntsk;
#ifdef FIX_EXCEPT
  if(SetFixExept(preq) )
  {
    preq->fl &= ~F_KEEP_ALIVE;
    DBGL("");
    goto cnt;
  }
#endif
  runed[serv]++;
  preq->flsrv[0]=0;
  preq->tmout=GetTickCount();
  rreq[preq->ntsk]=preq;
  count_of_tr++;
  DBGL("");
  preq->HttpReq();

cnt:
  DBGL("");
  no_close_wait();
  if(rreq[ntsk]==preq)
  {
    rreq[ntsk]=0;
    count_of_tr--;
    if(count_of_tr<0)count_of_tr=0;
  }

  if( (preq->fl & (F_KEEP_ALIVE|F_VPNANY)) == F_KEEP_ALIVE )
  {
    RemoveOldestIfNeed();
    if( KeepAliveCount < maxKeepAlive )
    {
      DBGL("");
      AddKeepAlive(preq);
      memset(& preq->timout, 0, sizeof(Req) - offset(Req, timout) );
      preq->fl &= MAX_SERV_MASK;
    }
    else DeleteKeepAlive(preq);
  }
  else
  {
    DBGL("");
    DeleteKeepAlive(preq);
  }

  runed[serv]--;
#ifdef SYSUNIX
  rreq[ntsk] = (Req *) 1;
#endif

  return 0;
}

#ifndef SYSUNIX

static const uchar KeepAliveServers[] = {SRV_HTTP, SRV_SSL};
int WINAPI KeepAliveThread(void *)
{
  fd_set set;
  fd_set er_set;
  int n,i,k,kk;
  Req *preq;
  timeval  tval;

  while(is_no_exit)
  {
    if(!KeepAliveCount) Sleep(1000);
    else
    {
      memcpy(&set, &KeepAliveSet, sizeof(set) );
      n = KeepAliveCount;
      for(i=0; i< sizeof(KeepAliveServers) ; i++ )
      {
        k=KeepAliveServers[i];
        if( runed[k] < max_srv[k] )
          goto checkEach;
      }
      Sleep(1000);
      continue ;
    checkEach:
      for(i=0; i< sizeof(KeepAliveServers) && n>0 ; i++ )
      {
        k=KeepAliveServers[i];
        if(  max_srv[k] && runed[k] >= max_srv[k] )
        {
          MyLock(KeepAliveMutex);
          for(kk=0; kk<KeepAliveCount && n>0; kk++)
          {
            if( (KeepAliveList[kk]->flsrv[1] & MAX_SERV_MASK) == k) FD_CLR(KeepAliveList[kk]->s,&set);
            n--;
          }
          MyUnlock(KeepAliveMutex);
        }
      }
      if(!n) Sleep(1000);
      else
      {
        tval.tv_sec=1;
        tval.tv_usec=500000;
        memcpy(&er_set,&set,sizeof(er_set));
        if( (kk=select(keep_alive_max_fd+1,&set,0,&er_set,&tval))>0 )
        {
          MyLock(KeepAliveMutex);
          for(k=0; k<KeepAliveCount; )
          {
            preq = KeepAliveList[k];
            // DBGLA("ka %d %d",k, preq->s);
            if(FD_ISSET(preq->s,&er_set))
            {
              // DBGLA("ka err %d %d",k, preq->s);
              RemoveAndDelKeepAlive(k);
            }
            else if(FD_ISSET(preq->s,&set))
            {
              // DBGLA("ka select %d %d",k, preq->s);
              RemoveKeepAlive(k);
              CrThreadFunc((TskSrv)KeepAliveWike, preq);
              if((--kk) <= 0) break;
            }
            else k++;
          }
          MyUnlock(KeepAliveMutex);
        }
      }
    }
  }
  return 0;
}

#endif

int Req::HttpReturnError(char *err,int errcode)
{char out_buf[2124];
 int l;
#ifdef SEPLOG
  l=flsrv[1] & MAX_SERV_MASK;
  if(l>=N_LOG)l=0;
  sepLog[l]->LAddToLog(err,s,&sa_c46 ,FmtShortErr);
#else
 AddToLog(err,s,&sa_c46 ,FmtShortErr);
#endif
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
  {
    x=DTick(tt,r->tmout);
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
  dec_no_close_req();
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
 //DBGLA("%lX %d %d",(long)th, th->s, l)
 th->SleepSpeed();
 if(th->fl & F_CHUNKED && th->Rcv != &TLSRecv )
 {
   //DBGL("chunked!")
   if( (r=BSend(th->s,chu,sprintf(chu,"%X\r\n",l) ))<=0 ) return r;
   th->Tout+=r;
 }
 if((r=BSend(th->s,(char *)b,l))>0)
 {
    th->Tout+=r;
    if( th->fl & F_CHUNKED &&  th->Rcv != &TLSRecv)
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

 if(th->fl & F_JUSTPOOL)
 {
   th->fl &= ~F_JUSTPOOL;
 }
 else if(RESelect(th->timout,0,1,th->s)<=0)
 {
   DBGLA("Timeout %lX s=%d timout=%d l=%d", (long)th, th->s, th->timout,l)
   return -1;
 }
 if((r=recv(th->s,(char *)b,l,0))>0 )th->Tin+=r;
 else if(th->Rcv == &TLSRecv)
 {
   ((OpenSSLConnection *)th->Adv)->state |= ST_CONNECTION_CLOSED;
 }
 //DBGLA("%lX s=%d timout=%d l=%d", (long)th, th->s, th->timout,r)
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
  if( ((u_long)(r=rreq[i]))>1 && (r->fl&F_SERV_MASK)==F_SERV_PROXY &&
     r->trn &&
#ifdef A_64
     ((ulong)(u_long)(r->req_var))== IPv4addr(&sa_c) && //sa_c.sin_addr. S_ADDR &&
     ((ulong)(u_long)(r->pst))==sa_c.sin_port
#else
     ((ulong)(r->req_var))== IPv4addr(&sa_c) && //sa_c.sin_addr. S_ADDR &&
     ((ulong)(r->pst))==sa_c.sin_port
#endif
  )
  {i=IsInIPRange(a);
   dec_no_close_req();
   return i<=0;
  }
 }
 dec_no_close_req();
 return 0;
};

#ifdef USE_IPV6
int IsIPv6(sockaddr_in *sa)
{
   if(sa->sin_family!=AF_INET6) return 0;
   if(
      ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[0]==0 &&
      ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[1]==0 &&
     (
       ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[2]==0xFFFF0000
       //|| ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr16[2]==0
     )
   ) return 0;
   return 1;
}

uint IPv4addr(sockaddr_in *sa)
{
   if(sa->sin_family==AF_INET) return sa->sin_addr. S_ADDR;
   if(sa->sin_family==AF_INET6 &&
      ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[0]==0 &&
      ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[1]==0 &&
     (
       ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[2]==0xFFFF0000
       //|| ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr16[2]==0
     )
   ) return        ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[3];
   return 1;
}
#endif

int FndLimit(int lst,LimitBase **ip, LimitBase **net, sockaddr_in *sa)
{ulong x;
#ifndef CD_VER
 *ip=0;
 *net=0;
 if(!(ltime[lst]))return 0;
 LimitCntr *lip,*lnet;
 x=time(0)-ltime[lst];
 if(!ipcnts[lst].n)ipcnts[lst].Push();
 if(ipcnts[lst].d[0].CheckLimit(limit[lst],ltime[lst]))
 {
   debug("Limit for %u all %u>%u",lst,ipcnts[lst].d[0].cnt,limit[lst]);

 lErrLim:


   Sleep(0x4000);
 //  Send("421 limit overflow\r\n",sizeof("421 limit overflow\r\n")-1);
   return 1;
 }
#ifdef USE_IPV6
 if(IsIPv6(sa)) //sa->sin_family==AF_INET6)
 {
   LimitCntrIPv6 *lip6,*lnet6;
    if(!(lip6= ipv6cnts[lst].Find(((sockaddr_in6 *) sa)->sin6_addr)))
    {
        ipv6cnts[lst].FreeOld(x); lip6=ipv6cnts[lst].Push(); lip6->Set( ((sockaddr_in6 *) sa)->sin6_addr );
        //lip6= AddToList(lst,sa);
    }
    struct IPv6c n6;
    n6.Set(((sockaddr_in6 *) sa)->sin6_addr );
    n6.ip.s6_addr32[3]&=0xFF;   //0xFF000000;
    if(!(lnet6=ipv6cnts[lst].Find(n6.ip)))
    { lnet6=ipv6cnts[lst].Push(); lnet6->Set(n6.ip);}
    *ip =lip6 ;
    *net=lnet6;
    if(lnet6->CheckLimit(net_limit[lst],ltime[lst])){
         debug("Limit for %u IPv6 net %u>%u %u",lst,lnet6->cnt,net_limit[lst],lnet6-ipv6cnts[lst].d);

        goto lErrLim;}
    if(lip6->CheckLimit(ip_limit[lst],ltime[lst])){

        debug("Limit for %u IPv6 addr %u>%u %u",lst,lnet6->cnt,ip_limit[lst],lip6-ipv6cnts[lst].d);

        goto lErrLim;
    }
    return 0;
 }
#endif
 int i=IPv4addr(sa); //sa->sin_addr. S_ADDR;
 if(!(lip= ipcnts[lst].Find(i)))
 {
     ipcnts[lst].FreeOld(x); lip=ipcnts[lst].Push(); lip->ip=i;
//     lip6= AddToList(lst,sa);
 }
 i&=0xFFFF;
 if(!(lnet=ipcnts[lst].Find(i)))
 { lnet=ipcnts[lst].Push(); lnet->ip=i;}
 *ip =lip ;
 *net=lnet;
 if(lnet->CheckLimit(net_limit[lst],ltime[lst]))
 {

     debug("Limit for %u IPv4 net %u>%u",lst,lnet->cnt,net_limit[lst]);
     goto lErrLim;
 }
 if(lip->CheckLimit(ip_limit[lst],ltime[lst]))
 {
       debug("Limit for %u IPv4 addr %u>%u",lst,lip->cnt,ip_limit[lst]);

     goto lErrLim;
 }
#endif
 return 0;
}



