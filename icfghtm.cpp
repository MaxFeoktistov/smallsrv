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

#ifdef TLSVPN
#include "vpn.h"
#endif


#ifndef CD_VER



#ifdef RUS
#define  HTML_TOP1 "HTTP/1.1 200 Ok\r\n"\
"Pragma: NO-CACHE\r\n"\
"Content-Type: text/html; charset=windows-1251\r\n"\
"\r\n"\
"<html><head>"\
"<style>\n"\
"font {font-size:9pt; font-style: normal ; line-height:1em ; font-family: Times,serif ;}\n"\
"font.f{font-size:9pt; line-height:1em }\n"\
"font.f2{font-size:9pt; line-height:1em}\n"\
"font.f5{font-size:8pt; line-height:1em }\n"\
"font.f3, font.f3a {font-size:8pt; line-height:1em }\n"\
"font.f3a{ white-space: nowrap }\n"\
"table { border-collapse: collapse ; border-spacing: 0px ; font-size:9pt; line-height:10pt; font-family: Times,serif ; }\n"\
"</style>"\
"<script language=javascript>"\
" function SHTbl(tb)"\
" {var tbl;"\
"  tbl=document.getElementById(tb);"\
"  lnk=document.getElementById('l'+tb);"\
"  if(tbl.style.display == \"none\") {"\
"  tbl.style.display = \"\";  if(lnk) lnk.innerHTML='[-]' ; "\
"  } else { tbl.style.display = \"none\";  if(lnk) lnk.innerHTML='[+]' ; }"\
" };"\
"</script>"\
"<title>Администрирование Small HTTP сервера</title></head>"\
"<body bgcolor=#ffffff text=#000000>"\
"<center><h2>Small HTTP server</h2><br>Страничка администора.<br><br>"\
"<table cellspacing=3 cellpadding=8 border=0 width=90%><tr valign=top align=center>"

#define  HTML_TOP2 "<td><font size=2 class=f><b><a href=/$_admin_$conf>Установки</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$host>Пользователи и Виртуальные хосты</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$loglast>Текущий лог</a></b></font></td>"

#define  HTML_TOP3 "<td><font size=2 class=f><b><a href=/$_admin_$stat>Статистика</a> / <a href=/$_admin_$stat?t=0>страницы</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$stte>Состояние</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$susr>Итоговая статистика</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$logout>Logout</a></b></font></td>"\
"</tr>"\
"</table>"
#else

#define  HTML_TOP1 CXS(S5t2T_622312, "HTTP/1.1 200 Ok\r\n"\
"Pragma: no-cache\r\n"\
"Content-Type: text/html%s\r\n"\
"\r\n"\
"<html><head>"\
"<style>\n"\
"font {font-size:10pt; font-family: Times,serif ;}\n"\
"font.f{font-size:9pt; line-height:10pt ; font-family: Times,serif ;}\n"\
"font.f2{font-size:9pt; line-height:11pt}\n"\
"font.f5{font-size:8pt; line-height:12pt}\n"\
"font.f3, font.f3a {font-size:8pt; line-height:9pt; }\n"\
"font.f3a{ white-space: nowrap }\n"\
"table { border-collapse: collapse ; border-spacing: 0px ; font-size:10pt; line-height:11pt; font-family: Times,serif ;}\n"\
"</style>"\
"<script language=javascript>"\
" function SHTbl(tb)"\
" {var tbl,lnk;"\
"  tbl=document.getElementById(tb);"\
"  lnk=document.getElementById('l'+tb);"\
"  if(tbl.style.display == \"none\") {"\
"  tbl.style.display = \"\";  if(lnk) lnk.innerHTML='[-]' ;  "\
"  } else { tbl.style.display = \"none\";  if(lnk) lnk.innerHTML='[+]' ; }"\
" };\n"\
"</script>\n"\
"<title>Administration of Small HTTP server</title></head>"\
"<body bgcolor=#ffffff text=#000000>"\
"<center><h2>Small HTTP server</h2><br>Administration page.<br><br>"\
"<table cellspacing=3 cellpadding=8 border=0 width=90%%><tr valign=top align=center>%s"\
"<td><font size=2 class=f><b><a href=/$_admin_$stat>Current statistics</a> / <a href=/$_admin_$stat?t=0>pages</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$stte>Current state</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$susr>Summary statistics</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$logo>Logout</a></b></font></td>"\
"</tr>"\
"</table>")


