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




#include "srv.h"
#include "fcgi.h"

//#include <stdio.h>

extern "C" {


#define FCGIDEBUG(a, b...)
  // debug( "%s:%d:" a , __func__, __LINE__, b );
  // printf( "%s:%d:" a , __func__, __LINE__, b );
#define FCGIDEBUG0()
 // debug( "%s:%d\n" , __func__, __LINE__ );
 // printf( "%s:%d\n" , __func__, __LINE__ );

int  Req::CallFCGI(char *name)
{
  FCGI_task *p;

  FCGIDEBUG0()

  for(p=fcgi_list; p ; p=p->next)
  {
    if(!strcmp(name,p->app_name))
      return DoFCGI(p);
  }
  FCGIDEBUG0()
  p = (FCGI_task *) malloc( sizeof(FCGI_task) + (max_tsk >> 3 ) ) ;
  if(!p) return -1;
  if(p->Open(name) < 0)
  {
    FCGIDEBUG0()
    free(p);
    return 0;
  }  ;
  p->next = fcgi_list;
  fcgi_list = p;
  FCGIDEBUG0()
  return DoFCGI(p);
}


#ifndef SYSUNIX
char fcgi_env[] = "S=FCGI\x00";

static int  unix_socket_number;
#else
int unix_socket_number;
#endif

int FCGI_task::Open(char *name)
{
  int i;
#ifdef SYSUNIX

  if( s_flgs[3] & FL3_FCGI_SI )
  {
    FCGIDEBUG("%X\n", s_flgs[3] )

    listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    sockaddr_len = sprintf(sau.sun_path, "%s/shs%u",fcgi_upath, ++unix_socket_number);
    sau.sun_family=AF_UNIX;
    unlink(sau.sun_path);
    sockaddr_len+=sizeof(sau.sun_family) + 1;
    if(bind(listen_fd,(sockaddr *)&sau, sockaddr_len)<0)
    {
      debug("FCGI bind error\r\n");
      goto err_exit;
    }

  }
  else
#endif
  {
    FCGIDEBUG("%X %X\n", s_flgs[3], INADDR_LOOPBACK );

    sa.sin_family=AF_INET;
    sa.sin_port=0;
    sa.sin_addr.s_addr = 0x0100007F; //INADDR_LOOPBACK;
    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_len = sizeof(sa);

    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(int));

    if( bind(listen_fd,(sockaddr *)&sa, sizeof(sa)) <0 )
    {
      debug("FCGI bind error %d %s\r\n", errno, strerror(errno));
      goto err_exit;
    }
    getsockname(listen_fd,(sockaddr *)&sa, &(i=sizeof(sa)) );

    debug("FCGI bind ok. port %u\r\n", htons(sa.sin_port));

  }

  if( listen(listen_fd, 1) < 0 )
  {
    debug("FCGI listen error %d %s\r\n", errno, strerror(errno));
    goto err_exit;
  }
  strncpy(app_name, name, sizeof(app_name));

#ifdef SYSUNIX
  if( !(child_pid=fork()) )
  {
    // Child
    //  debug("Run %s %X\n",loc,req);
    dup2(listen_fd, 0);
    //close(1);
    //close(2);
    execl(name, name, 0);
    printf("Execl %s error %d %s\n",name, errno ,strerror(errno));
    exit(0);
  }
  if(child_pid<0) {

    FCGIDEBUG("Fork error %d %s\n", errno, strerror(errno))

  err_exit:
    close(listen_fd);
    return -1;
  }
#else

  STARTUPINFO cb;
  char dir[512];
  char *p;

  memset(&cb,0,sizeof(cb));
  cb.cb=sizeof(cb);
  cb.dwFlags=STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
  cb.wShowWindow=SW_HIDE;
  cb.hStdInput=(HANDLE) listen_fd;
  cb.hStdOutput=GetStdHandle((ulong)STD_OUTPUT_HANDLE);
  cb.hStdError=GetStdHandle((ulong)STD_ERROR_HANDLE);
  strncpy(dir,name,512);
  p = strrchr(dir, FSLUSH);
  if(p) *p = 0;
  if(! (child_pid = MyCreateProcess(name, fcgi_env, dir, &cb,&pi) ) )
  {
  err_exit:
    closesocket(listen_fd);
    return -1;
  }
#endif

  FCGIDEBUG0()

#if 0
  i = 3;
  do{
    if(--i < 0 )
    {
      debug("Can't connect to FCGI");
      Close();
    }
    if( ! CheckLive() )
    {
      debug("Exec %s error\n", name);
      goto err_exit;
    }
    Sleep(100);
  } while(Reconnect() < 0) ;
#endif

#ifdef FCGI_THREAD
  if(! FCGI_thread_runed) {
    ulong id;
    CreateThread(&secat,0x8000,(TskSrv)FCGI_thread,(void *)0,0,&id);
  }
#endif
  return 0;
}

