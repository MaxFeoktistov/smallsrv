/*
 * Copyright (C) 1999-2022 Maksim Feoktistov.
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
#define MAINSTR 1
#include "g4strhtm.hh"
#endif

#ifndef SRV_H
#include "srv.h"
#endif

#include "icfghtm.cpp"

#ifndef CD_VER

//const char AuthErr1[]="HTTP/1.0 401\r\nWWW-Authenticate: Basic realm=\"Administration of Small HTTP server\"\r\nContent-Type: text/html\r\n\r\n<center><h2>Authorization request<br><a href=/$_admin_$stat>Login</a></h2></center>";
//const char AuthErr[]="HTTP/1.0 401\r\nWWW-Authenticate: Basic realm=\"Authorization request\"\r\nContent-Type: text/plain\r\n\r\nAuthorization request\r\n";
char *charset="";
int Req::ChkValidPth(char *p,char *b)
{
#ifndef SYSUNIX
 if( (!CheckBadName(p)) || WORD_PTR(p[1])!=0x5C3A )
#else
 if( (!CheckBadName(p)) || p[0]!='/' )
#endif
 if( (!CheckBadName(p)) || WORD_PTR(p[1])!=0x5C3A )
 {HTTPAdminErrorMessage(
#ifdef RUS
  "Плохой путь. Укажите полное путь-имя."
#else
   sBAD_PATHN
#endif
  ,b);
  return -1;
 }
 return 0;
};
//--
#ifdef SYSUNIX
int a_is_no_exit=1;
#define is_no_exit a_is_no_exit
#endif

#undef send
#undef recv
#define send(a,b,c,d) Send(b,c)
#define recv(a,b,c,d) Recv(b,c)
int logo_cntr;

User * Req::ChUser(int typ)
{
 User *puser=0;
 char *u,*p;
 char *b;

 if((u=CheckAuth(p=b=loc+8192+260)))
 {
     puser=FindUser(u,typ,p,this);
 }
 return (
      (!puser)
      || (DWORD_PTR(loc[dirlen+10])==0x6F676F6C x4CHAR("logo") && ++logo_cntr&1)
    )? 0 : puser  ;
}

int Req::Admin()
{
 int l;
 char *u,*p,*t,*t1;
 char *b;
 int i2=0;
 union{
 User *tuser;
 host_dir *a;
 };
 host_dir *ab;
 int i,j,k;
 uint who;

 DBGLINE

 BFILE bfl;

   p=b=loc+8192+260;
   bfl.Init(this,(PrintFlush)(Snd),b);

   if( ! (tuser=ChUser(UserADMIN)) )
   {
  lbLogout:
#ifdef WITHMD5
       SendDigestAuthReq(loc);
#else
    Send(AuthErr, strlen(AuthErr)  ) ;// sizeof(AuthErr)-1);
#endif
    return -1;
   }
   if(s_flg&FL_RADMIN)
   {
    bfl.bprintf(  HTMLTop, ( (charset && charset[0]) || lang_data ) ? charset : "; charset=utf-8", HTML_TOP2);
    bfl.fflush();


    if( DWORD_PTR(loc[dirlen+10])==0x656C6966 x4CHAR("file") )
    {
     if(Wmail(tuser) == 162) LoadDomainM();
     return  0;
    }
   }
   else
   {
    bfl.bprintf(  HTMLTop, ( (charset && charset[0]) || lang_data ) ? charset : "; charset=utf-8" ,"");
   }

   bfl.fflush();

   DBGLINE



  if((u=GetVar(http_var,"CONTENT_LENGTH")) && (l=atoui(u)) )
  {if(l>0x8000)
   {HTTPAdminErrorMessage("Too long request",b);
    goto lbEx;
   }
   while(postsize<l)
   {if((i=Recv(pst+postsize,l-postsize))<=0) return 0;
    postsize+=i;
   }
   req=pst;
   pst[postsize]=0;
  }
  prepare_Req_var();
  if( !(s_flg&FL_RADMIN) )
  {if(DWORD_PTR(loc[dirlen+10])!=0x74617473 x4CHAR("stat")
#ifdef SYSUNIX
    && DWORD_PTR(loc[dirlen+10])!=0x69676572
#endif
    && DWORD_PTR(loc[dirlen+10])!=0x72737573
    && DWORD_PTR(loc[dirlen+10])!=0x65747473 x4CHAR("stte")

   )
   {HTTPAdminErrorMessage(
#ifdef RUS
    "Сначала выберите \"Разрешить удаленное администрирование\"."
#else
     sCHECK___E
#endif
    ,b);
    DWORD_PTR(loc[dirlen+10])=0x74617473 x4CHAR("stat");
   }

  }
  switch(who = DWORD_PTR(loc[dirlen+10]))
  {
   case 0x666E6F63 x4CHAR("conf") :
    if( req_var != (char**)  &NullLong)
    {CfgParam *cp;
     for(cp=ConfigParams;cp->desc;++cp)
     {
      if( cp->v )
      {
       if( (u=GetVar(req_var,cp->name)))
       {
        if(cp->max)
        {if( (l=atoui(u))!=*(cp->v) )
         {is_no_exit=!(cp->IsR());
          if(l>*(cp->v) && strstr(cp->name,"max"))i2++;
          *(cp->v)=l;
         }
        }
        else
        {
         if( ((*cp->v)?strcmp(u,*(char**)(cp->v)):u[0] ))
         { l=strlen(u);
           if( (*cp->v) && l<strlen(*(char**)cp->v) )
           {

            if(*u)
              strcpy(*(char**)(cp->v),u);
            else
            {
             if(cp->v == (uint *)&cgi_detect )
               (*(char**)(cp->v))= "\\cgi-bin\\";
             else if(   (cp->v != (uint *)&   def_dir)
                     && (cp->v != (uint *)&  out_path)
                     && (cp->v != (uint *)&  err_path)
                     && (cp->v != (uint *)& smtp_name)
                     && (cp->v != (uint *)& def_name )
                    )*cp->v=0;
            }
           }
           else if(l)
           {
             FREE_IF_HEAP2((*(char**)(cp->v)));
             strncpy((*(char**)(cp->v))=new char[l+8],u,l+1);
           }
           else  *(char**)(cp->v) =  NullString;
         }
        }
       };
      }
      else if(cp->max)
      {if( GetVar(req_var,cp->name) )s_flgs[cp->min] |=cp->max;
       else s_flgs[cp->min]&=~cp->max;
      }
     }
     CheckValidCGIIdent();

     if( (s_flgs[2]&FL2_DOH) && doh_w<=0 )
     {
#ifdef SYSUNIX
        pipe(doh_pipe);
#else
        CreatePipe((HANDLE *)&doh_r,(HANDLE *)&doh_w,&secat,0x400);
#endif
     }

     u=SaveCfgFile(b);
     if(i2){i2=max_tsk;
      max_tsk=max_cln+max_ftp+max_pop+max_smtp+max_prx+max_ssl;
      Req **rr1, **rrq=(Req **) Malloc(sizeof(void*)*max_tsk*2+20 );
                  //new void *[max_tsk*2+20];
      memcpy(rrq,rr1=rreq,i2);
      memcpy(rrq+max_tsk,hndls,i2);
      hndls=(THREADHANDLE *)((rreq=rrq)+max_tsk);
      delete rr1;
     }
    }

   if(0){
   case 0x656C7061 x4CHAR("aple"):
    if((u=GetVar(req_var,"n")) &&
       ( (i=atoui(u))<10     ) &&
       (t=GetVar(req_var,"e")) &&
       ((p=GetVar(req_var,"s")) || (p=GetVar(req_var,"s1")) ) &&
       (u=GetVar(req_var,"t"))
      )
    {i*=2;
     if(!strcmp(p,"Remove") ) while(ext[i]=ext[i+2] )++i;
     else
     {if(ChkValidPth(u,b))goto lbOutCfg;
      l=strlen(t);
      if(*t!='.')l++;
      j=strlen(u);
      if(ext[i])
      {if( (( (k=strlen(ext[i])
              ) <4
            )&& (l>k)
           )||
           (j>strlen(ext[i+1]))
         )
       {while(ext[i]=ext[i+2] )++i;
        goto lbNewExt;
       }
      }
      else
      {
      lbNewExt:;
        ext[i+1]=(ext[i]=new char[j+6])+5;
        DWORD_PTR(ext[i][1])=0;
      }
      DWORD_PTR(*ext[i])=(*t=='.')?DWORD_PTR(*t):(DWORD_PTR(*t)<<8)|'.';
      strcpy(ext[i+1],u);
     }
     SaveCfgFile(b);
    }
  if(0){
  case 0x656D696D x4CHAR("mime"):
   if((t=GetVar(req_var,"e")) &&
       ((p=GetVar(req_var,"s"))  || (p=GetVar(req_var,"s1")) ) &&
      (u=GetVar(req_var,"t"))
     )
   {
     if(t1=mime)
     {
      while(t1=strstr(t1,t) )
      {
       if((*t=='.') || (t1[-1]=='.') )break;
       ++t1;
      }
      l=strlen(mime);
     }
     else l=0;
     if( !strcmp(p,"Remove") )
     {
       if(!t1)
       {
         goto lbOutCfg;
       }
       if( (p=strchr(t1,';')) && (p=strchr(p+1,';')))
       {
        strcpy(t1,p+1);
       }else
       {
         if(t1==mime)mime=0;
         else t1[-1]=0;
       }
     }
     else
     { i=sprintf(b,(char *)((*t=='.')?"%s;%s":".%s;%s"),t,u);
       if(t1)
       {
         if( (p=strchr(t1,';')) && (p=strchr(p+1,';')))
         {
           if( (p-t1)>=i )
           {
            strcpy(t1,b);
            strcpy(t1+i,p);
           }
           else
           {
            strcpy(t1,p+1);
            goto lbAddMime;
           }
         }
       }
       else
       {
       lbAddMime:
         sprintf(p=new char[l+i+2],(l>0)?"%s;%s":"%s",b,mime);
         mime=p;
       }
     }
     SaveCfgFile(b);
   }
   }}
   lbOutCfg:
   if(0){
   case 0x6E646E73 x4CHAR("sndn") : SMTPCounter=0x7FF;
   }
   if(0){
   case 0x736E646C x4CHAR("ldns") : LoadDomainM();
   }
     HTTPOutCfg(& bfl ); //b);
     if( (!is_no_exit) && req!=NullString )
     {
   case 0x74736572 x4CHAR("rest") :
       AddToLog( s_R_N____R ,s, &sa_c46);
       CloseSocket(s);
       u=cmdline; if(*u=='\"')++u;
       RestartServer(u,1);
     }
     break;

   case 0x61656C63 x4CHAR("clea") :
    {
     p=GetVar(req_var,"s");
     if(p)
     {
#ifdef USE_IPV6
        if( (u=GetVar(req_var,"n6")) )
        {
            LimitCntrIPv6 *pv;
            in6_addr xaddr;
           // MListCntr *pm;
            //i=atoui(u);
            IPv6Addr(xaddr.s6_addr16,u);
            j=atoui(p);
            if(j<9 && ipcnts[j].n)
              if(!i){ipv6cnts[j].d[0].cnt=0; }
              else if((pv=ipv6cnts[j].Find(xaddr)))ipv6cnts[j].Del(pv);
        }
        else
#endif
        if( (u=GetVar(req_var,"n")) )
        {
            LimitCntr *pv;
         //   MListCntr *pm;
            i=atoui(u); j=atoui(p);
            if(j<9 && ipcnts[j].n)
              if(!i){ipcnts[j].d[0].cnt=0; }
              else if((pv=ipcnts[j].Find(i)))ipcnts[j].Del(pv);

        }
     }
    }
if(0){
   case 0x70766C63 x4CHAR("clvp") :
   case 0x61657262 x4CHAR("brea") :
     if( (u=GetVar(req_var,"n")) && (t=GetVar(req_var,"t" )) )
     {
       i=atoui(u);
       j=atoui(t);
#ifdef TLSVPN
       if (0x70766C63 x4CHAR("clvp") == who)
       {
         ClearLimits(i, j);
       }
       else
#endif
       {
         ++no_close_req;
         if(i & CONID_KEEPALIVE_MASK)
         {
           i&=~CONID_KEEPALIVE_MASK;
           if(i<KeepAliveCount && KeepAliveList && KeepAliveList[i]->tmout==j)
           {
             RemoveAndDelKeepAlive(i);
           }
         }
         #ifdef TLSVPN
         else if(i ==  CONID_VPNCL_VALUE)
         {
           if(vpn_cln_connected && vpn_cln_connected->tmout == j)
             vpn_cln_connected->Shutdown();
         }
         else if(i & CONID_VPN_MASK)
         {
           i&=~CONID_VPN_MASK;
           if(i<vpn_count && vpn_list[i]->tmout==j )
             vpn_list[i]->Shutdown();
         }
         #endif
         else if( (i<max_tsk) &&
           ((u_long)(rreq[i]) )>1  &&
           rreq[i]->tmout==j
         ) rreq[i]->Shutdown();

         dec_no_close_req();
         Sleep(50);
       }
     }
   }
   case 0x6F676F6C x4CHAR("logo") :
   case 0x70636864 x4CHAR("dhcp"):
     if((u=GetVar(req_var,"a"))) DelDHCPIP(atoui(u),GetVar(req_var,"d"),GetVar(req_var,"s"));
   case 0x65747473 x4CHAR("stte"):
    OutActualConn(b);
#ifdef TLSVPN
    OutVPNLimit(b);
#endif
    OutVirusList(b);
    for(i=0;i<9; ++i) if(ipcnts[i].n)
      OutSMTPLimit(b,i);
    if(total_dhcp_ip)ShowDHCP(b);


    break;
   case 0x72657375 x4CHAR("user") :
    if(req!=NullString)
    {
     if((u=GetVar(req_var,"n")) && ((p=GetVar(req_var,"s"))  || (p=GetVar(req_var,"s1")) )  )
     {
      if(!strcmp(p,"Add") )
      {
      lbAdd:
        t=GetVar(req_var,"p");
        p=GetVar(req_var,"d");
        if(ChkValidPth(p,b))goto lbOutUPg;
        if(p[(i=strlen(p))-1]==FSLUSH)p[--i]=0;
#ifdef NEWSTRUCT
        tuser=(User *) malloc( sizeof(User )+i+strlen(t)+strlen(u)+3 );
        tuser->name=(char *)(tuser+1);
        tuser->ddr=tuser->name+sprintf(tuser->name,"%s",u)+1;
        tuser->pwd=tuser->ddr+sprintf(tuser->ddr,"%s",p)+1;
        sprintf(tuser->pwd,"%s",t);
        UpdPwdCrypt(tuser->pwd);
#else
        tuser=(User *) new char[sizeof(User )+i+strlen(t)+strlen(u)+3 ];
        t1+=sprintf(t1=tuser->name,"%s",u)+1;
        t1+=sprintf(t1,"%s",t)+1;
        sprintf(t1,"%s",p);
#endif
        tuser->next=userList;
        userList=tuser;
        goto lbState;
      }
      else
      {
        if( (tuser=FindUser(u,0xFF)) )
        {
          if(!strcmp(p,"Remove") )
          {
            tuser->state=0;
          }
          else // change
          {
            char pbfr[40];
            p=GetVar(req_var,"p");
            t=tuser->pasw();
            if (Pass2Txt(pbfr,t))
              t=pbfr;
            if(strcmp(p,t))
            {
              DBGLA("other password: |%s| != |%s|", p, t)

              if(strlen(t)<strlen(p) )
              {tuser->state=0; goto lbAdd;}
              strcpy(t,p);
            }
            p=GetVar(req_var,"d");
            t=tuser->dir(t);
            if(strcmp(p,t))
            {
              if( strlen(t)<strlen(p) )
              {tuser->state=0; goto lbAdd; }
              strcpy(t,p);
            }
            lbState:
            i=0x80;
            if(GetVar(req_var,"ftp")) i|=UserFTPR ;
            if(GetVar(req_var,"wri")) i|=UserFTPW ;
            if(!GetVar(req_var,"cgi")) i|=UserNOCGI;
            if(GetVar(req_var,"smtp")) i|=UserSMTP ;
            if(GetVar(req_var,"adm")) i|=UserADMIN;
            if(GetVar(req_var,"hp")) i|=UserHTTP;
            if(GetVar(req_var,"pop" )){i|=UserPOP3 ;}
            tuser->state=i;
            tuser->MkDir();
          }
          SaveCfgFile(b);
        }
        else{ HTTPAdminErrorMessage( sUSER_NOT_ ,b); }
      }

     }
    }
    if(0)
    {
   case 0x74736F68 x4CHAR("host") :
     if(req!=NullString)
     {
      if( (u=GetVar(req_var,"h")) && (t=GetVar(req_var,"d")) && ( (p=GetVar(req_var,"s"))  || (p=GetVar(req_var,"s1")) )  )
      {ab=&hsdr;
       if((i=strcmp(p,"Remove")))
       {i=strlen(t);
        if( DWORD_PTR(t[3])!=0x2F2F3A70 x4CHAR("p://")  && ChkValidPth(t,b))goto lbOutUPg;
        if(t[i-1]==FSLUSH)t[--i]=0;
       }
       for(a=hsdr.next;a;a=(ab=a)->next)if(!stricmp(a->h,u))
       {
        {ab->next=a->next;
         break;
        }
       }
       if(i)
       {j=0;
        if(GetVar(req_var,"p"))
        {t1=GetVar(req_var,"n");
         j=strlen(t1);
        }
#ifdef NEWSTRUCT
        a=(host_dir *) Malloc(sizeof(host_dir) + i + j+ strlen(u) +5);
        a->h=(char *)(a+1);
#else
        a=(host_dir *) new char[sizeof(host_dir) + i + j+ strlen(u) +5];
#endif
        i=sprintf(a->d=p+sprintf(p=a->h,u)+1,t);
        if(j)strcpy(a->d+(a->flg=i+1),t1);

        a->next=hsdr.next;
        hsdr.next=a;
       }
      lbCmnHosts:
       if( (t=GetVar(req_var,"i")) && GetVar(req_var,"u") &&  u[0]!='/')
       {
#ifndef SYSUNIX
        strcpy(b+GetWindowsDirectoryA(b,256), (s_aflg & 0x80000000)?
         "\\hosts": "\\SYSTEM32\\DRIVERS\\ETC\\hosts"   );
        if( ( (i=_lopen(b,1))>0 ) || ((i=_lcreat(b,0))>0 ) )
#else
        if( ( (i=_lopen("/etc/hosts",1))>0 ) || ((i=_lcreat("/etc/hosts",0))>0 ) )
#endif
        {
         _llseek(i,0,2);
         _hwrite(i,b,sprintf(b,"%s %s\r\n",t,u));
         _lclose(i);
        };

       }
       SaveCfgFile(b);
      }
     }
   }

    lbOutUPg:
      HTTPOutUsersPage(&bfl); //b);
      break;
   case 0x74617473 x4CHAR("stat") :
       DBGLINE
       j=255;
     if( (u=GetVar(req_var,"t")) )j=atoui(u);
     if( flog && (u=GetVar(req_var,"n")) && ((k=atoui(u))<3232) )
     {
#define MAX_LOG_SIZE   0x4000000

#ifdef SEPLOG
      int hh[10];
      int ll;
      if( (FL2_SEPARATELOG & s_flgs[2])    )
      {
          i2=0;
          u=0;
          for(i=0; i<10; i++)
          {
            hh[i]=0;
            if(i==6 || i==8 || j==5) continue;
            if((i) && sepLog[i] == &gLog )   continue;
            if( j!=255 && j )
            {
               l= isCountAr[i]>>4;
               if(l!=i)  continue;
            }
            if(i2>=MAX_LOG_SIZE)continue;
            sepLog[i]->MkFileName(b,k);
            if( (hh[i]=_lopen(b,0))>0)
            {
               l=FileSize(hh[i]);
               i2+=l;
            }


          }
          l=i2;
      }
      else
      {
         gLog.MkFileName(b,k);
         memset(hh,0,sizeof(hh));
         l=0;
         MyLock(mutex_pcnt);

         if((hh[0]=_lopen(b,0))>0)
         {
           l=FileSize(hh[0]);
         }
      }

      if(l>MAX_LOG_SIZE)l=MAX_LOG_SIZE;
      if(l)
      {
        do{
            ///if((u=new char[l+64]))break;
            if( (u=(char *)malloc(l+64) ) )break;
            debug("No enought memory to load log (%u) bytes. Try half (%u)",l,l>>1);
            if((l>>=1)<0x4000)
            {
              goto lbEndStat;
            };
        }while(1);
        ll=0;
        for(i=0;i<10;i++)if(hh[i]>0)
        {
         i2=_hread(hh[i],u+ll,l-ll);
         ll+=i2;
         if(ll>=l)break;
        }
      }

   lbEndStat:
      for(i=0;i<10;i++)if(hh[i]>0)_lclose(hh[i]);
      MyUnlock(mutex_pcnt);
      if(u)
      {
       for(i2=0;i2<l;i2++)if(!u[i2])u[i2]=' ';
       u[l]=0;
       DBGLINE
       HTTPOutStatistics(loc,u,j,k);
       free(u);
       break;
      }


#else

      l=sprintf(b,"%s",flog);
      if( !(t=strrchr(b,'.')) )t=b+l;
      if(k)sprintf(t,"%4.4u",k);
      MyLock(pcnt);


      if((i=_lopen(b,0))>0)
      {l=_llseek(i,0,2);
       if(l>MAX_LOG_SIZE)l=MAX_LOG_SIZE;
       do{
        ///if((u=new char[l+64]))break;
        if( (u=(char *)malloc(l+64) ) )break;
        debug("No enought memory to load log (%u) bytes. Try half (%u)",l,l>>1);
        if((l>>=1)<0x4000)
        {_lclose(i);
         goto lbStat;
        };
       }while(1);

       _llseek(i,-l,2);

       i2=_hread(i,u,l);
       if(i2!=l)
       {
        debug("Error read log (%u of %u)  bytes.  (%d %s)",i2,l,errno, strerror(errno));
       }
       _lclose(i);

       MyUnlock(pcnt);
       if(i2<=0)goto lbStat;
       l=i2;
       for(i2=0;i2<l;i2++)if(!u[i2])u[i2]=' ';
       u[l]=0;
       DBGLINE
       HTTPOutStatistics(loc,u,j,k);
       i2=0;
     ///  delete u;
       free(u);
       break;
      }
      MyLock(pcnt);

#endif
     }
   lbStat:
#ifdef SEPLOG
    if( (u=(char *)malloc(LOG_SIZE*8) ) )
    {

       i2=0;
       for(i=0;i<10;i++)
       {
         if(i==6 || i==8) continue;
         if((!i) || sepLog[i] !=&gLog )
         {
            memcpy(u+i2,sepLog[i]->lb_prot,l=sepLog[i]->lpprot-sepLog[i]->lb_prot);
            i2+=l;
         }
       }
       u[i2]=0;
       HTTPOutStatistics(loc,u,j,3232);
       i2=0;
     ///  delete u;
       free(u);


    }
#else
     memcpy(loc,b_prot,i=pprot-b_prot);
     loc[i]=0;
     HTTPOutStatistics(loc+i+1,loc,j,3232);
#endif
     break;
   case 0x72737573 x4CHAR("susr") :
     HTTPUserStat();
     break;

#ifdef SYSUNIX
#ifndef FREEVER
   case 0x69676572 x4CHAR("regi"):
   if(! ((user_name==(about+sizeof(ABOUT_STR)+14)) || Registrate()))
   {HTTPOutRegPage(); break;}
#endif
#endif
   case 0x6C676F6C x4CHAR("logl") :
#ifdef SEPLOG
    b_prot=gLog.lb_prot;
    pprot=gLog.lpprot;
    if( FL2_SEPARATELOG & s_flgs[2]     )
    {
      j=0;
      if( (u=GetVar(req_var,"t")) )
      {
        j=atoui(u);
        if((uint)j < N_LOG){
            b_prot=sepLog[j]->lb_prot;
            pprot=sepLog[j]->lpprot;
        }
      }
      u=SrvNameSufix[j];
      if(*u == '.')u++;

      bfl.bprintf( "<h2>%s</h2><table border=0 width=100%%><tr align=center><td><a href=\"?t=0\">General</a></td>" LF, u );
      for(j=1; j < N_LOG; j++)
        if(sepLog[j]!=&gLog)
        {
            u=SrvNameSufix[j];
            if(*u != '.')continue;
            u++;
            bfl.bprintf( "<td><a href=\"?t=%u\">%s</a></td>" LF ,j, u);
        }
      bfl.bprintf( "</tr></table>");
      bfl.fflush();
    }
#endif
       HTTPOutCurentLog(b); break;
   default:
     HttpReturnError( sERROR__BA );
  }
  if( (req_var!= (char**)  &NullLong))delete req_var;
lbEx:
  HTMLOutBottom(b);
  return 0;
};
#undef send
#undef recv
#undef is_no_exit
#endif
