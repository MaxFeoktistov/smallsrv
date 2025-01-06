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

#include "srv.h"

#undef LF
#define LF "\n"

#ifndef STRING_CONST_H
#include "g4strc.h"
#endif

#define MAINSTR 1
#include "g4strcwm.h"

//#include "bvprintf.h"

#ifdef SYSUNIX
#include<pwd.h>
#include<grp.h>
const char atrib[5]={'-','x','w',' ','r'};
#endif

char *loc_sent ="sent";
char *loc_draft="draft";
char *loc_trash="trash";


//extern "C"{
//  ulong c1e7=10000000;
//};

/*

struct  OMsg
{
 ulong m;
 char *boundary,*txt;
 char *from,*to,*to_email,*dat,*subj,*fromemail;
 char *bfr;
 int type;
 int Load();
 void OutMSG();
 void OutQuote();
};

*/


char shello[]="                      Hello $to!\n";
char stmpl[]="                      Hello $to !\n"
" In $date $to &lt;$to_email&gt; wrote about \"$subj\":\n"
"\n";
char ssign[] ="                                                 Best regards, I\n";



#define OMsg WMail
#define SCRIPTNAME  "/$_wmail_$"
#undef LF
#define LF "\n"



char WM_HEAD[]=
"HTTP/1.0 200 Ok\r\n"
"Content-type: text/html%s"
//#ifdef RUS
//"; charset=windows-1251"
//#endif
    "\r\n"
"Pragma: No-Cache\r\n"
"\r\n"
"<html><head>"
//"\n<!-- WebMail for Small HTTP Server. -->\n"
"<style>\n"
"font {font-size:9pt; font-family: times; font-style: normal ; line-height:1em}\n"
"table { border-collapse: collapse ; border-spacing: 0px ; font-size:10pt; line-height:11pt; font-family: Times,serif;}\n"
"</style>"
"<script language=javascript>\n"
" function SHTbl(tb)\n"
" {var tbl;\n"
"  tbl=document.getElementById(tb);"
"  lnk=document.getElementById('l'+tb);"
"  if(tbl.style.display == \"none\") {"
"  tbl.style.display = \"\";  if(lnk) lnk.innerHTML='[-]' ; "
"  } else { tbl.style.display = \"none\";  if(lnk) lnk.innerHTML='[+]' ; }"
" };\n"
"</script>"
"<title>" "%s" // sWeb_mail
"</title>"
"</head> <body><center><h1>" "%s" //  sWeb_mail
"</h1>"
"<table width=100%% border=0 bgcolor=#FFFFFF>"
"<tr align=center><td>"
"<b><a href='" SCRIPTNAME "?s=1' >Mailbox</a></b>"
"</td><td>"
"<b><a href='" SCRIPTNAME "%s" //  s_S_____NE
SCRIPTNAME
"%s" //     s_S_____BL
SCRIPTNAME
"%s" //     s_S_____OP
SCRIPTNAME "?s=6&u=%s' >Logout</a></b>"
"</td></tr>"
"</table><br>" LF
;



char BinHead[]=
"HTTP/1.0 200 Ok\r\n"
"Content-type: application/octet-stream\r\n"
"Content-Length: %u\r\n"
"\r\n" ;


// char NEW_MSG[]=
// "<form name=nmsg "
//  //  " action= " SCRIPTNAME
//     " method=post>"
//  //   "<input type=hidden name=s value=7 >"
//     s_POP3a1
// ;
#define NEW_MSG s_POP3a1

char NEW_END[]=
"\n---"
"%.512s\n"
"</textarea>"
"</td></tr><tr><td align=center>"

"<input type=submit value=\""
  "%s"  //sSend
    "\">"
" &nbsp; <input type=submit value=\""
   "%s"  // sDraft
    "\"  onClick=\"javascript:form.s.value=17;\" >"

"</td></tr>"
"</table>"
"</form> "
;

char llist_comon[]=
"[<a href='" SCRIPTNAME "?s=25' >Badlist</a>] &nbsp; &nbsp;  "
"[<a href='" SCRIPTNAME "?s=26' >Goodlist</a>]&nbsp; &nbsp;  "
"[<a href='" SCRIPTNAME "?s=27' >Graylist</a>] &nbsp; &nbsp;  "
"[<a href='" SCRIPTNAME "?s=28' >Forward</a>] <br><br> "
"<form method=post name=m>" LF
"<input type=hidden name=s value=%u >"
"<center>" LF
"<h2>%s</h2>" LF
"%s" LF
"</center>" LF


"<script language=javascript >" LF
"" LF
"function AddE(a)" LF
"{" LF
" document.m.v.value += \"\\n\"+a;" LF
"}" LF
"function AddEm(a,b)" LF
"{" LF
" document.m.v.value += \"\\n? $\" + a + \"=~ /\" + b +  \"/\";" LF
"}" LF
"" LF
"</script>" LF


"<table border=0 bgcolor=#F0F8F8>" LF
"<tr valign=bottom>" LF
"<td>" LF
;

char alist_comon[]=
"[<a href='?s=153' >Badlist</a>] &nbsp; &nbsp;  "
"[<a href='?s=154' >Goodlist</a>]&nbsp; &nbsp;  "
"[<a href='?s=155' >Graylist</a>] &nbsp; &nbsp;  "
"[<a href='?s=156' >Antivirus</a>] &nbsp; &nbsp;  "
"[<a href='?s=157' >DNS hosts</a>] &nbsp; &nbsp;  "
"[<a href='?s=158' >Config file</a>] <br><br> "
"<form method=post name=m>" LF
"<input type=hidden name=s value=%u >"
"<center>" LF
"<h2>%s</h2>" LF
"%s" LF
"</center>" LF


"<script language=javascript >" LF
"" LF
"function AddE(a)" LF
"{" LF
" document.m.v.value += \"\\n\"+a;" LF
"}" LF
"function AddEm(a,b)" LF
"{" LF
" document.m.v.value += \"\\n? $\" + a + \"=~ /\" + b +  \"/\";" LF
"}" LF
"" LF
"</script>" LF


"<table border=0 bgcolor=#F0F8F8>" LF
"<tr valign=bottom>" LF
"<td>" LF
;


/*
char llist[]=

    LLIST

"</ul> " LF
"</ul> " LF
"<center>" LF
"" LF
"<textarea name=v rows=30 cols=80 >" LF
;*/
#define llist LLIST
//*
char llist_end[]=
//"</textarea>" LF
"<br>" LF
"</td>" LF
"<td>" LF
"  " LF

" %s "  // LLEND

"  <br>" LF
"  </td>" LF
"  </tr></table>" LF
"  " LF
"<center>" LF
"<input type=submit name=submit value=\"%s\" >" LF   //" sSubmit "\">" LF
"</center>" LF
"</form>" LF
    ; //*/

//#define llist_end  LLEND

/*
char HDR_OPT[]=
  " <form method=post name=m>" LF
s_HEADER0
;*/

#define HDR_OPT s_HEADER0
/*
char HdForward[]=
sFORWARD
"" LF
"<center>" LF
"" LF
"<textarea name=v rows=30 cols=80 >" LF
;*/

#define HdForward sFORWARD
//*
char HdForwardEnd[]=

//"</textarea>" LF
"<br>" LF
"</td>" LF
"<td>" LF
"" LF

//sFORWARD2
"%s"

"" LF
"  <br>" LF
"  </td>" LF
"  </tr></table>" LF
"  " LF
"<center>" LF
"<input type=submit name=submit value=\"%s\" >" LF   // sSubmit "\">" LF
"</center>" LF
"</form>" LF
    ; //  */

//#define HdForwardEnd sFORWARD2
/*
char ns_lst[]=

    NSLST

"<br>" LF
"<center>" LF
"<textarea name=v rows=30 cols=80 >" LF
;*/

#define ns_lst NSLST



char ns_lst_end[]=



