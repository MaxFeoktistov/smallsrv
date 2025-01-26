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

#ifndef WBSTRING_CONST_H
#ifdef  RUS
#include "strcwmr.h"
#else
#define WBSTRING_CONST_H


#define s_METHOD_P " method=post><input type=hidden name=s value=21><table"\
" width=100%% border=1 bgcolor=#E0F8F0><tr bgcolor=#C0F0E0 align=center>"\
/* "<td><font" size=2>Open</td> */ \
"<td><font size=2> &nbsp; </td><td width=96%% ><font"\
" size=2>Message data</td>"\
/* <td><font size=2>Remote POP3</td>*/ \
"</tr>"
#define s_TD__FONT    "<tr><td><font size=2><input type=checkbox name=\"dl%X\" > </td>"\
                          "<td>"\
                          "<font size=2><a href='?s=20&r=%X' > <b>Subject:</b> %s </a><br>"\
                          "<table width=100%%>"\
                          "<tr>"\
                            "<td><font size=2>"\
                              "<b>From:</b>%s &lt;<a href=\"mailto:%s\">%s</a>&gt;"\
                            "</td>"\
                            "<td align=right><font size=2><b>To:</b> %s</td>"\
                          "</tr><tr>"\
                            "<td><font size=2><b>Date:</b> %s </td>"\
                            "<td align=right><font size=2><b>Size:</b> %u<br></td>"\
                          "</tr></table><hr style=\"width:94%%;text-align:center;border: 1px gray\"> %s... </td></tr>"


#define s_B__S__B_ "<b>%s</b><br><input type=checkbox name=d%X > Del"
#define s_BR__INPU "<br><input type=checkbox name=g%X >Get (%uKb)"
#define s_TD_BGCOL "<td bgcolor=#E0E0E8><b>POP3 server: <br><input"\
" type=text name=pop maxlength=64 value=\"\" > </td> "
#define s_TD_BGCOL0 "<td bgcolor=#E8E0E8><b>User: <br><input type=text"\
" name=user maxlength=64 value=\"\" > </td> "
#define s_TD_BGCOL1 "<td bgcolor=#E0E0E8><b>Password: <br><input"\
" type=password name=pwd maxlength=64 value=\"\" ></td> "
#define s__INPUT_T " <input type=checkbox checked name=save value=1>Save"\
" record<br> "
#define s__INPUT_T0 " <input type=checkbox checked name=savepwd"\
" value=1>Save password<br> "
#define s__INPUT_T1 " <input type=checkbox name=delmsg value=1>Delete"\
" messages from remote.<br> "
#define s__INPUT_T2 " <input type=checkbox %s name=auto value=1>Auto"\
" load by schedule  "
#define s_TD_BGCOL2 "<td bgcolor=#E0E0E8><font size=1><b><input"\
" type=checkbox checked name=limit value=1>Message limit<input type=text"\
" name=lmt maxlength=10 size=8%% value=\"20\" >Kb</b><br>"
#define s_OTHERWIS " Otherwise, try to receive only some<BR>first"\
" lines, and leave message. "
#define s_____N___ ",0);\n"\
 "function fn(i){\n"\
 "document.x.pop.value=srv[i];\n"\
 "document.x.user.value=usr[i];\n"\
 "document.x.pwd.value=pwd[i];\n"\
 "document.x.lmt.value=flg[i]&0xFFFF;\n"\
 "document.x.save.checked=(flg[i]>>16)&0x1;\n"\
 "document.x.savepwd.checked=(flg[i]>>17)&0x1;\n"\
 "document.x.delmsg.checked=(flg[i]>>18)&0x1;\n"\
 "document.x.limit.checked=(flg[i]>>19)&0x1;\n"\
 "document.x.auto.checked=(flg[i]>>20)&0x1;\n"\
 "};\n"\
 "//-->\n"\
 "</script>\n"\
 "<select name=sl value=0 onChange='fn(this.selectedIndex)' >\n"\
 "<option value=0>Do not connect to remote\n"



