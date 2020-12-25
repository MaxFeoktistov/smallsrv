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


#ifdef MAINSTR
 char S3s_METHOD_P[] = " method=post><input type=hidden name=s value=21><table"
 " width=100%% border=1 bgcolor=#E0F8F0><tr bgcolor=#C0F0E0 align=center>"
 /* "<td><font" size=2>Open</td> */ 
 "<td><font size=2> &nbsp; </td><td width=96%% ><font"
 " size=2>Message data</td>"
 /* <td><font size=2>Remote POP3</td>*/ 
 "</tr>";
 char *lS3s_METHOD_P = S3s_METHOD_P;
#else
 extern char *lS3s_METHOD_P;
 extern char S3s_METHOD_P[];
#endif
#define s_METHOD_P lS3s_METHOD_P
#define x_s_METHOD_P S3s_METHOD_P

#ifdef MAINSTR
 char S3s_TD__FONT[] = "<td><font size=2><a href='?s=20&r=%X' > <b>Subject:</b> %s </a><br><table"
 " width=100%%><tr><td><font size=2><b>From:</b>%s &lt;<a href=\"mailto:%s\""
 " >%s</a>&gt;</td> <td align=right><font size=2><b>To:</b> %s</td></tr><tr><td><font"
 " size=2><b>Date:</b> %s </td><td align=right><font size=2><b>Size:</b>"
 " %u<br></td></tr>";
 char *lS3s_TD__FONT = S3s_TD__FONT;
#else
 extern char *lS3s_TD__FONT;
 extern char S3s_TD__FONT[];
#endif
#define s_TD__FONT lS3s_TD__FONT
#define x_s_TD__FONT S3s_TD__FONT

#ifdef MAINSTR
 char S3s_B__S__B_[] = "<b>%s</b><br><input type=checkbox name=d%X > Del";
 char *lS3s_B__S__B_ = S3s_B__S__B_;
#else
 extern char *lS3s_B__S__B_;
 extern char S3s_B__S__B_[];
#endif
#define s_B__S__B_ lS3s_B__S__B_
#define x_s_B__S__B_ S3s_B__S__B_

#ifdef MAINSTR
 char S3s_BR__INPU[] = "<br><input type=checkbox name=g%X >Get (%uKb)";
 char *lS3s_BR__INPU = S3s_BR__INPU;
#else
 extern char *lS3s_BR__INPU;
 extern char S3s_BR__INPU[];
#endif
#define s_BR__INPU lS3s_BR__INPU
#define x_s_BR__INPU S3s_BR__INPU

#ifdef MAINSTR
 char S3s_TD_BGCOL[] = "<td bgcolor=#E0E0E8><b>POP3 server: <br><input"
 " type=text name=pop maxlength=64 value=\"\" > </td> ";
 char *lS3s_TD_BGCOL = S3s_TD_BGCOL;
#else
 extern char *lS3s_TD_BGCOL;
 extern char S3s_TD_BGCOL[];
#endif
#define s_TD_BGCOL lS3s_TD_BGCOL
#define x_s_TD_BGCOL S3s_TD_BGCOL

#ifdef MAINSTR
 char S3s_TD_BGCOL0[] = "<td bgcolor=#E8E0E8><b>User: <br><input type=text"
 " name=user maxlength=64 value=\"\" > </td> ";
 char *lS3s_TD_BGCOL0 = S3s_TD_BGCOL0;
#else
 extern char *lS3s_TD_BGCOL0;
 extern char S3s_TD_BGCOL0[];
#endif
#define s_TD_BGCOL0 lS3s_TD_BGCOL0
#define x_s_TD_BGCOL0 S3s_TD_BGCOL0

#ifdef MAINSTR
 char S3s_TD_BGCOL1[] = "<td bgcolor=#E0E0E8><b>Password: <br><input"
 " type=password name=pwd maxlength=64 value=\"\" ></td> ";
 char *lS3s_TD_BGCOL1 = S3s_TD_BGCOL1;
#else
 extern char *lS3s_TD_BGCOL1;
 extern char S3s_TD_BGCOL1[];
#endif
#define s_TD_BGCOL1 lS3s_TD_BGCOL1
#define x_s_TD_BGCOL1 S3s_TD_BGCOL1

#ifdef MAINSTR
 char S3s__INPUT_T[] = " <input type=checkbox checked name=save value=1>Save"
 " record<br> ";
 char *lS3s__INPUT_T = S3s__INPUT_T;
#else
 extern char *lS3s__INPUT_T;
 extern char S3s__INPUT_T[];
#endif
#define s__INPUT_T lS3s__INPUT_T
#define x_s__INPUT_T S3s__INPUT_T

#ifdef MAINSTR
 char S3s__INPUT_T0[] = " <input type=checkbox checked name=savepwd"
 " value=1>Save password<br> ";
 char *lS3s__INPUT_T0 = S3s__INPUT_T0;
#else
 extern char *lS3s__INPUT_T0;
 extern char S3s__INPUT_T0[];
#endif
#define s__INPUT_T0 lS3s__INPUT_T0
#define x_s__INPUT_T0 S3s__INPUT_T0

#ifdef MAINSTR
 char S3s__INPUT_T1[] = " <input type=checkbox name=delmsg value=1>Delete"
 " messages from remote.<br> ";
 char *lS3s__INPUT_T1 = S3s__INPUT_T1;
#else
 extern char *lS3s__INPUT_T1;
 extern char S3s__INPUT_T1[];
#endif
#define s__INPUT_T1 lS3s__INPUT_T1
#define x_s__INPUT_T1 S3s__INPUT_T1