#define  HTML_TOP2 CXS(S5t2T_622313, "<td><font size=2 class=f><b><a href=/$_admin_$conf>Setting</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$host>Users & Virtual Hosts</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$logl>Current Log</a></b></font></td>")

/*
#define  HTML_TOP3 "<td><font size=2 class=f><b><a href=/$_admin_$stat>Current statistics</a> / <a href=/$_admin_$stat?t=0>pages</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$stte>Current state</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$susr>Summary statistics</a></b></font></td>"\
"<td><font size=2 class=f><b><a href=/$_admin_$logo>Logout</a></b></font></td>"\
"</tr>"\
"</table>"
*/


#endif

#define HTMLTop HTML_TOP1

/*
const char HTMLTop[]=
HTML_TOP1
"%s"
//HTML_TOP2
HTML_TOP3
;*/

/*
const char  BanerHTML[]=
"<br><center>"
#ifdef SYSUNIX
 "<b><a href=/$_admin_$regi>Registration</a></b><br><br>"
#endif
"<table width=500 height=60 border=1 cellspacing=0 cellpadding=1>"
"<tr vallign=top cellpadding=0 cellspacing=0>"
 "<td height=4 bgcolor=#8030e0> "
 "<table width=494 height=8 border=0 cellspacing=0 cellpadding=1>"
  "<tr cellpadding=1 cellspacing=0>"
   "<td bgcolor=#5030a0 width=60 height=4>"
  // "<a href=http://srv.mf.inc.ru/reg2.cgi>"
  "<font size=0 color=#ffffff class=f3>Unregistred</font>"
  // </a>
  "</td>"
   "<td bgcolor=#6030b0 width=60 height=4>"
   //<a href=http://srv.mf.inc.ru/reg2.cgi>"
   "<font size=0 color=#ffffff class=f3a>copy</font>"
   //"</a>"
   "</td>"
   "<td bgcolor=#7030c0 width=60 height=4 align=right>"
   //<a href=http://srv.mf.inc.ru/news.htm>
   "<font size=0 color=#ffffff class=f3a>of <b>Small</b></font>"
   //</a>
   "</td>"
   "<td bgcolor=#8030d0 height=4>"
   //<a href=http://srv.mf.inc.ru/news.htm>"
   "<font size=0 color=#ffffff class=f3a><b>HTTP server</b></font>"
   //</a>
   "</td>"
   "<td bgcolor=#9030e0 width=60 height=4><font size=0 class=f3>&nbsp;</font></td>"
   "<td bgcolor=#a030f0 width=60 height=4><font size=0 class=f3>&nbsp;</font></td>"
   "<td bgcolor=#b030ff width=60 height=4><font size=0 class=f3>&nbsp;</font></td>"
   "<td bgcolor=#c0c0c0 width=12 height=4><a href=http://smallsrv.com/news.htm><font size=0 color=#00c0f0 class=f3><b>/\\\\</b></font></a></td>"
  "</tr></table>"
 "</td>"
"</tr>"
"<tr valign=middle align=center><td bgcolor=#f0ffff><b>"

//*
#ifdef RUS

/*
"<a href=http://smallsrv.com/devrus.htm><font size=2 color=#00c0ff class=f>"
"Разработка софта и устройств автоматизации.<br>Обеспечение научных исследований, компьютерное моделирование.</font></a>"
"<a href=http://smallsrv.com/regrus.cgi><font size=2 color=#00c090 class=f><br>Зарегистрируйтесь, чтобы спрятать рекламу</font></b></a>"
* /

"<a href=http://%s ><font size=2 color=#00c0ff class=f>"
"%s</font></a><br>"
"<a href=http://smallsrv.com/regrus.cgi><font size=2 color=#00c090 class=f><br>Зарегистрируйтесь, чтобы спрятать рекламу</font></b></a>"

#else
//*/

/*
"<a href=http://smallsrv.com/dev.htm><font size=2 color=#00c0ff class=f>"
"Software and hardware development<br>All by your request, hight quality, low price</font></a>"
"<a href=http://smallsrv.com/reg2.cgi><font size=2 color=#00c090 class=f><br>Please registrate, to hide the advertismant</font></b></a>"
* /


"<a href=http://%s ><font size=2 color=#00c0ff class=f>"
"%s</font></a><br>"
"<a href=http://smallsrv.com/reg2.cgi><font size=2 color=#00c090 class=f><br>Please registrate, to hide the advertismant</font></b></a>"


#endif
"</td></tr>"
"</table></center><br>"
;

*/
#undef send
#define  send(a,b,c,d) Send(b,c)


