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


#ifdef A_64
static int StatMutex;
static char *statbasepoint;
#endif


#ifndef CD_VER
//!->21/12 22:42:29 [1.1.0.1:1>3] (t1 1)
/*
#ifndef PACKED
#define PACKED __attribute__((packed))
#endif
*/

struct StatLog
{
#ifdef A_64
 // union{
 // int next_offs;
 // StatLog *next;   //0
 // };
 // StatLog *nexto(char *b){return next_offs?(StatLog *)(b+next_offs):0; };   //0
 // StatLog *Next(){return  nexto(statbasepoint); }
    int size;
    StatLog *Next(){return (size)? (StatLog *) ( ((char *)this) + size ) : 0 ; }
   #define nexto(a) Next()
#else
 StatLog *next;   //0
 StatLog *Next(){return  next;}
#endif
 ulong time;      //4
 union{ulong ip; uchar cip[4];}; //8
 ulong stt;              //12
 ushort port;            //16
 ulong  hsh;             //18
 ushort ip6[8];
 void Parse();           //22
 int Addr(char *bfr);
 int NetAddr(char *bfr);
 char *find(char *what){return strstr( ((char *)this)+sizeof(StatLog),what ); }
} PACKED;



void StatLog::Parse()
{ char *p,*pp;
 int i1,i2,i3,i4,i5,i6;
 ushort ipv6[12];
 ulong lip;
 p=(( char *) this)+5;
 i1=atouis(p); ++p;
 i2=atouis(p); ++p;
 i3=atouis(p); ++p;
 i4=atouis(p); ++p;
 i5=atouis(p); ++p; ++p;
 if((pp=strpbrk(p,".:")) && *pp=='.')
   lip=ConvertIP(p);
 else{
  p=IPv6Addr(ipv6,p);
  lip=(ulong)-1;
  ++p;
 }

 if((p[1]=='>')||(p[1]==':') )p++; else
 p=strchr(p,'>');
 if(p) i6=atoui(++p);
 time=((((i1*31+i2)*24)+i3)*60+i4)*60+i5;
 ip=lip;
 port=i6;
 stt=0;
 hsh=0;
 if(lip==0xFFFFffff)memcpy(ip6,ipv6,16);
};

int StatLog::Addr(char *bfr)
{
 return (ip!=0xFFFFFFFF)?
          sprintf(bfr,"%u.%u.%u.%u",cip[0],cip[1],cip[2],cip[3]):
          sprintf(bfr,"%X:%X:%X:%X:%X:%X:%X:%X",
                   htons(ip6[0]),htons(ip6[1]),htons(ip6[2]),htons(ip6[3])
                  ,htons(ip6[4]),htons(ip6[5]),htons(ip6[6]),htons(ip6[7]) );
}

int StatLog::NetAddr(char *bfr)
{
 return (ip!=0xFFFFFFFF)?
          sprintf(bfr,"%u.%u.%u.%u",cip[0],cip[1],(cip[0]<191)?0:cip[2],0):
          sprintf(bfr,"%X:%X:%X:%X:%X:%X:0:0",
                   htons(ip6[0]),htons(ip6[1]),htons(ip6[2]),htons(ip6[3])
                  ,htons(ip6[4]),htons(ip6[5]) );
}

char* TmpStr(char *t,char &tc)
{if( (t=strpbrk(t," \"\'\t\n\r*?;")))
 {tc=*t; *t=0;}
 return t;
};
char* TmpStr2(char *t,char &tc,char *fnd)
{if( (t=strpbrk(t,fnd))){tc=*t;  *t=0;}
 return t;
};

char* fFindSubStr(StatLog *psl,char *bfr,char *s)
{char *t,*t2,t2c,*t3,*t4,t4c,*p;
 if( !(t=stristr(((char*)psl)+sizeof(StatLog),s)))return 0;
 t+=strlen(s);
 if((t2=TmpStr(t,t2c)))
 {strncpy(bfr,t,80);
  DWORD_PTR(bfr[80])=0x2E2E;
  *t2=t2c;
 }
 return t2;
};

//uint IfFindSubStr(StatLog *psl,char *bfr,char *s){return fFindSubStr(psl,bfr,s)&0x7FFFffff ; }
#ifndef A_64
#define IfFindSubStr (uint) fFindSubStr
#else
inline int IfFindSubStr(StatLog *psl,char *bfr,char *s){char *t=fFindSubStr(psl,bfr,s); if(t)return t-statbasepoint; return 0;  }
#endif