#define s_OPTION_V "<option value=%u>%s,%s\n"
#define s__TABLE__ "</table><br><input type=submit value=\"Submit\"></form><hr>"
#define s_H__BAD_M "<h3>Bad message id.</h3><hr>"
#define sBAD_MIME_ "Bad mime format"
#define s_TABLE_WI "<table width=100%% border=1 bgcolor=#E0F0E8><tr><td><table><tr><td><b>"\
" From: </b></td><td>%.64s &lt;%.64s&gt;</td></tr><tr><td><b>To:"\
" </b></td><td>%.64s &lt;%.64s&gt;</td></tr><tr><td><b>Date: </b></td><td>%.64s</td></tr><tr><td><b>Subject:"\
" </b></td><td>%.64s</td></tr></table></td></tr><tr><td bgcolor=#F0F0F8>"
#define s_METHOD_G " method=GET ><input type=hidden name=s value=9><input"\
" type=hidden name=r value=%X ><input type=hidden name=subj value=\"Re:%s\""\
" ><input type=hidden name=to value=\"%s\" >\n<input type=hidden"\
" name=to_email value=\"%s\" >\n<input type=hidden name=date value=\"%s\""\
" >\n<input type=submit value=\"Reply\" > </form> "\
"</td><td align=center"\
" ><form  "

// action="

#define s_METHOD_G1 " method=GET ><input type=hidden name=s value=19><input"\
" type=hidden name=r value=%X ><input type=hidden name=subj value=\"%s\""\
" ><input type=hidden name=to value=\"%s\" >\n<input type=hidden"\
" name=to_email value=\"%s\" >\n<input type=hidden name=date value=\"%s\""\
" >\n<input type=submit value=\"Edit\" > </form> "\
"</td><td align=center"\
" ><form  "

// action="

#define s_METHOD_G0 " method=GET ><input type=hidden name=s value=22><input"\
" type=hidden name=r value=%X ><input type=submit  value=\"Delete\""\
" ></form> </td></tr></table></td></tr></table> "
#define sRECEIVE_T1 "Receive timeout"
#define sCANNOT_GE "Cannot get host by name"
#define sCALL_TO_S "call to socket failed\n"
#define s_CALL_TO_ " Call to connect failed"
#define sREPLY_TIM "Reply timeout"
#define sCONNECTIO "Connection closed"
#define sBAD_REQUE "Bad request..<br>"
#define s_N_B_TRY_ "\n<b>Try to connect %s (%s)...<br>"
#define sUNKNOW_US "Unknow user<br>"
#define sLOGIN_OK_ "Login Ok %u messages, %u bytes. Check new...<BR>"
#define sMAILBOX_I "Mailbox is empty\r\n"
#define sERROR_CAN "Error can't get list.<BR>"
#define sERROR_CAN0 "Error can't get UIDL.<BR>"
#define sCAN_T_CRE "Can't creat file %s."
#define s_U_NEW_ME "%u new messages are loaded<br>"
#define s_S_____NE "?s=5' >New message</a></b></td><td><b><a href='"
#define s_S_____OP "?s=15' >Options</a></b></td><td><b><a"\
" href='"
#define s_S_____BL "?s=25' >Lists</a></b></td><td><b><a"\
" href='"
#define s_H__ACCOU "<h2>Account options</h2><form action="
#define s_H__LOAD_ "<h2>Load messages from other POP3 server:</h2><table"\
" width=100%% border=1 bgcolor=#F0E0E8>"
#define s_TD_BGCOL3 "<td bgcolor=#e0e0e8><b>POP3 server: <br><input"\
" type=text name=pop maxlength=64 value=\"%.64s\" > </td> "
#define s_TD_BGCOL4 "<td bgcolor=#e8e0e8><b>User: <br><input type=text"\
" name=user maxlength=64 value=\"%.64s\" > </td> "
#define s_TD_BGCOL5 "<td bgcolor=#e0e0e8><b>Password: <br><input"\
" type=password name=pwd maxlength=64 value=\"%.64s\" ></td> "
#define s__INPUT_T3 " <input type=checkbox %s name=save value=1>Save"\
" record<br>"
#define s__INPUT_T4 " <input type=checkbox %s name=savepwd value=1>Save"\
" password<br>"
#define s__INPUT_T5 " <input type=checkbox %s name=delmsg value=1>Delete"\
" messages from remote.<br>"
#define s__INPUT_T6 " <input type=checkbox %s name=auto value=1>Auto"\
" load by schedule "
#define s_TD_BGCOL6 "<td bgcolor=#e0e0e8><font size=1><b><input"\
" type=checkbox %s name=limit value=1>Message limit<input type=text"\
" name=lmt maxlength=10 size=8%% value=\"%u\" >Kb</b><br>"
#define s_OTHERWIS0 " Otherwise, script trys to receive only some<BR>first"\
" lines, and leave message. "
#define s_TD_BGCOL7 "<td bgcolor=#e8e0e8><input type=submit value=\"LOAD\"></td>"\
" "
#define sSTATUS___ "Status: 401\r\nWWW-Authenticate: Basic realm=\"Access"\
" to Web mail\"\r\nContent-Type: text/plain\r\n\r\n Autorisation request "
#define sFROM__S__ "From %s (via Web [%s]) %3.3s, %u %3.3s %4.4u"\
" %2.2u:%2.2u:%2.2u GMT For %s\r\nFrom: %.64s<%.64s>\r\nTo: %.64s<%.64s>\r\nReply-To:"\
" %.64s<%.64s>\r\nDate: %3.3s, %u %3.3s %4.4u %2.2u:%2.2u:%2.2u"\
" GMT\r\nSubject: %.256s\r\nX-Mailer: WebMail for Small HTTP server\r\n\r\n"
#define s_B_MESSAG "<b>Message for %s &lt;%s&gt; stored and will"\
" be send in nearest time </b><br>"