#ifdef MAINSTR
 char S3s__INPUT_T2[] = " <input type=checkbox %s name=auto value=1>Auto"
 " load by schedule  ";
 char *lS3s__INPUT_T2 = S3s__INPUT_T2;
#else
 extern char *lS3s__INPUT_T2;
 extern char S3s__INPUT_T2[];
#endif
#define s__INPUT_T2 lS3s__INPUT_T2
#define x_s__INPUT_T2 S3s__INPUT_T2

#ifdef MAINSTR
 char S3s_TD_BGCOL2[] = "<td bgcolor=#E0E0E8><font size=1><b><input"
 " type=checkbox checked name=limit value=1>Message limit<input type=text"
 " name=lmt maxlength=10 size=8%% value=\"20\" >Kb</b><br>";
 char *lS3s_TD_BGCOL2 = S3s_TD_BGCOL2;
#else
 extern char *lS3s_TD_BGCOL2;
 extern char S3s_TD_BGCOL2[];
#endif
#define s_TD_BGCOL2 lS3s_TD_BGCOL2
#define x_s_TD_BGCOL2 S3s_TD_BGCOL2

#ifdef MAINSTR
 char S3s_OTHERWIS[] = " Otherwise, try to receive only some<BR>first"
 " lines, and leave message. ";
 char *lS3s_OTHERWIS = S3s_OTHERWIS;
#else
 extern char *lS3s_OTHERWIS;
 extern char S3s_OTHERWIS[];
#endif
#define s_OTHERWIS lS3s_OTHERWIS
#define x_s_OTHERWIS S3s_OTHERWIS

#ifdef MAINSTR
 char S3s_____N___[] = ",0);\n"
  "function fn(i){\n"
  "document.x.pop.value=srv[i];\n"
  "document.x.user.value=usr[i];\n"
  "document.x.pwd.value=pwd[i];\n"
  "document.x.lmt.value=flg[i]&0xFFFF;\n"
  "document.x.save.checked=(flg[i]>>16)&0x1;\n"
  "document.x.savepwd.checked=(flg[i]>>17)&0x1;\n"
  "document.x.delmsg.checked=(flg[i]>>18)&0x1;\n"
  "document.x.limit.checked=(flg[i]>>19)&0x1;\n"
  "document.x.auto.checked=(flg[i]>>20)&0x1;\n"
  "};\n"
  "//-->\n"
  "</script>\n"
  "<select name=sl value=0 onChange='fn(this.selectedIndex)' >\n"
  "<option value=0>Do not connect to remote\n";
 char *lS3s_____N___ = S3s_____N___;
#else
 extern char *lS3s_____N___;
 extern char S3s_____N___[];
#endif
#define s_____N___ lS3s_____N___
#define x_s_____N___ S3s_____N___




#ifdef MAINSTR
 char S3s_OPTION_V[] = "<option value=%u>%s,%s\n";
 char *lS3s_OPTION_V = S3s_OPTION_V;
#else
 extern char *lS3s_OPTION_V;
 extern char S3s_OPTION_V[];
#endif
#define s_OPTION_V lS3s_OPTION_V
#define x_s_OPTION_V S3s_OPTION_V

#ifdef MAINSTR
 char S3s__TABLE__[] = "</table><br><input type=submit value=\"Submit\"></form><hr>";
 char *lS3s__TABLE__ = S3s__TABLE__;
#else
 extern char *lS3s__TABLE__;
 extern char S3s__TABLE__[];
#endif
#define s__TABLE__ lS3s__TABLE__
#define x_s__TABLE__ S3s__TABLE__

#ifdef MAINSTR
 char S3s_H__BAD_M[] = "<h3>Bad message id.</h3><hr>";
 char *lS3s_H__BAD_M = S3s_H__BAD_M;
#else
 extern char *lS3s_H__BAD_M;
 extern char S3s_H__BAD_M[];
#endif
#define s_H__BAD_M lS3s_H__BAD_M
#define x_s_H__BAD_M S3s_H__BAD_M

#ifdef MAINSTR
 char S3sBAD_MIME_[] = "Bad mime format";
 char *lS3sBAD_MIME_ = S3sBAD_MIME_;
#else
 extern char *lS3sBAD_MIME_;
 extern char S3sBAD_MIME_[];
#endif
#define sBAD_MIME_ lS3sBAD_MIME_
#define x_sBAD_MIME_ S3sBAD_MIME_

#ifdef MAINSTR
 char S3s_TABLE_WI[] = "<table width=100%% border=1 bgcolor=#E0F0E8><tr><td><table><tr><td><b>"
 " From: </b></td><td>%.64s &lt;%.64s&gt;</td></tr><tr><td><b>To:"
 " </b></td><td>%.64s &lt;%.64s&gt;</td></tr><tr><td><b>Date: </b></td><td>%.64s</td></tr><tr><td><b>Subject:"
 " </b></td><td>%.64s</td></tr></table></td></tr><tr><td bgcolor=#F0F0F8>";
 char *lS3s_TABLE_WI = S3s_TABLE_WI;
#else
 extern char *lS3s_TABLE_WI;
 extern char S3s_TABLE_WI[];
#endif
#define s_TABLE_WI lS3s_TABLE_WI
#define x_s_TABLE_WI S3s_TABLE_WI

#ifdef MAINSTR
 char S3s_METHOD_G[] = " method=GET ><input type=hidden name=s value=9><input"
 " type=hidden name=r value=%X ><input type=hidden name=subj value=\"Re:%s\""
 " ><input type=hidden name=to value=\"%s\" >\n<input type=hidden"
 " name=to_email value=\"%s\" >\n<input type=hidden name=date value=\"%s\""
 " >\n<input type=submit value=\"Reply\" > </form> "
 "</td><td align=center"
 " ><form  ";
 char *lS3s_METHOD_G = S3s_METHOD_G;
