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
#endif

#ifndef SRV_H
#include "srv.h"
#endif

char *wkday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
char *month[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
char *bad_list[]={
FSLUSHS "AUX",FSLUSHS "PRN",FSLUSHS "COM1",FSLUSHS "COM2",
FSLUSHS "COM3",FSLUSHS "COM4",FSLUSHS "CON",FSLUSHS "LPT",
FSLUSHS "LPT1",FSLUSHS "LPT2",FSLUSHS "NUL", FSLUSHS "CONIN$",
FSLUSHS "CONOUT$",FSLUSHS "CONERR$",FSLUSHS "CONFIG$",FSLUSHS "CLOCK$",FSLUSHS "IPC$",
FSLUSHS "ADMIN$",0
};


char *tipes[]={"text/html","image/gif","image/jpeg","image/jpeg","text/plain","text/css","application/octet-stream","application/x-javascript","*/*","application/octet-stream","*/*","image/png",0};
const ulong *ext_types= (ulong *)(void *)".HTM.GIF.JPG.JPE.TXT.CSS.CLA.JS\x00.ASI.PNG\x00\x00\x00\x00\x00\x00";
//                                         0   1   2   3   4   5   6  7      8


char bbch[]="/\\! \"#$%&\'()*+,-:;<=>?";

#ifndef SYSUNIX
/*
char* ConvertUtf(char *s,ushort *w)
{
 uchar *t,*p;
 int r=0,i;

   for(t=(uchar *)s; *t ; t++ )
   {
     if( (*t&0xE0)==0xC0 )
     {
       t++;  
       if((*t&0xC0)!=0xC0) return 0;
       ++r;
       ++t;
     }
     else if( (*t&0xE0)>=0x80 )return 0;
   }
   if(!r)return 0;
   
   if(w)
   {
     utf2unicode((uchar *)s,w);  
   }

    for(p=t=(uchar *)s; *t ;  )
    {
     if( (*t&0xE0)==0xC0 )
     {
       i=((*t&0x1F)<<6)| (t[1]&0x3F);
       //if( (i-=128)>0 && i< )
       *p++=(i>=0x400 && i<0x440 )? i - 0x350 :  //(0x410-0xC0) :
           (i&0x7F)|0x80 ;
       t+=2;

       continue ;
     }

     *p++=*t++;
    }

  *p=0;
  return (char*)p;
}
*/
#undef FindFirstFile    
#undef _lopen           
#undef GetFileAttributes
#undef FindNextFile     

#define   FindFirstFile        FindFirstFile_UTF
#define   _lopen               lopen_UTF 
#define   GetFileAttributes   GetFileAttributes_UTF
#define   FindNextFile        FindNextFile_UTF


#endif


char* CheckBadName(char *in_buf)
{char *t,*p,*pp;
 int ii;
 for(p=t=in_buf;*t;++t,++p)
  if(*t=='/'){*p=FSLUSH; if(p!=in_buf && p[-1]==FSLUSH) --p;}
  else if(*t=='%' && (ii=atouix2(t+1))>=0x20 && t[2]>='0'){*p=ii; t+=2;}
  else *p=*t;
 *p=0;
#ifndef SYSUNIX
// if(s_flgs[2]&FL2_UTF) if( (t=ConvertUtf( in_buf,wbuf ) ) ) p=t;
#endif
 pp=in_buf;
 while((pp=strstr(pp,"..")))
 {if(! (s_flgs[1]&FL1_2P) )return 0;
  if( pp[-1]<0x20 || strchr(bbch,pp[-1]) || pp[2]<0x20 || strchr(bbch,pp[2]))return 0;
  ++pp;
 }
 if(strstr(in_buf,".\\") )return 0;
 for(char **bdlst=bad_list;*bdlst;++bdlst )
  if((t=stristr(in_buf,*bdlst )))
   if((!(ii=t[strlen(*bdlst)]))
     || (ii==FSLUSH) || (ii=='.') || (ii<='&') || (ii>'}') )return 0;
 if(strpbrk(in_buf,"<>\"\n\r*?"))return 0;
 return p;
};
int IsCGI(char *bb,int j)
{
#if defined(SYSUNIX) && !defined(CD_VER)
 struct stat st;
 if(stat(bb,&st)<0) return 0;
 return (st.st_mode&j); // !User X
#else
 int i,k;
 if(stristr(bb,cgi_detect)||stristr(bb,".pht")||stristr(bb,".php")||stristr(bb,".pml"))
   return 1;
 if( (!(s_flgs[1]&(FL1_RUNSYSTEM|FL1_DNTSHOWHIDEN)
     ) ) || ( (i=GetFileAttributes(bb))==-1) )return 0;
 k= (s_flgs[1]&FL1_RUNSYSTEM) && (i&FILE_ATTRIBUTE_SYSTEM) ;
 if( (j&4) && (s_flgs[1]&FL1_DNTSHOWHIDEN) && !(FILE_ATTRIBUTE_HIDDEN&i) ) k|=4;
 return k;
#endif
};
//---
int IsInStrLst(char *pwd,char *t)
{int h;
 union{uint x; uchar xb;};
 char *s;
 s=pwd;
 h=strlen(t);
 while( (pwd=strstr(pwd,t)))
 {if( s==pwd || !(pwd[-1]&~0x2C) )
  {if( !(pwd[h]&~0x2C) ) return 1;
   if( (pwd[h]==':') )
   {x=DWORD_PTR(pwd[h+1])
#if 0
    &0xDFDFDF;
    return (x==0x5257 || x==0x5752)?0xE:(xb==0x52)?0xB:(xb==0x57)?0xC:8;
#else
    ;
    h=8;
    while(x&~0x2C)
    {switch(x&0xDF)
     {
      case 'F': return 0x80;
      case 'R': h|=3; break;
      case 'W': h|=5; break;
      case 'H': h|=0x20; break;
      case 'D': h|=0x10; break;
     }
     x>>=8;
    }
    return h;
#endif
   }
  }
  ++pwd;}
 return 0;
}

#ifdef USE_FUTEX
const  struct timespec timeout_30s={30,50000000};
#endif

int Req::HttpReq()
{
#undef send
#undef recv
#define send(a,b,c,d) Send(b,c)
#define recv(a,b,c,d) Recv(b,c)
 char *xin_buf,*in_buf;
 char *p,*t,*tp,*tp1,*tpp,*templ,*pwd;
 int l,i,h,ii,xgz,ll,ncntn,proxy_flg;
 ulong ttm=0,tick;
 int head;
#ifndef SYSUNIX 
union{ 
#endif    
 WIN32_FIND_DATA fd;
#ifndef SYSUNIX 
 WIN32_FIND_DATAW fdw;
};
#endif    
 SYSTEMTIME tm;
 HANDLE hf;
 union{host_dir *a; ulong  ul;};
 char *vdir;
 User *puser;
 int mn;
 union {
  long long llmn;
  ulong llmnv[2];
 };

 union {
  long long l0;
  ulong l0l[2];
 };

 union {
  long long l1;
  ulong l1l[2];
 };


 LimitBase *lipo,*lneto;
 proxy_flg=((fl>>16)%9)==1; /*F_PROXY*/
 if(  FndLimit(ncntn=(proxy_flg?2:7) ,&lipo,&lneto,&sa_c ) )
 {
   HttpReturnError("Limit overflow");
   return 1;
 }
#define MAX_KEEPALIVE 512
 int KeepAliveCounter=MAX_KEEPALIVE;
 if(! (xin_buf=new char[0x10200]) )return -1;
// Tin=0; Tout=0;
 gz=0;
 xgz=0;
 //zz[1]=0;
#define out_buf (xin_buf+8448)
 a=0;
 trn=0;
do{
 if(!proxy_flg)trn=0;
 else if(trn_sock && RESelect(0,0,1,trn_sock) )
 {
   CloseSocket(trn_sock);
   trn=0;
 }

 p=in_buf=xin_buf+256;
 dir=def_dir; vdir=0; templ=0; l=0; head=0;
 postsize=0;
 vhdir=0;
 tick=GetTickCount();
 do{
  if(l>=8190){ goto bdreq; }
  if((i=Recv(p,8190-l)) <= 0)
  {if(l)break;
  er_recv:
   if(KeepAliveCounter==MAX_KEEPALIVE)
   {AddToLog("Connection closed",s,FmtShort);
    setsockopt(s,SOL_SOCKET,SO_LINGER,(char *)&lngr,sizeof(lngr));
   }
   goto ex2;
  }
  p+=i; l=p-in_buf;
  in_buf[l]=0;
  if((l>32) && ((GetTickCount()-tick)/(ulong)l)>1024)goto er_recv;
  if((pst=strstr(in_buf,"\r\n\r\n")))break;
  if((pst=strstr(in_buf,"\n\n"))){pst+=3; break;}
 }while(1);
 postsize=l-(pst-in_buf)-4;
 KeepAlive=stristr(in_buf,"Connection: Keep-Alive");

 if( (s_flgs[1]&FL1_GZ) &&
    zlibstate>0 && (t=StrVar(in_buf,"Accept-Encoding")) && stristr(t,"gzip") )
 { fl|=xgz=F_GZ;  }
lbxx:
 fl=xgz;
 if( strstr(in_buf,"HTTP/1.0") )
 {  fl|=F_HTTP10;
    xgz|=F_HTTP10;
 //   debug("HTTP 1.0");
 }

 AddToLog(in_buf,s,(KeepAliveCounter!=MAX_KEEPALIVE)?FmtBasicC:FmtBasic);

 sprintf(inf,"%.94s",in_buf+((in_buf[8]==':')?11:4));

 
 tp1=inf+1;
 
 if( DWORD_PTR(*in_buf)==0x4E4E4F43  // x4CHAR("CONN")
               ) {tp1+=4; h=4; }
 else  if( DWORD_PTR(in_buf[0]) == 0x20544547 x4CHAR("GET ") ) h=4;
 else if( ( (DWORD_PTR(in_buf[0]) == 0x54534F50 x4CHAR("POST") ) )
  || (head= ( DWORD_PTR(in_buf[0]) == 0x44414548 x4CHAR("HEAD") )
 ))h=5;
 else{HttpReturnError("Error. Unsupported method.",405); goto ex2;}


 if( (tp1=strchr(tp1,' ')  ) )*tp1=0;

#if V_FULL
 if( proxy_flg /*   (fl&0xF0000)==F_PROXY */){rq=in_buf; dirlen=l; ProxyReq(); goto ex1;};
#endif
 tp1=0;
 if((tp=strchr(in_buf,'\r'))){*tp=0;
  for(a=hsdr.next;a;a=a->next)if(a->h[0]=='/')
   if((vdir=stristr(in_buf,a->h)) && vdir<strpbrk(in_buf+5,"? ") )goto vhstfound;
  *tp='\r';
 }a=0;
 if((t=StrVar(in_buf,"HOST"))) if((tp=strchr(t,'\r')))
 {*tp=0;
  if((tp1=strchr(t,':')))*tp1=0;
  for(a=hsdr.next;a;a=a->next)
  {if(!stricmp(a->h,t)){
vhstfound: dir=a->d;
     vhdir=a;
   break;}
  }
  *tp='\r'; if(tp1)*tp1=':';
 }
// debug((vdir || a)?" VDIR : %s %s %.4s %X ":"NO vdir",vdir,dir,dir+3,DWORD_PTR(dir[3]));
#ifndef CD_VER
 if(//vdir &&
     DWORD_PTR(dir[3]) == 0x2F2F3A70 x4CHAR("p://") )
 {
  h=80;
  if((tp=strchr(dir+7,':')))
  {
   h=atoui(tp+1);
   strncpy(out_buf,dir,72);
   out_buf[tp-dir]=0;
   dir=out_buf;
  }

  if((h=call_socket(dir+7,h))<=0)
  { HttpReturnError("Error. Can\'t connect...");
  }
  else
  { BSend(h,in_buf,l);
    p2p(h,xin_buf);
    //shutdown(h,2);
    //closesocket(h);
    CloseSocket(h);
  }

  goto ex2b;

 }
#endif
 dirlen=strlen(dir);
 if(dir[dirlen-1]==FSLUSH )dir[--dirlen]=0;
 fl=xgz;

 /*
 if( DWORD_PTR(in_buf[0]) == 0x20544547 x4CHAR("GET ") ) h=4;
 else if(((fl=  (DWORD_PTR(in_buf[0]) == 0x54534F50 x4CHAR("POST") ) ))
  || (head= ( DWORD_PTR(in_buf[0]) == 0x44414548 x4CHAR("HEAD") )
 ))h=5;
 else{HttpReturnError("Error. Unsupported method."); goto ex2;}
 */
 if( DWORD_PTR(in_buf[0]) == 0x54534F50 x4CHAR("POST") )fl|=1;

 if( DWORD_PTR(in_buf[h+3]) == 0x2F2F3A70 x4CHAR("p://") )
 {if( (t=strchr(in_buf+h+8,'/'))) h=t-in_buf;else in_buf[(h+=6)+1]=' ';}
 if(vdir){ if((t=strchr(vdir+1,'/'))) h=t-in_buf; }
 memcpy( (in_buf-=dirlen-h),dir,dirlen);
 l+=dirlen-h;
 for(p=in_buf+dirlen;*p&~0x20;p++);
 *p=0;

 if( (t=stristr(in_buf, "\nAUTHORIZATION"))
      &&
      ((ulong) ( strchr(t+8,'\n')-t )) > 512ul
   )
 {
  AddHack(0,100);
  goto bdreq;
 }
 if((p-in_buf)>7168)
 {

bdreq:
  HttpReturnError( sERROR__BA );
  goto ex2;
 }

 if((req=strchr(in_buf,'?'))){
   *req++=0;
   if(req[-2]=='/')
   {
    ll=strlen(def_name)+8; //32;
    if((ll+l)<8190)
    {
  //   debug("1req=|%s| p=|%s| %u %u **%s**",req,p+1,l,ll,in_buf+l);
     memcpy_back(in_buf+l+ll,in_buf+l,in_buf+l-req+1 );

     if(p>=req)p+=ll;
     if(pst>=req)pst+=ll;
     req+=ll;
     l+=ll;
    // debug("req=|%s| p=|%s| %u %u",req,p+1,l,ll);
    }
   }

 }
 else req=NullString;

 if(pst){*pst=0;pst+=4;}
 trn=0;

 loc=in_buf;

 rq=p+1;

 http_var=(char **) (xin_buf+0x8000);
 prepare_HTTP_var();
#ifndef CD_VER
 if( strin(in_buf+dirlen,"/$_admin_$") )
 {if( (s_flgs[1]&FL1_ATLS && (Snd!=&TLSSend)) ||
      (IsInIPRange(adm_range)<=0 )
       || IsProxyRange(adm_range) ) HttpReturnError(sACCESS_DE);
  else Admin();
  goto ex2b;
 }
 if( strin(in_buf+dirlen,"/$_wmail_$") )
 {if( (s_flgs[2]&FL2_WMTLS && (Snd!=&TLSSend)) ||
       (! (s_flgs[2] & FL2_WMAIL) ) ||
       (IsInIPRange(pop_range)<=0 )
       || IsProxyRange(pop_range) ) HttpReturnError(sACCESS_DE);
  else Wmail();
  goto ex2b;
 }
 if( strin(in_buf+dirlen,"/$_ip2country_$" ) && (s_flgs[2]&FL2_IPCONTRY)  )
 {
  IP2country();
  goto ex2b;  
 }    
 if(  (s_flgs[2]&FL2_DOH) && doh_w>0 && ! strcmp(in_buf+dirlen,"/dns-query" )  )
 {
  if( req!=NullString && DWORD_PTR(req[0]) == 0x3D736E64 x4CHAR("dns=") )
  {
    pst=out_buf;
    t= CopyBB(pst,req+4);
    postsize=t-pst;
//    debug("DOH:%u %X %X %X %X %s\n",postsize,DWORD_PTR(pst[0]),DWORD_PTR(pst[4]),DWORD_PTR(pst[8]),DWORD_PTR(pst[12]),pst+14);
  }
  if(postsize<17) goto bdreq;
  dirlen=0;
  t=(char *) this;
  _hwrite(doh_w,(char *)&t,sizeof(t));
    while(!dirlen)
    {    
#ifdef USE_FUTEX
     futex((int *)&dirlen,FUTEX_WAIT,0,&timeout_30s,0,0);  
#else      
     Sleep(50); 
#endif       
    }  
//    debug("DoH ready %d",dirlen);
    if(dirlen>0)
    {
        t=out_buf+1024;
        Send(t,sprintf(t,  
  "HTTP/1.0 200 Ok\r\n"
  "Content-Length: %u\r\n"
  //"Cache-control: max-age=%u\r\n"
  "Content-Type: application/dns-message\r\n\r\n", dirlen
                 )); 
       Send(pst,dirlen); 
    }
    else
    {
      HttpReturnError("Resolving error");  
    }    
    goto ex2b;  
 }    
// vhdir=a;
 if(a && a->flg)
 {if(!((t=CheckAuth(pwd=xin_buf+0x9000))
    &&(puser=FindUser(t,255,pwd,this))
    && (ii=IsInStrLst(a->d+a->flg,t))
    && (! (ii&0x20) )
    && ( (ii&7)!=5  )
  ) )
  {
#ifdef WITHMD5
//    if(  ( (s_flgs[2] & FL2_MD5PASS) || ! (s_flgs[1]&FL1_CRYPTPWD) ) &&  (s_flgs[2] & FL2_USEMD5D) && !(fl&F_DIGET_UNAVILABLE)  )
//    {
       SendDigestAuthReq(in_buf); 
//    }
#else
     send(s,AuthErr,    strlen(AuthErr)  //sizeof(AuthErr)-1
              ,0);
#endif      
   goto ex2b;
  }
  dir=a->d;
 }

#else
 if(!strcmp(in_buf+dirlen,"/exit") )
 {HttpReturnError("Exit... Bye.");
  CloseServer();
  return 0;
 }
#endif
 if(! (p=CheckBadName(in_buf))) goto bdreq;
 if(p[-1]==FSLUSH)
 {
  if(fl&1 )//POST
  {
    ll=strlen(def_name)+8; //32;
    if((ll+l)<8190)
    {
     memcpy_back(in_buf+l+ll,in_buf+l,in_buf+l-p+1 );
     if(pst>=p)pst+=ll;
     if(rq>=p)
     {
         rq+=ll;
//         for(char ** phh=http_var; phh && *phh ; phh++ )if(*phh>p && *phh<pst)(*phh)+=ll;
         for(int ij=0; http_var[ij] ; ij++ )if(http_var[ij]>=p && http_var[ij]<pst)http_var[ij]+=ll;
     }
     l+=ll;
    // debug("req=|%s| p=|%s| %u %u",req,p+1,l,ll);
    }    
      
  }
  p+=sprintf(templ=p,"%.63s",def_name);}
lcnt_trn:
//debug("DBG1 %s",in_buf);
 if((hf=FindFirstFile(in_buf,&fd) )!=INVALID_HANDLE_VALUE)
 {
//  debug("DBG2 %s %u",fd.cFileName,templ);

  if(templ)
  {while(fd.cFileName[0]=='.' && fd.cFileName[1]<'0'  )if(!FindNextFile(hf,&fd))
   {FindClose(hf); goto lbTmpl;}
    //if(!trn)
         rq+=sprintf(templ,"%.63s",fd.cFileName);

  //  debug("DBG3 %s %o %s",fd.cFileName,fd.st.st_mode,templ);


  }
  FindClose(hf);
#if defined(SYSUNIX) && !defined(CD_VER)
  if(!(fd.st.st_mode&5))goto  lbe1; // bdreq; // !User R|X
#else
  if( (FILE_ATTRIBUTE_HIDDEN&fd.dwFileAttributes)  &&
      (s_flgs[1]&FL1_DNTSHOWHIDEN)  ) goto  lbe1; //bdreq;

  if( (!(tpp=stristr(in_buf,cgi_detect))) && vdir)tpp=stristr(a->h,cgi_detect);
  if( (!tpp) &&
      (s_flgs[1]&FL1_RUNSYSTEM) &&
      (fd.dwFileAttributes&FILE_ATTRIBUTE_SYSTEM)
  )tpp=(char *)1;
#endif
  
  ii=9;
  
  if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
  {for(p=in_buf;*p;p++)if(*p=='\\')*p='/';
   WORD_PTR(*p)='/';
   send(s,p+2,sprintf(p+2,"HTTP/1.0 301 \r\nLocation: %s\r\n\r\n",in_buf+dirlen),0);
   ii=9;
   goto ex2a;
  }
  else if( (t=strrchr(in_buf,'.')))
  {if(!t[1])goto bdreq;
   h=DWORD_PTR(*t);
   if(t[1]>='a')h&=~0x2000;
   if(t[2]>='a')h&=~0x200000;
   if(t[3]>='a')h&=~0x20000000;
   if(xgz && (stristr(NoGZSuf,t))) {xgz=0; fl&=~F_GZ; }
   for(ii=0;ext_types[ii];ii++)if(h==ext_types[ii]) //goto lbExtFound; 
     break;
  // ii=9; 
  // lbExtFound:
   if(
#if defined(SYSUNIX) && !defined(CD_VER)
    (!(fd.st.st_mode&1)) //!User X
#else
    (!tpp)
#endif
    && (h==0x4D54532E x4CHAR(".STM") ||
        h==0x5448532E x4CHAR(".SHT") ||  h==0x5053412E x4CHAR(".ASP")
       ||((s_flg&0x80) && (h==0x4D54482E x4CHAR(".HTM")))
     ))
   {
    //fl<<=8;
#ifdef SYSUNIX
    KeepAlive=(char *)&(fd.st);
#endif
    if(head)
    {
     send(s,in_buf,sprintf(in_buf,"HTTP/1.0 %u Ok\r\nContent-Type: text/html\r\n"
     "%s\r\n"
     "\r\n",(loc!=error_file)?200:404,header),0) ;
//! remove next line     
//    debug("SSI HEAD");
    }
    else
    {
      SendSSI();
      if(fl&F_CHUNKED)
      {
        fl&=~F_CHUNKED;
        if( send(s,"0\r\n\r\n",5,0) > 0 )  goto ex1;
      }
    }
    goto ex2a;
   }

#if !defined(SYSUNIX) || defined(CD_VER)
   if( h==0x4D54502E x4CHAR(".PTM") || h==0x5448502E x4CHAR(".PHT") || h==0x5048502E x4CHAR(".PHP")  )
   {
       fl|=F_PHP; 
       goto lcgi;
       
   }
#endif
  }
//  else ii=9;


  if(
#if defined(SYSUNIX) && !defined(CD_VER)
    (fd.st.st_mode&1) //!User X
#else
    tpp && (ii>=4 ||
     (fd.dwFileAttributes&FILE_ATTRIBUTE_SYSTEM) ||!(s_flg&FL_NORUNHTM))
#endif
  ){
lcgi:
#ifdef SYSUNIX
   KeepAlive=(char *)&(fd.st);
#endif
   ExecCGI();
   goto ex2a;
  };
  FileTimeToSystemTime(& (fd.ftLastWriteTime),&tm);
  if( (tpp=GetVar(http_var,"If_modified_since")) )
  {sprintf(p=out_buf,"%2.2u:%2.2u:%2.2u GMT",tm.wHour,tm.wMinute,tm.wSecond);
   if(tpp=stristr(tpp,p))
   {send(s,"HTTP/1.0 304 \r\n\r\n",sizeof("HTTP/1.0 304 \r\n\r\n")-1,0);
    goto ex2a;
   }
  };
  tpp=tipes[ii];
  if( (ii>=9) && mime && t && t[1] )
  {
    /*  
    if(in_buf==error_file)
    {
       in_buf=xin_buf;
       strcpy(in_buf,error_file);
    }    
    */
    DWORD_PTR(* (tpp=in_buf+16384) ) = DWORD_PTR(*t);
   tpp[4]=0;
   if( (p=stristr(mime,tpp)) && (p=strchr(p,';')) )
   {strncpy(tpp,p+1,64);
    if(p=strchr(tpp,';'))*p=0;
   }else tpp=tipes[ii];
  }
 }
 else
 {
  if(!trn)
  {if(templ)
   {
   lbTmpl:
    if( (s_flg&FL_NODIR) // && user_name==(about+sizeof(ABOUT_STR)+14) 
      )goto lbe1;
    DWORD_PTR(*templ)='*';

    if( (hf=FindFirstFile(in_buf,&fd) )!=INVALID_HANDLE_VALUE )
    {OutDirTop();
     do{
     if(WORD_PTR(fd.cFileName[0])!='.'
#ifndef SYSUNIX
         && !
        ( (FILE_ATTRIBUTE_HIDDEN&fd.dwFileAttributes)  &&
          (s_flgs[1]&FL1_DNTSHOWHIDEN) )
#endif
     )
     {FileTimeToSystemTime(&(fd.ftLastWriteTime),&tm);
      t=(char *) ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)?"/":"");
      if( Send(in_buf,sprintf(in_buf,(char *) HTMLDirBody2,
       fd.cFileName,t,fd.cFileName,t,fd.cFileName,t,(ulong)fd.nFileSizeLow,(ulong)fd.nFileSizeLow,fd.cFileName,t,
       tm.wYear,tm.wMonth,tm.wDay,
       tm.wHour,tm.wMinute,tm.wSecond,
       tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond
       )) <=0 )break;
      }
     }while(FindNextFile(hf,&fd));
     FindClose(hf);
     OutDirBottom(in_buf);
     goto ex2a;
    }
    else *templ=0;
   }
// else
   {
    if((t=strchr(in_buf,'.')))
    { if( (trn=strchr(t,FSLUSH) ) )
      { trn[-1]=0; if(templ)*templ=0;
        goto lcnt_trn;
      };
    }


    if((!templ) && (templ=strrchr(in_buf,FSLUSH)))
    {rq+=sprintf(trn=rq-1,".asi*");
     ++templ;
     goto lcnt_trn;
    }
   };
  }
  else *trn=0;
  goto lbe1;
 };

 if((h=_lopen(in_buf,0))<0)
 {lbe1:;
  if( //in_buf!=error_file && 
      loc !=error_file )
  {if( (h=strlen(in_buf)) > 800){ DWORD_PTR(in_buf[800])=0x2E2E; h=802; }
   sprintf(in_buf+h,"(%s)\r\nError. File not found.\r\n",a?a->h:"Undefined/default");
   if(error_file)
   {
    if( stristr(error_file,".php") ||  stristr(error_file,".pht")  )
    {
      req=in_buf+dirlen-4;
      DWORD_PTR(*req)=0x3D727265 x4CHAR("err=") ;
      fl|=F_PHP;
      AddToLog(in_buf+dirlen,s);
    }
    else
    {    
     trn=in_buf;
     AddToLog(req=in_buf+dirlen,s);
    }
    in_buf[h]=0;
    templ=0;
//    loc=in_buf=error_file;
    loc=error_file;
    in_buf=xin_buf;
    strcpy(in_buf,error_file);

    
    goto lcnt_trn;
   }
  }
  else {in_buf=trn; in_buf[h]='(';}
  HttpReturnError(in_buf+dirlen,404);
  goto ex2a;
 };
 l1l[0]=fd.nFileSizeLow;
 l1l[1]=fd.nFileSizeHigh;

 if( (t=GetVar(http_var,"Range")) && ( t[ mn=(t[0]=='b')?6:0]!='-'))
 {
  if(s_flgs[1]&FL1_NOM )
  {++no_close_req;
   for(int j=0;j<max_tsk;++j)
   {if( ((u_long)(rreq[j]))>1 && rreq[j] !=this &&
         rreq[j]->sa_c.sin_addr. S_ADDR == sa_c.sin_addr. S_ADDR
      )
    {
     --no_close_req;
     HttpReturnError("Only one stream enabled");
     _lclose(h);
     goto ex2a;
    }
   }
   --no_close_req;
  }
  llmn=_llseek64(h,atoll(t+mn),0);
    //fd.nFileSizeLow-llmnv[0];

  if(
     (
       ( tp=strchr(t+mn,'-') ) &&
       (l0=atoll(tp+1)) &&
       l0<l1
     )
    )
    {
      l1=l0-llmn+1;
    }
    else
    {
     l0=l1;
     l1-=llmn;
    }
 }
 else llmn=0;

 l=MIN(l1l[0],0x10000);
 OutLL(l1l[0],l1l[1],      //fd.nFileSizeHigh-(fd.nFileSizeLow<llmnv[0])-llmnv[1],
  out_buf+1024);
 i=sprintf( out_buf,"HTTP/1.1 %u Ok\r\nContent-Length: %s\r\nLast-modified: %3.3s, %u %3.3s %4.4u %2.2u:%2.2u:%2.2u GMT\r\nContent-Type: %s\r\nConnection: Keep-Alive"
 "\r\n%s\r\n\r\n",(//in_buf
                   loc!=error_file)?200:404,
 // (ulong)fd.nFileSizeLow-mn,
   out_buf+1024,
 wkday[tm.wDayOfWeek],tm.wDay,month[tm.wMonth-1],tm.wYear,tm.wHour,tm.wMinute,tm.wSecond, tpp,header /* tipes[ii] */ );
 if(!*header) i-=2;

 if(ii!=8){
  OutLL(fd.nFileSizeLow,fd.nFileSizeHigh,out_buf+1024);
  if(llmnv[0]){

   OutLL(l0l[0],l0l[1],out_buf+1054);
   OutLL(llmnv[0],llmnv[1],out_buf+1084);
   i+=sprintf(out_buf+i-2,"Content-Range: bytes %s-%s/%s\r\n\r\n",
   //(ulong)fd.nFileSizeLow,(ulong)fd.nFileSizeLow
   out_buf+1084,out_buf+1054,out_buf+1024
   )-2;
   out_buf[11]='6';

   //fd.nFileSizeHigh-= (fd.nFileSizeLow<llmnv[0]) + llmnv[1];
   //fd.nFileSizeLow-=llmnv[0];
  };
 }else{i=9; xgz=0; _llseek(h,8,0); l1-=8; //fd.nFileSizeLow-=8;
 }
 if(
  (xgz) &&
  (! (llmnv[0]) ) &&
  fd.nFileSizeLow>GZSizeLow
    && IGZP())
 {

//debug("+* %d %u %X",zlibstate,GZSizeLow,xgz);
   i=sprintf(out_buf,
   "HTTP/1.1 %u Ok\r\nLast-modified: %3.3s, %u %3.3s %4.4u %2.2u:%2.2u:%2.2u GMT\r\nContent-Type: %s\r\n"
   "%s\r\n", //fd.nFileSizeLow-mn,
   (//in_buf
    loc!=error_file)?200:404,
   wkday[tm.wDayOfWeek],tm.wDay,month[tm.wMonth-1],tm.wYear,tm.wHour,tm.wMinute,tm.wSecond, tpp,header /* tipes[ii] */
  );
  KeepAlive=0;
  memcpy(out_buf+i,gz_head,sizeof(gz_head));
  i+=sizeof(gz_head);
 }
 else fl&=~F_GZ;

 if(send(s,out_buf,i,0)>0)
 if(!head)
 { if(fl&F_GZ)Snd= GZSnd;
   do{ i=_hread(h,xin_buf,l); if( (i<=0) || send(s,xin_buf,i,0)<=0)break;
    //if((ulong)fd.nFileSizeLow<(ulong)i) --fd.nFileSizeHigh;
    //fd.nFileSizeLow-=i;
   }while( (l1-=i)>0 ) ; //fd.nFileSizeLow>0 || fd.nFileSizeHigh>0 );
 }
 _lclose(h);
#undef l
 ex1:;
 ttm+=GetTickCount()-tick;
 if( (! KeepAlive) || --KeepAliveCounter<=0 )break;
 i=20;
 if( proxy_flg /*fl&F_PROXY */) i=3;
 do{if(--i<=0 || !(waited[(fl>>16)&1]) )goto ex3;}while(!(ii=RESelect(0,300000,1,s)));
}while(ii>0);
ex3:
ex2:
// debug("ex23: %d %d %d %X",s,trn,ii,proxy_flg);

 if( proxy_flg /*fl&F_PROXY */)
 { if(trn_sock)
   { //shutdown(trn_sock,2); closesocket(trn_sock);

     CloseSocket(trn_sock);
  //   debug("Close trn %d",trn);

 }  }
 else{
ex2a:
  GZEnd();
  if(!ttm)ttm+=GetTickCount()-tick;
  sprintf(xin_buf,"HTTP  in:%u out:%u %s Time:%u\r\n",Tin,Tout,(a && a->h) ?a->h:"",ttm);

  AddToLog(xin_buf,s,FmtShort);
 }
 if(lipo && lneto)
 {  tick=(Tin+Tout+1023)>>10;
    lipo->cnt+=tick;
    lneto->cnt+=tick;
    ipcnts[ncntn].d[0].cnt+=tick;
 }
