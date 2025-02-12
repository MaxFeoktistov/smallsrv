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


#ifndef MINGW64
#define _WINDOWS_
#else
#define __CRT__NO_INLINE 1
#endif

#ifdef  DJGPP
typedef void* HKEY;
#else

#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wnarrowing"
#pragma GCC diagnostic ignored "-Wliteral-suffix"

#endif
#include <sys/types.h>
#include <ctype.h>

#ifdef MINGW
#include "mstring1.h"
#else
#include "mstring2.cpp"
#endif
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <shlobj.h>
#include <winreg.h>
#include "ddeml.h"
#ifdef SERVICE
#include <winsvc.h>
#endif
#include "mrc.cpp"
//#include "mdef.h"


#define sprintf wsprintf
//extern "C" int pak_exe(char *e,int l);

extern "C" char  fb_ind[10],fb_lis[10],fb_lisrus[10],fe_ind[10],fe_lis[10],fe_lisrus[10],fb_http[10],fe_http[10],fb_uttp[10],fe_uttp[10],*end,fb_lang[10],fe_lang[10],
fb_lange[10],fe_lange[10],
fb_ipbase[10],fe_ipbase[10],
fb_lnotes[10],fe_lnotes[10],
fb_s1[10],fe_s1[10],
fb_s2[10],fe_s2[10],
fb_s3[10],fe_s3[10],
fb_s4[10],fe_s4[10],
fb_vpnc[10], fe_vpnc[10],
fb_sert[10],
fe_sert[10]
;
char *easyfl[]=
{
 "uninst.exe",fb_uttp,fe_uttp,
 "http.exe",fb_http,fe_http,
 "desc.htm",fb_ind,fe_ind,
 "license.txt",fb_lis,fe_lis,
 "lang_notes.txt",fb_lnotes,fe_lnotes,
 "vpn_if_up.bat",fb_s1,fe_s1,
 "vpn_if_client_up.bat",fb_s2,fe_s2,
 "vpn_if_client_down.bat",fb_s3,fe_s3,
 "http.exe.manifest",fb_s4,fe_s4,

 "vpnclient.exe", fb_vpnc, fe_vpnc,
 "vpnclient.exe.manifest", fb_s4,fe_s4,
 "temp_sert.pem", fb_sert,fe_sert,
#ifdef RUS
  "shs_lang.cfg",fb_lang,fe_lang,
#endif
#ifdef IPCONTRY
  "ipbase.bin",fb_ipbase,fe_ipbase,
#endif
  "langpacks\\ru\\shs_lang.cfg",fb_lang,fe_lang,
  "langpacks\\en\\shs_lang.cfg",fb_lange,fe_lange,
 0,0,0
};

char *ddirs []={"langpacks\\en","langpacks\\ru","langpacks",0};
char  folder[]="\\shttps";
char  target[256]="c:\\shttps";
#ifndef RICON
uchar icn2[192]={
#include "ico2.h"
32,32,1,1
};
char *ticon;
#elif ! defined(MINGW)
#include "cres.cpp"
#endif
char *Ucode;
char  uninst[256];
HWND  mwnd;
#ifdef SERVICE
SC_HANDLE  sch;
#ifndef MINGW
HANDLE
#else
SC_HANDLE
#endif
  hs;

QUERY_SERVICE_CONFIG Service;
#endif
char apname[]=
#ifdef RUS
"Установка Small HTTP server 3.0";
#define DISPLAY_NAME  apname+10
#else
"Setup Small HTTP server 3.0";
#define DISPLAY_NAME  apname+6
#endif

