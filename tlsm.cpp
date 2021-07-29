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

#define DBG_PRINT(a...)
//debug

#ifdef SEPLOG

#undef debug
#undef AddToLog

#define debug(a...)  sepLog[5]->Ldebug(a)
#define AddToLog(a...)  sepLog[5]->LAddToLog(a)

#endif



char *CApath,
     *CAfile,
     *s_cert_file="server.pem",
     *s_key_file //="s1024key.pem"
     ,*TLSLibrary=
#ifdef SYSUNIX
"./libsec.so"
#else
"seclib.dll"
#endif
;
#ifndef  TLSWODLL
int (*PInitLib)( TFprintf prnt,TFtransfer fsend,TFtransfer frecv,
  char *CApath,char *CAfile, char * s_cert_file,char * s_key_file);
int (*PSecAccept)(OpenSSLConnection *s);
int (*PSecRecv  )(OpenSSLConnection *s,char *b,int l);
int (*PSecSend  )(OpenSSLConnection *s,char *b,int l);
int (*PSecClose )(OpenSSLConnection *s);
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

int InitSecDLL()
{
    
#ifdef x86_64
   dlerror(); 
   hSecDLL=dlopen(TLSLibrary
      // "/dev/shm/o/o64/libsec.so"
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
   
   

#else
    
#if 1
//ndef SYSUNIX
  if(
     (!(hSecDLL=LoadLibrary(TLSLibrary))) ||
     (!(PSecAccept=(TSecAccept) GetProcAddress(hSecDLL,"SecAccept"))) ||
     (!(PSecRecv  =(TSecRecv )  GetProcAddress(hSecDLL,"SecRecv"))) ||
     (!(PSecSend  =(TSecSend )  GetProcAddress(hSecDLL,"SecSend"))) ||
     (!(PSecClose =(TSecClose)  GetProcAddress(hSecDLL,"SecClose"))) ||
     (!(PInitLib=(TInitLib) GetProcAddress(hSecDLL,"InitLib")))
  )
  {
#ifdef SYSUNIX
   printf("Error loading TLS/SSL library: %s %s\n",dlerror(),TLSLibrary);
#endif
   return 0;
  }
#else
  
//  hSecDLL==dlopen(TLSLibrary);


#endif

#endif


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

int TLSSend(Req *th, const void *b,int l)
{
 int r;
 DBG_WRITE((char *)b,l,2);
 while(l>0)
 { r= SecSend((OpenSSLConnection *)(th->Adv),(char *)b,l);
   if(r<=0)break;
   l-=r;
   DWORD_PTR(b)+=r;
 }
 DBG_PRINT("TLS send r=%d l=%d",r,l);
 return r;
};

int TLSRecv(Req *th,void *b,int l)
{
 int r;
 r=SecRecv((OpenSSLConnection *) (th->Adv),(char *)b,l);
 DBG_PRINT("TLS recv r=%d l=%d",r,l);
 DBG_WRITE((char *)b,r,3);
 return r;
};

int TLSmutex;
int Req::TLSBegin(OpenSSLConnection *x)
{
  Snd=(tfSnd) &TLSSend;
  Rcv=(tfRcv) &TLSRecv;
  Adv=x;
  x->CallbackParam=this;
  DBG_PRINT("TLS request");
  MyLock(TLSmutex);
  if(SecAccept(x))
  { 
      MyUnlock(TLSmutex);
      return 1;
  }
  MyUnlock(TLSmutex);
  Snd=(tfSnd) &JustSnd;
  Rcv=(tfRcv) &JustRcv;
  return 0;
}
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
   SecClose(&x);
  }
  else
  {
    AddToLog("TLS error\r\n",s);    
  }
  DBG_PRINT("TLS end");
  return 1;
};


