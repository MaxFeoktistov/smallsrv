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

int   SmtpMutex;
ulong SmtpLast;

struct MListCntr ipcnts[10];
#define ipcnt ipcnts[0]
#define netcnt ipcnts[0]
#define totalcnt ipcnts[0].d[0]

int LimitCntr::CheckLimit(ulong t1,ulong lst_tim)
{ulong t,dt;
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

#define X LimitCntr
X *MListCntr::Find(ulong x){X *r,*e;  for(e=(r=d)+n;r<e;++r)if(r->ip==x)return r;  return 0;};
void MListCntr::FreeOld(ulong told){X *r,*e;  for(e=(r=d)+n;r<e;)if(r->first<told){Del(r); --e;} else ++r;};
int Req::OutSMTPLimit(char *bfr,int ll)
{X *r,*e;
 int j,k;
 MListCntr *l;
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
   "</td></tr>"HTML_LN,
    k++,r->ip&0xFF,(r->ip>>8)&0xFF,(r->ip>>16)&0xFF,(r->ip>>24),contry,
    r->cnt&msk ,r->ip,ll ,(s_flg&FL_RADMIN)?"submit":"hidden");
   if(j>0x3000)
   {if(Send(bfr,j)<=0) return -1;
    j=0;
   }
 }
 j+=sprintf(bfr+j,"</table><hr><a name=l%X></a>",ll);
 return Send(bfr,j);
};
#undef X



inline void AddSendMessage(int i){last_file=i;}

int GetCMD(int s,char *b,int timo) //=POPTimeout)
{int x,l,ll;
 int c,ttimo=timo;
 l=0; ll=0;
 if(!ttimo)ttimo=POPTimeout;
 *b=0;
 do{
  if(l>510)break;
  if(ll<=0)
  {if( RESelect(ttimo,0,1,s) <=0)
   {dbg("Timeout");
    return -1;
   };
   ioctlsocket(s,FIONREAD,(ulong *)&ll);
  }
  if(recv(s,b+l,1,0)<=0)
  {dbg("Connection closed");
   return -1;
  }
  --ll;
  b[++l]=0;
 }while( b[l-1] != '\n' );
 if(l<2)l=2;
 DWORD_PTR(b[l-2])=0;
 if(!timo) x=atoui(b);
 else x=DWORD_PTR(*b)|0x20202020;
 if(s_flg&FL_FULLLOG)AddToLog(b,s,(timo)?FmtShort:FmtShortR);
 return x;
};

#define SendCMD(xxb,xxl)  if( send(s,(xxb),xxl,0)<0 ){ dbg("Can\'t send.." );goto ex1;};
#define SendConstCMD(b)   if( ( send(s,(b),sizeof(b)-1,0) ) < 0){ dbg("Can\'t send..");goto ex1;};
#define SendPrint(b,f...)   if( ( send(s,(b),sprintf((b),f),0) ) < 0){ dbg("Can\'t send..");goto ex1;};

