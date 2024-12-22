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


#ifndef SRV_H
#include "srv.h"
#endif
#undef send
#undef recv
#define send(a,b,c,d)  SendChk(b,c)
#define recv(a,b,c,d)  Recv(b,c)

#define DBG()
//DBGL("")
// debug("D %d", __LINE__ )
#if defined(ANDROID) || defined(AT_ARM)
#define __environ environ
#endif

#undef DBG
#define DBG()
 //sprintf(einf, " %d", __LINE__ );
#define PDBG(a,b...)
//printf(" %d " a "\n", __LINE__ ,b);
 //sprintf(einf, " %d " a , __LINE__ ,b);
//#define DBGL(a...) debug(a)

const char ChunkedHead[] = "HTTP/1.1 200 \r\n"
                           "Connection: Keep-Alive\r\n"
                           "Transfer-Encoding: chunked\r\n";

const int ChunkedHeadSize = sizeof(ChunkedHead) - 1;

char *memfind(const char *s, const char *patern, int l, int l2)
{
  char *s1;
  while( l>=l2 && (s1 = memchr(s,patern[0], l - l2 + 1) ) )
  {
    if(! memcmp(s1, patern, l2) ) return s1;
    l -= (s1 - s) + 1;
    s = s1 + 1;
  }
  return 0;
}

int Req::SendChk(const char *b,int l)
{
 if( (fl&F_SKIPHEAD) && !Tout)
 {const char *t;

 // debug("Skip head! %X %u",fl,l);

  if(fl&F_LASTLF)
  {fl&=~F_LASTLF;
   if(*b=='\n' ){++b; --l; goto lbGo; }
   if(*b=='\r' ){b+=2; l-=2; goto lbGo; }
  }
  if( (t=strstr(b,"\n\r\n")) || (t=strstr(b,"\n\n"))-- )
  {t+=3;
   l-=t-b;
   b=t;
 lbGo:
   if(l<=0){return ++Tout;}
  }
  else
  {if(b[l-1]=='\n')fl|=F_LASTLF;
   return 1;
  }
 }
 DBGLA("SendChk %d %d fl=%X",l,Tout,fl);

 if(fl&F_STDERRSEL)
 {
   if(! (fl&F_NAMEOUTED) )
   {
     #ifdef SEPLOG
     //               sepLog[CGI_ERR_LOG]->LAddToLog(loc,s,FmtShortErr);
     sepLog[CGI_ERR_LOG]->LAddToLog(0,s,&sa_c46,">>%.256s ; %.512s\r\n",loc,req?req:"");
     #else
     AddToLog(loc,s,&sa_c46,FmtShortErr);
     #endif
     fl|=F_NAMEOUTED;
   }

   if(!(fl&F_ERROUTED))
   {
     //               b[l]=0;
     #ifdef SEPLOG
     //             sepLog[CGI_ERR_LOG]->Ldebug("%X:%.128s%s\r\n",fl,b, (l>128)?"...":"" );
     sepLog[CGI_ERR_LOG]->Ldebug("%.128s%s\r\n",b, (l>128)?"...":"" );
     #else
     debug("%.128s%s\r\n",b, (l>128)?"...":"" );
     #endif
     if(l>64 ) // || Tout>128 )
       fl|=F_ERROUTED ;
   }
   if( (s_flgs[2]&FL2_NOERROUT) ) return 1;
 }

 if(fl&F_UPCHUNKED)
 {
   int dl;
   char *pp;
   int ret;
   if(fl & F_LASTLF)
   {
     dl = 2;
     if(*b == '\r' || *b == '\n')
     {
       if(*b == '\n') dl=1;
       if((ret = Send(b,dl)) <=0 ) return ret;
       fl = (fl& ~(F_UPCHUNKED | F_LASTLF)) | F_CHUNKED;
       if( (l -= dl) <= 0 ) return ret;
       b += dl;
       goto lbSnd;
     }
   }

   dl = 4;
   pp = memfind(b,"\r\n\r\n",l,4);
   if(!pp)
   {
     pp = memfind(b,"\n\n",l,2);
     dl = 2;
   }
   if(pp)
   {
     dl += (pp - b);
     if( (ret = Send(b, dl) ) <=0 ) return ret;
     fl = (fl& ~(F_UPCHUNKED | F_LASTLF)) | F_CHUNKED;
     if( (l-=dl) <= 0 ) return ret;
     b += dl;
   }
   else if(b[l-1] == '\n') fl |= F_LASTLF;
 }
lbSnd:
 return Send(b,l);
};

int Req::ExecCGI()
{char *http_v[MAX_HTTP_VARS+16];
 memcpy(http_v,http_var,sizeof(http_v));
 http_var=http_v;
// prepare_HTTP_var();
DBG();
 return  ExecCGIEx();
};
//----

ulong D64X(uchar i)
{if(i=='+')return 62;
 if(i=='/')return 63;
 if(i<'0')return 64;
 if(i<='9')return i-'0' + 52;
 if(i<='Z')return i-'A';
 return i-'a' + 26;
};

char * Decode64(char *t, char *s, int max_size)
{
  char *y = t;
  uint i,j;

  while((i=D64X(*s))<64)
  {
    s++;
    if( (j=D64X(*s))>=64 )break;
    *y=(i<<2)|(j>>4);
    s++; y++;
    if( (i=D64X(*s))>=64 )break;
    *y=(j<<4)|(i>>2);
    s++; y++;
    if( (j=D64X(*s))>=64 )break;
    *y=(i<<6)|(j);
    s++; y++;
    if((y-t) >= max_size) return 0;
  }
  *y=0;
  return y;
}

