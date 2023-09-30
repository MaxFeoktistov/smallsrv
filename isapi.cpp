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
#endif


#ifdef SYSUNIX

#include <dlfcn.h>

#define LoadLibrary(a)   dlopen(a,2)
#define FreeLibrary(a)   dlclose(a)
#define GetProcAddress(a,b)   dlsym(a,b)
#define SetLastError(a) {errno=a;}

struct SEHRec
{
 void *h;
 // inline void BegTry(void *hndlr){h=hndlr;};
//#define  BegTry(a){if( setjmp( req.jmp_env ) ) goto a; }
  inline void EndTry(){};
};
#define except()

#define GetCurrentDirectory(a,b) getcwd(b,a)
#endif

extern "C" {

#include "isapi.h"
//#ifdef DJGPP
#if defined(DJGPP) || defined(MINGW)
#include "wgnuext.cpp"
#endif
//char   TskStat[30];
#define xdbg(ttt)
#define zdbg(ttt) {AddToLog(ttt,e->sock);};
#define send(a,b,c,d) a->SendChk(b,c)
#define recv(a,b,c,d) a->Recv(b,c)

BOOL WINAPI GetServerVariable(EXTENSION_CONTROL_BLOCK *e,LPSTR lpszVariableName,LPVOID lpvBuffer,LPDWORD lpdwSize )
{
 char *t;
 int ll,kl;

 ll=strlen(lpszVariableName);
// debug("GetServerVariable |%s| %u",lpszVariableName,*lpdwSize);
 for(t=e->bf+512;*t;t=memchr(t,0,16384)+1)
   if( ( (t[ll]=='=')  && striin(t,lpszVariableName)  )
     )
   {
    kl=*lpdwSize;
    t+=ll+1;
    if(kl)strncpy((char *)lpvBuffer,t,kl);
    *lpdwSize=(ll=strlen(t)+1);
    if( ll>kl  )
    {
      SetLastError(122); //ERROR_INSUFFICIENT_BUFFER    );
      return 0;
    }
    return 1;
   }
 if(*lpdwSize && lpvBuffer)*(char *)lpvBuffer=0;
 SetLastError(1413);// ERROR_INVALID_INDEX);
 return 0;
};


BOOL WINAPI WriteClient( EXTENSION_CONTROL_BLOCK *e,LPVOID Buffer,LPDWORD lpdwBytes,DWORD dwReserved )
{
 char abuf[512];
 char *t;

 if(e->state&2)return 0;
// debug("WriteClient state=%X flg=%X %.128s ",e->state,e->sock->fl,Buffer);
 if(!(e->state&1))
 {if(!e->dwHttpStatusCode)e->dwHttpStatusCode=200;
  if( DWORD_PTR(*(char *)Buffer) != 0x50545448 x4CHAR("HTTP") )
   if( send(e->sock,abuf,sprintf(abuf,"HTTP/1.0 %u \r\nContent-type: text/html\r\n\r\n",e->dwHttpStatusCode),0) <=0)
   {
 lbErr:
    e->state|=2;
    return 0;
   }
  e->state|=1;
 }
/*
 else if( (e->state&4) &&  DWORD_PTR(*(char *)Buffer) == 0x50545448 x4CHAR("HTTP") )
 { if( t=strstr((char *)Buffer,"\r\n\r\n"))
   {*lpdwBytes-=((char *)Buffer)-t-4;
    Buffer=t+4;
   }
   e->state^=4;
 }
*/
 if( (*lpdwBytes=send(e->sock,(char *)Buffer,*lpdwBytes,0)) <=0)
 { goto lbErr;
//  e->state|=2;
//   return 0;
 }
 return 1;
};

BOOL WINAPI ReadClient( EXTENSION_CONTROL_BLOCK *e,LPVOID lpvBuffer,LPDWORD lpdwSize )
{
  if( ( *lpdwSize=recv(e->sock,(char *)lpvBuffer,*lpdwSize,0) )<=0 )return 0 ;
  return 1;
};

BOOL WINAPI ServerSupportFunction( EXTENSION_CONTROL_BLOCK *e,DWORD dwHSERRequest,LPVOID lpvBuffer,LPDWORD lpdwSize,LPDWORD lpdwDataType )
{
 char bfr[512];
 int i,j;
 char *p;
//  debug("ServerSupport %u\r\n\r\n",dwHSERRequest);
  switch(dwHSERRequest)
  {
   case HSE_REQ_SEND_URL:
   case HSE_REQ_SEND_URL_REDIRECT_RESP:
        send(e->sock,bfr,sprintf(bfr,"HTTP/1.0 301 \r\nLocation: %.460s\r\nContent-type: text/html\r\n\r\n",lpvBuffer),0);
       // e->state|=1;
       // return 1;
   if(0){
   case HSE_REQ_SEND_RESPONSE_HEADER:
//      if(!(e->state&5))
//      {
//        debug("HSE_REQ_SEND_RESPONSE_HEADER state=%X flg=%X %X",e->state,e->sock->fl,lpvBuffer);
//        send(e->sock,bfr,sprintf(bfr,"HTTP/1.0 %.460s\r\n",lpvBuffer?(char *)lpvBuffer:"200"),0);
#if 0
          send(e->sock,"HTTP/1.0 ",sizeof("HTTP/1.0 ")-1,0);
          if(!lpvBuffer)lpvBuffer="200\r\n";
          send(e->sock,(char *)lpvBuffer,i=strlen((char *)lpvBuffer),0);
   //       if(DWORD_PTR(((char *)lpvBuffer)[i-4])!= 0xA0D0A0D x4CHAR("\r\n\r\n") )
          if(!strstr((char *)lpvBuffer,"\r\n\r\n") ) send(e->sock,"\r\n\r\n",(((char *)lpvBuffer)[i-1]=='\n')?2:4,0);
//      }
     // if(lpdwSize)*lpdwSize=send(e->sock,(char *)lpdwDataType,*lpdwSize,0);
#else
          send(e->sock,"HTTP/1.0 ",sizeof("HTTP/1.0 ")-1,0);
          if(!lpvBuffer)lpvBuffer=(void *)"200\r\n";
          send(e->sock,(char *)lpvBuffer,j=i=strlen((char *)lpvBuffer),0);
          p=(char *)lpvBuffer;
          if(lpdwSize)
          {
           p=(char *)lpdwSize;
           send(e->sock,p,i=strlen(p),0);
          }
          if( (!strstr(p,"\r\n\r\n")) &&
              (
               (p== (char *)lpvBuffer) ||
               (
                (!strstr((char *)lpvBuffer,"\r\n\r\n"))
                && (((char *)lpvBuffer)[j-1]!='\n' || *p!='\r'  )
               )
              )
            )
          {
           send(e->sock,"\r\n\r\n",(p[i-1]=='\n')?2:4,0);
          }

#endif
   }
        e->state|=1;
        return 1;
//   if(0){
  // case HSE_REQ_MAP_URL_TO_PATH:
   case 5:
        strncpy(bfr,(char *)lpvBuffer,510);
        if(*lpdwSize < (i=strlen(bfr)+def_dirlen) )return 0;
        //strcpy((char *)lpvBuffer,def_dir);
        //strcat((char *)lpvBuffer,bfr);
        sprintf((char *)lpvBuffer,"%s" FSLUSHS "%s",e->sock->dir,bfr);
        *lpdwSize=i;
//   }
        return 1;
  };
  return 0;
};


EXTENSION_CONTROL_BLOCK  secb=
{
 sizeof(EXTENSION_CONTROL_BLOCK), // DWORD cbSize; // size of this struct.
 0x10000, //                         DWORD dwVersion ; // version info of
 0,       //                         HCONN ConnID; // Context number DO NO
 200,     //                         DWORD dwHttpStatusCode; // HTTP Statu
 {0},     //                         CHAR lpszLogData[HSE_LOG_BUFFER_LEN];
 "GET",     //                       LPSTR lpszMethod; // REQUEST_METHOD
 "",     //                          LPSTR lpszQuery_string; // QUERY_STRI
 "",     //                          LPSTR lpszPathInfo; // PATH_INFO
 "",     //                          LPSTR lpszPathTranslated; // PATH_TRA
 0,      //                          DWORD cbTotalBytes; // Total bytes av
 0,      //                          DWORD cbAvailable; // available numbe
 0,      //                          LPBYTE lpbData; // pointer to client
 "",      //                         LPSTR lpszContentType; // Content dat
 (TGetServerVariable    ) &GetServerVariable, //               BOOL (WINAPI *GetServerVariable)( HCO
 (TWriteClient          ) &WriteClient,       //               BOOL (WINAPI *WriteClient)( HCONN Con
 (TReadClient           ) &ReadClient,       //                BOOL (WINAPI *ReadClient)( HCONN Conn
 (TServerSupportFunction) &ServerSupportFunction, //           BOOL (WINAPI *ServerSupportFunction)(
 0,          //
 0           //
};


int Req::ExecDllEx(char *fnm)
{
  SEHRec  stry;
  char *t;
  EXTENSION_CONTROL_BLOCK  ecb;
  //void *
  HINSTANCE hf;
//  union{
  PFN_HTTPEXTENSIONPROC  fn;
//  PFN_DllRegisterServer  fnc;
//  };
#ifdef SYSUNIX
  char **henv;
#endif

//  LPEXCEPTION_POINTERS   excp;
  memcpy(&ecb,&secb,sizeof(EXTENSION_CONTROL_BLOCK) );
  ecb.ConnID=&ecb;
#define bf (ecb.bf)
//  if(!(bf=new char[0x3000]))return 0;
  bf=rq+9192;
  ecb.cbTotalBytes=MakeEnv(bf+512,bf+0x4000
#ifdef SYSUNIX
  ,&henv
#endif
  );

  if(fl&F_POST)
  {
   ecb.lpszMethod="POST";
   ecb.lpbData=(uchar *)pst;
   ecb.cbAvailable= (pst)? postsize  //strlen(pst)
     :0;
   if((t=GetVar(http_var,"CONTENT_TYPE")))  ecb.lpszContentType=t;
  }
  if(req)ecb.lpszQuery_string=req;

  ecb.lpszPathTranslated=loc;
  ecb.lpszPathInfo=trn?trn:loc;
  ecb.sock=this; //s;
  ecb.hv=http_var;
  ecb.snm=loc;
//  if(fl&F_SKIPHEAD)ecb.state|=5;

#ifndef SYSUNIX
  if(! (hf=GetModuleHandle(fnm) ) )
#else
  setgid(fileStat->st_gid);
  setuid(fileStat->st_uid);
#endif
  {
   GetCurrentDirectory(512,bf);
   if( (t=strrchr(fnm,FSLUSH) ) )
   {*t=0;
    SetCurrentDirectory(fnm);
    *t=FSLUSH;
   }
   hf=LoadLibrary(fnm);
   SetCurrentDirectory(bf);

   if(
#ifndef SYSUNIX
   (uint)(hf) < 32
#else
     !hf
#endif
   )
   {
    dbg(fnm);
    dbg("Can\'t Load...");
     err:
      sprintf(bf,"Error run %s with %s \r\n",loc+dirlen,req);
      HttpReturnError(bf);
//      delete bf;
      return 0;
   }

  #if 0
   if( (fnc= (PFN_DllRegisterServer)
            GetProcAddress(hf,"DllRegisterServer")))
      (fnc)();
  #endif

   if( (fn= (PFN_HTTPEXTENSIONPROC)
            GetProcAddress(hf,"GetExtensionVersion")))
   {
   #if 1
      (fn)( (EXTENSION_CONTROL_BLOCK *)bf );
   #else
      //((LPHSE_VERSION_INFO)bf)->dwExtensionVersion=0x20000;
      //strcpy(((LPHSE_VERSION_INFO)bf)->lpszExtensionDesc,fnm);
      if( !(fn)( (EXTENSION_CONTROL_BLOCK *)bf ) )
      {  debug("ExtensionVersion Failed (%u)", GetLastError());
      }
      else
      debug("ExtensionVersion %u %.8s\r\n",((LPHSE_VERSION_INFO)bf)->dwExtensionVersion,
        ((LPHSE_VERSION_INFO) bf)->lpszExtensionDesc);
   #endif
   };
  }
  if(! (fn= (PFN_HTTPEXTENSIONPROC)GetProcAddress(hf,"HttpExtensionProc"))){ FreeLibrary(hf); dbg("Function not found..."); goto err;};
#ifdef SYSUNIX
  if( setjmp( jmp_env ) ) goto lbErr;
#else  
stry.BegTry( &&lbErr);
#endif
  if(
      (fn)(&ecb)
      >= 4)
    FreeLibrary(hf);
if(0)
{
lbErr:
    except();
    HttpReturnError("Exception from DLL\r\n");
    FreeLibrary(hf);
#if 0
    excp=GetExceptionInformation();
    if( excp->ExceptionRecord->NumberParameters)
      debug("%.64\r\n",excp->ExceptionRecord->ExceptionInformation[0]);
#endif
}
  stry.EndTry();
  if(ecb.lpszLogData[0])dbg(ecb.lpszLogData);

//  delete bf;
  return 1;

#undef bf

};

#undef send
#undef recv
}