char* fFindSubStr2(StatLog *psl,char *bfr,char *s, char *brk)
{char *t,*t2,t2c,*t3,*t4,t4c,*p;
 if( !(t=strstr(((char*)psl)+sizeof(StatLog),s)))return 0;
 t+=strlen(s);
 if((t2=TmpStr2(t,t2c,brk))){
  strncpy(bfr,t,80);
  DWORD_PTR(bfr[80])=0x2E2E;
  *t2=t2c;
 }
 return t2;
};


//uint IfFindSubStr2(StatLog *psl,char *bfr,char *s, char *brk){return IfFindSubStr(psl,bfr,s,brk)&0x7FFFffff ; }
#ifndef A_64
#define IfFindSubStr2 (uint) fFindSubStr2
#else
inline int IfFindSubStr2(StatLog *psl,char *bfr,char *s, char *brk){char *t=fFindSubStr2(psl,bfr,s,brk); if(t)return t-statbasepoint; return 0;  }
#endif

int GetFuncType(tfFind f);

#include "cstat.cpp"


uint fFindProxyInAll(StatLog *psl,char *bfr,int n)
{char *t,*t1,*t2,j,*txt;
 ulong i,prt,prt2,k;
 prt2=prt=proxy_port;
 txt="Proxy in:";
 if(n&0x10){prt=http_port; prt2=ssl_port; txt="HTTP  in:"; n&=~0x10;}

 if( (psl->port==prt || psl->port==prt2) && (t=strstr(((char*)psl)+sizeof(StatLog),txt) ) &&
    (t1=strstr(t,"out:")) )
 {if(n&0x20)
  {if(!(t2=strstr(t1," Time:")))return 0;
   k=atoui(t2+sizeof("Time:"));
  }
  i=atouis( (t1+=sizeof("out:")-1));
  if(n&4)
  {n&=0x63;
   t2=TmpStr(++t1,j);
   strncpy(bfr,t1,80);
   *t2=j;
   if(strin(bfr,"Time:") )*bfr=0;
  }
  else psl->Addr(bfr);
   //sprintf(bfr,"%u.%u.%u.%u",psl->cip[0],psl->cip[1],psl->cip[2],psl->cip[3]);
  if(n==2){if(i) return i; // |0x80000000;
  }
  else if(n&0x40)
  { if(k<3 || !i)return 0;
     return (
             ((i+atoui(t+sizeof("Proxy in:")-1)
             )<<10)/k
            ) //|0x80000000
            ;}
  else if(n&0x20) return k; //|0x80000000;
  else if(n+i) return atoui(t+sizeof("Proxy in:")-1); //|0x80000000;
 }
 return 0;
}

uint fFindProxyIn(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,1);}
uint fFindProxyInNC(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,0);}
uint fFindProxyOut(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,2);}
uint fFindProxyInU(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,5);}
uint fFindProxyInUNC(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,4);}
uint fFindProxyOutU(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,6);}

uint fFindHTTPIn(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,0x11);}
uint fFindHTTPOut(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,0x12);}
uint fFindHTTPInU(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,0x15);}
uint fFindHTTPOutU(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,0x16);}
uint fFindHTTPTime(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,0x31);}
uint fFindHTTPTimeU(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,0x35);}
uint fFindHTTPSpeed(StatLog *psl,char *bfr){return fFindProxyInAll(psl,bfr,0x73);}

uint fFindHTTPHour(StatLog *psl,char *bfr)
{if( (psl->port==http_port || psl->port==ssl_port) && !strstr(((char*)psl)+sizeof(StatLog),"HTTP  in:"))
 {sprintf(bfr,"%2.2u",(psl->time/3600)%24);
  return 1;
 }
 return 0;
}


uint fFindPortUser(StatLog *psl,char *bfr,int xport)
{if(xport==psl->port) return psl->Addr(bfr);
  // sprintf(bfr,"%u.%u.%u.%u", psl->cip[0],psl->cip[1],psl->cip[2],psl->cip[3]);
 return 0;
}

uint fFindHTTPUser(StatLog *psl,char *bfr){return fFindPortUser(psl,bfr,http_port);}
uint fFindHTTPUNet(StatLog *psl,char *bfr){ return (http_port==psl->port)?psl->NetAddr(bfr):0;}

#if V_FULL
uint fFindProxyUser(StatLog *psl,char *bfr){return fFindPortUser(psl,bfr,proxy_port);}
uint fFindSMTPUser(StatLog *psl,char *bfr){return fFindPortUser(psl,bfr,smtp_port);}
uint fFindFTPUser(StatLog *psl,char *bfr){return fFindPortUser(psl,bfr,ftp_port);}
uint fFindPOPUser(StatLog *psl,char *bfr){return fFindPortUser(psl,bfr,pop_port);}
uint fFindSSLUser(StatLog *psl,char *bfr){return fFindPortUser(psl,bfr,ssl_port);}
uint fFindDNSUser(StatLog *psl,char *bfr){return fFindPortUser(psl,bfr,53);}
uint fFindDNSHost(StatLog *psl,char *bfr){if(53==psl->port) return IfFindSubStr(psl,bfr,") >"); return 0;}
#endif

