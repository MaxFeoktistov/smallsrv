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

#undef send
#define  send(a,b,c,d) Send(b,c)

#ifndef CD_VER

const char ConfigTop[]=
"<script language=javascript><!--\n"
"function EUp(n,m){var i=eval(\"document.cfrm.\"+n);" HTML_LN
"if( isNaN(i.value) )i.value=m;" HTML_LN
"if(i.value<m)++i.value; else i.value=m;}"HTML_LN
"function EDn(n,m){var i=eval(\"document.cfrm.\"+n);" HTML_LN
"if(isNaN(i.value))i.value=m; if(i.value>m)--i.value; else i.value=m;}"HTML_LN
"function ChkN(n)"HTML_LN
"{var i=eval(\"document.cfrm.\"+n),j=eval(\"document.cfrm.no\"+n);" HTML_LN
" if(j.checked){if(i.value!=0)j.value=i.value; i.value=0;}" HTML_LN
" else{if(j.value!='0' && (i.value<j.value) )i.value=j.value; else i.value=12;}" HTML_LN
"}"
"function ChkC(n)"
"{var i=eval(\"document.cfrm.\"+n),j=eval(\"document.cfrm.no\"+n);" HTML_LN
" if(j.checked){if(i.value!='')j.value=i.value; i.value=\"\";}" HTML_LN
" else{if(j.value && j.value!='0')i.value=j.value; else i.value=document.cfrm.dir.value"
"+\"\\\\\" +n"
";}" HTML_LN
"}"
"// -->\n"
"</script>"HTML_LN
"<form name=cfrm method=POST action=/$_admin_$conf>"
"<table bgcolor=#f8f8ff border=1>"HTML_LN
;

const char  UserTblBottom[]=
"<tr valign=center><td align=center><font size=2 class=f2>"
"<form method=POST action=/$_admin_$user>"
"<input type=text name=n maxlength=63 size=12> "
"</font></td><td align=center><font size=2 class=f2>"
"<input type=password name=p maxlength=63 size=12> "
"</font></td><td align=center><font size=2 class=f2>"
"<input type=text name=d maxlength=256 size=32>"
"</font></td><td><font size=2 class=f5>"
"<input type=checkbox name=ftp checked>FTP &"
"<input type=checkbox name=wri checked>write &"
"<input type=checkbox name=cgi>CGI; &nbsp; &nbsp; "
"<input type=checkbox name=smtp checked>SMTP "
"<input type=checkbox name=pop checked>POP3"
"<br><font color=#e00000><u><input type=checkbox name=adm>Admin</u></font> &nbsp; &nbsp;"
"<input type=checkbox name=hp checked>Proxy "
"<input type=hidden name=s1 value=Add >"
"<input type=submit name=s value=Add>"
"</font></form></td></tr>"
"<tr><td bgcolor=#cccccc colspan=4>"
"<a name=c1></a><br><b><super>*</super>) Type of access:</b> "
"%s" //sUSER_COULD
"</td></tr>"
"</table><br>" HTML_LN
"<br><hr>"
HTML_LN
;