int FCGI_task::Connect(FCGI_req *f_req)
{
  FCGIDEBUG0()
 // if(f_req->s > 0)
 //   CloseSocket(f_req->s);
#ifdef SYSUNIX
  if( s_flgs[3]&FL3_FCGI_SI )
    f_req->s = socket(AF_UNIX, SOCK_STREAM, 0);
  else
#endif
  {
    f_req->s = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(f_req->s, SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(int));
  }
  #ifdef SYSUNIX
  fcntl(f_req->s, F_SETFD, fcntl(f_req->s, F_GETFD) | FD_CLOEXEC);
  #endif

  if( connect(f_req->s, (sockaddr *)&sa, sockaddr_len) < 0 )
  {
    closesocket(f_req->s);
    f_req->s = 0;
    return -1;
  }

  return 0;
}

int FCGI_task::CheckLive()
{

    FCGIDEBUG("%s\r\n", app_name)

#ifdef SYSUNIX
    int status;
    return ( waitpid(child_pid, (int *)&status ,WNOHANG) )? 0 : 1;
#else
    int ec;
    GetExitCodeProcess(pi.hProcess,(ulong *)&ec);
    return (ec==STILL_ACTIVE )? 1 : 0;
#endif
}

void FCGI_task::Close()
{
  FCGIDEBUG0()
  /*
  for(int idx=0; idx<max_tsk; idx++ )
    if(reqs[idx].s > 0)
    {
      CloseSocket(reqs[idx].s);
      reqs[idx].s = 0;
    }
  */
  if(listen_fd > 0)
  {
    closesocket(listen_fd);
    listen_fd = 0;
  }
  if(child_pid > 0)
  {
    #ifdef SYSUNIX
    int status, ec;

    FCGIDEBUG0()

    if( !(ec=waitpid(child_pid, (int *)&status ,WNOHANG))  )
    {

      FCGIDEBUG0()
      pid_to_wait = 0;
      kill(child_pid, SIGTERM); //15
      //sleep(1);
      status = SrvEventWait(&pid_to_wait,1000);

      FCGIDEBUG("SrvEventWait return %d\r\n",status)

      ec=waitpid(child_pid,(int *)&status,WNOHANG);

    }

    FCGIDEBUG0()

    while( ec>0 && ec!=child_pid)
    {

      FCGIDEBUG0()
      pid_to_wait = 0;
      kill(child_pid, SIGKILL); //9
      //usleep(10000);
      SrvEventWait(&pid_to_wait,10);
      ec=waitpid(child_pid,(int *)&status,WNOHANG);
    }

    #else
    int ec;
    GetExitCodeProcess(pi.hProcess,(ulong *)&ec);
    if(ec==STILL_ACTIVE )
    {
      TerminateProcess(pi.hProcess,0);
    }
    #endif
  }
  RemoveFromList((void **) &fcgi_list, this, STRUCT_OFFSET(FCGI_task, next) );

}

static const FCGI_BeginRequestRecord def_brr = {
  {FCGI_VERSION_1},
  {0, FCGI_RESPONDER, FCGI_KEEP_CONN}
};

static FCGI_Header def_param = {
  FCGI_VERSION_1, FCGI_PARAMS
};

int  FCGI_req::AppWrite(int type, int l,const void *data)
{
 int ret = 0;
 int r;
  FCGIDEBUG("type:%X %u %lX\r\n", type, l, data)
//  struct FCGI_BeginRequestRecord fh;
  struct FCGI_Header fh;
  memcpy(&fh,&def_brr,sizeof(fh) );
  fh.type = type;
  fh.Id = (short) req->ntsk;
  fh.contentLength = htons(l);
  //MyLock(lock);
  r=send( s,(char *)&fh,sizeof(fh),0);
  if(r == sizeof(fh))
  {
    if(l) ret=send( s,(char *)data,l,0);
  }
  else ret = -1;

  //MyUnlock(lock);

  return ret;
}
/*
int  FCGI_task::AppRead(Req *req,int max_l, void *data)
{
  struct FCGI_BeginRequestRecord fh;

}
*/

