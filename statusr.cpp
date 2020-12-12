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

#ifndef STRING_CONST_H
#include "g4strc.h"
#endif


char *selc[2]={NullString,"selected"};

char* OutListDays(char * p, int d,int n)
{int i;
 p+=sprintf(p,"<select name=%c>",n);
 for(i=1;i<=31;++i)p+=sprintf(p,"<option %s>%u",selc[i==d],i);
 return p+sprintf(p,"</select> ");
};
char* OutListMonth(char * p, int d,int n)
{int i;
 p+=sprintf(p,"<select name=%c>",n);
 for(i=1;i<=12;++i)p+=sprintf(p,"<option value=%u %s>%s",i,selc[i==d],month[i-1]);
 return p+sprintf(p,"</select> ");
};
char *GetVarS(char **varlist,char *var)
{
 return (var=GetVar(varlist,var) )?var:NullString;
};

inline ulong GetVarI(char **varlist,char *var)
{return atoui(GetVarS(varlist,var) );}

StatLog *ParseFile(char *log,char *b) //=0)
{
 StatLog *tsl,*tsl1;
 char *t1,*t,*p;
 int n=0;
 if( (p=strstr(log,"\r\n!->") ) )
 {for(t1=t=p;t;)
  { ++n;
#ifdef x86_64
//   DDWORD_PTR(*t1)=(u_long) (t=strstr((t1=t)+1,"\r\n!->"));
   t=strstr((t1=t)+1,"\r\n!->");  
//   DWORD_PTR(*t1)=t? (ulong) (t-log):0;
   DWORD_PTR(*t1)=t? (ulong) (t-t1):0;
#else   
   DWORD_PTR(*t1)=(ulong) (t=strstr((t1=t)+1,"\r\n!->"));
#endif   
  }
  if(b)sprintf(b,"%14.14s -- %14.14s ; %u",p+5,t1+5,n);
  for(tsl=(StatLog *)p;tsl;
#ifdef x86_64
        tsl=//tsl->next=
            tsl->nexto(log)
#else
        tsl=tsl->next
#endif
     )tsl->Parse();
  return (StatLog *)p;
 }
 return 0;
};


const char *afHstStr[]=
{
#ifdef RUS
"HTTP по хостам. Входящий трафик (в байтах)",
"HTTP по хостам. Исходящий трафик (в байтах)",
#else
"HTTP by hosts. Incomming traffic (bytes)",
"HTTP by hosts. Outgoing traffic (bytes)",
#endif
0
};


const tfFind afHstFnd[]=
{&fFindHTTPInU,&fFindHTTPOutU,0};

const tfFind afUsrFnd[]=
{
&fFindProxyInU ,
&fFindProxyInUNC,
&fFindProxyOutU ,
&fFindFTPLogin,
&fFindFTPdownloadU,
&fFindFTPuploadU,
&fFindPOPLogin,
&fFindPOPx,
&fFindSMTPUser,
&fFindSMTPs,
&fFindSMTPr,
0};

const char* afUsrStr[]=
{
#ifdef RUS
 "Proxy. Запрошенный входящий трафик (в байтах)"
,"Proxy. Запрошенный трафик без учета возвращенного из кеша (в байтах)"
,"Proxy. Исходящий трафик (в байтах)"
,"FTP Login"
,"FTP скачено (в байтах)"
,"FTP залито (в байтах)"
,"POP Login"
,"SMTP вызовы"
,"SMTP трафик сообщений от пользователей (в байтах)"
,"SMTP трафик сообщений для пользователей (в байтах)"
,
#else
 sPROXY__RE
,sPROXY__IN
,sPROXY__OU
,sFTP_LOGIN
,sFTP_DOWNL1
,sFTP_UPLOA1
,sPOP_LOGIN
,sSMTP_CALL
,sSMTP_DATA0
,sSMTP_DATA1
,
#endif
0
};

const tfFind afIPFnd[]=
{
&fFindEventSumary,
&fFindHTTPUser,
&fFindHTTPIn,
&fFindHTTPOut,
&fFindProxyUser,
&fFindProxyIn  ,
&fFindProxyInNC,
&fFindProxyOut ,
&fFindFTPLogin,
&fFindFTPdownload,
&fFindFTPupload,
&fFindPOPLogin,
&fFindPOPx,
&fFindSMTPUser,
&fFindSMTPxIP,
&fFindSMTPs,
&fFindSMTPr,
&fFindDNSUser,
0};

