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


//#include <stdio.h>

extern "C" {

int TabStops[]={60,170,220};

short MnuOffset[20];
int iofs;


}

HWND CreateWindowExX(
  DWORD     dwExStyle,
  LPCSTR    lpClassName,
  LPCSTR    lpWindowName,
  DWORD     dwStyle,
  int       X,
  int       Y,
  int       nWidth,
  int       nHeight,
  HWND      hWndParent,
  HMENU     hMenu,
  HINSTANCE hInstance,
  LPVOID    lpParam
)
{
  if(strstr(charset," utf") )
  {
    ushort cm[64];
    ushort wm[256];
    int r=0;
    #ifdef UTF16SUPPORT
    if(strstr(charset," utf-16"))
    {
      //    r=utf162unicode((ushort *)lpClassName,cm);
      //    r+=utf162unicode((ushort *)lpWindowName,wm);
      return CreateWindowExW(
                             dwExStyle,
                             (LPCWSTR) lpClassName,
                             (LPCWSTR) lpWindowName,
                             dwStyle,
                             X,
                             Y,
                             nWidth,
                             nHeight,
                             hWndParent,
                             hMenu,
                             hInstance,
                             lpParam);
    }
    else
    #endif
    {
      r=utf2unicode((uchar *)lpClassName,cm);
      r|=utf2unicode((uchar *)lpWindowName,wm);
    }

    if(r>0)return CreateWindowExW(
                                  dwExStyle,
                                  (LPCWSTR) cm,
                                  (LPCWSTR) wm,
                                  dwStyle,
                                  X,
                                  Y,
                                  nWidth,
                                  nHeight,
                                  hWndParent,
                                  hMenu,
                                  hInstance,
                                  lpParam);

  }
  return CreateWindowEx(
    dwExStyle,
    lpClassName,
    lpWindowName,
    dwStyle,
    X,
    Y,
    nWidth,
    nHeight,
    hWndParent,
    hMenu,
    hInstance,
    lpParam);


};

#ifndef VPNCLIENT_ONLY

void ReinitListBox(int n,ListNext fln,void *lst)
{
 char bfr[784];
  SendDlgItemMessage(dwndc,n,LB_RESETCONTENT,0,(ulong)bfr );
  while(lst)
  {
   lst= (fln)(lst,bfr);
   SendDlgItemMessage(dwndc,n,LB_ADDSTRING,0,(ulong)bfr );
  };
 s_flg|=FL_CFGUNSV;
}

User   * UserListNext(User *tuser,char *bfr)
{
 char *p;
 int  j;
  p=tuser->pasw();
  j=sprintf(bfr,"%s\t%s\t",tuser->name,tuser->dir(p));
  j+=tuser->FlgString(bfr+j);
  bfr[j]=0;
  do{tuser=tuser->next;}while(tuser && !(tuser->state&0x80) );
  return tuser;
};

char ** CGIListNext(char **a,char *bfr)
{
  if(*a && a[1])
  {
    sprintf(bfr,"%s\t%s",*a,a[1]);
    a+=2;
    if(*a)return a;
  }else *bfr=0;
  return 0;
}

char * MimeListNext(char *a,char *bfr)
{
 char *t;
 strncpy(bfr,a,512);
 if(t=strchr(a,';'))
 {
   bfr[t-a]='\t';
   if( (t=strchr(t+1,';')) )
   {
    bfr[t-a]=0;
    return t+1;
   }
 }
 return 0;
}
host_dir * HostListNext(host_dir *a,char *bfr)
{
 sprintf(bfr,"%s\t%s\t%s",a->h,a->d,a->flg?a->d+a->flg:"" );
 return a->next;
};

ListNext  LstMsgLst[]={(ListNext)CGIListNext, (ListNext)MimeListNext,
                       (ListNext)UserListNext,(ListNext)HostListNext};