int FCGI_req::Param(const char *name, const char *val)
{
  int l, lv;
  int ret;
  union {
    char bfr[sizeof(FCGI_Header) + 512];
    FCGI_Header fh;
  };

  FCGIDEBUG("%s %lX\r\n",name,val)

  l=strlen(name);
  lv=strlen(val);
  memcpy(&fh,&def_param, sizeof(fh));
  fh.Id = (short)req->ntsk;
  fh.data[0]=l;
  if(lv<127)
  {
    fh.contentLength = htons(l + lv + 2);
    sprintf((char *)fh.data+2, "%s%s", name, val);
    fh.data[1]=lv;
    //MyLock(lock);
    ret = send( s, (char *) (void *) &fh, sizeof(fh) + l + lv + 2, 0);
  }
  else
  {
    fh.contentLength = htons(l + lv + 5);
    sprintf(fh.data + 5, "%s", name, val);
    DWORD_PTR(fh.data[1]) = htonl(lv);
    fh.data[1] |= 0x80;
    //MyLock(lock);
    ret = send( s,(char *) &fh, sizeof(fh) + l + 5, 0);
    if(ret > 0) {
      ret = send( s, val, lv, 0);
    }
  }
//  MyUnlock(lock);
  return ret;
}


#ifdef FCGI_THREAD
int Req::DoFCGI(FCGI_task *fcgi)
{
  char *t,**tt;
  int cl = ( ( t=GetVar(http_var,"CONTENT_LENGTH") )?atoui(t): 0);
  int pl;
  timeval  tval;


   FCGIDEBUG("cl=%d\r\n", cl)

   if(cl>post_limit)
   {
       debug("Size of method POST %u exceeds maximum %u",cl,post_limit);
       HttpReturnError("Size of method POST exceeds maximum");
       //goto ern;
       return 0;
   };

  fcgi->AppWrite(this, FCGI_BEGIN_REQUEST, sizeof(def_brr.body),(void *)&def_brr.body);
  fcgi->Param(this, "REQUEST_METHOD", ((fl&F_POST)?"POST":"GET"));
  fcgi->Param(this, "REMOTE_HOST", (char *) (http_var+MAX_HTTP_VARS+2));
  fcgi->Param(this, "SCRIPT_FILENAME", loc);
  fcgi->Param(this, "PATH_TRANSLATED", loc);
  fcgi->Param(this, "DOCUMENT_ROOT", dir);
  if(vhdir && vhdir->h[0]=='/')
  {
    char bb[512];
    msprintfchk(bb,bb+511,"SCRIPT_NAME=%s%s\n",vhdir->h,loc+dirlen+1);
    fcgi->Param(this, "SCRIPT_NAME", bb);
  }
  else
    fcgi->Param(this, "SCRIPT_NAME", loc+dirlen);
  if( trn ) fcgi->Param(this, "PATH_INFO", trn);
  for(tt= http_var;(*tt) && (tt[1]); tt+=2)
  {
    char bb[512];
    sprintf(bb, "HTTP_%.255s", *tt);
    fcgi->Param(this, *tt , tt[1]);
    fcgi->Param(this, bb , tt[1]);
  }
  fcgi->AppWrite(this, FCGI_PARAMS, 0, "");

  if( (fl&F_POST) && pst && cl )
  {
    pl=postsize; t=pst;
    do {
      if(pl && fcgi->AppWrite(this, FCGI_STDIN, pl, t)<=0) break;
      if( (cl-=pl)<=0 )break;
      //if((ec=waitpid(child_pid,(int *)&status,WNOHANG))) break;
      if((pl=Recv(t,MIN(1024,cl)))<=0) {
        fcgi->AppWrite(this, FCGI_ABORT_REQUEST, 0, "");
        return 0;
      }
    } while( 1 );
  }
  fcgi->AppWrite(this, FCGI_STDIN, 0, "");
  //fcgi->state |= fcgistWAIT_RECV;

  event = 0;
  fl |= F_FCGI;
  fcgi->n_wait ++;
  fcgi->reqs[ntsk>>ARH_SHIFT] |= (ARH_ONE << (ntsk & ARH_MASK) ) ;
  do{
    if(SrvEventWait(&event, 2000)) break;
    if(RESelect1(0,1000,s)) {
      fcgi->AppWrite(this, FCGI_ABORT_REQUEST, 0, "");
      break;
    }
  } while(1);
  fcgi->n_wait --;
  fcgi->reqs[ntsk>>ARH_SHIFT] &= ~ (ARH_ONE << (ntsk & ARH_MASK) ) ;
  fl &= ~F_FCGI;
  KeepAlive = 0;

  FCGIDEBUG("End request %s\r\n", fcgi->app_name)

  return 1;
}
#endif