char * Req::CheckAuth(char *&p)
{
  char *t,*z,*y;
  if( (t= GetVar(http_var,"AUTHORIZATION")) )
  {
    z=y=p;
    //  debug("|%s|",t);
    if( strin(t,"Basic") )
    {
      if(! (Decode64(y, t+6, 256) ) ) return 0;
      if((y=strchr(z,':'))){*y=0;y++;}else y="";
      p=y;
      return z;
    }
    else   if( strin(t,"Digest") )
    {
      p=t;
      if((z=strstr(t, "username=\"")))
      {
        z+=sizeof("username=");
        return z;
      }
    }
  }
  return 0;
}

#ifndef SYSUNIX
int MyCreateProcess(char *p,void *env,char *loc, STARTUPINFO *cb,PROCESS_INFORMATION *pi)
{
 union{
   ushort ut[512];
   uchar uc[2048];
 };
  ushort ut2[512];
  if(utf2unicode((uchar *)p,ut)>0)
  {
     utf2unicode((uchar *)loc,ut2);
     return   CreateProcessW(0,(WCHAR *) ut,&secat,&secat,1,NORMAL_PRIORITY_CLASS,env,(WCHAR *)ut2,(STARTUPINFOW *)cb,pi);
  }
  else return
//  (hToken)?
//  CreateProcessAsUser(hToken,0,p,0,0,1,NORMAL_PRIORITY_CLASS,env,loc,cb,pi):
  CreateProcess(0,p,&secat,&secat,1,NORMAL_PRIORITY_CLASS,env,loc,cb,pi);
}

int Req::MakeEnv(char *env, char *ee)
{char *t,**tt,*penv,*z,*y;
 struct sockaddr_in sa;
 int l;//=sizeof(sockaddr_in);
// getpeername(s,(sockaddr *)&sa,&l);
   //z=inet_ntoa(sa.sin_addr);
#define msprintf(a,b...) msprintfchk(a,ee,b)
   //debug("MakeEnv %X %X %X",(http_var+MAX_HTTP_VARS+2),loc, dir);

   t=penv=env+
      msprintf(env,
     "GATEWAY_INTERFACE=CGI/1.1\nREQUEST_METHOD=%s\n"
     "SERVER_PROTOCOL=HTTP/1.0\n"
//   SERVER_PORT=%u\n"
//    "REMOTE_ADDR=%s\n
     "REMOTE_HOST=%.64s\n" // %u.%u.%u.%u\n"
     "SERVER_SOFTWARE=Small_HTTP_Server\n"
     "SCRIPT_FILENAME=%.128s\n"
     "PATH_TRANSLATED=%.128s\n"
     "DOCUMENT_ROOT=%.128s\n"
     ,((fl&F_POST)?"POST":"GET"),
//    http_port,z, z,
   (http_var+MAX_HTTP_VARS+2)
/*
#ifndef SYSUNIX
   sa_c.sin_addr.S_un.S_un_b.s_b1,  sa_c.sin_addr.S_un.S_un_b.s_b2,
   sa_c.sin_addr.S_un.S_un_b.s_b3,  sa_c.sin_addr.S_un.S_un_b.s_b4
#else
   sa_c.sin_addr.s_addr&0xFF, BYTE_PTR(sa_c.sin_addr.s_addr,1),
   BYTE_PTR(sa_c.sin_addr.s_addr,2),BYTE_PTR(sa_c.sin_addr.s_addr,3)
#endif
*/
     ,loc,loc, dir  //(vhdir)?vhdir->d :
   );
   penv+=(vhdir && vhdir->h[0]=='/')?
        msprintf(penv,"SCRIPT_NAME=%s%s\n",vhdir->h,loc+dirlen+1):
          msprintf(penv,"SCRIPT_NAME=%s\n",loc+dirlen);
   if( trn )penv+=msprintf(penv,"PATH_INFO=/%s\n",trn);
   for(;*t;t++)if(*t=='\\')*t='/';
   //t=env+0x2400+Tin*2;
   t=ee-12;
   for(tt=http_var;(*tt) && (tt[1]);tt+=2)
   {
     /*
     if(strstr(*tt,"QUERY_STRING"))
     {
         if( (penv+=sprintf(penv,"%.64s=%.8192s\n",*tt,tt[1],*tt,tt[1]))>t) break;
         //if( (penv+=sprintf(penv,"HTTP_%.64s=%.8192s\n",*tt,tt[1],*tt,tt[1]))>t) break;
     }
     else
     */
     /*
       if( (penv+=sprintf(penv,"%.64s=%.8000s\n",*tt,tt[1]))>=ee) break;
       if( (penv+=sprintf(penv,"HTTP_%.64s=%.8000s\n",*tt,tt[1],*tt,tt[1]))>=ee) break;
     */
     if( (penv+=l=msprintfchk(penv,ee,"%.64s=%.8000s\nHTTP_%.64s=%.8000s\n",*tt,tt[1],*tt,tt[1])) >t || !l )break;
   }

   l=( ( t=GetVar(http_var,"CONTENT_LENGTH") )?atoui(t):0) ;

//   if( (z=CheckAuth(t=penv+200) ) )
//     penv+=sprintf(penv,"AUTH_TYPE=Basic\nREMOTE_USER=%.64s\nAUTH_PASS=%.64s\n",z,t);
   for(t=env;*t;t++)if(*t=='\n')*t=0;
   memcpy(t,eenv,leenv);
   return l;
#undef msprintf
};

