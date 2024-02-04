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

#ifdef SEPLOG

#undef debug
#undef AddToLog

#define debug(a...)  sepLog[3]->Ldebug(a)
#define AddToLog(a...)  sepLog[3]->LAddToLog(a)

#endif

int   SmtpMutex;
ulong SmtpLast;

struct MListCntr ipcnts[10];
#define ipcnt ipcnts[0]
#define netcnt ipcnts[0]
#define totalcnt ipcnts[0].d[0]

#ifdef USE_IPV6

MListCntrIPv6 ipv6cnts[10];

#define ip6cnt ipv6cnts[0]
#define net6cnt ipv6cnts[0]
#define total6cnt ipv6cnts[0].d[0]

#endif

int LimitBase::CheckLimit(ulong t1,ulong lst_tim)
{
  ulong t,dt;
  t=time(0);
  if((!cnt) || !(first))
  {first=t;
    //return 0;
  }
  else
  {
    dt=t-first;
    if(dt>lst_tim)
    { cnt=MULDIV(cnt,lst_tim,dt);
      first=t-lst_tim;
    }
  }
  if(cnt<t1)return 0;
  return 1;
}

#ifdef WITHOUTTEMPL

#define X LimitCntr
X *MListCntr::Find(ulong x){X *r,*e;  for(e=(r=d)+n;r<e;++r)if(r->ip==x)return r;  return 0;};
void MListCntr::FreeOld(ulong told){X *r,*e;  for(e=(r=d)+n;r<e;)if(r->first<told){Del(r); --e;} else ++r;};

#else

LimitCntr *MListCntr::Find(ulong x){return FindT<ulong>(x);};
void MListCntr::FreeOld(ulong told){return FreeOldT(told);};

LimitCntrIPv6 *MListCntrIPv6::Find(in6_addr &x){return FindT<in6_addr &>(x);};
void MListCntrIPv6::FreeOld(ulong told){return FreeOldT(told);};

#define X LimitCntr
#define X6 LimitCntrIPv6

#endif

int Req::OutSMTPLimit(char *bfr,int ll)
{
  int j,k;
  #ifdef USE_IPV6
  union{
    X *r;
    X6 *r6;
  };
  union{
    X *e;
    X6 *e6;
  };
  union{
    MListCntr *l;
    MListCntrIPv6 *l6;
  };
  #else
  X *r;
  X *e;
  MListCntr *l;
  #endif
  CntrCode  *cc;
  char contry[8];

  int msk=-1;
  if(ll==5)msk=7;
  l=ipcnts+ll;
  j=sprintf(bfr,
            "<h2><a href=\"javascript:SHTbl('t%u');\"><span id=lt%x >[-]</span></a> %s: &nbsp; <a href=#l%X>\\/</a></h2>"
            "<table id=t%u bgcolor=#a0fcff border=1>"
            "<tr bgcolor=#b0cec8><td align=center><font size=3 class=f3><b>"
            "Remote IP</b></font>"
            "</td><td align=center><font size=3 class=f3><b>%s</b></font>"
            "</td><td align=left><font size=2 class=f2> &nbsp;</font>"
            "</td></tr>" HTML_LN,ll,ll,lim_str[ll],ll,ll,l_str[ll]);

  k=1;
  for(e=(r=l->d)+l->n;r<e;++r)
  {
    contry[0]=0;
    if(cntr_dat)
    {
      if( (cc=FindCntr(htonl(r->ip) ) ) )
      {
        sprintf(contry," (%2.2s)",cc->nm);
      }
    }
    j+=sprintf(bfr+j,
               "<tr valign=center><td><font size=2 class=f2>%u) <b>%u.%u.%u.%u%s"
               "</td><td align=center><font size=2 class=f2><b>%u</b></font>"
               "</td><td align=left bgcolor=#ff8040><font size=2 class=f2>"
               "<form method=GET action=/$_admin_$clear>"
               "<input type=hidden name=n value=%u>"
               "<input type=hidden name=s value=%u>"
               "<input type=%s value=Clear>"
               "</form></font>"
               "</td></tr>" HTML_LN,
               k++,r->ip&0xFF,(r->ip>>8)&0xFF,(r->ip>>16)&0xFF,(r->ip>>24),contry,
               r->cnt&msk ,r->ip,ll ,(s_flg&FL_RADMIN)?"submit":"hidden");
    if(j>0x3000)
    { if(Send(bfr,j)<=0) return -1;
      j=0;
    }
  }
  #ifdef USE_IPV6
  char ipv6a[64];
  l6=ipv6cnts+ll;

  for(e6=(r6=l6->d)+l6->n;r6<e6;++r6)
  {
    IPv6S(ipv6a,r6->ip.ip);
    j+=sprintf(bfr+j,
               "<tr valign=center><td><font size=2 class=f2>%u) <b>%.64s"
               "</td><td align=center><font size=2 class=f2><b>%u</b></font>"
               "</td><td align=left bgcolor=#ff8040><font size=2 class=f2>"
               "<form method=GET action=/$_admin_$clear>"
               "<input type=hidden name=n6 value='%s'>"
               "<input type=hidden name=s value=%u>"
               "<input type=%s value=Clear>"
               "</form></font>"
               "</td></tr>" HTML_LN,
               k++,ipv6a,
               r6->cnt&msk ,ipv6a,ll
               ,(s_flg&FL_RADMIN)?"submit":"hidden");
    if(j>0x3000)
    {if(Send(bfr,j)<=0) return -1;
      j=0;
    }
  }
  #endif

  j+=sprintf(bfr+j,"</table><hr><a name=l%X></a>",ll);
  return Send(bfr,j);
};
#undef X