//"</textarea>" LF
//"<br>" LF
"<script language=javascript >" LF
"" LF
"var names=Array('','www.','ftp.','mail.','ns.', 'ns2.','mx.','ns3.');" LF
"var rts=Array('198.41.0.4','192.228.79.201','192.33.4.12','128.8.10.90','192.203.230.10','192.5.5.241','192.112.36.4','128.63.2.53'," LF
"'192.36.148.17','192.58.128.30','193.0.14.129','199.7.83.42','202.12.27.33');" LF
"var ab='abcdefghijklmn';" LF
"" LF
"var c=0,n=0;" LF
"var  dm='domain.com';" LF
"" LF
//"function AddE(a)" LF
//"{  document.m.v.value += \"\\n\" +a; }" LF
"function upd(a)" LF
"{" LF
" var i,j,s,t;" LF
" s=document.m.v.value;" LF
" if( (i=s.lastIndexOf(' SOA '))>0 )" LF
" { j=s.lastIndexOf('\\n',i);" LF
"   t= s.substr(j+1,i-j-1);" LF
"   t=t.replace(/ IN/g,'');" LF
"   t=t.replace(/ /g,'');" LF
"   dm = t;" LF
"   a=a.replace(/domain.com/g,t);" LF
" }" LF
" return a;" LF
"}" LF
"" LF
"function updi(a)" LF
"{" LF
" var i,j,s,t;" LF
" s=document.m.v.value;" LF
" if( (i=s.lastIndexOf(' A '))>0 )" LF
" {" LF
"  if( (j=s.indexOf('\\n',i+=3))>i) " LF
"       t= s.substr(i,j-i);" LF
"  else t= s.substr(i);" LF
"  t=t.replace(/ /g,'');" LF
"  a=a.replace(/10.1.0.1/g,t);" LF
" }" LF
" return a;" LF
"}" LF
"" LF
"" LF
"function AddD(a)" LF
"{" LF
" a=upd(a);" LF
" document.m.v.value += '\\n' + names[c] + dm + ' IN '+a; " LF
"}" LF
"function AddA(a)" LF
"{ " LF
"  upd(a);" LF
"  a=updi(a);" LF
"  document.m.v.value += '\\n' + names[c] + dm +' IN '+a; " LF
"  ++c;" LF
"  c&=7;" LF
"}" LF
"function IR()" LF
"{ " LF
" var ns;" LF
" ns=ab.substr(n,1) + '.root-servers.net';" LF
" document.m.v.value = '\\n. IN NS ' + ns+ '\\n ' + ns + ' IN A ' + rts[n] + '\\n' + document.m.v.value;" LF
" if(++n>=rts.length) n=0;" LF
"}" LF
"</script>" LF
"</td>" LF
"<td>" LF
"  <input type=button value='Insert root NS server' onClick=\"javascript:IR();\"><br>" LF
"  <input type=button value='Set TTL' onClick=\"javascript:AddE('$TTL 3600');\"><br>" LF
"  <input type=button value='Add SOA' onClick=\"javascript:AddD('SOA  ns.domain.com  info@domain.com (\\n 2005020205 ; Serial \\n   36000      ; Refresh\\n   3000       ; Retry\\n   36000000   ; Expire\\n  36000      ; Minimum\\n ) ');\"><br>" LF
"  <input type=button value='Add NS' onClick=\"javascript:AddD('NS ns.domain.com');\"><br>" LF
"  <input type=button value='Add MX' onClick=\"javascript:AddD('MX 10 mx.domain.com');\"><br>" LF
"  <input type=button value='Add A' onClick=\"javascript:AddA('A 10.1.0.1');\"><br>" LF
"  <input type=button value='Add SLAVE' onClick=\"javascript:AddE('$SLAVE domain.com 10.1.0.1');\"><br>" LF
"  <input type=button value='Change IP if service down' onClick=\"javascript:AddE('$IF_DOWN 10.1.0.1:80 300 10.1.0.1=10.1.0.2');\"><br>" LF
"<br>" LF
"</td></tr></table>" LF
"<center>" LF
"<input type=submit name=submit value=Submit>" LF
"</center>" LF
"</form>" LF
    ;


char conf_htm_end[]=
//"</textarea>" LF
//"<br>" LF
"</td></tr></table>" LF
"<center>" LF
"<input type=submit name=submit value=Submit>" LF
"</center>" LF
"</form>" LF
    ;
//#define SCRIPTNAME

struct WMail
{
 Req *r;
 BFILE bfl;
 char *mbox;
 User *puser;
 char fromm[192];

 void OutMbox();
 //int S_printf(char *fmt,...);
#define S_printf bfl.bprintf


 ulong m;
 char *subdir;
 char *boundary,*txt,*end,*bfr;
 char *from,*to,*to_email,*dat,*subj,*fromemail;
 char *hello, *tmpl,*sign;
 int type;

 char bbfr[1];



 int Load();
 void OutMSG();
 int OutMSGSrc();
 int OutAtthRA();
 void OutAtth();
 void OutQuote();
 //void SendMsg();
 void DelOne();
 void CheckDel();
 char *LoadUserOpt(User * usr);
 void UpdUserOpt();
 void SubTempl(char *t,char *s);
 void OutList(int n);
 void SaveList(int n);
 void OutQV(char *txt,char *sf);
 void OutOpt()
 {
   S_printf(HDR_OPT,from,fromm,//reply,reto,
            hello,tmpl,sign,sign);
 }
 void SaveAll();
 void CrDir();
#ifdef SYSUNIX
void CHUsr(int h);
#endif
  void ChPwd(User *);

};
/*
char *GetVarS(char **varlist,char *var)
{
 char *r;
 if(!(r=GetVar(varlist,var) ) )return "";
 return r;
}
*/

int UTFMutex;
uchar *utfbfr;
int lovUtfCode=0xC0;
int utfShift=0x410;

void WMSendHead(WMail * wm,char *u,int utf)
{
     wm->S_printf(WM_HEAD, utf?      //(s_flgs[2]&FL2_WMUTF)?
     "; charset=utf-8":
#ifdef RUS
"; charset=windows-1251"
#else
charset
#endif
  ,sWeb_mail,sWeb_mail,s_S_____NE,s_S_____BL,s_S_____OP,u
  );


}

#define UTFSNDBFRSIZE 0x4000
int ConvertUTFSnd(Req* r,uchar *s,int l)
{
 int i,j,k,ll,ret=0;
 MyLock(UTFMutex);
 if(!utfbfr)utfbfr=(uchar *)malloc(UTFSNDBFRSIZE+32);
 for(j=i=0;i<l;++i)
 {
  if(s[i]<0x80)utfbfr[j++]=s[i];
  else
  {
    if(s[i]>=lovUtfCode)k=(s[i]-lovUtfCode)+utfShift;
    else k=s[i];
    utfbfr[j++]=0xC0|(k>>6);
    utfbfr[j++]=0x80|(k&0x3F);
  }
  if(j>=UTFSNDBFRSIZE)
  {
    ret+=k=(r->Snd)(r,utfbfr,j);
    j=0;
    if( k <=0 ) break;
  }
 }
 if(j) ret+=(r->Snd)(r,utfbfr,j);
 MyUnlock(UTFMutex);
 return ret;
};


WMail * InitWMail(Req *rr, char *b,int utf)
{
 WMail *ret;

  ret=((WMail *)(void *) b );
  ret->r=rr;
  ret->bfl.Init(rr, (utf? (PrintFlush)ConvertUTFSnd : (PrintFlush)rr->Snd),ret->bbfr);
  return  ret;
}

void WMail::SubTempl(char *t,char *s)
{
 char c,*p,*v,*vv;
// debug("%s",s);
 while(*s)
 {
  if(*s=='$')
  {
    if( (p=strpbrk(++s," \"\'\t\n\r!#$%&'()*+,-./:;<=>?")))
    { c=*p;
      *p=0;
    }
    //vv=GetVarS(r->req_var,s);
    //debug("%s=%s",s,vv);
    //if(!vv)vv="";
    t+=sprintf(t,"%.256s",GetVarS(r->req_var,s));
                //vv);
    if(!p)return;
    s=p;
    *p=c;
  }
  *t++=*s++;
 };
};
//-----------

int wbmMutex,wmlg_cntr;
ulong lloutIP;

