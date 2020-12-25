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


#ifdef DJGPP
typedef  void * HKEY;
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

#include  <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <winreg.h>

#include "mdef.h"
//#include "mstring.h"

#define sprintf wsprintf
#define APPNAME "Small HTTP server"
#define APPMENU "Small HTTP server"

char *files[]=
{
"desc.htm",
"license.txt",
"http.cfg",
"http.exe",
 0
};

struct DTT
{
 ulong sifn[2];
 char  dir[512];
}dtt=
{0x4D4D4D4D,0x46464646};

#ifdef SERVICE
#include <winsvc.h>
SC_HANDLE  sch;
HANDLE hs;
QUERY_SERVICE_CONFIG Service;
#endif
HKEY startup=0;


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



#endif

extern "C" { 

#ifdef MINGW
#define TYPE_SC_HANDLE     (SC_HANDLE__*)
int WINAPI WinMain(HINSTANCE__*hinst, HINSTANCE__*prev_inst, CHAR*cmdline, int cmdshow)
#else
#define TYPE_SC_HANDLE  
int WINAPI WinMain( HANDLE hinst, HANDLE prev_inst, LPSTR cmdline, int cmdshow )
#endif
{
 char uninst[1024];
 char bfr[1024];
 int i,nt;
 _ITEMIDLIST *x;
 HANDLE h;
 union{
  WIN32_FIND_DATA fd;
  SERVICE_STATUS SerStat;
 };


  if(MessageBox(0,"Do you want to remove " APPNAME "?","Are you sure?", MB_YESNO )==IDYES )
  {

#ifdef SERVICE
   if(!(nt=GetVersion() & 0x80000000))
    if((sch=OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS)))
    {if((hs=OpenService(sch,"shttps",SERVICE_ALL_ACCESS)))
     { ControlService(TYPE_SC_HANDLE hs,SERVICE_CONTROL_STOP,&SerStat);
      DeleteService(TYPE_SC_HANDLE hs);
     }
    }
    else MessageBox(0,"Can't open SCM","Error",MB_OK);
#endif

   RegOpenKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",&startup);
   if(startup)
   {RegDeleteValue(startup,"http.exe");
    RegCloseKey(startup);
   }

   for(i=0;files[i];++i)
   {sprintf(bfr,"%s%s",dtt.dir,files[i]);
    DeleteFile(bfr);
   }
   i=CSIDL_PROGRAMS;
  lbNT:
   if((!SHGetSpecialFolderLocation(0,i,&x)) && (SHGetPathFromIDList(x,bfr),
      sprintf(uninst,"%s\\" APPMENU "\\*.*",bfr),
      ( (h=FindFirstFile(uninst,&fd) ) != INVALID_HANDLE_VALUE)
     )
    )
   {
//    MessageBox(0,uninst,bfr,MB_OK);
    do{
     if(fd.cFileName[0]!='.')
     {sprintf(uninst,"%s\\" APPMENU "\\%s",bfr,fd.cFileName);
//      MessageBox(0,uninst,bfr,MB_OK);
      DeleteFile(uninst);
     }
    }while( FindNextFile(h,&fd) );
    FindClose(h);
    sprintf(uninst,"%s\\" APPMENU ,bfr);
    RemoveDirectory(uninst);
   }
   else if(i!=CSIDL_COMMON_PROGRAMS){i=CSIDL_COMMON_PROGRAMS; goto lbNT;}
  };
}

}