inline void AddSendMessage(int i){last_file=i;}

int GetCMD(int s,char *b,int timo)
{
  int x,l,ll;
  int c,ttimo=timo;
  l=0; ll=0;
  if(!ttimo)ttimo=POPTimeout;
  *b=0;
  do{
    if(l>510)break;
    if(ll<=0)
    {if( RESelect(ttimo,0,1,s) <=0)
      {dbgs("Timeout");
        return -1;
      };
      ioctlsocket(s,FIONREAD,(ulong *)&ll);
    }
    if(recv(s,b+l,1,0)<=0)
    {dbgs("Connection closed");
      return -1;
    }
    --ll;
    b[++l]=0;
  }while( b[l-1] != '\n' );
  if(l<2)l=2;
  DWORD_PTR(b[l-2])=0;
  if(!timo) x=atoui(b);
  else x=DWORD_PTR(*b)|0x20202020;
  if(s_flg&FL_FULLLOG)AddToLog(b,s,0,(timo)?FmtShort:FmtShortR);
  return x;
};

#define SendCMD(xxb,xxl)  if( send(s,(xxb),xxl,0)<0 ){ dbg("Can\'t send.." );goto ex1;};
#define SendConstCMD(b)   if( ( send(s,(b),sizeof(b)-1,0) ) < 0){ dbg("Can\'t send..");goto ex1;};
#define SendPrint(b,f...)   if( ( send(s,(b),sprintf((b),f),0) ) < 0){ dbg("Can\'t send..");goto ex1;};

int IsUser(char *s,char *b)
{
  char *t;
  if(!(s=strchr(s,'<')))return 0;
  s++;
  if(!(t=strchr(s,'>')))return 0;
  *t=0;
  strncpy(b,s,127);
  b[127]=0;
  return 1;
};
//---
#ifdef SYSUNIX
#include <sys/time.h>
#endif

ulong GetMsgName(SYSTEMTIME  &stime)
{ulong i;
#ifdef SYSUNIX
 struct timeval tv;
 gettimeofday(&tv,0);
#endif

  MyLock(SmtpMutex);
  Sleep(40);
  GetSystemTime(&stime);
  i=
   (( ( ( ( ( ( (stime.wYear-2001)*12+stime.wMonth
              )*31+stime.wDay
            )*24
          )+stime.wHour
        )*60+stime.wMinute
      )*60+stime.wSecond
    )<<4
#ifndef SYSUNIX
   )+(stime.wMilliseconds/63);
#else
   )+(tv.tv_usec/15873);
#endif

  if(i<=SmtpLast)i=SmtpLast+1;
  SmtpLast=i;
  MyUnlock(SmtpMutex);
  return i;
}
//---
int IsUsHost(char *t)
{if( !stricmp(++t,smtp_name) )return 1;
 if(FL_VHALIAS&s_flg)for(host_dir *a=hsdr.next;a;a=a->next)if(!stricmp(a->h,t))return 1;
 return 0;
}
//---
#undef send
#undef recv
#define recv(a,b,c,d) Recv(b,c)

int Req::RGetCMD(char *b)
{
  char *t;
  int i,j;
  #ifdef  SEPLOG
  uint k=flsrv[1] & MAX_SERV_MASK;
  if(k>=10)k=3;

  #undef debug
  #undef AddToLog

  #define debug(a...)  sepLog[k]->Ldebug(a)
  #define AddToLog(a...)  sepLog[k]->LAddToLog(a)

  #endif
  i=0;
  do{
    if((j=Recv(b+i,512-i))<=0)
    {dbg("Timeout");
      return -1;
    }
    if((i+=j)>=510)
    {debug("DETECTED HACKER");
      return -1;
    }
    b[i]=0;
  }while( !(t=strchr(b,'\n')) );
  if(b[i-1]=='\n')b[i-2]=0;
  if(s_flg&FL_FULLLOG)
  {
    AddToLog(b,s,&sa_c46, (fl&F_SMTP_CL) ? FmtShort:FmtShortR);
  }
  if (fl&F_SMTP_CL) return atoui(b);
  DWORD_PTR(t[-1])=0;
  return (DWORD_PTR(*b)|0x20202020);
}
#ifdef SEPLOG