uint fFindEventSumary(StatLog *psl,char *bfr)
{char *t,t2;
 if( (t=strstr(((char*)psl)+sizeof(StatLog)," in:")
     ) &&
      t<strchr(((char*)psl)+sizeof(StatLog),'\n') ) return 0;
 return sprintf(bfr,"%u",psl->port);
}
uint fFindErrors(StatLog *psl,char *bfr)
{char *t,*t2,t2c;
// if( !(t=strstr(((char*)psl)+sizeof(StatLog),"\r\n" )))return 0;
// t+=2; if(*t==FSLUSH && t!=(char*)(psl->Next()) && (t2=TmpStr(t,t2c)))
 if( strstr(((char*)psl)+sizeof(StatLog),"\r\n" ) &&
    (t=strstr(((char*)psl)+sizeof(StatLog),") >" FSLUSHS )) &&
    (t2=TmpStr(t+=3,t2c))
   )
 {
  strcpy(bfr,t);
 *t2=t2c;
  return 1;
 }
 return 0;
};
#if V_FULL
uint fFindFTPdownload(StatLog *psl,char *bfr){return IfFindSubStr2(psl,bfr,"\r\nFTP: download ","\t\r\n"); };
uint fFindFTPupload(StatLog *psl,char *bfr){return IfFindSubStr2(psl,bfr,"\r\nFTP: upload ","\t\r\n"); };
uint fFindFTPLogin(StatLog *psl,char *bfr){return IfFindSubStr(psl,bfr,"\r\nFTP: login in "); };
uint fFindPOPLogin(StatLog *psl,char *bfr){return IfFindSubStr(psl,bfr,"\r\nPOP: login in "); };

uint fFindSMTPx(StatLog *psl,char *bfr,char *s)
{char *t;
 if((psl->port==smtp_port) && fFindSubStr(psl,bfr,s) && *bfr &&
  (t=strstr( ((char*)psl)+sizeof(StatLog),"\r\nSMTP:"))
 )return atoui(t+sizeof("\r\nSMTP:")); //|0x80000000;
 return 0;
};
uint fFindSMTPs(StatLog *psl,char *bfr){return fFindSMTPx(psl,bfr," user>"); }
uint fFindSMTPr(StatLog *psl,char *bfr){return fFindSMTPx(psl,bfr," user<"); }

uint fFindSMTPxIP(StatLog *psl,char *bfr)
{int r;
 if((psl->port==smtp_port) && fFindSubStr(psl,bfr,"\r\nSMTP: ") )
 {r=atoui(bfr); //|0x80000000;
  psl->Addr(bfr);
  //sprintf(bfr,"%u.%u.%u.%u",psl->cip[0],psl->cip[1],psl->cip[2],psl->cip[3]);
  return r;
 }
 return 0;
};

uint fFindPOPx(StatLog *psl,char *bfr)
{char *t;
 if((psl->port==pop_port) && fFindSubStr(psl,bfr,"bytes for ") &&
    (t=strstr( ((char*)psl)+sizeof(StatLog),"+OK"))
 )return atoui(t+sizeof("+OK")); //|0x80000000;
 return 0;
};

uint fFindFTPx(StatLog *psl,char *bfr, char *str)
{char *t;
 if((psl->port==ftp_port) && (t=(char*)fFindSubStr(psl,bfr,str))
 )return  atoui(t+1); //|0x80000000;
 return 0;
};
uint fFindFTPdownloadU(StatLog *psl,char *bfr){return fFindFTPx(psl,bfr,"\tfor "); };
uint fFindFTPuploadU(StatLog *psl,char *bfr){return fFindFTPx(psl,bfr,"\tfrom "); };


#endif
uint fFindUserAgent(StatLog *psl,char *bfr){return IfFindSubStr2(psl,bfr,"\r\nUser-Agent: ","\r\n"); };
uint fFindHost(StatLog *psl,char *bfr){return IfFindSubStr(psl,bfr,"\r\nHost: "); };