#define s_HEADER0 "<input type=hidden name=s value=2>"\
"<table width=100%% border=1 bgcolor=#E0F0E8>"\
"<tr><td>"\
"<b> Your name: <input type=text name=from maxlength=64 size=40%% value=\"%.64s\">&lt;%.255s&gt;</b> "\
"</td></tr><tr><td>"\
/* "<b>Reply-To: <input type=text name=reply maxlength=64 size=40%% value=\"%.64s\">&lt;<input type=text name=reto maxlength=64 size=30%% value=\"%.64s\">&gt;</b>"\
"</td></tr><tr><td>" */\
"<b>Default hello string:</b><br>"\
"<textarea name=hello rows=3 cols=80>\n%.512s\n"\
"</textarea>"\
"</td></tr><tr><td>"\
"<b>Reply template:</b><br>"\
"<textarea name=tmpl rows=5 cols=80 >%.1024s"\
"</textarea> "\
"<br>(Next variables are available: $to,$to_email,$subj,$date)"\
"</td></tr><tr><td>"\
"<b>Signature:</b><br>"\
"<textarea name=sign rows=3 cols=80 >\n%.512s"\
"</textarea>"\
"</td></tr><tr><td align=center >"\
"<input type=submit value=\"Submit changes\">"\
"</td></tr>"\
"</table>"\
"</form>"\
"<br>"\
"<br>"\
"<h2>Change password:</h2><form name=z method=post> <input type=hidden name=s value=3 >New password: <input type=password name=p size=32 maxlength=32> <input type=submit value='Change' > </form>  <br> <br> "


#define s_POP3a1 "<form name=nmsg method=post>"\
"<input type=hidden name=s value=7>"\
"<table width=100%% border=1 bgcolor=#E0F0E8>"\
"<tr><td>"\
"<table>"\
"<tr><td>"\
"<b> From: </b></td><td><input type=text name=from maxlength=64 size=50%% value=\"%.64s\" >&lt;%.255s&gt;"\
"</td></tr><tr><td>"\
"<b>To: </b></td><td><input type=text name=to maxlength=64 size=50%% value=\"%.64s\" >&lt;<input type=text name=tomail maxlength=64 size=30%% value=\"%.64s\" >&gt;"\
"</td></tr>"\
"<tr><td>"\
"<b>Subject: </b></td><td><input type=text name=subj maxlength=128 size=85%% value=\"%.64s\" >"\
"</td></tr>"\
"</table>"\
"</td></tr><tr><td>"\
"<textarea name=msg rows=12 cols=80 >\n%.512s\n\n\n"

#define s_OPEN000 "?s=20&r=%X\">Open</A></B>"