#undef debug
#undef AddToLog

#define debug(a...)  sepLog[3]->Ldebug(a)
#define AddToLog(a...)  sepLog[3]->LAddToLog(a)

#endif

u32 BadMSGID[17],iBadMSGID;
u32 SpamMSGID[17],iSpamMSGID;
u32 POP3usr[17],iPOP3usr,POP3usrTime;
char *MailVars[12]={"msg",0,"sender",0,"hello",0,"control",0,"checkhello",0,0};

LimitBase * AddToList(int i,sockaddr_in *sa,int c)
{
  time_t tmt=time(0);

  #ifdef USE_IPV6
  union{
    LimitCntr *spm;
    LimitCntrIPv6 *spm6;
  };
  if(IsIPv6(sa))
  {
    if(ltime[i])ipv6cnts[i].FreeOld(tmt-ltime[i]);
    spm6=ipv6cnts[i].Push();
    spm6->Set( ((sockaddr_in6 *)sa)->sin6_addr );
    spm6->first=tmt;
    spm6->cnt=c;
    return spm6;
  }
  #else
  LimitCntr *spm;
  #endif

  if(ltime[i])ipcnts[i].FreeOld(tmt-ltime[i]);
  spm=ipcnts[i].Push();
  spm->ip= IPv4addr(sa);
  spm->first=tmt;
  spm->cnt=c;
  return spm;
}


int FileLine::ReadLine()
{
  int i;
  if(l>0)
  {memcpy(b,p,l);
    b[l]=0;
    if((p=strpbrk(b,"\r\n")) )
    {
      lb1:
      do{*p++=0; }while(*p=='\n');
      l-=p-b;
      if(l<0)l=0;
      return 1;
    }
  }
  if((i=_hread(h,b+l,LINESIZE-l))<=0)return i;
  l+=i;
  b[l]=0;
  if((p=strpbrk(b,"\r\n")) ) goto lb1;
  l=0;
  p=b;
  return 2;
}

struct CheckUserList{
  union{
    struct sockaddr_in *psa;
    struct sockaddr_in6 *psa6;
  };
  Req  *th;
  RegVars rv;
  char *bb,*rr,*p,*t;
  char *vars[12];
  char hello_msg[132];
  char sender[132];
  char checkhello[4];
  User *puser_s;
  User *puser_r;

  int  Do(char *list,char *nm=0);
  int  Reg(char *x);
};

int CheckUserList::Reg(char *x)
{
  memcpy(vars,MailVars,sizeof(MailVars));
  vars[1]=bb; vars[3]=sender; vars[5]=hello_msg;
  vars[7]=rr; vars[9]=checkhello;
  rv.Init(vars,0,0);
  return rv.LogAn(x);
}

int CheckUserList::Do(char *list,char *nm)
{
  FileLine f;
  char *t;
  int r=0;

  if(!nm)
  {
    if((!list) || (!puser_r) ) return 0;
    sprintf(nm=f.b,"%.128s" FSLUSHS "%.64s",puser_r->dir(),list);
  }
  if(!(nm[0])) return 0;
  if(f.Open(nm)>0)
  {
    while(f.ReadLine()>0)
    {
      t=SkipSpace(f.b);
      if( *t=='?')
      {
        if(strstr(t,"$checkhello") && ! (checkhello[0]) )
        {
          checkhello[0]=0x30;
          if(anychars(hello_msg) && strchr(hello_msg,'.') && (! strpbrk(hello_msg,"[](){}@#$~!^%=")) && strlen(hello_msg)<64  )
          {
            struct hostent  *hp;
            if((hp=gethostbyname(hello_msg)))
            {
              char **pl;
              for(pl=hp->h_addr_list; pl && *pl ; pl++)
              {
                if( DWORD_PTR(*pl[0]) == psa->sin_addr.s_addr  )
                {
                  checkhello[0]=0x31;
                  break;
                }
              }
            }
          }
        }
        if(
          ( (t[1]=='?')?
          th->CheckDNSBL( SkipSpace(t+2), psa) :
          Reg(t+1)
          )
        )
        {++r;
          DBGLA("True %u",r);
          break;
        }
      }
      else if( *t!='#' && *t)
      {
        if(stristr(rr,t) )
        {
          ++r;
          DBGLA("True2 ??? %u",r);
          break;
        }
      }
    }
    f.Close();

  }
  return r;
}

