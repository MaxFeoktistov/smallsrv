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


char *selc[2] = {NullString, "selected"};

int OutListDays(BFILE *bfl, int d, int n)
{ int i;
  bfl->bprintf("<select name=%c>", n);
  for(i = 1; i <= 31; ++i)bfl->bprintf("<option %s>%u", selc[i == d], i);
  return bfl->bprintf("</select> ");
};
int OutListMonth(BFILE *bfl, int d, int n)
{ int i;
  bfl->bprintf( "<select name=%c>", n);
  for(i = 1; i <= 12; ++i)bfl->bprintf( "<option value=%u %s>%s", i, selc[i == d], month[i - 1]);
  return bfl->bprintf("</select> ");
};
char *GetVarS(char **varlist, char *var)
{
  return (var = GetVar(varlist, var) ) ? var : NullString;
};

inline ulong GetVarI(char **varlist, char *var)
{return atoui(GetVarS(varlist, var) );}

StatLog *ParseFile(char *log, char *b) //=0)
{
  StatLog *tsl;
  char *t1, *t, *p;
  int n = 0;
  if( (p = strstr(log, "\r\n!->") ) )
  { for(t1 = t = p; t;)
    { ++n;
#ifdef A_64
//   DDWORD_PTR(*t1)=(u_long) (t=strstr((t1=t)+1,"\r\n!->"));
      t = strstr((t1 = t) + 1, "\r\n!->");
//   DWORD_PTR(*t1)=t? (ulong) (t-log):0;
      DWORD_PTR(*t1) = t ? (ulong) (t - t1) : 0;
#else
      DWORD_PTR(*t1) = (ulong) (t = strstr((t1 = t) + 1, "\r\n!->"));
#endif
    }
    if(b)sprintf(b, "%14.14s -- %14.14s ; %u", p + 5, t1 + 5, n);
    for(tsl = (StatLog *)p; tsl;
#ifdef A_64
        tsl = //tsl->next=
          tsl->nexto(log)
#else
        tsl = tsl->next
#endif
       )tsl->Parse();
    return (StatLog *)p;
  }
  return 0;
};


const char *afHstStr[] =
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


const tfFind afHstFnd[] =
{&fFindHTTPInU, &fFindHTTPOutU, 0};

const tfFind afUsrFnd[] =
{
  &fFindProxyInU,
  &fFindProxyInUNC,
  &fFindProxyOutU,
  &fFindFTPLogin,
  &fFindFTPdownloadU,
  &fFindFTPuploadU,
  &fFindPOPLogin,
  &fFindPOPx,
  &fFindSMTPUser,
  &fFindSMTPs,
  &fFindSMTPr,
  0
};

const char* afUsrStr[] =
{
#ifdef RUS
  "Proxy. Запрошенный входящий трафик (в байтах)"
  , "Proxy. Запрошенный трафик без учета возвращенного из кеша (в байтах)"
  , "Proxy. Исходящий трафик (в байтах)"
  , "FTP Login"
  , "FTP скачено (в байтах)"
  , "FTP залито (в байтах)"
  , "POP Login"
  , "SMTP вызовы"
  , "SMTP трафик сообщений от пользователей (в байтах)"
  , "SMTP трафик сообщений для пользователей (в байтах)"
  ,
#else
  sPROXY__RE
  , sPROXY__IN
  , sPROXY__OU
  , sFTP_LOGIN
  , sFTP_DOWNL1
  , sFTP_UPLOA1
  , sPOP_LOGIN
  , sSMTP_CALL
  , sSMTP_DATA0
  , sSMTP_DATA1
  ,
#endif
  0
};

const tfFind afIPFnd[] =
{
  &fFindEventSumary,
  &fFindHTTPUser,
  &fFindHTTPIn,
  &fFindHTTPOut,
  &fFindProxyUser,
  &fFindProxyIn,
  &fFindProxyInNC,
  &fFindProxyOut,
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
  0
};