//extern const char *anFnd[];
int Req::Wmail(User *usr)
{
  char *u;
  char *rpost;
  int l,i,ret;
  char *t;
  WMail * wm;
  int f=0;

//   debug("Here: 0 %X",usr);
   if(usr)f=0x80;
   else if( ! (usr=ChUser(UserPOP3)) )
   {
  lbLout:
#ifdef WITHMD5
//    if(  ( (s_flgs[2] & FL2_MD5PASS) || ! (s_flgs[1]&FL1_CRYPTPWD) ) &&  (s_flgs[2] & FL2_USEMD5D) && !(fl&F_DIGET_UNAVILABLE) )
//    {
       SendDigestAuthReq(loc);
//    }
#else
    Send(AuthErr, strlen(AuthErr)  ) ;//sizeof(AuthErr)-1);
#endif
    return -1;
   }

//debug("XDD %X |%.8s|",anFnd[0],anFnd[0]);

  DBGLA("WM")
  rpost=0;
  if((u=GetVar(http_var,"CONTENT_LENGTH")) && (l=atoui(u)) )
  {
   rpost=pst;
   if(l>0x7000)
   {
     if(l>post_limit)
     {
       HttpReturnError("Too long request");
       return -1;
     }
     rpost=new char[l+64];
         //(char *)malloc(l+64);
     if(postsize)memcpy(rpost,pst,postsize);

   }
   while(postsize<l)
   {
    if((i=Recv(rpost+postsize,l-postsize))<=0)     return 0;
    postsize+=i;
   }
   req=rpost;
   rpost[postsize]=0;
  }
  prepare_Req_var();
  int action=0;
  if((t=GetVar(req_var,"s"))) action=atoui(t);
//  debug("action %u %u");
  if(action<f)return 0;
  ret=action;

  DBGLA("action=%u", action)

  if(action==6 && ( (++wmlg_cntr&1) ||   lloutIP!=sa_c.sin_addr. S_ADDR  ) )
  {
    lloutIP=sa_c.sin_addr. S_ADDR;
    wmlg_cntr|=1;
    goto  lbLout;
  }

  //debug("XDD2 %X |%.8s|",anFnd[0],anFnd[0]);
  int  utf;
  utf=(s_flgs[2]&FL2_WMUTF) && action < 128;
  wm=InitWMail(this, /*loc+0x8000+260  ); */ ((char *)http_var)+0x200,utf);

  wm->subdir=loc+dirlen+sizeof("/$_wmail_$")-1;
//debug("subdir |%s|",wm->subdir);
  sprintf(wm->fromm,"%.64s@%.64s",usr->name,smtp_name);
  if(action<64) {
    //Send(WM_HEAD,sizeof(WM_HEAD)-1 );
//     wm->S_printf(WM_HEAD, utf?      //(s_flgs[2]&FL2_WMUTF)?
//      "; charset=utf8":
// #ifdef RUS
// "; charset=windows-1251"
// #else
// ""
// #endif
//   );

    WMSendHead(wm,usr->name,utf);
  }

  if(* wm->subdir!='/'   || ! (t= strrchr(wm->subdir +1,'/' ))
    ) {   wm->subdir="";  }
  else
  {
    *t=0;
#ifndef SYSUNIX
    * wm->subdir='\\';
#endif
    if((t=strpbrk(wm->subdir+1,".?* ") ))*t=0;
    if(strlen(wm->subdir)>64 || !CheckBadName(wm->subdir))goto exPoint;
    if(action<64)
    {
      wm->S_printf("<center><a href=/$_wmail_$>[..]</a> &nbsp; [%s]</center>",wm->subdir );


    }

  }
//debug("2 subdir |%s|",wm->subdir);

 //  debug("s=%u",action);
  wm->mbox=usr->dir();
  wm->puser=usr;

  //debug("XDD3 %X |%.8s|",anFnd[0],anFnd[0]);

  DBGLA("action=%u", action)

  switch(action)
  {

//    case (0x80|30):
    case (0x80|31):
    case (0x80|32):
    case (0x80|33):
    case (0x80|34):
    case (0x80|35):
    case (0x80|36):

    // case 30:
     case 31:
     case 32:
     case 33:
     case 34:
         wm->SaveList(action-31);
         action-=6;
    case (0x80|25):
    case (0x80|26):
    case (0x80|27):
    case (0x80|28):
    case (0x80|29):
        if(0){
    case (0x80|30):   SaveCfgIfNeed(); }
     case 25:
     case 26:
     case 27:
     case 28:

         DBGLA("OutList %u", action-25)

         wm->OutList(action-25);
         break;

     case 64:
         if(!(wm->OutMSGSrc()))  break;
         if(0){
     case 65:
         if(!(wm->OutAtthRA())) break;
         }
         //Send(WM_HEAD,sizeof(WM_HEAD)-1 );
//              wm->S_printf(WM_HEAD, (s_flgs[2]&FL2_WMUTF)? "; charset=utf8":
// #ifdef RUS
// "; charset=windows-1251"
// #else
// ""
// #endif
//           );

         WMSendHead(wm,usr->name,utf);

         wm->S_printf( s_H__BAD_M );
         break;

     case 20:
         wm->OutMSG();
         break;

     case 3:
        wm->ChPwd(usr);
     break;

     case 2:
        t=wm->LoadUserOpt(usr);
        wm->UpdUserOpt();
        wm->SaveAll();
       /*
       for(pcp=ConfigParams;pcp->name;++pcp)
         if(pcp->min != 255)
         {
           if( (i=GetVarN(pcp->name))>0)
           {
            if(strcmp(form_val[i],*(char**)pcp->v ) )
            {
             state|=1;
             *pcp->v=(int)form_val[i];
            }
           }
         }
       */

     if(0){
    case 15:
       t=wm->LoadUserOpt(usr);
     }
       wm->OutOpt();
       //MyUnlock(wbmMutex);
       //break;
      if(0){
     case 9:  //  Reply
     case 19:  //Edit

       //*

       t=//new char[0x10000];
       wm->LoadUserOpt(usr);
       if(action != 19) wm->SubTempl(t,wm->tmpl);

       wm->S_printf(NEW_MSG,wm->from,wm->fromemail,
       GetVarS(req_var,"to"),GetVarS(req_var,"to_email"), //wm->reply,wm->reto,
       GetVarS(req_var,"subj"),t);
       if(wm->Load())
       {
          if(action != 19) wm->OutQuote();
          else
          {
            if(wm->type==2)html2text(wm->txt,wm->txt);
            wm->S_printf( "%s\n",wm->txt);
          }
          delete wm->bfr;
       }

       // */
       /*
     if(0){

     case 19:  //Edit
       t=wm->LoadUserOpt(usr);
       wm->S_printf(NEW_MSG,wm->from,wm->fromemail,
       GetVarS(req_var,"to"),GetVarS(req_var,"to_email"), //wm->reply,wm->reto,
       GetVarS(req_var,"subj"),t);
       if(wm->Load())
       {
          if(wm->type==2)html2text(wm->txt,wm->txt);
          wm->S_printf( "%s\n",wm->txt);
          //OutMSGEdt();
          delete wm->bfr;
       }

       */


     if(0){
     case 5:  // New MSG
       //*
    //   debug("1");
       t=//new char[0x10000];
       wm->LoadUserOpt(usr);
    //   debug("2");
       wm->SubTempl(t,wm->hello);
    //   debug("3");
       wm->S_printf(NEW_MSG,wm->from,wm->fromemail,"","",//wm->reply,wm->reto,
                   // "","",
                    "",t)
      // */
       ;

    //   debug("4");
     }
     //}
       wm->bfl.bprintf(NEW_END,wm->sign,sSend,sDraft);
     }
       delete t;

       MyUnlock(wbmMutex);

       break;

     case 17:  // Save draft
     case 7:  // Send msg
       //wm->
       SendMsg(wm->fromm,wm);
       //wm->S_printf(  sMSGSENT );
    // };
       //break;
//     case 6:
//       if(++wmlg_cntr&1) goto lbLogOut;
//     case 1:
        if(0){
     case 22:
         wm->DelOne();
         if(0){
     case 21:
         wm->CheckDel();
         if(0){
     case 8:
         wm->CrDir();
         }}};

     default:
         wm->OutMbox();
//         break;

//printf(HDR_OPT,from,fromm,reply,reto,hello,tmpl,sign,sign);
;

    };



 // }

  wm->bfl.fflush();
exPoint:
  if( (req_var!= (char**)  &NullLong))delete req_var;
  if(rpost && rpost!=pst)delete rpost;
  if(action<64)HTMLOutBottom( wm->bfl.bfr);
  return ret;
}

void ConvertBQ(char *from);

void WMail::CrDir()
{
 char tbb[512];
 char *t;
 if( (t=GetVarS(r->req_var,"t"))  && (CheckBadName(t)) )
 {
  sprintf(tbb,"%s" FSLUSHS "mbox"   FSLUSHS "%.128s",mbox,t);
  S_printf((char *) (CreateDirectory(tbb,&secat)?"<b>Created...</b>":"<b>Unable to create %.128s</b>"),t );
 }

}

void WMail::ChPwd(User *usr)
{
  int i,j;
  char *t,*p,*d;
  User * tuser=0,*pu;
     if( (t=GetVar(r->req_var,"p") ) )
     {
       i=strlen(t);
       d=usr->dir(p=usr->pasw());

       if( i> strlen(p) )
       {
         tuser=(User *) new char[sizeof(User )+i+strlen(d)+(j=p-usr->name)+3 ];
         memcpy(tuser,usr,sizeof(User)+j );
         p=tuser->name+j;
         usr->state=0;
         tuser->next=userList;
         userList=tuser;
       }
       strcpy(p,t);  //sprintf(p,"%s;",t);
       strcpy(p+i+1,d);
       SaveCfgFile(bbfr);
       S_printf( s_PWDChanged );

       return;
     }
 //    S_printf( s_NEWPWD );

};