mrc_obj  dlg1[]=
{
#ifdef RUS
{apname,0,"FMform", WS_OVERLAPPED|WS_CAPTION|WS_VISIBLE|WS_SYSMENU| WS_MINIMIZEBOX,2,16,220*2,248*2,WS_EX_STATICEDGE},
{"&Деинсталяция", 111, "BUTTON", WS_DISABLED|BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP,150,212,50,14},
{fb_lis,    120, "EDIT", WS_BORDER|WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_READONLY|WS_VSCROLL|WS_TABSTOP,4,4,208,96,WS_EX_STATICEDGE},
{"Я согласен с этой лицензией, во всех пунктах.", 144, "BUTTON", BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|WS_TABSTOP,8,102,160,10},
{"Каталог установки:", 146, "STATIC", SS_LEFT|WS_CHILD|WS_VISIBLE, 8, 112, 47, 8 },
{target,180, "EDIT",ES_LEFT|ES_AUTOHSCROLL|WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP,8,120,170,12,WS_EX_STATICEDGE},
{"...",145, "BUTTON", BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP,180,120,12,12,WS_EX_CLIENTEDGE|WS_EX_NOPARENTNOTIFY|WS_EX_STATICEDGE},
{"Создать пункт в меню \"Пуск\".", 147, "BUTTON", BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|WS_TABSTOP,8,136,90,10},
{"Автозагрузка",148,"BUTTON",BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|WS_TABSTOP,100,136,65,10},
#ifdef SERVICE
{"Служба NT",149,"BUTTON",WS_DISABLED|BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|WS_TABSTOP,170,136,50,10},
#endif

{"Административный вход:",102,"BUTTON",BS_GROUPBOX|WS_CHILD|WS_VISIBLE,2,147,212,64},

{"Эта версия включает удаленное администрирование. Для доступа, необходимо создать акаунт. "
 "После установки вы сможете добавлять, менять, или удалять любые акаунты.",
 103,"STATIC", SS_LEFT|WS_CHILD|WS_VISIBLE, 8, 158, 204,30 },

{"Имя:",146,"STATIC", SS_LEFT|WS_CHILD|WS_VISIBLE, 8, 188, 30, 8 },
{"admin",181,"EDIT",ES_LEFT|ES_AUTOHSCROLL|WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP, 42, 188, 150, 10, WS_EX_STATICEDGE },
{"Пароль:", 46, "STATIC", SS_LEFT|WS_CHILD|WS_VISIBLE, 8, 199, 30, 8 },
{"",182,"EDIT",ES_LEFT|ES_AUTOHSCROLL|WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP|ES_PASSWORD, 42, 199, 150, 10, WS_EX_STATICEDGE },

{"&Установить",110,"BUTTON",WS_DISABLED|BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 10, 212, 50, 14},
{"&Отмена",112,"BUTTON",BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 80, 212, 50, 14  },

#else
{apname,0,"FMform", WS_OVERLAPPED|WS_CAPTION|WS_VISIBLE|WS_SYSMENU| WS_MINIMIZEBOX,2, 16, 220*2, 248*2,WS_EX_STATICEDGE},
{"&Uninstall", 111, "BUTTON", WS_DISABLED|BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 150, 212, 50, 14},
{fb_lis,120,"EDIT", WS_BORDER|WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_READONLY|WS_VSCROLL| WS_TABSTOP,4,4,208,96, WS_EX_STATICEDGE },

{"I agree with this license, and I accept all items.", 144, "BUTTON", BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 8, 102,120,10},
{"Target:", 146, "STATIC", SS_LEFT|WS_CHILD|WS_VISIBLE, 8, 112, 47, 8 },
{target,180, "EDIT",   ES_LEFT|ES_AUTOHSCROLL|WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP, 8, 120, 170, 12, WS_EX_STATICEDGE },
{"...",145, "BUTTON", BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 180, 120, 12, 12, WS_EX_CLIENTEDGE|WS_EX_NOPARENTNOTIFY|WS_EX_STATICEDGE },
{"Update entry in main menu.", 147, "BUTTON", BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 8, 136,90,10},
{"Add to startup.", 148, "BUTTON", BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 100, 136,65,10},
#ifdef SERVICE
{"NT Service", 149, "BUTTON", WS_DISABLED|BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 170, 136,50,10},
#endif
{"Administrator account:",  102, "BUTTON", BS_GROUPBOX|WS_CHILD|WS_VISIBLE, 2,147,212, 64 },
{"This version has online administration. To get access, you have to create an account now. "
 "After setting you can add, change, or delete accounts",
 103, "STATIC", SS_LEFT|WS_CHILD|WS_VISIBLE, 8, 158, 204,30 },
{"User:",146, "STATIC", SS_LEFT|WS_CHILD|WS_VISIBLE, 8, 188, 30, 8 },
{"admin",181, "EDIT",ES_LEFT|ES_AUTOHSCROLL|WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP, 42, 188, 150, 10, WS_EX_STATICEDGE },
{"Password:", 46, "STATIC", SS_LEFT|WS_CHILD|WS_VISIBLE, 8, 199, 30, 8 },
{"",182,"EDIT",ES_LEFT|ES_AUTOHSCROLL|WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP|ES_PASSWORD, 42, 199, 150, 10, WS_EX_STATICEDGE },

{"&Install",110,"BUTTON",WS_DISABLED|BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 10, 212, 50, 14},
{"&Cancel",112,"BUTTON",BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP,80,212,50,14},
#endif
{0,0,0,0,0,0,0,0,0}

};
BROWSEINFO binf={0,0,target,"Select target",0,0,0,0};

