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


#ifndef SRV_H
#include "srv.h"
#define DBG_WRITE(b,l,x)
#endif
//#include "tlsdll.h"


#ifdef SEPLOG

#undef debug
#undef AddToLog

#define debug(a...)  sepLog[5]->Ldebug(a)
#define AddToLog(a...)  sepLog[5]->LAddToLog(a)

#endif

#define DBG_PRINT(a...)
//debug(a)
//DBGLA(a)
//debug


char *CApath,
     *CAfile,
     *s_cert_file="server.pem",
     *s_key_file //="s1024key.pem"
;
char *TLSLibrary=
#ifdef SYSUNIX
          "./libsec111.so";
#else
          "libsec111.dll";
#endif

#ifndef  TLSWODLL

int (*PInitLib)( TFprintf prnt,TFtransfer fsend,TFtransfer frecv,
  char *CApath,char *CAfile, char * s_cert_file,char * s_key_file);
int (*PSecAccept)(OpenSSLConnection *s);
int (*PSecRecv  )(OpenSSLConnection *s,char *b,int l);
int (*PSecSend  )(OpenSSLConnection *s,char *b,int l);
int (*PSecClose )(OpenSSLConnection *s);
int (*PSecConnect)(OpenSSLConnection *s, int anon, char *verfyhost);
int (*PSecUpdateCB )(OpenSSLConnection *s);

#ifndef SYSUNIX
HINSTANCE hSecDLL;

#else

#ifdef LPC_ARM
#include "nommc_dlfcn.h"
#else

#include <stdlib.h>
#include <dlfcn.h>

#endif
#ifndef MINGW
#define HINSTANCE void *
//typedef  void *HINSTANCE;
#endif

HINSTANCE hSecDLL;

//#define LoadLibrary(a)   dlopen(a,2)
//#define LoadLibrary(a)   dlopen(a,RTLD_LAZY | RTLD_GLOBAL)
//#define LoadLibrary(a)   dlopen(a,RTLD_NOW| RTLD_GLOBAL)
#define LoadLibrary(a)   dlopen(a,RTLD_NOW|RTLD_LOCAL)
#define GetProcAddress(a,b)   dlsym(a,b)

#endif
int SecConnectAbcent(OpenSSLConnection *s, int anon, char *vh)
{
  debug("Current version of TLS library doesn't content 'SecConnect' function. Please update it...");
  return 0;
}
int SecUpdateCBAbcent(OpenSSLConnection *s)
{
  debug("Current version of TLS library doesn't content 'SecUpdateCB' function. Please update it...");
  return 0;
}

int InitSecDLL()
{

#ifdef x86_64
   dlerror();
   hSecDLL=dlopen(TLSLibrary
       , //RTLD_LAZY|RTLD_GLOBAL
         RTLD_NOW|RTLD_LOCAL
   );
   if(!hSecDLL)
   {
    void *main_h;
    Lmid_t  lmid;


     printf("Error loading TLS/SSL library: %s %s %u %d\n",dlerror(),TLSLibrary, RTLD_NOW|RTLD_LOCAL, errno);


     main_h=dlopen(0,//RTLD_NOLOAD|
                     RTLD_LOCAL|RTLD_LAZY);

     if(!main_h)
     {
       printf("Error get handle for main: %s %s %u %d\n",dlerror(),TLSLibrary, RTLD_NOW|RTLD_LOCAL, errno);
       lmid=LM_ID_BASE; //LM_ID_NEWLM;
     }
     else if(dlinfo(main_h,RTLD_DI_LMID,&lmid)<0)
     {

       printf("Error get dlinfo for main: %s %s %d\n",dlerror(),TLSLibrary, errno);
       //return 0;
       lmid=LM_ID_NEWLM;

     };

     hSecDLL=dlmopen(//LM_ID_BASE
       // LM_ID_NEWLM
        lmid
        ,TLSLibrary
             , //RTLD_LAZY|RTLD_GLOBAL
              RTLD_NOW|RTLD_LOCAL
           );
     if(!hSecDLL)
     {
         printf("Error loading TLS/SSL library: %s |%s| %d %s\n",dlerror(),TLSLibrary,  errno,strerror(errno));

        return 0;
     }

   }
   dlerror();
   if(
     (!(PSecAccept=(TSecAccept) GetProcAddress(hSecDLL,"SecAccept"))) ||
     (!(PSecRecv  =(TSecRecv )  GetProcAddress(hSecDLL,"SecRecv"))) ||
     (!(PSecSend  =(TSecSend )  GetProcAddress(hSecDLL,"SecSend"))) ||
     (!(PSecClose =(TSecClose)  GetProcAddress(hSecDLL,"SecClose"))) ||
     (!(PInitLib=(TInitLib) GetProcAddress(hSecDLL,"InitLib")))
  )
  {
   printf("Error getfunction TLS/SSL library: %s %s\n",dlerror(),TLSLibrary);
   return 0;
  }
  PSecConnect = (TSecConnect) GetProcAddress(hSecDLL,"SecConnect");
  PSecUpdateCB = (TSecUpdateCB) GetProcAddress(hSecDLL,"SecUpdateCB");

#else // not  x86_64

#ifdef SYSUNIX
  dlerror();
#endif

  hSecDLL=LoadLibrary(TLSLibrary);

  if(!hSecDLL)
  {
#ifdef SYSUNIX
   printf("Error loading TLS/SSL library: %s %s\n",dlerror(),TLSLibrary);
#endif
   return 0;

  }
  else if(
     (!(PSecAccept=(TSecAccept) GetProcAddress(hSecDLL,"SecAccept"))) ||
     (!(PSecRecv  =(TSecRecv )  GetProcAddress(hSecDLL,"SecRecv"))) ||
     (!(PSecSend  =(TSecSend )  GetProcAddress(hSecDLL,"SecSend"))) ||
     (!(PSecClose =(TSecClose)  GetProcAddress(hSecDLL,"SecClose"))) ||
     (!(PInitLib=(TInitLib) GetProcAddress(hSecDLL,"InitLib")))
  )
  {
#ifdef SYSUNIX
   printf("Error in TLS/SSL library interface: %s %s\n",dlerror(),TLSLibrary);
   dlclose(hSecDLL);
#endif
   return 0;
  }
  PSecConnect = (TSecConnect) GetProcAddress(hSecDLL,"SecConnect");
  PSecUpdateCB = (TSecUpdateCB) GetProcAddress(hSecDLL,"SecUpdateCB");


#endif

 if(!PSecConnect) PSecConnect = SecConnectAbcent;
 if(!PSecUpdateCB) PSecUpdateCB = SecUpdateCBAbcent;


 if(tls_priority && tls_priority[0])
 {
  TSetPriority SetPriority;

  if( (SetPriority=(TSetPriority) GetProcAddress(hSecDLL,"SetPriority") ) )
  {
    SetPriority(tls_priority);
  }
  else
  {
    debug("SetPriority is abcent in the library\n");
  }
 }
 return 1;
}