uint fFindURL(StatLog *psl,char *bfr)
{char *p;
 if( fFindHost(psl,bfr) ){bfr+=strlen(bfr); }
 if(!fFindSubStr(psl,bfr,"\r\nGET ") )
 if(!fFindSubStr(psl,bfr,"\r\nPOST ") )
 if(!fFindSubStr(psl,bfr,"\r\nHEAD ") )return 0;
 if(DWORD_PTR(bfr[3]) == 0x2F2F3A70 x4CHAR("p://") )
 {if(!(p=strchr(bfr+8,'/')))DWORD_PTR(*bfr)='/';
  else strcpy(bfr,p);

}
return
#ifdef A_64
      bfr - statbasepoint;
#else
   (uint)bfr;
#endif

};
uint fFindURLHTTP(StatLog *psl,char *bfr){return (psl->port==http_port || psl->port==ssl_port)?fFindURL(psl,bfr):0;}
uint fFindURLProxy(StatLog *psl,char *bfr){return (psl->port==proxy_port)?fFindURL(psl,bfr):0;}
uint fFindRef(StatLog *psl,char *bfr)
{if( fFindSubStr(psl,bfr,"\r\nReferer: ") )
 {strcpy(bfr+=strlen(bfr)," =&gt; ");
  return fFindURL(psl,bfr+7);
 }
 return 0;
};
uint fFindRefHTTP(StatLog *psl,char *bfr){return (psl->port==http_port || psl->port==ssl_port )?fFindRef(psl,bfr):0;}
uint fFindForegRef(StatLog *psl,char *bfr)
{if( (psl->port==http_port || psl->port==ssl_port ) &&  fFindSubStr(psl,bfr,"\r\nReferer: ") )
 {
  host_dir *a;
  char *t;
  int l;
  for(t=bfr+8; *t && *t!='/' ; t++)
   if(*t>'9')
   {
    for(a=hsdr.next;a;a=a->next)
     if(a->h[0]!='/')
     {
         l=strlen(a->h);
         if( l>3 && (t=stristr(bfr,a->h)) && t[-3]==':' &&
             ( (! t[l]) || strchr("\r\n \t/",t[l]) )
         ) return 0;
     }
    strcpy(bfr+=strlen(bfr)," =&gt; ");
    return fFindURL(psl,bfr+7);
   }
 }
 return 0;
};

uint fFindRefProxy(StatLog *psl,char *bfr){return (psl->port==proxy_port)?fFindRef(psl,bfr):0;}
uint fFindRefHTTPHst(StatLog *psl,char *bfr)
{char *t;
 if( (psl->port==http_port || psl->port==ssl_port) && fFindSubStr(psl,bfr,"\r\nReferer: ") )
 {if((t=strchr(bfr+8,'/')))*t=0;

  return
#ifdef A_64
      bfr - statbasepoint;
#else
   (uint)bfr;
#endif
 }
 return 0;
}


#undef send
#define send(a,b,c,d) Send(b,c)
#define isStatContry 2
#ifdef SEPLOG
#define   logHTTP   0
#define   logProxy  0x10
#define   logFTP    0x20
#define   logSMTP   0x30
#define   logPOP3   0x40
#define   logTLS    0x50
#define   logDNS    0x70

#else

#define   logHTTP   0
#define   logProxy  0
#define   logFTP    0
#define   logSMTP   0
#define   logPOP3   0
#define   logTLS    0
#define   logDNS    0

#endif