void WMail::OutMbox()
{WIN32_FIND_DATA fnds;
 char tbb[512];
 char *bb;
 ulong *d;
 int h;
 ulong i,n,j;
 char **ch;
 char *from,*to,*dat,*subj,*t,*fromemail;
 HANDLE hdl;
 // POP3Ac *pc;
 ulong *pl;

  // debug("XDD5 %X |%.8s|",anFnd[0],anFnd[0]);


 S_printf(
 "Mailbox / <a href=\"javascript:SHTbl('t2');\"><span id=lt2 >[+]</span></a><div id=t2 style=\"display:none;\"><form name=y> <input type=hidden name=s value=8 > <input type=text name=t size=32 maxlength=64> <input type=submit value='Create Folder' > </form></div>  <br> <br> <form name=x %s",
  //   "action=" SCRIPTNAME
     s_METHOD_P
 );


 /*
 sprintf(tbb,"%s" FSLUSHS "mbox%.128s" FSLUSHS "*",mbox,subdir);
 if( ( hdl=FindFirstFile(tbb,&fnds) )!=INVALID_HANDLE_VALUE )
 {
   do{



   }while(FindNextFile(hdl,&fnds));


 }
*/

   //debug("XDD6 %X |%.8s|",anFnd[0],anFnd[0]);


// sprintf(tbb,"%s" FSLUSHS "mbox%.128s" FSLUSHS  "*.msg",mbox,subdir);
 sprintf(tbb,"%s" FSLUSHS "mbox%.128s" FSLUSHS  "*",mbox,subdir);

 //debug("mb=%s",bb);

 n=0;
 if( ( hdl=FindFirstFile(tbb,&fnds) )!=INVALID_HANDLE_VALUE )
 {d=(ulong *)malloc(0x2008*4*4);
  bb=(char *)(d+0x2008*3);
  do{
     if(fnds.cFileName[0]!='.' && (fnds.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) )
     {
       S_printf("<a href='%s%s/' >[%s/]</a> &nbsp; ",subdir[0]?"": SCRIPTNAME "/"  ,fnds.cFileName,fnds.cFileName);
     }
     else if(DWORD_PTR(fnds.cFileName[8])=0x67736D2E x4CHAR(".msg")   )
     {

   //  debug("mb=%s %s",bb,fnds.cFileName);

 //    i=((dwHighDateTime-0x1C075E1)<<)|((dwLowDateTime-)>>);
 //400*365.25*24*3600*1e7 >>32
     i=FileTime2time(fnds.ftCreationTime); //time(0);
//*
     for(j=n;j;--j)
     {if(i<d[j-1])break;
      d[j]=d[j-1];
      d[j+0x2000]=d[j+0x1FFF];
      d[j+0x4000]=d[j+0x3FFF];
     }
// */ j=n;
     d[j]=i;
     d[j+0x2000]=atouix(fnds.cFileName);
     d[j+0x4000]=fnds.nFileSizeLow;
     if(n++ >= 8191) break;
    }
   }while(FindNextFile(hdl,&fnds));
   FindClose(hdl);

  //debug("XDD7 %X |%.8s|",anFnd[0],anFnd[0]);

     //   debug("n=%u",n);

   for(i=0;i<n;++i)
   {sprintf(bb,"%s" FSLUSHS "mbox%.128s" FSLUSHS  "%8.8X.msg",mbox,subdir,d[i+0x2000]);
    //debug("XDD8 %X |%.8s|",anFnd[0],anFnd[0]);

    if((h=_lopen(bb,0))>=0)
    {j=_hread(h,bb,4096);
     bb[j]=0;
     _lclose(h);
       // debug("XDD9 %X |%.8s|",anFnd[0],anFnd[0]);

     if((from=stristr(bb,"\nFrom:"   )))from+=sizeof("\nFrom:"   ); else from=".";
     if((to  =stristr(bb,"\nTo:"     )))to  +=sizeof("\nTo:"     ); else to  =".";
     if((dat =stristr(bb,"\nDate:"   )))dat +=sizeof("\nDate:"   ); else dat =".";
     if((subj=stristr(bb,"\nSubject:")))subj+=sizeof("\nSubject:"); else subj=".";
     if((t=strchr(from,'\n'))){*t=0; /* if( (t-from)>40)DWORD_PTR(from[38])=0x00002E2E x4CHAR("..") ; */

      ConvertBQ(from);
     // debug("XDD10 %X |%.8s|",anFnd[0],anFnd[0]);


     }
     if((t=strchr(to  ,'\n'))){*t=0; /* if( (t-to  )>40)DWORD_PTR(to  [38])=0x00002E2E x4CHAR("..") ;*/
       ConvertBQ(to);

     }
     if((t=strchr(dat ,'\n'))){*t=0; if( (t-dat )>64)DWORD_PTR(dat [62])=0x00002E2E x4CHAR("..") ; }
     if((t=strchr(subj,'\n')))
     {
       *t=0;
       ConvertBQ(subj);
       if( (t-subj)>64)DWORD_PTR(subj[62])=0x00002E2E x4CHAR("..") ;
     }


     if((fromemail=strchr(from,'<')))
     {*fromemail++=0;
      if((t=strchr(fromemail,'>'))) *t=0;
     }else{fromemail=from;  from=""; }

     t=to  ; while( (t=strpbrk(t,"<>")))*t=' ';
     t=dat ; while( (t=strpbrk(t,"<>")))*t=' ';
     t=subj; while( (t=strpbrk(t,"<>")))*t=' ';

   //  debug("XDD11 %X |%.8s|",anFnd[0],anFnd[0]);

     S_printf(
//       "<tr>"
//          //"<td><font size=2><B><a href=\"" SCRIPTNAME s_OPEN000 LF
//       "<td><font size=2><input type=checkbox name=\"dl%X\" > </td>" LF
//        s_TD__FONT  LF
//       "</table></td></tr>"
      //"<td><font size=2>"
       s_TD__FONT
      ,d[i+0x2000],d[i+0x2000],subj,
      from,fromemail,fromemail,to,
      dat,d[0x4000+i]);

     // debug("XDD12 %X |%.8s|",anFnd[0],anFnd[0]);

#if 0
     if(strin(bb,"Received: via pop;") )for(pc=POPlist;pc;pc=pc->next)
     {for(pl=pc->msgs;*pl;++pl)
      {if(!*++pl)break;
       if(*pl==d[i+0x2000])
       {if(!(t=strchr(bb+sizeof("Received: via pop;"),';'))){break; }
        *t=0;
        S_printf( s_B__S__B_ ,
                   bb+sizeof("Received: via pop;")-1 ,pl[-1]);
        j=atoui(t+1);
        if(j>d[i+0x4000])
          S_printf( s_BR__INPU ,pl[-1],j/1024);
        goto exlp2;
       }
      }
     }
#endif
  exlp2:
     //S_printf(" &nbsp;</td>" LF  "</tr>" LF)
                     ;

    }
   }
 //  debug("XDD14 %X |%.8s|",anFnd[0],anFnd[0]);


   free(d);
 };
 S_printf(
 "</table>"
 " <input type=submit  value=\""
  "%s" //   sDelete
  "\""

" ></form><br>",sDelete
         );

     //      debug("XDD15 %X |%.8s|",anFnd[0],anFnd[0]);

 /*
"<table width=100%%> " LF
"<tr valign=center align=left>"LF
 s_TD_BGCOL LF
 s_TD_BGCOL0 LF
 s_TD_BGCOL1 LF
"<td bgcolor=#E8E0E8><font size=1> "LF
 s__INPUT_T LF
 s__INPUT_T0 LF
 s__INPUT_T1 LF
 s__INPUT_T2 LF
"</td> "LF
 s_TD_BGCOL2 LF
 s_OTHERWIS LF
"<br>"

"<script language=JavaScript>//<!--\n"
"var srv=new Array(''");

  for(pc=POPlist;pc;pc=pc->next)S_printf(",'%s'",pc->server);
 S_printf(",'');\n"
"var usr=new Array(''");
 for(pc=POPlist;pc;pc=pc->next)S_printf(",'%s'",pc->user);
 S_printf(",'');\n"
"var pwd=new Array(''");
 for(pc=POPlist;pc;pc=pc->next)S_printf(",'%s'",pc->password);
 S_printf(",'');\n"
"var flg=new Array(0xF0000");
 for(pc=POPlist;pc;pc=pc->next)S_printf(",%u",pc->flags<<16|pc->size);
 S_printf( s_____N___
 );
 for(j=1,pc=POPlist;pc;pc=pc->next)S_printf( s_OPTION_V ,j++,pc->server,pc->user);

 S_printf("</select>"
 "</td>"LF
"</tr>"LF
 s__TABLE__ LF
);
*/


}


