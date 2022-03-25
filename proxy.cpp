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

#ifdef SEPLOG

#undef debug
#undef AddToLog

#define debug(a...)  sepLog[1]->Ldebug(a)
#define AddToLog(a...)  sepLog[1]->LAddToLog(a)

#endif


#if 0
//def DBG
char tstlog[]="00000000.q";
char stlogn[]="sqpd";
void DBG_WRITE(char *bfr,int i,int t)
{int h;
 int j=7;
 tstlog[9]=stlogn[t];
 h=_lcreat(tstlog,0);
 _hwrite(h,bfr,i);
 _lclose(h);
 while(j>=0 && ++ tstlog[j]>'9' )tstlog[j--]='0';
}

#else
#define DBG_WRITE(bfr,i,t)
#endif

#define dbg2(a,b)

#if defined(SPECIAL) || ! defined(CD_VER)

int CacheMutex;
char *proxy_antivirus;
int proxy_antivirus_port=3310;

ulong MkName(char *t,int brk) //=' ')
{uint i=0,j=0,k=0;
 for(;*t && *t!=brk;t++){i+=((int)*t)<<(k&0x7); j^=*t; k++;};
 return  (i<<16)|(j<<8)|k;
};

ulong MkName2(char *t,int brk=' ')
{uint i=0,k=0; //,j=0
 for(;*t && *t!=brk;t++){i+=((int)*t)<<(k&0x15); // j*=*t;
   k++;};
 return rol(i,8)^k; //^j
};

//*
ulong MkName5(char *t,int brk=' ')
{uint i=0,k=0,j=0;
 for(;*t && *t!=brk;t++){i+=((int)*(uchar *)t)*(k+3);  j+=(~j&0xFF)<<(*t&0x1F);
   k++;};
 return i^j;
};

char fnabc[]="0123456789abcdefghijklmnopqrstuvwxyz_~";
void abname(char *t,uint n1,uint n2)
{
 int i,j;
 /*
 union{
  long long a;
  ulong v[2];
 };
 //a=(n1+1)*(n2+1);
 v[0]=n1;
 v[1]=n2;
 */
 for(i=0; i<8; ++i )
 {
   j=n1%37;
   t[i]=fnabc[j];
   n1/=37;
   if(n1<37){n1+=n2; n2=0;}
   //n1=(n1/37) + ((n2%37)*);
   //n2/=37;
 }
}

// */

#ifdef MEMPRX
#include "prxcach.cpp"
#endif

int anychars(char *name)
{for(; *name; name++) if(*name>='A') return 1;
 return 0;
}

int call_socket(char *lhstname, int portnum)
{
#ifdef  USE_IPV6
  union{
#endif
 struct sockaddr_in sa;
#ifdef  USE_IPV6
 struct sockaddr_in6 sa6;
  };
#endif

 struct hostent *hp;
 char *hhh;
 ulong *p,n,a;
 int s;
 memset((char *)&sa,0,
#ifdef  USE_IPV6
                      sizeof(sa6));
 if(lhstname[0]=='[' )
 {

   IPv6Addr((ushort *) & sa6.sin6_addr,lhstname+1);
   sa.sin_family=AF_INET6;
   s=socket(AF_INET6,SOCK_STREAM,IPPROTO_TCP);

 }
 else
#else
       sizeof(sa));
#endif
 {
   if(anychars(lhstname))
   {
     MyLock(ip_cach_mtx);
     if( !(p=memchr4(ip_cach,n=MkName(lhstname),16) ) )
     {MyUnlock(ip_cach_mtx);
//   if(s_aflg&0x80000000)MyLock(hp_mtx);
//printf("%X lhstname",time(0));
      hp=gethostbyname(lhstname);
//printf("%X end get lhstname",time(0));
//   if(s_aflg&0x80000000)MyUnlock(hp_mtx);
     if(!hp)
     {
   lbErr:
       debug("Cannot get host by name %s",lhstname);
       return -1;
     }

     if( (a=*(ulong *)hp->h_addr) == 0x100007F)
     {
       debug("*** localhost  *** %u",hp->h_length); //,hp->h_addr_list[1]);
       if(!(hp=gethostbyname(lhstname)) ) goto lbErr;
     }
   lbQErr:
     MyLock(ip_cach_mtx);
     *(p=ip_cach+iip_cach)=n; iip_cach=(iip_cach+1)&15;
     p[16]=a; // *(ulong *)(hp->h_addr);
    }
    sa.sin_addr.s_addr=p[16];
    if( sa.sin_addr.s_addr == 0x100007F)
    {
       debug("***2 localhost  *** %u",p-iip_cach); //,hp->h_addr_list[1]);
       if((hp=gethostbyname(lhstname)) && (a=*(ulong *)hp->h_addr) != 0x100007F ) goto lbQErr;
    }

    MyUnlock(ip_cach_mtx);
   }else sa.sin_addr.s_addr=inet_addr(hhh=lhstname);
   sa.sin_family=AF_INET;
   if((s=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) <= 0)return -1;
 }
 sa.sin_port=htons((ushort)portnum);
 /*
 debug("s=%d %X %X %X [%X:%X:%X:%X:%X:%X:%X:%X]",s,sa.sin_port,sa6.sin6_port,sa.sin_family,
       ((ushort *) & sa6.sin6_addr)[0],
       ((ushort *) & sa6.sin6_addr)[1],
       ((ushort *) & sa6.sin6_addr)[2],
       ((ushort *) & sa6.sin6_addr)[3],
       ((ushort *) & sa6.sin6_addr)[4],
       ((ushort *) & sa6.sin6_addr)[5],
       ((ushort *) & sa6.sin6_addr)[6],
       ((ushort *) & sa6.sin6_addr)[7]

      );
 */
 if(connect(s,(struct sockaddr *)&sa,
#ifdef USE_IPV6
   (sa.sin_family==AF_INET6)?sizeof(sa6):
#endif
    sizeof(sa)) //!=0 )  //
        < 0)
 {debug("Call to connect to %s failed (%d) " SER ,lhstname, errno Xstrerror(errno));
//  closesocket(s);
   CloseSocket(s);
  return -1;
 }
 return s;
}
int call_up(char *lhstname, int portnum)
{int i,s,j;
 if((s=call_socket(up_proxy,up_proxy_port))>0)
 {char bb[512];
  i=sprintf(bb,"CONNECT %.64s:%u HTTP/1.0\r\nHost: %.64s:%u\r\n\r\n",lhstname,portnum,lhstname,portnum);
  if(up_user)
  {i+=sprintf(bb+i-2,"Proxy-Authorization: Basic ");
   if((j=strlen(up_user))>128)j=128;
   i=Encode64(bb+i-2,up_user,j)-bb;
   DWORD_PTR(bb[i])=0xA0D0A0D x4CHAR("\r\n\r\n");
   i+=4;
  }
  send(s,bb,i,0);
  GetCMD(s,bb,PRXTimeout);
  if(bb[9]!='2')
  {lbEx:
     //shutdown(s,2); closesocket(s);
     CloseSocket(s);
     return -1;
  }
  while(GetCMD(s,bb,PRXTimeout)>=0){if(!bb[0]) return s; }
  goto lbEx;
 }
 return s;
}
int call_socket2(char *lhstname, int portnum)
{return (up_proxy && (FL1_UPPRX&s_flgs[1]) )?call_up(lhstname,portnum):call_socket(lhstname,portnum);}