#ifndef FREEVER

/*
char *banner[]=
{

#ifdef RUS
 "lsol.ru","Реклама в программных продуктах",
 "lsol.ru/soft.htm","Разаработка программного обеспечения",
 "lsol.ru/devices.htm","Разаработка приборов и устройств",
 "lsol.ru/linux.htm","Linux - помощь в установке, поддержка, обучение",
 "lsol.ru/orang.htm","Автоматизация теплиц и оранжерей",
#endif
 "smallsrv.com/dev.htm","Software developping, hight quality, best price",
 "smallsrv.com/dev.htm","Software for embedded system, portable code",
 "smallsrv.com/dev.htm","Creating device drivers for Windows and Linux",
 "smallsrv.com/devhard.htm","Hardware developping, PCI/USB/PC104 modules",
 "smallsrv.com/devhard.htm","Creating devices for control with microcontrollers",
 "smallsrv.com/devhard.htm","Automatisation for industrial technologies",
 0,0

};


int  cur_banner;
*/
#endif

void Req::OutBaner(char *b)
{
#ifndef CD_VER
#ifndef FREEVER

 if( user_name!=(about+sizeof(ABOUT_STR)+14) )
 {
    //send(s,BanerHTML,sizeof(BanerHTML)-1,0);
   BFILE bfl;
   bfl.Init(this,(PrintFlush)(Snd),b);
   bfl.bprintf((char *)BanerHTML,banner[cur_banner],banner[cur_banner+1]);
   bfl.fflush();
    cur_banner+=2;
    if(!(banner[cur_banner]))cur_banner=0;
 }
#endif
#endif
}


/*
void HTMLOutTop(int s)
{if(s_flg&FL_RADMIN)
  send(s,HTMLTop,sizeof(HTMLTop)-1,0);
 else
 {send(s,HTMLTop,sizeof(HTML_TOP1)-1,0);
  send(s,HTMLTop+sizeof(HTML_TOP1)+sizeof(HTML_TOP2)-2,sizeof(HTML_TOP3)-1,0);
 }
 OutBaner();
};
*/
#else
#define OutBaner()
#endif

#ifndef LF
#define LF
#endif
//#undef LF
//#define LF "\n"


struct sOutLine
{
  Req *th;
  char *bfr;
  int i,j,k;
  int OutConnLine(Req *r, char *alt_text);
};