void  //WMail
 Req::SendMsg(char *fromm,WMail *wm)
{
 SYSTEMTIME  stime;
 ulong i,lm1,lm2,j;
 char *t,*p,*d;
 int h;
 t=new char[0x1400];

  GetSystemTime(&stime);
  i=GetMsgName(stime);
  sprintf(t,"%.255s" FSLUSHS "%8.8X.msg",out_path,i);
  d=GetVarS(req_var,"s");
//  d=
  j=0;
  while(j<2)
  {
    if(d[0]=='1')
    {
     if(!loc_draft)loc_draft="draft";
     sprintf(t,"%.255s" FSLUSHS "mbox" FSLUSHS "%.255s" FSLUSHS "%8.8X.msg",wm->mbox,loc_draft,i);

//debug("draft: %s",t);
    }
    else j=2;
    if((h=_lcreat(t,0))>0)break;
    if(j++) goto errLoc;
    sprintf(t,"%.255s" FSLUSHS  "mbox" FSLUSHS "%.255s" ,wm->mbox,loc_draft);
    CreateDirectory(t,&secat);
  }
  if(!h)
  {
   wm->S_printf(  sMSGSENTERR );
   goto errLoc;
   //return;
  }

  _hwrite(h,t,lm1=sprintf(t, sFROM__S__ ,
          fromm, GetVarS(http_var,"REMOTE_ADDR"),
          wkday[stime.wDayOfWeek],
          stime.wDay,
          month[stime.wMonth-1],
          stime.wYear,
          stime.wHour,
          stime.wMinute,
          stime.wSecond,
          GetVarS(req_var,"tomail"),
          GetVarS(req_var,"from"),
          fromm,
          GetVarS(req_var,"to"),
          GetVarS(req_var,"tomail"),
          GetVarS(req_var,"reply"),
          (*(p=GetVarS(req_var,"reto")))?p:fromm,

          wkday[stime.wDayOfWeek],
          stime.wDay,
          month[stime.wMonth-1],
          stime.wYear,
          stime.wHour,
          stime.wMinute,
          stime.wSecond,
          GetVarS(req_var,"subj")

          )
         );

   p=GetVarS(req_var,"msg");
  _hwrite(h,p,lm2=strlen(p));
  _lclose(h);
  if(d[0]=='1')
  {
   wm->S_printf(  sMSGSAVED );
  }
  else
  {
   wm->S_printf(  sMSGSENT );
   if(loc_sent)
   {
     j=0;
     while(j<2)
     {
      sprintf(t+lm1+1,"%.255s" FSLUSHS  "mbox"  FSLUSHS "%.255s" FSLUSHS "%8.8X.msg",wm->mbox,loc_sent,i);
      if((h=_lcreat(t+lm1+1,0))>0)break;
      if(j++) goto errLoc;
      sprintf(t+lm1+1,"%.255s" FSLUSHS  "mbox" FSLUSHS "%.255s" ,wm->mbox,loc_sent);
      CreateDirectory(t+lm1+1,&secat);
     }
     _hwrite(h,t,lm1);
     _hwrite(h,p,lm2);
     _lclose(h);
   }
  }
errLoc:;
  delete t;
  last_file=i;


//  printf( s_B_MESSAG ,
//          GetVarS("to"),GetVarS("tomail"));
};


// #ifdef  SYSUNIX
// extern char  tbl1[];
//
// #else
char       tbl1[]="бвчздецъйклмнопртуфхжигюыэ€щшьасЅ¬„«ƒ≈÷Џ… ЋћЌќѕ–“”‘’∆»√ёџЁяўЎ№ј—£≥\000"
"јЅ¬√ƒ≈∆«»… ЋћЌќѕ–—“”‘’÷„ЎўЏџ№Ёёяабвгдежзийклмнопрстуфхцчшщъыьэю€Є®"
    ;

// #endif

char* CopyQ(char *b,char *t)
{
 int n=0;
  while(*t)
  {if(*t=='=')
   {n=WORD_PTR(t[1]);
    if(n==0xD0A || n==0xA0D)
    {
      t+=3;
    }
    else if((*b=atouix((char*)&n)))
    {++b;
     t+=3;
    }else ++t;
   }else *b++=*t++;
  }
  *b=0;
  return b;
};

ulong D64X(uchar i);

#if 0
char* CopyB(char *y,char *t)
{uint i,j;
  while((i=D64X(*t))<64 /* &&   *t!='?' */)
  {t++;
   //if(*t=='?')break;
   j=D64X(*t);
   *y=(i<<2)|(j>>4);
   t++; y++;
   if( (i=D64X(*t))>=64  //|| *t=='?'
     )break;
   *y=(j<<4)|(i>>2);
   t++; y++;
   if( (j=D64X(*t))>=64 //|| *t=='?'
     )break;
   *y=(i<<6)|(j);
   t++; y++;
  }
  *y=0;
  return y;
};

#else
#define CopyB(a,b) Decode64(a,b,256)
#endif

char* CopyBB(char *y,char *t)
{int i,j;
  while(1)
  {
   while(*t=='\r' || *t=='\n' )t++;
   if((i=D64X(*t))>=64)break;
   do{t++;} while(*t=='\r' || *t=='\n' );
   j=D64X(*t);
   *y=(i<<2)|(j>>4);
   do{ t++; } while(*t=='\r' || *t=='\n' );
   y++;
   if( (i=D64X(*t))>=64 )break;
   *y=(j<<4)|(i>>2);
   do{ t++; } while(*t=='\r' || *t=='\n' );
   y++;
   if( (j=D64X(*t))>=64 )break;
   *y=(i<<6)|(j);
   t++;
   y++;
  }
  *y=0;
  return y;
};


void ConvertBQ(char *from)
{char *b,*t;
 int j;
 //debug("|%.64s|",from);
 if(WORD_PTR(*from)== 0x3F3D ) //  =? )
 {j= (DWORD_PTR(from[2])|0x202020)==0x38696F6B x4CHAR("koi8") ;
  if((t=stristr(from,"?B?")))
  { if((b=CopyB(from,t+3)))
    {
      if( (t=strpbrk(b+1,"< \t") ) )
      {
       *b=' ';
       strcpy(b+1,t);
      }
    }
  }
  else if((t=stristr(from,"?Q?")))CopyQ(from,t+3);
  if(j) for(b=from;*b;++b){if( t=strchr(tbl1,*b))*b=t[67]; };
 }
}


void OMsg::CheckDel()
{
 WIN32_FIND_DATA fnds;
 char bb[512],bb2[512],*t;
 HANDLE hdl;
 ulong j,k=0;
    sprintf(bb,"%.128s" FSLUSHS  "mbox%.128s" FSLUSHS   "*.msg",mbox,subdir);
    if( ( hdl=FindFirstFile(bb,&fnds) )!=INVALID_HANDLE_VALUE )
    {
      do{
       sprintf(bb,"dl%X",j=atouix(fnds.cFileName) );
       if(GetVar(r->req_var,bb))
       {
        sprintf(bb,"%.128s" FSLUSHS "mbox%.64s" FSLUSHS  "%8.8X.msg",mbox,subdir,j);
        if(loc_trash && ! strstr(subdir,loc_trash) )
        {

          do{
           sprintf(bb2,"%.128s" FSLUSHS "mbox" FSLUSHS "%.64s" FSLUSHS  "%8.8X.msg",mbox,loc_trash,j);
           if(MoveFile(bb,bb2)) break;
           if(k++) break;
           sprintf(bb2,"%.128s" FSLUSHS "mbox" FSLUSHS "%.64s" ,mbox,loc_trash);
           CreateDirectory(bb2,&secat);
          }while(k<2);
        }
        else DeleteFile(bb);
       }
      }while(FindNextFile(hdl,&fnds));
      FindClose(hdl);
    }
 /*
   if((t=GetVar("sl")))
   {

    if((j=atoui(t)))
    {
      GetPop();
    }
   }
    */
};

void OMsg::DelOne()
{
 char *t;
 ulong j,k=0;
 char bb[520];
 char bb2[520];
  if( (t=GetVar(r->req_var,"r")))
  {
   j=atouix(t);
   sprintf(bb,"%s" FSLUSHS "mbox%.128s" FSLUSHS  "%8.8X.msg",mbox,subdir,j);
   if(loc_trash && ! strstr(subdir,loc_trash) )
   {
     do{
           sprintf(bb2,"%.128s" FSLUSHS "mbox" FSLUSHS "%.64s" FSLUSHS  "%8.8X.msg",mbox,loc_trash,j);
           if(MoveFile(bb,bb2)) break;
           if(k++) break;
           sprintf(bb2,"%.128s" FSLUSHS "mbox" FSLUSHS "%.64s" ,mbox,loc_trash);
           CreateDirectory(bb2,&secat);
      }while(k<2);
   }
   else DeleteFile(bb);
  }
};