//-----
#define DUP(a,b)  a=b
HANDLE DUP2(HANDLE b)
{
 HANDLE a;
 if(DuplicateHandle(hProcess,b,hProcess,&a,0,0,DUPLICATE_SAME_ACCESS))
 {
  CloseHandle(b);
//  debug("Ok dup %d",GetLastError());
  return a;
 }
// debug("Error dup %d",GetLastError());
 return b;
}

int Req::ExecCGIEx()
{HANDLE hrdp=0,hwrp,hrde,hrd,
hwr,hwre
// ahwr[3]
 ;
//#define hwr  ahwr[0]
//#define hwre ahwr[1]
//#define hs   ahwr[2]
 PROCESS_INFORMATION pi;
 STARTUPINFO cb;
 char *p=0,*pp,*t,**tt,*e,*t1,*t2;
 long ll,ec,pl=0,cl=0,l,i;
 char *env,*penv,z[300];
 ulong timeout=cgi_timeout+time(0);
 int http_status = 200;


 z[0]=0;
 e=z;
// hs=(void *)s;
 if((stristr(loc,".pl") ) )
 {//fl|=F_PERL;
  if(perl)
  { e=perl; goto llgs1; }
  goto llgs2;
 }
 else if( fl&F_PHP )
 { if(phtml_dir){e=phtml_dir; goto llgs1;}
   goto llgs2;
 }
 else if(strstrnc(loc,".EXE")){fl|=F_EXE; e=loc;}
 else if(strstrnc(loc,".DLL")) return  ExecDllEx(loc);
 else{
  for(tt=ext;*tt;tt+=2) if((*tt[0]=='.') && strstrnc(loc,*tt) ){e=tt[1]; goto llgs1;}
 llgs2:;
  if(!(s_flg&FL_NOWINTYPES)) if( (ll=(ulong)FindExecutable(loc,dir,z))>32 )
   {llgs1:
     if(strstrnc(e,".DLL")) return  ExecDllEx(e);
   }else goto er000;
 };
 if(pst && loc) i = pst-loc;
 else i=Tin;
 if(((uint)i)>8192)i=8192;
 if((p=new char[0x14900+leenv+i*2]))
 {pp=p;
  // *p++='\n';
  env=p+0x10800;
  cl=MakeEnv(env,env+0x4000+leenv+i*2);
  memset(&cb,0,sizeof(cb));
  cb.cb=sizeof(cb);
  cb.dwFlags=STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
  cb.wShowWindow=SW_HIDE;
  if( (fl&F_POST) && pst && cl)
  {if(cl>post_limit /*0x30000*/)
   { /* dbg("Very big post...");*/

       debug("Size of method POST %u exceeds maximum %u",cl,post_limit);
       HttpReturnError("Size of method POST exceeds maximum");

//       goto ern;
       goto ex2;

   };
   if(!CreatePipe(&hrdp,&hwrp,&secat,cl+256))
   {HttpReturnError("Error. Can\'t run script");
    goto ex;
   };
   pl=postsize;

//#define QW1
#ifdef QW1
   t=pst;
   do{ if(pl>cl)pl=cl; if(pl)WriteFile(hwrp,t,pl,(ulong *)&pl,0);
       if( (cl-=pl)<=0 )break; t=p;
       if( (pl=Recv(p,MIN(0x10000,cl)))<0 )goto tp;
     }while( pl );
 //  CloseHandle(hwrp);
#else
   t2=pst;
#endif
   cb.hStdInput=hrdp;
   //DUP(cb.hStdInput,hrdp);
   hwrp=DUP2(hwrp);

  };
 if((!CreatePipe(&hrd,&hwr,&secat,0x10000))||
     !CreatePipe(&hrde,&hwre,&secat,0x400))
 {
 er000:;
   if(p)delete p;
   return HttpReturnError("Error. Can\'t run script");
 };
 cb.hStdOutput=hwr;
 cb.hStdError=hwre;
 // DUP(cb.hStdOutput,hwr);
 // DUP(cb.hStdError,hwre);

 hrd=DUP2(hrd);
 hrde=DUP2(hrde);

  t=strrchr(loc,'\\');
  if(!t) t = loc;
  t1="";
  if(loc[1]==':' && strchr(loc, ' ') )t1="\"";
  if( ! (fl&F_EXE) )
  {
     if( (fl&F_PHP) )
        sprintf(p,"%s %s%s%s",e,t1,(loc[1]==':')?loc:t+1,t1);
     else
        sprintf(p,"%s %s%s%s %s",e,t1,(loc[1]==':')?loc:t+1,t1,req);
  }
  else sprintf(p,"%s %s",loc,req);
//  *t=0;
  ec=t[1];
  t[1]=0;
  ll=(int) MyCreateProcess(p,env,(doc_dir)?doc_dir:loc, &cb,&pi);
   //CreateProcess(0,p,&secat,&secat,1,NORMAL_PRIORITY_CLASS,env,(doc_dir)?doc_dir:loc, &cb,&pi);
  t[1]=ec;
//CloseHandle(hwr);
//CloseHandle(hwre);
//if(hrdp)CloseHandle(hrdp);
//  debug("Create: ll=%d pl=%u %u",ll,pl,cl);
  if( ll )
  { ll=0; ec=STILL_ACTIVE; goto lb1;
    do{

       if(!l)
       {PeekNamedPipe(hrde,0,0,0,(ulong *)&l,0);
        if(l)
        {
       lbrde:
         if(!ll)
         {
           Sleep(100);
           PeekNamedPipe(hrd,0,0,0,(ulong *)&i,0);
           if(i){l=i ; goto lbrd;}
         }

         if( !(s_flgs[2]&FL2_NOERROUT) )
         {
            if(!ll){HttpReturnError("Error in script:\r\n"); ll=1;}
            else
            {
                if( send(s,"\r\n\r\n<hr>",8,0) <=0) goto tp;
            }
         }
         xchgp(hrde,hrd);
         if( (s_flgs[2]&(FL2_NOERROUT|FL2_DUBSTDERR)) ) fl ^= F_STDERRSEL;
         goto lbrd;
        }
        GetExitCodeProcess(pi.hProcess,(ulong *)&ec);
       }else
       {lbrd:
        //l=0;
//        debug("Read %d %d",l,GetLastError());
        if(l>0x8000)l=0x8000;
        ReadFile(hrd,p,l,(ulong *)&l,0);
//        debug("Read done: list %d %d",l,GetLastError());
        if( !ll )
        {
          ll=sizeof("HTTP/1.0 200 \r\n")-1;
          tt=0;
          if(strinnc(p,(char *)"Status: "))
          {
            p+=8;
            l-=8;
            ll=9;
            t="HTTP/1.0 ";
            http_status = atoui(p);
          }
          //         else t=( (stristr(p-1,(char *)"\nLocation:"))?(char *)"HTTP/1.0 301\r\n"
          else if(StrVar(p,"Location"))  t=(char *)"HTTP/1.0 301 \r\n";
          else
          {
            t=(char *)"HTTP/1.0 200 \r\n";
            t1=strstr(p,"\r\n\r\n");
            if(!t1) t1=strstr(p,"\n\n");

            if(loc==error_file) t=(char *)"HTTP/1.0 404 \r\n";
            else if( (!(fl&F_SKIPHEAD)) &&
              (!stristr(p,"Content-Length")) &&
              (stristr(p,"Content-type: text")) && t1
            )
            {
              if( (fl&F_GZ)
                && Snd != GZSnd
              )
              {
                DBG();
                IGZP();
                DBG();
              }
              else if( (s_flgs[2]&FL2_CHUNKED) && //Snd!=&TLSSend &&
                KeepAlive && ! (fl&F_HTTP10) )
              {
                t = (char *) ChunkedHead;
                ll = sizeof(ChunkedHead) - 1;
                fl |= F_UPCHUNKED;
              }
            }
          }
          if( send(s,t,ll,0)<0 )goto tp;
          if(gz && t1 && !(fl&F_SKIPHEAD))
          {send(s,p,i=t1-p+1+(*t1=='\r'),0);
            send(s,(char *)gz_head,sizeof(gz_head),0);
            i+=1+(*t1=='\r');
            if(i<l)memcpy(p,p+i,l-=i);
            else l=0;
            Snd= GZSnd;
          }
          /* !
          else if( t == ChunkedHead )
          {
            send(s,p,i=t1-p+((t1[0]=='\n')?2:4), 0);
            if(i<l)memcpy(p, p+i, l-=i);
            else l=0;
            fl |= F_CHUNKED;
          }
          */
        }

          if(l)
          {
            p[l]=0;
            if( send(s,p,l,0) <0)
            {tp:
              if(s_flgs[1]&FL1_NBRK)
              {
                do{

                  if(PeekNamedPipe(hrd,0,0,0,(ulong *)&l,0) && l)
                  { ReadFile(hrd,p,0x4000,(ulong *)&l,0); }
                  if(timeout<time(0))goto tp2;
                  Sleep(10000);
                  GetExitCodeProcess(pi.hProcess,(ulong *)&ec);
                }while( ec==STILL_ACTIVE );
              }
              else{tp2: TerminateProcess(pi.hProcess,0); }
              debug("Connection aborted %d..",GetLastError()); goto ex2;
            };
            ll+=l;
          }
        }
        lb1:
     //  Sleep(100);
        if(http_status == 101) // Websocket
        {
          do {
            if(PeekNamedPipe(hrd,0,0,0,(ulong *)&l,0) && l){
              ReadFile(hrd,p,0x4000,(ulong *)&l,0);
              Send(p,l);
            }
            if( (i=RESelect(0,10000,1,s)) > 0 ) {
              if( (l = Recv(p,0x4000)) <= 0 ) goto tp2;
              write(hwr, p, l);
            }

            if( (s_flgs[2]&(FL2_NOERROUT|FL2_DUBSTDERR)) ) {
              if(PeekNamedPipe(hrde,0,0,0,(ulong *)&l,0) && l) {
                l = read(hrde,p,0x4000);
                if(ll>0) {
                  if(! (fl&F_NAMEOUTED) )
                  {
                    #ifdef SEPLOG
                    sepLog[CGI_ERR_LOG]->LAddToLog(0,s,&sa_c46,">>%.256s ; %.512s; Websocket\r\n",loc,req?req:"");
                    #else
                    AddToLog(loc,s,&sa_c46,FmtShortErr);
                    #endif
                    fl|=F_NAMEOUTED;
                  }

                  if(!(fl&F_ERROUTED))
                  {
                    p[l] = 0;
                    #ifdef SEPLOG
                    //             sepLog[CGI_ERR_LOG]->Ldebug("%X:%.128s%s\r\n",fl,b, (l>128)?"...":"" );
                    sepLog[CGI_ERR_LOG]->Ldebug("%.128s%s\r\n",p, (l>128)?"...":"" );
                    #else
                    debug("%.128s%s\r\n",p, (l>128)?"...":"" );
                    #endif
                    if(ll>128 ) // || Tout>128 )
                      fl |= F_ERROUTED ;
                  }
                }
              }
            }

            GetExitCodeProcess(pi.hProcess,(ulong *)&ec);
          } while(ec==STILL_ACTIVE);
          break;
        }



#ifndef QW1
   if(cl>0) do{
     if(pl>(cl+2))pl=(cl+2);
  //   if(pl>cl)pl=cl;
//     debug("cl=%d pl=%u",cl,pl);
     l=0;
     if(pl>0){WriteFile(hwrp,t2,pl,(ulong *)&l,0);
      pl-=l; cl-=l; t2+=l;
     }
//     debug("cl=%d pl=%u,%u",cl,pl,l);
     if( cl<=0)
     {
//      debug("Close hwrp1...");
      //FlushFileBuffers(hwrp);
      CloseHandle(hwrp);
      hwrp=0;
//      debug("Close hwrp...");
      break;
     }
     if(pl>0)break;
     t2=p;          //MIN(0x10000,cl)
     do{
      l=0;
      if(PeekNamedPipe(hrd,0,0,0,(ulong *)&l,0) && l) goto lbrd;
      if(PeekNamedPipe(hrde,0,0,0,(ulong *)&l,0) && l) goto lbrde;
      GetExitCodeProcess(pi.hProcess,(ulong *)&ec);
      if(ec!=STILL_ACTIVE)goto exLp2;
     }while( !RESelect(0,30000,1,s) );
     if( (pl=Recv(p,0x8000))<0 )goto tp;
   }while(pl);
   else pl=0;
#endif

//  if( WaitForMultipleObjects(3,ahwr,0,1000)==(WAIT_OBJECT_0 + 2) )
       if( RESelect(0,10240,1,s) ) if( Recv(p,0x8000)<=0 ) goto tp;
       //if(! --timeout) goto tp2;
       if(timeout<time(0)) goto tp2;
       PeekNamedPipe(hrd,0,0,0,(ulong *)&l,0);
//       debug("%X %X \r\n",ec,l);
      }while( (ec==STILL_ACTIVE) || l );
 exLp2:
    if(ll==0)
    {ern:
      HttpReturnError("Error. Script return nothing");
//      debug("lr=%d %X %X ll=%d cl=%u" ,GetLastError(),pi.hProcess,pi.hThread,ll,cl);
    };
 ex2:
    CloseHandle(pi.hThread); CloseHandle(pi.hProcess);
   }
   else
   {
  // errun:
  //   if(t) *t='\\';
     sprintf(p+1024,"Error run %s with %s (%s)",loc+dirlen,req,p);
     HttpReturnError(p+1024);
   }
ex:
//  delete (p-1);
  delete pp;
  if(hrdp)
  {CloseHandle(hrdp);
   if(hwrp)CloseHandle(hwrp);
  }

 }else{ dbg("No Memory");  }
 CloseHandle(hwr);
 CloseHandle(hrd);
 CloseHandle(hwre);
 CloseHandle(hrde);
 return 1;
}
#else