char *  cmdInstall="[CreateGroup(\"Small HTTP server\")]\n"
"[ShowGroup(\"Small HTTP server\",1)]\n"
"[AddItem(\"%s\\http.exe\",\"Small HTTP server\")]\n"
"[AddItem(\"%s\\vpnclient.exe\",\"VPN client\")]\n"
#ifdef RUS
"[AddItem(\"%s\\desc.htm\",\"Описание\")]\n"
"[AddItem(\"%s\\license.txt\",\"Лицензия\")]"
#else
"[AddItem(\"%s\\desc.htm\", \"Description\")]\n"
"[AddItem(\"%s\\license.txt\",\"License\")]"
#endif
"\n[AddItem(\"%s\\lang_notes.txt\",\"Localisation notes\")]"
"\n[AddItem(\"%s\\uninst.exe\",\"Uninstall\")]"

;

HDDEDATA CALLBACK ProgDdeCallback(
UINT wType,UINT wFmt,HCONV hConv,HSZ hsz1,HSZ hsz2,HDDEDATA hData,
DWORD dwData1,DWORD dwData2){return(0);}
//HANDLE hinstance;
HINSTANCE hinstance;

HKEY startup=0;
char *old_cfg;
char *old_user;
char *reg_code;
int old_cfg_len=0;
FILETIME CreationTime;



#ifdef MINGW
extern "C" {
//int WINAPI WinMain( HINSTANCE hinst, HANDLE prev_inst, LPSTR cmline, int cmdshow );
void start()
{

  ExitProcess(WinMain(GetModuleHandle(NULL) ,// hinstanse,
                       0,
                      GetCommandLine() //cmdline
                      ,0));
}

};

#define TYPE_SC_HANDLE     (SC_HANDLE__*)
#else
#define TYPE_SC_HANDLE
#endif

void  ProgmanMSG(char *dta)
{ulong idInst=0;
 HCONV hConv;
 HSZ hszProgman;
 DWORD dwResult;

  if(DdeInitializeA(&idInst, ProgDdeCallback,APPCMD_CLIENTONLY|CBF_SKIP_ALLNOTIFICATIONS,0L) != DMLERR_NO_ERROR)return ;
  hszProgman=DdeCreateStringHandleA(idInst,"PROGMAN",0);
  if(!(hConv=DdeConnect(idInst,hszProgman,hszProgman,NULL))) return ;

  DdeClientTransaction((PBYTE)dta,strlen(dta)+1,hConv,0,0,XTYP_EXECUTE,TIMEOUT_ASYNC,&dwResult);
  Sleep(500);
  DdeDisconnect(hConv);
  DdeUninitialize(idInst);
}