#else
 extern char *lS3s_METHOD_G;
 extern char S3s_METHOD_G[];
#endif
#define s_METHOD_G lS3s_METHOD_G
#define x_s_METHOD_G S3s_METHOD_G


// action="

#ifdef MAINSTR
 char S3s_METHOD_G1[] = " method=GET ><input type=hidden name=s value=19><input"
 " type=hidden name=r value=%X ><input type=hidden name=subj value=\"%s\""
 " ><input type=hidden name=to value=\"%s\" >\n<input type=hidden"
 " name=to_email value=\"%s\" >\n<input type=hidden name=date value=\"%s\""
 " >\n<input type=submit value=\"Edit\" > </form> "
 "</td><td align=center"
 " ><form  ";
 char *lS3s_METHOD_G1 = S3s_METHOD_G1;
#else
 extern char *lS3s_METHOD_G1;
 extern char S3s_METHOD_G1[];
#endif
#define s_METHOD_G1 lS3s_METHOD_G1
#define x_s_METHOD_G1 S3s_METHOD_G1


// action="

#ifdef MAINSTR
 char S3s_METHOD_G0[] = " method=GET ><input type=hidden name=s value=22><input"
 " type=hidden name=r value=%X ><input type=submit  value=\"Delete\""
 " ></form> </td></tr></table></td></tr></table> ";
 char *lS3s_METHOD_G0 = S3s_METHOD_G0;
#else
 extern char *lS3s_METHOD_G0;
 extern char S3s_METHOD_G0[];
#endif
#define s_METHOD_G0 lS3s_METHOD_G0
#define x_s_METHOD_G0 S3s_METHOD_G0

#ifdef MAINSTR
 char S3sRECEIVE_T1[] = "Receive timeout";
 char *lS3sRECEIVE_T1 = S3sRECEIVE_T1;
#else
 extern char *lS3sRECEIVE_T1;
 extern char S3sRECEIVE_T1[];
#endif
#define sRECEIVE_T1 lS3sRECEIVE_T1
#define x_sRECEIVE_T1 S3sRECEIVE_T1

#ifdef MAINSTR
 char S3sCANNOT_GE[] = "Cannot get host by name";
 char *lS3sCANNOT_GE = S3sCANNOT_GE;
#else
 extern char *lS3sCANNOT_GE;
 extern char S3sCANNOT_GE[];
#endif
#define sCANNOT_GE lS3sCANNOT_GE
#define x_sCANNOT_GE S3sCANNOT_GE

#ifdef MAINSTR
 char S3sCALL_TO_S[] = "call to socket failed\n";
 char *lS3sCALL_TO_S = S3sCALL_TO_S;
#else
 extern char *lS3sCALL_TO_S;
 extern char S3sCALL_TO_S[];
#endif
#define sCALL_TO_S lS3sCALL_TO_S
#define x_sCALL_TO_S S3sCALL_TO_S

#ifdef MAINSTR
 char S3s_CALL_TO_[] = " Call to connect failed";
 char *lS3s_CALL_TO_ = S3s_CALL_TO_;
#else
 extern char *lS3s_CALL_TO_;
 extern char S3s_CALL_TO_[];
#endif
#define s_CALL_TO_ lS3s_CALL_TO_
#define x_s_CALL_TO_ S3s_CALL_TO_

#ifdef MAINSTR
 char S3sREPLY_TIM[] = "Reply timeout";
 char *lS3sREPLY_TIM = S3sREPLY_TIM;
#else
 extern char *lS3sREPLY_TIM;
 extern char S3sREPLY_TIM[];
#endif
#define sREPLY_TIM lS3sREPLY_TIM
#define x_sREPLY_TIM S3sREPLY_TIM

#ifdef MAINSTR
 char S3sCONNECTIO[] = "Connection closed";
 char *lS3sCONNECTIO = S3sCONNECTIO;
#else
 extern char *lS3sCONNECTIO;
 extern char S3sCONNECTIO[];
#endif
#define sCONNECTIO lS3sCONNECTIO
#define x_sCONNECTIO S3sCONNECTIO

#ifdef MAINSTR
 char S3sBAD_REQUE[] = "Bad request..<br>";
 char *lS3sBAD_REQUE = S3sBAD_REQUE;
#else
 extern char *lS3sBAD_REQUE;
 extern char S3sBAD_REQUE[];
#endif
#define sBAD_REQUE lS3sBAD_REQUE
#define x_sBAD_REQUE S3sBAD_REQUE

#ifdef MAINSTR
 char S3s_N_B_TRY_[] = "\n<b>Try to connect %s (%s)...<br>";
 char *lS3s_N_B_TRY_ = S3s_N_B_TRY_;
#else
 extern char *lS3s_N_B_TRY_;
 extern char S3s_N_B_TRY_[];
#endif
#define s_N_B_TRY_ lS3s_N_B_TRY_
#define x_s_N_B_TRY_ S3s_N_B_TRY_

#ifdef MAINSTR
 char S3sUNKNOW_US[] = "Unknow user<br>";
 char *lS3sUNKNOW_US = S3sUNKNOW_US;
#else
 extern char *lS3sUNKNOW_US;
 extern char S3sUNKNOW_US[];
#endif
#define sUNKNOW_US lS3sUNKNOW_US
#define x_sUNKNOW_US S3sUNKNOW_US

