/*
 * Copyright (C) 1999-2023 Maksim Feoktistov.
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


#ifndef SYSUNIX
#include <windows.h>
#include <winsock.h>
#include "mstring.h"
#include "winpr.cpp"
#include "environ.cpp"

#define sprintf wsprintf

int SockFFlush(FILE *h,char *bfr,ulong i)
{return send((int)(h->_fileno),bfr,i,0);  }
int SockFRead(FILE *h,char *bfr,ulong i)
{return  recv((int)(h->_fileno),bfr,i,0); }
inline void SetSockFILE(FILE *h,int s)
{h->_fileno=(void *)s; h->Flush=SockFFlush; h->Read=SockFRead; }

WSADATA tmp;

char *argv[256];
int  argc;

#define FSLUSH "\\"
#define FSLUSHC '\\'

#define xfprintf fprintf
#define xfread   fread
#define xfwrite  fwrite
#define yfprintf fprintf
#define yfread   fread

#else

#define FSLUSH "/"
#define FSLUSHC '/'

#include <sys/types.h>

#include "mdef.h"
#include "mstring1.h"

#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include <sys/socket.h>
//#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <netinet/in.h>

#define SetSockFILE(h,s) h=fdopen(s,"w+");

typedef  struct tm  SYSTEMTIME;
inline void FileTimeToSystemTime(time_t *tx,SYSTEMTIME *tt)
{memcpy(tt,gmtime(tx),sizeof(*tt));}
inline void GetSystemTime(SYSTEMTIME *t){time_t x=time(0); memcpy(t,gmtime(&x),sizeof(*t));};
inline void GetLocalTime(SYSTEMTIME *t){time_t x=time(0); memcpy(t,localtime(&x),sizeof(*t));};

#define wSecond tm_sec
#define wDayOfWeek tm_wday
#define wDay tm_mday
#define wMonth tm_mon+1
#define wYear tm_year+1900
#define wHour tm_hour
#define wMinute tm_min

#define atoui atoi
#define SetCurrentDirectory chdir

#define CloseHandle  close
#define closesocket  close
#define ioctlsocket  ioctl
#define stristr   strcasestr
#define ExitProcess exit

char nbfr[0x3800];
#define xfprintf(a,b,c...)  ((s>0)?send(s,nbfr,sprintf(nbfr,b, ##c),0):fprintf(a,b, ##c))
#define xfread(a,b,c,d) ((s>0)?recv(s,a,c,0):fread(a,b,c,d))
#define xfwrite(a,b,c,d) ((s>0)?send(s,a,c,0):fwrite(a,b,c,d))
#define yfprintf(a,b,c...)  send(s,nbfr,sprintf(nbfr,b, ##c),0)
#define yfread(a,b,c,d)     recv(s,a,c,0)

#endif

#define dbg printf
#define dbg_ex printf
#define CPUTSN printf

const char *wkday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
const char *month[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

const char EMSG[]= "SndMsg -- sendmail emulator for Small HTTP server.\n"
"Command line:%s {Keys} [to@address]\n"
"Keys:\n"
"-t  -- read recipients from message\n"
"-o d:\\outbox\\foulder  -- Outbox subderectory in Small HTTP server or\n"
"-o smtp://smtp.address[:25] -- Out to SMTP server\n"
"-f from@address\n"
"-F \"full name\"\n"
"-s  -- save From lines in headers\n"
"-m filename  -- message file instead stdin\n"
"-a filename  -- attach binary file\n"
"\n(C) 2002-2007 by M.Feoktistov\n"
;

char fromm[128];
char fname[128];
char tomail[128];
char **too;
char boutbox[512];
char zbfr[0x11000];
char xbfr[0x110];
int  state=0;
char outbox_default[]="smtp://127.0.0.1";
char *outbox=outbox_default;
char *attach=0;
char bound[64]="\r\n----00000000-SNDMSG---";
char fnd1[]="\r\r\n";
FILE *fin;

#ifdef NOTINTEL

#define stristr strcasestr



#endif

inline uint D64X(uchar i)
{if(i=='+')return 62;
 if(i=='/')return 63;
 if(i<'0')return 64;
 if(i<='9')return i-'0' + 52;
 if(i<='Z')return i-'A';
 return i-'a' + 26;
};

inline int E64X(int i)
{if(i==63)return '/';
 if(i==62)return '+';
 if(i>=52)return '0'+(i-52);
 if(i>=26)return 'a'+(i-26);
 return 'A'+i;
};
inline int GetBit(uchar *t,uint i)
{return  ( (t[i>>3]) >> (7-(i&7)) ) &1;};



int anychars(const char *name)
{for(; *name; name++) if(((*name)|0x20)>='a' && ((*name)|0x20)<='z') return 1;
 return 0;
}


int on=1;
char url_ip[18];

int call_socket(const char *hostname, int portnum){
 struct sockaddr_in sa;
 struct hostent     *hp;
 int s;
#ifndef SYSUNIX
 if(!DWORD_PTR(tmp.wVersion)) WSAStartup(0x2,&tmp);
#endif
 memset((char *)&sa, 0, sizeof(sa));

 if (anychars(hostname)){
         if ((hp= gethostbyname(hostname)) == NULL) { /* do we know the host's */
           dbg("Cannot get host by name for %s\n",hostname);
           return(-1);
         }
    sa.sin_addr.s_addr= *(uint *)(hp->h_addr);
    sa.sin_family= hp->h_addrtype;
 }else{
    sa.sin_addr.s_addr= inet_addr(hostname);
    sa.sin_family=AF_INET; //hp->h_addrtype;
 }

 //sa.sin_family=AF_INET;
 sa.sin_port= htons((u_short)portnum);

 if ((s= socket(sa.sin_family,SOCK_STREAM,0)) < 0){
         dbg("call to socket failed\n");
         return(-1);
 }
 if (connect(s,(struct sockaddr *)&sa,sizeof(sa)) < 0) {
         shutdown(s, 2);
         dbg_ex("call to connect failed: IP=%X, %s \n",sa.sin_addr.s_addr,hostname);
         return(-1);
 }

 setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char *)&on,sizeof(on));

 strncpy(url_ip,inet_ntoa(sa.sin_addr),15);
 return(s);
}