int Req::HTTPOutCfg(BFILE *bf)//char *bfr)
{int r,nn=0,j;
 char *d,*t,tmpx[300],tmpy[300],*cpdesc;
 CfgParam *cp;
 d=def_dir;

 //j=sprintf(bfr,
 bf->bprintf(          "<a name=t></a><table border=0 bgcolor=#eff0ff width=100%%><tr align=center>");
 for(cp=ConfigParams;cp->desc;++cp)if(!(cp->v || cp->name))
  //j+=sprintf(bfr+j,
     bf->bprintf(  "<td bgcolor=#c0ffff><a href=#%u>%s</a></td>",nn++,cp->desc);

 //j+=sprintf(bfr+j,
            bf->bprintf("</tr></table><hr><table border=0 bgcolor=#fff0ff width=100%%><tr align=center>"
 "<td width=25%%><a href=/$_admin_$rest><font color=#FF0000>Restart</font></a></td><td width=25%%><a href=$_admin_$file?s=158>Edit config file</a></td> %s %s </tr></table>"
  " %s %s %s %s <hr>",
  ((max_smtp)?"<td width=33%><a href=/$_admin_$sndn>Try to send unsended mail now</a></td>":""),
  ((count_dns)?"<td width=33%><a href=/$_admin_$ldns>Reload</a>/<a href=/$_admin_$file?s=157 > Edit </a> host definition file</td>":""),
  (blst?"[<a href=/$_admin_$file?s=153 >Badlist</a>] &nbsp; ":""),
  (glst?"[<a href=/$_admin_$file?s=154 >Goodlist</a>] &nbsp; ":""),
  (graylst?"[<a href=/$_admin_$file?s=155 >Graylist</a>] &nbsp; ":""),
  (antiv?"[<a href=/$_admin_$file?s=156 >Antivirus</a>]":""  )



           );
 nn=0;
 //strcpy(bfr+j,(char *)ConfigTop); j+=sizeof(ConfigTop);
 bf->bprintf("%s",ConfigTop);

 for(cp=ConfigParams;cp->desc;++cp)
 {
  cpdesc=cp->desc;   
  if(cp->adv)
  {
    sprintf(cpdesc=tmpy,cp->desc,cp->adv);  
  }
     
  if(!(d=strchr(cpdesc,'.')))d=NullString; else *d++=0;
#define  END_TD
  if(!(cp->v))
  { //j+=sprintf(bfr+j,
              bf->bprintf("<tr valign=center><td colspan=2");
   if(!(cp->name))//j+=sprintf(bfr+j,
        bf->bprintf(" align=center bgcolor=#f8ffff><br><h2><a name=%u href=#t>/\\</a> %s</h2>" END_TD,nn++ ,cpdesc);
   else
   {sprintf(tmpx, (cp->max)?( ( (cp->max&s_flgs[cp->min] )==cp->max )?"checked":NullString)
       :"%s onClick=\'Chk%c(\"%s\");\'  value=0 ",
     ((cp[1].v && !*cp[1].v)?"checked":NullString),((cp[1].max)?'N':'C'),cp[1].name);
    //j+=sprintf(bfr+j,
               bf->bprintf(">" HTML_LN
    "<font size=2 class=f2><b><input type=checkbox name=%s %s>"
    "%s.</b>%s. %s" END_TD ,cp->name,tmpx ,cpdesc,d ,cp->IsR()? RSTNEED :"");
   }
  }
  else
  { //j+=sprintf(bfr+j,
              bf->bprintf("<tr valign=center><td align=right>"
   "<font size=2 class=f2><b>%s.</b> %s %s</font> </td><td align=right>"HTML_LN
   "<font size=2 class=f2>",cpdesc,d,cp->IsR()? RSTNEED :"");
   //j+=
       if(cp->max)
        //?       sprintf(bfr+j,
      bf->bprintf( "(range:%u,%u)<input type=button value=\"&lt;\" onClick=\'EDn(\"%s\",%u);\'>"
   " <input type=text name=%s maxlength=9 size=9 value=%u>"
   " <input type=button value=\"&gt;\" onClick=\'EUp(\"%s\",%u);\'>" END_TD
   ,cp->min,cp->max,cp->name,cp->min,cp->name,*(cp->v),cp->name,cp->max) ;//:
   else
   //sprintf(bfr+j,
      bf->bprintf( (char *)   ( (cp->min==2048)? "<textarea maxlength=%u rows=2 cols=30 name=%s>%s</textarea>" END_TD
   :"<input type=text maxlength=%u size=40 name=%s value=\"%s\">" END_TD )
   ,cp->min,cp->name, (*cp->v)?* (char **) cp->v: NullString);
  }
  //j+=sprintf(bfr+j,
             bf->bprintf("</font></td></tr>" HTML_LN );
  if(d!=NullString)d[-1]='.';
 // if(j>0x1000){if(Send(bfr,j)<=0)return -1; j=0;}
 }
 //j+=sprintf(bfr+j,
            bf->bprintf("</table><center><input type=submit value=\"Submit changes\"> &nbsp;  &nbsp;  &nbsp; <input type=reset value=\"Reset to current\"></form></center><hr>");
 //if(Send(bfr,j)<=0)return -1;
#ifndef SYSUNIX
 HTTPCGIAppOut(bf);
#endif
 HTTPMimeTypesOut(bf);
 bf->fflush();
 return 0;
}