#ifdef MAINSTR
 char S3sLOGIN_OK_[] = "Login Ok %u messages, %u bytes. Check new...<BR>";
 char *lS3sLOGIN_OK_ = S3sLOGIN_OK_;
#else
 extern char *lS3sLOGIN_OK_;
 extern char S3sLOGIN_OK_[];
#endif
#define sLOGIN_OK_ lS3sLOGIN_OK_
#define x_sLOGIN_OK_ S3sLOGIN_OK_

#ifdef MAINSTR
 char S3sMAILBOX_I[] = "Mailbox is empty\r\n";
 char *lS3sMAILBOX_I = S3sMAILBOX_I;
#else
 extern char *lS3sMAILBOX_I;
 extern char S3sMAILBOX_I[];
#endif
#define sMAILBOX_I lS3sMAILBOX_I
#define x_sMAILBOX_I S3sMAILBOX_I

#ifdef MAINSTR
 char S3sERROR_CAN[] = "Error can't get list.<BR>";
 char *lS3sERROR_CAN = S3sERROR_CAN;
#else
 extern char *lS3sERROR_CAN;
 extern char S3sERROR_CAN[];
#endif
#define sERROR_CAN lS3sERROR_CAN
#define x_sERROR_CAN S3sERROR_CAN

#ifdef MAINSTR
 char S3sERROR_CAN0[] = "Error can't get UIDL.<BR>";
 char *lS3sERROR_CAN0 = S3sERROR_CAN0;
#else
 extern char *lS3sERROR_CAN0;
 extern char S3sERROR_CAN0[];
#endif
#define sERROR_CAN0 lS3sERROR_CAN0
#define x_sERROR_CAN0 S3sERROR_CAN0

#ifdef MAINSTR
 char S3sCAN_T_CRE[] = "Can't creat file %s.";
 char *lS3sCAN_T_CRE = S3sCAN_T_CRE;
#else
 extern char *lS3sCAN_T_CRE;
 extern char S3sCAN_T_CRE[];
#endif
#define sCAN_T_CRE lS3sCAN_T_CRE
#define x_sCAN_T_CRE S3sCAN_T_CRE

#ifdef MAINSTR
 char S3s_U_NEW_ME[] = "%u new messages are loaded<br>";
 char *lS3s_U_NEW_ME = S3s_U_NEW_ME;
#else
 extern char *lS3s_U_NEW_ME;
 extern char S3s_U_NEW_ME[];
#endif
#define s_U_NEW_ME lS3s_U_NEW_ME
#define x_s_U_NEW_ME S3s_U_NEW_ME

#ifdef MAINSTR
 char S3s_S_____NE[] = "?s=5' >New message</a></b></td><td><b><a href='";
 char *lS3s_S_____NE = S3s_S_____NE;
#else
 extern char *lS3s_S_____NE;
 extern char S3s_S_____NE[];
#endif
#define s_S_____NE lS3s_S_____NE
#define x_s_S_____NE S3s_S_____NE

#ifdef MAINSTR
 char S3s_S_____OP[] = "?s=15' >Options</a></b></td><td><b><a"
 " href='";
 char *lS3s_S_____OP = S3s_S_____OP;
#else
 extern char *lS3s_S_____OP;
 extern char S3s_S_____OP[];
#endif
#define s_S_____OP lS3s_S_____OP
#define x_s_S_____OP S3s_S_____OP

#ifdef MAINSTR
 char S3s_S_____BL[] = "?s=25' >Lists</a></b></td><td><b><a"
 " href='";
 char *lS3s_S_____BL = S3s_S_____BL;
#else
 extern char *lS3s_S_____BL;
 extern char S3s_S_____BL[];
#endif
#define s_S_____BL lS3s_S_____BL
#define x_s_S_____BL S3s_S_____BL

#ifdef MAINSTR
 char S3s_H__ACCOU[] = "<h2>Account options</h2><form action=";
 char *lS3s_H__ACCOU = S3s_H__ACCOU;
#else
 extern char *lS3s_H__ACCOU;
 extern char S3s_H__ACCOU[];
#endif
#define s_H__ACCOU lS3s_H__ACCOU
#define x_s_H__ACCOU S3s_H__ACCOU

#ifdef MAINSTR
 char S3s_H__LOAD_[] = "<h2>Load messages from other POP3 server:</h2><table"
 " width=100%% border=1 bgcolor=#F0E0E8>";
 char *lS3s_H__LOAD_ = S3s_H__LOAD_;
#else
 extern char *lS3s_H__LOAD_;
 extern char S3s_H__LOAD_[];
#endif
#define s_H__LOAD_ lS3s_H__LOAD_
#define x_s_H__LOAD_ S3s_H__LOAD_

#ifdef MAINSTR
 char S3s_TD_BGCOL3[] = "<td bgcolor=#e0e0e8><b>POP3 server: <br><input"
 " type=text name=pop maxlength=64 value=\"%.64s\" > </td> ";
 char *lS3s_TD_BGCOL3 = S3s_TD_BGCOL3;
#else
 extern char *lS3s_TD_BGCOL3;
 extern char S3s_TD_BGCOL3[];
#endif
#define s_TD_BGCOL3 lS3s_TD_BGCOL3
#define x_s_TD_BGCOL3 S3s_TD_BGCOL3

#ifdef MAINSTR
 char S3s_TD_BGCOL4[] = "<td bgcolor=#e8e0e8><b>User: <br><input type=text"
 " name=user maxlength=64 value=\"%.64s\" > </td> ";
 char *lS3s_TD_BGCOL4 = S3s_TD_BGCOL4;