#define sWeb_mail "Web mail"



#define  LLIST "Each line of this list may be: " LF \
"<ul>" LF\
"<li> E-mail address or any part of address " LF\
"<li> IP address or part of IP address " LF\
"<li>? Logical expresion where you may do action with $sender, $hello, $control variables:" LF\
"<ul>" LF\
"<li>$sender -- return address " LF\
"<li>$hello -- self identification from remote server. " LF\
"<li>$control -- full identification line in flowing format:" LF\
" \"From sender (remote_hello [IP]) date and time For  receptors\\r\\n\"" LF\
"</ul>" LF\
"<li>?? List of DNSBL servers" LF


#define LLEND "<input type=button value='Check for IP address' onClick=\"javascript:AddE('10.0.0.1');\"><br>" LF\
"<input type=button value='Check for host.name' onClick=\"javascript:AddE('host.name');\"><br>" LF\
"<input type=button value='Check for source address by patern' onClick=\"javascript:AddEm('sender','user@host.net');\" ><br>" LF\
"<input type=button value='Check for hello by patern' onClick=\"javascript:AddEm('hello','host.net');\" ><br>" LF\
"<input type=button value='True if hello does not contenen the point'  onClick=\"javascript:AddE('? ! $hello =~ /\\\\./');\"  ><br> " LF\
"<input type=button value='Check for control line by patern' onClick=\"javascript:AddEm('control','host.net');\" ><br>" LF

#define  sFORWARD  "You may put \"forward\" file, to redirect or percolate messages.<br>" LF\
"Server parses each line of this file and understands next instruction:" LF\
"<li><b>#if </b><i>expression</i> -- next lines will be checked if expression is true" LF\
"<li><b>#elif </b><i>expression</i> --  if previous condition is false then next lines will be checked if expression is true" LF\
"<li><b>#else</b> -- next lines will be checked if previous condition is false" LF\
"<li><b>#endif</b> -- end of conditions block" LF\
"<li><b>#mv</b> <i>where</i> -- move message" LF\
"<li><b>#cp</b> <i>where</i> -- copy message" LF\
"<li><b>#rm</b>  -- remove message" LF\
"<li><b>#</b> <i>anything</i> -- comment" LF\
"<li><b>!</b><i>d:\\path\\application {params}</i> -- execute" LF\
"\"d:\\path\\application {params} users_home\\mbox\\name.msg\". If executing is enable in options only." LF\
"Also posible to  direct variable ($variable_name) in command line, but" LF\
"in this case:" LF\
"<ul>" LF\
"<li> -- \"users_home\\mbox\\name.msg\" will not be added automaticaly." LF\
"You may use variable $msgfile to insert name of message file." LF\
"<li>  -- the string consider to be interpretable,  the slash '\\' understud as" LF\
"begin of escape sequence,  replase it to two slash. (\"\\\\\")" LF\
"</ul>" LF\
"<li><i>to1<b>@</b>host1 {toN<b>@</b>hostN}</i> -- redirect message to this addresses." LF\
"<br> The conditions can consist of variables (<b>$size_kb</b> -- size of message in KB;<br>" LF\
"<b>$in_text(</b><i>text to find</i><b>)</b> -- is true if the text was found in the message;<br>" LF\
"<b>$text</b> -- full text with head;<br>" LF\
"<b>$body</b> -- message body;<br>" LF\
"<b>$errorlevel</b> -- the value returned by last external script or zero if no successful runs before)" LF\
"and values as well as different logical operators between them:" LF\
"<br>" LF\
"! -- \"Not\"<br>" LF\
"= or == -- \"Equal to\"<br>" LF\
"!= -- \"Not equal to\"<br>" LF\
"&lt;,&gt;,&lt;=,&gt;= -- \"Less than\", \"Greater than\", \"Less than or equal to\"," LF\
"\"Greater than or equal to\".<br>" LF\
"&amp;&amp; --\"AND\"<br>" LF\
"|| --\"OR\"<br><br>" LF\
"str1 =~ /pattern/<i>ig</i> -- pattern it is regular expressions like Unix." LF\
"The result is be true, if  in the string <i>str1</i>" LF\
"has been found substring equal by pattern. <br>" LF\
"" LF\
"Space and back-slash (' \\') at end of line mean continues current command" LF\
"at next line." LF\
"<br>" LF