const char *afIPStr[] =
{
#ifdef RUS
  "Сумарно по портам"
  , "HTTP вызовы"
  , "HTTP. Входящий трафик (в байтах)"
  , "HTTP. Исходящий трафик (в байтах)"
  , "Proxy вызовы"
  , "Proxy. Запрошенный входящий трафик (в байтах)"
  , "Proxy. Запрошенный трафик без учета возвращенного из кеша (в байтах)"
  , "Proxy. Исходящий трафик (в байтах)"
  , "FTP Login"
  , "FTP. Утянули"
  , "FTP. Залили"
  , "POP Login"
  , "POP трафик сообщений (в байтах)"
  , "SMTP вызовы"
  , "SMTP трафик сообщений (в байтах)"
  , "SMTP трафик сообщений от пользователей (в байтах)"
  , "SMTP трафик сообщений для пользователей (в байтах)"
  , "DNS вызовы"
#else
  sSUMMARY_B
  , sHTTP_CALL
  , sHTTP__INC
  , sHTTP__OUT
  , sPROXY_CAL
  , sPROXY__RE
  , sPROXY__IN
  , sPROXY__OU
  , sFTP_LOGIN
  , sFTP_DOWNL
  , sFTP_UPLOA
  , sPOP_LOGIN
  , sPOP_DATA_
  , sSMTP_CALL
  , sSMTP_DATA
  , sSMTP_DATA0
  , sSMTP_DATA1
  , sDNS_CALLS
#endif
  , 0

};

uint fFindJustRef(StatLog *psl, char *bfr)
{return IfFindSubStr(psl, bfr, "\r\nReferer: ");};

const char *afFileStr[] =
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

const tfFind afFileFnd[] =
{&fFindURLHTTP, &fFindURLProxy, &fFindJustRef, &fFindFTPdownload, &fFindFTPupload, 0};


const tfFind *StatFppFnc[4] =
{afIPFnd, afUsrFnd, afHstFnd, afFileFnd};


const char **StatFppStr[] =
{afIPStr, afUsrStr, afHstStr, afFileStr};

void LL2Lc(u64 *pars, int n)
{
  while(n-- > 0)
  {
    if(*pars > 0x40000000)
    {
      *pars >>= 10;
      if(*pars > 0x40000000)
      {
        *pars >>= 10;
        ((int *)pars)[1] = 'M';
      }
      else
        ((int *)pars)[1] = 'K';
    }
    else ((int *)pars)[1] = ' ';
    ++pars;
  }
};