char strKb[]="K";
void Req::OutStatTblBody(StatLog *psl,tfFind fnd,  //char *bfr,
    BFILE *bf,
    const char *Name,int state_str2)
{StatLog *tsl,*tsl1;
 char *t,*t1,*t2;
 ulong long i,n;
 ulong ki,kn,ks,kj,j,it=0,nn,tnn=1,max=0;
 ulong hsh;
 int state_str=state_str2&1;
 CntrCode  *cc;
 char contry[8];
 //t1=bfr+0x1000;
 //t2=bfr+0x1200;
 t1=bf->bfr+0x1000;
 t2=bf->bfr+0x1200;
 char *pfor= GetVar(req_var,"for");
 ulong fip=0;

 if(!Name)
 {
   bf->bprintf("\nBad name??? <BR>\n");
   return ;
 }

 if(pfor)
 {
     pfor=SkipSpace(pfor);
     if(!pfor[0])pfor=0;
     //debug("pfor = %.32s",pfor);
 }
 t = GetVar(req_var,"fip");
 if(t)
 {
   t=SkipSpace(t);
   if(*t)
     fip=ConvertIP(t);
 }

// Send(bfr,sprintf(bfr,
  bf->bprintf(
#ifdef RUS
  "<hr><div align=right>[<a name=\"%s\" href=#top>Содержание</a>] &nbsp; &nbsp; [<a href=\"#end_%s\">Дальше</a>]  </div><h2><a href=\"javascript:SHTbl('t%x');\"><span id=lt%x >[-]</span></a> %s:</h2> " HTML_LN
  "<table id=t%x bgcolor=#F8FCFC border=1>"HTML_LN
  "<tr bgcolor=#E0F8FF><td><b>Название:</b></td><td><b>Количество:</b></td><td width=210>Проценты:</td></tr>"HTML_LN
#else
   s_HR__DIV_  //HTML_LN
//   "<table id=t%x bgcolor=#F8FCFC border=1>" HTML_LN
//     HTML_LN
#endif
 ,Name,Name,(long)fnd,(long)fnd,Name,(long)fnd); //);

 n=0;
 //state_str=0;
 for(tsl=psl;tsl;tsl=tsl->Next())
 {if(!(tsl->stt))
  {
   if( (fip && fip!=tsl->ip) || (pfor && ! (tsl->find(pfor)))  )continue;
   nn=1;
   if((i=(fnd)(tsl,t1))  )
   {
    //if(i&0x80000000) i&=~0x80000000; else i=1;
    if(!state_str) i=1;
    if(!(hsh=tsl->hsh)) hsh=MkName(t1);
    for(tsl1=tsl->Next();tsl1;tsl1=tsl1->Next())
    {
     if( (!tsl1->stt) && ((!tsl1->hsh) || (tsl1->hsh==hsh)) )
     {
      // if(pfor && ! (tsl1->find(pfor)) )continue;
      if( ( (!fip) || fip==tsl->ip) && ((!pfor) || tsl1->find(pfor)) && (j=(fnd)(tsl1,t2)) )
      {
       //if(j&0x80000000) j&=~0x80000000; else j=1;
       if(!state_str)j=1;
       if(!stricmp(t1,t2) )
       {i+=j; tsl1->stt=0xFFFF;}
       else tsl1->hsh=MkName(t2);
       ++nn ;
       ++tnn;
      }
      else tsl1->stt=0xFFFF;
     }
    }
    tsl->stt=(*Name=='~')?(nn?(ulong)i/nn:0):i;
    if(i>=0x10000000)tsl->stt=(i>>10)|0x80000000;
    if(i>max)max=i;
    n+=i;
   }
  }
 }

 ks=0;
 if(*Name=='~'){kn=max;}
 else
 {kn=n;
  if(n>=0x10000000){kn=n>>10; ks=1;}
 }
 contry[0]=0;
 if(kn)for(tsl=psl;tsl;tsl=tsl->Next())
 {if( tsl->stt && tsl->stt!=0xFFFF)
  {ki=1;
   if( (fnd)(tsl,t1) )
   {
    kj=tsl->stt&0x7FFFffff;
    if(ks && ! (tsl->stt&0x80000000))kj>>=10;
    ki= MULDIV(kj,100,kn);
    if(cntr_dat && ( state_str2 & isStatContry))
    {
      contry[0]=0;
      if( (cc=FindCntr(htonl(tsl->ip))) )
      {
         sprintf(contry," (%2.2s)",cc->nm);
      }
    }
  //  Send(bfr,sprintf(bfr,
    bf->bprintf(
     "\n<tr><td><font size=2 class=f><b>%.168s</b>%s</font></td>"
     "<td><font size=2 class=f><b>%u%s (%u%%)</b></font></td>"
     "<td><table border=0 width=%u><tr><td bgcolor=#8080FF><font size=1 class=f3>|</font></td></tr></table></td></tr>" HTML_LN,
    t1,contry,kj,strKb+1-ks,ki,ki*2+1); //);
    it++;
   }
  }
  tsl->stt=0;
  tsl->hsh=0;
 }
 //Send(bfr,sprintf(bfr,
 bf->bprintf(
 s_TR__TD_C ,(*Name=='~')?(tnn?((ulong)n)/tnn:0):kn,strKb+1-ks,it,Name); //);
};

const tfFind afFnd[]=
{
&fFindEventSumary,
&fFindHTTPUser,
&fFindHTTPUNet,
&fFindSSLUser,
&fFindHost,
&fFindURLHTTP,
&fFindRefHTTP,
&fFindForegRef,
&fFindRefHTTPHst,
&fFindUserAgent,
&fFindErrors,
&fFindHTTPIn,
&fFindHTTPOut,
&fFindHTTPInU,
&fFindHTTPOutU,
&fFindHTTPTime,
&fFindHTTPTimeU,
&fFindHTTPSpeed,
&fFindHTTPHour,
&fFindCountry,
#if  V_FULL
&fFindProxyUser,
&fFindProxyIn  ,
&fFindProxyInNC,
&fFindProxyOut ,
&fFindProxyInU ,
&fFindProxyInUNC,
&fFindProxyOutU ,
&fFindURLProxy,
&fFindRefProxy,
&fFindFTPUser,
&fFindFTPLogin,
&fFindFTPdownload,
&fFindFTPupload,
&fFindFTPdownloadU,
&fFindFTPuploadU,
&fFindPOPUser  ,
&fFindPOPLogin,
&fFindPOPx,
&fFindSMTPUser,
&fFindSMTPxIP,
&fFindSMTPs,
&fFindSMTPr,
&fFindDNSUser,
&fFindDNSHost,
#endif
0
};