int sOutLine::OutConnLine(Req *r, char *alt_text)
{
  #ifdef USE_IPV6
  union{ struct sockaddr_in  san;  struct sockaddr_in6 san6;};
  char   xs[64];
  #else
  struct sockaddr_in  san;
  #endif
  int ll,l; //=sizeof(sockaddr_in);
  char *t1;
  CntrCode  *cc;
  char contry[8];


  #ifdef USE_IPV6
    l=sizeof(sockaddr_in6);
    IP2S(xs, &(r->sa_c));
  #else
    l=sizeof(sockaddr);
  #endif
  if(getsockname(r->s,(sockaddr *)&san,&l)) return j;
  san.sin_port=htons((ushort)san.sin_port);
  /*
   * if( (soc_port[0]==san.sin_port || soc_port[1]==san.sin_port) &&
   *   DWORD_PTR(r->inf[0])!=0x544345 x4CHAR("ECT") &&
   *   (t1=strchr(r->inf+1,' '))
   * )*t1=0; */
  contry[0]=0;
  if(cntr_dat)
  {
    uint ipv4 = r->sa_c.sin_addr.s_addr;
    #ifdef USE_IPV6
    if(r->sa_c6.sin6_family == AF_INET6)
    {
      if(
        r->sa_c6.sin6_addr.s6_addr32[0]==0 &&
        r->sa_c6.sin6_addr.s6_addr32[1]==0 &&
        r->sa_c6.sin6_addr.s6_addr32[2]==0xFFFF0000
      ) ipv4 = r->sa_c6.sin6_addr.s6_addr32[3];
      else goto lbIPv6;
    }
    #endif
    if( (cc=FindCntr(htonl(ipv4) ) ) )
    {
      sprintf(contry," (%2.2s)",cc->nm);
    }
    lbIPv6:;
  }
  // Dont_fix_var
  j+=sprintf(bfr+j,"<tr valign=center><td><font size=2 class=f2>%u) <b>"
  #ifdef USE_IPV6
  "%s"
  #else
  "%u.%u.%u.%u"
  #endif
  // Dont_fix_var
  "%s</td><td align=center><font size=2 class=f2><b>%u</b></font>"
  "</td><td align=left><font size=2 class=f2>%d</font>"
  "</td><td align=left><font size=2 class=f2>%u / %u</font>"
  "</td><td align=left><font size=2 class=f2>%s..</font>"
  "</td><td align=left bgcolor=#ff8040><font size=2 class=f2>"
  "<form method=GET action=/$_admin_$break>"
  "<input type=hidden name=n value=%u>"
  "<input type=hidden name=t value=%u>"
  "<input type=%s value=Break>"
  "</form></font>"
  "</td></tr>" HTML_LN
  , k++,
#ifdef USE_IPV6
  xs,
#else
#ifndef SYSUNIX
  r->sa_c.sin_addr.S_un.S_un_b.s_b1,  r->sa_c.sin_addr.S_un.S_un_b.s_b2,
  r->sa_c.sin_addr.S_un.S_un_b.s_b3,  r->sa_c.sin_addr.S_un.S_un_b.s_b4,
#else
  r->sa_c.sin_addr.s_addr&0xFF ,  BYTE_PTR(r->sa_c.sin_addr.s_addr,1),
             BYTE_PTR(r->sa_c.sin_addr.s_addr,2),BYTE_PTR(r->sa_c.sin_addr.s_addr,3),
#endif
#endif
             contry,
             (ushort)san.sin_port,
             GetTickCount()-r->tmout, //[i],
             r->Tin,r->Tout, (alt_text)? alt_text : r->inf,i,r->tmout //[i]
             ,(s_flg&FL_RADMIN)?"submit":"hidden"
  );
  if(j>0x4000)
  {if(th->Send(bfr,j)<=0)
    {no_close_req=0;
      return -1;
    }
    j=0;
  }

 return j;

}

int Req::OutActualConn(char *bfr)
{
#ifndef CD_VER
 //int i,oline.j,sx,k;
// struct sockaddr_in sa;
 sOutLine oline;
 oline.th = this;
 oline.bfr = bfr;

 oline.j=sprintf(bfr,
  CXS(S5t2T_0, "<h2>Now connected:</h2>"
   "<table bgcolor=#cffce0 border=1>"
   "<tr bgcolor=#a0eef8><td align=center><font size=3 class=f3><b>"
     "Remote IP</b></font>"
   "</td><td align=center><font size=3 class=f3><b>Port</b></font>"
   "</td><td align=left><font size=2 class=f2>Time (ms)</font>"
   "</td><td align=left><font size=2 class=f2>in/out (bytes)</font>"
   "</td><td align=left><font size=2 class=f2>info</font>"
   "</td><td align=left><font size=2 class=f2> &nbsp;</font>"
   "</td></tr>" )  );
 Req *r;
 ++no_close_req;
 oline.k=0;
 for(oline.i=0; oline.i<max_tsk; ++oline.i)
   if( ((u_long)(r=rreq[oline.i]))>1 )
     if(oline.OutConnLine(r, 0)<0) return -1;

 if(KeepAliveList)
 {
   MyLock(KeepAliveMutex);
   for(int i=0; i<KeepAliveCount; i++)
   {
     oline.i = i | CONID_KEEPALIVE_MASK;
     if(oline.OutConnLine( KeepAliveList[i],  "Keep-Alive Sleep" )<0)
     {
       MyUnlock(KeepAliveMutex);
       return -1;
     }
   }
   MyUnlock(KeepAliveMutex);
 }
#ifdef TLSVPN
 char txt[64];
 uint ip;
 if(vpn_count)
 {
   VPNclient *v;
   for(int i=0; i<vpn_count; i++)
   {
     oline.i = i | CONID_VPN_MASK;
     v = vpn_list[i];
     ip = v->ipv4;
     sprintf(txt,"TLS VPN:%s: %u.%u.%u.%u", TUNTAPNames[v->tun_index], ip&0xFF,(ip>>8)&0xFF ,(ip>>16)&0xFF, ip>>24); // TODO: Big endian fix

     if(oline.OutConnLine(v,  txt )<0)
     {
       return -1;
     }
   }
 }
 if(vpn_cln_connected)
 {
   oline.i = CONID_VPNCL_VALUE;
   ip = vpn_cln_connected->ipv4;
   sprintf(txt,"VPN Client: %u.%u.%u.%u",  ip&0xFF,(ip>>8)&0xFF ,(ip>>16)&0xFF, ip>>24);  // TODO: Big endian fix

   if(oline.OutConnLine(vpn_cln_connected,  txt )<0)
     return -1;
 }
#endif
 --no_close_req;
 oline.j+=sprintf(bfr+oline.j, "</table><hr>");
 if(send(s,bfr,oline.j,0)<=0)return -1;
#endif
 return 0;
#undef san
}