#else
 extern char *lS3s_TD_BGCOL4;
 extern char S3s_TD_BGCOL4[];
#endif
#define s_TD_BGCOL4 lS3s_TD_BGCOL4
#define x_s_TD_BGCOL4 S3s_TD_BGCOL4

#ifdef MAINSTR
 char S3s_TD_BGCOL5[] = "<td bgcolor=#e0e0e8><b>Password: <br><input"
 " type=password name=pwd maxlength=64 value=\"%.64s\" ></td> ";
 char *lS3s_TD_BGCOL5 = S3s_TD_BGCOL5;
#else
 extern char *lS3s_TD_BGCOL5;
 extern char S3s_TD_BGCOL5[];
#endif
#define s_TD_BGCOL5 lS3s_TD_BGCOL5
#define x_s_TD_BGCOL5 S3s_TD_BGCOL5

#ifdef MAINSTR
 char S3s__INPUT_T3[] = " <input type=checkbox %s name=save value=1>Save"
 " record<br>";
 char *lS3s__INPUT_T3 = S3s__INPUT_T3;
#else
 extern char *lS3s__INPUT_T3;
 extern char S3s__INPUT_T3[];
#endif
#define s__INPUT_T3 lS3s__INPUT_T3
#define x_s__INPUT_T3 S3s__INPUT_T3

#ifdef MAINSTR
 char S3s__INPUT_T4[] = " <input type=checkbox %s name=savepwd value=1>Save"
 " password<br>";
 char *lS3s__INPUT_T4 = S3s__INPUT_T4;
#else
 extern char *lS3s__INPUT_T4;
 extern char S3s__INPUT_T4[];
#endif
#define s__INPUT_T4 lS3s__INPUT_T4
#define x_s__INPUT_T4 S3s__INPUT_T4

#ifdef MAINSTR
 char S3s__INPUT_T5[] = " <input type=checkbox %s name=delmsg value=1>Delete"
 " messages from remote.<br>";
 char *lS3s__INPUT_T5 = S3s__INPUT_T5;
#else
 extern char *lS3s__INPUT_T5;
 extern char S3s__INPUT_T5[];
#endif
#define s__INPUT_T5 lS3s__INPUT_T5
#define x_s__INPUT_T5 S3s__INPUT_T5

#ifdef MAINSTR
 char S3s__INPUT_T6[] = " <input type=checkbox %s name=auto value=1>Auto"
 " load by schedule ";
 char *lS3s__INPUT_T6 = S3s__INPUT_T6;
#else
 extern char *lS3s__INPUT_T6;
 extern char S3s__INPUT_T6[];
#endif
#define s__INPUT_T6 lS3s__INPUT_T6
#define x_s__INPUT_T6 S3s__INPUT_T6

#ifdef MAINSTR
 char S3s_TD_BGCOL6[] = "<td bgcolor=#e0e0e8><font size=1><b><input"
 " type=checkbox %s name=limit value=1>Message limit<input type=text"
 " name=lmt maxlength=10 size=8%% value=\"%u\" >Kb</b><br>";
 char *lS3s_TD_BGCOL6 = S3s_TD_BGCOL6;
#else
 extern char *lS3s_TD_BGCOL6;
 extern char S3s_TD_BGCOL6[];
#endif
#define s_TD_BGCOL6 lS3s_TD_BGCOL6
#define x_s_TD_BGCOL6 S3s_TD_BGCOL6

#ifdef MAINSTR
 char S3s_OTHERWIS0[] = " Otherwise, script trys to receive only some<BR>first"
 " lines, and leave message. ";
 char *lS3s_OTHERWIS0 = S3s_OTHERWIS0;
#else
 extern char *lS3s_OTHERWIS0;
 extern char S3s_OTHERWIS0[];
#endif
#define s_OTHERWIS0 lS3s_OTHERWIS0
#define x_s_OTHERWIS0 S3s_OTHERWIS0

#ifdef MAINSTR
 char S3s_TD_BGCOL7[] = "<td bgcolor=#e8e0e8><input type=submit value=\"LOAD\"></td>"
 " ";
 char *lS3s_TD_BGCOL7 = S3s_TD_BGCOL7;
#else
 extern char *lS3s_TD_BGCOL7;
 extern char S3s_TD_BGCOL7[];
#endif
#define s_TD_BGCOL7 lS3s_TD_BGCOL7
#define x_s_TD_BGCOL7 S3s_TD_BGCOL7

#ifdef MAINSTR
 char S3sSTATUS___[] = "Status: 401\r\nWWW-Authenticate: Basic realm=\"Access"
 " to Web mail\"\r\nContent-Type: text/plain\r\n\r\n Autorisation request ";
 char *lS3sSTATUS___ = S3sSTATUS___;
#else
 extern char *lS3sSTATUS___;
 extern char S3sSTATUS___[];
#endif
#define sSTATUS___ lS3sSTATUS___
#define x_sSTATUS___ S3sSTATUS___

#ifdef MAINSTR
 char S3sFROM__S__[] = "From %s (via Web [%s]) %3.3s, %u %3.3s %4.4u"
 " %2.2u:%2.2u:%2.2u GMT For %s\r\nFrom: %.64s<%.64s>\r\nTo: %.64s<%.64s>\r\nReply-To:"
 " %.64s<%.64s>\r\nDate: %3.3s, %u %3.3s %4.4u %2.2u:%2.2u:%2.2u"
 " GMT\r\nSubject: %.256s\r\nX-Mailer: WebMail for Small HTTP server\r\n\r\n";
 char *lS3sFROM__S__ = S3sFROM__S__;