const uchar isCountAr[]= {
0, // &fFindEventSumary,             "Сумарно по портам"
0|isStatContry, // &fFindHTTPUser,                ,"HTTP вызовы"
0|isStatContry, // &fFindHTTPUNet,                ,"HTTP подсети"
0|isStatContry|logTLS, // &fFindSSLUser,                 ,"SSL/TLS вызовы"
0, // &fFindHost,                    ,"Запрошенные хосты"
0, // &fFindURLHTTP,                 ,"Страницы"
0, // &fFindRefHTTP,                 ,"Ссылки"
0, // &fFindForegRef,                 ,"Внешние Ссылки"
0, // &fFindRefHTTPHst,              ,"Хосты ссылок"
0, // &fFindUserAgent,               ,"Браузеры"
0, // &fFindErrors,                  ,"Ошибки"
1|isStatContry, // &fFindHTTPIn,                  ,"HTTP. Входящий трафик (в байтах)"
1|isStatContry, // &fFindHTTPOut,                 ,"HTTP. Исходящий трафик (в байтах)"
1, // &fFindHTTPInU,                 ,"HTTP по хостам. Входящий трафик (в байтах)"
1, // &fFindHTTPOutU,                ,"HTTP по хостам. Исходящий трафик (в байтах)"
1|isStatContry, // &fFindHTTPTime,                ,"HTTP. Время обмена(в мс)"
1, // &fFindHTTPTimeU,               ,"HTTP. Время обмена по хостам. (в мс)"
1, // &fFindHTTPSpeed,               ,"~HTTP. Средняя скорость. (в байт/с)"
0, // &fFindHTTPHour,                ,"HTTP заходы по часам"
0, // &fFindCountry,                 ,"HTTP заходы по странам. (нужна база IP)"
#if  V_FULL
0|isStatContry|logProxy, // &fFindProxyUser,               ,"Proxy вызовы"
1|logProxy, // &fFindProxyIn  ,               ,"Proxy. Запрошенный входящий трафик (в байтах)"
1|logProxy, // &fFindProxyInNC,               ,"Proxy. Запрошенный трафик без учета возвращенного из кеша (в байтах)"
1|logProxy, // &fFindProxyOut ,               ,"Proxy. Исходящий трафик (в байтах)"
1|logProxy, // &fFindProxyInU ,               ,"Пользователи Proxy. Запрошенный входящий трафик (в байтах)"
1|logProxy, // &fFindProxyInUNC,              ,"Пользователи Proxy. Запрошенный трафик без учета возвращенного из кеша (в байтах)"
1|logProxy, // &fFindProxyOutU ,              ,"Пользователи Proxy. Исходящий трафик (в байтах)"
0|logProxy, // &fFindURLProxy,                ,"Proxy страницы"
0|logProxy, // &fFindRefProxy,                ,"Proxy ссылки"
0|isStatContry|logFTP, // &fFindFTPUser,                 ,"FTP вызовы"
0|logFTP, // &fFindFTPLogin,                , sFTP_LOGIN
0|logFTP, // &fFindFTPdownload,             ,"FTP. Утянули"
0|logFTP, // &fFindFTPupload,               ,"FTP. Залили"
1|logFTP, // &fFindFTPdownloadU,            ,"FTP. Утянули (в байтах)"
1|logFTP, // &fFindFTPuploadU,              ,"FTP. Залили (в байтах)"
0|isStatContry|logPOP3, // &fFindPOPUser  ,               ,"POP3 вызовы"
0|logPOP3, // &fFindPOPLogin,                , sPOP_LOGIN
1|logPOP3, // &fFindPOPx,                    ,"POP трафик сообщений (в байтах)"
0|isStatContry|logSMTP, // &fFindSMTPUser,                ,"SMTP вызовы"
1|logSMTP, // &fFindSMTPxIP,                 ,"SMTP трафик сообщений (в байтах)"
1|logSMTP, // &fFindSMTPs,                   ,"SMTP трафик сообщений от пользователей (в байтах)"
1|logSMTP, // &fFindSMTPr,                   ,"SMTP трафик сообщений для пользователей (в байтах)"
0|isStatContry|logDNS, // &fFindDNSUser,                 ,"DNS вызовы"
0|logDNS, // &fFindDNSHost,                 ,"DNS хосты"
#endif
0
};

int GetFuncType(tfFind f)
{
 for(int i=0; afFnd[i] ; ++i) if(afFnd[i]==f)return (isCountAr[i]&1);
 return 0;
}