int Req::HTTPOutHosts(BFILE *bf ) //char *bfr)
{host_dir *a;
 int i,j;

 //j=sprintf(bfr,
 bf->bprintf(
#ifdef RUS
"<h2>Виртуальные хосты и каталоги:</h2>"
"Сервер может поддерживать много виртуальных хостов. Каждый хост имеет собственный "
"корневой каталог и также может включать виртуальные каталоги общие для всех. "
"Чтобы добавить виртуальный хост введите <i>any.host.name</i> и полный путь к корневому каталогу. "
"Чтобы добавить виртуальный каталог введите <i><b>/</b>directory<b>/</b></i> "
"(между двумя слешами) и полный путь к каталогу подстановки."
"Чтобы сделать ресурс приватным, можно через запятую указать пользователей для которых он будет доступен. После каждого пользователя вы можите указать двоеточие и тип доступа: "\
"R - чтение, W-запись,  NOFTP, NOHTTP .<BR> Пример:"\
" <I>admin:RW,anonymous:RNOHTTP,user1,user2:R,user3:W,user4:RWNOHTTP</I><BR> " \
"Вместо рот каталогов можно указать ссылку на другой Web сервер: http://host  Для такого хоста сервер будет работать как прокси."
"<table bgcolor=#bcf8cc border=1>"
"<tr bgcolor=#bcf0ff><td>Имя хоста или <b>/</b>каталог<b>/</b></td><td>Каталог</td><td>Пользователи</td>"
#else
"%s"  //sVIRTUAL_HOSTS
// "<table bgcolor=#bcf8cc border=1><tr bgcolor=#bcf0ff>"
//  "<td>Host name or <b>/</b>directory<b>/</b></td>"
//  "<td>Directory</td><td>Users</td>"
#endif
 "<td>&nbsp;</td>"
"</tr>"HTML_LN
,sVIRTUAL_HOSTS
);
 i=0;
 for(a=hsdr.next;a;a=a->next)
 {
   //j+=sprintf(bfr+j,
   bf->bprintf(
"<tr valign=center align=center><td><font size=2 class=f2>"
"<form method=GET action=/$_admin_$host>"
"<input type=hidden name=h value=\"%s\"><b>%s</b>"
"</font></td><td><font size=2 class=f2>"
"<input type=text name=d maxlength=256 size=30 value=\"%s\">"
"</font></td><td><font size=2 class=f2>"
"<input type=checkbox name=p %s>Private for "
"<input type=text name=n maxlength=256 size=16 value=\"%s\">"
"</font></td><td><font size=2 class=f2>"
"<input type=hidden name=s1 value=Change >"
"<input type=submit name=s value=Change > "
"<input type=submit name=s value=Remove onClick=\"form.s1.value='Remove';\"> "
"<input type=reset value=Reset>"
"</form></font></td></tr>"HTML_LN
  ,a->h,a->h,a->d,(a->flg)?"checked":NullString,(a->flg)?a->d+a->flg:"");
   ++i;
   //if(j>0x4000){if(send(s,bfr,j,0)<=0)return -1; j=0;}
 }

 //j+=sprintf(bfr+j,
 bf->bprintf(
            "<tr valign=center align=center><td><font size=2 class=f2>"
"<form method=GET action=/$_admin_$host>"
"<input type=text name=h maxlength=127 size=32>"
"</font></td><td><font size=2 class=f2>"
"<input type=text name=d maxlength=256 size=32>"
"</font></td><td><font size=2 class=f2>"
"<input type=checkbox name=p>Private for "
"<input type=text name=n maxlength=256 size=16>"
"</font></td><td><font size=2 class=f2>"
"<input type=checkbox name=u>Update system hosts file"
"IP:<input type=text name=i maxlength=33 size=12 value=\"127.0.0.1\"> &nbsp; "
"<input type=hidden name=s1 value=Add >"
"<input type=submit name=s value=Add>"
"</form></font></td></tr></table><br><hr>" HTML_LN );
 //if(send(s,bfr,j,0)<=0)return -1;
 return 0;
};

