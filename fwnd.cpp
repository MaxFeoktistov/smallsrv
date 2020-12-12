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


#ifndef SRV_H
#include "srv.h"
#endif
void *id_heap;
#ifdef MINGW
extern "C" { 
int WINAPI WinMain1( HINSTANCE hinst, HANDLE prev_inst, LPSTR cmline, int cmdshow );
void start()
{
   id_heap=GetProcessHeap();
 
// GetInstance 
  //hinstanse=  GetModuleHandle(NULL);
  //cmdline=GetCommandLine()  ;
  ExitProcess(WinMain1(GetModuleHandle(NULL) ,// hinstanse,
                       0,
                      GetCommandLine() //cmdline
                      ,0));
}

};

#define MALLOC(n) HeapAlloc(id_heap,HEAP_ZERO_MEMORY,n)
#define REALLOC(p,n)  HeapReAlloc(id_heap,HEAP_ZERO_MEMORY,p,n)
#define WFREE(p)  HeapFree(id_heap,0,p)
/*

void * operator new[](unsigned int s)
{
  return MALLOC(s);
}
void operator delete[](void *a)
{
  WFREE(a);
}

void operator delete(void *a)
{
  WFREE(a);
}
*/


#endif

//#include "srvdat.cpp"
#ifndef CD_VER
#include "wndcfg.cpp"
#endif

int IsDChk(HWND hwnd,int d){return SendDlgItemMessage(hwnd,d,BM_GETCHECK,0,0);}
int GetDlgItemTextLen(HWND hwnd,int d){return SendDlgItemMessage(hwnd,d,WM_GETTEXTLENGTH,0,0);}