//const
char *anFnd[]=
{
#ifdef RUS
 "Сумарно по портам"
,"HTTP вызовы"
,"HTTP подсети"
,"SSL/TLS вызовы"
,"Запрошенные хосты"
,"Страницы"
,"Ссылки"
,"Внешние Ссылки"
,"Хосты ссылок"
,"Браузеры"
,"Ошибки"
,"HTTP. Входящий трафик (в байтах)"
,"HTTP. Исходящий трафик (в байтах)"
,"HTTP по хостам. Входящий трафик (в байтах)"
,"HTTP по хостам. Исходящий трафик (в байтах)"
,"HTTP. Время обмена(в мс)"
,"HTTP. Время обмена по хостам. (в мс)"
,"~HTTP. Средняя скорость. (в байт/с)"
,"HTTP заходы по часам"
,"HTTP заходы по странам. (нужна база IP)"
#if V_FULL
,"Proxy вызовы"
,"Proxy. Запрошенный входящий трафик (в байтах)"
,"Proxy. Запрошенный трафик без учета возвращенного из кеша (в байтах)"
,"Proxy. Исходящий трафик (в байтах)"
,"Пользователи Proxy. Запрошенный входящий трафик (в байтах)"
,"Пользователи Proxy. Запрошенный трафик без учета возвращенного из кеша (в байтах)"
,"Пользователи Proxy. Исходящий трафик (в байтах)"
,"Proxy страницы"
,"Proxy ссылки"
,"FTP вызовы"
, sFTP_LOGIN
,"FTP. Утянули"
,"FTP. Залили"
,"FTP. Утянули (в байтах)"
,"FTP. Залили (в байтах)"
,"POP3 вызовы"
, sPOP_LOGIN
,"POP трафик сообщений (в байтах)"
,"SMTP вызовы"
,"SMTP трафик сообщений (в байтах)"
,"SMTP трафик сообщений от пользователей (в байтах)"
,"SMTP трафик сообщений для пользователей (в байтах)"
,"DNS вызовы"
,"DNS хосты"
#endif

#else
  x_sSUMMARY_B
, x_sHTTP_CALL
, "HTTP subnets"
, x_sSSL_CALL
, x_sREQUESTED
,"Pages"
,"Referer"
,"External referer"
,"Hosts of referer"
,"Browsers"
,"Errors"
, x_sHTTP__INC
, x_sHTTP__OUT
, x_sHTTP_BY_H
, x_sHTTP_BY_H0
,"HTTP. Time for exchange (ms)"
,"HTTP. Time for exchange by hosts (ms)"
,"~HTTP. Average speed (Bytes/s)"
,"HTTP calls by hours"
,"HTTP calls by countries. (the base of IP need)"
#if V_FULL
, x_sPROXY_CAL
, x_sPROXY__RE
, x_sPROXY__IN
, x_sPROXY__OU
, x_sPROXY_USE
, x_sPROXY_USE0
, x_sPROXY_USE1
, x_sPROXY_PAG
, x_sPROXY_REF
, x_sFTP_CALLS
, x_sFTP_LOGIN
, x_sFTP_DOWNL
, x_sFTP_UPLOA
, x_sFTP_DOWNL1
, x_sFTP_UPLOA1
, x_sPOP__CALL
, x_sPOP_LOGIN
, x_sPOP_DATA_
, x_sSMTP_CALL
, x_sSMTP_DATA
, x_sSMTP_DATA0
, x_sSMTP_DATA1
, x_sDNS_CALLS
, x_sDNS_HOSTS
#endif
#endif
,0


};

#include "statusr.cpp"

typedef  int (* cmpf)(const void *,const void *);
int  cmpw(ushort *a,ushort *b){return *a-*b; }