#else
 extern char *lS3sFROM__S__;
 extern char S3sFROM__S__[];
#endif
#define sFROM__S__ lS3sFROM__S__
#define x_sFROM__S__ S3sFROM__S__

#ifdef MAINSTR
 char S3s_B_MESSAG[] = "<b>Message for %s &lt;%s&gt; stored and will"
 " be send in nearest time </b><br>";
 char *lS3s_B_MESSAG = S3s_B_MESSAG;
#else
 extern char *lS3s_B_MESSAG;
 extern char S3s_B_MESSAG[];
#endif
#define s_B_MESSAG lS3s_B_MESSAG
#define x_s_B_MESSAG S3s_B_MESSAG





#ifdef MAINSTR
 char S3s_HEADER0[] = "<input type=hidden name=s value=2>"
 "<table width=100%% border=1 bgcolor=#E0F0E8>"
 "<tr><td>"
 "<b> Your name: <input type=text name=from maxlength=64 size=40%% value=\"%.64s\">&lt;%.255s&gt;</b> "
 "</td></tr><tr><td>"
 /* "<b>Reply-To: <input type=text name=reply maxlength=64 size=40%% value=\"%.64s\">&lt;<input type=text name=reto maxlength=64 size=30%% value=\"%.64s\">&gt;</b>"
 "</td></tr><tr><td>" */
 "<b>Default hello string:</b><br>"
 "<textarea name=hello rows=3 cols=80>\n%.512s\n"
 "</textarea>"
 "</td></tr><tr><td>"
 "<b>Reply template:</b><br>"
 "<textarea name=tmpl rows=5 cols=80 >%.1024s"
 "</textarea> "
 "<br>(Next variables are available: $to,$to_email,$subj,$date)"
 "</td></tr><tr><td>"
 "<b>Signature:</b><br>"
 "<textarea name=sign rows=3 cols=80 >\n%.512s"
 "</textarea>"
 "</td></tr><tr><td align=center >"
 "<input type=submit value=\"Submit changes\">"
 "</td></tr>"
 "</table>"
 "</form>"
 "<br>"
 "<br>"
 "<h2>Change password:</h2><form name=z method=post> <input type=hidden name=s value=3 >New password: <input type=password name=p size=32 maxlength=32> <input type=submit value='Change' > </form>  <br> <br> ";
 char *lS3s_HEADER0 = S3s_HEADER0;
#else
 extern char *lS3s_HEADER0;
 extern char S3s_HEADER0[];
#endif
#define s_HEADER0 lS3s_HEADER0
#define x_s_HEADER0 S3s_HEADER0



#ifdef MAINSTR
 char S3s_POP3a1[] = "<input type=hidden name=s value=7>"
 "<table width=100%% border=1 bgcolor=#E0F0E8>"
 "<tr><td>"
 "<table>"
 "<tr><td>"
 "<b> From: </b></td><td><input type=text name=from maxlength=64 size=50%% value=\"%.64s\" >&lt;%.255s&gt;"
 "</td></tr><tr><td>"
 "<b>To: </b></td><td><input type=text name=to maxlength=64 size=50%% value=\"%.64s\" >&lt;<input type=text name=tomail maxlength=64 size=30%% value=\"%.64s\" >&gt;"
 "</td></tr>"
 /* "<tr><td>Reply-To: </td><td><input type=text name=reply maxlength=64 size=50%% value=\"%.64s\" >&lt;<input type=text name=reto maxlength=64 size=30%% value=\"%.64s\" >&gt; (may be skiped)"
 "</td></tr>" */ "<tr><td>"
 "<b>Subject: </b></td><td><input type=text name=subj maxlength=128 size=85%% value=\"%.64s\" >"
 "</td></tr>"
 "</table>"
 "</td></tr><tr><td>"
 "<textarea name=msg rows=12 cols=80 >\n%.512s\n\n\n";
 char *lS3s_POP3a1 = S3s_POP3a1;
#else
 extern char *lS3s_POP3a1;
 extern char S3s_POP3a1[];
#endif
#define s_POP3a1 lS3s_POP3a1
#define x_s_POP3a1 S3s_POP3a1


#ifdef MAINSTR
 char S3s_OPEN000[] = "?s=20&r=%X\">Open</A></B>";
#else
extern char *S3s_OPEN000;
#endif
#define s_OPEN000 S3s_OPEN000
#define x_s_OPEN000 "?s=20&r=%X\">Open</A></B>"

#ifdef MAINSTR
 char S3sWeb_mail[] = "Web mail";
 char *lS3sWeb_mail = S3sWeb_mail;
#else
 extern char *lS3sWeb_mail;
 extern char S3sWeb_mail[];
#endif
#define sWeb_mail lS3sWeb_mail
#define x_sWeb_mail S3sWeb_mail




#ifdef MAINSTR
 char S3LLIST[] = "Each line of this list may be: " LF 
 "<ul>" LF
 "<li> E-mail address or any part of address " LF
 "<li> IP address or part of IP address " LF
 "<li>? Logical expresion where you may do action with $sender, $hello, $control variables:" LF
 "<ul>" LF
 "<li>$sender -- return address " LF
 "<li>$hello -- self identification from remote server. " LF
 "<li>$control -- full identification line in flowing format:" LF
 " \"From sender (remote_hello [IP]) date and time For  receptors\\r\\n\"" LF
 "</ul>" LF
 "<li>?? List of DNSBL servers" LF;
 char *lS3LLIST = S3LLIST;
#else
 extern char *lS3LLIST;
 extern char S3LLIST[];