int Req::DoFCGI(FCGI_task *fcgi)
{
  FCGI_req f_req;
  char *t,**tt;
  int cl = ( ( t=GetVar(http_var,"CONTENT_LENGTH") )?atoui(t): 0);
  int pl;
  timeval  tval;
  char  *bfr;


   FCGIDEBUG("cl=%d\r\n", cl)

   if(cl>post_limit)
   {
       debug("Size of method POST %u exceeds maximum %u",cl,post_limit);
       HttpReturnError("Size of method POST exceeds maximum");
       //goto ern;
       return 0;
   };

  bfr = (char *) malloc(0x10010);
  if(!bfr){
    debug("FCGI: No memory\r\n");
    HttpReturnError("Temporary unavilable") ;
    return 0;
  }

  memset(&f_req, 0, sizeof(f_req));
  f_req.req = this;
  fcgi->Connect(&f_req);

  f_req.AppWrite( FCGI_BEGIN_REQUEST, sizeof(def_brr.body),(void *)&def_brr.body);
  f_req.Param( "REQUEST_METHOD", ((fl&F_POST)?"POST":"GET"));
  f_req.Param( "REMOTE_HOST", (char *) (http_var+MAX_HTTP_VARS+2));
  f_req.Param( "SCRIPT_FILENAME", loc);
  f_req.Param( "PATH_TRANSLATED", loc);
  f_req.Param( "DOCUMENT_ROOT", dir);
  if(vhdir && vhdir->h[0]=='/')
  {
    char bb[512];
    msprintfchk(bb,bb+511,"SCRIPT_NAME=%s%s\n",vhdir->h,loc+dirlen+1);
    f_req.Param( "SCRIPT_NAME", bb);
  }
  else
    f_req.Param( "SCRIPT_NAME", loc+dirlen);
  if( trn ) f_req.Param( "PATH_INFO", trn);
  for(tt= http_var;(*tt) && (tt[1]); tt+=2)
  {
    char bb[512];
    sprintf(bb, "HTTP_%.255s", *tt);
    f_req.Param( *tt , tt[1]);
    f_req.Param( bb , tt[1]);
  }
  f_req.AppWrite( FCGI_PARAMS, 0, "");

  if( (fl&F_POST) && pst && cl )
  {
    pl=postsize; t=pst;
    do {
      if(pl && f_req.AppWrite( FCGI_STDIN, pl, t)<=0) break;
      if( (cl-=pl)<=0 )break;
      //if((ec=waitpid(child_pid,(int *)&status,WNOHANG))) break;
      if((pl=Recv(t,MIN(1024,cl)))<=0) {
        f_req.AppWrite( FCGI_ABORT_REQUEST, 0, "");
        return 0;
      }
    } while( 1 );
  }
  f_req.AppWrite( FCGI_STDIN, 0, "");

  event = 0;
  fl |= F_FCGI;
  fcgi->n_wait ++;
  //fcgi->reqs[ntsk>>ARH_SHIFT] |= (ARH_ONE << (ntsk & ARH_MASK) ) ;
  while( f_req.FCGIDoInput(bfr) ) {
    //if(SrvEventWait(&event, 2000)) break;
    if(RESelect1(0,1000,s)) {
      f_req.AppWrite(FCGI_ABORT_REQUEST, 0, "");
      break;
    }
  }
  fcgi->n_wait --;
  //fcgi->reqs[ntsk>>ARH_SHIFT] &= ~ (ARH_ONE << (ntsk & ARH_MASK) ) ;
  fl &= ~F_FCGI;
  KeepAlive = 0;
  CloseSocket(f_req.s);

  free(bfr);
  FCGIDEBUG("End request %s\r\n", fcgi->app_name)

  return 1;
}


// void FCGI_task::Run(Req*  req)
// {
//
// }