long CALLBACK DefProc(HWND hwnd, UINT msg,UINT wparam, LONG lparam)
{union{
 RECT  xrc;
 char *bfr;
 //APPBARDATA  abd;
 POINT ap;
  void *pp;
  host_dir *ph;
  char *pm;
  char **pe;
  User *pu;
 };
 int i,j,k,l;
 char *p;
 union{
  char *t;
  host_dir *pth;
  HWND  tw;
 };
 switch(msg)
 {case WM_CREATE:
  case WM_INITDIALOG:
    return 1;
  case WM_DESTROY:
    if(hwnd == dwnd2)
    {dwnd2=0;
#if (! defined(CD_VER) ) && (! defined(FREEVER) )
     if((dlg2[0].Style&DS_MODALFRAME) && (user_name!=(about+sizeof(ABOUT_STR)+14)) )
     {is_no_exit=0; PostQuitMessage(0);}
#endif
    }
    else if(hwnd == dwndc )
    {dwndc=0;
     SaveCfgIfNeed(); 
     /*
     if(s_flg&FL_CFGUNSV)
     {bfr=new char[0x10000];
      SaveCfgFile(bfr);
      delete bfr;
     }
     */
    }
    else PostQuitMessage(0);
    break;
#ifndef CD_VER
  case WM_MOUSEWHEEL:
  case WM_KEYDOWN:
  case WM_VSCROLL:
  lbScroll:
   if(hwnd!=dwndc)break;
   i=j=GetScrollPos(hwnd,SB_VERT);
   if(msg==WM_KEYDOWN)
   {switch((lparam>>16)&0x7F)
    {case 0x51: //PgDn
       goto lPgDn;
     case 0x49: //PgUp
       goto lPgUp;
     case 0x48: //Up
       goto lUp;
     case 0x50: //Down
       goto lDn;
     case 0x1C: //Enter
       if( (tw=GetFocus()) && (GetWindowLong(tw,GWL_STYLE)&ES_WANTRETURN) )goto lEx; //break;
       goto lbOk;
     case 0x1:
       goto lbCancel;
    }
    break;
   }
   if(msg==WM_MOUSEWHEEL)i=-(((short)HIWORD(wparam))>>2);
   else switch(LOWORD(wparam))
   {lUp:
    case SB_LINEUP: i=-8; break;
    lPgUp:
    case SB_PAGEUP: i=-60; break;
    lDn:
    case SB_LINEDOWN: i=8; break;
    lPgDn:
    case SB_PAGEDOWN: i=60; break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK: i=HIWORD(wparam)-j; break;
    default: goto lEx;
   };
   GetScrollRange(hwnd,SB_VERT,&k,&l);
   if( (j+=i)<0 ){i+=-j; j=0;}
   else if(j>l){i-=j-l; j=l;}
   SetScrollPos(hwnd,SB_VERT,j,1);
   ScrollWindowEx(hwnd,0,-i,0,0,0,0, SW_INVALIDATE|SW_SCROLLCHILDREN|SW_ERASE);
   UpdateWindow(hwnd);
   break;
#endif
  case WM_USER:
   if(lparam==WM_LBUTTONUP)
   {lxz0:
#ifndef CD_VER
    if(ServiceStatus.dwCurrentState==SERVICE_PAUSED && wparam==WM_LBUTTONUP )
    {MessageBox(0, sWARNING__ ,wnd_name,MB_OK);
     ServiceStatus.dwCurrentState=SERVICE_RUNNING;
     SetServiceStatus(sesh,&ServiceStatus);
    }
#endif
    ;
    if(0){
  case WM_CLOSE: if( (s_aflg&AFL_EX2) || !(s_flg&0x10) ) break;
    }
    if(wstate)
    {ShowWindow(hwnd,SW_RESTORE);
     wstate=0; ShowProt();
    }
    SetForegroundWindow(hwnd);
    if(msg==WM_CLOSE) if(MessageBox(0, sDO_YOU_WA ,wnd_name,MB_YESNO|MB_ICONQUESTION) != IDNO)break;
   }
   else if(lparam == WM_RBUTTONUP)
   {SetForegroundWindow(hwnd);
  case WM_LBUTTONDBLCLK:       
    GetCursorPos(&ap);
    TrackPopupMenuEx(hmnu,TPM_RIGHTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON|TPM_VERTICAL
    ,ap.x,ap.y,hwnd,0);
   }
   return 1;
  case WM_SYSCOMMAND:
  case WM_COMMAND:
   switch( (((i=LOWORD(wparam))<800) || (i>=3900) )? i:700+(i&7) )
   {
   #ifndef CD_VER
    case 3900:
    case 3901:
    case 3902:
    case 3903:
    case 3904:
    case 3905:
    case 3906:
    case 3907:
    case 3908:
    case 3909:
    case 3910:
    case 3911:
    case 3912:
    case 3913:
      if((i=LOWORD(wparam)-3900)<iofs)
      {wparam=(MnuOffset[i]<<16)|SB_THUMBTRACK;
       goto lbScroll;
      }
      break;
    case 700: if(HIWORD(wparam)!=EN_KILLFOCUS)break; //Edit
    case 701: //<
    case 702: //>
      k=(i-800)>>3;
      l=j=GetDlgItemInt(hwnd,i&0xFF8,0,0);
      if(!(i&7)){if(!ConfigParams[k].max)break; }
      else j+=1-(i&1)*2;
      if(j<(int)ConfigParams[k].min )j=ConfigParams[k].min;
      if(j>ConfigParams[k].max )j=ConfigParams[k].max;
      if(l!=j)SetDlgItemInt(dwndc,i&0xFF8,j,0);
      break;
    case 729:
    case 703:
      if(GetOpenFileName(&ofn) && dwndc)SetDlgItemText(dwndc,i&0xFF8,fnamebuf);
      break;
    case 769:
    case 793:
    case 704:
      if(SHGetPathFromIDList( (_ITEMIDLIST *)SHBrowseForFolder(&binf),fnamebuf) && dwndc)
      SetDlgItemText(dwndc,i&0xFF8,fnamebuf);
      break;
    case 705:
      k=(i-800)>>3;
      if(!ConfigParams[k].max)
      {EnableWindow( GetDlgItem(hwnd,l=800+((k+1)<<3)) ,j=!(IsDChk(hwnd,i)&1));
       if(ConfigParams[k+1].max){
        EnableWindow( GetDlgItem(hwnd,l+1),j);
        EnableWindow( GetDlgItem(hwnd,l+2),j);
        SetDlgItemInt(hwnd,l,j? (j=*(ConfigParams[k+1].v)?j:2):0,0);
       }else
       {EnableWindow( GetDlgItem(hwnd,l+3+(ConfigParams[k+1].min==255)),j);
        SetDlgItemText(hwnd,l,(j && ConfigParams[k+1].v)? * (char **) (ConfigParams[k+1].v):NullString);
       }
      }
      break;
   case 720:
   case 740:
   case 760:
   case 780: SelectInList(i); break;
   case 721://Add CGI
    i=0;
    for(pe=ext;*pe;pe+=2)if(++i>10)return 0;
    *pe=NullString; pe[1]=NullString; pe[2]=0;
    if(0){
   case 722://Change CGI
    if(!(pp=CurInList(720,fnamebuf)) )return 0;  };
    i=GetDlgItemTextLen(hwnd,724);
    j=GetDlgItemTextLen(hwnd,728);
    if( (i>strlen(*pe)) || (j>strlen(pe[1])) )
    {pe[1]=((*pe=new char[i+j+4])+i+2);}
    GetDlgItemText(hwnd,724,*pe,256);
    if(**pe!='.'){**pe='.'; GetDlgItemText(hwnd,724,(*pe)+1,256);};
    GetDlgItemText(hwnd,728,pe[1],256);
    if(0){
   case 723: //del CGI
    for(pp=CurInList(720,fnamebuf);pe;pe+=2 ){if(!(*pe=pe[2]))break;pe[1]=pe[3];};
    }
    ReinitListBox(720,(ListNext)CGIListNext,ext);
    break;
   case 742: //Change mime
    if( (!(pp=CurInList(740,fnamebuf))) || !mime )return 0;
    i=GetDlgItemTextLen(hwnd,744);
    j=GetDlgItemTextLen(hwnd,745);
     if( (l=strlen(fnamebuf)+1)>(i+j+1) )
     {GetDlgItemText(hwnd,744,pm,256);
      GetDlgItemText(hwnd,745,pm+i+1,256);
      pm[i]=';';pm[i+j+1]=';';
      strcpy(pm+i+j+2,pm+l);
     }
     else
     {strcpy(pm,pm+l);
   case 741: //Add
     i=GetDlgItemTextLen(hwnd,744);
     j=GetDlgItemTextLen(hwnd,745);
     p=new char [(l=(mime)?strlen(mime):0)+i+j+5];
     if(l){strcpy(p,mime);  p[l]=';'; }
     else l=-1;
     mime=p;
     GetDlgItemText(hwnd,744,mime+l+1,256);
     if(mime[l+1]!='.'){mime[1+l++]='.'; GetDlgItemText(hwnd,744,mime+l+1,256);}
     GetDlgItemText(hwnd,745,mime+l+i+2,256);
     mime[l+i+1]=';';
     if(0){
   case 743: //del
      if(!(pp=CurInList(740,fnamebuf)))return 0;
      l=strlen(fnamebuf);
      strcpy(pm,pm+l+(pm[l]==';'));
     }}
    ReinitListBox(740,(ListNext)MimeListNext,mime);
    break;
   case 762: //Change u
     if(!(pp=CurInList(760,fnamebuf)) )return 0;
     i=GetDlgItemTextLen(hwnd,764);
     j=GetDlgItemTextLen(hwnd,765);
     k=GetDlgItemTextLen(hwnd,768);
     p=pu->pasw();
     t=pu->dir(p);
     if( (((ulong)t -(ulong)pu) +strlen(t) )< (i+j+k+sizeof(*pu) ) )
     {pu->state=0;
         
   case 761: //Add u
     i=GetDlgItemTextLen(hwnd,764);
     j=GetDlgItemTextLen(hwnd,765);
     k=GetDlgItemTextLen(hwnd,768);
     pm=new char[i+j+k+sizeof(*pu)+3 ];
     pu->next=userList;
     userList=pu;
     }
     GetDlgItemText(hwnd,764,pu->name,256);
     GetDlgItemText(hwnd,765,pu->name+i+1,256);
     GetDlgItemText(hwnd,768,p=pu->name+i+j+2,256);
     CreateDirectory(p,&secat);
     i=0x80;
     if( IsDChk(hwnd,770)) i|=UserFTPR ;
     if( IsDChk(hwnd,771)) i|=UserFTPW ;
     if(!IsDChk(hwnd,772)) i|=UserNOCGI;
     if( IsDChk(hwnd,774)) i|=UserSMTP ;
     if( IsDChk(hwnd,775)) i|=UserADMIN;
     if( IsDChk(hwnd,776)) i|=UserHTTP;
     if( IsDChk(hwnd,773))
     {i|=UserPOP3 ;
      sprintf(fnamebuf,"%s\\mbox",p);
      CreateDirectory(fnamebuf,&secat);
     }
     pu->state=i;
    if(0){
   case 763: //del
     if(!(pp=CurInList(760,fnamebuf)) )return 0;
     pu->state=0;
    }
    ReinitListBox(760,(ListNext) UserListNext,userList);
    break;
   case 782: //Change h
    if(!(pp=CurInList(780,fnamebuf)) )return 0;
    for(pth=&hsdr;pth;pth=pth->next) if(pth->next==ph)
    {pth->next=ph->next; break; }
   case 781: //Add h
    i=GetDlgItemTextLen(hwnd,784);
    j=GetDlgItemTextLen(hwnd,792);
    k=0;
    if((IsDChk(hwnd,796)))k=GetDlgItemTextLen(hwnd,797);
     pm=new char[i+j+k+12];
     ph->d=pm+10+i;
     ph->next=hsdr.next;
     hsdr.next=ph;
     GetDlgItemText(hwnd,784,ph->h,256);
     GetDlgItemText(hwnd,792,ph->d=ph->h+i+1,256);
     if(k)GetDlgItemText(hwnd,797,ph->d+(ph->flg=j+1),256);
     if(IsDChk(hwnd,794) && ph->h[0]!='/' )
     {strcpy(fnamebuf+GetWindowsDirectoryA(fnamebuf,256), (s_aflg & 0x80000000)?
       "\\hosts": "\\SYSTEM32\\DRIVERS\\ETC\\hosts"   );
      if(  ((i=_lopen(fnamebuf,1))>0 ) || ((i=_lcreat(fnamebuf,0))>0 ) )
      {_llseek(i,0,2);
       j=GetDlgItemText(hwnd,795,fnamebuf,256);
       _hwrite(i,fnamebuf,j+sprintf(fnamebuf+j," %s\r\n",ph->h));
       _lclose(i);
      };
     }
   if(0){
   case 783: //del
     if(!(pp=CurInList(780,fnamebuf)) )return 0;
     for(pth=&hsdr;pth;pth=pth->next) if(pth->next==ph)
     {pth->next=ph->next; break;}
   }
    ReinitListBox(780,(ListNext)HostListNext,hsdr.next);
    break;
   case  796:
   case  794: EnableWindow( GetDlgItem(hwnd,i+1),(IsDChk(hwnd,i)&1));
     if(i==794 && !GetDlgItemTextLen(hwnd,795))SetDlgItemText(hwnd,795,"127.0.0.1");
     break;
   case 346: Restart();
   case 345: SMTPCounter=0x7FF; return 1;
   case 347: LoadDomainM(); return 1;
#endif
   case 344: MessageBox(0,about,wnd_name,MB_OK); return 1;
   case 125:// Open Window
     goto lxz0;
#ifndef CD_VER
   case 126:// &Setting / Current state
     if(!dwndc)CreatCfgWindow();
     break;
     case 354:// Setting / Current state
     case 355:// Users && Virtual Hosts
     case 356:// Current statistics
     case 352:// Description (&Local)
     case 350:// What &new?  (Online)
     case 353:// Order regestration
      p=(char *)(url_desc[LOWORD(wparam)-350]);
      if(p[7]=='1')
       if(s_flgs[1]&FL1_ATLS && max_srv[5]){
        t=new char[48];
        sprintf(t,"https://%.9s:%u%s",p+7,ssl_port,p+16);
        goto lb5;
       }
       else if(http_port!=80 && p[16]!=':' )
       {t=new char[48];
        sprintf(t,"%.16s:%u%s",p,http_port,p+16);
       lb5:
        url_desc[LOWORD(wparam)-350]=p=t;
       }
      ShellExecute(0,"open",p,0,0,SW_SHOWNORMAL);
      return 1;
#endif
     case 150:/*"E&xit" */
     lbCancel:
       DestroyWindow(hwnd);
       return 1;
#ifndef CD_VER
     case 710:
     lbOk:
       OkCfgWindow();
#endif
    }
    break;

  case WM_SIZE:
    if( (wparam==SIZE_MAXHIDE) || (wparam==SIZE_MINIMIZED) )
    {ShowWindow(hwnd,SW_HIDE); wstate=1;} break;

 };
lEx:
 return DefWindowProc(hwnd,msg,wparam,lparam);
};


