/*
 * Copyright (C) 1999-2021 Maksim Feoktistov.
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



const char  ReseivF[]= "Received: f";
#undef SendConstCMD
#undef SendCMD
#undef send
#define send(a,b,c,d) BSend(a,b,c)
int SendCMDf(int s,char *xxx,int lll)
{if(s_flg&FL_FULLLOG)AddToLog(xxx,s,0,FmtShrtR); return send(s,(xxx),(lll),0);}
#define SendCMD(xxx,lll)  if(SendCMDf(s,(xxx),(lll))<=0 )goto brkConn;
#define SendConstCMD(xxx)   SendCMD(xxx,(sizeof(xxx)-1) )

#ifndef SYSUNIX
STARTUPINFO cbFwd;
#else
#include<pwd.h>
#endif

int time_btw=3;

struct MLChk{
 char *b,*t,*t1,*t2,*t3,*t4,*t5,*t6,*t7,*t8,*t9;
 char pth2[600];
 char pth3[800];
 int l,n;
 ulong rez;
 SYSTEMTIME stime;
#ifdef SYSUNIX
 struct passwd *pwd;
#else
 PROCESS_INFORMATION piFwd;
#endif
 int RunForward(char *mbx);
 void SndErrMsg(char *msg,char *sbj="Error send your message");
};
#ifndef SYSUNIX

void FndRedir(char **t,char *n,int x)
{
 if( (!(t[0]=strrchr(n,x))) ||
     (
      (t[1]=strpbrk(t[0],"\'\" <>\r\n"))
      && (*t[1]=='\'' || *t[1]=='\"')
     )
   )
 {t[0]=0; t[1]=0;}
}
#endif
char *FwdVars[12]=
{"size_kb",0,"errorlevel",0,"text",0,"msgfile",0,"body",0,0,0};
int MLChk::RunForward(char *mbx)
{int h,i,s,x,r;
 r=0;
 rez=0;
 char *vars[12];
 char vsz[12];
 char verr[12];
 char  *nm[8];
 RegVars rv;

 if( (h=_lopen(pth2,0))>0 )
 {
  debug("Forward file %s",pth2);
  memcpy(vars,FwdVars,sizeof(FwdVars));
  rv.Init(vars,0,0);

  sprintf(vsz,"%u",(l+1023)/1024);

  dprint("vsz=%s\n",vsz);

  vars[1]=vsz;
  vars[3]=verr;
  vars[5]=t1;
  if(!(vars[9]=strstr(t1,"\r\n\r")))vars[4]=0; else vars[9]+=3;
  DWORD_PTR(verr[0])='0';

  x=FileSize(h);
  if((t5=new char[x+700]))
  {_hread(h,t9=t5,x);
   _lclose(h);
   sprintf(vars[7]=t5+x+8,"%.500s%s" FSLUSHS "%8.8X.msg",t8,mbx,n);
   while(t5 && *t5)
   {do{
     if( (t4=strchr(t5,'\n'))){
      if( WORD_PTR(t4[-2])==0x5C20 && (WORD_PTR(t4[-3])==0x5C20 && t4[-1]=='\r' ))
      {if(t4[-2]=='\\')t4[-2]=' '; WORD_PTR(t4[-1])=0x2020; continue; }
      *t4++=0;
      t6=t4-2;
      break;
     }else {t6=t5+strlen(t5)-1; break;}
    }while(1);
    for(;t6>t5 && strchr(" \t\r\n",*t6);*t6--=0);
    while(*t5 && strchr(" \t\r\n",*t5))*t5++=0;
    switch( DWORD_PTR(*t5))
    {
   case 0x20666923 x4CHAR("#if "):
     t6=t5+4;
     while((t6=stristr(t6,"$in_text(") ))
     {if(! (t7=strchr(t6,')')))break;
      *t7=0;
      h=0;
      if(stristr(t1,t6+9))++h;
      memset(t6,' ',t7-t6+1);
      *t6='0'+h;
      t6=t7;
     }
     t6=t5+4;
     sprintf(verr,"%d",rez);
     DelSpaceRe(t6);
     if(!(t6=NextIf(t4+1,"#endif","#if","#endif" )) )
     {debug("Error in %.255s" FSLUSHS "forward: #endif not found\r\n:%s",t8,t4);
      break;
     }
     *t6=0;
     t7=NextIf(t4+1,"#el","#if","#endif");
     *t6='#';
    // dprint(" LogAn '%.128s' \n",t5+4);
     if( rv.LogAn(t5+4) )
     {
      //   dprint(" LogAn '%.64s' return true\n",t5+4);
         if(t7)memset(t7,' ',t6-t7);
     }else
     {

     //  dprint(" LogAn '%.64s' return false\n",t5+4);

       t4=t6;
 // ! temporary
 // debug("%.80s.. false",t5+4);
      if(t7){
       if( WORD_PTR(t7[3])==0x6573 /* else */ ) t4=t7;
       else if( WORD_PTR(t7[3])==0x6669 /* elif */ ) *(t4=t7+2)='#';
      }
     }
    break;
   case 0x6D7223 x4CHAR("#rm") :
   case 0x20766D23 x4CHAR("#mv "):
     r=1;
     if(DWORD_PTR(*t5)==0x6D7223 x4CHAR("#rm"))
     {DeleteFile(vars[7]);
      break;
     }
   case 0x20706323 x4CHAR("#cp ") :
    t5+=4;
    while(*t5<=' ')++t5;
    if(*t5){
     sprintf(pth2+512,"%.500s" FSLUSHS "%8.8X.msg",t5,n);
     if(!r)
     {if( (h=_lcreat(pth2+512,0))>0)
      {_hwrite(h,t1,l);
       _lclose(h);
      }
     }
     else MoveFile(vars[7],pth2+512);
    }
    break;
   default:
    if(*t5!='#')
    {rv.SubstVar(pth2,t5);
     if( t5[0]=='!' ){if( (s_flg&FL_FWDRUN) || !*mbx)
     {
      if(! strchr_meta(t5,'$') )
       sprintf(pth2+1,"%.512s %s%s" FSLUSHS "%8.8X.msg",t5+1,t8,mbx,n);
#ifndef SYSUNIX
      {FndRedir(nm,pth2,'<');
       FndRedir(nm+2,pth2,'>');
       s=0;
       if(nm[2])
       {
        *nm[2]=0;
        if(nm[2][-1]=='>')
        { nm[2][-1]=0;
          s=1;
        }
        FndRedir(nm+4,pth2,'>');
        if(nm[2][-1-s]=='2')
        {
         nm[2][-1-s]=0;
         xchgp(nm[2],nm[4]);
         s<<=1;
         if(nm[2] && nm[2][-1]=='>'){nm[2][-1]=0; s|=1; }
        }
        else if(nm[4])
        { if(nm[4][-1]=='>')
          {
           s|=2;
           nm[4][-1]=0;
           if(nm[4][-2]=='2')nm[4][-2]=0;
          }
        }
       }
       else
       {nm[4]=0;
        nm[5]=0;
       }
       if(nm[1])*nm[1]=0;
       if(nm[3])*nm[3]=0;
       if(nm[5])*nm[5]=0;
      }
      cbFwd.cb=sizeof(cbFwd);
      cbFwd.dwFlags=STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW|STARTF_FORCEOFFFEEDBACK;
      cbFwd.wShowWindow=SW_HIDE;
      if(!(((ulong)nm[0]) && ((ulong)nm[2]) && ((ulong)nm[4])))
       cbFwd.hStdError=cbFwd.hStdOutput=cbFwd.hStdInput=(void*)_lopen("nul",2);
      if(nm[0]){*nm[0]=0; if((x=_lopen(nm[0]+1,0))>0)cbFwd.hStdInput=(void*)x; }
      if(nm[2]){if( ( (s&1) && (x=_lopen(nm[2]+1,1))>0 )
                    || (x=_lcreat(nm[2]+1,1))>0){cbFwd.hStdOutput=(void*)x; _llseek(x,0,2); } }
      if(nm[4]){*nm[4]=0;
        if(stricmp(nm[2]+1,nm[4]+1) )
        {if(((s&2) && (x=_lopen(nm[4]+1,1))>0 )
            || (x=_lcreat(nm[4]+1,1))>0 ){cbFwd.hStdError=(void*)x; _llseek(x,0,2); }
        }else cbFwd.hStdError=cbFwd.hStdOutput;
      }
      debug("Run: %s",pth2+1);
      if(MyCreateProcess(pth2+1,0,t8,&cbFwd,&piFwd)){
       WaitForSingleObject(piFwd.hProcess,ttl_avr<<10);
       GetExitCodeProcess(piFwd.hProcess,&rez);
       if(rez==STILL_ACTIVE){
        debug("Mail script: executable '%s' still active more then %u seconds and will be hard terminated by server.",pth2,ttl_avr);
        TerminateProcess(piFwd.hProcess,0);
       }
       CloseHandle(piFwd.hThread);
       CloseHandle(piFwd.hProcess);
      }
      CloseHandle(cbFwd.hStdOutput);
      if(cbFwd.hStdOutput!=cbFwd.hStdInput)CloseHandle(cbFwd.hStdInput);
      if(cbFwd.hStdOutput!=cbFwd.hStdError)CloseHandle(cbFwd.hStdError);
#else
      debug("Run: '%s'",pth2+1);
      pid_to_wait = 0;
      if( (s=
#ifdef LPC_ARM
	vfork()
#else
	fork()
#endif
      )>0 )
      {x=ttl_avr;
       do{do{
          //sleep(1);
          SrvEventWait(&pid_to_wait,1000);
          if( waitpid(s,(int *)&rez,WNOHANG))goto exFork;}while(--x>0);
          pid_to_wait = 0;
          kill(s,SIGKILL);
       }while(x>-3);
       debug("Mail script: executable '%s' still active more then %u seconds and will be hard terminated by server.",pth2,ttl_avr);
exFork:;
        if(rez<0)debug("Exec error %s %d " SER,pth2+1 ,errno Xstrerror(errno));
      }
      else if(!s)
      {
       if(pwd)
       {
         setregid(pwd->pw_gid,pwd->pw_gid);
         setreuid(pwd->pw_uid,pwd->pw_uid);
       }
       execl("/bin/bash","/bin/bash","-c",pth2+1,0);
       //fprintf(stderr, "Exec error %s %d " SER,pth2+1 ,errno Xstrerror(errno));
       exit(errno);
      }
#endif
      }}
      else if(strchr(pth2,'@') && (t6=strchr(t7=b+5,' ')))
      {*t6=0;
       x=sprintf(pth2,"From %s [redirect from %s] For ",t7,t2);
       *t6=' ';
       if((!strstr(t1,pth2+(t6-t7)+5)) && (!strstr(b+7,t2)))
       {sprintf(pth2+256,"%.255s" FSLUSHS "%8.8X.msg",out_path,GetMsgName(stime));
        if( (h=_lcreat(pth2+256,0))>0)
        {_hwrite(h,pth2,x);
         rv.SubstVar(pth2,t5);
         _hwrite(h,pth2,strlen(pth2));
         t1[-2]='\r';  _hwrite(h,t1-2,l+2);
         _lclose(h); t1[-2]=0;
        }
       }
      }
     }
    }
    t5=t4;
   }
   delete t9;
  }
  else _lclose(h);
 }
 /*
 else  // ! Temporary!!!
 {
   debug("Can't open '%s' %d",pth2?pth2:"",h);

 }
 */
 return r;
}