int IsUser(char *s,char *b)
{char *t;
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
{char *t;
 int i,j;
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
 DWORD_PTR(t[-1])=0;
 if(s_flg&FL_FULLLOG)AddToLog(b,s,FmtShort);
 return DWORD_PTR(*b)|0x20202020;
}

ulong BadMSGID[17],iBadMSGID;
ulong SpamMSGID[17],iSpamMSGID;
ulong POP3usr[17],iPOP3usr,POP3usrTime;
char *MailVars[12]={"msg",0,"sender",0,"hello",0,"control",0,"checkhello",0,0};

void AddToList(int i,ulong ip,int c=0)
{LimitCntr *spm;
 if(ltime[i])ipcnts[i].FreeOld(time(0)-ltime[i]);
 spm=ipcnts[i].Push();
 spm->ip=ip;
 spm->first=time(0);
 spm->cnt=c;
}


int FileLine::ReadLine()
{int i;
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
 struct sockaddr_in sa;
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
{memcpy(vars,MailVars,sizeof(MailVars));
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
 {if((!list) || (!puser_r) ) return 0;
  sprintf(nm=f.b,"%.128s" FSLUSHS "%.64s",puser_r->dir(),list);
 }
 if(!(nm[0])) return 0;
 if(f.Open(nm)>0)
 {
  while(f.ReadLine()>0)
  {
//   debug("|%s|", f.b);
   t=SkipSpace(f.b);
//   debug("%s", t);

   if( *t=='?')
   { //strncpy(bb+0x7000,spamfltr,0xFF0);

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
           if( DWORD_PTR(*pl[0]) == sa.sin_addr.s_addr  )
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
             th->CheckDNSBL( SkipSpace(t+2), &sa) :
             Reg(t+1)
         )
       )
     {++r;
//      debug("True %u",r);
      break;
     }
   }
   else if( *t!='#' && *t)
   {if(stristr(rr,t) )
    {++r;
//     debug("True2 ??? %u",r);
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
    t //dnsbl
    );
    if(gethostbyname(bfr))
    {
     Send(bfr,sprintf(bfr,"%u %.64s spamers list content your IP\r\n",pass_port,t));
     AddToLog(bfr,s);
     return 1;
    }
    if(!p)break;
    t=SkipSpace(p+1);
  }
  return 0;


}

const char  endmsg[]="\r\n.\r\n";
int Req::SMTPReq()
{
 union{
 char bfr[520];
 int handles[33]; };
 int rcnt,x,i,h,ll,us_ip,chkl=0,chkspm=0,em;

 int l=sizeof(sockaddr_in);
 SYSTEMTIME stime;


 char *rcode;
 uint rcodeLen;
 char *recepter=0;
 int status=0,cmd=0,bdecntr=0;
 ulong mID=0,sID,gl;
 OpenSSLConnection otls;
 char *p,*t,*t1;
 uchar *p2;

 CheckUserList ul;
#define  sa        ul.sa
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
 LimitCntr *lip,*lnet,*spm=0;
#undef SendCMD
#undef SendConstCMD
#undef printSendCMD

#define SendCMD(xxb,xxl) rcode=xxb; rcodeLen=xxl;
#define SendConstCMD(b)  rcode=b; rcodeLen=sizeof(b)-1;
#define printSendCMD(b,fmt,a...)  rcodeLen=sprintf(rcode=b,fmt ,## a);



 getpeername(s,(sockaddr *)&sa,&l);
 us_ip=IsInIPRange(smtp_range);//,htonl(sa_c.sin_addr. S_ADDR));
 if(us_ip<0)goto erSnd;

 if(!us_ip)
 {
   if( (s_flgs[1]&FL1_SMTP_AVT) &&
       memchr4(POP3usr,sa_c.sin_addr. S_ADDR,16) &&
       (GetTickCount()-POP3usrTime)<0x200000
     ) ++us_ip;
 }

 ul.th=this;

 if((spm=ipcnts[1].Find(sa_c.sin_addr. S_ADDR)))
 {if( (! ltime[1] ) || (time(0) - spm->first)<ltime[1] ){spm->cnt++; goto erSnd; }
  ipcnts[1].Del(spm);
  spm=0;
 }


 if(smtp_ltime && ((!us_ip) || !(s_flgs[1]&FL1_NOLUS))  )
 {++chkl;
  if(FndLimit(0,&lip,&lnet,sa_c.sin_addr. S_ADDR) )
  {
 erSnd:
    Send("421 limit overflow\r\n",sizeof("421 limit overflow\r\n")-1);
    AddToLog("421 limit overflow\r\n",s);
    return 1;
  }
 }

 printSendCMD(bfr,"220 %s Simple Mail Transfer Service Ready\r\n",smtp_name);
 hello_msg[0]=0;
 bb=new char[0x8800];
do{
lpcnt:
 if( ( Send(rcode,rcodeLen) ) < 0)
 {dbg("Can\'t send..");
  break;
 };
 if(s_flg&FL_FULLLOG)AddToLog(rcode,s,FmtShrt);
 if(cmd==0x74697571 x4CHAR("quit"))break;
lbb1:
 if((cmd=RGetCMD(bfr))<0)break;
 switch(cmd)
 {case 0x6F6C6865 x4CHAR("ehlo"):
  case 0x6F6C6568 x4CHAR("helo"):
    strncpy(hello_msg,bfr+5,127);
    hello_msg[127]=0;
    status=0;
    printSendCMD(bfr, (cmd==0x6F6C6865 x4CHAR("ehlo"))?
    "250-%.127s\r\n%s"
    "250-8BITMIME\r\n"
    "250 SIZE=%u\r\n":
    "250 %.127s\r\n"
    ,smtp_name,(s_aflg&AFL_TLS)?"250-STARTTLS\r\n":"",max_msg_size);
    break;

  case 0x646E6573 x4CHAR("send")://1:
  case 0x6C69616D x4CHAR("mail")://5:
  case 0x6C6D6F73 x4CHAR("soml")://6:
  case 0x6C6D6173 x4CHAR("saml")://7:
  // Send
    rcnt=0;
    if( (p=stristr(bfr," SIZE=")) && atoui(p+6)>max_msg_size )
    {SendConstCMD("552 Size too much\r\n");
     break;
    }
    if(chkl && //smtp_ltime &&
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
      AddToList(1,sa_c.sin_addr. S_ADDR);
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
           ! ( puser_s || ( puser_s=FindUser(sender,UserSMTP,0,0) ) ) )goto ler450;
     *t='@';
     if(us_ip)status|=2;
    }

    recepter=bb;
    goto ret_ok;
  case 0x74706372 x4CHAR("rcpt")://2:
    if(rcnt>32)
    {SendConstCMD("552 To many...");
     break;
    };
    if(!recepter)recepter=bb;
    if( (!IsUser(bfr,recepter))  || !(t=strchr(recepter,'@'))  ||
       (strpbrk(recepter,":;*,/\\? ") )
      ) //&& (!status) )
    goto ler450;
    if( fake && fake[0] && (!spm) &&
        ((t1=stristr(fake,recepter)) && (t1==fake || t1[-1]<=',' ) )
      ) //!stricmp(recepter,fake) )
    {AddToList(1,sa_c.sin_addr. S_ADDR);
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
             !((us_ip
               )
               && ( (status&2) || (s_flg&FL_SMTPANY) )
              )

     )goto ler450;
    *t='@';
    recepter+=strlen(recepter);
    DWORD_PTR(*recepter)=0x20;
    ++recepter;




    goto ret_ok;
  case 0x61746164 x4CHAR("data")://3:
    pass_port=551;
    if( (!(status&2)) && dnsbl)
    {
      sprintf(bfr,"%.510s",dnsbl);
      if(CheckDNSBL(bfr,&sa)) goto ex1;
    }

    if(recepter && (recepter!=bb) )
    {
     i=GetMsgName(stime);
     sprintf(inf,"%.45s %.45s",sender,bb);

     l=sprintf(rr=bb+0x4005,"From %s (%s [%u.%u.%u.%u]) %3.3s, %u %3.3s %4.4u %2.2u:%2.2u:%2.2u GMT For %s\r\n",
      sender,hello_msg,
#ifndef SYSUNIX
     sa.sin_addr.S_un.S_un_b.s_b1, sa.sin_addr.S_un.S_un_b.s_b2,
     sa.sin_addr.S_un.S_un_b.s_b3, sa.sin_addr.S_un.S_un_b.s_b4,
#else
     sa.sin_addr.s_addr&0xFF, BYTE_PTR(sa.sin_addr.s_addr,1),
     BYTE_PTR(sa.sin_addr.s_addr,2), BYTE_PTR(sa.sin_addr.s_addr,3),
#endif
     wkday[stime.wDayOfWeek],stime.wDay,month[stime.wMonth-1],stime.wYear,
     stime.wHour,stime.wMinute,stime.wSecond,bb);
     sID=MkName(rr,']')+MkName(bb,0);
//debug("+++====%u %X {%X %X %X %X}",iSpamMSGID,sID,SpamMSGID[0],SpamMSGID[1],SpamMSGID[2],SpamMSGID[3]);
   //  x=550;
     p=strchr(sender,'@');
     p2=0;
     gl=0;
     x=time(0);
     if(memchr4(SpamMSGID,sID,16) ||
        (  (!(status&2)) &&
            (
           //   ( (s_flgs[1]&FL1_CHKBACK) && (x=CheckBack(sender)) ) ||
              ( puser_r &&
                 ((!ul.Do(0,glst)) &&
                  (!  ((s_flgs[1]&FL1_CHKUSR) && ul.Do("goodlist")  ) ) &&
                  (  ul.Do(0,blst) ||
                   ( (s_flgs[1]&FL1_CHKUSR)   &&  ul.Do("badlist") ) ||
                   ((
                      (pass_port=451) ,
                      (
                        ( (!(spm=ipcnts[6].Find(sa_c.sin_addr. S_ADDR))  ) ||
                          sID!=spm->cnt  ||
                          ( x - spm->first)<30
                        )  &&
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
                 )))
              ||  ( (!p2) && (s_flgs[2]&FL2_CHKMX) && p && ! GetMailHost(p+1,(d_msg  *)(bb+l+20),1 ) )
              )
            )
        )
//       )
     {
      //if(!x)x=550;
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
        AddToList(6,sa_c.sin_addr. S_ADDR,sID );
       }
       pass_port=452;

      }
      if(pass_port==550)debug("DECLINED: %s",rr);
      printSendCMD(bb+0x7000,
       "%u %.128s\r\n"
       //"%u %.520s\r\n"
      ,pass_port,msg_spam);
      cmd=0x74697571;
      break;
     }

lbSn1:

     Send(("354 send the mail data, end with .\r\n"),
      sizeof("354 send the mail data, end with .\r\n")-1);
     if(s_flg&FL_FULLLOG)AddToLog("354 send the mail data, end with .\r\n",s,FmtShrt);
//     i=GetMsgName(stime);
     sprintf(bb+0x5000,"%.255s" FSLUSHS "%8.8X.msg",out_path,i);
     if( (x=Recv(bb,0x4004) )<=0 )
     {
       SendConstCMD("451 Error\r\n");
       goto ex1;
     }
     postsize=i;
     dirlen=0x29041975;
     if((h=
#ifdef SYSUNIX
      open(bb+0x5000,O_CREAT|O_EXCL|O_WRONLY,secat)
#else
     (int)CreateFile(bb+0x5000,GENERIC_WRITE,0,&secat,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,0)
#endif
     )<=0){SendConstCMD("451 Error\r\n");
     debug("Error. Can't create file %s. Check free disk space",recepter+1);
     goto ex1;
      //break;
     };

   _hwrite(h,rr,l);
   AddToLog(rr,s);
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
     if( ul.Reg(bb+0x7000) )AddToList(1,sa_c.sin_addr. S_ADDR);
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
      cmd=0x74697571 x4CHAR("quit"); //12;
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

//debug("=======%u %X {%X %X %X %X}",iSpamMSGID,sID,SpamMSGID[0],SpamMSGID[1],SpamMSGID[2],SpamMSGID[3]);
         dbg("**Spam detected");
         if( (! (t=stristr(bb,"\nSubject:")
                )
             )   || t>strstr(bb,"\r\n\r\n")  )
         {//t=bb-1; x=sizeof("Subject: [SPAM]\r\n")-1;
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
    //if((x=Recv(bb+l,0x4004-l))<=0)
    //{dbg("Timeout"); break; };

   }while((x=Recv(bb+l,0x4004-l))>0);


   if(l)_hwrite(h,bb,l);
   CloseHandle((FHANDLE)h);
   AddSendMessage(i);
   sprintf(bb,"SMTP: %u user>%s user<%s\r\n",Tin,puser_s?puser_s->name:"*",puser_r?puser_r->name:"*");
   AddToLog(bb,s);
   /*
   if(chkl)// smtp_ltime)
   {i=(Tin+1023)>>10;
    totalcnt.cnt+=i;
    lip->cnt+=i;
    lnet->cnt+=i;
   }
   */
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
 if(Snd==&TLSSend)SecClose(&otls);
 if(chkl)// smtp_ltime)
 {i=(Tin+1023)>>10;
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