int Req::HTTPOutStatistics(char *bfr,char *log,int tbl,int dt)
{int i,n,err,j,x;
 union {
  char *p;
  StatLog *psl;
 };
 union {
  char *t1;
  StatLog *psl1;
 // ushort *st1;
 };
 StatLog *tsl,*tsl1;
 char *t2,*t3,*t;
 WIN32_FIND_DATA fnds;
 HANDLE hdl;
 n=0;
 BFILE bfl;
 bfl.Init(this,(PrintFlush)(Snd),bfr);
#ifdef A_64
  MyLock(StatMutex);
  statbasepoint=log;
#endif
 char *pfor,*pfort,*pfip;
 uint statdays[12];


 if((psl=ParseFile(log,bfr+0x4000)))
 {
  if( (pfor=GetVar(req_var,"for")) )
  {
    pfort=" for ";
  }
  else
  {
    pfor="";
    pfort="";
  }
  if( (pfip=GetVar(req_var,"fip")) )
  {
    pfort=" for ";
  }
  else
  {
    pfip="";
  }

  //t2=bfr+sprintf(bfr,
  bfl.bprintf(
#ifdef RUS
   "<a name=top></a><h2>Статистика.</h2>(%s событий%%)</center><ul>"
#else
   "<a name=top></a><h2>Statistics.</h2>(%s event%s%s %s)</center><ul>\n"
#endif
  ,bfr+0x4000,pfort,pfor,pfip);

  for(i=0;anFnd[i];++i)
   //t2+=sprintf(t2,
   bfl.bprintf((char *)
   (
   (tbl==255)?"\n<!-- %X%X --><li><a href=\"#%s\">%s</a>":
    "<li><a href=\"/$_admin_$state?n=%u&t=%u#%s\">%s</a>"),dt,i,anFnd[i],anFnd[i]);
  //t2+=sprintf(t2,
   bfl.bprintf(
#ifdef RUS
   "<li><a href=\"#Files\"><b>Log файлы за другие дни</b></a> (Если опция <i>Новый log файл каждый день</i> выбрана) </ul><center> ");
#else
    s_LI__A_HR , dt, tbl,pfor,pfip);
#endif
  //send(s,bfr,t2-bfr,0);
  if(tbl!=255) OutStatTblBody(psl,afFnd[tbl],//bfr
        &bfl,anFnd[tbl],isCountAr[tbl]);
  else for(i=0;anFnd[i];++i)OutStatTblBody(psl,afFnd[i], //bfr
             &bfl
          ,anFnd[i],isCountAr[i]);
 }

#ifdef RUS
#define LastHTML "<hr><div align=right><a name=\"Files\" href=#top>Contents</a></div>"\
  "<h2>Доступны Log файлы:</h2><font size=2 class=f><b>" HTML_LN
#else
#define LastHTML  s_HR__DIV_0 // HTML_LN
#endif
// send(s,LastHTML,sizeof(LastHTML)-1,0);
 bfl.bprintf("%s\n",LastHTML);
#undef LastHTML
 if(flog)
 {sprintf(t3=bfr+0x2000,"%s*",flog);
  if( (t=strrchr(//bfr
     t3,'.')) )DWORD_PTR(*t)='*';
  if( ( hdl=FindFirstFile(t3,&fnds) )!=INVALID_HANDLE_VALUE )
  {
   t1=bfr+0x3000;
   j=0;

   memset(statdays,0,sizeof(statdays)) ;

   do
   {if(!(fnds.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {i=0;
     if(fnds.cFileName[(n=strlen(fnds.cFileName)) -1]=='.')--n;
     if(n>4) i=atoui(fnds.cFileName+n-4);
    /*
    // send(s,bfr,sprintf(bfr, (i)?
     bfl.bprintf(
      "|<a href=/$_admin_$state?n=%u&t=%u>%u/%u</a>| &nbsp;" HTML_LN
      :
      "|<a href=/$_admin_$state?n=%i&t=%u>today</a>| &nbsp;" HTML_LN
      ,i,tbl,i/100,i%100
     ); //,0);
    // */
     if(i){
         //st1[j++]=i;
         //if(j>365) break;
         j=i/100;
         j--;
         if((uint)j<12)
         {
           statdays[j]|=1u<<(i%100);
         }
         dprint("i=%u j=%u statdays[j]=%X\n",i,j,statdays[j]);
     }
     else if(! (statdays[0]&1)  )
     {
     // send(s,bfr,sprintf(bfr,
      bfl.bprintf(
       "|<a href=/$_admin_$state?n=0&t=%u>today</a>| &nbsp;" HTML_LN
       ,tbl);
      //,0);
      statdays[0]|=1;
     }

    }
   }while(FindNextFile(hdl,&fnds));
   FindClose(hdl);
   //qsort(st1,j,2,(cmpf)cmpw);
   //mqsort(st1,j,(cmpf)cmpw,(cmpf)xchgw);
//    for(i=0;i<j;++i)
//    {
//      bfl.bprintf( "|<a href=/$_admin_$state?n=%u&t=%u>%u/%u</a>| &nbsp;" HTML_LN
//       ,st1[i],tbl,st1[i]/100,st1[i]%100 );
//    }
    for(i=0;i<12;++i)
       for(j=1;j<=31;++j)
         if( statdays[i] & (1<<j) )
           bfl.bprintf( "|<a href=/$_admin_$state?n=%u&t=%u>%u/%u</a>| &nbsp;" HTML_LN
                       ,(i+1)*100+j,tbl,i+1,j );
  };
 }

#ifdef A_64
 MyUnlock(StatMutex);
#endif

#define LastHTML  s__B___FON
 //send(s,LastHTML,sizeof(LastHTML)-1,0);
 bfl.bprintf("%s",LastHTML);
#undef LastHTML
 bfl.fflush();
 return 0;
}
#undef send

#endif
