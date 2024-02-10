/*
 * Copyright (C) 1999-2024 Maksim Feoktistov.
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
//#include <sys/fsuid.h>

#ifdef SYSUNIX

#include <utime.h>
#define DBG_MSG(a...)
#else
#define DBG_MSG(a...)
//debug(a)
#endif

//#define printSendCMD(b,fmt,a...) if((send(s,(b),sprintf((b),fmt,## a),0) ) < 0){ dbg( sCAN__T_SE ); goto ex1; };
#define printSendCMD(b,fmt,a...)  rcodeLen=sprintf(rcode=b,fmt ,## a);
#undef send
#define send(a,b,c,d) xx
//BSend(a,b,c)

#ifdef SEPLOG

#undef debug
#undef AddToLog

#define debug(a...)  sepLog[2]->Ldebug(a)
#define AddToLog(a...)  sepLog[2]->LAddToLog(a)

#endif

int first_pass_port;

struct LPath
{User *puser;
 char *SrcPath;
 int  mode;
#define  MODE_UNICODE 0x10000
 char cpath[258];
 int SrcDirLen;
 int BSrcDirLen;
// #ifndef SYSUNIX
//   ushort ut[512];
// #endif
 int MkFName(char *t,const char *p,uint vh=FL_FTPVDIR);
};

int LPath::MkFName(char *t,const char *p,uint vh) //=FL_FTPVDIR)
{
  int i,j;
  char *x;
  SrcDirLen=BSrcDirLen;
  mode=0;
  if(*p=='/')
  {i=sprintf(t,"%.255s%.255s",SrcPath,p);
  }else
  {
    i=sprintf(t,"%.255s",cpath);
    while(t[i-1]=='/')t[--i]=0;
    if( i<SrcDirLen )goto exlp;
    #ifndef SYSUNIX
    //  for(x=t;*x;++x)if(*x=='/')*x='\\';
    for(x=t;*x;++x)if(*x=='\\')*x='/';
    #endif
    // DBG_MSG("%s <-> %s",t,p);
    while( WORD_PTR(*p)==0x2E2E)
    {
      if(!(x=strrchr(t,'/')))// FSLUSH)))
      {
        exlp:
        strcpy(t,SrcPath);
        // DBG_MSG("!!%s SrcDirLen=%u SrcDir=%s cpath=%s",t,SrcDirLen,SrcPath,cpath);
        return SrcDirLen;
      }
      *x=0;
      i=x-t;
      if( i<SrcDirLen )goto exlp;
      if(!p[2])goto lbVH;
      //return i;
      if(p[2]!='/')goto exlp;
      p+=3;
    }
    i+=sprintf(t+i,"/%.255s",p);
  }
  #if 1
  if( i<SrcDirLen )goto exlp;
  lbVH:
  if(vh&s_flg)
  {
    host_dir *a;
    char *vdir;
    DWORD_PTR(t[i])='/';
    t[SrcDirLen]='/';
    //  mode=0;
    for(a=hsdr.next;a;a=a->next)
      if(a->h[0]=='/' && strin(t+SrcDirLen,a->h)
        && a->flg  && 7&(mode=IsInStrLst(a->d+a->flg,puser->name))  )
      {x=t+i;
        /*
         *    vdir=t+SrcDirLen+strlen(a->h);
         *    if((i=x-vdir)>0) x=(char *) memcpy_back(t+220,x,i);
         *    else x="";
         *    i=sprintf(t,"%s/%s",a->d,x);
         */
        j=sprintf(t+256,"%.255s",t+SrcDirLen+strlen(a->h));
        i=sprintf(t,"%.255s/%s",a->d,t+256);
        SrcDirLen=i-j-1;
      }
      DWORD_PTR(t[i])=0;
  }
  #endif
  while((t[i-1]&0xFE)=='.' || t[i-1]=='\\' )t[--i]=0;
  // DBG_MSG("%s %u SrcDirLen=%u SrcDir=%s cpath=%s",t,i,SrcDirLen,SrcPath,cpath);
  // #ifndef SYSUNIX
  //   if(utf2unicode(t,ut)>0)
  //   {
  //     mode|= MODE_UNICODE;
  //   };
  //
  // #endif

  return i;
};

#ifdef SYSUNIX
#include <locale.h>
#include<pwd.h>
#include<grp.h>
const char atrib[5]={'-','x','w',' ','r'};
#else