#define bb_SIZE 0xF00
int OMsg::Load()
{
 char bb[bb_SIZE]; //,*bfr;
 char *t,*b,*koi,*b64;
 ulong n;
 int h,j;
 end=0;
 if(!(t=GetVar(r->req_var,"r")))goto err;

 sprintf(bb,"%s" FSLUSHS "mbox%.128s" FSLUSHS  "%8.8X.msg",mbox,subdir,m=atouix(t));

 if((h=_lopen(bb,0))<0)
 {
 err:
  S_printf( s_H__BAD_M );
  debug("Can't open %s  (%s)",bb,subdir);
  return 0;
 }

 n=FileSize(h);
 //    _llseek(h,0,2);
 //_llseek(h,0,0);
 if(!n)
 {
  _lclose(h);
  return 0;
 }

  bfr=new char[n+30];

  j=_hread(h,bfr,n);
  DWORD_PTR(bfr[j])=0;
  _lclose(h);
  if(j<=0)
  {
    delete bfr;
    return 0;
  }
#define BOUNDARY "boundary="

   if((from=stristr(bfr,"\nFrom:"   )))from+=sizeof("\nFrom:"   ); else from=".";
   if((to  =stristr(bfr,"\nTo:"     )))to  +=sizeof("\nTo:"     ); else to  =".";
   if((dat =stristr(bfr,"\nDate:"   )))dat +=sizeof("\nDate:"   ); else dat =".";
   if((subj=stristr(bfr,"\nSubject:")))subj+=sizeof("\nSubject:"); else subj=".";
   b=0;

   if((boundary=stristr(bfr,BOUNDARY)))
   {boundary+=sizeof(BOUNDARY)-1;
    if(*boundary=='\"')boundary++;
    if((t=strpbrk(boundary,"\"\r\n")))*t++=0;
    else txt= sBAD_MIME_ ;
    if(!(txt=strstr(t,boundary)))txt="";
    else
    {if( (t=strstr(txt,"\r\n\r\n") ) ){*t=0; t+=4;}
     else if( (t=strstr(txt,"\n\n") ) ){*t=0; t+=2;}
     else t=".";
   /*
   if( (strstr(txt,"text/plain"))  ) type=1;
   else
   */
     if( (strstr(txt," attachment"))) t=".";
     else if( (strstr(txt,"text/html"))  ) type=2;
     if(stristr(txt,"quoted-printable") )b=t;
  // koi=stristr(txt,"; charset=koi8-r");
     if(!(koi=stristr(txt,"charset=koi8-r")))
       koi=stristr(txt,"charset=\"koi8-r\"");
     b64=stristr(txt,"base64");
     if((t=strstr(txt=t,boundary)))
     {
     //if(t[-1]=='\n')--t;
     //if(t[-1]=='\r')--t;
      *t++=0;
     }
     end=t;
    }
   }
   else
   {if( (txt=strstr(bfr,"\r\n\r\n") ) ) {*txt=0; txt+=4;}
    else if( (txt=strstr(bfr,"\n\n") ) ){*txt=0; txt+=2;}
    else txt=".";
    b64=stristr(bfr,"base64");
    if( (strstr(bfr,"text/html"))  ) type=2;
    if(stristr(bfr,": quoted-printable") )b=txt;
//  koi=stristr(bfr,"; charset=koi8-r");
    if(!(koi=stristr(bfr,"charset=koi8-r")))
      koi=stristr(bfr,"charset=\"koi8-r\"");

   }
 //debug("koi=%X",koi);

   if(b64)CopyBB(txt,txt);
   if(b){
    CopyQ(b,b);
   }

   if((t=strchr(from,'\n'))){*t=0; if( (t-from)>64)DWORD_PTR(from[62])=0x00002E2E x4CHAR("..") ; }
   if((t=strchr(to  ,'\n'))){*t=0; if( (t-to  )>64)DWORD_PTR(to  [62])=0x00002E2E x4CHAR("..") ; }
   if((t=strchr(dat ,'\n'))){*t=0; if( (t-dat )>64)DWORD_PTR(dat [62])=0x00002E2E x4CHAR("..") ; }
   if((t=strchr(subj,'\n'))){*t=0; if( (t-subj)>64)DWORD_PTR(subj[62])=0x00002E2E x4CHAR("..") ; }

   if((fromemail=strchr(from,'<')))
   {*fromemail++=0;
    if((t=strchr(fromemail,'>'))) *t=0;
   }else{
    fromemail=from;
    from="";
   }
   if((to_email=strchr(to,'<')))
   {
    *to_email++=0;
    if((t=strchr(to_email,'>'))) *t=0;
   }else{
    to_email=to;
    to="";
   }

   ConvertBQ(to);
   ConvertBQ(from);
   ConvertBQ(subj);
   t=to ; while( (t=strpbrk(t,"<>\"")))*t=' ';
   t=from ; while( (t=strpbrk(t,"<>\"")))*t=' ';
   t=dat ; while( (t=strpbrk(t,"<>\"")))*t=' ';
   t=subj; while( (t=strpbrk(t,"<>\"")))*t=' ';
   if(koi)for(b=txt;*b;++b){if( t=strchr(tbl1,*b))*b=t[67]; };

   return 1;

}


void OMsg::OutQV(char *txt,char *sf)
{
  char *t,*tl,*t1;
  int cc;
   t=txt;
   do{
    tl="";
   l1:
    if((t=strpbrk(t,sf)))
    {
    /*
     if(*t=='<') tl="&lt;";
     else if(*t=='>') tl="&gt;";
     else if(*t=='&') tl="&amp;";
     else if(*t=='\n') tl="<br>";
     else if(*t==' ' && t[1]==' ') tl=" &nbsp;";
     else {++t; goto l1;}
     */
     switch(*t)
     {
      case '<': tl="&lt;"; break;
      case '>': tl="&gt;"; break;
      case '&': tl="&amp;"; break;
      case '\n': tl="<br>"; break;
      case 'h':
          if( strin(t,"http://") && (t1=strpbrk(t," \r\n\t'\"")) )
          {
            cc=*t1;
            *t1=0;
            *t=0;
            S_printf("%s<a href=\"h%s\" >h%s</a>",txt,t+1,t+1);
            *t1=cc;
            txt=t=t1;
            goto l1;


      case ' ': if( t[1]==' ') { tl=" &nbsp;"; break; }
          }
      default: ++t; goto l1;


    };
    *t++=0;
    }
    S_printf("%s%s",txt,tl);
   }while((txt=t));
}

void OMsg::OutMSG()
{char *t,*tl;
 int j;

 if( Load() )
 {
  S_printf( s_TABLE_WI , from,fromemail,to,to_email,
            dat,subj);

  if(type==2)OutQV(txt,"<>&");
     //S_printf("%s",txt);
  else
  {//S_printf("<pre>");
    OutQV(txt,"<>&h\n ");
   //S_printf("</pre>");
  };
 OutAtth();

 S_printf(
"</td></tr><tr><td align=left>"
"<table><tr><td align=left width=20%%>"
 "[<a href=\"" SCRIPTNAME "%s/%X.msg?s=64&r=%X\">"

  "%s" //  sDownload_source

     "</a>] &nbsp; "
 "</td>"
     "<td align=right > "
//"[<a href=\"/" SCRIPTNAME "/?s=&r=%X&\">Reply</a>] &nbsp; "

// "<form  action=" SCRIPTNAME  s_METHOD_G  SCRIPTNAME  s_METHOD_G1 SCRIPTNAME  s_METHOD_G0
"<form  "  "%s %s %s" // s_METHOD_G   s_METHOD_G1  s_METHOD_G0

     ,subdir,m,m,m,subj,from,fromemail,dat,m,subj,to,to_email,dat,m
     ,sDownload_source,s_METHOD_G,   s_METHOD_G1,  s_METHOD_G0 );

  delete bfr;
 }
}


int WMail::OutMSGSrc()
{

 char bb[bb_SIZE + 4]; //,*bfr;
 char *t,*b;
 ulong n;
 int h,j;

 if(!(t=GetVar(r->req_var,"r")))
 {
  return -1;
 }

 sprintf(bb,"%s" FSLUSHS "mbox%.128s" FSLUSHS  "%8.8X.msg",mbox,subdir,m=atouix(t));

 if((h=_lopen(bb,0))<0)
 {
 // debug("Cant open %s",bb);
  return -1;
 }
 n=FileSize(h);
 if(n)
 {
  S_printf(BinHead,n);
  bfl.fflush();
  do{
   n=_hread(h,bb,bb_SIZE);
   if(n<=0)break;
   n=r->Send(bb,n);
  }while(n==bb_SIZE);
 }
 _lclose(h);
 return 0;
}

char *EEname(char *t,char *s,int l=1024)
{
 int i=0,j;
 char *e=t+l-3;

  while( (j=*(uchar *)s) )
  {
    if( j>128 || strchr(" ?=&+\t%",j) )
    {  t+=sprintf(t,"%%%2.2X",j);  }
    else
    { *t++=j;  }
    if(t>=e)break;
    s++;
  }
  *t=0;
  return t;
};