#ifndef CD_VER

const char CBPre[]="</center><pre>\r\n";
const char CEPre[]="\r\n</pre><center>";
void Req::HTTPOutCurentLog(char *b)
{char *p,*t,c;
 int i;
  p=b+sprintf(b,CBPre);
  for(t=b_prot;t<pprot;++t)
  {
   if( (i=((c=*t)=='<')) || c=='>' ){DWORD_PTR(*p)=0x3B746726 x4CHAR("&gt;") + 0x500*i; p+=4; }
   else *p++=c;
   if((i=p-b)>0x2000)Send(p=b,i);
  }
  p+=sprintf(p,CEPre);
  Send(b,p-b);
}


#endif


// Dont_fix_var
#define XHTMLDirHead "HTTP/1.0 200 Ok\r\nConnection: close\r\nContent-Type: text/html ; charset=utf-8\r\n\r\n<html>" \
"<head>" \
"<script language=\"javascript\">\n" LF \
"var k,r,c,n,u="



char HTMLDirHead[]= XHTMLDirHead
#ifndef CD_VER
 #ifndef SYSUNIX
 "9"
 #else
 "10"
 #endif
#else
 "6"
#endif
// Dont_fix_var
 " ;" LF
"var h=document.links;" LF
"function L(x)" LF
"{" LF
 "if(h[x].text)return h[x].text;" LF
 "var z,s=h[x].hash;"
 "if(s && s!=\"#\")" LF
 "{if(s.substring(0,1)==\"#\")return s.substring(1,200);" LF
  "return s;" LF
 "}" LF
 "s=h[x].href;" LF
 "if(s)" LF
 "{" LF
  "if(location.href.indexOf(s)==0)return \"../\";" LF
  "if(!x)return \"../\";" LF
  "z=s.lastIndexOf(\"#\");" LF
  "if(z>=0)return s.substring(z+1,200);" LF
  "z=s.lastIndexOf(\"/\");" LF
  "if(z>=0)" LF
  "{if(z>=(s.length-1))z=s.lastIndexOf(\"/\",z-1);" LF
   "if(z>=0)return s.substring(z+1,200);" LF
  "}" LF
  "return s;" LF
 "}" LF
 "return h[x].pathname;" LF
"}" LF
"function M(a,b)" LF
"{var x,y;" LF
 "x=L(a*3+k+6);" LF
 "y=L(b*3+k+6);" LF
 "if(k==1 || k==4){x*=2;y*=2;}" LF
 "if(x>y)" LF
 "return r;" LF
 "if(x<y)return -r;" LF
 "return 0;" LF
"};" LF
"function A(x,y)" LF
"{var z=x+3;" LF
 "return \"<b><a href='javascript:O(\"+x+\");'>\"+y+\" /&#92; </a> - <a href='javascript:O(\"+z+\");'>&#92;/</a></b></td>\";" LF
"};" LF
"function S()" LF
"{return \"cript>\";}" LF
"function F(x,y)" LF
"{return \"<td><a href='\" + L(y) + ((y==x)?\"\":\"#\" + L(x)) + \"'>\" + L(x) + \"</a></td>\";};" LF
"function O(z)" LF
"{var i,j,w,o;" LF
 "r=1;" LF
 "k=z;" LF
 "if(k>=3){r=-1;k-=3;}" LF
 "c=(document.links.length-u)/3; u=6;" LF
 "n=new Array(c);" LF
 "for(i=0;i<c;++i)n[i]=i;" LF
 "n.sort(M);" LF
 "o=\"<scr\"+\"ipt language=javascript>var k,r,c,n,u=6; var h=document.links;\"" LF
 "+L.toString()" LF
 "+M.toString()" LF
 "+A.toString()" LF
 "+F.toString()" LF
 "+O.toString()" LF
 "+S.toString()" LF
 "+\"\\n</s\";" LF
 "o+=S() + " LF
 "\"<table border=0 width=100% bgcolor=#f0f0ff><tr bgcolor=#aaaaff><td width=50%>\"+A(0,\"Name\")+\"<td width=15%>\"+A(1,\"Size\")+\"<td>\"+A(2,\"Date\")+\"</tr>\";" LF
 "for(i=0;i<c;++i)" LF
 "{j=n[i]*3+6;" LF
  "o+=\"<tr>\" + F(j,j) + F(j+1,j) + F(j+2,j) + \"</tr>\";" LF
 "};" LF
 "w=document;" LF
 "o+=\"</table><hr>\";" LF
 "w.open();" LF
 "w.write(o);" LF
 "w.close();" LF
 "o=\"\";" LF
 "delete n;" LF
