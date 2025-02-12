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



mrc_mnu  mnu2[]=
{
 {0,125,"&Open Window" },
#ifndef CD_VER
 {0,126,"&Settings..." },
// {0,346,"&Reconnect" },
 {MF_SEPARATOR,1,0},
 {0,98,"&VPN Client connect" },
 {0,99,"VPN Client &disconnect" },

 {MF_SEPARATOR,1,0},
 {0,352,"Description (&Local)" },
 {0,350,"What &new?  (Online)" },
 {MF_SEPARATOR,1,0},
#endif

 {0,344,"&About" },
 {MF_SEPARATOR,1,0},
 {0,150,"E&xit" },
 {0,0,0}
};

mrc_mnu  mnu3[]=
{
 {0,3900,"&General" },
 {0,3901,"&TLS/SSL"},
 {0,3902,"VPN C&lient"},
 {MF_HELP,710,"O&k" },
 {MF_HELP,150,"&Cancel" },
 {0,0,0}
};



#ifndef CD_VER
const char *url_desc[]=
{
 "http://smallsrv.com/index.htm#new",// "What &new?  (Online)" },
 "http://smallsrv.com/news.htm",// "Description (&Online)" },
// "http://smallsrv.com/desc.htm",// "Description (&Online)" },
 "desc.htm",              //
 "http://smallsrv.com/donation.shtml", // "&Order regestration (Online)"
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