int lock_FCGItask;
void CloseFCGI_tasks()
{
  FCGI_task *p,*t;

  FCGIDEBUG0()

  MyLock(lock_FCGItask);
  for(p=fcgi_list; p ; )
  {
    p->Close();
    t = p;
    p=p->next;
    free( t );
  }
  MyUnlock(lock_FCGItask);
}


int FCGI_req::FCGIDoInput(char *bfr)
{
  //uint i;
  int j,l;
  char *data;
  int type;
  timeval  tval;
  //FCGI_req *f_req;
  union {
    char *pbfr;
    FCGI_BeginRequestRecord *prec;
    FCGI_Header *pheader;
  };


  while( (j=RESelect1(0,10000,s)) > 0 )
  {
    //FCGIDEBUG("p= %lX\r\n",p)
    if(state & fcgistPART_HEAD)
    {
      state &= ~fcgistPART_HEAD;
      l = recv(s, bfr + recv_len_left , 0x10000 - recv_len_left, 0);
      if(l>0)l+=recv_len_left;
    }
    else
    {
      if(state & fcgistSKIP_PADD)
      {
        l = recv(s, bfr, padd , 0);
        if(l <= 0 ) return 0;

          //goto lb_closed;
        if( (padd -= l)>0 ) continue; //goto lb_continue;
        state &= ~fcgistSKIP_PADD;
      }
      l = recv(s, bfr, 0x10000, 0);
    }

    FCGIDEBUG("recv %d %d\r\n", s, l)

    if(l<=0)
    {
     // lb_closed:
       return 0;
      //p->EndReq(idx);
      /*
       *    // Close all request
       *    p->EndAllReq();
       *
       *    if(p->Reconnect() < 0) {
       *
       *      FCGIDEBUG("p= %lX\r\n",p)
       *      t = p;
       *      p = p->next;
       *      t->Close();
       *      free(t);
    }
    else
      p = p->next;
    */
    }
    pbfr = bfr;
    while( l > sizeof(FCGI_Header) )
    {
      if(recv_len_left && (state & fcgistPART_BODY) )
      {
        j = recv_len_left;
        state &= ~ fcgistPART_BODY;
        data = pbfr;
        type = FCGI_STDOUT;
      }
      else
      {
        if( (pheader->Id) > max_tsk || pheader->type >= FCGI_MAXTYPE )
        {
          debug("FCGI Bad Id or sinhronisation error. %d %d", pheader->Id, pheader->type);
          return 0;
          //p->EndReq(idx);
          /*
           *                    if(p->Reconnect()<0)
           *                    {
           *                      t=p;
           *                      p->EndAllReq();
           *                      p=p->next;
           *                      t->Close();
           *                      free(t);
        }
        else
          p = p->next;
        */
          //continue;
        }
        j = htons(pheader->contentLength);
        padd = pheader->paddingLength;
        l -= sizeof(FCGI_Header);
        data = pbfr + sizeof(FCGI_Header);
        type = pheader->type;
      }
      switch(type)
      {
        case  FCGI_STDERR:
        case  FCGI_STDOUT:

          FCGIDEBUG("FCGI_STDOUT %X %d %d |%.32s|\r\n", type,l,j,data)
          if(j > l)
          {

            recv_len_left = j - l;
            state |= fcgistPART_BODY;
            //last_id = i;
            j = l;
          }
          if(j>0) {
            if( (!req->Tout) && !(req->fl&F_SKIPHEAD) )
            {
              if(req->SendChk( HTTP_HEAD_BEGIN , sizeof(HTTP_HEAD_BEGIN) - 1 ) <=0 ) goto lb_disconnect;
            }
            if( req->SendChk(data, j) <= 0 )
            {
             lb_disconnect:
              AppWrite(FCGI_ABORT_REQUEST, 0, "");
              //SendEvent(req->event, 1);
              return 0;
             // p->EndReq(idx);
            }
          }
          break;
        case FCGI_END_REQUEST:

          FCGIDEBUG("FCGI_END_REQUEST %d\r\n",l)

          //SendEvent(req->event, 1);
          //p->EndReq(idx);
          return 0;
          //break;
        default:
          debug("Error. Unknow FCGI type: %X left=%d j=%d\r\n", type ,l,j);
      }
      l -= j;
      pbfr = data+j;
      if(padd && l>0) {
        if(l >= padd)
        {
          l -= padd;
          pbfr += padd;
        }
        else
        {
          padd-=l;
          l=0;
          state |= fcgistSKIP_PADD;
        }
      }
    };
    if(l>0)
    {
      recv_len_left = l;
      state |= fcgistPART_HEAD;
    }
  }
  return !j;
}