inline int IsDChk(HWND hwnd,int d){return SendDlgItemMessage(hwnd,d,BM_GETCHECK,0,0);}

long CALLBACK  dlgFnc(HWND hwnd, UINT msg,UINT wparam, LONG lparam)
{HANDLE hn;
 union{
 WIN32_FIND_DATA fd;
 SERVICE_STATUS SerStat;
 };
 int i,l,x,xx;
 char *m,*t1;
 char **t;

 switch(msg)
 {
  case WM_INITDIALOG:
  case WM_CREATE: return 1;
  case WM_DESTROY: PostQuitMessage(0); break;
  case WM_COMMAND:
  switch(LOWORD(wparam))
  {
   case 144: // Agree
    EnableWindow(GetDlgItem(hwnd,110),IsDChk(hwnd,144));
    break;
   case 145: //...
    if( SHGetPathFromIDList( (_ITEMIDLIST *)SHBrowseForFolder(&binf),target))
    {if(!strstr(target,"shttps") )strcat(target,folder+(target[3]==0));
     SetDlgItemText(hwnd,180,target);
    }
    return 0;

   case 111: // Uninstal
    if((i=strlen(target))>3)
    {strcpy(target+i,"\\http.exe");
     if(hs){ControlService(hs,SERVICE_CONTROL_STOP,&SerStat); DeleteService(hs);} DeleteFile(target);
/*
     strcpy(target+i,"\\http.cfg"); DeleteFile(target);
     strcpy(target+i,"\\desc.htm"); DeleteFile(target);
     strcpy(target+i,"\\license.txt");DeleteFile(target);
*/
     for(t=easyfl;*t;t+=3)
     {
       sprintf(target+i,"\\%s",*t);
        DeleteFile(target);
     }

     for(t=ddirs;*t;t++)
     {
       sprintf(target+i,"\\%s",*t);
       RemoveDirectory(target);
     }
     target[i]=0;
     RemoveDirectory(target);
    }

    if( (m=strrchr(uninst,'\\')))
    {strcpy(m,"\\Small HTTP server.lnk"); DeleteFile(uninst);
#ifdef RUS
     strcpy(m,"\\Описание.lnk");  DeleteFile(uninst);
     strcpy(m,"\\Лицензия.lnk");  DeleteFile(uninst);
#else
     strcpy(m,"\\Description.lnk"); DeleteFile(uninst);
     strcpy(m,"\\License.lnk");     DeleteFile(uninst);
#endif
     strcpy(m,"\\lang_notes.lnk");     DeleteFile(uninst);

     *m=0;
     RemoveDirectory(uninst);
    }

    if(0){
   case 110: // Install
    if(hs)ControlService(hs,SERVICE_CONTROL_STOP,&SerStat);
    GetDlgItemText(hwnd,180,target,256);
    CreateDirectory(target,0);
    if( SetCurrentDirectory(target))
    {char **t;
     HCURSOR hc;
     CreateDirectory("langpacks",0);
     CreateDirectory("langpacks\\ru",0);
     CreateDirectory("langpacks\\en",0);

     for(m=fb_uttp;m<fe_uttp;++m)
      if( DWORD_PTR(*m)==0x4D4D4D4D && DWORD_PTR(m[4])==0x46464646)
      {sprintf(m+8,"%s\\",target);
       break;
      }
     for(t=easyfl;*t;t+=3)
     {
       while( (i=_lcreat(t[0],0))<0)
       if(MessageBox(0,
#ifndef RUS
       "Can't create file\nMay be preveus version now using?\nClose it and try again."
#else
       "Невозможно создать файл\nВероятно предыдущая версия сейчас используется?\nЗакройте и попробуйте снова. "
#endif
       ,t[0],MB_RETRYCANCEL|MB_ICONSTOP )==IDCANCEL) return 0;
       l=t[2]-t[1];
       if(stristr(t[0],".bat") || stristr(t[0],".pem") || stristr(t[0],".cfg") || stristr(t[0],".manifest") || stristr(t[0],".txt")  )
         while(0 == t[1][l-1] ) --l;

       _hwrite(i,t[1],l);
       _lclose(i);
     }
#ifdef SERVICE

#ifndef RICON
   if((xx=IsDChk(mwnd,149)&1) && sch)
     ticon[132]=1;
#else
 //  if ! defined(MINGW)
  if((xx=IsDChk(mwnd,149)&1) && sch) Ucode[4]=1;
#endif
#endif

     if(IsDChk(hwnd,147)&1)
     {HGLOBAL hgl=GlobalAlloc(GMEM_DDESHARE,1024);
      m=(char *)GlobalLock(hgl);
      sprintf(m,cmdInstall,target,target,target,target,target);
      ProgmanMSG(m);
      GlobalUnlock(hgl);
      GlobalFree(hgl);
     }

     if(startup)
     {if( (IsDChk(hwnd,148)&1) && !xx )
      {RegSetValueEx(startup,"http.exe",0,REG_SZ,(uchar *)fb_http,sprintf(fb_http,"%s\\http.exe",target));}
      else RegDeleteValue(startup,"http.exe");
      RegCloseKey(startup);
     }
     if((i=_lopen("http.cfg",2) )<=0)i=_lcreat("http.cfg",0);
     else _llseek(i,0,2);
     GetDlgItemText(hwnd,181,fb_http+8192,80);
     GetDlgItemText(hwnd,182,fb_http+4096,80);
     _hwrite(i,fb_http,sprintf(fb_http," \n user=%s;%s;%s;A\n norunhtm\n",fb_http+8192,fb_http+4096,target));


#ifdef SERVICE
if(sch)
{
 if(xx)
 {strcat(target,"\\http.exe service");
  if(!hs)
  {if(!(hs=CreateService(sch,folder+1,DISPLAY_NAME,SERVICE_ALL_ACCESS,
   SERVICE_WIN32_OWN_PROCESS /*_SHARE_PROCESS  */ |SERVICE_INTERACTIVE_PROCESS,
   SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,target,0,0,0,0,0)))
   {
lbsrverr:
    sprintf(uninst,
#ifdef RUS
    "Невозможно установиться как NT служба %X %X %d"
#else
     "Can't setup server as service %X %X %d"
#endif
    ,sch,hs,GetLastError());
    MessageBox(0,uninst,"Error",MB_OK);
   }
  }
  else if(Service.lpBinaryPathName && !strstr(target,Service.lpBinaryPathName))
  {
   if( !ChangeServiceConfig(hs,SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,
    SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,target,0,0,0,0,0,0)
   )goto lbsrverr;
//   goto lsecset;
  }
//  else goto lsecset;
 }
 else if(hs)DeleteService(hs);
}
#endif

    _lclose(i);
    }
   }
   MessageBox(hwnd,"Installation complete.",apname,MB_OK);
   case 112: DestroyWindow(hwnd); break;
  }

 };
 return DefWindowProc(hwnd,msg,wparam,lparam);
};