ex2b:
 delete xin_buf;
#undef out_buf
 return 0;
};



#ifdef WITHMD5
//#undef debug
int nonceCounter,nonceFirst,tempKey;
int Req::SendDigestAuthReq(char *bfr){
 ulong opaque;   
 char nonce[64];
 char ipv6[40];
 int is_proxy;
 
 if(  ( (s_flgs[1]&FL1_CRYPTPWD) && !(s_flgs[2] & FL2_MD5PASS) ) || (fl&F_DIGET_UNAVILABLE)  || ! (s_flgs[2] & FL2_USEMD5D)   )
     return Send(AuthErr, strlen(AuthErr)  ) ; //sizeof(AuthErr)-1); 
 if(!tempKey)
 {
     nonceFirst=nonceCounter =Rnd()^(count_of_tr*pval);
#ifdef SYSUNIX
  // usleep(10);
  sched_yield();
#else
   Sleep(1);
#endif
     tempKey=Rnd();
 }    
 nonceCounter++;
 
 DWORD_PTR(ipv6[0])='n';
 opaque=sa_c.sin_addr. S_ADDR;
#ifdef USE_IPV6
 if(sa_c.sin_family==AF_INET6)
 {
   sprintf(ipv6,"i%Xz%Xz%Xz%Xx",sa_c6.sin6_addr.s6_addr32[0],sa_c6.sin6_addr.s6_addr32[1],sa_c6.sin6_addr.s6_addr32[2],sa_c6.sin6_addr.s6_addr32[3]); 
   if(sa_c6.sin6_addr.s6_addr32[2]==0xFFFF0000)opaque=sa_c6.sin6_addr.s6_addr32[3];
 }
#endif 
 sprintf(nonce,"%8.8X%8.8X%s%Xr%X",
         tempKey,opaque, //sa_c.sin_addr. S_ADDR, 
           ipv6 ,nonceCounter,Rnd()
 );
 opaque=MkName(nonce); 
 
 is_proxy=((fl>>16)%9)==1;
 return 
 //int r=
 Send(bfr,sprintf(bfr,  
  "HTTP/1.0 40%u Unauthorized" CRLF  
  "%s-Authenticate: Digest realm=\"%s\","   
//  " qop=\"auth\","
  "%s"
  " nonce=\"%s\","    
  " opaque=\"%X\""    CRLF 
  "Content-Type: text/html" CRLF  
   CRLF  
  "<h1>Authorization request\r\n", is_proxy?7:1  ,  is_proxy?"Proxy":"WWW" ,realm,(FL2_PARANOIDMD5 & s_flgs[2]  )?" qop=\"auth\",":"", nonce+8,opaque
                 ));
 
 // debug(bfr);
 // return r;
}; 
int Req::CheckNonce(char *nonce,char *opaque)
{
 char nonce2[64];
 char ipv6[40];
 char *p;
#ifdef USE_IPV6
 uint l;   
 p=nonce+9;
 if(sa_c.sin_family==AF_INET6)
 {
   l=sprintf(ipv6,"i%Xz%Xz%Xz%Xx",sa_c6.sin6_addr.s6_addr32[0],sa_c6.sin6_addr.s6_addr32[1],sa_c6.sin6_addr.s6_addr32[2],sa_c6.sin6_addr.s6_addr32[3]);  
   if(! (p=strstr(nonce,ipv6)))return 0;
   p +=l;
 }
 else
#endif 
   if(strtoul(nonce,0,16)!=sa_c.sin_addr. S_ADDR) 
   {
       debug("Bad nonce IP");

       return 0;   
   }   
   
 sprintf(nonce2,"%8.8X%s",tempKey,nonce);
 if(MkName(nonce2)!= strtoul(opaque,0,16) )
 {
     debug("Bad nonce 2");
     
     return 0;
 }
 l=strtoul(p,&p,16);
 if( (!p) || *p!='r'  )
 {
     debug("Bad nonce 3");
     return 0;
 }   
 if( (nonceCounter>=nonceFirst) ?
       l>=nonceFirst && l<=nonceCounter :
       l>=nonceFirst || l<=nonceCounter 
   )
  {
//      debug("Ok nonce");
      
     return 1;
  }   

 debug("Bad nonce 4");
 return 0;
};

#endif 

#undef send
#undef recv

#ifndef SYSUNIX

#undef   _lopen            
#undef   GetFileAttributes
#undef   FindFirstFile  
#undef   FindNextFile     

#define   FindNextFile        FindNextFileA
#define   _lopen              lopen           
#define   GetFileAttributes   GetFileAttributesA
#define   FindFirstFile       FindFirstFileA


#endif
