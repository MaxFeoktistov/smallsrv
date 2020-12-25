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


char  tbl1[]="áâ÷çäåöúéêëìíîïğòóôõæèãşûıÿùøüàñÁÂ×ÇÄÅÖÚÉÊËÌÍÎÏĞÒÓÔÕÆÈÃŞÛİßÙØÜÀÑ£³\000"
"ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõö÷øùúûüışÿ¸¨";

void DecodeKoi(char *b)
{register char *t;
 while(*b){if( t=strchr(tbl1,*b))*b=t[67]; b++;};
};


#ifndef FREEVER
//CD_VER


const char REG_HTML[]=
  "<h2>Registration:</h2>"
  "<form method=GET action=\"/$_admin_$regi\">"
  "<table bgcolor=#cffce0 border=1>"
  "<tr bgcolor=#a0eef8><td align=center><font size=3 class=f3><b>"
  "Name:</b></font>"
  "</td><td align=left><font size=2 class=f2>"
  "<input type=text name=n maxlength=256 size=32></b></font>"
  "</td></tr><tr>"
  "<td align=left><font size=2 class=f2>"
  " Code:</font>"
  "</td><td align=left><font size=2 class=f2>"
  "<input type=text name=h maxlength=127 size=30>"
  "<input type=submit value=Submit>"
  "</form></font>"
  "</td></tr></table>"HTML_LN
  "<b><a href=\"http://smallsrv.com/reg2.cgi\">Order</a></b><br><hr>"
   ;

void Req::HTTPOutRegPage()
{ //HTMLOutTop(s);
  Send(REG_HTML,sizeof(REG_HTML)-1);
  //HTMLOutBottom(s);
}

int Req::Registrate()
{int i,j,k,l;
 char *bfr,*n,*p;
 if(! (bfr=GetVar(req_var,"h") )) return 0;
 if(! (n=GetVar(req_var,"n") )) return 0;

       chk_code3a=0;
       chk_code3b=0;
       j=0;
       if( (bfr[1]<'a') && ((i=atoui(bfr))>=27) && (i<=33)  )
       {i=16128473;
        j=33846;
       }
       else
       {bfr[1]-='a'-'0';
        bfr[3]-='f'-'0';
        i=atoui(bfr);
        bfr[1]+='a'-'0';
        bfr[3]+='f'-'0';
       }
       if( (p=strchr(bfr,'-') ))  j=atouis(++p);
       if(CheckCode((uchar *) n,i,j) )
       {DecodeKoi(n);
        if(CheckCode((uchar *) n,i,j) )
         goto lbErrCode;
       }

       chk_code3=WORD_PTR(bfr[5])^0x5A3C;
//debug("*--------%X %s",user_name,user_name?user_name:"*");
  k=MkName(n);
  l=Rnd();
  chk_code3a=rol(k^l,*n)^chk_code5;
  chk_code3b=ror(k+~l,n[1])^i;
//debug("!--------%X %s",user_name,user_name?user_name:"*");
        if( CheckReg((uchar *) n,i,j,p) )
        {
      lbErrCode:
         chk_code3a=0;
         chk_code3b=0;
         //Sleep(1500);
         //MessageBox(hwnd, sBAD_CODE ,"Error",MB_OK);
         //delete bfr;
         //break;
         HTTPAdminErrorMessage("Bad code","");
         return 0;

        }

        if(pprot) debug( sTHANK_YOU ,about);
        //SetWindowText(hwnd, sTHANK_YOU0 );
//debug("--------%X %s",user_name,user_name?user_name:"*");
#define TYFUS   "<h1>Thank you for your support!</h1>"
//   send(s,TYFUS,sizeof(TYFUS)-1,0);
//   HTTPAdminErrorMessage(s,TYFUS,"");
   SaveCfgFile(loc+8192+260);
   return 1;



}

#include "depend.cpp"

#endif
