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


#ifndef MRC_H
#define MRC_H

//extern "C" {
//extern HANDLE hinstance;
extern HINSTANCE hinstance;

struct mrc_obj
{
 char    *name;
 ulong   id;
 char    *xClassName;
 ulong   Style;
 ushort  x,y,w,h;
 ulong   ExStyle;
};

HWND MkDlg(const mrc_obj *,HWND ,long font);

struct mrc_mnu
{
 ushort flg;
 ushort id;
 char *name;
};

//HMENU MkMnu(mrc_mnu *);
typedef  HMENU  WINAPI (*TCreateMenu)();
HMENU MkMnu(//const 
     mrc_mnu *mr,TCreateMenu crm=CreateMenu);

    
//}

#endif