#endif
#define LLIST lS3LLIST
#define x_LLIST S3LLIST



#ifdef MAINSTR
 char S3LLEND[] = "<input type=button value='Check for IP address' onClick=\"javascript:AddE('10.0.0.1');\"><br>" LF
 "<input type=button value='Check for host.name' onClick=\"javascript:AddE('host.name');\"><br>" LF
 "<input type=button value='Check for source address by patern' onClick=\"javascript:AddEm('sender','user@host.net');\" ><br>" LF
 "<input type=button value='Check for hello by patern' onClick=\"javascript:AddEm('hello','host.net');\" ><br>" LF
 "<input type=button value='True if hello does not contenen the point'  onClick=\"javascript:AddE('? ! $hello =~ /\\\\./');\"  ><br> " LF
 "<input type=button value='Check for control line by patern' onClick=\"javascript:AddEm('control','host.net');\" ><br>" LF;
 char *lS3LLEND = S3LLEND;
#else
 extern char *lS3LLEND;
 extern char S3LLEND[];
#endif
#define LLEND lS3LLEND
#define x_LLEND S3LLEND


#ifdef MAINSTR
 char S3sFORWARD[] = "You may put \"forward\" file, to redirect or percolate messages.<br>" LF
 "Server parses each line of this file and understands next instruction:" LF
 "<li><b>#if </b><i>expression</i> -- next lines will be checked if expression is true" LF
 "<li><b>#elif </b><i>expression</i> --  if previous condition is false then next lines will be checked if expression is true" LF
 "<li><b>#else</b> -- next lines will be checked if previous condition is false" LF
 "<li><b>#endif</b> -- end of conditions block" LF
 "<li><b>#mv</b> <i>where</i> -- move message" LF
 "<li><b>#cp</b> <i>where</i> -- copy message" LF
 "<li><b>#rm</b>  -- remove message" LF
 "<li><b>#</b> <i>anything</i> -- comment" LF
 "<li><b>!</b><i>d:\\path\\application {params}</i> -- execute" LF
 "\"d:\\path\\application {params} users_home\\mbox\\name.msg\". If executing is enable in options only." LF
 "Also posible to  direct variable ($variable_name) in command line, but" LF
 "in this case:" LF
 "<ul>" LF
 "<li> -- \"users_home\\mbox\\name.msg\" will not be added automaticaly." LF
 "You may use variable $msgfile to insert name of message file." LF
 "<li>  -- the string consider to be interpretable,  the slash '\\' understud as" LF
 "begin of escape sequence,  replase it to two slash. (\"\\\\\")" LF
 "</ul>" LF
 "<li><i>to1<b>@</b>host1 {toN<b>@</b>hostN}</i> -- redirect message to this addresses." LF
 "<br> The conditions can consist of variables (<b>$size_kb</b> -- size of message in KB;<br>" LF
 "<b>$in_text(</b><i>text to find</i><b>)</b> -- is true if the text was found in the message;<br>" LF
 "<b>$text</b> -- full text with head;<br>" LF
 "<b>$body</b> -- message body;<br>" LF
 "<b>$errorlevel</b> -- the value returned by last external script or zero if no successful runs before)" LF
 "and values as well as different logical operators between them:" LF
 "<br>" LF
 "! -- \"Not\"<br>" LF
 "= or == -- \"Equal to\"<br>" LF
 "!= -- \"Not equal to\"<br>" LF
 "&lt;,&gt;,&lt;=,&gt;= -- \"Less than\", \"Greater than\", \"Less than or equal to\"," LF
 "\"Greater than or equal to\".<br>" LF
 "&amp;&amp; --\"AND\"<br>" LF
 "|| --\"OR\"<br><br>" LF
 "str1 =~ /pattern/<i>ig</i> -- pattern it is regular expressions like Unix." LF
 "The result is be true, if  in the string <i>str1</i>" LF
 "has been found substring equal by pattern. <br>" LF
 "" LF
 "Space and back-slash (' \\') at end of line mean continues current command" LF
 "at next line." LF
 "<br>" LF;
 char *lS3sFORWARD = S3sFORWARD;
#else
 extern char *lS3sFORWARD;
 extern char S3sFORWARD[];
#endif
#define sFORWARD lS3sFORWARD
#define x_sFORWARD S3sFORWARD


#ifdef MAINSTR
 char S3sFORWARD2[] = "<input type=button value='Add redirect' onClick=\"javascript:AddE('name@host');\"><br>" LF
 "<input type=button value='Add redirect by condition' onClick=\"javascript:AddE('#if $text =~ /importen@address/\\nname@host\\n#endif');\"><br>" LF
 "<input type=button value='Remove message by condition' onClick=\"javascript:AddE('#if $text =~ /Viagra/\\n#rm\\n#endif');\"><br>" LF;
 char *lS3sFORWARD2 = S3sFORWARD2;
#else
 extern char *lS3sFORWARD2;
 extern char S3sFORWARD2[];
#endif
#define sFORWARD2 lS3sFORWARD2
#define x_sFORWARD2 S3sFORWARD2


#ifdef MAINSTR
 char S3sMSGSENT[] = "Your message plased to queue for send <hr>";
 char *lS3sMSGSENT = S3sMSGSENT;
#else
 extern char *lS3sMSGSENT;
 extern char S3sMSGSENT[];
#endif
#define sMSGSENT lS3sMSGSENT
#define x_sMSGSENT S3sMSGSENT

#ifdef MAINSTR
 char S3sMSGSENTERR[] = "Error send message  <hr>";
 char *lS3sMSGSENTERR = S3sMSGSENTERR;