int InitApplication()
{WNDCLASS wc;
 char bfr[512],*t;
 uint i,j;
 int h;
 wc.style=CS_HREDRAW|CS_VREDRAW;
 wc.lpfnWndProc=dlgFnc;
 wc.cbClsExtra=0;
 wc.cbWndExtra=0;
 wc.hInstance=hinstance;
 i=GetVersion() & 0x80000000;

#ifndef RICON
 for(ticon=fb_http;ticon<fe_http;++ticon )
   if( DWORD_PTR(*ticon)==0xdd7d7777 //8x8x16
//0x77777777  //16x16x16
// 0x00FFFF7F  //16x16x2
  )
  {
#else
//      if !defined(MINGW)
 for(Ucode=fb_http;Ucode<fe_http;++Ucode)
   if( DWORD_PTR(*Ucode)==0x25FC2488) break;
#endif


  if(!i)
  {
#ifdef SERVICE
   if((sch=OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS)))
   {if((hs=OpenService(sch,folder+1,SERVICE_ALL_ACCESS)))
    {QueryServiceConfig(hs,&Service,sizeof(Service),(LPDWORD)&i);
     if(Service.lpBinaryPathName)strcpy(target,Service.lpBinaryPathName);
     if((t=strrchr(target,'\\')))*t=0;
     dlg1[1].Style &= ~WS_DISABLED;
    }
    dlg1[9].Style&=~WS_DISABLED;
   }
   else MessageBox(0,"Can't open SCM","Error",MB_OK);
#endif
#ifndef RICON
   memcpy(ticon,icn2,132);
  }
  break;
#endif
 }

 wc.hIcon=