#endif

int XRecv(int s1,char *b,ulong l,int f,int tmout//=PRXTimeout
         )
{if(!s1){l=_hread(f,b,l);}
 else{if(RESelect(tmout,0,1,s1)<=0)return -1;
  l=recv(s1,b,l,0); }
 return l;
};
#define Xsend(a,b,c,d) send(a,b,c,0)
#define Xrecv XRecv
int YSend(Req* s,char *b,int l,int f)
{if(f>0) _hwrite(f,b,l);
 return s->Send(b,l);
}
#define Ysend(a,b,l,f)  YSend(this,b,l,f)

/*
void FlushIn(int s)
{ulong ll;
 int i;
 char  b[65];
 Sleep(10);
 while(ioctlsocket(s,FIONREAD,(ulong *)&ll)>=0 && ((int)ll)>0 && (ll<0x2000) ){
  while(ll>0)
  {i=64;
   if(i>ll)i=ll;
   i=recv(s,b,i,0);
   if(i<=0)return;
   ll-=i;
  }
  Sleep(10);
 }
}
*/
#define FlushIn(s)

int FGetCMD(int s,char *b)
{int r,i=0;
 while(( (!(r=GetCMD(s,b,0))) || (r>0 && b[3]=='-') ) && ++i<128 );
 return r;
}


void DelMime(char *t)
{char *p;
 int ii;
 for(p=t;*t;++t,++p)
   if(*t=='%' && (ii=atouix2(t+1))>=0x20 && t[2]>='0'){*p=ii; t+=2;}
   else *p=*t;
 *p=0;
}


