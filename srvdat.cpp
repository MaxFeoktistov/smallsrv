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


#ifdef RUS

#if (! defined(CD_VER) ) && (! defined(FREEVER) )

mrc_obj  dlg2[]=
{
 {"�����������",0, "FMFROM", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_MINIMIZEBOX,2, 16, 180*2, 80*2,WS_EX_STATICEDGE},
 {"",          580, "EDIT", ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 30, 10, 132, 12, WS_EX_STATICEDGE },
 {"",          584, "EDIT", ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 30, 24, 132, 12, WS_EX_STATICEDGE },
 {"���",           148, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE,     2, 10, 20, 8                                                                       },
 {"���",           149, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE,     2, 24, 20, 8                                                                       },
 {"O&k",       512, "BUTTON", BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 8,   40, 40, 14  },
 {"&�����",    353, "BUTTON", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP,    70,  40, 40, 14    },
 {"&������",   150, "BUTTON", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP,    120, 40, 40, 14    },
 {0,0,0,0,0,0,0,0,0}
};
#endif
mrc_mnu  mnu2[]=
{
 {0,125,"&������� ����" },
#ifndef CD_VER 
 {0,126,"&���������..." },
 {0,354,"��&�������/������� ��������� (�������)" },
 {0,355,"&������������ � ����������� ����� (�������)" },
 {0,356,"&������� ���������� (�������)" },
 {MF_SEPARATOR,1,0},
 {0,352,"��������" },
// {0,351,"Description (&Online)" },
 {0,350,"��� &������?  (Online)" },
 {MF_SEPARATOR,1,0},
 {0,346,"����������" },
 {MF_GRAYED,345,"��������� �������������� �����" },
 {MF_GRAYED,347,"������������� ���� ����������� ������" },
#endif
 {0,344,"&�..." },
 {MF_SEPARATOR,1,0},
 {0,150,"��&���" },
#if (! defined(CD_VER) ) && (! defined(FREEVER) )
 {MF_SEPARATOR,135,0},
 {0,353,"&����� ����������� (Online)"    },
 {0,134 ,"&�����������"    },
#endif
 {0,0,0}
};

//const 
mrc_mnu  mnu3[]=
{
 {0,3900,"�&����" },
 {0,3901,"&HTTP" },
#if V_FULL
 {0,3902,"&DNS" },
// {0,3903,"Dynamic &IP" },
 {0,3903,"&Proxy" },
 {0,3904,"&FTP" },
 {0,3905,"&POP3" },
 {0,3906,"&SMTP" },
 {0,3907,"DH&�P" },
 {0,3908,"&TLS/SSL"},
 {MF_POPUP,5,"&������" },
 {0,3909,"&CGI ��������������" },
 {0,3910,"&MIME ����" },
 {0,3911,"&������������" },
 {0,3912,"&����������� �������� � �����" },
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
 {MF_HELP,150,"&������" },
 {0,0,0}
};

#else
    //no RUS

#if (! defined(CD_VER) ) && (! defined(FREEVER) )

mrc_obj  dlg2[]=
{
 {"Registration",0, "FMFROM", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU| WS_MINIMIZEBOX,2, 16, 180*2, 80*2,WS_EX_STATICEDGE},
 {"",          580, "EDIT", ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 30, 10, 132, 12, WS_EX_STATICEDGE },
 {"",          584, "EDIT", ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 30, 24, 132, 12, WS_EX_STATICEDGE },
 {"Name",           148, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE,     2, 10, 20, 8                                                                       },
 {"Code",           149, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE,     2, 24, 20, 8                                                                       },
 {"O&k",       512, "BUTTON", BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 8,   40, 40, 14  },
 {"&Order",    353, "BUTTON", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP,    70,  40, 40, 14    },
 {"&Cancel",   150, "BUTTON", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP,    120, 40, 40, 14    },
 {0,0,0,0,0,0,0,0,0}
};
#endif
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
 {MF_POPUP,5,"&Other" },
 {0,3909,"&CGI Interpreters" },
 {0,3910,"&Mime types" },
 {0,3911,"&Users" },
 {0,3912,"&Virtual hosts and directories" },
#define MNU_CGI   10
#define MNU_VHOST 13
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


HMENU hmnu;
HWND dwnd2=0;
HWND  dwndc;

#if (! defined(CD_VER) ) && (! defined(FREEVER) )
#include "depend.cpp"
#endif