int WINAPI lopen_UTF (char *f,ulong t)
{
  ushort ut[512];
   if(utf2unicode((uchar *)f,ut)>0)
   {
     return (int)CreateFileW((WCHAR *)ut,(0xB0000000ul<<t)&0xC0000000,FILE_SHARE_READ,&secat,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
   }
   return (int)CreateFile(f,(0xB0000000ul<<t)&0xC0000000,FILE_SHARE_READ,&secat,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
}
/////////////////////
int WINAPI lcreat2_UTF(char *f)
{
 ushort ut[512];
   if(utf2unicode((uchar *)f,ut)>0)
   {
     return  (int) CreateFileW((WCHAR *)ut,GENERIC_WRITE,FILE_SHARE_READ,&secat,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,0);
   }
   return (int) CreateFile(f,GENERIC_WRITE,FILE_SHARE_READ,&secat,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,0);
}
////////////////////////////////
int MoveFile_UTF(char *a,char *b)
{
 ushort ut[512];
 ushort ut2[512];
   if(utf2unicode((uchar *)a,ut)>0 && utf2unicode((uchar *)b,ut2)>0 )
   {
     return  MoveFileW((WCHAR *)ut,(WCHAR *)ut2);
   }
   return  MoveFile(a,b);
}
///////////////////////////////
int CreateDirectory_UTF(char *a,SECURITY_ATTRIBUTES *sa)
{
   ushort ut[512];
   if(utf2unicode((uchar *)a,ut)>0)
   {
     return  CreateDirectoryW((WCHAR *)ut,sa);
   }
   return CreateDirectory(a,sa);

}
/////////////////////////////////////
int RemoveDirectory_UTF(char *a)
{
   ushort ut[512];
   if(utf2unicode((uchar *)a,ut)>0)
   {
     return  RemoveDirectoryW((WCHAR *)ut);
   }
   return RemoveDirectory(a);
}
//////////////////////////////////
int DeleteFile_UTF(char *a)
{
   ushort ut[512];
   if(utf2unicode((uchar *)a,ut)>0)
   {
     return  DeleteFileW((WCHAR *)ut);
   }
   return DeleteFile(a);
}
//////////////////////////////////
int GetFileAttributes_UTF(char *a)
{
   ushort ut[512];
   if(utf2unicode((uchar *)a,ut)>0)
   {
     return  GetFileAttributesW((WCHAR *)ut);
   }
   return GetFileAttributes(a);
}
/////////////////////////////
int SetFileAttributes_UTF(char *a,int i)
{
   ushort ut[512];
   if(utf2unicode((uchar *)a,ut)>0)
   {
     return  SetFileAttributesW((WCHAR *)ut,i);
   }
   return SetFileAttributes(a,i);
}
uchar *   unicode2utf(uchar *utf8_buf,ushort *ucs4_buf, int l)
{
  uchar *e=utf8_buf+l-1;
  uint a;
  do{
     if(utf8_buf>=e)
     {
       *utf8_buf=0;
       break;
     }
     a = *ucs4_buf++ ;
#define UNI_SUR_HIGH_START  0xD800
#define UNI_SUR_HIGH_END    0xDBFF
#define UNI_SUR_LOW_START   0xDC00
#define UNI_SUR_LOW_END     0xDFFF

     if(a>=UNI_SUR_HIGH_START && a<=UNI_SUR_HIGH_END)
     {
       a=((a&0x3FF)<<10)|(*ucs4_buf++&0x3FF);
     }
     if ( (a) <= 0x7F)    // ASCII chars no conversion needed
     {
      *utf8_buf++ = a;
     }
     else if ( a <= 0x07FF ) // In the 2 byte utf-8 range
     {
        *utf8_buf++ = (unsigned char) (0xC0 + (a>>6));
        *utf8_buf++ = (unsigned char) (0x80 + (a&0x3F));
     }
     else if ( a <= 0xFFFF ) /* In the 3 byte utf-8 range. The
                                    values 0x0000FFFE, 0x0000FFFF
                                    and 0x0000D800 - 0x0000DFFF do
                                    not occur in UCS-4 */
     {
          *utf8_buf++= (unsigned char) (0xE0 +
                      (a>>12));
          *utf8_buf++= (unsigned char) (0x80 +
                      ((a>>6)&0x3F));
          *utf8_buf++= (unsigned char) (0x80 + (a&0x3F));
     }
     else // if ( a <= 0x1FFFFF ) //In the 4 byte utf-8 range
     {
          *utf8_buf++= (unsigned char) (0xF0 +
                         (a>>18));
          *utf8_buf++= (unsigned char) (0x80 +
                         ((a>>12)&0x3F));
          *utf8_buf++= (unsigned char) (0x80 +
                         ((a>>6)&0x3F));
          *utf8_buf++= (unsigned char) (0x80 + (a&0x3F));

     }
/*
     else if ( a <= 0x03FFFFFF )//In the 5 byte utf-8 range
     {
      *utf8_buf++= (unsigned char) (0xF8 +
                   (a>>24));
      *utf8_buf++= (unsigned char) (0x80 +
                   ((a>>18)&0x3F));
      *utf8_buf++= (unsigned char) (0x80 +
                   ((a>>12)&0x3F));
      *utf8_buf++= (unsigned char) (0x80 +
                   ((a>>6)&0x3F));
      *utf8_buf++= (unsigned char) (0x80 +
                     (a&0x3F));
     }
     else  if ( a <= 0x7FFFFFFF )//In the 6 byte utf-8 range
     {
       *utf8_buf++= (unsigned char)
                    (0xF8 +(a>>30));
       *utf8_buf++= (unsigned char) (0x80 +
                    ((a>>24)&0x3F));
       *utf8_buf++= (unsigned char) (0x80 +
                    ((a>>18)&0x3F));
       *utf8_buf++= (unsigned char) (0x80 +
                    ((a>>12)&0x3F));
       *utf8_buf++= (unsigned char) (0x80 +
                    ((a>>6)&0x3F));
       *utf8_buf++= (unsigned char) (0x80 +
                      (a&0x3F));
     }
*/
  }while( a );

//  debug("unicode2utf %u: %s",l,e-l+1);

  return utf8_buf;
};

HANDLE FindFirstFile_UTF(char  *   a, LPWIN32_FIND_DATA lpFindFileData)
{
 union{
   ushort ut[512];
   uchar uc[2048];
 };
 uchar dbg[256];
   HANDLE r;
   int err;
   if(utf2unicode((uchar *)a,ut)>=0)
   {
     r=FindFirstFileW((WCHAR *)ut,(LPWIN32_FIND_DATAW  ) lpFindFileData);
     err=GetLastError();
     if(r!=INVALID_HANDLE_VALUE)
     {
       unicode2utf(uc,(ushort *)lpFindFileData->cFileName,2048);
       strncpy(lpFindFileData->cFileName,(char *)uc,sizeof(lpFindFileData->cFileName));
       unicode2utf(uc,(ushort *)(lpFindFileData->cFileName + MAX_PATH*2),14);
       strncpy(lpFindFileData->cAlternateFileName,(char *)uc,sizeof(lpFindFileData->cAlternateFileName));
     }
     else r=FindFirstFile(a,lpFindFileData);
     return  r;
   }
   return FindFirstFile(a,lpFindFileData);
};

int FindNextFile_UTF(HANDLE a, LPWIN32_FIND_DATA lpFindFileData)
{
   int r;
   uchar uc[2048];

     r=FindNextFileW(a,(LPWIN32_FIND_DATAW) lpFindFileData);
     if(r)
     {
       unicode2utf(uc,(ushort *)lpFindFileData->cFileName,2048);
       strncpy(lpFindFileData->cFileName,(char *)uc,sizeof(lpFindFileData->cFileName));
       unicode2utf(uc,(ushort *)(lpFindFileData->cFileName + MAX_PATH*2),14);
       strncpy(lpFindFileData->cAlternateFileName,(char *)uc,sizeof(lpFindFileData->cAlternateFileName));
     }
     return  r;
};


#undef   _lopen
#undef   _lcreat2
#undef   MoveFile
#undef   CreateDirectory
#undef   RemoveDirectory
#undef   DeleteFile
#undef   GetFileAttributes
#undef   SetFileAttributes
#undef   FindNextFile
#undef   FindFirstFile



#define   _lopen               lopen_UTF
#define   _lcreat2(a,b)       lcreat2_UTF(a)
#define   MoveFile            MoveFile_UTF
#define   CreateDirectory     CreateDirectory_UTF
#define   RemoveDirectory     RemoveDirectory_UTF
#define   DeleteFile          DeleteFile_UTF
#define   GetFileAttributes   GetFileAttributes_UTF
#define   SetFileAttributes   SetFileAttributes_UTF
#define   FindNextFile        FindNextFile_UTF
#define   FindFirstFile       FindFirstFile_UTF


#endif

inline char* LL2STR(long long x,char *t){return OutLL( ((long*)&x)[0],((long*)&x)[1],t); };

int FTPPortMtx;
/*
struct FTPSecCon
{
 int ss2;
 int sec_state=0;
 OpenSSLConnection m_con,d_con;
 tfSnd oSnd;
 tfRcv oRcv;
};

int Req::ChkFTPSec(FTPSecCon *th2)
{
       xchg(s,th2->s2);
       if(th2->sec_state&2)
       {
           th2->oSnd=Snd;
           th2->oRcv=Rcv;
           if(th2->sec_state&4)
           {
               adv=&d_con;
               Snd=(tfSnd) &TLSSend;
               Rcv=(tfRcv) &TLSRecv;
           }
           else if(!TLSBegin(&th2->d_con))
           {
               adv=&th2->m_con;
               Snd=th2->oSnd;
               Rcv=th2->oRcv;

               SendConstCMD( "421 security connection failed\r\n");

               xchg(s,th2->s2);

               shutdown(th2->s2,2);
               closesocket(th2->s2);
               th2->s2=0;

             //  goto  lbEndTransfer;
               return 0;
           }
           sec_state|=4;
       }
  return 1;
};

*/

int IsUnlogCmd(ulong cmd)
{
  switch(cmd)
  {
    case 0x74636361:    //acct
    case 0x72657375:    //user
    case 0x73736170 :   //pass
    case 0x74616566 :   //feat
    case 0x68747561 x4CHAR("auth"):
    case 0x746F7270 x4CHAR("prot"):
    case 0x7A736270 x4CHAR("pbsz"):
      return 1;
  }
  return 0;
}

//---------
int Req::FTPReq()
{
  int status;
  HANDLE  hdl;
  int  i,l,w,r,x,ww;
  int cmd=0,oldcmd;
  char *t,*bb,*fn,*p;
  char *rcode;
  int  StreamType,rcodeLen;
  union{
    ulong VResumePos[2];
    long long HResumePos;
  };
  LPath pth;
  LimitBase *lip,*lnet,*lipo,*lneto;
  int sec_state=0;
  OpenSSLConnection m_con,d_con;
  tfSnd oSnd;
  tfRcv oRcv;

  if(  ( FndLimit(3,&lip,&lnet,&sa_c)+
         FndLimit(4,&lipo,&lneto,&sa_c )
       ) == 2
  )
  {
    ex2a:
    Send("421 limit overflow\r\n",sizeof("421 limit overflow\r\n")-1);
    return 1;
  }

  if(s_flgs[2]&FL2_NOMFTP )
  {
    ++no_close_req;
    for(int i=0;i<max_tsk;++i)
    {
      if( ((u_long)(rreq[i]))>1 && rreq[i] !=this &&
        rreq[i]->sa_c.sin_addr. S_ADDR == sa_c.sin_addr. S_ADDR
      )
      {
        dec_no_close_req();
        goto ex2a;
      }
    }
    dec_no_close_req();
  }
  #define SrcDirLen pth.SrcDirLen
  #define cpath    pth.cpath
  #define SrcPath  pth.SrcPath
  #define puser    pth.puser

  union {
    sockaddr prt;
    sockaddr_in prti;
    sockaddr_in6 prti6;
  };
  int s2=0;
  int s3=0;
  union{
    sockaddr_in sa;
    sockaddr_in6 sa6;
    char bfr[520];
    WIN32_FIND_DATA fnds;
    #ifndef SYSUNIX
    WIN32_FIND_DATAW fndsw;
    #endif
  };
  union{
    SYSTEMTIME tm;
    sockaddr_in prti1;
    sockaddr prt1;
  };
  FILETIME ftm;
  #ifdef SYSUNIX
  struct passwd *pwd,*upwd;
  struct group  *grp;
  int gid,uid=-1;
  char *uname;
  uname=getenv("LC_ALL");
  if( (!uname) || ! stristr(uname,".UTF-8") )
  {
    setlocale(LC_ALL,"en_US.UTF-8");
  }
  #else
  int isFindUTF;
  #endif
  prti.sin_port=0;
  pass_port=0;
  SrcPath=0;
  puser=0;
  status=0;
  prti.sin_port=0;
  prti.sin_family=AF_INET;
  HResumePos=0;
  StreamType='I';
  bb=new char[0x9800];
  #define BBSIZE 0x8000
  timout=FTPTimeout;
  goto lbNop;
  do{
    lbLp:
    if(rcodeLen)
    {
      if(Send(rcode,rcodeLen)<0)
      { dbg("Can\'t send.."); goto ex1;};
      if(s_flg&FL_FULLLOG)AddToLog(rcode,s,&sa_c46 ,FmtShrt);
    }
    #undef SendCMD
    #undef SendConstCMD
    #define SendCMD(xxb,xxl) rcode=xxb; rcodeLen=xxl;
    #define SendConstCMD(b) rcode=b; rcodeLen=sizeof(b)-1;
    #define SendPrint(xxb...) rcodeLen=sprintf(rcode=bb,xxb);
    if((oldcmd=cmd)==0x74697571 x4CHAR("quit"))break;
    if((cmd=RGetCMD(bfr))<0) goto ex1;
    if((!(status&1)) && //cmd!=0x74636361 && cmd!=0x72657375 && cmd!=0x73736170 && cmd!=0x74616566 && cmd!= 0x68747561 x4CHAR("auth")
      !IsUnlogCmd(cmd)
    ){SendConstCMD("530 not logged in\r\n");}
    else  switch(cmd)
    {
      case 0x68747561 x4CHAR("auth"):
        if((s_flgs[2]&FL2_FTPTLS)
          #ifndef TLSWODLL
          && PSecAccept
          #endif
        )
        {
          Send("234 Init TLS\r\n",sizeof("234 Init TLS\r\n")-1 );
          if(s_flg&FL_FULLLOG)AddToLog("234 Init TLS\r\n",s,&sa_c46,FmtShrt);
          if(TLSBegin(&m_con))
          {
            sec_state|=3;
            rcodeLen=0;
          }
          else SendConstCMD( "334 TLS error\r\n");
        }
        else SendConstCMD( "334 unsupported\r\n");
        break;
      case  0x746F7270 x4CHAR("prot"):
        sec_state|=2;
      case  0x7A736270 x4CHAR("pbsz"):
        SendConstCMD("200 Ok\r\n");
        break;
      case 0x20636363  x4CHAR("ccc ") :
        if(sec_state&1)
        {
          Send("200 Ok\r\n",sizeof("200 Ok\r\n") -1 );
          SecClose(&m_con);
          Snd=(tfSnd) &JustSnd;
          Rcv=(tfRcv) &JustRcv;
          sec_state=0;
          rcodeLen=0;
        }
        else SendConstCMD("200 Ok\r\n" );
        break;
      case 0x74636361 x4CHAR("acct"):
      case 0x72657375 x4CHAR("user"):
        status&=~1;
        strncpy(cpath,bfr+5,32);
        if( (! ( s_flgs[3] & FL3_FTP_ALWPASS ) ) && (puser=FindUser(cpath,UserFTP,0,this)))
        {
          if( (t=puser->pasw()) && !*t)
          {
            dir=puser->dir(t);
            goto LoginOk;
          }
          puser=0;
        }
        SendConstCMD( "331 Password request\r\n");
        break;
      case 0x73736170 x4CHAR("pass")://1:
        if(!(puser=FindUser(cpath,UserFTP,bfr+5,this)))
        {
          ex3:
          SendConstCMD("530 not logged in\r\n");
          cmd=0x74697571;
          break;
        }
        LoginOk:
        status|=1;
        if(first_pass_port)
        {
          pass_port=first_pass_port;
          ++no_close_req;
          MyLock(FTPPortMtx);
          for(i=0;i<max_tsk;i++)
          { if( ((u_long)rreq[i])>1 &&  rreq[i]->pass_port == pass_port && (rreq[i]->fl& F_SERV_MASK)== F_SERV_FTP) pass_port++;
          }
          MyUnlock(FTPPortMtx);
          dec_no_close_req();
        }
        if(fl&F_PRX)
        {
          status|=4;
          if((w=CallUp(puser))<0)goto ex3;
          #if ! (defined(SYSUNIX) || defined(SELECT1) )
          while(RESelect(FTPTimeout,0,2,s,w) >0 )
          {
            if(RESelect(0,0,1,w))
            {
          #else
          while( (l=RESelect2(timout,0,s,w)) >0 )
          {
            if(l==w)
            {
              #endif
              if( (l=XRecv(w,bb,BBSIZE,0,FTPTimeout))<=0)
              {
                if(!(status&2))goto ex3;
                break;
              }
              cmd=atoui(bb);
              if(!(status&2))
              {
                if(cmd!=230)break;
                status|=2;
              }
              if(cmd==227)
              { // PASV
                if(!(t=strchr(bb,'(')) ) goto lbPRX3;
                ++t;
                prti1.sin_addr.s_addr=ConvertIP(t); ++t;
                prti1.sin_port =atouis(t); ++t;
                prti1.sin_port |=atoui(t)<<8;
                if(status&0x10)
                {status&=~0x10;
                  l=sprintf(bb,"200\r\n");
                }
                else
                {
                  goto lbPASV;
                  lbPRX3: rcodeLen=sprintf(bb,"421 Err\r\n");
                  lbPRX1:
                  l=rcodeLen;
                }
              }
              Send(bb,l);
              if(cmd==221)break;
            }
            else
            {
              //lbRcv:
              if((l=Recv(bb,BBSIZE))<=0) break;
              if( (r=DWORD_PTR(*bb)|0x20202020) == 0x74726F70 x4CHAR("port")  )
              {sprintf(bfr,"%.64s",bb);
                //bb[l]=0;
                goto lbPORT;
                lbPRX4:
                if(rcode!=bb)
                {Send(bb,rcodeLen);
                  continue;
                }
                l=sprintf(bb,"PASV\r\n");
                status|=0x10;
              }
              //      if( send(w,bb,l,0)<=0) break;
              if( BSend(w,bb,l)<=0) break;
              #if 1
              if(r==0x65707061 x4CHAR("appe")
                ||r==0x72746572 x4CHAR("retr")
                ||r==0x7473696C x4CHAR("list")
                ||r==0x74736C6E x4CHAR("nlst")
                ||r==0x726F7473 x4CHAR("stor")
              )
              {
                l=0;
                if(RESelect(0,100000,1,w))
                { l=XRecv(w,bb,BBSIZE,0,FTPTimeout);
                  if( (atoui(bb)/100) !=1)  //150
                  {Send(bb,l);
                    continue;
                  }
                }
                #if 0
                prti1.sin_family=AF_INET;
                if((x=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) > 0
                  )
                  {
                  //         ioctlsocket(x,FIONBIO,(ulong *)&one);
                  if(connect(x,&prt1,sizeof(prt1))>=0)
                  {
                  #else
                  //*
                  sprintf(bb,"%u.%u.%u.%u",
                          #ifndef SYSUNIX
                          prti.sin_addr.S_un.S_un_b.s_b1,prti.sin_addr.S_un.S_un_b.s_b2,
                          prti.sin_addr.S_un.S_un_b.s_b3,prti.sin_addr.S_un.S_un_b.s_b4
                          #else
                          prti.sin_addr.s_addr&0xFF, BYTE_PTR(prti.sin_addr.s_addr,1),
                          BYTE_PTR(prti.sin_addr.s_addr,2), BYTE_PTR(prti.sin_addr.s_addr,3)
                  #endif
                  );

                  if((x=call_socket2(bb,htons(prti1.sin_port)))>=0)
                  {
                    #endif
                    if( l || (l=XRecv(w,bb,BBSIZE,0,FTPTimeout))>0)
                    {
                      Send(bb,l);
                      //          ioctlsocket(x,FIONBIO,(ulong *)&NullLong);
                      if( ((l=atoui(bb))/100) == 1)   //==150)
                      {
                        goto lbCONECT;
                        lbPRX2:
                        xchg(s,s2); p2p(x,bb); xchg(s,s2);
                        //shutdown(s2,2); closesocket(s2);
                        CloseSocket(s2);
                      }
                    }
                    //shutdown(x,2); closesocket(x);
                    CloseSocket(x);
                  }

              }
              #endif
            }
          } //while(1);
#ifdef FOR_EDITOR_NEVER_DEF
            }
          }
#endif
          CloseSocket(w);
          sprintf(bb,"FTP: proxy\tfor %s %d %d\r\n",puser->name,Tin,Tout);
          AddToLog(bb,s,&sa_c46);

          goto ex1;
        } // fl&F_PRX

        if(0x72657375 x4CHAR("user") == cmd)
        {
          SendConstCMD("232 Logged in successfully\r\n");
        }
        else
        {
          SendConstCMD("230 Logged in successfully\r\n");
        }
        DWORD_PTR(cpath[pth.BSrcDirLen=sprintf(cpath,"%.254s",SrcPath=dir)//+1
        ])=0;
        sprintf(bb,"FTP: login in %.20s;%.127s\r\n",puser->name,((FL1_DELPAS&s_flgs[1]) && puser->pasw()[0])?"":bfr+5);
        AddToLog(bb,s,&sa_c46);
        #ifdef SYSUNIX
        if( (upwd=getpwnam(uname=puser->name)) || (upwd=getpwnam(uname="ftp")) )
        {// gid=getgid(); uid=getuid();
          gid=upwd->pw_gid;
          uid=upwd->pw_uid;
          //  debug("gid %u uid %u",gid,uid);
          //     setegid(pwd->pw_gid);
          //     seteuid(pwd->pw_uid);
          umask(6);
        }
        #define CheckAccess(f,t)   if(uid>0) if( IsAssess(f,uid,gid,t,uname)<=0 ) goto lerr550;
        #else
        #define CheckAccess(f,t)
        #endif

        break;
        case 0x746E6D73 x4CHAR("smnt")://5:
        case 0x6F6C6C61 x4CHAR("allo")://17:
        case 0x74617473 x4CHAR("stat")://30:
        case 0x706C6568 x4CHAR("help")://31:
        case 0x756F7473 x4CHAR("stou")://15:
          SendConstCMD( "502 \r\n"); break;
        case 0x76737065 x4CHAR("epsv")  :
          status|=0x28;
          if(bfr[5]=='2')
          {

            if(!s3)
            {
              memset(&sa6,0,sizeof(sa6));
              sa.sin_family=AF_INET6;
              sa6.sin6_port=htons(pass_port);
              goto lbCon;
            }


          }
        case 0x76736170 x4CHAR("pasv")://9:
          lbPASV:
          status|=8;

          if(!s3)
          {

            sa.sin_family=AF_INET;
            sa.sin_port=htons(pass_port);
            sa.sin_addr.s_addr=INADDR_ANY;
            lbCon:
            s3=socket(sa.sin_family,SOCK_STREAM,IPPROTO_TCP);

            bind(s3,(sockaddr *)&sa, (sa.sin_family==AF_INET6)? sizeof(sa6) : sizeof(sa));
            listen(s3,1);
            //  ioctlsocket(s3,FIONBIO, (ulong *) &one);
          }
          #ifdef SYSUNIX
          fcntl(s3, F_SETFD, fcntl(s3, F_GETFD) | FD_CLOEXEC);
          #endif
          getsockname(s,(sockaddr *)&prti, &(i=sizeof(sa)) );
          getsockname(s3,(sockaddr *)&sa, &(i=(sa.sin_family==AF_INET6)? sizeof(sa6) : sizeof(sa)) );

          i=(status&0x20)?
          sprintf(bb,"229 Entering passive mode (|||%u|)\r\n",htons((sa.sin_family==AF_INET6)?sa6.sin6_port:sa.sin_port) )
          :sprintf(bb,"227 Entering passive mode (%u,%u,%u,%u,%u,%u)\r\n",
                   #ifndef SYSUNIX
                   prti.sin_addr.S_un.S_un_b.s_b1,prti.sin_addr.S_un.S_un_b.s_b2,
                   prti.sin_addr.S_un.S_un_b.s_b3,prti.sin_addr.S_un.S_un_b.s_b4,
                   #else
                   prti.sin_addr.s_addr&0xFF, BYTE_PTR(prti.sin_addr.s_addr,1),
                   BYTE_PTR(prti.sin_addr.s_addr,2), BYTE_PTR(prti.sin_addr.s_addr,3),
                   #endif
                   sa.sin_port&0xFF,sa.sin_port>>8
          );
          SendCMD(bb,i);
          if(status&4)goto lbPRX1;
          break;
        case 0x65707974 x4CHAR("type")://10:
          StreamType=bfr[5];
          if(0){
        case 0x65646F6D x4CHAR("mode")://12:
          if(bfr[5]!='S')goto er502;
          }
        case 0x75727473 x4CHAR("stru")://11:
          SendConstCMD( "200 Ok\r\n");
          break;

        case 0x6D74646D x4CHAR("mdtm") ://29:
          //     if( (l=atouis(t=bfr+5)) > 19700000000000 )
          if( (HResumePos=atoll(bfr+5)) > 19691231235959ll && bfr[5+14] == ' ' )
          {
            l=pth.MkFName(bb,bfr+5+15);
            if(
              ((puser->state&UserFTP)!=UserFTP && ! (pth.mode & V_NOFTP) )
              || pth.mode == V_RWLIM
            )goto lerr550;
            if( (!CheckBadName(bb)) )goto lerr501;
            CheckAccess(bb,2);

            #ifdef SYSUNIX
            tm.ttm.tm_sec=  VResumePos[0]%100;
            tm.ttm.tm_min= (VResumePos[0]/100)%100;
            tm.ttm.tm_hour=(VResumePos[0]/10000)%100;
            tm.ttm.tm_mday=(VResumePos[0]/1000000)%100;
            tm.ttm.tm_mon= ((VResumePos[0]/100000000)%100) -1;

            bfr[5+4]=0;
            tm.ttm.tm_year=atoui(bfr+5)-1900;
            VResumePos[0]=VResumePos[1]=mktime(&tm.ttm);
            if(!utime(bb,(struct utimbuf *) VResumePos ))
            {
              //213 Modify=20020717210715;
              goto lbOkModify;
            };
            goto lerr501;
            #else

            tm.wSecond=  VResumePos[0]%100;
            tm.wMinute= (VResumePos[0]/100)%100;
            tm.wHour   =(VResumePos[0]/10000)%100;
            tm.wDay    =(VResumePos[0]/1000000)%100;
            tm.wMonth = ((VResumePos[0]/100000000)%100);

            bfr[5+4]=0;
            tm.wYear=atoui(bfr+5) ;

            i=_lopen(bb,1) ;
            if(i==(int)INVALID_HANDLE_VALUE)  goto lerr501;
            SystemTimeToFileTime(&tm,&ftm);
            l= SetFileTime(
              (HANDLE) i, //         hFile,
                           0,//const FILETIME *lpCreationTime,
                           0,//const FILETIME *lpLastAccessTime,
                           &ftm//const FILETIME *lpLastWriteTime
            );
            _lclose(i);
            if(l)  goto lbOkModify;
            goto lerr501;
            #endif

          }
          else
          {
            goto lbSize;
          }
          break;
          case 0x74736572 x4CHAR("rest")://18:
            HResumePos=atoll(bfr+5);
            if(0){
          case 0x72666E72 x4CHAR("rnfr")://19:
            if((puser->state&UserFTP)!=UserFTP){cmd=0x6F746E72 x4CHAR("rnto"); goto lerr550;}
          case 0x657A6973 x4CHAR("size")://28:

            lbSize:
            l=pth.MkFName(bb,bfr+5);
            if( //(l<=SrcDirLen) ||
              (!CheckBadName(bb)) )goto lerr501;
            if(cmd==0x657A6973 x4CHAR("size") || cmd == 0x6D74646D x4CHAR("mdtm"))
            {if( ( hdl=FindFirstFile(bb,&fnds) )==INVALID_HANDLE_VALUE )goto lerr501;
              FindClose(hdl);
              if( cmd == 0x6D74646D x4CHAR("mdtm") )
              {
                FileTimeToSystemTime(&fnds.ftLastWriteTime,&tm);
                lbOkModify:
                printSendCMD(bb+256,"213 %4.4u%2.2u%2.2u%2.2u%2.2u%2.2u\r\n",
                             tm.wYear, tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond );
              }
              else
              {
                OutLL(fnds.nFileSizeLow,fnds.nFileSizeHigh,bb+290);
                printSendCMD(bb+256,"213 %s\r\n",bb+290);  //(ulong) fnds.nFileSizeLow);
              }
              break;
            }
            CheckAccess(bb,2);
            }
            SendConstCMD( "350 Ok\r\n");
            break;
          case 0x6F746E72 x4CHAR("rnto")://20: //
            if(oldcmd!=0x72666E72 x4CHAR("rnfr"))
            {SendConstCMD( "503 Bad sequence\r\n"); }
            else
            {if(pth.mode & V_LIMITED)goto lerr550;
              i=pth.MkFName(t=bb+l+1,bfr+5);
              if(pth.mode == V_RWLIM)goto lerr550;
              debug("rename to %s",t);
              if( //(i<=SrcDirLen) ||
                (!CheckBadName(t)) )goto lerr501;
              CheckAccess(t,2);
              #ifndef SYSUNIX
              if( (puser->state&UserNOCGI) && IsCGI(t,1) )goto lerr550;
              #endif
              MoveFile(bb,t);
              SendConstCMD( "250 Ok\r\n");
            }
            break;
          case 0x6E696572 x4CHAR("rein")://7:
          case 0x74697571 x4CHAR("quit")://6:
          case 0x726F6261 x4CHAR("abor")://21:
            SendConstCMD("221 Bye\r\n");
            cmd=0x74697571;
            break;
          case 0x656C6564 x4CHAR("dele")://22:
            i=pth.MkFName(bb,bfr+5);
            if(
              ((puser->state&UserFTP)!=UserFTP && ! (pth.mode & V_NOFTP) )
              || pth.mode == V_RWLIM
            )goto lerr550;
            if( //(i<=SrcDirLen) ||
              (!CheckBadName(bb)) )goto lerr501;
            CheckAccess(bb,2);
            DeleteFile(bb);
            if(0)
            {
          case 0x64776378 x4CHAR("xcwd")://3:
              memcpy(bfr,bfr+1,256);
              if(0){
          case 0x70756463 x4CHAR("cdup")://4:
                DWORD_PTR(bfr[4])=0x2E2E;
              }
          case 0x20647763 x4CHAR("cwd ")://3:
              pth.MkFName(bb,bfr+4);
              //   DBG_MSG("%s %d atr:%X >%X",CheckBadName(bb),GetFileAttributes(bb),FILE_ATTRIBUTE_DIRECTORY);
              if((!CheckBadName(bb)) ||
                ((GetFileAttributes(bb)&(FILE_ATTRIBUTE_DIRECTORY|0x80000000))!=FILE_ATTRIBUTE_DIRECTORY ) ||
                (i=pth.MkFName(bb,bfr+4,0))>254
              )goto lerr501;
              strcpy(cpath,bb);
              DWORD_PTR(cpath[i])=0;
              if(0)
              {
          case 0x20646D72 x4CHAR("rmd ")://23:
                pth.MkFName(bb,bfr+4);
                if(
                  ((puser->state&UserFTP)!=UserFTP)
                  && !(pth.mode & V_NOFTP)
                )goto lerr550;

                if( //(i<=SrcDirLen) ||
                  (!CheckBadName(bb)))goto lerr501;
                //  debug(bb);
                CheckAccess(bb,2);
                RemoveDirectory(bb);
                } // dele
                }
                lb250:
                SendConstCMD("250 Ok\r\n");
                break;
          case 0x20646B6D x4CHAR("mkd ")://24:
                pth.MkFName(bb,bfr+4);
                if( //(i<=SrcDirLen) ||
                  (!CheckBadName(bb)))goto lerr501;
                if(
                  ((puser->state&UserFTP) != UserFTP || pth.mode == V_RWLIM )&&
                  (!(ftp_upload && stristr(bb+SrcDirLen,ftp_upload)))
                  && !(pth.mode & V_WRITE)
                )goto lerr550;
                //i=
                CheckAccess(bb,2);
                CreateDirectory(bb,&secat);
                #ifdef SYSUNIX
                if(uid>0)chown(bb,uid,gid);
                #endif
          case 0x20647770 x4CHAR("pwd ")://25:
          case 0x2D647770 x4CHAR("pwd-")://34:
          case 0x2A647770 x4CHAR("pwd*")://34:
          case 0x64777078 x4CHAR("xpwd")://35:
                i=sprintf(t=bb+512,"257 \"/%.255s\" Ok\r\n",((cmd==0x20646B6D)?bb:cpath)+pth.BSrcDirLen+1);
                //DBG_MSG("%s %u %s",((cmd==0x20646B6D)?bb:cpath),SrcDirLen+1,((cmd==0x20646B6D)?bb:cpath)+SrcDirLen+1);
                #ifndef SYSUNIX
                for(;*t;++t)if(*t=='\\')*t='/';
                #endif
                SendCMD(bb+512,i);
                break;
          case 0x65707061 x4CHAR("appe")://16:
                if( !(puser->state&UserFTPW)) goto lerr550;
          case 0x72746572 x4CHAR("retr")://13:
                if(!(puser->state&UserFTPR))
                {
                  lerr550:
                  SendConstCMD( "550 Access denied\r\n");
                  break;
                }
          case 0x7473696C x4CHAR("list")://26:
          case 0x74736C6E x4CHAR("nlst")://27:
                if((p=strpbrk(bfr,"?*")))
                {
                  if(strpbrk(p,"/\\"))goto lerr501;
                  if(!(p=strrchr(bfr,'/')))p=bfr+4;
                  *p=0;
                }
                if(0){
          case 0x726F7473 x4CHAR("stor")://14:
                    if(
                      (lneto  && lneto->CheckLimit(net_limit[4],ltime[4]) )
                      || (lipo && lipo->CheckLimit(ip_limit[4],ltime[4])  )
                    )
                    {SendConstCMD( "501 Limit overflow\r\n");
                      break;
                    }
                }
                t="";
                if(bfr[4])
                {t=bfr+5;
                  if(*t=='-')if( !(t=strrchr(t,' ')))t=(cmd==0x7473696C)?(char *)"":bfr+5; else ++t;
                }
                l=pth.MkFName(bb,t);
                t=bb;
                if( ! (fn=CheckBadName(bb)) )
                {
                  lerr501:
                  SendConstCMD( "501 bad pathname\r\n");
                  break;
                }
                #ifndef SYSUNIX
                if( (puser->state&UserNOCGI) && IsCGI(bb,1) )goto lerr550;
                #else
                //*
                if(cmd==0x7473696C x4CHAR("list") ||
                  cmd==0x74736C6E x4CHAR("nlst") ) //CheckAccess(bb,1);
                if(uid>0) if( IsAssess(bb,uid,gid,1,uname)==0 ) goto lerr550;
                //    */
                #endif
                i=ftp_upload && stristr(bb+SrcDirLen //-1
                ,ftp_upload);
                x=
                #ifndef SYSUNIX
                (int)(GetFileAttributes(bb));
                #else
                stat(bb,(struct stat *) (bb+1024) );
                if(x>=0)x=((struct stat *) (bb+1024))->st_mode ;
                #endif
                if(cmd==0x72746572 x4CHAR("retr"))
                {if(x==-1)goto lerr501;
                  if( (pth.mode & V_LIMITED)?!(pth.mode & V_READ): (i && (UserFTP & ~(puser->state))) )goto lerr550;
                  if(x&FILE_ATTRIBUTE_DIRECTORY)goto lerr501;
                  CheckAccess(bb,4);
                }
                sprintf(inf,"%.94s",bb);
                if( cmd==0x726F7473 x4CHAR("stor") || cmd== 0x65707061 x4CHAR("appe") )
                { if( ((pth.mode & V_LIMITED)?!(pth.mode & V_WRITE):!(puser->state&UserFTPW)) &&  ! i  ) goto lerr550;
                  if( x!=-1  &&
                    (
                      (UserFTP & ~(puser->state)) ||
                      #ifdef SYSUNIX
                      !(((struct stat *) (bb+1024))->st_mode  &0444 )
                      #else
                      (x&FILE_ATTRIBUTE_READONLY)
                      #endif
                    )
                  ) goto lerr550;
                  CheckAccess(bb,2);
                }
            lbCONECT:
                if( status&8 ) //PASIV mode
                {if((RESelect(30,0,1,s3)<=0) ||
                  ((s2=accept(s3,(sockaddr *) &sa, &(i=sizeof(sa))))<=0))goto er421;
                }
                else
                {
                  if(prti.sin_port && (prti.sin_addr. S_ADDR == /* saddr[ntsk]*/ sa_c.sin_addr. S_ADDR || !(s_flgs[1]&FL1_FTPSAME)
                  )                 )
                  {
                    if((s2=socket(prti.sin_family,SOCK_STREAM,IPPROTO_TCP)
                    ) <= 0)goto er421;
                    if(connect(s2,&prt, (sa.sin_family==AF_INET)? sizeof(prt):sizeof(prti6)   ) < 0){
                      //shutdown(s2,2);
                      //closesocket(s2);
                      CloseSocket(s2);
                      goto er421;
                    }
                    #ifdef SYSUNIX
                    fcntl(s2, F_SETFD, fcntl(s2, F_GETFD) | FD_CLOEXEC);
                    #endif
                  }else
                  {
                    er421:
                    SendConstCMD( "421 Err\r\n");
                    if(status&4)goto lbPRX3;
                    break;
                  }
                }
                if(status&4)goto lbPRX2;
                if( ( Send("150 Ok\r\n",sizeof("150 Ok\r\n")-1) ) < 0)
                { dbg("Can\'t send..");goto ex1;  };
                if(s_flg&FL_FULLLOG)AddToLog("150 Ok\r\n",s,&sa_c46,FmtShrt);

                xchg(s,s2);
                timout=FTPTimeout;

                if(sec_state&2)
                {
                  //  debug("FTP Setup TLS data connection\r\n");
                  oSnd=Snd;
                  oRcv=Rcv;
                  if(!TLSBegin(&d_con))
                  {
                    Adv=&m_con;
                    Snd=oSnd;
                    Rcv=oRcv;

                    SendConstCMD( "421 security connection failed\r\n");

                    xchg(s,s2);
                    CloseSocket(s2);
                    s2=0;
                    sec_state&=~4;

                    goto  exLp5;
                  }
                  sec_state|=4;
                }
                //        else
                //        {
                //            debug("FTP without TLS\r\n");
                //
                //        }
                switch(cmd)
                {
                  case 0x72746572 x4CHAR("retr")://13:
                    if((lnet &&  lnet->CheckLimit(net_limit[3],ltime[3]) )
                      || ( lip && lip->CheckLimit(ip_limit[3],ltime[3]) )
                    )
                    {
                      SendConstCMD( "501 Limit overflow\r\n");
                      break;
                    }

                    if( (i=_lopen(bb,0))>0)
                    {
                      t=bb+1024;
                      x=0;
                      l=0;
                      if(HResumePos)_llseek64(i,HResumePos,0);
                      HResumePos=0;
                      do{
                        l=_hread(i,t,BBSIZE);
                        if( (!l) || (ww=Send(t,l)) <l) break;
                        x+=l;
                      }while(l==BBSIZE);

                      sprintf(t,"FTP: download %.255s\tfor %s %d\r\n",bb,puser->name,x);
                      AddToLog(t,s2,0);
                      if(lip && lnet)
                      { x=(x+1023)>>10;
                        lip->cnt+=x;
                        lnet->cnt+=x;
                        ipcnts[3].d[0].cnt+=x;
                      }
                      _lclose(i);
                      if(x<0 || (l && !x))
                      {
                        SendConstCMD("426 Err\r\n");
                        goto exLp5;
                      }
                    }
                    else
                    {
                      er450:;
                      SendConstCMD("450 Not found");
                    }
                    break;



                  case 0x726F7473 x4CHAR("stor")://14:
                    if(HResumePos)
                    {
                  case 0x65707061 x4CHAR("appe")://16:
                    if( (i=_lopen(bb,1))>0)
                    {
                      //   debug("SEEK %u %u ",VResumePos[1],VResumePos[0]);
                      if(HResumePos){_llseek64(i,HResumePos,0); HResumePos=0;}
                      else _llseek(i,0,2);
                      goto lbSTOR;
                    }else goto er450;
                    }

                    if( (i=_lcreat2(bb,0))>0)
                    {lbSTOR:
                      t=bb+1024;
                      x=0;

                      do{l= Recv(t,BBSIZE);
                        //XRecv(s2,t,BBSIZE,0,FTPTimeout);

                        //recv(s2,t,BBSIZE,0);
                        if(l<=0)
                        {
                          //         debug("TTXD %d %d s=%d s2=%d i=%d %d %s %X %X",l,BBSIZE,s,s2,i,errno,strerror(errno),bb,t);
                          break;
                        }
                        _hwrite(i,t,l);
                        x+=l;
                        Tin+=l;
                      }while(1);
                      #ifdef SYSUNIX
                      if(uid>0)fchown(i,uid,gid);
                      #endif
                      _lclose(i);
                      /*
                       *       xchg(s,s2);
                       *       if(sec_state&2)
                       *       {
                       *         adv=&m_con;
                       *         Snd=oSnd;
                       *         Rcv=oRcv;
                    }
                    */
                      sprintf(t,"FTP: upload %.255s\tfrom %s %u\r\n",bb,puser->name,x);
                      AddToLog(t,s2,0);
                      if(lipo && lneto)
                      { x=(x+1023)>>10;
                        lipo->cnt+=x;
                        lneto->cnt+=x;
                        ipcnts[4].d[0].cnt+=x;
                      }
                    }
                    else goto er450;
                    break;
                    case 0x74736C6E x4CHAR("nlst")://27:
                    case 0x7473696C x4CHAR("list")://26:
                      GetSystemTimeAsFileTime(&ftm);
                      if(GetFileAttributes(bb)&FILE_ATTRIBUTE_DIRECTORY )
                        //    DWORD_PTR(bb[l]) =
                        DWORD_PTR(*fn) =
                        #ifndef SYSUNIX
                        0x2A5C; //"\\*";
                      #else
                      0x2A2F; // "/*"
                      #endif
                      DBG_MSG("LIST: %.64s %u",bb,l);
                      w=(puser->state&UserFTPW)?'w':'-';
                      r=(puser->state&UserFTPR)?'r':'-';
                      if( stristr(bb,cgi_detect) )
                      {x='x';
                        if(puser->state&UserNOCGI) r=w='-';
                      }else x='-';
                      DBG_MSG("Find: %.64s",bb);
                      if( ( hdl=FindFirstFile(bb,&fnds) )!=INVALID_HANDLE_VALUE )
                      {
                        DBG_MSG("LIST2: %.64s %u",bb,l);
                        bb[l]=0;
                        if(s_flg&FL_FTPVDIR && !strcmp(bb,SrcPath) )
                        {host_dir *a;
                          for(a=hsdr.next;a;a=a->next)
                            if(a->h[0]=='/' && a->flg  && (7&IsInStrLst(a->d+a->flg,puser->name)) )
                            {t+=sprintf(t,"drwxrwxrwx    1 nouser   nogroup        0 Jan 01 00:00 %s\r\n",a->h+1);
                              if(t[-3]=='/'){DWORD_PTR(t[-3])=0x0A0D; --t;}
                              if( (i=t-bb)>=0x8000 )
                              {
                                //        if(send(s2,bb,i,0)<i)goto erSnd;
                                if(Send(bb,i)<i)goto erSnd;
                                //        bb[i]=0;
                                t=bb;
                                Tout+=i;
                              }
                            }
                        }
                        DBG_MSG("LIST3: %.64s %u",bb,l);

                        do{
                          //DBG_MSG("LIST3.1: %.64s %u",fnds.cFileName,l);
                          FileTimeToLocalFileTime(&fnds.ftLastWriteTime,&fnds.ftLastAccessTime);
                          //DBG_MSG("LIST3.2: %s %u",fnds.cFileName,l);
                          FileTimeToSystemTime(&fnds.ftLastAccessTime,&tm);
                          DBG_MSG("LIST4.1: %s %u",fnds.cFileName,l);
                          OutLL(fnds.nFileSizeLow,fnds.nFileSizeHigh,bb+0x87C0);
                          #ifndef SYSUNIX
                          t+=sprintf(t,
                                     "%c%c%c%c%c%c%c%c%c%c    1 nouser   nogroup %8s %3.3s %2u "
                                     ,(fnds.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)?'d':'-'
                          ,r,w,x,r,w,x,
                          (fnds.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)?'-':r,
                                     (fnds.dwFileAttributes & FILE_ATTRIBUTE_READONLY)?'-':w,
                                     (fnds.dwFileAttributes & (FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_DIRECTORY))?'x':x,
                                     bb+0x87C0//fnds.nFileSizeLow
                                     ,month[tm.wMonth-1],tm.wDay);
                          #ifdef MINGW
                          t+=        ( (ftm.dwHighDateTime-fnds.ftLastWriteTime.dwHighDateTime)<67306)?
                          sprintf(t,        "%2.2u:%2.2u %.256s\r\n",tm.wHour,tm.wMinute,( (s_flg&FL_FTWOSPACE) && (int) strchr(fnds.cFileName,' ') )?fnds.cAlternateFileName:fnds.cFileName
                          ):
                          sprintf(t,        "%4.4u %.256s\r\n",tm.wYear,( (s_flg&FL_FTWOSPACE) && (int) strchr(fnds.cFileName,' ') )?fnds.cAlternateFileName:fnds.cFileName
                          );
                          #else
                          t+=sprintf(t,
                                     ( (ftm.dwHighDateTime-fnds.ftLastWriteTime.dwHighDateTime)<67306)?
                                     "%2.2u:%2.2u%0.0c %.256s\r\n":"%0.0c%0.0c %4.4u %.256s\r\n"
                                     ,tm.wHour,tm.wMinute,tm.wYear
                                     ,( (s_flg&FL_FTWOSPACE) && (int) strchr(fnds.cFileName,' ') )?fnds.cAlternateFileName:fnds.cFileName
                          );
                          #endif
                          #else
                          pwd=getpwuid(fnds.st.st_uid);
                          grp=getgrgid(fnds.st.st_gid);
                          //DBG_MSG("LIST4.11: %X %o %o",t,pwd,grp);
                          strncpy(bb+0x8800,fnds.cFileName,512);
                          #if 1
                          //ndef _BSD_VA_LIST_
                          t+=sprintf(t,
                                     (( (ftm-fnds.ftLastWriteTime)< 15768000 )? //28512000)?
                                     //   1d r w x r w x r w x
                                     "%c%c%c%c%c%c%c%c%c%c    1 %-8.8s %-8.8s %8s %3.3s %2u %2.2u:%2.2u%0.0s %.256s\r\n"
                                     :"%c%c%c%c%c%c%c%c%c%c    1 %-8.8s %-8.8s %8s %3.3s %2u %0.0s%0.0s %4.4u %.256s\r\n")

                                     ,((fnds.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)?'d':'-'),
                                     atrib[(fnds.st.st_mode>>6)&4],atrib[(fnds.st.st_mode>>6)&2],atrib[(fnds.st.st_mode>>6)&1],
                                     atrib[(fnds.st.st_mode>>3)&4],atrib[(fnds.st.st_mode>>3)&2],atrib[(fnds.st.st_mode>>3)&1],
                                     atrib[(fnds.st.st_mode)&4],atrib[(fnds.st.st_mode)&2],atrib[(fnds.st.st_mode)&1],
                                     (pwd)?pwd->pw_name:"nouser",(grp)?grp->gr_name:"nogroup",
                                     bb+0x87C0 //(ulong)fnds.nFileSizeLow
                                     ,month[tm.wMonth-1],tm.wDay,tm.wHour,tm.wMinute
                                     ,tm.wYear,bb+0x8800); //((fnds.cFileName)==bb)?bfr+5:fnds.cFileName);

                                     #else

                                     //        "%c%c%c%c%c%c%c%c%c%c    1 %-8.8s %-8.8s %8u %3.3s %2u %2.2u:%2.2u%c %.256s\r\n"
                                     //       :"%c%c%c%c%c%c%c%c%c%c    1 %-8.8s %-8.8s %8u %3.3s %2u %c%c %4.4u %.256s\r\n")
                                     t+=sprintf(t,"%c%c%c%c%c%c%c%c%c%c   1 %-8.8s %-8.8s %8u %.256s\r\n"

                                     ,((fnds.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)?'d':'-'),
                                     atrib[(fnds.st.st_mode>>6)&4],atrib[(fnds.st.st_mode>>6)&2],atrib[(fnds.st.st_mode>>6)&1],
                                     atrib[(fnds.st.st_mode>>3)&4],atrib[(fnds.st.st_mode>>3)&2],atrib[(fnds.st.st_mode>>3)&1],
                                     atrib[(fnds.st.st_mode)&4],atrib[(fnds.st.st_mode)&2],atrib[(fnds.st.st_mode)&1],
                                     ((pwd)?pwd->pw_name:"nouser"),
                                     ((grp && grp->gr_name)?grp->gr_name:"nogroup"),
                                     bb+0x87C0//(ulong)fnds.nFileSizeLow,
                                     //    month[tm.wMonth-1],
                                     //    tm.wDay,
                                     //tm.wHour,tm.wMinute,
                                     //    tm.wYear,
                                     bb+0x8800);

                                     #endif

                                     DBG_MSG("LIST4.12: %.64s %u",fnds.cFileName,l);
                                     #endif
                                     if(StreamType=='E')
                                     {
                                       (--t)[-1]=0;
                                       DBG_MSG("LIST4.15: STREAM E");
                                     }
                                     if( (i=t-bb)>=0x8000 )
                                     {
                                       DBG_MSG("LIST4.2: %.64s %u",fnds.cFileName,l);

                                       //       if(send(s2,bb,i,0)<i)
                                       if(Send(bb,i)<i)
                                       {
                                         erSnd:
                                         dbg("Error send") ;
                                         SendConstCMD("421 Err\r\n");
                                         goto exLp5;
                                       }
                                       //       bb[i]=0;
                                       t=bb;
                                       Tout+=i;
                                     }
                                     DBG_MSG("LIST4.3: %.64s %u",fnds.cFileName,l);

                        }while(FindNextFile(hdl,&fnds));
                        FindClose(hdl);
                      };

                      DBG_MSG("LIST4: %.64s %u",bb,l);

                      if((i=t-bb)>0)
                      {
                        if(Send(bb,i)<0)goto erSnd;
                        Tout+=i;
                      }
                }
                SendConstCMD("226 Ok\r\n");
                exLp5:
                if(sec_state&2)
                {
                  if(sec_state&4)
                  {
                    SecClose(&d_con);
                    sec_state &= ~4 ;
                  }
                  Adv=&m_con;
                  Snd=oSnd;
                  Rcv=oRcv;
                }
                xchg(s,s2);
                CloseSocket(s2);
                s2=0;

                break;
            case 0x74737973 x4CHAR("syst")://29:
                SendConstCMD( "215 UNIX A N\r\n");
                break;
            case 0x74727065 x4CHAR("eprt")  :
                t=bfr+8;
                if(bfr[6]=='1')
                {
                  prti.sin_addr.s_addr=ConvertIP(t); ++t;
                  prti1.sin_port=prti.sin_port =htons(atouis(t));
                  ++t;
                  prti.sin_family=AF_INET;
                }
                else if(bfr[6]=='2')
                {
                  prti6.sin6_family=AF_INET6;
                  t=IPv6Addr((ushort *) &(prti6.sin6_addr) ,t); ++t;
                  prti1.sin_port= prti6.sin6_port =htons(atouis(t));
                }
                else goto erp;
                i=sprintf(bb,"200 port: %u\r\n",htons(prti1.sin_port));
                SendCMD(bb,i);
                status&=~8;
                break;
            case 0x74726F70 x4CHAR("port")://8:
            lbPORT:
                t=bfr+5;
                prti.sin_addr.s_addr=ConvertIP(t); ++t;
                prti.sin_port =atouis(t)<<8; ++t;
                if( (prti.sin_port |=atoui(t)) )
                {

                  i=sprintf(bb,"200 port: %u\r\n",prti.sin_port);
                  prti.sin_port=htons(prti.sin_port);
                  SendCMD(bb,i);
                  status&=~8;
                }
                else
                {erp:
                  SendConstCMD("500 bad port\r\n");
                }
                if(status&4)goto lbPRX4;
                break;
            case 0x7374706F x4CHAR("opts") :
                SendConstCMD( "200 Ok\r\n");
                break;
            case 0x706F6F6E x4CHAR("noop")://32: //NOOP
            lbNop:
                SendConstCMD( "220 FTP server ready\r\n");
                break;
            case 0x65746973 x4CHAR("site")://36: //SITE CHMOD 66
                if( (p=stristr(bfr,"chmod ")) && (t=strrchr(p+6,' ')) )
                {
                  l=pth.MkFName(bb,t+1);
                  if( !((pth.mode & V_LIMITED)?((pth.mode & V_RW) == V_RW):((puser->state&(UserFTPW|UserFTPR))==(UserFTPW|UserFTPR))
                  )
                  ) goto lerr550;
                  if(!CheckBadName(bb) )goto lerr501;
                  #ifdef SYSUNIX
                  CheckAccess(bb,2);
                  chmod(bb,strtol(p+6,&t,8)&((puser->state&UserNOCGI)?0666:0777)  );
                  #else
                  i=GetFileAttributes(bb);
                  r=strtoul(p+6,&t,8);
                  if(r&0x4)i&=~FILE_ATTRIBUTE_HIDDEN  ; else i|=FILE_ATTRIBUTE_HIDDEN  ;
                  if(!(puser->state&UserNOCGI))
                  {
                    if(r&0x2)i&=~FILE_ATTRIBUTE_READONLY; else i|=FILE_ATTRIBUTE_READONLY;
                    if(r&0x1)i|=FILE_ATTRIBUTE_SYSTEM;  else i&=~FILE_ATTRIBUTE_SYSTEM  ;
                  }
                  SetFileAttributes(bb,i);
                  #endif
                  goto lb250;
                }
            case 0x74616566 x4CHAR("feat") :
                SendPrint( "211-Extensions supported:\r\n"
                          " UTF8\r\n"
                          " MDTM\r\n"
                          " SIZE\r\n%s"
                          "211 end\r\n",( (s_flgs[2]&FL2_FTPTLS)
                          #ifndef TLSWODLL
                          && PSecAccept
                          #endif
                          ) ?
                          " AUTH TLS\r\n"
                          " PBSZ\r\n"
                          " PROT\r\n"
                          : ""
                );
                break;
            default:
              er502:
              SendConstCMD("502 Command not implemented\r\n");
              break;
    };
  }while(1);
  ex1:;
  if(cmd!=0x74697571)Send("421 \r\n",sizeof("421 \r\n")-1);
  if(s3>0)
  {
    CloseSocket(s3);
  }
  delete bb;
  Close();
  return 0;

  #undef SrcDirLen
  #undef cpath
  #undef SrcPath
  #undef puser
  #undef send
  #undef  s2
  #undef   _lopen
  #undef   _lcreat2
  #undef   MoveFile
  #undef   CreateDirectory
  #undef   RemoveDirectory
  #undef   DeleteFile
  #undef   GetFileAttributes
  #undef   SetFileAttributes
  #undef   FindNextFile
  #undef   FindFirstFile

  #define   _lopen              lopen
  #define   _lcreat2(a,b) lcreat2(a)
  #define   MoveFile            MoveFileA
  #define   CreateDirectory     CreateDirectoryA
  #define   RemoveDirectory     RemoveDirectoryA
  #define   DeleteFile          DeleteFileA
  #define   GetFileAttributes   GetFileAttributesA
  #define   SetFileAttributes   SetFileAttributesA
  #define   FindNextFile        FindNextFileA
  #define   FindFirstFile       FindFirstFileA

};