#ifdef _BSD_VA_LIST_
extern "C" {
extern char** environ;
#define __environ environ
};
#endif

int Req::MakeEnv(char *env, char *tend,char ***new_env)
{
 char *t,**tt,*penv,*z,**tt1;
 //struct sockaddr_in sa;
 int l;

#define msprintf(a,b...) msprintfchk(a,tend,b)

 //=sizeof(sockaddr_in);
 //getpeername(s,(sockaddr *)&sa,(uint *) &l);
// tend=env+0x3000+Tin;
   z=(char *) (http_var+MAX_HTTP_VARS+2);
   //inet_ntoa(sa.sin_addr);
   t=penv=env+msprintf(env,"GATEWAY_INTERFACE=CGI/1.1\nREQUEST_METHOD=%s\n"
           "SERVER_PROTOCOL=HTTP/1.0\n"
//         "SERVER_PORT=%u\n"
//         "REMOTE_ADDR=%s\n
         "REMOTE_HOST=%s\n"
           "QUERY_STRING=%s\nSERVER_SOFTWARE=Small_HTTP_Server\n"
           "SCRIPT_FILENAME=%s\n"
           "PATH_TRANSLATED=%s\n"
           "DOCUMENT_ROOT=%.128s\n"
           ,((fl&F_POST)?"POST":"GET"),
//           http_port,z,
           z,req,loc,loc,dir);
     penv+=(vhdir && vhdir->h[0]=='/')? msprintf(penv,"SCRIPT_NAME=%s%s\n",vhdir->h,loc+dirlen+1):
     msprintf(penv,"SCRIPT_NAME=%s\n",loc+dirlen);
     if( trn )penv+=msprintf(penv,"PATH_INFO=/%s\n",trn);
     for(;*t;t++)if(*t=='\\')*t='/';
   t=tend - 8;
   for(tt=http_var;(*tt) && (**tt) && (tt[1]);tt+=2)
   {
//    if(tend<=penv)break;
       //penv+=sprintf(penv,"HTTP_%.64s=%.8192s\n%.64s=%.8192s\n",*tt,tt[1],*tt,tt[1]);
//     penv+=sprintf(penv,"HTTP_%.64s=%.8192s\n",*tt,tt[1]);
     if(t<=penv)break;
     penv+=l=msprintf(penv,"HTTP_%.64s=%.8192s\n",*tt,tt[1]);
     if(!l)break;
   }
   l=( ( t=GetVar(http_var,"CONTENT_LENGTH") )?atoui(t):0) ;
/*
   if( (t= GetVar(http_var,"AUTHORIZATION")) )
   {
    char *y,*z;
    ulong i,j;
       z=y=penv+200;
    t+=6;
    while((i=D64X(*t))<64)
    {
     t++;
     j=D64X(*t);
     *y=(i<<2)|(j>>4);
     t++; y++;
     if( (i=D64X(*t))>=64 )break;
     *y=(j<<4)|(i>>2);
     t++; y++;
     if( (j=D64X(*t))>=64 )break;
     *y=(i<<6)|(j);
     t++; y++;
    }
    *y=0;
    if((y=strchr(z,':'))){*y=0;y++;}else y="";
    penv+=sprintf(penv,"AUTH_TYPE=Basic\nREMOTE_USER=%s\nAUTH_PASS=%s\n",z,y);
   }
*/
#ifndef A_64
   *new_env=tt=(char **) ((u_long)(penv+7)&0xFFFFfffc);
#else
   *new_env=tt=(char **) ((u_long)(penv+15)& ~7ul);
#endif
   for(*tt=t=env;t<penv;t++)
   {

    if(*t=='\n')
    {
     *t=0;
     *++tt=t+1;
     if(DWORD_PTR(t[1])==0x50545448 x4CHAR("HTTP") ) *++tt=t+1+5;
    }
   };
   tt1=__environ;
   while(   (*tt++ = *tt1++)   );
   //debug("Size of env %u l=%u",tt-*new_env,l);
   //for(tt=*new_env;*tt;++tt)debug("%s",*tt);
   *tt=0;
   return l;
#undef msprintf
};

