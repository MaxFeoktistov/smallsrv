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
//---


// Get value of server variable from varlist.
// If variable is absent in list return NULL
char * GetVar(char **varlist,char *var);

// Maximum count of HTTP variables. Used to alloc memory
//#define MAX_HTTP_VARS 60

// Pointers to strings with GMT and local time.
char gtime_bbf[30],ltime_bbf[30];
char sprt80[6];
char sprt443[6];

char *SVR[]=
{
  "DATE_GMT",gtime_bbf,"DATE_LOCAL",ltime_bbf,
  "QUERY_STRING",0,"SERVER_PORT",sprt80,
  "REMOTE_ADDR",0,"REMOTE_USER",0,"AUTH_PASS",0,
  "AUTH_TYPE","Basic",0,0,0,0
};

// Prepare list of HTTP variables from request
// This list content all incoming variables and DATE_GMT and DATE_LOCAL
// Name of vareables convert to upercase.
void Req::prepare_HTTP_var()
{
  SYSTEMTIME st;
  register int i;
  int j;
  char *t;
  char c;

  t=rq;
  for(i=0;i<(MAX_HTTP_VARS-22);i+=2)
  {
    do{
      if( !(t=strchr(t,'\r') ) )goto brk1;  //break;
      if(t[2]!='\t' && t[2]!=' ')break;
      ++t;
    }while(1);
    *t=0; t+=2;
    http_var[i]=t;
    for(; (c=*t) != ':'; t++)
    {
      if( (c >= 'a') && (c <= 'z') ) *t&=0xDF;
      else if(c =='-')*t='_';
      else if(!c) goto brk1;
    }
    *t=0;
    do{ ++t;}while(*t==' ');
    http_var[i+1]=t;
  };
  brk1:
  GetSystemTime(&st);
  memcpy(http_var+i,SVR,sizeof(SVR) );
  sprintf(gtime_bbf,"%2.2u/%2.2u/%2.2u %2.2u:%2.2u:%2.2u",st.wDay,st.wMonth,st.wYear,st.wHour,st.wMinute,st.wSecond);
  GetLocalTime(&st);
  sprintf(ltime_bbf,"%2.2u/%2.2u/%2.2u %2.2u:%2.2u:%2.2u",st.wDay,st.wMonth,st.wYear,st.wHour,st.wMinute,st.wSecond);
  http_var[i+5]=req;
  j=16;
  if( !(http_var[i+11]=CheckAuth(http_var[i+13]=rq+8192+256)))
  {
    memset(http_var+i+10,0,8*sizeof(char *));
    j=10;
  }
  else
  {
    if(strin(rq+8192+256,"Digest ") )
    {
      http_var[i+15]="Digest";
    }
  }

  //else {  debug("|%s|%s|",http_var[i+11],http_var[i+13]);}
  #ifndef CD_VER
  if(Snd==&TLSSend)http_var[i+7]=sprt443;
  #endif
  #ifdef USE_IPV6
  IP2S(http_var[i+9]=(char *) (http_var+MAX_HTTP_VARS+2),&sa_c);
  #else
  sprintf(http_var[i+9]=(char *) (http_var+MAX_HTTP_VARS+2),"%u.%u.%u.%u",
          #ifndef SYSUNIX
          sa_c.sin_addr.S_un.S_un_b.s_b1,  sa_c.sin_addr.S_un.S_un_b.s_b2,
          sa_c.sin_addr.S_un.S_un_b.s_b3,  sa_c.sin_addr.S_un.S_un_b.s_b4
          #else
          sa_c.sin_addr.s_addr&0xFF, BYTE_PTR(sa_c.sin_addr.s_addr,1),
          BYTE_PTR(sa_c.sin_addr.s_addr,2),BYTE_PTR(sa_c.sin_addr.s_addr,3)
          #endif
  );
  #endif
  #if (!defined(CD_VER)) && ! defined(VPNCLIENT_ONLY)
  i+=j;
  if(trn)
  {
    http_var[i++]="PATH_INFO";
    http_var[i++]=trn;
  }
  if((s_flgs[1]&FL1_IPCNTR) && country_names)
  {
    http_var[i]="REMOTE_COUNTRY";
    http_var[i+1]=GetCntr(sa_c.sin_addr.s_addr);
  }
  #endif // (!defined(CD_VER)) && ! defined(VPNCLIENT_ONLY)
};

//---
// Get index of server variable from varlist.
// If variable is absent in list return last index.
int GetIndVar(char **varlist,char *v)
{
  int i;
  for(i=0;varlist[i];i+=2)if(!strcmp(varlist[i],v))break;
  return i;
};

//---
//Return count of variables in list
int  CountOfVar(char **varlist)
{
  int i;
  for(i=0;varlist[i];i+=2);
  return i;
};

void Req::prepare_Req_var()
{
  int i;
  req_var=(char**) (void *) &NullLong;

  if(req[0])
  {
    char *t,**tt,*l;
    i=4;
    t=req;
    while(*t)if(*t++=='&')++i;
    #ifdef A_64

    if(!(req_var=(char **) Malloc(i*16+(t-req)*sizeof(char *)+128 ) ) )return;
    *req_var = t = ( (char *)(void *)req_var )+i*16;

    #else
    if(!(req_var=(char **) new char [i*8+(t-req)*sizeof(char *)+64]))return;
    *req_var = t = ( (char *)(void *)req_var )+i*8;
    #endif


    req_var[1] =0;
    for(tt=req_var+2,l=req;*l;l++,t++)
    {
      switch(*l)
      {
        case '&':
          *t=0;
          if(l[1]!='&'){
            if(t[-1]==' ')t[-1]=0;
            *tt=t+1; tt++;
            *tt=0; tt++;
            break;
        case '=':
          tt[-1]=t+1; *t=0;
          break;
        case '+':
          *t=' '; break;
        case '%':
          if( (!*++l) || (!l[1]) ) goto lbExpl;
          *t=atouix2(l++);
          break;
        default:;
          }
          *t=*l;
      };
    };
    lbExpl:
    *t=0;
    *tt=tt[1]=0;
  };
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

int BSend(int s,char *b,int l)
{
  int i;
  char *bb=b;
  #ifndef SPECSEND
  while(l>0)
  {
    if((i=send(s,bb,l,0))<=0) return i;
    l-=i;
    DWORD_PTR(bb)+=i;
  }
  #else
  ioctlsocket(s,FIONBIO,(ulong *) &one);
  while(l>0)
  {
    if(WESelect(120,0,1,s)<=0)break;  //return -1;
    if((i=send(s,bb,l,0))<=0) return i;
    l-=i;
    DWORD_PTR(bb)+=i;
  }
  ioctlsocket(s,FIONBIO,(ulong *) &NullLong);
  #endif

  return bb-b;
}

int JustSnd(Req *th,const void *b,int l)
{
  int r;
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
    }
  }
  return r;
};

int JustRcv(Req *th,void *b,int l)
{
  int r;

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
  return r;
};

//---
// Get value of server variable from varlist.
// If variable is absent in list return NULL
char * GetVar(char **varlist,char *v)
{
  if((!varlist) || !*v)return 0;
  while(*varlist)
  {
    if(!stricmp(*varlist,v))return varlist[1];
    varlist+=2;
  };
  return 0;
};

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