#define sFORWARD2 "<input type=button value='Add redirect' onClick=\"javascript:AddE('name@host');\"><br>" LF\
"<input type=button value='Add redirect by condition' onClick=\"javascript:AddE('#if $text =~ /importen@address/\\nname@host\\n#endif');\"><br>" LF\
"<input type=button value='Remove message by condition' onClick=\"javascript:AddE('#if $text =~ /Viagra/\\n#rm\\n#endif');\"><br>" LF

#define  sMSGSENT   "Your message plased to queue for send <hr>"
#define  sMSGSENTERR   "Error send message  <hr>"
#define  sMSGSAVED   "Your message saved as draft<hr>"

#define sDownload_source "Download source"

#define sDelete "Delete checked"
#define sSend "Send"
#define sDraft "Save draft"
#define sSubmit "Submit"


#define  NSLST "Each line may content domain-name and RR  description and comment may" LF\
"begin with ';'   Next lines are supported:<br>" LF\
"$ORIGIN <i>&lt;domain-name&gt;</i><br>" LF\
"$TTL <i>&lt;validate-time&gt;</i>  -- a 32 bit unsigned integer that specifies" LF\
"the time interval (in seconds) that the resource record may be cached before it should be discarded.<br>" LF\
"$SLAVE <i>&lt;domain-name&gt; &lt;ip-address-of-master&gt; [&lt;filename&gt;]</i>  -- Work as slave DNS server for this domain. Download full domain from master<br>" LF\
"the time interval (in seconds) that the resource record may be cached before it should be discarded.<br>" LF\
"$IF_DOWN <i>&lt;host:port&gt; &lt;interval&gt; Old.IP=New.IP</i>  -- By this option server will try to connect to the host:port for time interval (in seconds), and if fail in each record with Old.IP it will replasing to New.IP.<br>" LF\
"[<i>&lt;domain-name&gt;</i>] <i>&lt;blank&gt;</i> [<i>&lt;TTL&gt;</i>] IN <i>&lt;type&gt; &lt;RDATA&gt;</i><br>" LF\
" You may skip <i>&lt;domain-name&gt;</i> in this case" LF\
"preveus name will be used. Unlike RFC recomendation you must" LF\
"direct  class \"IN\" for each line with RR format." LF\
"<i>&lt;type&gt;</i> may be:" LF\
"<li> A  <i>&lt;IP-address&gt;</i> - a host address" LF\
"<li> AAAA  <i>&lt;IPv6-address&gt;</i> - a host IPv6 address" LF\
"<li> NS <i>&lt;full-name&gt;</i> - an authoritative name server" LF\
"<li> CNAME <i>&lt;full-name&gt;</i> - the canonical name for an alias.  The 'A' record" LF\
"for original name MUST present in this file." LF\
"<li> SOA <i>&lt;full-name&gt;</i> <i>&lt;e-mail by owner&gt;</i> (<i>&lt;SERIAL&gt;</i>,<i>&lt;REFRESH&gt;</i>,<i>&lt;RETRY&gt;</i>,<i>&lt;EXPIRE&gt;</i>,<i>&lt;MINIMUM&gt;</i>)" LF\
" - marks the start of a zone of authority" LF\
"<li> MX <i>&lt;preference&gt;</i> <i>&lt;full-name&gt;</i> - mail exchange. <i>&lt;preference&gt;</i> is numbre" LF\
"from 1 to 255.  Lower values are preferred." LF\
"<br> Other types are ignored.<br>" LF\
"For each type of record domain-name may begin from wildcard '*.' to descrpibe all" LF\
"sub domains. Server supports ANY and AXFR type of request to return all about domain. For" LF\
"domain with wildcard  reply also will content wildcard. For other types" LF\
"of request reply will be without wildcard." LF

//#define s_NEWPWD "<h2>Change password:</h2><form name=y> <input type=hidden name=s value=3 >New password: <input type=password name=p size=32 maxlength=32> <input type=submit value='Change' > </form>  <br> <br> "

#define s_PWDChanged  "<hr>Password changed<hr>"

#endif
#endif
