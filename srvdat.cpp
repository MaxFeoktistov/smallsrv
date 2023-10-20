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


#ifdef RUS

mrc_mnu  mnu2[]=
{
 {0,125,"&Открыть окно" },
#ifndef CD_VER
 {0,126,"&Настройки..." },
 {0,354,"На&стройки/текущие состояние (браузер)" },
 {0,355,"&Пользователи и Виртуальные хосты (браузер)" },
 {0,356,"&Текущая статистика (браузер)" },
 {MF_SEPARATOR,1,0},
 {0,352,"Описание" },
// {0,351,"Description (&Online)" },
 {0,350,"Что &нового?  (Online)" },
 {MF_SEPARATOR,1,0},
 {0,346,"Перезапуск" },
 {MF_GRAYED,345,"Отправить неотправленную почту" },
 {MF_GRAYED,347,"Перезагрузить файл определения хостов" },
#endif
 {0,344,"&О..." },
 {MF_SEPARATOR,1,0},
 {0,150,"Вы&ход" },
#if (! defined(CD_VER) ) && (! defined(FREEVER) )
 {MF_SEPARATOR,135,0},
 {0,353,"&Заказ регистрации (Online)"    },
 {0,134 ,"&Регистрация"    },
#endif
 {0,0,0}
};

//const
mrc_mnu  mnu3[]=
{
 {0,3900,"О&бщие" },
 {0,3901,"&HTTP" },
#if V_FULL
 {0,3902,"&DNS" },
// {0,3903,"Dynamic &IP" },
 {0,3903,"&Proxy" },
 {0,3904,"&FTP" },
 {0,3905,"&POP3" },
 {0,3906,"&SMTP" },
 {0,3907,"DH&СP" },
 {0,3908,"&TLS/SSL"},
 {MF_POPUP,5,"&Другое" },
 {0,3909,"&CGI интерпретаторы" },
 {0,3910,"&MIME типы" },
 {0,3911,"&Пользователи" },
 {0,3912,"&Виртуальные каталоги и хосты" },
#define MNU_CGI   10
#define MNU_VHOST 13
#else
#define MNU_CGI   3
#define MNU_VHOST 4
 {0,3902,"&CGI Interpreters" },
 {0,3903,"&MIME types" },
 {0,3904,"&Virtual hosts and directories" },
#endif

 {MF_HELP,710,"O&k" },
 {MF_HELP,150,"&Отмена" },
 {0,0,0}
};

#else
    //no RUS

