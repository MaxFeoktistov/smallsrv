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


#include <windows.h>
#include "mdef.h"
#include "mrc.h"

#ifdef DJGPP 
#define  HINST_CAST(a) a
#define  HMENU_CAST(a) a
#else
#define  HINST_CAST(a) (HINSTANCE__*) a
#define  HMENU_CAST(a) (HMENU__*) a
#endif

//extern "C" {

HWND MkDlg(const mrc_obj *mj,HWND parent,long font)
{
  HWND r;
  if( !(r=CreateWindowEx(mj->ExStyle,
                   mj->xClassName,
                   mj->name,
                   mj->Style,
                   mj->x,mj->y,mj->w,mj->h,
                   parent,
                   (HMENU ) mj->id,
                   HINST_CAST(hinstance),
                   0
                  ) ) )return 0;
  SendMessage(r,WM_SETFONT,font,0);
  for(mj++;mj->xClassName;mj++)
  {
   SendMessage(
   CreateWindowEx( mj->ExStyle,
                   mj->xClassName,
                   mj->name,
                   mj->Style,
                   mj->x*2,mj->y*2,mj->w*2,mj->h*2,
                   r,
                   (HMENU )  mj->id,
                   HINST_CAST(hinstance),
                   0
                 )
                   ,WM_SETFONT,font,0)
  ;
  };
  return r;
};


HMENU MkMnu(//const 
     mrc_mnu *mr,TCreateMenu crm)
{
 int next_id;
 HMENU r=(crm)(); //CreateMenu();
 while(mr->id)
 {
  if(mr->flg&MF_POPUP && (ulong)mr->id<128)
  {
   next_id=mr[mr->id].id;
   ((mrc_mnu *)mr)[mr->id].id=0;
   AppendMenu(r,mr->flg,(ulong)MkMnu(mr+1),mr->name);
   mr+=mr->id;
   ((mrc_mnu *)mr)->id=next_id;
  }
  else{
    AppendMenu(r,mr->flg,mr->id,mr->name);
    mr++;
  }
 }
 return r;
};

//}