#else
 extern char *lS3sMSGSENTERR;
 extern char S3sMSGSENTERR[];
#endif
#define sMSGSENTERR lS3sMSGSENTERR
#define x_sMSGSENTERR S3sMSGSENTERR

#ifdef MAINSTR
 char S3sMSGSAVED[] = "Your message saved as draft<hr>";
 char *lS3sMSGSAVED = S3sMSGSAVED;
#else
 extern char *lS3sMSGSAVED;
 extern char S3sMSGSAVED[];
#endif
#define sMSGSAVED lS3sMSGSAVED
#define x_sMSGSAVED S3sMSGSAVED


#ifdef MAINSTR
 char S3sDownload_source[] = "Download source";
 char *lS3sDownload_source = S3sDownload_source;
#else
 extern char *lS3sDownload_source;
 extern char S3sDownload_source[];
#endif
#define sDownload_source lS3sDownload_source
#define x_sDownload_source S3sDownload_source


#ifdef MAINSTR
 char S3sDelete[] = "Delete";
#else
extern char *S3sDelete;
#endif
#define sDelete S3sDelete
#define x_sDelete "Delete"
#ifdef MAINSTR
 char S3sSend[] = "Send";
#else
extern char *S3sSend;
#endif
#define sSend S3sSend
#define x_sSend "Send"
#ifdef MAINSTR
 char S3sDraft[] = "Save draft";
 char *lS3sDraft = S3sDraft;
#else
 extern char *lS3sDraft;
 extern char S3sDraft[];
#endif
#define sDraft lS3sDraft
#define x_sDraft S3sDraft

#ifdef MAINSTR
 char S3sSubmit[] = "Submit";
#else
extern char *S3sSubmit;
#endif
#define sSubmit S3sSubmit
#define x_sSubmit "Submit"


#ifdef MAINSTR
 char S3NSLST[] = "Each line may content domain-name and RR  description and comment may" LF
 "begin with ';'   Next lines are supported:<br>" LF
 "$ORIGIN <i>&lt;domain-name&gt;</i><br>" LF
 "$TTL <i>&lt;validate-time&gt;</i>  -- a 32 bit unsigned integer that specifies" LF
 "the time interval (in seconds) that the resource record may be cached before it should be discarded.<br>" LF
 "$SLAVE <i>&lt;domain-name&gt; &lt;ip-address-of-master&gt; [&lt;filename&gt;]</i>  -- Work as slave DNS server for this domain. Download full domain from master<br>" LF
 "the time interval (in seconds) that the resource record may be cached before it should be discarded.<br>" LF
 "$IF_DOWN <i>&lt;host:port&gt; &lt;interval&gt; Old.IP=New.IP</i>  -- By this option server will try to connect to the host:port for time interval (in seconds), and if fail in each record with Old.IP it will replasing to New.IP.<br>" LF
 "[<i>&lt;domain-name&gt;</i>] <i>&lt;blank&gt;</i> [<i>&lt;TTL&gt;</i>] IN <i>&lt;type&gt; &lt;RDATA&gt;</i><br>" LF
 " You may skip <i>&lt;domain-name&gt;</i> in this case" LF
 "preveus name will be used. Unlike RFC recomendation you must" LF
 "direct  class \"IN\" for each line with RR format." LF
 "<i>&lt;type&gt;</i> may be:" LF
 "<li> A  <i>&lt;IP-address&gt;</i> - a host address" LF
 "<li> AAAA  <i>&lt;IPv6-address&gt;</i> - a host IPv6 address" LF
 "<li> NS <i>&lt;full-name&gt;</i> - an authoritative name server" LF
 "<li> CNAME <i>&lt;full-name&gt;</i> - the canonical name for an alias.  The 'A' record" LF
 "for original name MUST present in this file." LF
 "<li> SOA <i>&lt;full-name&gt;</i> <i>&lt;e-mail by owner&gt;</i> (<i>&lt;SERIAL&gt;</i>,<i>&lt;REFRESH&gt;</i>,<i>&lt;RETRY&gt;</i>,<i>&lt;EXPIRE&gt;</i>,<i>&lt;MINIMUM&gt;</i>)" LF
 " - marks the start of a zone of authority" LF
 "<li> MX <i>&lt;preference&gt;</i> <i>&lt;full-name&gt;</i> - mail exchange. <i>&lt;preference&gt;</i> is numbre" LF
 "from 1 to 255.  Lower values are preferred." LF
 "<br> Other types are ignored.<br>" LF
 "For each type of record domain-name may begin from wildcard '*.' to descrpibe all" LF
 "sub domains. Server supports ANY and AXFR type of request to return all about domain. For" LF
 "domain with wildcard  reply also will content wildcard. For other types" LF
 "of request reply will be without wildcard." LF;
 char *lS3NSLST = S3NSLST;
#else
 extern char *lS3NSLST;
 extern char S3NSLST[];
#endif
#define NSLST lS3NSLST
#define x_NSLST S3NSLST


//#define s_NEWPWD "<h2>Change password:</h2><form name=y> <input type=hidden name=s value=3 >New password: <input type=password name=p size=32 maxlength=32> <input type=submit value='Change' > </form>  <br> <br> "

#ifdef MAINSTR
 char S3s_PWDChanged[] = "<hr>Password changed<hr>";
 char *lS3s_PWDChanged = S3s_PWDChanged;
#else
 extern char *lS3s_PWDChanged;
 extern char S3s_PWDChanged[];
#endif
#define s_PWDChanged lS3s_PWDChanged
#define x_s_PWDChanged S3s_PWDChanged


#endif
#endif