"}" LF
"\n</script>" LF
"</head>"
"<body>"
"<table border=0 width=100% bgcolor=#f0f0ff><tr bgcolor=#aaaaff>"
"<td width=50%><b><a href=\"javascript:O(0);\">Name /\\</a> - <a href=\"javascript:O(3);\">\\/</a></b></td>" LF
"<td><b><a href=\"javascript:O(1);\">Size /\\</a> - <a href=\"javascript:O(4);\">\\/</a></b></td>" LF
"<td><b><a href=\"javascript:O(2);\">Date /\\</a> - <a href=\"javascript:O(5);\">\\/</a></b></td>" LF
"</tr>";


const char HTMLDirBody[]=
// Dont_fix_var
"<tr><td><a href=\"%s%s\">%.64s%s</a></td><td><a href=\"%s%s#%s\">%s</a></td><td><a href=\"%s%s#%s %s %s\">%s %s %s</a></td></tr>";
const char HTMLDirBody2[]=
//"<tr><td><a href=\"%s%c\">%.64s%c</a></td><td>%u</td><td>%3.3s, %u %3.3s %4.4u %2.2u:%2.2u:%2.2u</td></tr>";
// Dont_fix_var
"<tr><td><a href=\"%s%s\">%.64s%s</a></td><td><a href=\"%s%s#%u\">%u</a></td><td><a href=\"%s%s#%4.4u-%2.2u-%2.2u %2.2u:%2.2u:%2.2u\">%4.4u-%2.2u-%2.2u %2.2u:%2.2u:%2.2u</a></td></tr>";

#ifndef CD_VER
void SRegHTM()
{ WORD_PTR(HTMLDirHead[sizeof(XHTMLDirHead)-1])=0x2036 //'6'
;}
#undef XHTMLDirHead

inline void Req::PutFTPLine(char *ln,char *bfr)
{
 int i;
 char *t,*ar[10],*p;
 t=ln;
 i=0;
 p="";
 while(*++t)
 {if(*t==' ' || *t=='\t' )*t=0;
  else if(!t[-1])
  {ar[i++]=t;
   if(i>=8)
   {if(*ln=='d')p="/";
   lbo:
    if(WORD_PTR(*ar[7])!='.')
      send(s,bfr,sprintf(bfr,HTMLDirBody,ar[7],p,ar[7],p,ar[7],p,ar[3],ar[3],ar[7],p,ar[4],ar[5],ar[6],ar[4],ar[5],ar[6]),0);
    return ;
   }
  }
 }
 if(i>=3)
 {ar[7]=ar[2]; ar[6]="";
  ar[5]=ln; ar[4]=ar[0]; ar[3]=ar[1];
  if( DWORD_PTR(*ar[3])==0x5249443C x4CHAR("<DIR") ) p="/";
  goto lbo;
 }
};

char * Req::PutFTPDir(char *ln,char *bfr)
{char *t;
 int  tt,n;
 while( (t=strchr(ln,'\n')) )
 {tt='\n'; n=1;
  if(t[-1]=='\r'){--t; tt='\r'; n=2;}
  *t=0;
  if( (t-ln) >= 32 )PutFTPLine(ln,bfr);
  *t=tt;
  ln=t+n;
 }
 return ln;
}

#endif

 void Req::OutDirTop(){ Send(HTMLDirHead,sizeof(HTMLDirHead)-1);}
 void Req::OutDirBottom(char *b)
 { Send("</table><hr>",sizeof("</table><hr>")-1); OutBaner(b);
   Send("</body></html>\n",sizeof("</body></html>"));
 };

#undef send