void WMail::OutAtth()
{
 char bb[1100];
 char b1[1100];
 //debug("end && boundary |%.230s| |%.30s|",end, boundary);
 if(end && boundary)
 {
  char *s;
  char *p,*t,*e;
  int c;
  s=end;
   while( (p=strstr(s,boundary+1) )  )
   {
   // debug("1 %X %X",p,s);
    if(!(t=strstr(p,"\r\n\r\n") )) break;
    *t=0;
   // debug("2 %s",p);
    if( (p=strstr(p,"filename=") ) )
    {

     p+=sizeof("filename=")-1;
   //  debug("3 %s",p);

     if(*p=='\"')
     {
      if(!(e=strchr(++p,'\"')))break;
     }
     else
      if(!(e=strpbrk(p," \t\"\r\n")))break;
     c=*e;
     *e=0;
     EEname(bb,p);
     ConvertBQ(p);
     EEname(b1,p);
     S_printf("<br>Atachment:<a href='" SCRIPTNAME "%s/%s?s=65&r=%X&a=%s'>%s</a>",subdir,b1,m,bb,p);
     *e=c;
    }
    *t='\r';
    s=t+4;
   }
 }
}

int WMail::OutAtthRA()
{
 char *s, *n;
 char *p,*t,*e,*s1,*b64;
 int c,a;
// debug("1");
 if(!(n=GetVar(r->req_var,"a")) )return -1;
// debug("2 %s",n);
 if(!Load())return -1;
// debug("3");

 s1=s=end;

 while( (p=strstr(s,boundary+1) )  )
 {
  if(!(t=strstr(p,"\r\n\r\n") )) break;
  *t=0;
  b64=stristr(p,"base64");
//   debug("2 %X",b64);
  if( (p=strstr(p,"filename=") ) )
  {
   p+=sizeof("filename=")-1;
   if(*p=='\"')
   {
    if(!(e=strchr(++p,'\"')))break;
   }
   else
    if(!(e=strpbrk(p," \t\"\r\n")))break;
   c=*e;
   *e=0;
   a=strcmp(p,n);
   *e=c;
   if(!a )
   {
    t+=4;
    if(!(e=strstr(t,boundary)))break; //return -1 ;
    if(b64 )
    { *e=0;
     // debug("11 %.10s",t);
      e=CopyBB(t,t);
    //  debug("12 %.10s",t);

    }
    S_printf(BinHead,a=e-t );
    bfl.fflush();
    r->Send(t,a);
    delete bfr;
    return 0;
   }
  }
  *t='\r';
  s=t+4;
 }
 delete bfr;
 return -1;

}

/*
void WMail::OutMSG()
{OMsg msg;
 if(msg.Load())msg.OutMSG();
};


void WMail::OutQuote()
{OMsg msg;
 if(msg.Load())msg.OutQuote();
};
*/



char * html_skip[]={"head","script","title","map","select",0};

char * html_lf[]={"br","p","div","tr","h1","h2","h3","h4","h5","h6","center",0};

char *Andcnst[]={"lt","gt","amp","quot",0};
char Andsmbl[]="<>&\" ";
char* isTag(char *s, char **tag_list)
{char *o,v;
 while(strchr(" \r\n\t",*s))if(!*++s)return 0;
 if(!(o=strpbrk(s," \r\n\t>=-+|?/\%#&.,")))return 0;
 v=*o;
 *o=0;
 while(*tag_list)
 {if( !stricmp(s,*tag_list) )
  {*o=v;
   return *tag_list;
  }
  tag_list++;
 }
 *o=v;
 return 0;
}

char* html2text(char *t, char *s)
{char *p;
 int i;
 while(*s)
 {
  switch(*s)
  {
   case '\n':
   case '\r':
   case '\t':
   case ' ':
        if(t[-1]!=' ')*t++=' ';
        s++;
        break;
   case '<':
         if(DWORD_PTR(*s)==0x2D2D213C x4CHAR("<!--")  )
         {
          if(! (s=strstr(s,"-->") ) ) return t;
          s+=3;
          break;
         }
         if(!*++s)return t;
         if( (p=isTag(++s,html_skip )))
         {
           while( (s=strchr(s,'<')))
           {
             while(strchr(" \r\n\t",*s))if(!*++s)return t;
             if(*s=='/')
             {
              if( isTag(++s,html_skip )==p) goto endtag;
             }

           }
           return t;

         }
     endtag:
         if(!(s=strchr(s,'>')))return t;
         s++;
         break;
     case '&':
         if((p=strchr(s,';')))
         {
          *p=0;
          ++s;
          if(*s=='#')
          {
           *t++=atouix(s+2);
          }
          else{
            i=0;
            while( Andcnst[i] && stricmp(Andcnst[i],s) )++i;
            *t++=  Andsmbl[i];
          }
          *p=';';
          s=p+1;
          break;
         }
   default: *t++=*s++;
  }
 };
 return t;
};


#if 0
void OMsg::OutMSGEdt()
{
 char qq[8];
 char *t,*tt,*x,c,*y;
 int i;
 if(type==2)html2text(txt,txt);


#if 0
  i=0;
 t=from;
 if(*from)
 {
  do{
   while(*t==' ' || *t=='`' || *t=='\'' )++t;
   qq[i++]=*t;
  }while(t=strchr(t,' '));
  qq[i]=0;
 }else
 {qq[0]=fromemail[0];
  qq[1]=0;
 }

 tt=txt;
 do{

  if( (t=strchr(tt,'\n') ) )*t++=0;
  if( (x=strchr(tt,'>'))
      // || (x=strstr(tt,"&gt;"))
      // && ((!(y=strstr(tt,"&gt;"))) || y>x)
    )
  {
   c=*x;
   *x=0;
   S_printf("%s&gt;%s\n",tt,x+1);
   *x=c;
  }
  else
  {
    while( strlen(tt) > 64 )
    {
     for(x=tt+64;x>tt;--x)
     { if(*x==' ')
       {
       lb2:
        *x=0;
        S_printf( "%s\n",tt);
        *x++=' ';
        tt=x;
        goto exLp2;
       }
     }
     for(x=tt+64;*x;++x) if(*x==' ')goto lb2;

     break;
     exLp2:;
    }
    S_printf( (*tt)? " %s&gt; %s\n":"\n",qq,tt);
  }
 }while(tt=t);
#else
     S_printf( "%s\n",txt);
#endif


};

#endif


void OMsg::OutQuote()
{
 char qq[8];
 char *t,*tt,*x,c,*y;
 int i;
 if(type==2)html2text(txt,txt);
 i=0;
 t=from;
 if(*from)
 {
  do{
   while(*t==' ' || *t=='`' || *t=='\'' )++t;
   qq[i++]=*t;
  }while(t=strchr(t,' '));
  qq[i]=0;
 }else
 {qq[0]=fromemail[0];
  qq[1]=0;
 }

 tt=txt;
 do{

  if( (t=strchr(tt,'\n') ) )*t++=0;
  if( (x=strchr(tt,'>'))
      // || (x=strstr(tt,"&gt;"))
      // && ((!(y=strstr(tt,"&gt;"))) || y>x)
    )
  {
   c=*x;
   *x=0;
   S_printf("%s&gt;&gt;%s\n",tt,x+1);
   *x=c;
  }
  else
  {
    while( strlen(tt) > 64 )
    {
     for(x=tt+64;x>tt;--x)
     { if(*x==' ')
       {
       lb2:
        *x=0;
        S_printf( " %s&gt; %s\n",qq,tt);
        *x++=' ';
        tt=x;
        goto exLp2;
       }
     }
     for(x=tt+64;*x;++x) if(*x==' ')goto lb2;

     break;
     exLp2:;
    }
    S_printf((char *) ((*tt)? " %s&gt; %s\n":"\n"),qq,tt);
  }
 }while(tt=t);


};


struct  MCfgParam
{
 char *name;
 uint min,max;
 char *    def;
 int offs;
//  char  *desc;
  union{
 uint *v;
 char **vv;
  };


 int  HTMLFormString(char *bfr);
 int  TextCfgString(char *bfr);
 void FindConfig(char *s,void *base);
 void FindEndConfig();
// void Default();
 void Default(void *base)
 {
  v=(uint *) (((char *)base)+offs);
#ifdef A_64
  *vv= def;
#else
  *v=(uint) def;
#endif
//  debug("Default: %s = %.8s %.8s",name,def,*v);
 }


};


#define WMCFG(a,n,x,d,v) {a,n,x,(char *)(d), offset(WMail,v) }
//#define WMCFG(a,n,x,d,v) {a,n,x,(int)(d), offset(WMail,v) }

MCfgParam  wmcfg[]=
{
  WMCFG("from",0,0,"",from ), // ,"Your name:"),
  WMCFG("fromemail",0,0,"",fromemail), //,"From E-mail"),
  WMCFG("hello",0,0,shello,hello), //,  "Message template  () :"),
  WMCFG("tmpl",0,0,stmpl,tmpl), //,"Reply template:"),
  WMCFG("sign",0,0,ssign,sign), //,"Signature"),
 // WMCFG("del",0,0,del,del),
  {0,0,0,0}
};