#endif

char *chunke_bfr;
int chunke_mutex;
#define MAX_FRAG_SIZE 0x10000

int TLSSend(Req *th, const void *b,int l)
{
 int r;
 int lock=0;
 int ll=l; ///!!! debug
 DBG_WRITE((char *)b,l,2);

 if( (th->fl & F_CHUNKED) && l>0)
 {
   int l2;
   while( l > MAX_FRAG_SIZE)
   {
     if((r=TLSSend(th,b,MAX_FRAG_SIZE)) <= 0) return r;
     l -= MAX_FRAG_SIZE;
     DWORD_PTR(b) += MAX_FRAG_SIZE;
   }
   lock = MyLock(chunke_mutex);
   if(!chunke_bfr) chunke_bfr = (char *) malloc(MAX_FRAG_SIZE+32);
   l2 = sprintf(chunke_bfr, "%X\r\n", l);
   memcpy(chunke_bfr+l2, b, l);
   b = chunke_bfr;
   l+=l2;
   WORD_PTR(chunke_bfr[l])=0xA0D;
   l+=2;
   //if( (r=SecSend((OpenSSLConnection *)(th->Adv),chu,) ))<=0 ) return r;
 }
 while(l>0)
 { r= SecSend((OpenSSLConnection *)(th->Adv),(char *)b,l);
   if(r<=0)break;
   l-=r;
   DWORD_PTR(b)+=r;
 }
 if(lock) MyUnlock(chunke_mutex);
 DBG_PRINT("TLS send s=%d r=%d l=%d",th->s,r,ll);

 return r;
};

int TLSRecv(Req *th,void *b,int l)
{
 int r;
 r=SecRecv((OpenSSLConnection *) (th->Adv),(char *)b,l);
 DBG_PRINT("TLS recv r=%d l=%d", r, l);
 DBG_WRITE((char *)b,r,3);
 return r;
};

int TLSmutex;
int Req::TLSBegin(OpenSSLConnection *x, int type, char *verfyhost)
{
  Snd=(tfSnd) &TLSSend;
  Rcv=(tfRcv) &TLSRecv;
  Adv=x;
  x->CallbackParam=this;
  x->state = 0;
  if(timout< 30) timout = 30;
  DBG_PRINT("TLS request");
  MyLock(TLSmutex);
  if( (type & tbtAccept) ? SecAccept(x) : SecConnect(x,type,verfyhost) )
  {
      MyUnlock(TLSmutex);
      return 1;
  }
  MyUnlock(TLSmutex);
  Snd=(tfSnd) &JustSnd;
  Rcv=(tfRcv) &JustRcv;
  return 0;
}

#ifndef VPNCLIENT_ONLY

int Req::TLSReq()
{
  OpenSSLConnection x;
#if 0
  Snd=(tfSnd) &TLSSend;
  Rcv=(tfRcv) &TLSRecv;
  Adv=&x;
  x.CallbackParam=this;
  DBG_PRINT("TLS request");
  if(SecAccept(&x))
#else
    if(TLSBegin(&x))
#endif
    {
      DBG_PRINT("TLS accept Ok");

      HttpReq();

      //   SecRecv(&x,(char *)b,10);
      if(! (fl & F_KEEP_ALIVE) ) {
        /*
        SecClose(&x);
        Snd=(tfSnd) &JustSnd;
        Rcv=(tfRcv) &JustRcv;
        */
        Close();
      }
      else if( ! (fl & F_VPNANY) )
      //if( (fl & F_KEEP_ALIVE) && ! (fl & F_VPNANY) )
      {
        TryToAddKeepAlive(this);
      }
    }
    else
    {
      AddToLog("TLS error\r\n",s,&sa_c46);
    }
    DBG_PRINT("TLS end");
    return 1;
};

#endif // VPNCLIENT_ONLY