#ifndef RICON
 CreateIcon(hinstance,
//16,16,4,1,
//32,32,4,1,
  32,32,1,1,(uchar *)uninst,(uchar *)icn2);
#else
 LoadIcon(hinstance,MAKEINTRESOURCE(502));
#endif
 wc.hCursor = LoadCursor( NULL, IDC_ARROW );
 wc.hbrBackground =(HBRUSH) (HBRUSH) (COLOR_BTNFACE+1);
  //GetStockObject(LTGRAY_BRUSH);  //( COLOR_WINDOW );
 wc.lpszMenuName =NULL;
 wc.lpszClassName ="FMform";

 if( RegisterClass(&wc) )
 {_ITEMIDLIST *x;
  i=CSIDL_PROGRAMS;
 lbNT:
  if((!SHGetSpecialFolderLocation(0,i,&x)) && (SHGetPathFromIDList(x,bfr),
     sprintf(uninst,"%s\\Small HTTP server\\Small HTTP server.lnk",bfr),
     ( (h=_lopen(uninst,0) )>0 )
    )
   )
  {GetFileTime( (HANDLE) h,&CreationTime,0,0);
   i=_hread(h,bfr,512);
   _lclose(h);
   for(j=0;j<i;++j)
   {if((bfr[j]==':') && (bfr[j+1]=='\\') && (t=strstr(bfr+j,"\\http.exe")) )
    {if( (i=(t-bfr)-j+1 )>255)i=255;
     strncpy(target,bfr+j-1,i);
     dlg1[1].Style &= ~WS_DISABLED;
     break;
    }
   }
  }
  else if(i!=CSIDL_COMMON_PROGRAMS){i=CSIDL_COMMON_PROGRAMS; goto lbNT;}

  RegOpenKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",&startup);
  if(dlg1[1].Style & WS_DISABLED && startup)
  {if(RegQueryValue(startup,"http.exe",target,(PLONG)&h)==ERROR_SUCCESS)
   {if(t=strrchr(target,'\\') )*t=0;
    dlg1[1].Style &= ~WS_DISABLED;
   }
  }
  ShowWindow(mwnd=MkDlg(dlg1,0,(long)GetStockObject(17)),SW_SHOWNORMAL);
  return 1;
 }
 return 0;
};

#ifdef MINGW
int WINAPI WinMain(HINSTANCE__*hinst, HINSTANCE__*prev_inst, CHAR*cmdline, int cmdshow)
#else
#define TYPE_SC_HANDLE
int WINAPI WinMain( HANDLE hinst, HANDLE prev_inst, LPSTR cmdline, int cmdshow )
#endif
{MSG msg;
 hinstance=hinst;
 if(InitApplication())
 {while( GetMessage(&msg,0,0,0))
  {if(!IsDialogMessage(mwnd,&msg))
   {TranslateMessage(&msg);
    DispatchMessage(&msg);
   };
  };
 };
 if(hs)CloseServiceHandle(hs);
 ExitProcess(0);
 return 0;
};