void WMail::UpdUserOpt()
{
  int i;
  char *t;
   for(i=0; wmcfg[i].name ; i++)
   {
     if( (t=GetVar(r->req_var,wmcfg[i].name)) && *t ) *(wmcfg[i].vv) = t;
   }
}
char * WMail::LoadUserOpt(User * usr)
{
 char bb[1024];
 int h,i,j;
 char *t,*tt;
 sprintf(bb,"%s" FSLUSHS "wmail.cfg" , mbox);
 MyLock(wbmMutex);
 if((h=_lopen(bb,0))>=0)
 {
  j=FileSize(h);
  t=new char [j+0x10008];
  tt=t+0x10000;
  j=_hread(h,tt,4096);
  tt[j]=0;
  _lclose(h);
  for(i=0; wmcfg[i].name ; i++)
    wmcfg[i].FindConfig(tt,this);

  for(i=0; wmcfg[i].name ; i++)
   if(! wmcfg[i].max) wmcfg[i].FindEndConfig();

 }
 else
 {
   t=new char [0x10000];
   for(i=0; wmcfg[i].name ; i++)
     wmcfg[i].Default(this);



 }
 if(! (fromemail[0]) )
    fromemail=fromm;


 return t;



};




void MCfgParam::FindConfig(char *s,void *base)
{char bfr[128],*t;
 uint i;
  v=(uint *) (((char *)base)+offs);
  i=sprintf(bfr,"<%s>",name);
  if( (t=stristr(s,bfr)) )
  {if(max)
   {if( (i=atoui(t+i))<min ) i=min;
    if( i>max ) i=max;
    *v=i;
   }
   else *vv=(t+i);
  }
  else *vv= def;
};

void MCfgParam::FindEndConfig()
{char bfr[128],*t;
 if(!max)
 {sprintf(bfr,"</%s>",name);
  if( vv && (t=stristr( *vv,bfr)) ) *t=0;
 }
};

#ifdef SYSUNIX
void WMail::CHUsr(int h)
{
 struct passwd *pwd,*upwd;
 struct group  *grp;
 char *uname;
  if( (upwd=getpwnam(uname=puser->name)) || (upwd=getpwnam(uname="ftp")) )
  {
       if(upwd->pw_uid>0)fchown(h,upwd->pw_uid,upwd->pw_gid);
  }
}
#endif

void  WMail::SaveAll()
{
 char *p;
 union {
  char *t;
  ulong *tl;
 };
 ulong *pl;
 int h,i;
 MCfgParam *pcp;
 char bb[1024];
 int j;
 sprintf(bb,"%s" FSLUSHS "wmail.cfg" , mbox);

// debug("save file %s<br>",fname);

 if((h=_lcreat(bb,0))<0) return;
#ifdef SYSUNIX
   CHUsr(h);
#endif

 t=p=new char[0x8000];

 for(pcp=wmcfg;pcp->name;++pcp)
  t+=sprintf(t,"<%s>%.2048s</%s>\n",pcp->name,*(pcp->vv),pcp->name);

/*
 t+=sprintf(t,"\n\n# Doesn't edit next lines manualy\n ");


 for(pc=POPlist;pc;pc=pc->next)
 {if((i=p-t)>0x10000)
  {_hwrite(h,p,i);
   t=p;
  }

  t+=sprintf(t,POP3_STR "%.64s;%.64s;%.64s;%u;%u",
              pc->server,pc->user,pc->password,pc->size,pc->flags
            );
  for(pl=pc->msgs;*pl;++pl){t+=sprintf(t,";%u",*pl);}
  t+=sprintf(t,POP3_STRE "\n");
 }
// debug("save file end<br>");
*/
 _hwrite(h,p,t-p);
 _lclose(h);
 delete p;
}
//=====================================



char *lstname[]=
{
"Badlist",
"Goodlist",
"Graylist",
"Forward",
"Hosts definetion file",
"Text configuration file"

};

char *lstfname[]=
{
"badlist",
"goodlist",
"graylist",
"forward",
"hostdef",
#ifdef SYSUNIX
"httpd.cfg"
#else
"http.cfg"
#endif
 //,0
};

char **advname[]=
{
 &blst,
 &glst,
 &graylst,
 //0,
 &antiv,
 &dns_file,
 &conf_name
 //,0
};

char *lstfmt[]=
{
 llist,llist,llist,HdForward//,HdForward
     ,ns_lst, "<br>This is an easy way to copy configuration from one server to another, but be careful. Restart the server after saving the file for the changes to take effect."   //conf_htm
};

char *lstfmtend[]=
{
 llist_end,llist_end,llist_end,HdForwardEnd,
     //,HdForwardEnd,
 ns_lst_end, conf_htm_end
};
char *lstfmtend2[]=
{
 LLEND,LLEND,LLEND,sFORWARD2,
 "", ""
};

char *lstdesc[]=
{
#ifdef RUS
"—ообщени€ от отправителей из этого списка не будут приниматьс€",
"—ообщени€ от отправителей из этого списка всегда будут приниматьс€.",
"ѕеред приемом сообщени€, сервер попытаетс€ дополнительно проверить отправителей подподающих под этот список.  —ообщение не будет приниматьс€ немедленно, но веро€тно будет прин€то когда отправитель попытаетс€ послать его еще раз.",
"ƒл€ проверки, пернаправлени€, или удаление€ сообщений после приема.",
//"‘айл дл€ проверки сообщени€ после приема (поиска вирусов, обнаружени€ спама, и т.д...) .",
"‘айл описани€ хостов",
"ћожно редактировать текстовой файл конфигурации вручную, но потребуетс€ перезапуск дл€ прин€ти€ изменений"

#else
"Messages from senders from this list will be declined",
"Messages from sender from this list will be always received.",
"Server try to check senders from this list, before receive message.  Message will not receive immidiatle, and probably will be received when remote try send it again.",
"File for checking, redirecting, or removing message after receiving.",
//"File for check message after receiving (find viruses, detect spam, etc...) .",
"Host definetion file",
"You may edit the text configuration file manualy, but restart required, to accept the changes."
#endif
};




void WMail::OutList(int nn)
{
 char bb[512];
 char *tt;
 int l,i,j,h,n;
 n=nn&0x3F;
 //debug("%u %u %X",n,nn,*advname[n]);

 DBGLA("Edit file: %d %d", n, nn);
 S_printf((nn&0x80)? alist_comon: llist_comon ,nn+31,lstname[n],lstdesc[n] );
 S_printf(lstfmt[n]);
 S_printf(
"</ul> " LF
"</ul> " LF
"<br><br><center><textarea name=v rows=30 cols=80 >");
 if(nn&0x80)
 {

   DBGLA("Edit file: %d %d %lX '%s'", n, ARRAY_SIZE(advname), *(advname[n]), *(advname[n]));

  if(n >= ARRAY_SIZE(advname) || ! *(advname[n]) ) goto lbEndListOut ;
  sprintf(bb,"%s", * (advname[n]) );
 }
 else sprintf(bb,"%s" FSLUSHS "%s" , mbox,lstfname[n]);

 DBGLA("Edit file: %s", bb);

 if((h=_lopen(bb,0))>=0)
 {
  j=FileSize(h);
  if(j)
  {
   tt=new char [j+0x800];
   j=_hread(h,tt,j);
   tt[j]=0;
   OutQV(tt,"<>&");
   delete tt;
  }
  _lclose(h);
 }
// else {   tt=""; }

lbEndListOut:

 DBGLA("elist")
 S_printf("</textarea>" LF
"<br>" LF );
 S_printf( lstfmtend[n], lstfmtend2[n], sSubmit ); // ,lstname[n],lstdesc[n],tt );

}

void WMail::SaveList(int nn)
{
 char bb[512];
 char *tt;
 int l,i,j,h,n,save=0;
 if(! (tt=GetVar(r->req_var,"v") )) return;
 n=nn&0x3F;

 //sprintf(bb,(nn&0x80)? * (advname[n]) : "%s" FSLUSHS "%s" , mbox,lstfname[n]);
 if(nn&0x80)
 {if(! *(advname[n]) )
  {
    *(advname[n])=new char[64];
    sprintf(*advname[n],"common_%s",lstfname[n]);
    ++save;
  }
  sprintf(bb,"%s", * (advname[n]) );
 }
 else sprintf(bb,"%s" FSLUSHS "%s" , mbox,lstfname[n]);

 if((h=_lcreat(bb,0))>=0)
 {
#ifdef SYSUNIX
   CHUsr(h);
#endif
   l=strlen(tt);
   _hwrite(h,tt,l);
   _lclose(h);
   S_printf("<b>File %s saved</b><br>",(nn&0x80)? bb:lstfname[n]);
   if(save && n!=30)
   {
     tt=new char[0x10000];
     SaveCfgFile(tt);
     delete tt;
   }
 }
 else
 {S_printf("<b>Can't save file %s</b><br>",(nn&0x80)? bb:lstfname[n]);
 }

}