const char *afIPStr[]=
{
#ifdef RUS
 "Сумарно по портам"
,"HTTP вызовы"
,"HTTP. Входящий трафик (в байтах)"
,"HTTP. Исходящий трафик (в байтах)"
,"Proxy вызовы"
,"Proxy. Запрошенный входящий трафик (в байтах)"
,"Proxy. Запрошенный трафик без учета возвращенного из кеша (в байтах)"
,"Proxy. Исходящий трафик (в байтах)"
,"FTP Login"
,"FTP. Утянули"
,"FTP. Залили"
,"POP Login"
,"POP трафик сообщений (в байтах)"
,"SMTP вызовы"
,"SMTP трафик сообщений (в байтах)"
,"SMTP трафик сообщений от пользователей (в байтах)"
,"SMTP трафик сообщений для пользователей (в байтах)"
,"DNS вызовы"
#else
 sSUMMARY_B
,sHTTP_CALL
,sHTTP__INC
,sHTTP__OUT
,sPROXY_CAL
,sPROXY__RE
,sPROXY__IN
,sPROXY__OU
,sFTP_LOGIN
,sFTP_DOWNL
,sFTP_UPLOA
,sPOP_LOGIN
,sPOP_DATA_
,sSMTP_CALL
,sSMTP_DATA
,sSMTP_DATA0
,sSMTP_DATA1
,sDNS_CALLS
#endif
,0

};


uint fFindJustRef(StatLog *psl,char *bfr)
{return IfFindSubStr(psl,bfr,"\r\nReferer: ");};

const char *afFileStr[]=
{
#ifdef RUS
"Загружен по HTTP (раз)",
"Загружен через proxy (раз)",
"Сылки c него (раз)",
"Загружен через FTP",
"Положен на FTP",
#else
"HTTP Downloaded",
"Proxy Downloaded",
"Links from",
"FTP Downloaded",
"FTP Uploaded",
#endif
0
};

const tfFind afFileFnd[]=
{&fFindURLHTTP,&fFindURLProxy,&fFindJustRef,&fFindFTPdownload,&fFindFTPupload,0};


const tfFind *StatFppFnc[4]=
{afIPFnd, afUsrFnd, afHstFnd, afFileFnd};


const char **StatFppStr[]=
{afIPStr,afUsrStr,afHstStr,afFileStr};





void LL2Lc(ulong long *pars,int n)
{
 while(n-->0)
 {if(*pars>0x40000000)
  {
   *pars>>=10;
   ((int *)pars)[1]='K';
  }
  else ((int *)pars)[1]=' ';
  ++pars;
 }
};