//----------------------
struct timeval  socktimeout={
  60,60
};
int Req::ExecCGIEx()
{
 int phrd[2],phrdp[2],phrde[2],child_pid;
 char *argv[12] ;
 #define  hrd  (phrd[0])
 #define  hwr  (phrd[1])
 #define  hrde (phrde[0])
 #define  hwre (phrde[1])
 #define  hrdp (phrdp[0])
 #define  hwrp (phrdp[1])


 char *p,*pp,*t,**tt,*e,**new_env,*t1; //,*ee=eenv;
 long ll,ec=0,pl=0,cl=0,l,i;
 char *env,*penv; //,z[300];
 ulong timeout=cgi_timeout+time(0);
 int status;
 int http_status = 200;

///!  DEBUG

 char *einf;
 i=strlen(inf);
 if(i>64)i=64;
 einf=inf+i;


/// //////////*/
//#define PDBG(a,b...)

//! Test

  setsockopt(s,SOL_SOCKET, SO_SNDTIMEO,&socktimeout,sizeof(socktimeout));

/// //////////

DBG();
// z[0]=0;
// e=z;
// if( s_flgs[1]&(FL1_ISAPI|FL1_ISAPIALL) && (t=strstrnc(loc,".so")) && t[3]<='.' ) return  ExecDllEx(loc);
 //for(tt=ext; *tt ;tt+=2) if( (*tt[0]=='.') && strstrnc(loc,*tt) )
 //{e=tt[1];  goto llgs1; }
 //llgs2:;
 //llgs1:;
 if( pipe(phrdp) || pipe(phrd) || pipe(phrde) )
 {
 er000:;
   return HttpReturnError("Error. Can\'t run script\r\n");
 };
DBG();
 if(pst && loc) i = pst-loc;
 else i=Tin;

 if(((uint)i)>8192)i=8192;

 if( (p=(char * )malloc(0x14800+i)/* new char[0x14100]*/ )  )
 { pp=p;
//debug("******** p=0x%X",p);
   env=p+0x10800;
   cl=MakeEnv(env,env+0x3C00+i,&new_env);
DBG();

   if(cl>post_limit)
   {
       /*dbg("Very big post...");*/

//       debug("Post size excetpt maximum")
       debug("Size of method POST %u exceeds maximum %u",cl,post_limit);
       HttpReturnError("Size of method POST exceeds maximum");
       //goto ern;
       goto ex;

   };

   DBGLA("run %s", loc)

// debug("|%s %s %.32s cl=%u %u %.250s %d %d |",loc,req,pst,cl,postsize,env,fileStat->st_gid,fileStat->st_uid);
    if( !(child_pid=fork()) )
    {
      // Child
   //  debug("Run %s %X\n",loc,req);
     dup2(hrdp,0);
     dup2(hwr,1);
     dup2(hwre,2);

   //  strcpy(p,loc);
     if((t=strrchr(loc,'/')))
     {*t=0;
      chdir((doc_dir)?doc_dir:loc);
      *t='/';
     }

     if((i=(fileStat->st_gid)))setregid(i,i);
     if((i=(fileStat->st_uid)))setreuid(i,i);


      if(phtml_dir && (fl&F_PHP) ) //( strstr(loc,".php") || strstr(loc,".phtm") ) )
      {
       argv[0]=t=phtml_dir;
       for(i=1; i<10 ; ++i )
       {
         if(!(t=strpbrk(t," \t"))) break;
         *t++=0;
         t=SkipSpace(t);
         if( *t=='\"' || *t=='\'' )
         {
          argv[i]=++t;
          t=strchr_meta(t,t[-1]);
          if(!t)break;
          *t++=0;
         }
         else
         {
          argv[i]=t;
         }

       }
       argv[i]=loc;
       argv[i+1]= 0;
       execve(phtml_dir,argv,new_env);
      }
      else
      {
       argv[0]=loc;
       argv[1]=
      //req;
      //argv[2]=
       0;


     //if(
       execve(loc,argv,new_env);
      // <0 )fprintf(stderr, "Exec error %s %d " SER,loc ,errno Xstrerror(errno));

      }

DBG();
     exit(0);
    }else if (child_pid>0)
    {
DBG();
     // fcntl(hrd,F_SETFL,O_NONBLOCK);
     // fcntl(hrde,F_SETFL,O_NONBLOCK);
 //     debug("postsize=%u %u %u %u", postsize ,(fl&F_POST),  pst , cl );
      ec=0;
      if( (fl&F_POST) && pst && cl )
      {
DBG();
        pl=postsize; t=pst;
        do{
           if(pl && write(hwrp,t,pl)<=0) break;
           if( (cl-=pl)<=0 )break;
           t=p;
           if((ec=waitpid(child_pid,(int *)&status,WNOHANG))) break;
          }while( (pl=Recv(p,MIN(0x10000,cl)))>0 );
      }
DBG();
      ll=0;

      do
      {
	l=0;
//        debug("Script loop %d\n",child_pid);

DBG();

//        if(RESelect(0,4096,1,hrde)>0 )
        if( (i=RESelect2(0,0x10000,hrd,hrde)) == hrde )
        {
DBG();

        lberr:
      //  debug("error in script %u %u",ll,Tout);
         if(RESelect(0,0x10000,1,hrd)) goto lbrd;
         if(! (s_flgs[2]&FL2_NOERROUT) )
            if(!ll)
            {
              HttpReturnError("Error in script:\r\n");
              ll=1;
            }
            else
            {
                if( send(s,"\r\n\r\n<hr>",8,0) <=0) goto tp;
            }


         if( (s_flgs[2]&(FL2_NOERROUT|FL2_DUBSTDERR)) ) fl ^= F_STDERRSEL;

         xchg(hrde,hrd);


DBG();

         goto lbrd;
        }
        else
        {
          if(i==hrd) goto lbrd;
        }

        if(RESelect(0,0x10000,1,hrd)>0 )
        {
     lbrd:
DBG();
        l=0;
        p=pp;
PDBG("read %d %d hrd=%d fl=%X",ll,ec,hrd,fl);

        do{
         i=read(hrd,p+l,0x4000-l);
	 if(i<=0)break;
	 l+=i;
	}while(l<0x3E00
	       && (!(i&0x1FF)) // i==PIPE_BUF
	       && RESelect(0,8192,1,hrd)>0);
    DBG();

    if(l>0)
    {
      DBG();

      if( !ll )
      {
        DBG();

        ll=sizeof("HTTP/1.0 200 \r\n")-1;
        if(strinnc(p,(char *)"Status:")) {
          p=SkipSpace(p+7);
          l-=ll=p-pp; t="HTTP/1.0 ";
          http_status = atoui(p);
          DBGLA("Status %.20s = %u\r\n",p, http_status);
        }
        //           else t=( (stristr(p-1,(char *)"\nLocation:"))?(char *)"HTTP/1.0 301\r\n"
        else if(StrVar(p,"Location"))
        {
          t=(char *)"HTTP/1.0 301 \r\n"; //ll=sizeof("HTTP/1.0 301 \r\n")-1;
          http_status = 301;
        }
        else
        {
          t=(char *)"HTTP/1.0 200 \r\n";
          t1=strstr(p,"\r\n\r\n");

          if( (!(fl&F_SKIPHEAD)) &&
            (!stristr(p,"Content-Length")) &&
            (stristr(p,"Content-type: text")) && t1
          )
          {
            if( (fl&F_GZ)
                && Snd != GZSnd )
            {
              DBG();
              IGZP();
              DBG();
            }
            else if(
                     (s_flgs[2]&FL2_CHUNKED) && //Snd!=&TLSSend &&
                     KeepAlive && ! (fl&F_HTTP10) )
            {
              t = (char *) ChunkedHead;
              ll = sizeof(ChunkedHead) - 1;
              fl |= F_UPCHUNKED;
            }
          }
        }
        // printf("|%.12s| %X",t,gz);
        if( send(s,t,ll,0)<=0 )
        {
          DBG();
          goto tp;
        }
        if(gz && t1 && (!(fl&F_SKIPHEAD)) && Snd != GZSnd )
        {
          DBG();
          send(s,p,i=t1-p+2,0);
          send(s,(char *)gz_head,sizeof(gz_head),0);
          DBG();
          i+=2;
          if(i<l)memcpy(p,p+i,l-=i);
          else l=0;
          Snd= GZSnd;
        }
     /*
        else if( t == ChunkedHead ) {
          send(s,p,i=t1-p+4,0);
          if(i<l)memcpy(p,p+i,l-=i);
          else l=0;
          fl |= F_CHUNKED;
        }
      */
        //  printf("=|%.82s|= %u",p,l);
        if(http_status == 101) // Websocket
        {
          do {
            if( (i=RESelect2(0,0x10000,hrd,s)) == s ) {
              if( (l = Recv(p,0x4000)) <= 0 ) goto tp2;
              write(hwr, p, l);
            } else if(i == hrd) {

              if( (l = read(hrd,p,0x4000))<=0) goto tp2;
              Send(p,l);
            }

            if( (s_flgs[2]&(FL2_NOERROUT|FL2_DUBSTDERR)) ) {
              if( RESelect(0, 0, 1, hrde)) {
                l = read(hrde,p,0x4000);
                if(l>0) {
                  if(! (fl&F_NAMEOUTED) )
                  {
                    #ifdef SEPLOG
                    sepLog[CGI_ERR_LOG]->LAddToLog(0,s,&sa_c46,">>%.256s ; %.512s; Websocket\r\n",loc,req?req:"");
                    #else
                    AddToLog(loc,s,&sa_c46,FmtShortErr);
                    #endif
                    fl|=F_NAMEOUTED;
                  }

                  if(!(fl&F_ERROUTED))
                  {
                    p[l] = 0;
                    #ifdef SEPLOG
                    //             sepLog[CGI_ERR_LOG]->Ldebug("%X:%.128s%s\r\n",fl,b, (l>128)?"...":"" );
                    sepLog[CGI_ERR_LOG]->Ldebug("%.128s%s\r\n",p, (l>128)?"...":"" );
                    #else
                    debug("%.128s%s\r\n",p, (l>128)?"...":"" );
                    #endif
                    if(l>128 ) // || Tout>128 )
                      fl |= F_ERROUTED ;
                  }
                }
              }
            }


            if( ec>=0 && ec!=child_pid)
              ec=waitpid(child_pid,(int *)&status,WNOHANG);
          }while(!ec);
          break;
        }
      }

PDBG("%d %d %d hrd=%d fl=%X",ll,l,ec,hrd,fl);
          if(l>0)
          {
             p[l]=0;
             if( send(s,p,l,0) <=0)
             {tp:
    DBG();
                if(s_flgs[1]&FL1_NBRK)
                {
                  DBG();
                  do{
                    if(timeout<time(0))goto tp2;
                    if(RESelect(10,50000,1,hrd)>0)
                      if(read(hrd,p,0x4000)<=0) goto tp2;
                      if( ec>=0 && ec!=child_pid)
                        ec=waitpid(child_pid,(int *)&status,WNOHANG);
                  }while( (!ec) );
                }
                else if( ec>=0 && ec!=child_pid)
                {
                  tp2:
                  pid_to_wait = 0;
                  kill(child_pid,9);
                  DBG();
                  //sleep(1);
                  SrvEventWait(&pid_to_wait,1000);
                  DBG();
                  ec=waitpid(child_pid,(int *)&status,WNOHANG);
                }
                while( ec>0 && ec!=child_pid)
                {
                  DBG();
                  ec=waitpid(child_pid,(int *)&status,WNOHANG);
                }
    DBG();
                dbg("Connection aborted..\r\n ");
                goto ex;
             };

             ll+=l;
          }
         }
       lb1:;
        // ioctl(hrd,FIONREAD,(ulong *)&l);
        }
        if(timeout<time(0) && ec==0 )goto tp2;

        if(l<=0)
        {
DBG();

          if( (!ll) && RESelect(0,1024,1,hrde)>0 ) goto lberr ;
DBG();
         if( ec>=0 && ec!=child_pid)  ec=waitpid(child_pid,(int *)&status,WNOHANG);
         while( ec>0 && ec!=child_pid)
         {
           DBG();
           ec=waitpid(child_pid,(int *)&status,WNOHANG);
         }
         DBG();
         if(ec>=0 && ec!=child_pid && WIFSTOPPED(status))
         { goto tp2;  }
        }

DBG();

        if(RESelect(0,1024,1,s) )
        {
DBG();

            if( Recv(p,0x8000)<=0 ) goto tp;
        }

    }while(!ec);
DBG();
     if(ll==0)
     {ern:
       HttpReturnError("Error.Script return nothing\r\n");
     };
    }else{
    //errun:
      //if(t) *t = '\\';
      sprintf(p,"Error run %s with %s \r\n",loc+dirlen,req);
      HttpReturnError(p);
    }
 ex:
DBG();
//  debug("!******** p=0x%X",p);
  free(pp); //delete p;
DBG();
 }else
 {
     dbg("No Memory");
   //  debug("i=%u %s %d",i,loc,errno);
 }
 close(hwrp);
 close(hrdp);
 close(hwr);
 close(hrd);
 close(hwre);
 close(hrde);
DBG();
 return 1;

 #undef  hrd
 #undef  hwr
 #undef  hrdp
 #undef  hwrp

}



#endif
#undef send
#undef recv