void MLChk::SndErrMsg(char *mg,char *sbj)
{int x,h;
 if( !striin(b+5,"MAILER-DAEMON")   )
 {x=GetMsgName(stime);
  sprintf(pth2+256,"%.255s" FSLUSHS "%8.8X.msg",out_path,x);
  if(!(t4=strchr(b+5,' ')))return; //goto lexcnt;
  *t4=0;
  if( (! strpbrk(b+5,":;*,/\\?= ") ) && strchr(b+5,'@') )
  {
   if((h=_lcreat(pth2+256,0))>0)
   {
    _hwrite(h,pth2+256,
    sprintf(pth2+256,"From MAILER-DAEMON@%s (%s) For %s\r\nTo: %s\r\n"
      "From: MAILER-DAEMON\r\n"
      "Date: %3.3s, %u %3.3s %4.4u %2.2u:%2.2u:%2.2u GMT\r\n"
      "Subject: %s\r\n\r\n"
      "%s to %s:\r\n\r\n>%.250s\r\n\r\n",
      smtp_name,smtp_name,b+5,b+5,
      wkday[stime.wDayOfWeek],stime.wDay,month[stime.wMonth-1],stime.wYear,
      stime.wHour,stime.wMinute,stime.wSecond
      ,sbj,sbj,t2,mg));
    *t4=' ';
    if(l>2048)
    {_hwrite(h,t1,2048);
     _hwrite(h,"\r\n ... \r\n",9);
    }else _hwrite(h,t1,l);
    _lclose(h);
    last_file=x;
   }
  }
 }
}
ulong SmtpLErr;
ulong WINAPI SMTPcl(void *)
{char pth[512],*bb;
 MLChk chk;
 ulong last;
 WIN32_FIND_DATA fnds;
 HANDLE hdl=INVALID_HANDLE_VALUE,hdlf;
 User *puser;
 int h,i,s,x,sent_cnt,no_move,dir_loop,esmtp;

#define b  chk.b
#define t  chk.t
#define t1 chk.t1
#define t2 chk.t2
#define t3 chk.t3
#define t4 chk.t4
#define t5 chk.t5
#define t6 chk.t6
#define t7 chk.t7
#define t8 chk.t8
#define t9 chk.t9
#define pth2 chk.pth2
#define pth3 chk.pth3
#define l chk.l
#define n chk.n
#define stime chk.stime
#define pwd chk.pwd
#define piFwd chk.piFwd

#ifdef SYSUNIX
  pwd=0;
#endif
goto lbFind;

while( is_no_exit )
{no_move=0;
 if(hdl!=INVALID_HANDLE_VALUE)
 {
   n=atouix(fnds.cFileName);
   ++no_close_req;
   for(i=0;i<max_tsk;i++)
    if( ((u_long)(rreq[i])>1) &&
          rreq[i]->postsize == n &&         rreq[i]->dirlen==0x29041975)
    {
     --no_close_req;
     goto lbDxx;

    }
   --no_close_req;
 }
 else
 {
#ifndef SYSUNIX
  i=WAIT_TIMEOUT;
  hdlf=FindFirstChangeNotification(out_path,0,FILE_NOTIFY_CHANGE_LAST_WRITE);
  while(!last_file)
  {DEBUGVAR(tmout);
   if(-1!=(int)hdlf) i=WaitForSingleObjectEx(hdlf,1000,1);
   else
   {SleepEx(1000,1);
    i=WAIT_TIMEOUT;
   }
   if( (!(++SMTPCounter&0x7FF)) || i!=WAIT_TIMEOUT)
   {if(-1!=(int)hdlf)FindCloseChangeNotification(hdlf);
    goto lbX;
   }
   smtp_chk.CheckProxy();
  }
  FindCloseChangeNotification(hdlf);
#else
  while( !last_file)
  {sleep(1);
   if(! (++SMTPCounter&0x7FF) ) goto lbX;
   if(!is_no_exit) return 0;
  }
#endif
  n=last_file;
  last_file=0;
 }
if(n)
{sprintf(pth,"%.255s" FSLUSHS "%8.8X.msg",out_path,n);
 if((h=_lopen(pth,0))>0)
 {++dir_loop;
  debug("Send %s",pth);
  sent_cnt=0;
  l=FileSize(h);
  if(l>max_msg_size)
  {_lclose(h);
   goto lbXX;
  };
  if(! (bb=new char[l+64]) )
  {debug("Error. No enought memory. Can't send %s size %u",pth,l);
   _lclose(h);
   if(l<0xC0000)Restart();
   goto lbXX;
  }
  b=bb+sizeof(ReseivF)-2;
  _hread(h,b,l);
  _lclose(h);
  b[l]=0;
  if( (t1=strstr(b,"\r\n")) )
  {*t1=0;
   t1+=2;
   l-=t1-b;
   if( !(t=strstr(b," For ")))goto lBadFile;
   DWORD_PTR(*t)=0x0A0D;
   memcpy(bb,(void *)ReseivF,sizeof(ReseivF)-1);
   t2=t+5;

  debug("to: %s",t2);
  if(antiv)
  {sprintf(pth2,"%.255s",antiv);
   t8=out_path;
   if(chk.RunForward(""))goto lexcnt2;
  }
  do{
    if( (t3=strchr(t2,' ') ) )*t3=0;
    if( !(t4=strchr(t2,'@') ) )break;
    if(IsUsHost(t4))
    { //Local
     *t4=0;
     puser=FindUser(t2,UserPOP3,0,0);
     *t4='@';
     if(!puser)
     {
     lerrMsg1:;
      no_move=0;
      chk.SndErrMsg(pth2);
     }
     else
     {sprintf(pth2,"%.255s" FSLUSHS "mbox" FSLUSHS "%8.8X.msg",t8=puser->dir(),n);
      if( (h=_lcreat(pth2,0))<0)goto lerrMsg1;
#ifdef SYSUNIX
      if( (pwd=getpwnam(puser->name)) || (pwd=getpwnam("ftp")) )
      {fchown(h,pwd->pw_uid,pwd->pw_gid); }
#endif
      _hwrite(h,bb,(t-bb)+2);
      _hwrite(h,t1,l);
      _lclose(h);
      ++sent_cnt;
      sprintf(pth2,"%.255s" FSLUSHS "forward",t8);
      if(s_flg&FL_FORWARD) chk.RunForward( FSLUSHS "mbox");
     }
    }
    else
    {//Remote
     no_move=1;
     pth3[512]=0;
     pth3[512+68]=0;
     if(smtproxy)
     {strncpy(pth3+512,smtproxy,64);
      t5=0; //pth2;
      DWORD_PTR(*pth2)=0;
     }
     else if( (!(t5=GetMailHost(t4+1,(d_msg *)pth3,0))))//||(pth3[3]&0x7F)
     {strcpy(pth2,"Mail host not found...");
      debug(pth2);
      if(n>SmtpLErr)
      {chk.SndErrMsg("Mail host not found...", "Can't send message. May be it is temporary, server will try again" );
       SmtpLErr=n;
      }
      goto lbErr2;
     }
     else
     {
      x=0;
  loopNextMH:
      if(! (t5=(char *)GetNextMH((uchar *)pth3 //,(uchar *)t5,pth3+512
        )))
      { if(x<=499)goto lbErr2;
        goto lerrMsg1;
      }
  lbAgain:
      debug("MAILHOST: %s %s",pth3+512,pth3+512+68);
     }
     if( (s=call_socket2(pth3+ (pth3[512+68]?512+68:512),25) )>0 )
     {AddToLog("|Open",s,0,FmtShrtR);
      x=0;

       if((x=GetCMD(s,pth2,0))!=220)goto lerrmsg;
       esmtp=
#ifdef A_64
      NULL !=
#else
   (int)
#endif
         stristr(pth2,"ESMTP");
       while(pth2[3]=='-')GetCMD(s,pth2,0);
       SendCMD(pth2, sprintf(pth2,(esmtp)?"EHLO %s\r\n": "HELO %s\r\n",smtp_name) );
       if( ((x=GetCMD(s,pth2,0))!=250) || !stricmp(pth2+4,smtp_name) )
       {lerrmsg:
        SendConstCMD("QUIT\r\n" );
       brkConn:
        debug("!SMTP error send to %.127s message %X <%.127s> \r\n",t2,n,pth2);
        //shutdown(s,2);
        //closesocket( (int) s);
         CloseSocket(s);

//if(t5 && (t5=(char *)GetNextMH((uchar *)pth2,(uchar *)t5,pth2+512))) goto lbAgain;
        if(t5)goto loopNextMH;
        if(x<0)strcpy(pth2,"Connection error");
        if(n>SmtpLErr)
        {
         chk.SndErrMsg(pth2,"Can't send message. May be it is temporary, server will try again" );
         SmtpLErr=n;
        }


         //if((x/10)==45) goto lbErr2;
         if(x<=499) goto lbErr2;
         goto lerrMsg1;
       };
       while(pth2[3]=='-')GetCMD(s,pth2,0);
       if(!(t4=strchr(b+5,' ')))goto lexsend;
       x=' ';
       do{
        *t4=0;
        SendCMD(pth2, sprintf(pth2,(esmtp)?"MAIL FROM:<%s> SIZE=%u\r\n":"MAIL FROM:<%s>\r\n",b+5,l+(t-bb)+2) );
        *t4=x;
        if( (x=GetCMD(s,pth2,0))==250) break;
        if( (!striin(b+5,"MAILER-DAEMON")) || !striin(b+5+sizeof("MAILER-DAEMON"),smtp_name ) ) goto lerrmsg;
        t4=b+5;
        x='M';
       }while(1);
       SendCMD(pth2, sprintf(pth2,"RCPT TO:<%s>\r\n",t2) );
       if( (x=GetCMD(s,pth2,0))!=250) goto lerrmsg;
       SendConstCMD( "DATA\r\n" );
       if( (x=GetCMD(s,pth2,0))!=354) goto lerrmsg;
       send(s,bb,(t-bb)+2,0);
       send(s,t1,l,0);
       SendConstCMD("\r\n.\r\n" );
       if( (x=GetCMD(s,pth2,0))!=250){t5=0; goto lerrmsg;}
       if( (s_flgs[1]&FL1_CONFIRM) && (t4=stristr(t1,"\nGenerate-Delivery-Report:")) && t4<stristr(t1,"\r\n\r\n")  )
       { chk.SndErrMsg(pth2, "Delivery success");  }
       ++sent_cnt;
       no_move=0;
     lexsend:
      SendConstCMD("QUIT\r\n" );
      //shutdown(s,2);
      //closesocket( (int) s);
       CloseSocket(s);
      if(time_btw<2)time_btw=2;
      Sleep(time_btw*1024 ); //2048);
     }
     else if(!t5) *pth2=0;
     if(no_move )
     {
     lbErr3:
      debug("Call to %s failed",pth3+512);
      if(t5)goto loopNextMH;
     lbErr2:
     // no_move=1;
      debug("!SMTP connect to %.127s failed. (Msg %X)\r\n",t2,n);

      if(hdl!=INVALID_HANDLE_VALUE)
      {GetSystemTimeAsFileTime(&fnds.ftLastAccessTime );
       if(
#ifndef SYSUNIX
          (fnds.ftLastAccessTime.dwHighDateTime
           - fnds.ftCreationTime.dwHighDateTime)>201
#else
     (fnds.ftLastAccessTime - fnds.ftCreationTime) > 86400
#endif
         ){no_move=0; goto lerrMsg1; }
       }
      }
     }
  lexcnt:
     t2=t3+1;
   }while(t3);
  }
  else
  {lBadFile:
   debug("Bad file %s",pth);
  }
 lexcnt2:

  delete bb;
  if(sent_cnt || !no_move)
  {
 lbXX:
   if( (!(t6=(sent_cnt)?sent_path:err_path)) ||
      (sprintf(pth2,"%.255s" FSLUSHS "%8.8X.msg",t6,n), !MoveFile(pth,pth2))
     )DeleteFile(pth);
  }
 }
}
lbX:

 if(hdl==INVALID_HANDLE_VALUE)
 {
 lbFind:;
   sprintf(pth,"%.255s" FSLUSHS "*.msg",out_path);
   hdl=FindFirstFile(pth,&fnds);
   dir_loop=0;
 }else goto lbDxx;
 if(hdl!=INVALID_HANDLE_VALUE)
 { while( (!fnds.nFileSizeLow) || (fnds.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    ||fnds.cFileName[8]!='.')
   {
   lbDxx:
    if(! (FindNextFile(hdl,&fnds)))
    {FindClose(hdl);
     hdl=INVALID_HANDLE_VALUE;
     if(dir_loop && !no_move)goto lbFind;
     break;
    }
   };
 }
}
 return 0;
};

extern "C"{
 ulong ConstPerSecond=10000000;
};
#undef b
#undef t
#undef t1
#undef t2
#undef t3
#undef t4
#undef t5
#undef t6
#undef t7
#undef t8
#undef t9
#undef pth2
#undef pth3
#undef l
#undef n
#undef stime
#undef pwd
#undef piFwd

struct ToDel
{
 int cnt;
 ulong d[512];
 //ulong dd[256];

 void Init(){cnt=0;}
// void Push(ulong x){d[cnt++]=x;}
 void Flush(char *bfr,char *dir,char *pdir,char *ext);
 void PushChk(char *fname, //ulong x,
               char *bfr,char *dir,char *pdir,char *ext)
 {
  if(cnt>=255)Flush(bfr,dir,pdir,ext);
  d[cnt++]=DWORD_PTR(fname[0]);
  d[cnt++]=DWORD_PTR(fname[4]);
 }
};

void ToDel::Flush(char *bfr,char *dir,char *pdir,char *ext)
{
 int i;
 for(i=0; i<cnt ; ++i)
 {
  sprintf(bfr,"%s" FSLUSHS "%s%8.8s.%s",dir,pdir,d+i*2,ext);
  if(!DeleteFile(bfr))
   debug("Can't delete %s (%d)",bfr,errno);//GetLastError());
 }
 cnt=0;
}

//-------------
void CheckFileTime::CheckDDir(char *bfr,char *pdir,long d)
{
 WIN32_FIND_DATA fnds;
 HANDLE  hdl;
 char  dbfr[72];
 long dd;
 sprintf(bfr,"%s" FSLUSHS "%s*",dir,pdir);
 if( (hdl=FindFirstFile(bfr,&fnds))!=INVALID_HANDLE_VALUE)
 {
  do{
   if((fnds.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) &&
      (dd=strlen(fnds.cFileName))>2
   )
   {
    if(!(*pdir))
    {
     CheckDDir(bfr,fnds.cFileName,d);
     CheckDir(bfr,fnds.cFileName,d);
    }
    else
    {sprintf(dbfr,"%.8s"  FSLUSHS  "%.64s",pdir,fnds.cFileName);
     CheckDir(bfr,dbfr,d);
    }
   }
  }while(FindNextFile(hdl,&fnds)>0);
  FindClose(hdl);
 }
}

void CheckFileTime::CheckDir(char *bfr,char *pdir,long d)
{
 WIN32_FIND_DATA fnds;
 HANDLE  hdl;
 ToDel dlst;
 long dd,c=0;

  dlst.Init();
  sprintf(bfr,"%s" FSLUSHS "%s*.%s",dir,pdir,ext);
  if( (hdl=FindFirstFile(bfr,&fnds))!=INVALID_HANDLE_VALUE)
  {do{
    ++c;
    if(((s_flgs[1]&FL1_LACPROXY)?fnds.ftLastAccessTime:fnds.ftLastWriteTime)
#ifndef SYSUNIX
     .dwHighDateTime
#endif
     <d)
    {//sprintf(bfr,"%s" FSLUSHS "%s%8.8X.%s",dir,pdir,atouix(fnds.cFileName),ext);
     //DeleteFile(bfr);
     //if((dd=atouix(fnds.cFileName)))
     if(fnds.cFileName[0]!='.' && strlen(fnds.cFileName)>8 && fnds.cFileName[8]=='.')
      dlst.PushChk(fnds.cFileName,bfr,dir,pdir,ext);
    }
   }while(FindNextFile(hdl,&fnds)>0);
   FindClose(hdl);
   dlst.Flush(bfr,dir,pdir,ext);
  }
  if( (!c) && ext[0]=='a' && *pdir &&  (s_flgs[1]&FL1_BBPROXY) )
  {
    sprintf(bfr,"%s" FSLUSHS "%s",dir,pdir);
    RemoveDirectory(bfr);
  }

}
void CheckFileTime::CheckProxy()
{
 WIN32_FIND_DATA fnds;
 HANDLE  hdl;
 SYSTEMTIME  stime;
 FILETIME FTime;
 long d,dd;
 char bfr[512];
 char pdir[12];
 ToDel dlst;
 int i=0;
 if(dir && time)
 {
//  dlst.cnt=0;
#ifndef SYSUNIX
  GetLocalTime(&stime);
  SystemTimeToFileTime(&stime,&FTime);
  if( FTime.dwHighDateTime<CheckProxyTime    )return;
  CheckProxyTime=FTime.dwHighDateTime+67;
#ifdef  NOTINTEL

#error ("code for not Intel CPU must be here...")

#else

  asm("imull $86400,%%eax \n"
       "mull _ConstPerSecond \n"
       "negl %%edx \n"
    :"=d"(d),"=&a"(hdl)
    :"1"(time)
    );
#endif
  d+=FTime.dwHighDateTime;
#else
  FTime=::time(0);
  if( FTime<CheckProxyTime )return;
  CheckProxyTime=FTime+86400;
  d=FTime-time*86400;
#endif
  pdir[0]=0;
  debug("Begin check proxy cache");
do{
   CheckDir(bfr,pdir,d);
   if(*ext!='a' || (!(s_flgs[1]&FL1_BIGPROXY)) || i>0x3F )break;
   sprintf(pdir,"%2.2X" FSLUSHS,i);
   ++i;
  }while(1);

  if(*ext=='a' && (s_flgs[1]&FL1_BBPROXY) )
  {
   CheckDDir(bfr,"",d);
  }
  debug("End check proxy cache");
 }
};
#undef send