void Req::HTTPUserStat()
{
 char *nm,*bfr,*p,*t,*tt;
 const char **strs;
// ulong *dt;
 int typ,day2,mnt2,l,k,i,h;
 StatLog *tsl,*tsl1;
 uchar funcType[32];
 union {
 ulong long pars[88];
 char  *cpars[172];
 va_list  vl_pars;
 };
 ulong ip;
#define mnt1 (*(long*)pars)
#define day1 (*(long*)(pars+1))
 const tfFind *fpFnc;
 p=bfr=loc;
//  HTMLOutTop(s);
   memset(pars,0,sizeof(pars));
   nm=GetVarS(req_var,"h");
   typ=GetVarI(req_var,"t")&3;
   day1=GetVarI(req_var,"f");
   day2=GetVarI(req_var,"c");
   mnt1=GetVarI(req_var,"d");
   mnt2=GetVarI(req_var,"g");


 p+=sprintf(p,
"<form action=/$_admin_$susr method=get>"
"<center>"
"<h1> Summary statistics<br>"
"<select name=t>"
"<option value=0 %s>IP"
"<option value=1 %s>User"
"<option value=2 %s>Host"
"<option value=3 %s>File"
"</select> "
"<input type=text name=h value='%s'>"
"</h1>"


"Since:"
, selc[typ==0]
, selc[typ==1]
, selc[typ==2]
, selc[typ==3]
, nm);

 p=OutListDays(p,day1,'f');
 p=OutListMonth(p,mnt1,'d');
p+=sprintf(p," To:");
 p=OutListDays(p,day2,'c');
 p=OutListMonth(p,mnt2,'g');
p+=sprintf(p,"<br><br><input type=submit value=Show> <input type=reset></form><hr>");
 Send(bfr,p-bfr);
//*
 if( ! (s_flg&FL_LOGDAY) )
 {
#ifdef RUS
#undef MSG_LD
#define MSG_LD "<h2>Это работает только если опция <i>Новый log файл каждый день</i> выбрана</h2>"
#endif
  Send(MSG_LD,sizeof(MSG_LD)-1);
 }
//*/
 if(*nm)
 {
//  hdat=(mnt2*31+day2)*86400;
  ip=ConvertIP(t=nm);
  fpFnc=StatFppFnc[typ];
  strs=StatFppStr[typ];
  
//  dt=new ulong[12*32*4];
  p=bfr+sprintf(bfr,"<table bgcolor=#f8f8ff border=1><tr><td>Date</td>");
  k=0xC0C0E0;
  strcpy(bfr+0x9000,"<td>Total</td>");
  strcpy(bfr+0x8000,"<td>%2.2u%c/%2.2u%c</td>");
  tt=bfr+0x9000+sizeof("<td>Total</td>")-1;
  t=bfr+0x8000+sizeof("<td>%2.2u%c/%2.2u%c</td>")-1;
  for(i=0;strs[i];++i)
  {
    p+=sprintf(p,"<td bgcolor=#%x><font size=1 class=f3>%s</font></td>",k,strs[i]);
    t+=sprintf(t,"<td bgcolor=#%x>%%u%%c</td>",k&0xF0F8FF);
    tt+=sprintf(tt,"<td bgcolor=#%x><b>%%u%%c</b></td>",k);
    k=(((k^(0x301020>>((i%3)*8)))+0x010101)|0xC3C3C3)&0xFFFFFF;
  };
  p+=sprintf(p,"</tr><tr align=center>");
  t+=sprintf(t,"</tr><tr align=center>");
  tt+=sprintf(tt,"</tr></table><hr>");

  Send(bfr,p-bfr);

  for(i=0;fpFnc[i];++i) funcType[i]=GetFuncType(fpFnc[i]);
  do
  {
   l=sprintf(bfr,"%s",flog);
   if( !(t=strrchr(bfr,'.')) )t=bfr+l;
   //if(s_flg&FL_LOGDAY)
   sprintf(t,"%2.2u%2.2u",mnt1,day1);

//   ldat=(mnt1*31+day1)*86400;

//   Send(bfr,vsprintf(bfr,UsrTblHead[typ],anFnd));

   if((h=_lopen(bfr,0))>0)
   {l=FileSize(h);
    if((t=new char[l+20]))
    {l=_hread(h,t,l);
     t[l]=0;
     memset(((ulong*)pars)+3,0,40*8+4);
     ((ulong*)pars)[1]=0;

     tsl=ParseFile(t);
     for(;tsl;tsl=tsl->Next())
     {for(i=0;fpFnc[i];++i)
      {
       
       if((k=(fpFnc[i])(tsl,bfr)))
       {
        if( (!funcType[i]) )k=1;
       };
       
       //if( (k=(fpFnc[i])(tsl,bfr))&0x80000000) k&=0x7FFFFFFF;
       //else if(k)k=1;
       if( ( (!typ)? (ip==tsl->ip):(typ==3)?
#ifdef x86_64 
           NULL !=      
#else
          (int)
#endif
                  stristr(bfr,nm):(!stricmp(nm,bfr)) ) )
       {pars[i+2]+=k; pars[i+42]+=k;
       }
      }
     }
     LL2Lc(pars,42);
#if defined(LPC_ARM) || defined(AT_ARM) ||  defined(x86_64)
     Send(bfr,vsprintf(bfr,bfr+0x8000,
      vl_pars    ));
#else
     Send(bfr,vsprintf(bfr,bfr+0x8000,
#ifdef _BSD_VA_LIST_
 ( _BSD_VA_LIST_ )
#else
 (va_list)
#endif
     pars
    ));
#endif     
     delete t;
    }
    _lclose(h);
   }
   if(++day1>=32)
   { day1=1;
     if(mnt1==mnt2)break;
     if(++mnt1>12)mnt1=1;
   }
  }while( mnt1!=mnt2 || day1<=day2);
  LL2Lc(pars+42,40);
#if defined(LPC_ARM) || defined(AT_ARM) || defined(x86_64)
  union {
  va_list *pvl;
  ulong long * ppar;
  };
  ppar=pars+42;
  Send(bfr,vsprintf(bfr,bfr+0x9000, *pvl));

#else
  Send(bfr,vsprintf(bfr,bfr+0x9000,
#ifdef _BSD_VA_LIST_
 ( _BSD_VA_LIST_ )
#else
 (va_list)
#endif
  (pars+42)));
#endif
#undef mnt1
#undef day2
//  delete dt;
 }
// HTMLOutBottom(s);

}