//////

int ExtractAddr(char *b,char *s)
{
 char *p,*p1;
 if(!(p=strchr(s,'\r')))return 0;
 *p=0;
 if(!(p1=strchr(s,'<')))
  if(!(p1=strrchr(s,' ')))return 0;
 sprintf(b,"%.128s",++p1);
 if((p1=strchr(b,'>')))*p1=0;
 *p='\r';
 return 1;
}
int GetCmdPar(char *b,char *&s)
{
 int c;
 char *p;
 while(*s==' ')++s;
 c=' ';
 if(*s==0)return 0;
 if(*s=='\"')
 {
  ++s;
  c='\"';
 }
 if((p=strchr(s,c))) *p=0;
 sprintf(b,"%.128s",s);
 if(p){
  *p=c;
  s=++p;
 }
 return 1;
};
int CRLF(char *t,int i)
{
 int j,k;
 char *e,*p;
 j=0;
 e=t+i;
 for(p=t;p<e;++p)if(*p=='\n' && p[-1]!='\r') ++j;
 for(p=e+j;e>t;){if( (*p--=*e--)=='\n' && *e!='\r'  )*p--='\r';}
 return i+j;
};

FILE F1;
int  SendMsg()
{
 SYSTEMTIME  stime;
 int i,j;
 int s=0;
 char *p,*t,**pp;
 FILE *h=&F1;
 FILE *fa;
  j=0;
  do{
    if(j>=0x8000 || (i=fread(zbfr+j,1,0x8000-j,fin))<=0)
    {
     fprintf(stderr,"*Error:  From: or To: fields not found %X %.56s %.56s %d %d\n",state,fromm,tomail,i,j);
     return -1;
    }
    zbfr[j+=i]=0;
    j=CRLF(zbfr,j);
    if((!(state&1)) && (p=stristr(zbfr,"\nFrom:")) && ExtractAddr(fromm,p+6) )state|=1;
    if((!(state&2)) && (p=stristr(zbfr,"\nTo:")) && ExtractAddr(tomail,p+4) )state|=2;
  }while( (state&3)!=3);

 GetSystemTime(&stime);
 if( DWORD_PTR(outbox[2])== C4CHAR("tp:/") )
 {
  outbox+=7;
  i=25;
  if((t=strchr(outbox,':')))
  { *t++=0;
    i=atoi(t);
  }
  if((s=call_socket(outbox,i))<=0)
  {
    fprintf(stderr,"*Error: Can't connect to SMTP\n");
    return -1;
  }
#ifndef SYSUNIX
  SetSockFILE(h,s);
#endif
  yfread(xbfr,1,256,h);
  if( (i=atoui(xbfr))>=400 || i<200)
  {errS: shutdown(s,2); closesocket(s);
   printf("*Error:  SMTP error\n");
   return -1;
  }
  yfprintf(h, "HELO sndmsg\r\n"); yfread(xbfr,1,256,h);
  yfprintf(h, "SEND from <%s>\r\n",fromm); yfread(xbfr,1,256,h);
  yfprintf(h, "RCPT to <%s>\r\n",tomail); yfread(xbfr,1,256,h);
  if(too)
   for(pp=too;*pp;pp++)
   {
    if(strchr(*pp,'@'))
    {
     yfprintf(h, "RCPT to <%s>\r\n",*pp); yfread(xbfr,1,256,h);
    }
   }
  yfprintf(h, "DATA\r\n"); yfread(xbfr,1,256,h);
  if(atoui(xbfr)!=354)goto errS;
  yfprintf(h, "Received: ");
 }else
 {
  i=(
   (( ( ( ( ( ( (stime.wYear-2001)*12+stime.wMonth
              )*31+stime.wDay
            )*24
          )+stime.wHour
        )*60+stime.wMinute
      )*60+stime.wSecond
    )<<4
   )+(
#ifdef SYSUNIX
     getpid()&0xF
#else
    stime.wMilliseconds/63
#endif
   )
  )^0x80000000;
  do{
   sprintf(p=zbfr+j,"%s" FSLUSH  "%8.8X.msg",outbox,i);
   --i;
  }while(
#ifdef SYSUNIX
     stat(p,(struct stat *)xbfr)>=0
#else
     GetFileAttributes(p)!=0xFFFFFFFF
#endif
     );
  if(!(h=fopen(p,"w")))
  {
    fprintf(stderr,"*Error: outbox foulder not found\n");
    return -1;
  };
 }
  xfprintf(h,"From %s (via SndMsg [%s]) %3.3s, %u %3.3s %4.4u %2.2u:%2.2u:%2.2u GMT For %s\r\n"
          ,fromm, (t=getenv("REMOTE_ADDR"))?t:"",
          wkday[stime.wDayOfWeek],
          stime.wDay,
          month[stime.wMonth-1],
          stime.wYear,
          stime.wHour,
          stime.wMinute,
          stime.wSecond,
          tomail
         );
   if(state&0x20)
     xfprintf(h,"From: %s <%s>\r\n"
          ,fname,fromm
         );
   if(!stristr(zbfr,"Date: "))
     xfprintf(h,"Date: %3.3s, %u %3.3s %4.4u %2.2u:%2.2u:%2.2u GMT\r\n",
      wkday[stime.wDayOfWeek],stime.wDay,month[stime.wMonth-1],stime.
      wYear,stime.wHour,stime.wMinute,stime.wSecond);
   if(attach)
   {if(!(t=stristr(zbfr,"\r\n\r\n")))
    {xfprintf(h, "Content-Type: multipart/mixed; boundary=\"%s\"\r\n\r\n\r\n%s\r\nContent-Type: text/plain\r\n\r\n",bound+4,bound);
    }
    else if( (p=stristr(zbfr," boundary=")) && p<t )
    {
     if(p[11]=='\"')
     { ++p; fnd1[0]='\"';}
     memcpy(bound+2,p+10,60);
     if((p=strpbrk(bound+2,fnd1)))*p=0;
    }
    else
    {if((p=stristr(zbfr,"\r\nContent-Type:")))
     {i=p+2-zbfr;
      xfwrite(zbfr,1,i,h);
      xfprintf(h, "Content-Type: multipart/mixed; boundary=\"%s\"\r\n\r\n\r\n%s\r\n",bound+4,bound+2);
     }
     else
     {i=t+2-zbfr;
      xfwrite(zbfr,1,i,h);
      xfprintf(h, "Content-Type: multipart/mixed; boundary=\"%s\"\r\n\r\n\r\n%s\r\nContent-Type: text/plain\r\n\r\n",bound+4,bound);
      i+=2;
     }
     if((j-=i)>0)
     {memcpy(zbfr,zbfr+i,j); }
     else j=0;
    }
   }
   do{
    if(j)
    {j=CRLF(zbfr,j);
     xfwrite(zbfr,1,j,h);
     zbfr[j]=0;
    }
   }while((j=fread(zbfr,1,0x8000,fin))>0);
   if(attach)
   {if(!strstr(zbfr,bound)) xfprintf(h,"%s\r\n",bound);
    if(!(t=strrchr(attach,FSLUSHC) ) )t=attach;
    else ++t;
    xfprintf(h,
       "Content-Type: application/octet-stream\r\n"
       "Content-Transfer-Encoding: base64\r\n"
       "Content-Disposition: attachment; filename=\"%s\"\r\n\r\n",t);
    if((fa=fopen(attach,"rb")))
    {
     int k,ii,cnt;
     uchar xs[18];
     t=zbfr;
     ii=0;
     while((i=fread(xs,1,12,fa))>0)
     { cnt=(i*8);
       for(i=0;i<cnt;)
       { for(j=k=0;k<6;k++,i++){  j<<=1;  j|=GetBit((uchar *)xs,i);};
         *t++= E64X(j);
         if(!((++ii)%80))
         {*t++='\r'; *t++='\n';
          if(ii>0x8000)
          { xfwrite(zbfr,1,t-zbfr,h);
            t=zbfr;
            ii=0;
          }
         };
       }
     }
     j=cnt%6;
     while(j--){*t++='='; }
     if(t>zbfr)xfwrite(zbfr,1,t-zbfr,h);
    }
    xfprintf(h,"%s\r\n",bound);
   }
   if(s>0)
   {
    yfprintf(h, "\r\n.\r\n"); yfread(zbfr,1,256,h);
    yfprintf(h, "QUIT\r\n");
    shutdown(s,2);
    closesocket(s);
   }
   else fclose(h);
   return 0;
};
//---
#ifndef SYSUNIX
int PASCAL WinMain( HANDLE hinst, HANDLE prev_inst, LPSTR cmdline, int cmdshow )
{
 int i,j;
 char *p,**pp,**em;
 p=cmdline;
 argc=0;
 goto  ll1;
 while(*p){
  if( *p!=' ' )
  {
   if(!(p[-1]))
   {
   ll1:
    argv[argc++]=p;
    if(argc>=255)break;
   }
  }
  else *p=0;
  *p++;
 };
 argv[argc]=0;
#else

int main(int argc,char *argv[])
{int i,j;
 char *p;
 char **pp,**em;
#define cmdline argv[0]
#endif
 em=0;
 fin=stdin;
 for(pp=argv+1;(p=*pp)  ;++pp)
 {if(*p=='-')
  {state|=0x4;
   switch(*++p)
   {
    case 't': state|=8;  break;
    case 's': state|=0x20; break;
    case 'F': //GetCmdPar(fname,++p);
             if(pp[1])  strcpy(fname,*++pp);
             break;
    case 'f': //if(GetCmdPar(fromm,++p))state|=1;
             if(pp[1])
             {strcpy(fromm,*++pp);  state|=1;
             }
             break;
    case 'm':
             if(pp[1])
             {
              if(! (fin=fopen(*++pp,"rb")))
              {fprintf(stderr,"**Error. Can't open %s\n",*pp);
               exit(0);
              };
             }
             break;
    case 'o': //if(GetCmdPar(boutbox,++p))
             if(pp[1])
             {strcpy(boutbox,*++pp);
              state|=0x10; outbox=boutbox;
             }
             break;
    case 'a': //if(GetCmdPar(boutbox,++p))
             if(pp[1]) attach=*++pp;
             break;
   }
  }
  else
  {
   if(!em)em=pp;
  }
 }
 if(em)
 {pp=em;
  p=*em;
 }
 if( ! (state&8) )
 {
  if(
//      (! (p=strrchr(cmdline,' ') ) )
      (!p)
//     || (!GetCmdPar(tomail,p))
//     || !strchr(tomail,'@')
     || !strchr(p,'@')
    )
  {
    fprintf(stderr,EMSG,argv[0]);
    //if(p)printf("--- |%s| %X --",p,state);
    //CPUTSN(EMSG,stderr);
    return -1;
  }else state|=2;
  strcpy(tomail,p);
  ++pp;
 }
 if(*pp)too=pp;
 if(! (state&0x10) )
 {
  GetCmdPar(zbfr,cmdline);
  if((p=strrchr(zbfr,FSLUSHC)))
  {
   *p=0;
   SetCurrentDirectory(zbfr);
  };
 }
 SendMsg();
// printf("Send Ok\n");
#ifndef SYSUNIX
 if(DWORD_PTR(tmp.wVersion)) WSACleanup();
// printf("Clean Up\n");
#endif
 ExitProcess(0);
 return 0;
}