void* CurInList(int n,char *bfr)
{
 uint i,j,k;
 void *p;

  if( (i=SendDlgItemMessage(dwndc,n,LB_GETCURSEL,0,0) )>=0)
  {
   switch(j=(n-720)/20)
   {
    case 0: p=ext;  break;
    case 1: p=mime;  break;
    case 2: p=userList;  break;
    case 3: p=hsdr.next;  break;
    default: return 0;
   }

   for(k=0;k<i;++k)
   {
    if(!p)return 0;
    p=(LstMsgLst[j])(p,bfr);
   }
  }
  if(p)(LstMsgLst[j])(p,bfr);
  return p;
}
void SelectInList(int n)
{
 union{
  void *p;
  host_dir *ph;
  char     *pm;
  char    **pe;
  User     *pu;
 };
 char  *t;
 char bfr[784];
 int  i,j;
 if( (p=CurInList(n,bfr)) )
 {
  switch(n)
  {
   case 720:if(*pe){
            SetDlgItemText(dwndc,724,*pe);
            SetDlgItemText(dwndc,728, pe[1]); }
            break;
   case 740:if(t=strchr(bfr,'\t'))
            {*t++=0;
             SetDlgItemText(dwndc,744,bfr);
             SetDlgItemText(dwndc,745,t);
            }
            break;
   case 760:SetDlgItemText(dwndc,764,pu->name);
            SetDlgItemText(dwndc,765,t=pu->pasw());
            SetDlgItemText(dwndc,768,t=pu->dir());
            CheckDlgButton(dwndc,770,(pu->state&UserFTPR)?BST_CHECKED:BST_UNCHECKED );
            CheckDlgButton(dwndc,771,(pu->state&UserFTPW)?BST_CHECKED:BST_UNCHECKED );
            CheckDlgButton(dwndc,772,(!(pu->state&UserNOCGI))?BST_CHECKED:BST_UNCHECKED);
            CheckDlgButton(dwndc,773,(pu->state&UserPOP3 )?BST_CHECKED:BST_UNCHECKED);
            CheckDlgButton(dwndc,774,(pu->state&UserSMTP )?BST_CHECKED:BST_UNCHECKED);
            CheckDlgButton(dwndc,775,(pu->state&UserADMIN)?BST_CHECKED:BST_UNCHECKED);
            CheckDlgButton(dwndc,776,(pu->state&UserHTTP)?BST_CHECKED:BST_UNCHECKED);
            break;
   case 780:SetDlgItemText(dwndc,784,ph->h);
            SetDlgItemText(dwndc,792,ph->d);
            t=""; i=0; j=BST_UNCHECKED;
            if(ph->flg){t=ph->d+ph->flg; i=1; j=BST_CHECKED; }
            SetDlgItemText(dwndc,797,t);
            EnableWindow(GetDlgItem(dwndc,797),i);
            CheckDlgButton(dwndc,796,j);
  }

 }
};
int AddCfgListEdit(int pos,char *name,ListNext fln,void *lst,int base,int cntrl,...)
{
 int i=0;
 int *tt;
 int k=base+4;
 int j,n=0;
 HWND hwnd;
 char bfr[784];

  SendMessage(
  hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,"LISTBOX","",
      WS_VSCROLL | WS_HSCROLL | WS_CHILD |LBS_NOTIFY| WS_VISIBLE |  WS_BORDER | WS_TABSTOP| LBS_USETABSTOPS
                   ,4,pos+18,300,208,dwndc,(HMENU)base,hinstance,0)
  ,WM_SETFONT,j=(ulong)GetStockObject(17),0);
  SendMessage(hwnd,LB_SETTABSTOPS,3,(int)TabStops);
  SendMessage(hwnd,LB_SETHORIZONTALEXTENT,2048,0);
  while(lst)
  {
   lst= (fln)(lst,bfr);
   SendMessage(hwnd,LB_ADDSTRING,0,(ulong)bfr );
  };

  for(tt=&cntrl;*tt;tt+=2)
  {
    if(*tt&1)
    {//Edit
     if(n){n=0;  i++;};

     SendMessage(
      CreateWindowExX( 0,"STATIC",  (char*) (tt[1])  ,SS_LEFT|WS_CHILD|WS_VISIBLE
                   ,308,pos+18+i*21,312,16,dwndc,(HMENU)0,hinstance,0)
     ,WM_SETFONT,j,0);

    if(*tt&2)
    {
     k=(k+7)&~7;
     SendMessage(
     CreateWindowEx(WS_EX_CLIENTEDGE|WS_EX_NOPARENTNOTIFY|WS_EX_STATICEDGE,
       "BUTTON","..", BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP
        , 587, pos+36+i*21, 20, 20,
         dwndc,(HMENU)(k+1),hinstance,0)
      ,WM_SETFONT,j,0);
    }

     SendMessage(
     CreateWindowEx(WS_EX_STATICEDGE,"EDIT","",ES_LEFT|ES_AUTOHSCROLL|WS_CHILD|
                      WS_VISIBLE|WS_BORDER|WS_TABSTOP|( (*tt&4)?ES_PASSWORD:0 )|( (*tt&8)?WS_DISABLED:0 )
                   ,308,pos+36+i*21,274,20,dwndc,(HMENU)(k++),hinstance,0)
     ,WM_SETFONT,j,0);

    if(*tt&2) ++k;

     i+=2;
    }
    else
    {
      SendMessage(
      CreateWindowExX(0,"BUTTON",(char*) (tt[1]) ,
      BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                     308+n*100,pos+16+i*21,90,20,dwndc,(HMENU)(k++),hinstance,0)
      ,WM_SETFONT,j,0);
      if(++n>=3){n=0;  i++;};
    }
  }
  i=16+(i+1)*21;
  SendMessage(
    CreateWindowEx( 0,"BUTTON",
#ifdef RUS
    "Добавить"
#else
    "Add"
#endif
    ,WS_CHILD | WS_VISIBLE | WS_TABSTOP,
      320,pos+i //196
      ,80,20,dwndc,(HMENU)(base+1),hinstance,0)
    ,WM_SETFONT,j,0);

  SendMessage(
    CreateWindowEx( 0,"BUTTON",
#ifdef RUS
    "Изменить"
#else
    "Change"
#endif

    ,WS_CHILD | WS_VISIBLE | WS_TABSTOP,
      420,pos+i //196
      ,80,20,dwndc,(HMENU)(base+2),hinstance,0)
    ,WM_SETFONT,j,0);

  SendMessage(
    CreateWindowEx( 0,"BUTTON",
#ifdef RUS
    "Удалить"
#else
    "Delete"
#endif
    ,WS_CHILD | WS_VISIBLE | WS_TABSTOP,
      520,pos+i //196
      ,80,20,dwndc,(HMENU)(base+3),hinstance,0)
    ,WM_SETFONT,j,0);

  SendMessage(
  CreateWindowExX(0
      ,"BUTTON",name,BS_GROUPBOX|BS_CENTER|WS_CHILD|WS_VISIBLE|WS_DISABLED|WS_BORDER
                   ,0,MnuOffset[iofs++]=pos,612
                 ,n=(i<200)?220:i+24,dwndc,(HMENU)0,hinstance,0)
  ,WM_SETFONT,j,0);

 return pos+n+4;
}
#endif // VPNCLIENT_ONLY