int Req::HTTPUserAdd(BFILE *bf)//char *bfr)
{User *tuser;
 int i,j;
 char *p;
 //j=sprintf(bfr,
 bf->bprintf(
"<h2>Users:</h2>"
//"<script>"
//"function uld(a,b){ a.s1.value=b; return true;}"
//"</script>"
#ifndef FREEVER
"<b>%s</b>"
#endif
"<table bgcolor=#ccf0ff border=1 cellspacing=1 cellpadding=1>"
"<tr valign=center bgcolor=#ccfff0>"
"<td><font size=2 class=f2><b>User</b></font></td>"
"<td><font size=2 class=f2><b>Password</b></font></td>"
"<td><font size=2 class=f2><b>Directory</b></font></td>"
"<td width=300><font size=2 class=f2><b>Type of access <a href=#c1><sup>*</sup></a></b></font></td>"
"</tr>" HTML_LN
);
 for(tuser=userList;tuser;tuser=tuser->next) if((i=tuser->state)&0x80)
 {p=tuser->pasw();
  //j+=sprintf(bfr+j,
     bf->bprintf(
  "<tr valign=center><td align=center><font size=2 class=f2>"
  "<form method=POST action=/$_admin_$user>"
  "<input type=hidden name=n value=\"%s\"><b>%s</b>"
  "</font></td><td align=center><font size=2 class=f2>"
  "<input type=password name=p maxlength=63 size=12 value=\"%s\">"
  "</font></td><td align=center><font size=2 class=f2>"
  "<input type=text name=d maxlength=256 size=32 value=\"%s\">"
  "</font></td><td><font size=2 class=f5>"
  "<input type=checkbox name=ftp %s>FTP &"
  "<input type=checkbox name=wri %s>write &"
  "<input type=checkbox name=cgi %s>CGI; &nbsp;  &nbsp; "
  "<input type=checkbox name=smtp %s>SMTP "
  "<input type=checkbox name=pop %s>POP3<br>"
  "<font color=#e00000><u><input type=checkbox name=adm %s>Admin</u></font> "
  "<input type=checkbox name=hp %s>Proxy &nbsp; "
  "<input type=hidden name=s1 value=Change >"
  "<input type=submit name=s value=Change > "
  "<input type=submit name=s value=Remove onClick=\"form.s1.value='Remove';\"> "
  "<input type=reset value=Reset>"
  "</form></font></td></tr>" HTML_LN,
  tuser->name,tuser->name,p,tuser->dir(p),
  (i&UserFTP)?"checked":NullString,
  (i&UserFTPW)?"checked":NullString,
  (!(i&UserNOCGI))?"checked":NullString,
  (i&UserSMTP)?"checked":NullString,
  (i&UserPOP3)?"checked":NullString,
  (i&UserADMIN)?"checked":NullString,
  (i&UserHTTP)?"checked":NullString
  );

 // if(j>0x4000){if(send(s,bfr,j,0)<=0)return -1; j=0;}
 }
 bf->bprintf(//"%s",
             UserTblBottom,sUSER_COULD);
 //strcpy(bfr+j,(char *)UserTblBottom);
 //if(send(s,bfr,j+sizeof(UserTblBottom)-1,0)<=0)return -1;
 return 0;
};