mrc_mnu  mnu2[]=
{
 {0,125,"&Open Window" },
#ifndef CD_VER
 {0,126,"&Settings..." },

 {0,354,"Se&tting/Current state (browser)" },
 {0,355,"&Users && Virtual Hosts (browser)" },
 {0,356,"&Current statistics (browser)" },
 {MF_SEPARATOR,1,0},
 {0,352,"Description (&Local)" },
// {0,351,"Description (&Online)" },
 {0,350,"What &new?  (Online)" },
 {MF_SEPARATOR,1,0},
 {0,346,"Restart" },
 {MF_GRAYED,345,"Send outgoing mail now" },
 {MF_GRAYED,347,"Reload host definition file" },
#endif
#ifdef SEPLOG
   {MF_POPUP,11,"&Log to view"},
   {0,360,"&General/HTTP" },
   {0,361,"&Proxy" },
   {0,362,"&FTP" },
   {0,363,"&SMTP" },
   {0,364,"P&OP" },
   {0,365,"SS&L" },
   {0,366,"HTTP.&err" },
   {0,367,"&DNS" },
   {0,369,"DH&CP" },
   {0,360+VPN_LOG,"&VPN" },
#endif

 {0,344,"&About" },
 {MF_SEPARATOR,1,0},
 {0,150,"E&xit" },

#if (! defined(CD_VER) ) && (! defined(FREEVER) )
 {MF_SEPARATOR,135,0},
 {0,353,"&Order regestration (Online)"    },
 {0,134 ,"&Registration"    },
#endif
 {0,0,0}
};
/*
mrc_mnu  mnu4[]=
{
   {MF_POPUP,0,"&Server"},
#ifdef SEPLOG
   {MF_POPUP,10,"Log to view"},
   {0,360,"General/HTTP" },
   {0,361,"Proxy" },
   {0,362,"FTP" },
   {0,363,"SMTP" },
   {0,364,"POP" },
   {0,365,"SSL" },
   {0,366,"HTTP.err" },
   {0,367,"DNS" },
   {0,369,"DHCP" },
#endif
   {0,0,0}

};
*/
//const
mrc_mnu  mnu3[]=
{
 {0,3900,"&General" },
 {0,3901,"&HTTP" },
#if V_FULL
 {0,3902,"&DNS" },
// {0,3903,"Dynamic &IP" },
 {0,3903,"&Proxy" },
 {0,3904,"&FTP" },
 {0,3905,"&POP3" },
 {0,3906,"&SMTP" },
 {0,3907,"DH&CP" },
 {0,3908,"&TLS/SSL"},
 {0,3909,"VP&N Server"},
 {0,3910,"VPN C&lient"},
 {MF_POPUP,5,"&Other" },
 {0,3911,"&CGI Interpreters" },
 {0,3912,"&Mime types" },
 {0,3913,"&Users" },
 {0,3914,"&Virtual hosts and directories" },
#define MNU_CGI   12
#define MNU_VHOST 15
#else
#define MNU_CGI   2
#define MNU_VHOST 4
 {0,3902,"&CGI Interpreters" },
 {0,3903,"&Mime types" },
 {0,3904,"&Virtual hosts and directories" },
#endif

 {MF_HELP,710,"O&k" },
 {MF_HELP,150,"&Cancel" },
 {0,0,0}
};
#endif



#ifndef CD_VER
const char *url_desc[]=
{
 "http://smallsrv.com/index.htm#new",// "What &new?  (Online)" },


#ifdef RUS
 "http://smallsrv.com/indexr.htm#new",// "Description (&Online)" },
 "desc.htm",              //
 "http://smallsrv.com/donation.shtml?rus=1", // "&Order regestration (Online)"
#else
 "http://smallsrv.com/news.htm",// "Description (&Online)" },
// "http://smallsrv.com/desc.htm",// "Description (&Online)" },
 "desc.htm",              //
 "http://smallsrv.com/donation.shtml", // "&Order regestration (Online)"
#endif

 "http://127.0.0.1/$_admin_$conf",// "&Setting / Current state"
 "http://127.0.0.1/$_admin_$host", //
 "http://127.0.0.1/$_admin_$stat",//
};
#endif
char fnamebuf[512];
char hstflt[]="*.*\x00*.*\x00";
OPENFILENAME ofn={
 sizeof(OPENFILENAME) ,//     DWORD         lStructSize;
 0,//     HWND          hwndOwner;
 0,//     HINSTANCE     hInstance;
 hstflt, //      lpstrFilter;
 0,//     LPTSTR        lpstrCustomFilter;
 0,//     DWORD         nMaxCustFilter;
 0,//     DWORD         nFilterIndex;
 fnamebuf,//     LPTSTR        lpstrFile;
 256,//     DWORD         nMaxFile;
 0, //     LPTSTR        lpstrFileTitle;
 0, //     DWORD         nMaxFileTitle;
 0, //     LPCTSTR       lpstrInitialDir;
 0,//     LPCTSTR       lpstrTitle;
 0, //OFN_PATHMUSTEXIST ,//     DWORD         Flags;
 0, //     WORD          nFileOffset;
 0, //     WORD          nFileExtension;
 0, //     LPCTSTR       lpstrDefExt;
 0, //     DWORD         lCustData;
 0, //     LPOFNHOOKPROC lpfnHook;
 0  //     LPCTSTR       lpTemplateName
};

BROWSEINFO binf={ 0,0,fnamebuf,"Select",0,0,0,0};


HMENU hmnu,hmmnu;
#if ! defined(FREEVER)
HWND dwnd2=0;
#endif
HWND  dwndc;