#define  HGH 50

void CreatCfgWindow()
{
 CfgParam *cp;
 char  bfr[512],*t;
 HWND  tw;
 int i,j,k,l,chk=0,kk;
// char *dd, bfr2[300];
   iofs=0;
   dwndc=CreateWindowEx(0,"FMFROM","Configuration",
   DS_3DLOOK|WS_CAPTION|WS_VISIBLE|WS_POPUP|WS_SYSMENU
   |WS_DLGFRAME|WS_VSCROLL|WS_THICKFRAME
   ,0,0,640,480,0,MkMnu(mnu3,CreateMenu), hinstance, 0);
  SendMessage(dwndc,WM_SETFONT,j=(ulong)GetStockObject(17),1);


  for(k=0;ConfigParams[k].desc  ;++k)
  {
   kk=k*HGH;

   sprintf(bfr+
    sprintf(bfr,ConfigParams[k].desc,ConfigParams[k].adv),"%s", ConfigParams[k].IsR()?  RSTNEED :"");
//   sprintf(bfr,"%s%s",ConfigParams[k].desc, ConfigParams[k].IsR()? ". " RSTNEED :"");
//   printf("Desk%u:%s %s\n",k,ConfigParams[k].desc,bfr);
   if( ConfigParams[k].v )
   {
    tw=CreateWindowExX( 0,"STATIC", bfr,SS_RIGHT|WS_CHILD|WS_VISIBLE
                   ,8,16+kk,380,HGH-2,dwndc,(HMENU)0,hinstance,0);
    SendMessage(tw,WM_SETFONT,j,0);
    if(ConfigParams[k].max)
    {
      sprintf(bfr,"%u",*ConfigParams[k].v);
      SendMessage(tw=CreateWindowEx( WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY | WS_EX_STATICEDGE,
       "BUTTON","<", BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP|chk
          , 400, 20+kk, 20, 20,
         dwndc,(HMENU)(801+k*8),hinstance,0)
      ,WM_SETFONT,j,0);
      SendMessage(tw=CreateWindowEx( WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY | WS_EX_STATICEDGE,
       "BUTTON",">", BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP|chk
        , 480, 20+kk, 20, 20,
         dwndc,(HMENU)(802+k*8),hinstance,0)
      ,WM_SETFONT,j,0);

      tw=CreateWindowExX( WS_EX_STATICEDGE ,"EDIT",bfr,ES_LEFT|ES_AUTOHSCROLL|WS_CHILD|
                      WS_VISIBLE|WS_BORDER|WS_TABSTOP|ES_NUMBER|chk
                   ,420,20+kk,60,20,dwndc,(HMENU)(800+k*8),hinstance,0);
    }
    else
    {
      t=*(char **)ConfigParams[k].v;
      if( (i=ConfigParams[k].min==255) || (ConfigParams[k].min==256) )
      SendMessage(CreateWindowEx( WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY | WS_EX_STATICEDGE ,
       "BUTTON","..", BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP|chk
        , 587, 20+kk, 20, 20,
         dwndc,(HMENU)(803+k*8+i),hinstance,0)
      ,WM_SETFONT,j,0);
      l=20;
      if(ConfigParams[k].min==2048)
      {chk|=ES_AUTOVSCROLL|ES_MULTILINE|WS_VSCROLL|ES_WANTRETURN; l=40;}
      tw=CreateWindowExX( WS_EX_STATICEDGE,"EDIT",t?t:"",ES_LEFT|ES_AUTOHSCROLL|WS_CHILD|
                      WS_VISIBLE|WS_BORDER|WS_TABSTOP|chk
                   ,394,20+kk,192,l,dwndc,(HMENU)(800+k*8),hinstance,0);
    };
    chk=0;
   }
   else if(ConfigParams[k].name )
   {
     tw=CreateWindowExX( 0,"BUTTON", bfr,
      BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_TABSTOP |BS_MULTILINE,
                     8,24+kk,600,40,dwndc,(HMENU)(l=805+(k<<3)),hinstance,0);
     if(ConfigParams[k].max)
     {
      if( (ConfigParams[k].max&s_flgs[ConfigParams[k].min])==ConfigParams[k].max  )
        CheckDlgButton(dwndc,l,BST_CHECKED);
     }
     else
     {
      //if(!ConfigParams[k+1].v) MessageBox(0,ConfigParams[k+1].name,"Err",MB_OK); else
      if( ! *(ConfigParams[k+1].v) )
      {
        CheckDlgButton(dwndc,l,BST_CHECKED);
        chk=WS_DISABLED;
      }
     }
   }
   else
   {
    for(i=1;ConfigParams[k+i].name && ConfigParams[k+i].desc;++i);
    tw=CreateWindowExX(0
      ,"BUTTON", ConfigParams[k].desc,BS_GROUPBOX|BS_CENTER|WS_CHILD|WS_VISIBLE|WS_DISABLED
                   ,0,
                   MnuOffset[iofs++]=24+kk,612,HGH*i-8,dwndc,(HMENU)0,hinstance,0);

   }
   SendMessage(tw,WM_SETFONT,j,0);

  }

  CreateWindowEx(0,
       "BUTTON","Ok", BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 180, 70+kk,68, 30,
         dwndc,(HMENU)710,hinstance,0);
  CreateWindowEx(0,
       "BUTTON","Cancel", BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, 420, 70+kk,68, 30,
         dwndc,(HMENU)150,hinstance,0);

#ifndef VPNCLIENT_ONLY

 kk=AddCfgListEdit(kk+108,mnu3[MNU_CGI].name //"CGI Interpreters"
#ifdef RUS
 ,(ListNext)CGIListNext,ext,720,1,".ext",3,"Приложение:",0);
#else
 ,(ListNext)CGIListNext,ext,720,1,".ext",3,"Application:",0);