int Req::HTTPMimeTypesOut(BFILE *bf) // char *bfr)
{int i,j;
 char *p,*t,*t1;
 //j=sprintf(bfr,
  bf->bprintf(
 "<h2>Advansed MIME types</h2>"
 "<table bgcolor=#f0fef8 border=1><tr valign=center>"
  "<td bgcolor=#f0fcff><font size=2 class=f2><b>.ext</b>ension</font></td>"
  "<td bgcolor=#f0fcff><font size=2 class=f2><b>MIME type</b></font></td>"
  "<td bgcolor=#f0fcff><font size=2 class=f2> &nbsp; </font></td>"
 "</tr>" HTML_LN
 );
 i=0;
 if((p=mime))
 do{
  if(!(t=strchr(p,';')))break;
  *t++=0;
  if((t1=strchr(t,';')))*t1=0;
  //j+=sprintf(bfr+j,
  bf->bprintf(
  "<tr valign=center><td align=center><font size=2 class=f2>"
  "<form method=GET action=/$_admin_$mime>"
  "<input type=hidden name=e size=7 value=\"%s\"><b>%s</b>"
  "</font></td><td><font size=2 class=f2>"
  "<input type=text name=t maxlength=127 size=48 value=\"%s\">"
  "</font></td><td><font size=2 class=f2>"
  "<input type=hidden name=s1 value=Change >"
  "<input type=submit name=s value=Change > &nbsp;"
  "<input type=submit name=s value=Remove  onClick=\"form.s1.value='Remove';\" > &nbsp;"
  "<input type=reset value=Reset>"
  "</form></font></td></tr>" HTML_LN,p,p,t);
  i++; t[-1]=';';
  if(!t1)break;
  *t1=';'; p=t1+1;
  /*
  if(j>0x4000)
  {if(send(s,bfr,j,0)<=0)return -1;
   j=0;
  }
  */
 }while(1);

 //j+=sprintf(bfr+j,
 bf->bprintf(
 "<tr valign=center><td><font size=2 class=f2>"
 "<form method=GET action=/$_admin_$mime>"
 "<input type=hidden name=n value=%u>"
 "<input type=text name=e maxlength=5 size=7>"
 "</font></td><td><font size=2 class=f2>"
 "<input type=text name=t maxlength=127 size=48>"
 "</font></td><td><font size=2 class=f2>"
 "<input type=hidden name=s1 value=Add >"
 "<input type=submit name=s value=Add> &nbsp;"
 "</form></font></td></tr></table>" HTML_LN
 "<hr>"HTML_LN,i);
 //if(send(s,bfr,j,0)<=0)return -1;
 return 0;
};

#ifndef SYSUNIX
int Req::HTTPCGIAppOut(BFILE *bf) //char *bfr)
{int i,j;
 char *t,**tt;
 //j=sprintf(bfr,
 bf->bprintf(
 "<h2>Interpreters for diferent types of CGI.</h2>"
 "<table bgcolor=#fcf0e0 border=1>"
 "<tr>"
  "<td bgcolor=#f0fcff><font size=2 class=f2><b>.ext</b>ension</font></td>"
  "<td bgcolor=#f0fcff><font size=2 class=f2><b>Application</b></font></td>"
  "<td bgcolor=#f0fcff><font size=2 class=f2> &nbsp;</font></td>"
 "</tr>" HTML_LN );
 i=0;
 for(tt=ext; *tt; tt+=2) if(*tt[0]=='.')
 { //j+=sprintf(bfr+j,
  bf->bprintf(
  "<tr valign=center><td><font size=2 class=f2>"
  "<form method=GET action=/$_admin_$aple>"
  "<input type=hidden name=n value=%u>"
  "<input type=text name=e maxlength=5 size=7 value=\"%s\">"
  "</font></td><td><font size=2 class=f2>"
  "<input type=text name=t maxlength=127 size=48 value=\"%s\">"
  "</font></td><td><font size=2 class=f2>"
  "<input type=hidden name=s1 value=Change >"
  "<input type=submit name=s value=Change> &nbsp;"
  "<input type=submit name=s value=Remove onClick=\"form.s1.value='Remove';\"> &nbsp;"
  "<input type=reset value=Reset>"
  "</form></font></td></tr>" HTML_LN,i,tt[0],tt[1]);
  if(++i>10)break;
 }

 //j+=sprintf(bfr+j,
 bf->bprintf(
            (i<10)?
 "<tr valign=center><td><font size=2 class=f2>"
 "<form method=GET action=/$_admin_$aple>"
 "<input type=hidden name=n value=%u>"
 "<input type=hidden name=s1 value=Add >"
 "<input type=text name=e maxlength=5 size=7>"
 "</font></td><td><font size=2 class=f2>"
 "<input type=text name=t maxlength=127 size=48>"
 "</font></td><td><font size=2 class=f2>"
  "<input type=submit name=s value=Add> &nbsp;"
   "</form></font></td></tr></table><hr>"HTML_LN
 :"</table><hr>" HTML_LN
 ,i);

 //if(send(s,bfr,j,0)<=0)return -1;
 return 0;
};
#endif

#endif
#undef send