int Req::CheckDNSBL(char *t,struct sockaddr_in *sa)
{
  char bfr[128];
  char *p;
  while(t && *t)
  {
   if((p=strpbrk(t," ,)\t\r\n") ) )*p=0;
#ifdef USE_IPV6
   if( IsIPv6(sa) )
   {
     sprintf(bfr+IPv6S(bfr,((sockaddr_in6 *)sa)->sin6_addr),".%64s",t);
   }
   else
   {
     uint tt=IPv4addr(sa);
     sprintf(bfr,"%u.%u.%u.%u.%.64s",
     BYTE_PTR(tt,3),
     BYTE_PTR(tt,2),
     BYTE_PTR(tt,1),
     tt&0xFF,
     t
    );
   }
#else

     sprintf(bfr,"%u.%u.%u.%u.%.64s",
#ifndef SYSUNIX
     sa->sin_addr.S_un.S_un_b.s_b4,
     sa->sin_addr.S_un.S_un_b.s_b3,
     sa->sin_addr.S_un.S_un_b.s_b2,
     sa->sin_addr.S_un.S_un_b.s_b1,
#else
     BYTE_PTR(sa->sin_addr.s_addr,3),
     BYTE_PTR(sa->sin_addr.s_addr,2),
     BYTE_PTR(sa->sin_addr.s_addr,1),
     sa->sin_addr.s_addr&0xFF,
#endif
    t
    );
#endif
    if(gethostbyname(bfr))
    {
     Send(bfr,sprintf(bfr,"%u %.64s spamers list content your IP\r\n",pass_port,t));
     AddToLog(bfr,s,&sa_c46);
     return 1;
    }
    if(!p)break;
    t=SkipSpace(p+1);
  }
  return 0;
}

ulong AddrHash(sockaddr_in *sa)
{
  if(sa->sin_family == AF_INET) return sa->sin_addr. S_ADDR;
  if(sa->sin_family == AF_INET6){
    if(
      (( sockaddr_in6 *)sa)->sin6_addr.s6_addr32[0]==0 &&
      (( sockaddr_in6 *)sa)->sin6_addr.s6_addr32[1]==0 &&
      (
        (( sockaddr_in6 *)sa)->sin6_addr.s6_addr32[2]==0xFFFF0000
      )
    )  return (( sockaddr_in6 *)sa)->sin6_addr.s6_addr32[3];
    return (( sockaddr_in6 *)sa)->sin6_addr.s6_addr32[0]^(( sockaddr_in6 *)sa)->sin6_addr.s6_addr32[1]^(( sockaddr_in6 *)sa)->sin6_addr.s6_addr32[2]^(( sockaddr_in6 *)sa)->sin6_addr.s6_addr32[3];
  }
  return 0;
};