#endif

 kk=AddCfgListEdit(kk/*+338*/,mnu3[MNU_CGI+1].name //"Mime types"
#ifdef RUS
    ,(ListNext)MimeListNext,mime,740,1,".ext",1,"MIME тип:",0);
#else
    ,(ListNext)MimeListNext,mime,740,1,".ext",1,"MIME type:",0);
#endif

#ifndef CD_VER
 kk=AddCfgListEdit(kk/*+578*/,mnu3[MNU_CGI+2].name //"Users"
  ,(ListNext) UserListNext,userList,760,1,
#ifdef RUS
  "Пользователь:",5,"Пароль:",3,"Домашний каталог:",
   8,"Читать FTP",  8,"Писать FTP",  8,"в т.ч. CGI",8,"POP3", 8,"SMTP",8,"Админ.",8,"Прокси",0);
#else
  "User:",5,"Password:",3,"Home:",
   8,"Read FTP",8,"Write FTP",8,"Upload CGI",8,"POP3",8,"SMTP",8,"Admin",8,"Proxy",0);
#endif
#endif
 kk=AddCfgListEdit(kk/*+808*/,mnu3[MNU_VHOST].name //"Virtual hosts"
 ,(ListNext)HostListNext,hsdr.next,780,1,
#ifdef RUS
 "Имя хоста или /каталог/:",3,"Каталог или http://host",8,"Добавить в",9,"системный хост файл.  IP:"
 ,8,"Приватный",9,"для (через запятую: пользователь[:R W NOFTP NOHTTP],..)"
 ,0);
#else
 "Host name or /directory/:",3,"Root dir or http://host",8,"Add entry",9,"to system hosts file.  IP:"
  ,8,"Private",9,"for users (list through comma: name[:R W NOFTP NOHTTP],..)"
 ,0);
#endif

#endif // VPNCLIENT_ONLY

 SetScrollRange(dwndc,SB_VERT,0,kk/*+820*/,0);
 ShowWindow(dwndc,SW_SHOWNORMAL);
};