#ifdef FCGI_THREAD

void FCGI_task::EndReq(int idx)
{
  FCGI_req *f_req = reqs + idx;
  SendEvent(f_req->req->event, 1);
  CloseSocket(f_req->s);
  MyLock(lock);
  f_req->s = 0;
  f_req->req = 0;
  n_wait --;
  MyUnlock(lock);
};

void FCGI_task::EndAllReq()
{
 int i;
  for(i=0; i<max_tsk && n_wait > 0 ; i++ )
  {
    if(reqs[i].s > 0)EndReq(i);
    /*
    if(rreq[i] &&  (reqs[i>>ARH_SHIFT] & (ARH_ONE << (i&ARH_MASK) )  ) ) {
      SendEvent(rreq[i]->event, 2);
    }
    */
  }
}

int FCGI_thread(void *)
{
#ifdef MINGW
#undef fd_set
#define win_fd_set fd_set
#endif

  FCGI_task *p,*t;
  fd_set set;
  int s, j, l, w;
  uint i;
  char *bfr;
  char *data;
  int type;
  timeval  tval;
  FCGI_req *f_req;
  Req *c_req;

  union {
    char *pbfr;
    FCGI_BeginRequestRecord *prec;
    FCGI_Header *pheader;
  };

  FCGIDEBUG0()

  FCGI_thread_runed = 1;

  bfr =(char *) malloc(0x10008);
  while(is_no_exit)
  {
    s = 0;
    FD_ZERO(&set);
    tval.tv_sec=0;
    tval.tv_usec=500000;
    MyLock(lock_FCGItask);
    for(p=fcgi_list; p ; p=p->next) if(p->n_wait) // (p->state & (fcgistWAIT_RECV | fcgistRECV_READY)) == fcgistWAIT_RECV)
    {

      w = 0;
      for(int idx=0; idx<max_tsk; idx++ )
      {
        j=p->reqs[idx].s;
        if(j>0) {
          if( j > s ) s = j;
          FD_SET(j, &set);
          FCGIDEBUG("set %u\r\n",j)
          if( (++w) >= p->n_wait  ) break;
        }
      }
    }
    MyUnlock(lock_FCGItask);
    if(!s)
    {
      Sleep(300);
    }
    else if( (j=select(s+1, &set,0,0,&tval))>0 )
    {
      if(!is_no_exit) break;
      FCGIDEBUG("select %d\r\n",j)
      for(p=fcgi_list; p ; p=p->next)
      {

        FCGIDEBUG("p= %lX\r\n",p)
        w = 0;
        for(int idx=0; w < p->n_wait && idx<max_tsk; idx++ )
        {
          f_req = p->reqs + idx;
          if(f_req->s > 0)
          {
            w++;
            if( FD_ISSET(f_req->s, &set) )
            {
              c_req = f_req->req;
              FCGIDEBUG("p= %lX\r\n",p)
              if(f_req->state & fcgistPART_HEAD)
              {
                memcpy(bfr, f_req->bfr, f_req->recv_len_left);
                f_req->state &= ~fcgistPART_HEAD;
                l = recv(f_req->s, bfr + f_req->recv_len_left , 0x10000 - f_req->recv_len_left, 0);
                if(l>0)l+=f_req->recv_len_left;
              }
              else
              {
                if(f_req->state & fcgistSKIP_PADD)
                {
                  l = recv(f_req->s, bfr, f_req->padd , 0);
                  if(l <= 0 ) goto lb_closed;
                  if( (f_req->padd -= l) ) continue; //goto lb_continue;
                  f_req->state &= ~fcgistSKIP_PADD;
                }
                l = recv(f_req->s, bfr, 0x10000, 0);
              }

              FCGIDEBUG("recv %d %d\r\n", f_req->s, l)

              if(l<=0)
              {
                lb_closed:
                p->EndReq(idx);
                /*
                 *    // Close all request
                 *    p->EndAllReq();
                 *
                 *    if(p->Reconnect() < 0) {
                 *
                 *      FCGIDEBUG("p= %lX\r\n",p)
                 *      t = p;
                 *      p = p->next;
                 *      t->Close();
                 *      free(t);
              }
              else
                p = p->next;
              */
                continue ;
              }
              pbfr = bfr;
              while( l > sizeof(FCGI_Header) )
              {
                if(f_req->recv_len_left && (f_req->state & fcgistPART_BODY) )
                {
                  j = f_req->recv_len_left;
                  f_req->state &= ~ fcgistPART_BODY;
                  data = pbfr;
                  type = FCGI_STDOUT;
                }
                else
                {
                  if( (i=pheader->Id) > max_tsk || pheader->type >= FCGI_MAXTYPE )
                  {
                    debug("FCGI Bad Id or sinhronisation error. %d %d",i, pheader->type);
                    p->EndReq(idx);
                    /*
                    if(p->Reconnect()<0)
                    {
                      t=p;
                      p->EndAllReq();
                      p=p->next;
                      t->Close();
                      free(t);
                    }
                    else
                      p = p->next;
                    */
                    continue;
                  }
                  j = htons(pheader->contentLength);
                  f_req->padd = pheader->paddingLength;
                  l -= sizeof(FCGI_Header);
                  data = pbfr + sizeof(FCGI_Header);
                  type = pheader->type;
                }
                switch(type)
                {
                  case  FCGI_STDERR:
                  case  FCGI_STDOUT:

                    FCGIDEBUG("FCGI_STDOUT %X %d %d |%.32s|\r\n", type,l,j,data)
                    if(j > l)
                    {

                      f_req->recv_len_left = j - l;
                      f_req->state |= fcgistPART_BODY;
                      //f_req->last_id = i;
                      j = l;
                    }
                    if(j>0) {
                      if( (!c_req->Tout) && !(c_req->fl&F_SKIPHEAD) )
                      {
                        if(c_req->SendChk( HTTP_HEAD_BEGIN , sizeof(HTTP_HEAD_BEGIN) - 1 ) <=0 ) goto lb_disconnect;
                      }
                      if( c_req->SendChk(data, j) <= 0 )
                      {
                      lb_disconnect:
                        p->AppWrite(c_req, FCGI_ABORT_REQUEST, 0, "");
                        //SendEvent(c_req->event, 1);
                        p->EndReq(idx);
                      }
                    }
                    break;
                  case FCGI_END_REQUEST:

                    FCGIDEBUG("FCGI_END_REQUEST %d\r\n",l)

                    //SendEvent(c_req->event, 1);
                     p->EndReq(idx);
                    break;
                  default:
                    debug("Error. Unknow FCGI type:%s %X left=%d j=%d\r\n", p->app_name, type ,l,j);
                }
                l -= j;
                pbfr = data+j;
                if(p->padd && l>0) {
                  if(l >= f_req->padd)
                  {
                    l -= f_req->padd;
                    pbfr += f_req->padd;
                  }
                  else
                  {
                    f_req->padd-=l;
                    l=0;
                    f_req->state |= fcgistSKIP_PADD;
                  }
                }
              };
              if(l>0)
              {
                f_req->recv_len_left = l;
                memcpy(f_req->bfr, pbfr, l);
                f_req->state |= fcgistPART_HEAD;
              }

            }
          }

        }

      }
    }
  }
  // Exit
  CloseFCGI_tasks();

  FCGI_thread_runed = 0;

  return 0;
}

#endif

int SrvEventWait(int *event, uint timeout)
{
#ifdef USE_FUTEX
  struct timespec tout;
  if(*event) return *event;
  tout.tv_sec = timeout / 1000;
  tout.tv_nsec = (timeout % 1000) * 1000000;
  futex((int *)event,FUTEX_WAIT,0,&tout,0,0);
  return *event;
#else
  int w;
  do {
    if(*event) return *event;
    Sleep(w = timeout < 20 ? timeout : 20);
    if(*event) return *event;
  } while(timeout-= w);
  return 0;
#endif
};

void* RemoveFromList(void **plist, void *object, int next_offset)
{
  union
  {
    void *pnext;
    void **ppnext;
    char *pnextc;
  };
  void *list = *plist;
  if(list && list == object)
  {
    pnextc = ((char *)list) + next_offset;
    *(void **)plist = * (void **) ( ((char*)object) + next_offset );
    return plist;
  }
  while(list)
  {
    pnextc = ((char *)list) + next_offset;
    if(*ppnext == object)
    {
      *ppnext = * (void **) ( ((char*)object) + next_offset );
      return pnext;
    }
    list = *ppnext;
  }
  return 0;
};

}