void Req::HTTPUserStat(BFILE *bfl)
{
  char *nm, *bfr;
  char *t;
  const char **strs;
  int typ, day2, mnt2, l, k, i, h;
  StatLog *tsl;
  uchar funcType[32];
  union {
    u64   pars[88];
    u32   upars[88];
    char  *cpars[172];
  };
  ulong ip;
#define mnt1 (*(long*)pars)
#define day1 (*(long*)(pars+1))
  const tfFind *fpFnc;
  bfr = loc + 0x8000;
  memset(pars, 0, sizeof(pars));
  nm = GetVarS(req_var, "h");
  typ = GetVarI(req_var, "t") & 3;
  day1 = GetVarI(req_var, "f");
  day2 = GetVarI(req_var, "c");
  mnt1 = GetVarI(req_var, "d");
  mnt2 = GetVarI(req_var, "g");

  bfl->bprintf( "<form action=/$_admin_$susr method=get>"
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
                , selc[typ == 0]
                , selc[typ == 1]
                , selc[typ == 2]
                , selc[typ == 3]
                , nm);

  OutListDays(bfl, day1, 'f');
  OutListMonth(bfl, mnt1, 'd');
  bfl->bprintf( " To:");
  OutListDays(bfl, day2, 'c');
  OutListMonth(bfl, mnt2, 'g');
  bfl->bprintf( "<br><br><input type=submit value=Show> <input type=reset></form><hr>");
//*
  if( ! (s_flg & FL_LOGDAY) )
  {
#ifdef RUS
#undef MSG_LD
#define MSG_LD "<h2>Это работает только если опция <i>Новый log файл каждый день</i> выбрана</h2>"
#endif
    bfl->bprintf(MSG_LD);

  }
//*/
  if(*nm)
  {
    ip = ConvertIP(t = nm);
    fpFnc = StatFppFnc[typ];
    strs = StatFppStr[typ];

    bfl->bprintf("<table bgcolor=#f8f8ff border=1><tr><td> &nbsp; Date  &nbsp; </td>");
    k = 0xC0C0E0;
    for(i = 0; strs[i]; ++i)
    {
      bfl->bprintf( "<td bgcolor=#%x><font size=1 class=f3>%s</font></td>", k, strs[i]);
      k = (((k ^ (0x301020 >> ((i % 3) * 8))) + 0x010101) | 0xC3C3C3) & 0xFFFFFF;
    };
    bfl->bprintf( "</tr><tr align=center>");

    for(i = 0; fpFnc[i]; ++i) funcType[i] = GetFuncType(fpFnc[i]);
    do
    {
      l = sprintf(bfr, "%s", flog);
      if( !(t = strrchr(bfr, '.')) )t = bfr + l;
      sprintf(t, "%2.2u%2.2u", mnt1, day1);

      if((h = _lopen(bfr, 0)) > 0)
      { l = FileSize(h);
        if((t = new char[l + 20]))
        { l = _hread(h, t, l);
          t[l] = 0;
          memset(pars + 2, 0, 40 * 8);
          tsl = ParseFile(t);
          for(; tsl; tsl = tsl->Next())
          { for(i = 0; fpFnc[i]; ++i)
            {

              if((k = (fpFnc[i])(tsl, bfr)))
              {
                if( (!funcType[i]) )k = 1;
              };

              if( ( (!typ) ? (ip == tsl->ip) : (typ == 3) ?
#ifdef A_64
                    NULL !=
#else
                    (int)
#endif
                    stristr(bfr, nm) : (!stricmp(nm, bfr)) ) )
              { pars[i + 2] += k; pars[i + 42] += k;
              }
            }
          }
          LL2Lc(pars + 2, 40);
          k = 0xC0C0E0;
          bfl->bprintf("<td> %2.2u/%2.2u </td>", upars[0], upars[2]);
          for(i = 0; strs[i]; ++i)
          {
            bfl->bprintf("<td bgcolor=#%x>%u%c</td>", k & 0xF0F8FF, upars[i * 2 + 4], upars[i * 2 + 5]);
            k = (((k ^ (0x301020 >> ((i % 3) * 8))) + 0x010101) | 0xC3C3C3) & 0xFFFFFF;
          };
          bfl->bprintf( "</tr><tr align=center>");
          delete t;
        }
        _lclose(h);
      }
      if(++day1 >= 32)
      { day1 = 1;
        if(mnt1 == mnt2)break;
        if(++mnt1 > 12)mnt1 = 1;
      }
      if(fl & F_CON_CLOSED)
        return;
    } while(mnt1 != mnt2 || day1 <= day2);
    LL2Lc(pars + 42, 40);
    k = 0xC0C0E0;
    bfl->bprintf("<td>Total:</td>");
    for(i = 0; strs[i]; ++i)
    {
      bfl->bprintf("<td bgcolor=#%x>%u%c</td>", k, upars[i * 2 + 84], upars[i * 2 + 85]);
      k = (((k ^ (0x301020 >> ((i % 3) * 8))) + 0x010101) | 0xC3C3C3) & 0xFFFFFF;
    };
    bfl->bprintf("</tr></table><hr>");


#undef mnt1
#undef day2
  }
  bfl->fflush();

}