void Req::p2p(int ss,char *in_buf)
{int i,h,l;
 i=ss;
 h=s;
#if 0

#ifndef SYSUNIX
 while(RESelect(timout,0,2,i,s) >0 )
 {if(RESelect(0,0,1,i))xchg(i,s);
#else
 while( (i=RESelect2(timout,0,h,ss)) >0 )
 {s=(i==h)?ss:h;
#endif
  if((l=recv(i,in_buf,0x8000,0))<=0)break;
  //if(send(h,in_buf,l,0)<=0)break;
  if(JustSend(in_buf,l)<=0)break;
  if(s==ss)
  {Tin+=l;
   Tout-=l;
  }
  DBG_WRITE(in_buf,l,h==ss);
 }
 s=h;
#else

 i=s;
 h=ss;
 
#if ! (defined(SYSUNIX) || defined(SELECT1) )  
 while(RESelect(timout,0,2,i,h) >0 )
 {if(RESelect(0,0,1,h))xchg(i,h);
#else
 while( (i=RESelect2(timout,0,s,ss)) >0 )
 {h=(i==s)?ss:s;
#endif
  if(i==ss)
  {
    if((l=recv(ss,in_buf,0x8000,0))<=0)break;
    if(Send(in_buf,l)<=0)break;
  }
  else
  {
    if((l=Recv(in_buf,0x8000))<=0)break;  
    if(BSend(ss,in_buf,l)<=0)break;
  }    
/*     
  if((l=recv(i,in_buf,0x8000,0))<=0)break;
  if(BSend(h,in_buf,l)<=0)break;
  ((h==ss)?Tin:Tout)+=l;
  DBG_WRITE(in_buf,l,h==ss);
*/  
 }


#endif
}

int ConvertUtf8(char *t)
{
  int r=0,i;
  char *p=t;
  while( (*p=*t) )
  {
    if( (*t&0xE0)==0xC0 && (t[1]&0xC0)==0x80 &&
          (i=((*t&0x1F)<<6) | (t[1]&0x3F)) >=1040 && i<1072
      )
    {
      *p=i-848;
      ++t;
      ++r;
    }
    t++;
    p++;
  }

  return r;
}

int IsHostFromList(char *t2,char *bad_hosts,char *t5)
{
 char *t3;
  for(;1<(u_long)t2 && *t2;t2=strchr(t2,'.')+1 )
  {for(t3=bad_hosts;(t3=stristr(t3,t2));++t3)
   {if( (t3==bad_hosts || ! (t3[-1]&~0x2C) ) && ! (t3[t5-t2]&~0x2C)  )
    return 1;
   }
  }
 return 0;
}

#if !defined(CD_VER)
char def_pwd[]="shs@";

int MkPName(char *url, char *t)
{
 char *p,*p1,*p2,*p3;
// int n=0;
 ulong nm,nm2=0,kl;
 char  uu[9];
 nm=MkName(url);

 url+=4;
// debug("*** %.32s...",url);
 if(*url=='/')++url;

//http://
  if( (p=strchr(url,'/')) )
  {
   nm2=MkName5(p+1);
   *p=0;
   p1=p-6;
   if(p1<url)p1=url;
   DWORD_PTR(uu[0])=DWORD_PTR(p1[0]);
   DWORD_PTR(uu[4])=DWORD_PTR(p1[4]);

//  debug("***p1= %s url=%s",p1,url);

   if( (p1=strrchr(url,'.')) )
   {//n|=2;
//   debug("***p1_= %s",p1);
    *p1=0;
    if( (p2=strrchr(url,'.')) )
    {
     if((p1-p2)<=3)
     {
      p3=p2;
      *p3=0;
      p2=strrchr(url,'.');
      *p3='.';
      if(!p2) goto lbUrl;
     }
     *p2++;
    }
    else
    {
   lbUrl:
     p2=url;
    }
    kl=
    sprintf(t,"%s"  FSLUSHS "%s"  FSLUSHS "%s"  FSLUSHS "%8.8X.asi"
        ,  (proxy_dir)?proxy_dir:"",uu,p2,nm);
    *p1='.';

    if(nm2)abname(t+kl-12,nm,nm2);
    // debug("name= %s",t);
 //   debug("***p2= %X t=%X uu=%X",p2,t,uu);
 //   debug("***p2= %s t=%s uu=%s",p2,t,uu);
   }
   else{ *p='/'; goto lb1;}
   *p='/';
  }
  else
  {
    lb1:
        sprintf(t,"%s"  FSLUSHS   "%8.8X.asi", (proxy_dir)?proxy_dir:"",nm);
 //   debug("N.F");
  }
 return nm;
}


ulong MkOldPath(char *t,char *s,ulong xu)
{
 ulong xu2=0;
 if(s_flgs[1]&FL1_BIGPROXY)
 {xu2=MkName2(s);
  sprintf(t,"%s" FSLUSHS "%2.2X"  FSLUSHS "%8.8X.asi",proxy_dir,xu2&0x3F,xu);
 }
 else
  sprintf(t,"%s" FSLUSHS "%8.8X.asi",proxy_dir,xu);
 return xu2;
}

void MkDirs(char *fname)
{
 char *p;
 for(p=fname+strlen(proxy_dir)+1 ; *p ; ++p)if(*p==FSLUSH)
 {*p=0;
  CreateDirectory(fname,0);
  *p=FSLUSH;
 }
}

int _creatd(char *fname)
{int i;
 if((i=_lcreat(fname,0))<=0)
 {MkDirs(fname);
  i=_lcreat(fname,0);
 }
 return i;
}

#define _lcreatd(a,b) _creatd(a)
struct virus_url
{
 virus_url *next;
 char url[1];

};
virus_url *virus_list;
//int virus_list_size;

int AvSockBeg(char *av,int port);
int AvSend(int s,char *b,ulong l);
int AvEnd(int s, char *url);


int AvSockBeg(char *av,int port)
{
 int s;
 if((s=call_socket(av,port))>0)
 {
  send(s,"nINSTREAM\n",sizeof("nINSTREAM\n")-1,0);
 }
 return s;
}
int AvSend(int s,char *b,ulong l)
{
 ulong ll;
 ll=htonl(l);
 send(s,(char *)&ll,4,0);
 send(s,b,l,0);
}

//const char AvCmdEnd[]=//"\x00\x00\x00\x00\n"
//"nSTATS\n";
int AvEnd(int s,char *url)
{
 ulong ll,tt;
 char  bb[512];
 ll=0;
 tt=GetTickCount();
 send(s,(char *)&ll,4,0);
 if(RESelect(30,30,1,s)>0)
 {
  ll=recv(s,bb,500,0);
  if(ll>0)
  {
    bb[ll]=0;
    debug("\r\nAV (%ums):%s",GetTickCount()-tt,bb);
    ll=0;
    if(stristr(bb,"FOUND"))
    {
      virus_url *v;
      /*
      char *p;
      if((p=strchr(url,' ')))
      {*p=0;
       ll=p-url;
      }
      else
      */
       ll=strlen(url);
      v=(virus_url *) ALLOC(ll+sizeof(virus_url)+4 );
      strcpy(v->url,url);
      v->next=virus_list;
      virus_list=v;
      ll=1;
    }
  }
 };
 /*
 send(s,AvCmdEnd,sizeof(AvCmdEnd)-1,0);

 if(RESelect(15,15,1,s)>0)
 {
  ll=recv(s,bb,500,0);
  if(ll>0)
  {
    bb[ll]=0;
    debug("Av:%u(%s)",ll,bb);
  }
 };
 */
 //shutdown(s,2);
 //closesocket(s);
  CloseSocket(s);
 return ll;
}



int CheckInVirusURL(ulong xu,char *url)
{
 if(virus_list)
 {
   char *p;
   virus_url *v;


   //if((p=strchr(url,' ')))*p=0;


   for(v=virus_list; v ; v=v->next)
   {
    if(v->url && ! stricmp(url,v->url) )
    {
     //if(p)*p=' ';
      debug("Virus URL: %s",url);
     return 1;
    }
   }

   //if(strstr(virus_list,url))return 1;
   //if(p)*p=' ';
 }
 return 0;
}

int Req::OutVirusList(char *bfr)
{
 int j,k;
 virus_url *v;
 if(!virus_list) return 0;
 j=sprintf(bfr,
   "<h2><a href=\"javascript:SHTbl('t%u');\"><span id=lt%x >[-]</span></a> %s: &nbsp; <a href=#l%X>\\/</a></h2>"
   "<table id=t%u bgcolor=#a0fcff border=1>"
   "<tr bgcolor=#b0cec8><td align=center><font size=3 class=f3><b>%s</b></font>"
   "</td></tr>" HTML_LN,20,20,"Detected viruses",20,20,"URL");

   k=1;
   for(v=virus_list; v ; v=v->next)
   {
    if(v->url)
    {
      j+=sprintf(bfr+j,
   "<tr valign=center><td><font size=2 class=f2>%u) %.128s"
   "</td></tr>"HTML_LN,
    k++, v->url );
     if(j>0x3000)
     {if(Send(bfr,j)<=0) return -1;
      j=0;
     }
    }
   }
  j+=sprintf(bfr+j,"</table><hr><a name=l%X></a>",20);
  return Send(bfr,j);
};

/*
int ProxyAntivirus(char *fname,char *url)
{
  int r,l;
  char *p;
  virus_url *v;
  char bb[512];

  sprintf(bb,"%s %s", proxy_antivirus,fname);
  debug("Antivirus check:%.64s..",url);

  r=system(bb);
  if(r==-1)
    debug("Can't run %s",bb);
  else if(r)
  {
    if((p=strchr(url,' ')))
    {*p=0;
      l=p-url;
    }
    else l=strlen(url);
    v=(virus_url *) ALLOC(l+sizeof(virus_url)+4 );
    strcpy(v->url,url);
    v->next=virus_list;
    virus_list=v;
    return 1;
  }
  return 0;
}

*/
int Req::ProxyReq()
{
 char *p,*in_buf,*ibf,*t,*t2,*t3,*t4,*t6,*u,
 *nocashe,*auth,*pp,*ModifiedSince,*fname;
 int l,h,i,rcode,pr,j;
 ulong xu;
 union {sockaddr prt; sockaddr_in prti;};
 Req *r;
struct{
 int ss,fs,fss,tin,tout,ss_ftp,tocashe,cl,post,xu2,xu3,exe_state,avs;
 ulong nm4; //,oldurl,oldurl1;
 User *puser;
 char *d,*t1,*t5;
 char *tend;
// CacheTag * csh;
 int new_st;
 int cont_st;
 int up_prox;
 char *cook;
}zz;
#define ss      zz.ss
#define ss_ftp  zz.ss_ftp
#define fs      zz.fs
#define fss     zz.fss
#define tin     zz.tin
#define tout    zz.tout
#define DelFlag zz.DelFlag
#define puser   zz.puser
#define d       zz.d
#define nm4     zz.nm4
#define csh     zz.csh
#define post    zz.post
#define tocashe zz.tocashe
#define cont_st zz.cont_st
#define t1 zz.t1
#define t5 zz.t5
#define xu2 zz.xu2
#define xu3 zz.xu3
#define up_prox zz.up_prox
 //LimitCntr *lip,*lnet;
 int ttimeout;

 ttimeout=PRXTimeout;
 /*
 if( FndLimit(2,&lip,&lnet,sa_c.sin_addr. S_ADDR) )
 {HttpReturnError("Limit overflow");
  return 1;
 }
 */
  timout=PRXTimeout;
  memset(&zz,0,sizeof(zz));
  http_var=(char **)((ibf=(in_buf=rq)+0x4000)+1024);
  if(!(nocashe=stristr(in_buf,/*"PRAGMA*/": NO-CACHE\r\n" )))
    nocashe=stristr(in_buf,"\nCACHE-CONTROL:");

  auth=StrVar(in_buf,"Authorization"); //stristr(in_buf,"\nAuthorization:" );
  ModifiedSince=StrVar(in_buf,"If-modified-since"); //stristr(in_buf,"\nIf-modified-since");
  if(FL1_PRXHRD&s_flgs[1])zz.cook=stristr(in_buf,"\nCookie");

  if( (t=stristr(in_buf,"\nProxy-Authorization:") )  )
  {
#ifdef x86_64
   DDWORD_PTR(ibf[1024]) =(u_long) "Authorization";
   DDWORD_PTR(ibf[1032])=(u_long) (t+sizeof("\nProxy-Authorization:"));
#else      
   DWORD_PTR(ibf[1024]) =(ulong) "Authorization";
   DWORD_PTR(ibf[1028])=(ulong) (t+sizeof("\nProxy-Authorization:"));
#endif   
   if((t1=CheckAuth(p=ibf+1048))) puser=FindUser(t1,UserHTTP,p,this);
   /*
   if((p=strchr(t+12,'\n')))
   {memcpy(t,p,dirlen-(p-in_buf));
    dirlen-=p-t;
   };
   */
   dirlen=DelStr(t+1,in_buf,dirlen);
  }
  if( (s_flg&FL_PRXUSER) && !puser)
  {KeepAlive=0;
#define STRAVTPR "HTTP/1.1 407 deny\r\nProxy-Authenticate: Basic realm=\"Proxy\"\r\nContent-Type: text/plain\r\n\r\nAccess denyed"
   return 
#ifdef WITHMD5
      (  ( (s_flgs[2] & FL2_MD5PASS) || ! (s_flgs[1]&FL1_CRYPTPWD) )  &&  (s_flgs[2] & FL2_USEMD5D) && !(fl&F_DIGET_UNAVILABLE) )?
      SendDigestAuthReq(in_buf): 
#endif   
      Send(STRAVTPR,sizeof(STRAVTPR)-1);
  }

  xu=(s_flgs[1]&FL1_BBPROXY)? MkPName(in_buf+6,ibf):
    MkName(in_buf+6);
  if(proxy_antivirus && CheckInVirusURL(xu,inf)) //in_buf+6))
  {
   KeepAlive=0;
  #define STRVIRUS "HTTP/1.1 400 deny\r\n\r\nContent-Type: text/html\r\n\r\n<h1>Virus. Access denyed</h1>"
   return Send(STRVIRUS,sizeof(STRVIRUS)-1);


  }
  if(cnt_same)
  {dir=0;
   l=0;
   do{
    h=0;
    ++no_close_req;
    for(i=0;i<max_tsk;++i)
      if( ((u_long)(r=rreq[i]))>1 &&
          (r->fl&0xF0000)==1 &&
          r->sa_c.sin_addr.s_addr==sa_c.sin_addr.s_addr &&
          DWORD_PTR(r->dir)==xu
        ) ++h;
    --no_close_req;
    if(h<cnt_same)break;
    if(RESelect(1,1,1,s) || ++l>PRXTimeout)return 0;
   }while(1);
  }
  DWORD_PTR(dir)=xu;
  if(proxy_dir)
  {fname=ibf;
   if(!(s_flgs[1]&FL1_BBPROXY))
   {xu2=MkOldPath(ibf,in_buf,xu);
    ++xu3;
   }
  }
  p=in_buf+(h=dirlen);
  //debug("--------%u |%s|",h,in_buf);
  if( !(t=StrVar(in_buf,"HOST") ) )
  {
   if((t=strstr(in_buf,"tp://"))) t+=5;
   else t=in_buf+8;
   //if( *t == ':' )t+=3;
   //if( t[-1] == ':' )t+=2;
   zz.tend=strpbrk(t,":/ ");
  }
  else if( (!(t5=strchr(t,'\r'))) || (ulong)(t5-t)>64ul ) goto errz2x;
  else *t5=0;

  if(IsHostFromList(t,bad_hosts,t5))
  {KeepAlive=0; return HttpReturnError("Access denied");}
  if(up_proxy && up_proxy[0] && !IsHostFromList(t,nohosts,t5) ) ++up_prox;
if(!up_prox)
{ pr=80;
  if(DWORD_PTR(in_buf[4])==0x3A707466 x4CHAR("ftp:") )
  {pr=21;
   ss_ftp=-1;
  // url--;
   if(strstr(in_buf,"/ "))goto lb_nocashe;
   KeepAlive=0;
  }
}
//* Cashe:
 if( (DWORD_PTR(*in_buf) == 0x20544547 x4CHAR("GET ") )
     && ( (i=t5-t)<255 )
     && (!ModifiedSince)
     && !zz.cook
   )
 {
// */ tocashe=0;

    if(proxy_dir)
    {
     if(!nocashe)
     {
    lpag:
      if((fs=_lopen(ibf,0)) >=0)
      {if( GetFileSize(
#ifndef x86_64          
          (HANDLE)
#endif          
          fs,0) )
       {goto oklxx;}
       _lclose(fs);
       fs=0;
      }
      else
      {
       if((s_flgs[1]&FL1_BBPROXY) && !xu3)
       { xu2=MkOldPath(ibf+512,in_buf,xu);
         ++xu3;
         MkDirs(ibf);
         if( MoveFile(ibf+512,ibf) ) goto lpag;
       }

       if(s_flgs[1]&FL1_BIGPROXY && xu2)
       {sprintf(ibf+512,"%s" FSLUSHS "%8.8X.asi",proxy_dir,xu);
        xu2=0;
        if( MoveFile(ibf+512,ibf) ) goto lpag;
       }
      }
     }
    }
 }

if(up_prox)
{t=up_proxy;
 pr=up_proxy_port;
 if(up_user)
 {t2=Encode64(ibf+=512,up_user,strlen(up_user));
  t3=in_buf;
  t4=in_buf-=sizeof("\r\nProxy-Authorization: Basic")+(t2-ibf);
  while(*t3!='\n' && *t3)*t4++=*t3++;
  i=sprintf(t4,"\nProxy-Authorization: Basic %s\r",ibf);
  t4[i]='\n';
  ibf-=512;
  h+=i;
 }
}

lb_nocashe:
  if(
     (*t=='[')? ( (t2=strstr(t,"]:") ) && ++t2 ) :
     (
      (t2=strchr(t,':')) || ((t2=strchr(in_buf+11,':')) &&
      t2<strchr(in_buf+11,'/') && (pr!=21 || t2>strchr(in_buf+11,'@') ) )
     )
    )
  {
   *t2=0;
   if((i=atoui(t2+1))==proxy_port)goto errz2x;
   if(i && !up_prox)pr=i;
  };
  i=MkName(t);
  if(zz.tend){rcode=*zz.tend; *zz.tend=0;}

  if( (ss=trn_sock) >0 )
  {if( (i==reqi) && !RESelect(0,0,1,ss))goto lbCn;
lbCls:
   //shutdown(ss,2); closesocket(ss);
   CloseSocket(ss);
  }
tryNext:
  trn=0;
  SleepSpeed();
  if((ss=call_socket(t,pr))>0)
  {trn_sock=ss;
   zz.new_st++;
   l=sizeof(prt);
   getsockname(ss,(sockaddr *)&prt,&l);
//   req_var=(char **)(prti.sin_addr.s_addr);
   req_vari=(prti.sin_addr.s_addr);
//   pst= (char*)(int) (prti.sin_port);
   psti= (prti.sin_port);
//   req=(char*)i;
   reqi=i;
lbCn:
   if(zz.tend)*zz.tend=rcode;
   if(t5)*t5='\r';
   if(t2)*t2=':';

if(!up_prox)
{  if(ss_ftp)
   {if((t6=strchr(in_buf,'@')))
    {*t6=0; u=t6+1;
     if( (t2=strchr(t3=in_buf+10,':')))
     {*t2=0; t4=t2+1;}
     else t4=def_pwd;
    }
    else {t3="anonymous";  t4=def_pwd; u=in_buf+10; }
    if(auth)
    {
#ifdef x86_64
   DDWORD_PTR(ibf[1024]) =(u_long) "Authorization";
   DDWORD_PTR(ibf[1032])=(u_long) auth;
#else      
     DWORD_PTR(ibf[1024]) =(ulong) "Authorization";
     DWORD_PTR(ibf[1028])=(ulong)auth;
#endif     
     t3=CheckAuth(t4=ibf+1048);
    }
    //do{
    if( (uint) (((ulong)FGetCMD(ss,p))/100)>2u)goto lerr3;
    //}while(p[3]=='-' || p[0]<'0' || p[0]>'9' );
    FlushIn(ss);
    tout+=send(ss,p,sprintf(p,"USER %s\r\n",t3),0);
    if((i=FGetCMD(ss,p))!=230)
    {if( i!=331 )goto lerr3;
     FlushIn(ss);
     tout+=send(ss,p,sprintf(p,"PASS %s\r\n",t4),0);
     //do{
      if( (uint) (FGetCMD(ss,p)/100)>2u)
      {//if(t4!=def_pwd)goto lerr3;
       Send(AuthErr, strlen(AuthErr)  ) ;//sizeof(AuthErr)-1);
       goto lerr4;
      }
     //}while(p[3]=='-');
    }
    if(p[3]=='-')Sleep(100);
    FlushIn(ss);
    tout+=send(ss,"TYPE I\r\n",sizeof("TYPE I\r\n")-1 ,0);
    FGetCMD(ss,p);
    if(t6){*t6='@'; if(t2)*t2=':';}

    if( !(t3=strchr(u,' ')))goto lerr3;
    *t3=0;
    if( !(t4=strchr(u,'/')))goto lerr3;

    //CheckBadName(t4);
    DelMime(t4);

utfAgain:
    FlushIn(ss);
    if( t3[-1] != '/' )
    { tout+=send(ss,p,sprintf(p,"SIZE %s\r\n",t4),0);
      if(FGetCMD(ss,p)==213)
      {for(t6=p+3; *t6==' ';++t6);
       zz.cl=atoui(t6);
      }
    }

    if(cont_st)
    {tout+=send(ss,p,sprintf(p,"REST %u\r\n",tin),0);
     if( ((ulong)FGetCMD(ss,p))>400) goto lerr3;
//     FlushIn(ss);
    }
    tout+=send(ss,"PASV\r\n",sizeof("PASV\r\n")-1 ,0);
    if( (FGetCMD(ss,p)!=227) || ! (t6=strchr(p,'(')) )goto lerr3;
    ++t6;
    prti.sin_addr.s_addr=ConvertIP(t6); ++t6;
    prti.sin_port =atouis(t6); ++t6;
    prti.sin_port |=atoui(t6)<<8;
//    debug("XXX2a %s",t4);
//    FlushIn(ss);

    tout+=send(ss,p,sprintf(p,(char *)((t3[-1] == '/' )?"LIST %s\r\n":"RETR %s\r\n"),t4),0);
//    debug("%s",p);
    if((ss_ftp=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) <= 0)goto lerr3;
    prti.sin_family=AF_INET;
    Sleep(20);
    if(connect(ss_ftp,&prt,sizeof(prt)) < 0 || (FGetCMD(ss,p)/100) != 1 ) {
      if( ConvertUtf8(t4) )
      {
        //shutdown(ss_ftp,2);
        //closesocket(ss_ftp);

        CloseSocket(s);
        goto utfAgain;
      }
      goto lerr3; }
    //if( (FGetCMD(ss,p)/100) != 1  // !=150)
    //  )goto lerr3;
    if(t3[-1] == '/')
    {OutDirTop();
     i=0;
     while( (i+=Xrecv(ss_ftp,in_buf+i,0x8000,fs,PRXTimeout))>0 )
     {tin+=i;
      in_buf[i]=0;  t4=PutFTPDir(in_buf,in_buf+i+20);
      if( (i-=t4-in_buf)>0)memcpy(in_buf,t4,i); else i=0;
      tin-=i;
     }
     OutDirBottom(in_buf);
     goto lerr4;
    }
    if(proxy_dir && (!auth) && fs<=0 )
     if(fss<=0 && ((!max_pfile) || zz.cl<max_pfile)) // || zz.exe_state) )
      if(cont_st)
      {fss=_lopen(fname,OF_WRITE);
       _llseek(fss,0,2);
      }else fss=_lcreatd(fname,0);
#define FTP_PROXY_REPLY  "HTTP/1.0 200\r\nContent-type: application/octet-stream\r\n\r\n"
#define FTP_PROXY_REPLY2 "HTTP/1.0 200\r\nContent-Length: %u\r\nContent-type: application/octet-stream\r\n\r\n"
    if(!cont_st)
    {Ysend(s,p,sprintf(p,(zz.cl)?FTP_PROXY_REPLY2:FTP_PROXY_REPLY,zz.cl ),fss );
     tin=0;
    }
    xchg(ss,ss_ftp); // tocashe=0;
    if( proxy_antivirus )
    {
     zz.exe_state=1;
     zz.avs=AvSockBeg(proxy_antivirus,proxy_antivirus_port);
    }
    goto  lbRe;
   }

   if((ibf=strchr(in_buf+12,'/') ) && ibf[4] != '\r'
      && ((in_buf[8] == ':' ) || (in_buf[9] == ':' ) ) )
   {
    if(in_buf[9] == ':' )
    {  //zz.oldurl1=ibf[-1];
      *--ibf=' ';}
    //zz.oldurl=DWORD_PTR( *(ibf-4) );
    DWORD_PTR( *(ibf-=4) )=DWORD_PTR(in_buf[0]); //C4CHAR(in_buf);
    h-=ibf-in_buf;
   }else ibf=in_buf;

   if((t4=stristr(ibf,"\nProxy-Connection:")))
   {h-=6; memcpy(t4+1,t4+7,h-(t4-ibf)); }

}else ibf=in_buf;
if(DWORD_PTR(*ibf)==0x4E4E4F43 x4CHAR("CONN") )
{
 if(!up_prox)JustSend("HTTP/1.1 200\r\n\r\n",sizeof("HTTP/1.1 200\r\n\r\n")-1);
 else
 {
//  l=dirlen;
//  xchg(i,h);
//  goto lb5;
  send(ss,in_buf,dirlen,0);
 }
 p2p(ss,in_buf);
 tin+=Tin;
 tout+=Tout;
 goto lerr4;
}
//debug("PP: %X %X %X",fl&F_GZ,zlibstate, DWORD_PTR(ibf[h-4]) );
   if((t4=StrVar(ibf,"Content-Length")) )post=atoui(t4)-postsize;
   else if( (!(fl&F_GZ)) && (s_flgs[1]&FL1_GZPRX) && zlibstate>0 && (t6=strstr(ibf,"\r\n\r\n")) ) // && DWORD_PTR(ibf[h-4])==0xA0D0A0D && ibf[h-5]!='\n'  )
   {
     h=sprintf(t6+2,"Accept-Encoding: gzip\r\n\r\n")+(t6-ibf)+2 ;

    //  h+=sprintf(ibf+h-2,"Accept-Encoding: gzip\r\n\r\n")-2;
    //debug("a*------%u |%s|",h,in_buf);

   }

   i=h;
   do{
    if( (i=Xsend(ss,ibf,i,0) )<0 )
    {lerr3:
     if(!cont_st)HttpReturnError("Error. Can\'t send request...");
     goto lerr4;
    }
    tout+=i;
    if(post<=0)break;
    post-=i=XRecv(s,ibf,4000,0,PRXTimeout);
   }while(i>0);
   /*
   if(oldurl1)
   {
     ibf[-1]=oldurl1;
     DWORD_PTR(ibf[-5]=oldurl;
   }
   else if(oldurl) DWORD_PTR(ibf[-4]=oldurl;
   */
  oklxx:;
   l=0;
   do{
     if((i=Xrecv(ss,p+l,5000-l,fs,PRXTimeout))<=0)
     {if(l){t2=p+l; if(cont_st)goto lerr4; break;};
// debug("Last error %d i=%d ss=%d s=%d fs=%d trn=%d",WSAGetLastError(),i,ss,s,fs,trn);
      if((!zz.new_st) && !t4){ if(t5)*t5=0; goto lbCls;}
      HttpReturnError("Error. Can\'t receive header...");
      goto lerr4;
     }
    tin+=i;
    l+=i; p[l]=0;
    if((t2=strstr(p,"\n\r\n")) )
    {
     if(t2[-1]!='\r' && t2[3]=='\r' ) ++t2;
     break;
    }
    if((t2=strstr(p,"\n\n"))){--t2; break;}
    if(l>=5000){t2=p+l; if(cont_st)goto lerr4;  break;}
   }while(1);


   rcode=atoui(p+9);

   i=t2[1];
   t2[1]=0;
   if(KeepAlive)
     KeepAlive=stristr(p,"Connection: Keep-Alive");
   AddToLog(p,ss,FmtShrtR);
// * Cashe
   t2[1]=i;

   if( (!(fl&F_GZ)) && // (s_flgs[1]&FL1_GZPRX) &&
       zlibstate>0 &&
       (t6=StrVar(p,"Content-Encoding")) &&
       (striin(t6,"gzip") || striin(t6,"deflate"))
     )
   {
//     debug("Unpack gzip..");
     if(IGZU())
     {
       if(KeepAlive)l=DelStr(KeepAlive,p,l);
       //l=DelStr(strstr(p,"Content-Encoding"),p,l);
       //if((t6=strstr(p,"Content-Length")) ) l=DelStr(t6,p,l);
       KeepAlive=0;
     }
//     else  debug("P_ERROR: %X %X %X",fl&F_GZ,zlibstate,t6 );

   }
//   debug("P: %X %X %X",fl&F_GZ,zlibstate,t6 );

   if(rcode!=206 && cont_st && !stristr(p,"\nContent-Range:") ) goto lerr4;
   if((t1=StrVar(p,"Content-Length")))
   {zz.cl=atoui(t1) ; //+(t2-p)+3;
    if(!zz.cl)zz.cl=-1;
 //   debug("zz.cl=%u t1=%s",zz.cl,t1);
   }

   if(zz.cl<=0)
   {
     if(KeepAlive)
     {
      l=DelStr(KeepAlive,p,l);
      if((t1=stristr(p,"Keep-Alive:") ))
      {l=DelStr(t1,p,l);
      }
      KeepAlive=0;
     }
     ttimeout=5;
   }

   if(proxy_antivirus &&
       (
        (s_flgs[2]&FL2_PAVALL) ||
        stristr(p,"\nContent-type: application") ||
        (t2 &&  t2[3]=='M' && t2[4]=='Z') ||
        (s_flgs[2]&FL2_PAVHTML &&  stristr(p,"\nContent-type: text/html") )
       )
     )
   {
    zz.exe_state=1;
    zz.avs=AvSockBeg(proxy_antivirus,proxy_antivirus_port);
   }

   if( rcode >= 300 ||  rcode == 302 ||
       ((!(s_flg&FL_IGNNOCH)) &&
         ( stristr(p,/*"PRAGMA*/": NO-CACHE\r\n" ) ||
       stristr(p,"\nCACHE-CONTROL: Private" ) ||
           (
             (FL1_PRXHRD&s_flgs[1]) &&
             ( //zz.cook ||
               stristr(p,"\nCACHE-CONTROL:")
             )
           )
       //                               */
         )
     ) )
   {tocashe=0; }
//   else tocashe=1;
/*
   else if(fs>0)
   {
     if( (t3=stristr(p,"\nCACHE-CONTROL: max-age=") ) )
     {

     }
   }
*/
   else
   if( //(tocashe //||zz.exe_state
       //) &&

       proxy_dir && (!auth) && /* ModifiedSince &&*/ rcode < 300 && fs<=0 )
   {
    if( ( //zz.exe_state ||
          !max_pfile) || zz.cl<max_pfile)
     if(cont_st && (!fss))
     {fss=_lopen(fname,OF_WRITE);
      _llseek(fss,0,2);
     }else if( (!fss) && rcode!=206) fss=_lcreatd(fname,0);
    if((s_flg&FL_IGNNOCH) && (t3=stristr(p,"\nEXPIRES:") ) )t3[1]='N';
   }


   t1=stristr(p,"\nContent-type:");
   t3=stristr(p,"Transfer-Encoding: chunked");
   t6=p;
   if(cont_st)
   {
     l-=t2+3-p;
     p=t2+3;
     if(zz.avs>0 && l>0)
     {
       AvSend(zz.avs,p,l);

     }
   }
   else if(zz.avs>0 && (i=l-3-(t2-p) )>0 )
   {
      AvSend(zz.avs,t2+3,l);
      if(zz.cl && l>=zz.cl)
      {
       j=AvEnd(zz.avs,inf);
       zz.avs=0;
       if(j>0)
       {
        Ysend(s,STRVIRUS,sizeof(STRVIRUS)-1,0);
        t2[4]=0;
        KeepAlive=0;
        goto lerr5;
       }

      }
   }

   if(l>0)if( Ysend(s,p,l,fss)<0){t2[4]=0; goto lerr5; }


   p=t6;

   //Tout+=l;
////////!!!////////

   if( (cont_st) || ! zz.cl ){zz.cl=0;  KeepAlive=0; }
// */
   //if(!cont_st)
   {
    tin-=(t2-p)+3;
   }
lbRe:
//   debug("***%u %u",tin,zz.cl);
   while( ( ( //(!KeepAlive) &&   //rcode!=304
             !zz.cl) || ((int)tin<zz.cl)) && (i=Xrecv(ss,p,0x2000,fs,(rcode<300 || zz.cl || t1)? ttimeout :3))>0 )
   {tin+=i;
    if(t3)ttimeout=12;
//    debug("!**%u %u",tin,zz.cl);
#ifdef MEMPRX
    if(tocashe>0){memcpy(pp,p,MIN(tocashe,i)); tocashe-=i; pp+=i;}
#endif
    if(zz.avs>0)
    {
     AvSend(zz.avs,p,i);
     if(zz.cl)
     {
       if(tin>=zz.cl)
       {
         j=AvEnd(zz.avs,inf);
         zz.avs=0;
         if(j>0)
         {
          KeepAlive=0;
          break ;
         }
       }
     }
    }
/*
    if(zz.exe_state && proxy_antivirus && fss>0 && ( zz.cl?tin>=zz.cl:( i<0x2000 && RESelect(0,0,1,ss)) ) )
    {
      if(ProxyAntivirus(fname,in_buf+6) )break;

    }
*/
    if(Ysend(s,p,i,fss)<0)
    {
#ifdef MEMPRX
     if(tocashe>0)tin+=Xrecv(ss,pp,tocashe,fs,PRXTimeout);
#endif
     cont_st=max_cont_st;
     KeepAlive=0;
     break;
    }
    //Tout+=i;
    if(ss)SleepSpeed(); //sa_c.sin_addr.s_addr);
   }
//debug("X %d cl=%d %d tin=%d %d L=%d\r\n",i,zz.cl,ss,tin,fs,WSAGetLastError());
   if( /* fs>0 && */ zz.cl >0 && tin<zz.cl && cont_st<max_cont_st)
   {*p=0;
     if(ss_ftp>0){ //shutdown(ss_ftp,2); closesocket(ss_ftp);
                   CloseSocket(ss_ftp); ss_ftp=0; }
     else h=dirlen+=sprintf(in_buf+dirlen-2,"Range: bytes=%u-\r\n\r\n",tin)-2;
     cont_st++;
     t2=0;
     if(t5)*t5=0;
     //tin=0;
     if(fs>0)
     {_lclose(fs);
      fs=0;
     }
     if(ss>0){  //shutdown(ss,2); closesocket(ss);
         CloseSocket(ss); ss=0;}
 debug("Broken. %u of %u loaded. Try continue...",tin,zz.cl);
     goto tryNext;
   }
 lurl:;
  }else{errz2x:;
    if(ModifiedSince) Send("HTTP/1.0 304\r\n\r\n",sizeof("HTTP/1.0 304\r\n\r\n")-1);
    else HttpReturnError("Error. Can\'t connect...");
lerr4:
    KeepAlive=0;
  };
lerr5:;
  if(ss_ftp>0){  //shutdown(ss_ftp,2); closesocket(ss_ftp);
     CloseSocket(ss_ftp);
  }
//  if(gz)((GZObj *)(gz))->End();
  GZEnd();
  if(fs>0)_lclose(fs);
  if(fss>0)
  {
   _lclose(fss);
   if(max_pfile && tin>max_pfile)DeleteFile(fname);
  }
  if(zz.avs>0)AvEnd(zz.avs,inf); //in_buf+6);

  sprintf(in_buf,"Proxy in:%u out:%u %s\r\n",Tout,tout,puser?puser->name:"");
  AddToLog(in_buf,s,FmtShort);
#undef d
/*
  if(lip && lnet)
  {i=(Tout+1023)>>10;
   lip->cnt+=i;
   lnet->cnt+=i;
   ipcnts[2].d[0].cnt+=i;
  }
*/
  if(ss>0)
  {if(!KeepAlive)
   { //shutdown(ss,2); closesocket(ss);
    CloseSocket(ss);
   // debug("Close socket %u %u %X",tin,zz.cl,t3);

     trn=0;
   }
  }
 // debug("After close %u %u %X s=%d KA:%X trn:%d",tin,zz.cl,t3,ss,KeepAlive,trn);

//  proxy_chk.CheckProxy();
  DEBUGVAR(tmout);
  return 0;
};

#undef Ysend
#undef Xsend
#undef Xrecv
#undef ss
#undef ss_ftp
#undef fs
#undef fss
#undef tin
#undef tout
#undef DelFlag
#undef puser
#undef d
#undef tocashe
#undef nm4
#undef csh
#undef post
#undef t1
#undef t5
#undef up_prox
#undef _lcreatd

#include "strvar.cpp"

#endif