const char  endmsg[]="\r\n.\r\n";
int Req::SMTPReq()
{
  union {
    char bfr[520];
    int handles[33];
  };

  User *puser_a = 0;
  int rcnt,x,i,h,ll,us_ip,chkl=0,chkspm=0,em;

  int l=sizeof(sockaddr_in);
  SYSTEMTIME stime;


  char *rcode;
  uint rcodeLen;
  char *recepter=0;
  int status=0,cmd=0,bdecntr=0;
  u32 mID=0,sID,gl;
  OpenSSLConnection otls;
  char *p,*t,*t1;
  uchar *p2;

  CheckUserList ul;
  #define  rv        ul.rv
  #define  puser_s   ul.puser_s
  #define  puser_r   ul.puser_r
  #define  bb        ul.bb
  #define  rr        ul.rr
  #define  hello_msg ul.hello_msg
  #define  sender    ul.sender

  bb=0;
  puser_s=0;
  puser_r=0;

  timout=POPTimeout;
  LimitBase *lip,*lnet;
  union {
    LimitCntr  *spm;
    LimitCntrIPv6  *spm6;
  };
  spm=0;
  #undef SendCMD
  #undef SendConstCMD
  #undef printSendCMD

  #define SendCMD(xxb,xxl) rcode=xxb; rcodeLen=xxl;
  #define SendConstCMD(b)  rcode=b; rcodeLen=sizeof(b)-1;
  #define printSendCMD(b,fmt,a...)  rcodeLen=sprintf(rcode=b,fmt ,## a);



  ul.psa = &sa_c;
  us_ip = IsInIPRange(smtp_range);
  if(us_ip<0)goto erSnd;

  if(!us_ip)
  {
    gl = AddrHash(&sa_c);
    if( (s_flgs[1]&FL1_SMTP_AVT) &&
         gl &&
         memchr4(POP3usr,gl,16) &&
         (GetTickCount()-POP3usrTime)<0x200000
    ) ++us_ip;
    /*
     *   else {
     *     debug("dt:%X %u %X %X POP3usr{%X %X} {%X %X}\n",GetTickCount()-POP3usrTime,iPOP3usr, sa_c.sin_addr.S_ADDR, gl,
     *           POP3usr[0],POP3usr[1],
     *           POP3usr[(iPOP3usr-1)&0xF],
     *           POP3usr[(iPOP3usr-2)&0xF]  );
               }
     */
  }

  ul.th=this;
#ifdef USE_IPV6
  if(IsIPv6(&sa_c))
  {

    if((spm6=ipv6cnts[1].Find(sa_c6.sin6_addr)))
    {
      if( (! ltime[1] ) || (time(0) - spm6->first)<ltime[1] ){spm6->cnt++; goto erSnd; }
      ipv6cnts[1].Del(spm6);
      spm=0;
    }

  }
  else
#endif
    if((spm=ipcnts[1].Find( IPv4addr(&sa_c) )) )
    {
      if( (! ltime[1] ) || (time(0) - spm->first)<ltime[1] ){spm->cnt++; goto erSnd; }
      ipcnts[1].Del(spm);
      spm=0;
    }

    if(smtp_ltime && ((!us_ip) || !(s_flgs[1]&FL1_NOLUS))  )
    {
      ++chkl;
      if(FndLimit(0,&lip,&lnet, &sa_c ) )
      {
        erSnd:
        Send("421 limit overflow\r\n",sizeof("421 limit overflow\r\n")-1);
        AddToLog("421 limit overflow\r\n",s,&sa_c46);
        return 1;
      }
    }

    printSendCMD(bfr,"220 %s Simple Mail Transfer Service Ready\r\n",smtp_name);
    hello_msg[0]=0;
    bb=new char[0x8800];
    do{
      lpcnt:
      if( ( Send(rcode,rcodeLen) ) < 0)
      {
        lbErrSend:
        dbg("Can\'t send..");
        break;
      };
      if(s_flg&FL_FULLLOG)AddToLog(rcode,s,&sa_c46,FmtShrt);
      if(cmd==0x74697571 x4CHAR("quit"))break;
      lbb1:
      if((cmd=RGetCMD(bfr))<0)break;
      switch(cmd)
      {
        case 0x6F6C6865 x4CHAR("ehlo"):
        case 0x6F6C6568 x4CHAR("helo"):
          strncpy(hello_msg,bfr+5,127);
          hello_msg[127]=0;
          status=0;
          printSendCMD( bfr, (cmd==0x6F6C6865 x4CHAR("ehlo"))?
                        "250-%.127s\r\n%s"
                        "250-8BITMIME\r\n"
                        "250-SIZE=%u\r\n"
                        "250 AUTH CRAM-MD5 PLAIN\r\n"
                        :
                        "250 %.127s\r\n"
                        ,smtp_name,(s_aflg&AFL_TLS)?"250-STARTTLS\r\n":"",max_msg_size);
          break;

        case 0x68747561 x4CHAR("auth"):

          if( (p=stristr(bfr," PLAIN") ) )
          {
            x = 0;
            DBGLA("%s %s\r\n", p, bfr)
            if(p[6] == ' ' && p[7] >= 'A')
            {
              p+=7;
              DBGLA("p=%s\r\n", p)
              goto lb_chk;
            }
            SendConstCMD("334 \r\n");
          }
          else if( stristr(bfr," CRAM-MD5") )
          {
            struct timeval tv;
            gettimeofday(&tv,0);
            p=Encode64(bb+512,bb, postsize = sprintf(pst = bb, "<%u.%u@%s>", tv.tv_sec, tv.tv_usec, smtp_name));
            printSendCMD(bfr,"334 %s\r\n", bb+512);
            x = FindUserMD5cram;
          }
          else
          {
            SendConstCMD("534 5.7.9  Unsupported algorithm\r\n");
            break;
          }
          if( ( Send(rcode, rcodeLen) ) < 0) goto lbErrSend;
          if(s_flg&FL_FULLLOG)AddToLog(rcode, s, &sa_c46, FmtShrt);
          if( RGetCMD(bfr) < 0 ) break;
          p=bfr;

          lb_chk:;

          DBGLA("p=|%s|\r\n", p)

          if( (t1=Decode64(t=bb+1024, p, 128)) )
          {
            if( (!*t) && t<t1 ) t++;
            DBGLA("|%s| %s %X\r\n", t, p, x)
            p=strchr(t, ' ');
            if(p) *p++ = 0;
            else if( (p = t+1+strlen(t) ) >= t1  ) goto lbAuthInvalid;

            puser_a = FindUser(t, UserSMTP | x, p, this);
            if( puser_a )
            {
              SendConstCMD("235 2.7.0  Authentication Succeeded\r\n");
              ++us_ip;
              break;
            }
          }
          lbAuthInvalid:
          SendConstCMD("535 5.7.8  Authentication credentials invalid\r\n");

          break;

          case 0x646E6573 x4CHAR("send"):
          case 0x6C69616D x4CHAR("mail"):
          case 0x6C6D6F73 x4CHAR("soml"):
          case 0x6C6D6173 x4CHAR("saml"):
            // Send
            rcnt=0;
            if( (p=stristr(bfr," SIZE=")) && atoui(p+6)>max_msg_size )
            {SendConstCMD("552 Size too much\r\n");
              break;
            }
            if(chkl &&
              ((totalcnt.cnt >smtp_limit) ||
              (lip->cnt>smtp_ip_limit  ) ||
              (lnet->cnt>smtp_net_limit)
              )
            )
            {SendConstCMD("552 To many...");
              cmd=0x74697571;
              break;
            }
            if((!IsUser(bfr,sender)) ||
              (
                (!(t=strchr(sender,'@'))) &&
                (! (s_flgs[1]&FL1_NOCHKBCK)) &&
                *sender
              ) ||
              ( (p=stristr(blacklist,sender)) && strchr(" \t,;=",p[-1]) )||
              ( t && (p=stristr(blacklist,t)) && p[-1]=='*' ) ||
              (strpbrk(sender,";*,\\? ") )
            )
            {
              // Avilable recive mail to resend only from user.
              ler450:
              if(recepter)*recepter=0;
              SendConstCMD("550 Unknown user\r\n");
              if(++bdecntr>=2)
              {debug("Spamer detected.");
                AddToList(1,&sa_c);
                //      AddToList(1,sa_c.sin_addr. S_ADDR);
                cmd=0x74697571;
              }
              break;
            }

            if(!t)status|=8;
            else if(IsUsHost(t))
            {
              puser_s=FindUser(sender,UserSMTP,0,0);
              *t=0;
              if( ( ! ( (s_flgs[2]& FL2_MLNOIP) || us_ip)) ||
                ! ( puser_s || ( puser_s=FindUser(sender,UserSMTP,0,0) ) ) )
              {
                // debug("us_ip(%u) puser_s:%lX\n", us_ip, (long) puser_s );
                goto ler450;
              }
              *t='@';
              if(us_ip)status|=2;
            }

            recepter=bb;
            goto ret_ok;
            case 0x74706372 x4CHAR("rcpt"):
              if(rcnt>32)
              { SendConstCMD("552 To many...");
                break;
              };
              if(!recepter)recepter=bb;
              if( (!IsUser(bfr,recepter))  || !(t=strchr(recepter,'@'))  ||
                  (strpbrk(recepter,":;*,/\\? ") )
                )  goto ler450;
              if( fake && fake[0] && (!spm) &&
                  ((t1=stristr(fake,recepter)) && (t1==fake || t1[-1]<=',' ) )
                )
              {
                AddToList(1,&sa_c);
                cmd=0x74697571;
                break;
              }

              if(IsUsHost(t))
              {
                puser_r=FindUser(recepter,UserPOP3,0,0);
                *t=0;
                if( (!puser_r)  && !(puser_r=FindUser(recepter,UserPOP3,0,0))  )goto ler450;
              }
              else if( ((status&8) && ! us_ip ) ||
                       !( (us_ip)
                          && ( (status&2) || (s_flg&FL_SMTPANY) )
                        )
                     ) goto ler450;
              *t='@';
              recepter+=strlen(recepter);
              DWORD_PTR(*recepter)=0x20;
              ++recepter;
              goto ret_ok;
            case 0x61746164 x4CHAR("data"):
              pass_port=551;
              if( (!(status&2)) && dnsbl)
              {
                sprintf(bfr,"%.510s",dnsbl);
                if(CheckDNSBL(bfr,&sa_c)) goto ex1;
              }

              if(recepter && (recepter!=bb) )
              {
                i=GetMsgName(stime);
                sprintf(inf,"%.45s %.45s",sender,bb);

                l=sprintf(rr=bb+0x4005,"From %s (%s [%u.%u.%u.%u]) %3.3s, %u %3.3s %4.4u %2.2u:%2.2u:%2.2u GMT For %s\r\n",
                          sender,hello_msg,
                          #ifndef SYSUNIX
                          sa_c.sin_addr.S_un.S_un_b.s_b1, sa_c.sin_addr.S_un.S_un_b.s_b2,
                          sa_c.sin_addr.S_un.S_un_b.s_b3, sa_c.sin_addr.S_un.S_un_b.s_b4,
                          #else
                          sa_c.sin_addr.s_addr&0xFF, BYTE_PTR(sa_c.sin_addr.s_addr,1),
                          BYTE_PTR(sa_c.sin_addr.s_addr,2), BYTE_PTR(sa_c.sin_addr.s_addr,3),
                          #endif
                          wkday[stime.wDayOfWeek],stime.wDay,month[stime.wMonth-1],stime.wYear,
                          stime.wHour,stime.wMinute,stime.wSecond,bb);
                sID=MkName(rr,']')+MkName(bb,0);
                p=strchr(sender,'@');
                p2=0;
                gl=0;
                x=time(0);
                if( memchr4(SpamMSGID,sID,16) ||
                    ( (!(status&2)) &&
                      (
                        ( puser_r &&
                          ( (!ul.Do(0,glst)) &&
                            (!  ((s_flgs[1]&FL1_CHKUSR) && ul.Do("goodlist")  ) ) &&
                            (  ul.Do(0,blst) ||
                              ( (s_flgs[1]&FL1_CHKUSR)   &&  ul.Do("badlist") ) ||
                              (
                                (
                                  (pass_port=451) ,
                                  (
                                    ( (!(spm=ipcnts[6].Find(sa_c.sin_addr. S_ADDR))  ) ||
                                      sID!=spm->cnt  ||
                                      ( x - spm->first)<30 )  &&
                                      (
                                        (gl=( ul.Do(0,graylst) || ( (s_flgs[1]&FL1_CHKUSR)   &&  ul.Do("graylist") ) )   )
                                      )
                                  )
                                )
                                &&
                                !(
                                    (s_flgs[2]&FL2_MX_GLIST) && p &&
                                    (p2=(uchar *)GetMailHost(p+1,(d_msg  *)(bb+l+20),1 )) &&
                                    CheckMHIP(p2,sa_c.sin_addr. S_ADDR)
                                  )
                              )
                            )
                          )
                        )
                        ||  ( (!p2) && (s_flgs[2]&FL2_CHKMX) && p && ! GetMailHost(p+1,(d_msg  *)(bb+l+20),1 ) )
                      )
                    )
                  )
                {
                  if(gl)
                  {
                    if(spm)
                    {
                      if(sID!=spm->cnt)
                        spm->cnt=sID;
                      else if(  (x - spm->first)>30  )
                      {
                        ipcnts[6].Del(spm);
                        spm=0;
                        goto lbSn1;
                      }
                      else
                      {
                        spm->first=x-5;
                      }
                    }
                    else
                    {
                      ltime[6]=172800;
                      AddToList(6,&sa_c,sID);
                    }
                    pass_port=452;
                  }
                  if(pass_port==550)debug("DECLINED: %s",rr);
                  printSendCMD(bb+0x7000,
                               "%u %.128s\r\n"
                               ,pass_port,msg_spam);
                  cmd=0x74697571;
                  break;
                }
                lbSn1:
                Send(("354 send the mail data, end with .\r\n"),
                     sizeof("354 send the mail data, end with .\r\n")-1);
                if(s_flg&FL_FULLLOG)AddToLog("354 send the mail data, end with .\r\n",s,&sa_c46,FmtShrt);
                sprintf(bb+0x5000,"%.255s" FSLUSHS "%8.8X.msg",out_path,i);
                if( (x=Recv(bb,0x4004) )<=0 )
                {
                  SendConstCMD("451 Error\r\n");
                  goto ex1;
                }
                postsize=i;
                dirlen=dlNOW_SENDING_SIGN;
                if( (h=
                  #ifdef SYSUNIX
                      open(bb+0x5000,O_CREAT|O_EXCL|O_WRONLY|O_CLOEXEC,secat)
                  #else
                      (int)CreateFile(bb+0x5000,GENERIC_WRITE,0,&secat,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,0)
                  #endif
                  )<=0 )
                {
                  SendConstCMD("451 Error\r\n");
                  debug("Error. Can't create file %s. Check free disk space",recepter+1);
                  goto ex1;
                };
                _hwrite(h,rr,l);
                AddToLog(rr,s,&sa_c46);
                Tin=x;
                l=0;
                em=0;
                do{
                  bb[l+=x]=0;
                  t=strstr(bb,endmsg);
                  if(em && ! strcmp(bb,endmsg+em)  )t=bb;
                  em=0;

                  if((t || l>0x4000) && spamfltr && !chkspm)
                  {++chkspm;
                    strncpy(bb+0x7000,spamfltr,0xFF0);
                    if( ul.Reg(bb+0x7000) )
                      AddToList(1,&sa_c);
                  }

                  if(t)l=t+2-bb;
                  else
                  {if(Tin>max_msg_size && !(s_flg&FL_NOBRKSMTP) )
                    {BadMSGID[iBadMSGID]=mID;
                      iBadMSGID=(++iBadMSGID)&0xF;
                      lSizeToMutch:
                      if(l)_hwrite(h,bb,l);
                      _lclose(h);
                      SendConstCMD("552 Size too much\r\n");
                      cmd=0x74697571 x4CHAR("quit");
                      AddSendMessage(i);
                      goto lpcnt;
                    }
                    if(l>0x2008)
                    {if(!mID)
                      {x=bb[128];
                        bb[128]=0;
                        mID=MkName(bb,0);
                        bb[128]=x;
                        if(memchr4(BadMSGID,mID,16) )
                        {  goto lSizeToMutch; }
                        if(antispam)
                        {
                          strncpy(bb+0x7000,antispam,0xFF0);
                          if( ul.Reg(bb+0x7000) )
                          {SpamMSGID[iSpamMSGID]=sID;
                            iSpamMSGID=(++iSpamMSGID)&0xF;
                            DBGLA("=======%u %X {%X %X %X %X}",iSpamMSGID,sID,SpamMSGID[0],SpamMSGID[1],SpamMSGID[2],SpamMSGID[3]);
                            dbg("**Spam detected");
                            if( (! (t=stristr(bb,"\nSubject:")
                                   )
                                ) || t>strstr(bb,"\r\n\r\n")
                              )
                            {
                              _hwrite(h,"Subject: [SPAM]\r\n",sizeof("Subject: [SPAM]\r\n")-1);
                            }
                            else
                            {
                              ++t;
                              memcpy_back(bb+l+sizeof(" [SPAM]")-1,bb+l,l-(t-bb) );
                              memcpy(t,"Subject: [SPAM]",sizeof("Subject: [SPAM]")-1);
                              l+=sizeof(" [SPAM]")-1;
                            }
                            l+=sprintf(bb+l,"\r\n\r\n\t\t[Restricted by server]\r\n");
                            cmd=0x74697571;
                            break;
                          }
                        }
                      }
                      if( _hwrite(h,bb,l)<l )
                      {
                        debug("Error. Write error. Check free disk space");
                        CloseHandle((FHANDLE)h);
                        Send("451 Error\r\n",sizeof("451 Error\r\n")-1 );
                        goto ex1;
                      } ;
                      for(em=4;em>0;em--)if(em<l && strin((char *)endmsg,bb+l-em) )break;
                      l=0;
                    }
                  }
                  if(t)break;

                }while((x=Recv(bb+l,0x4004-l))>0);

                if(l)_hwrite(h,bb,l);
                CloseHandle((FHANDLE)h);
                AddSendMessage(i);
                sprintf(bb,"SMTP: %u user>%s user<%s\r\n",Tin,puser_s?puser_s->name:"*",puser_r?puser_r->name:"*");
                AddToLog(bb,s,&sa_c46);

                case 0x74657372 x4CHAR("rset")://4:
                  reset:
                  rcnt=0;
                  recepter=0;
                  status&=1;
                  ret_ok:
                  SendConstCMD("250 Ok\r\n");
                  break;
              };
              SendConstCMD( "503 Error\r\n");
              break;
                case 0x74697571 x4CHAR("quit")://12:
                  SendConstCMD( "221 Bye\r\n");
                  break;
                case 0x72617473 x4CHAR("star"): //STARTTLS
                  if( (DWORD_PTR(bfr[4])|0x20202020)==0x736C7474 x4CHAR("ttls") && (s_aflg&AFL_TLS))
                  {Send("220 Ok\r\n",sizeof("220 Ok\r\n")-1);
                    debug("**%snit TLS connection",TLSBegin(&otls)?"I":"Error to i");
                    goto lbb1;
                  }
                default:
                  SendConstCMD("502 Command not implemented\r\n");
      };
    }while(1);
    ex1:
    Close();
    if(chkl)
    {
      i=(Tin+1023)>>10;
      totalcnt.cnt+=i;
      lip->cnt+=i;
      lnet->cnt+=i;
    }
    if(bb)delete bb;
    return 1;
};

#undef sa
#undef rv
#undef puser_s
#undef puser_r
#undef bb
#undef rr
#undef hello_msg
#undef sender
#undef SendPrint


struct PopName{
 ulong name,size;
};



#include "pop3d.cpp"
#undef send
#undef recv
