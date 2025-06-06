/*
 * Copyright (C) 1999-2025 Maksim Feoktistov.
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

#include "g4s1.hh"
#include "g4strc.h"
#include "g4strcwm.h"
#include "g4strhtm.hh"

#endif


#ifndef SRV_H
#include "srv.h"
#endif


extern "C" {

extern "C" char xdcl[256];
extern "C" long dset;
extern "C" ulong xset;
extern "C" ulong  bios_chk,dbg_chk;
extern "C" long dta_chk;
extern void * id_heap;
#ifdef CD_VER
char lbl[50];
int GetCDLabel(char *fn,char *tg);
#endif
char *xxx;

#ifdef DEBUG_VERSION
#ifndef SYSUNIX
char xdclb[256];
void WIN_DBG_print(int l)
{
  static int h;
  if(!h) h=(int)GetStdHandle((ulong)STD_OUTPUT_HANDLE);
  _hwrite(h,xdclb,l);
}

#define printf(a...)  WIN_DBG_print(wsprintf(xdclb, a))

#else
#undef printf
#endif // SYSUNIX
#endif //DEBUG_VERSION


#ifndef NOATOUI
uint atoui(const char *a)
{register uint r=0,l;
 while( (l=*a-'0')<10 ){r=r*10+l;a++;};
 return r;
};

#if (! defined(SYSUNIX)) //&&  (! defined(MINGW))

int asci2n(int a)
{
  if(a>='a')  return a-('a' - 10);
  if(a>='A')  return a-('A' - 10);
  return a-'0';
}

long long strtoll(const char *a, char **endptr, int base)
{long long r=0;
 uint l;
 if(!base) base = 10;
 while( (l=asci2n(*a)) < base ){ r=r*base + l; a++;};
 if(endptr) *endptr = (char *) a;
 return r;
};


long long atoll(const char *a)
{long long r=0;
 uint l;
 while( (l=*a-'0')<10 ){r=r*10+l;a++;};
 return r;
};

#endif

uint  atouix(const char *a)
{register uint r=0,l;
 while( (l=*a-((*a>='a')?'a'-10:(*a>='A')?'A'-10:'0' ))<16 ){r=(r<<4)+l;a++;};
 return r;
};

uint atouisc(const char *&a)
{register uint r=0,l,n=0;
 while( (l=*a-'0')<10 ){r=r*10+l;a++; if(n++>11)break; };
 return r;
};
#endif
uint  atouix2(const char *a){ulong r=WORD_PTR(*a); return atouix((const char *)&r);};


inline int E64X(int i)
{
  if(i==63)return '/';
  if(i==62)return '+';
  if(i>=52)return '0'+(i-52);
  if(i>=26)return 'a'+(i-26);
  return 'A'+i;
};

inline int GetBit(uchar *t,uint i)
{ return   ( (t[i>>3]) >> (7-(i&7)) ) &1;};

char * Encode64(char *t,char *s,int cnt)
{register int i,j,k;
 cnt=(cnt*8);
 for(i=0;i<cnt;)
 { for(j=k=0;k<6;k++,i++){j<<=1; j|=GetBit((uchar *)s,i);};
   *t=E64X(j);
   t++;
 }
 j=cnt%6;
 while(j--) *t++='=';
 *t=0;
 return t;
};

char * PrepPath(char *p)
{
  if(p)
  {
    register int t;
    t=*p++;
    if( t!='\"' && t!='\'' && t!='`' )
    {
      --p;
      xxx=strpbrk(p," \t\r\n");
    }
    else
      xxx=strchr(p,t);
    if(xxx)*xxx=0;
    //  if( (xxx=strchr(p,t ) ) )*xxx=0;
  }
  return p;
};

#ifdef SYSUNIX
char * ArgvCompatibleStrstr(char *str,char *substr)
{ int x;
  if( ((char **)str)==__argv )
  {
   char **t=__argv+1;
   if((x=(substr[0]==' ')) )++substr;
   while(*t)
   {
     if( strin(*t,substr) )return (*t)-x;
     ++t;
   }
   return 0;
  }
  return strstr(str,substr);
};
#endif


ulong ConvertIP(//const
               char * &t)
{union{ uchar a[4]; ulong r;};
  a[0]=atouis(t); ++t;
  a[1]=atouis(t); ++t;
  a[2]=atouis(t); ++t;
  a[3]=atouis(t);
 return r;
};

#ifdef CD_VER
char* MkPatch(char *x,char *y)
{char *z=0,*z1;
 int i;
 goto l1;
 while(WORD_PTR(*y)==0x2E2E)
 {z1=strrchr(x,FSLUSH);
  if(z)*z=FSLUSH;
  if(!z1)break;
  z=z1; *z=0; y+=2;
 l1:
  if(*y==FSLUSH)++y;
 }
 i=sprintf( (z1 = new char[strlen(y)+strlen(x)+2]),"%s" FSLUSHS "%s",x,y);
 if(!*y)z1[i-1]=0;
 if(z)*z=FSLUSH;
 return z1;
}
#endif

ulong DefRange[]={1,0xFFFFffff,0};
ulong* ParseIPList(int x)
{ulong ip[64],*r,i,j;
 char *t;
 i=0;
 if( (t=Rangec[x]))while(*t)
 {if( (j=*t)==',')i=(i+1)&0x7E;
  else if(j=='/') i|=~ (0xFFFFFFFFul<<( 32-strtoul(t+1,&t,10) ) );
  else if(j!=' ' && (j!='-') )
  {if(! (j=htonl(ConvertIP(t))) ) ++j;
   ip[i+1]=ip[i]=j;
   if(++i>60)break;
   continue;
  }
  ++t;
 }
 i=(i+1)&0x7E;
 if(i<2)return (x&1)? (ulong*) &NullLong:DefRange;
 memcpy(r=new ulong[i+4],ip,i<<2);
 return r;
}

#ifdef USE_IPV6
ulong NullLong6[8];
ulong DefRange6[8]={0,0,0,0x1000000,0xFFFFFFFF};
in6_addr *ParseIP6List(int x)
{in6_addr ip[33],*r;
 ulong i,j;
 char *t;
 i=0;
 if( (t=Rangec6[x]))
 {while(*t)
  {if( (j=*t)==',')i=(i+1)&0x7E;
   else if(j!=' ' && (j!='-') )
   {t=IPv6Addr(ip[i].s6_addr16,t);
    memcpy(ip[i+1].s6_addr16,ip[i].s6_addr16,16);
    if(++i>30)break;
    continue;
   }
   ++t;
  }
 }
 i=(i+1)&0x7E;
 if(i<2)return (in6_addr *) ((x&1)?NullLong6:DefRange6);
 memcpy(r=new in6_addr[i+4],ip,i*sizeof(in6_addr));
 return r;
}
int IsInIP6RangeIP(in6_addr *lst,in6_addr *ip)
{
 while(lst->s6_addr32[0]|lst->s6_addr32[1]|lst->s6_addr32[2]|lst->s6_addr32[3])
 {
   if(memcmp(ip,lst,16)>=0 &&
     memcmp(ip,lst+1,16)<=0) return 1; lst+=2;}
 return 0;
}

#endif

inline int IsInIPRangeIP(ulong *lst,ulong ip)
{while(*lst){ if(ip>=*lst && ip<=lst[1]) return 1; lst+=2;}
 return 0;
}

inline int IsInIPR(int r,ulong ip)
{
    return
    // IsInIPRangeIP(Range[r],ip)?   1-2*IsInIPRangeIP(Range[r+1],ip) :0;
    ip && IsInIPRangeIP(Range[r],ip)  && ! IsInIPRangeIP(Range[r+1],ip) ;


}

int IsInIPRR(int r,sockaddr_in *psa_c)
{
#ifndef USE_IPV6
 return IsInIPR(r,htonl(psa_c->sin_addr. S_ADDR)) ;
#else
 #define psa_c6 ((sockaddr_in6*)psa_c)
 if( !IsIPv6(psa_c)  )return IsInIPR(r,htonl(IPv4addr(psa_c)));
 /*
 if(psa_c->sin_family!=AF_INET6) return IsInIPR(r,htonl(psa_c->sin_addr. S_ADDR)) ;
 if(psa_c6->sin6_addr.s6_addr32[0]==0 && psa_c6->sin6_addr.s6_addr32[1]==0 &&
    psa_c6->sin6_addr.s6_addr32[2]==0xFFFF0000
 )return IsInIPR(r,htonl(psa_c6->sin6_addr.s6_addr32[3]));
 */
 return IsInIP6RangeIP(Range6[r],& (psa_c6->sin6_addr))? 1- 2*IsInIP6RangeIP(Range6[r+1],& (psa_c6->sin6_addr)):0;
#endif
}

void CheckValidCGIIdent()
{
#ifndef VPNCLIENT_ONLY
 char *t,*p;
 char bb[256];
 int i;
#ifndef SYSUNIX
 for(t=cgi_detect; *t; t++ ) if( *t == '/' ) *t='\\';
 if( (t-cgi_detect)<2 )cgi_detect="\\cgi-bin\\";
 if(ftp_upload)for(t=ftp_upload; *t; t++ ) if( *t == '/' ) *t='\\';
#endif
 p=cmdline;

  if((t=strrchr(p,FSLUSH)))
  {if(*p=='\"')++p;
   *t=0;
   SetCurrentDirectory(p);
#ifdef SYSUNIX
   char xcdir[256];
   if(*p=='.')
   {getcwd(xcdir,255);
    p=xcdir;
   }
#endif

#ifdef CD_VER
   union{char *tt; ulong xx; char  cc[4];};
   int i;
   for(CfgParam *cp=ConfigParams;cp->desc||cp->name;++cp)
   if( (cp->min==255 || cp->min==256) && cp->v && (tt=*(char**)(cp->v))
#ifndef SYSUNIX
    && tt[1]!=':' && WORD_PTR(*tt)!=0x5C5C
#else
    && *tt!='/'
#endif
   ) *(char**)(cp->v)=MkPatch(p,tt);
#ifndef SYSUNIX
   for(i=0;ext[i] && (tt=ext[++i]);++i)if(tt[1]!=':' && WORD_PTR(*t)!=0x5C5C )
     ext[i]=MkPatch(p,tt);
#endif
   xx=DWORD_PTR(*p);
   cc[3]=0;
   *t=FSLUSH;
  }
#else
  if(!def_dir)sprintf(def_dir=new char[(t-p)+5],"%s" FSLUSHS "www",p);
   *t=FSLUSH;
  }else
#endif
   if(!def_dir)def_dir="www";
   def_dirlen=strlen(def_dir);
#ifndef CD_VER
 CreateDirectory(def_dir,&secat);
 if(proxy_dir)
 {CreateDirectory(proxy_dir,&secat);
  if(s_flgs[1]&FL1_BIGPROXY)
  {for(i=0;i<0x40;++i)
   {sprintf(bb,"%.234s" FSLUSHS "%2.2X",proxy_dir,i);
    CreateDirectory(bb,&secat);
   }
  }
 }
 if(max_smtp){
  CreateDirectory(out_path ,&secat);
  if(sent_path)CreateDirectory(sent_path,&secat);
  CreateDirectory(err_path ,&secat);
 }
#endif
#if 0
 proxy_range=ParseIPList(proxy_rangec);
 smtp_range =ParseIPList(smtp_rangec);
 ip_range   =ParseIPList(ip_rangec);
 dns_range  =ParseIPList(dns_rangec);
#else
 if(max_srv[0] && max_srv[0]<12 ) max_srv[0]=12;
 if(max_srv[1] && max_srv[1]<12 ) max_srv[1]=12;

 for(i=0;i<18;++i)
 {FREE_IF_HEAP(Range[i]);
  Range[i]=ParseIPList(i);
#ifdef USE_IPV6
  Range6[i]=ParseIP6List(i);
#endif
 }
#endif

 while((t=strrchr(header,'\n'))){if(t[1])break; t[-1]=0;}

 if(!(FL1_SMTPTLS&s_flgs[1]))s_aflg&=~AFL_TLS;

 if( ((FL1_GZ|FL1_GZPRX)&s_flgs[1]) && !zlibstate )InitZLib();
#if V_FULL
 if(!msg_spam)msg_spam="Message detected as SPAM";

 if(total_dhcp_ip)OptDHCP();

 LoadContries();
#endif

#ifdef SEPLOG
 InitSepLog();
#endif
 if(maxKeepAlive) KeepAliveList = (Req **) malloc( sizeof(Req *) * (maxKeepAlive + 2) );

#endif // VPNCLIENT_ONLY
}
#if 1
// TODO:

char *PrepLine(char *src, char **cmnt)
{
  char *t;
  char *comment = 0;
  char *e = strchr(src, '\n');
  char *ee = e;
  char *q = e;
  char *qe = src;
  if(e)
  {
    ee++;
    *e = 0;

    if( (t = strpbrk(src, "\"\'`#")) ) {
      if( *t != '#') {
        q = t;
        *e='\n';
#ifdef SYSUNIX
        qe= strchr_meta(t+1, *t);
#else
        qe= strchr(t+1, *t);
#endif
        if(qe) {
          e = strchr(qe + 1, '\n');
          if(e)
          {
            *e = 0;
            ee = e + 1;
          }
          else
          {
            ee = 0;
            e = qe + 1;
            goto fndCmnt;
          }
        }
        else {
          *e = 0;
          debug("Unclosed quote %.64s\n", src);
        }

      }
      else
        comment = t;
    }

    t = SkipSpace(ee);
    while(t && *t == '#')
    {
      if(!comment)
        comment = t + 1;
      ee = strchr(t, '\n');
      if(!ee) break;
      ee++;
      t = SkipSpace(ee);
    }
    if(ee)
    {
      ee[-1] = 0;
      if(ee[-2] == '\r') ee[-2] = 0;
    }

  }
fndCmnt:
  if( (t=strchr(qe, '#') ) ) {
    if(comment) {
      *e = '\n';
      if(! e[-1] )
        e[-1] = '\r';
    }
    comment = t + 1;
    *t = 0;
  }


  for(t=SkipSpace(src);*t;t++)
  {
    switch(*t) {

      case ' ':
      case '\t':
        if(t<q)  break;
#ifdef SYSUNIX
        if(0) {
      case '\\':
          if(t>q) {
            t++;
            *src++ = SlashSeq(t);
            break;
          }
        }
#endif
      default:;
       *src++ = *t;
    }
  }

  if(e && e[-1] == '\r')
    e[-1] = 0;

  *cmnt = comment;
  return ee;
}

// TODO:
char *first_coment = 0;


int FindParamCP(char *conf_txt, char *comment, CfgParam * cp)
{
  char *t;
  uint  j;

  //printf("%s - ", conf_txt);

  for(; cp->desc || cp->name ; cp++ )
  {
    if(cp->name && strin(conf_txt, cp->name) )
    {
      t = conf_txt + strlen(cp->name);

      if(*t < 'A')
      {
       // printf("%s\n", cp->name);

        if(cp->v)
        {
          t = SkipSpace(t);
          if(*t != '=' ) continue;
          t = SkipSpace(t+1);
          if(cp->max)
          {
            if( ((j=atoui(t))>=cp->min) && (j<=cp->max) )
              *(cp->v)=j;
          }
          else
          {
            *(char **)(cp->v) = PrepPath(t);
          }
        }
        else if(cp->max)
        {
          s_flgs[cp->min] |= cp->max;
        }

        if(cp[1].v || cp[1].name)
          cp->comment = comment;
        else if(cp[1].desc && comment)
        {
          if (strcmp(comment+1, cp[1].desc)) {
            cp->comment = comment;
          }
          else cp->comment = 0;
        }
        if(cp->fChange)cp->fChange(cp);
        return 1;
      }
    }
  }

  return 0;

}

int FindParam(char *conf_txt, char *comment)
{
  //DBGLA("conf_txt:'%.64s'", conf_txt)

  if (! *conf_txt ) return 0;
  if(*conf_txt == '@')
    return PrepCfg(PrepPath(conf_txt+1));
  if(FindParamCP(conf_txt, comment, ConfigParams))
    return 1;

#ifdef DEBUG_VERSION
  int ret;

  ret = FindParamCP(conf_txt, comment, ConfigParams2);
  if(!ret) {
    debug("Not found config param:%.64s", conf_txt);
  }
  return ret;
#else
  return FindParamCP(conf_txt, comment, ConfigParams2);
#endif
}


void ParseCfg(char * conf_txt)
{
  char *e;
  char *comment=0;
  int  cnt;

  conf_txt = SkipSpace(conf_txt);
  while(*conf_txt) {
    e = PrepLine(conf_txt, &comment);
    //conf_txt = SkipSpace(conf_txt);
    //if(!*conf_txt) break;

    if(FindParam(conf_txt, comment)) cnt++;
    else if( (!cnt) && !first_coment) first_coment = comment;

    if(!e) break;
    conf_txt = SkipSpace(e); // + 1;
  }

  CheckValidCGIIdent();
}

#ifndef VPNCLIENT_ONLY
int onCfgToStrVHost(CfgParam *th, char *bfr)
{
  int j=0;
  host_dir *a;

   for(a=hsdr.next;a;a=a->next)
     j+=sprintf(bfr+j,"hostpath=%s;\"%s\";\"%s\"" LF , a->h,a->d,a->flg?a->d+a->flg:"");

  return j;
}


int Pass2Txt(char *pbfr, char *p)
{
  if (*p==2)
    return FillMD5Pwd(pbfr, (uint *)(p+1));
  if (*p==1)
    return sprintf(pbfr, "+%8.8X.%8.8X", DWORD_PTR(p[1]),DWORD_PTR(p[5]));
  return 0;
}

int onCfgToStrUser(CfgParam *th, char *bfr)
{
  int j=0;
  User *tuser;
  int i;
  int k;
  char *p;

  for(tuser=userList;tuser;tuser=tuser->next)
    if((i=tuser->state)&0x80)
    {
      p=tuser->pasw();
      j+=sprintf(bfr+j,"user=%s;",tuser->name);
      k = Pass2Txt(bfr+j, p);
      if(k) j+=k;
      else if(s_flgs[1]&FL1_CRYPTPWD) j=ConvPwd(bfr+j,p)-bfr;
      else if(s_flgs[1]&FL2_MD5PASS) j=ConvPwdMD5(bfr+j,tuser->name,p)- bfr;
      else j+=sprintf(bfr+j, "%s", p);

      j+=sprintf(bfr+j,";%s;",tuser->dir(p));
      j+=tuser->FlgString(bfr+j);
#ifdef SYSUNIX
      WORD_PTR(bfr[j]) = '\n';
      j++;
#else
      DWORD_PTR(bfr[j]) = 0x0A0D;
      j += 2;
#endif
    }

  return j;
}

int onCfgChangeUser(CfgParam *th)
{
  //int l = strlen(th->v);
  User * u;
//  u = (User *) malloc(sizeof(User) + l + 1);
  u = (User *) malloc(sizeof(User));
  if (!u) return -1;
  u->next = 0;
  * (User **) th->vv = u;
  th->vv = & u->next;
  u->name = *(char **) th->v;
  u->Parse();
  return 0;
}


int onCfgChangeVHost(CfgParam *th)
{
  host_dir *a;
  host_dir *b;
  char *p, *t;

  p = *(char **)th->v;
  if( *p==';') return 0;

  if( (t=strchr(p,';') ) )
  {
    *t=0;
    t++;

    for(a=hsdr.next; (a) ;a=a->next)
      if(a->h && ! strcmp(a->h,p)) goto hst_found;

    a = (host_dir *) malloc(sizeof(host_dir) );
    a->h = p;
    a->next = 0;
    * (host_dir **) th->vv = a;
    th->vv = & a->next;

hst_found:

    a->flg=0;
    if((p=strchr(a->d=PrepPath(t),';'))) *p++=0;
    if(!xxx) return 0;
    t=xxx+1;
    if((!p) && *t==';') p=t+1;
    if(p)
    {
      p=PrepPath(p);
      if(*p)a->flg=p - a->d;
      if(xxx) t = xxx+1;
    }
  }
  return 0;
}

#endif //VPNCLIENT_ONLY

int onCfgChangeDisable(CfgParam *th)
{

  if(th[1].v) {
    //printf("%s disable %s %X\n", th[0].name, th[1].name, *th[1].v);
    *th[1].v=0;
  }
  return 0;
}

int onCfgChangeFlag(CfgParam *th)
{
  s_flgs[th->min] |= th->max;
  return 0;
}

int onCfgChangeNoFlag(CfgParam *th)
{
  s_flgs[th->min] &= ~ th->max;
  return 0;
}


#ifndef SYSUNIX

int onCfgChangeExt(CfgParam *th)
{
  char *pext = * (char **) th->v;
  int n = split(PrepPath(pext), ";", ext, 10);
  if(n > 0) ext[(n+1) & ~1] = 0;
  return 0;
}


int onCfgToStrExt(CfgParam *th, char *bfr)
{
  int j=0;
  char **tt;
  if(*ext)
  {
    j+=sprintf(bfr+j,"ext=\"");
    for(tt=ext; *tt; tt+=2) if(*tt[0]=='.')
      j+=sprintf(bfr+j,"%.4s;%s;",tt[0],tt[1]);
    DWORD_PTR(bfr[j-1])=0xA0D22 x4CHAR("\"\r\n");
    j+=2;
  }
  return j;
}


void InitParam(char *cln)
{
  char *arr[128];
  int n = split(cln, " \t", arr, 127);
  int i;
  int l;
  char *p;
  char *t;
  char *b;
  char *q;
  for (i=0; i<n; i++)
  {
    b = t = arr[i];
    while ((p=strpbrk(t,"'\"") ) )
    {
      while (! ((q=strchr(p+1, *p))) ) {
        i++;
        if(i > n) {
          debug("Unclosed quote %s\n", t);
          return;
        }
        l=strlen(t);
        t[l] = ' ';
      }
      t = q + 1;
    }
    FindParam(b, 0);
  }

  CheckValidCGIIdent();
}

#endif


#ifdef SYSUNIX
void InitParam(char *s)
{
  char **arg = ((char **)s) + 1;
  while(*arg)
  {
    FindParam(*arg, 0);
    arg ++;
  }

  CheckValidCGIIdent();
}
#endif

int PrepCfg(char *fname)
{
  char *t,*a,*b;
  int h,l,q;

  DBGLA("Fname: %s", fname)
  if( ( h=_lopen(fname,0) )>0 )
  {
    l=FileSize(h);
    if (!conf_name)
      conf_name = fname;

    if(! (t=new char[l+8]) )
      return -1;
    if( (t+l)>last_cfg )last_cfg=t+l;
    *t=' '; t++;
    _hread(h,t,l);
    t[l]=0;
    _lclose(h);
    ParseCfg(t);

    return 0;
  }
  return -1;
}


#endif

int CfgParam::TextCfgString(char *bfr)
{
 static char * last_comment;
 int j = (fToString)? fToString(this, bfr) :
            (v)?
                sprintf(bfr, max ? "%s=%u" LF
                                 : (*v) ?
                                          strchr(*(char **)v,'\"') ?
                                                                    "%s=`%s`"  LF
                                                                   :"%s=\"%s\"" LF
                                        :"",name, *(char **)v )
               :(name)?
                       (s_flgs[min]&max)? sprintf(bfr,"%s" LF ,name) : 0
               : (last_comment && !strcmp(last_comment+1, desc)) ? 0 : sprintf(bfr, LF "# %s" LF ,desc);
   if (comment)
   {
     DBGLA("comment: %s", comment)
     j+=sprintf(bfr+j, "#%s" LF , comment);
     last_comment = comment;
   }

   return j;

};
//-----
#ifndef CD_VER

int User::FlgString(char *bf)
{register char *bfr=bf;
  if(state&UserPOP3 )*bfr++='P';
  if(state&UserSMTP )*bfr++='S';
  if(state&UserHTTP )*bfr++='H';
  if(state&UserADMIN)*bfr++='A';
  if(state&UserNOCGI)*bfr++='N';
  if(state&UserFTPW )*bfr++='W';
  if(state&UserFTPR )*bfr++='F';
 return bfr-bf;
}
#endif


int PrepareTextCfg(char *bfr, CfgParam *cp)
{
  int j = 0;

  while(cp->desc || cp->name)
  {
    j+=cp->TextCfgString(bfr+j);
    ++cp;
  }
  return j;
}

void SaveConfigFile(char *bfr,char *fnm)
{
#ifndef CD_VER

 int h,i;
 host_dir *a;
 char **tt;
 User *tuser;
 char *p;

#ifdef SYSUNIX
 int j=2;
 DWORD_PTR(*bfr)=0x0A0D;
#else
 int j=1;
 DWORD_PTR(*bfr)='\n';
#endif

 debug("Save configuration...");

 if (first_coment)
   j += sprintf(bfr+j, "#%s\n", first_coment);

 j += PrepareTextCfg(bfr + j, ConfigParams);

 j += sprintf(bfr+j,  LF "# Other" LF);

 j += PrepareTextCfg(bfr + j, ConfigParams2);





 if( (h=_lcreat(fnm,0))>0)
 {
   _hwrite(h,bfr,j);
   _lclose(h);
 }
 else debug( sERROR__CA ,fnm);


#endif
};

void SaveCfgIfNeed()
{
  char *b;

     if(s_flg&FL_CFGUNSV)
     {b=new char[0x10000];
      SaveCfgFile(b);
      delete b;
     }
}
char * SaveCfgFile(char *b)
{char *u,*p;

 if(!conf_name)
 {
   int l;
    u=cmdline;
    if( (p=(*u=='\"')? strchr(++u,'\"') : strchr(u,' ') ))*p=0;
    if( (p=strrchr(u,'.')))
    {*p=0;
     l=sprintf(b,"%s.cfg",u);
     *p='.';
     conf_name=(char* ) malloc(l+3);
     strcpy(conf_name,b);
    }
    else conf_name=
#ifndef SYSUNIX
"http.cfg"
#else
"httpd.cfg"
#endif
    ;
 }
 SaveConfigFile(b,conf_name);
 s_flg&=~FL_CFGUNSV;
 return conf_name;
}


char * Rnames[]={"port","dnscache","ipv6","hosts","bind",0};
char * BRnames[]={"bad_","nouph","upproxy","dos_h",0};
int CfgParam::IsR()
{
  int i;
  if( name )
  {for(i=0;Rnames[i];++i)if(strstr(name,Rnames[i]) )
    {for(i=0;BRnames[i];++i)if(strstr(name,BRnames[i]) ) return 0;
      return 1;
    }
    if( v )
    {
      if(max)
      {
        if((!*(v)) && strstr(name,"max")) return 1;
        if((v== (uint *)&time_update) && ((!time_update) || !*v ) ) return 1;
      }
      if( v==(uint *) &dns_file) return 1;
    }
    else if(name && name[0]=='n' && strstr(name+2,"max") ) return 1;
  }
  return 0;
}


int DelStr(char *s,char *b,int l)
{char *p;
 int  ll;
 if((p=strchr(s,'\n')))
 {++p;
  if((ll=l-(p-b))>0)  memcpy(s,p,ll);
  if(ll>=0)
  {
   l-=p-s;
   b[l]=0;
  }
 }
 /*!!?
 else
 {
   *s = 0;
   l = s - b;
 }
 */
 return l;
}
ulong _PerSecond1E7=10000000;

char* OutLL(long x0, long x1,char *t)
{ulong a,b;
 if(x1<0)
 {x1=-x1-1;
  x0=-x0;
  *t++='-';
 }
 x1&=~0xE0000000;
 a=DIVMODLL(x0,x1,1000000000,b);
 if(a)
 {t+=sprintf(t,"%u%09u",a,b);}
 else t+=sprintf(t,"%u",b);
 return t;
}

#ifndef SYSUNIX
long long _llseek64(int h,long long p, int t )
{
  DWORD_PTR(p)= SetFilePointer((HANDLE)h,DWORD_PTR(p),((long *)&p)+1,t );
  return p;
};
#endif





struct LangRecord
{
 char *name;
 char **t;
};

LangRecord  LangData[]=
{
{"charset",&charset},
#include "S1_lf.hh"
#include "S2_lf.hh"
#include "S3_lf.hh"
#include "S5_lf.hh"
 {0,0}
};
void FixSlach(char *a)
{
  char *b;
 b=a;
 while(*b)
 {
   if(*b=='\\')
   {
     b++;
     switch(*b)
     {
         case 'n':  *a++='\n';   break ;
         case 'r':  *a++='\r';   break ;
         case 't':  *a++='\t';   break ;
    //     case '\\':  *a++='\\';  break ;
    //     case '\'':  *a++='\'';  break ;
    //     case '\"':  *a++='\"';  break ;
         default: *a++=*b;
     }
   }
   else if(*b==' '){ if(a[-1]!=' ') *a++=' ';}
   else if( *b!='\n' )*a++=*b;
   b++;
  }

 *a=0;

}
char *lang_data;

int CntLines(char *p,char *e)
{
  int r=0;
  while(p<e && *p)if(*p++=='\n')++r;
  return r;
}

int CheckLangItem(char *l_old, char *l_new)
{
  char *p1, *p2;

  while((p1=strchr(l_old, '%')))
  {
    if( (! (p2=strchr(l_new, '%') ) ) )
    {
      if(p1[1] == '%')
      {
        l_old = p1 + 2;
        continue;
      }
      return 0;
    }
    p1++;
    p2++;
    if(*p1 != *p2)
    {
      if(*p1 == '%') {
        l_old = p1 + 1;
        continue;
      }
      if(*p2 == '%') {
        l_new = p2 + 1;
        continue;
      }
      return 0;
    }

    l_new = p2 + 1;
    l_old = p1 + 1;
  }

  while( (p2=strchr(l_new, '%')) )
  {
    p2++;
    if(*p2 != '%') return 0;
    l_new = p2 + 1;
  }

  return 1;
}


int LoadLangCfg(char *fname)
{
#if 1
 char *t,*a,*b,*p,*e;
 int i,l,q,line;
 CfgParam *cp;
 if( ( i=_lopen(fname,0) )<=0 )
 {
   //  debug("Cant load %s; %s",fname,strerror(errno));
     return -1;
 }

 l=FileSize(i);


 if(! (lang_data=new char[l+16]) ){ return -1;}
 debug("Load %s; %u",fname,l);
 t=lang_data;
 *t++=0;
 *t++='\n';
 _hread(i,t,l);  t[l]=0;
 _lclose(i);
 a=b=t;
 t[l]=0;
 // Delete \r;
 while(*b)
 {
   if(*b!='\r')*a++=*b;
   b++;
 }
 *a=0;

 a=t-1;
 line=1;
 while(*a)
 {
 //  if( *a=='\n')line++;
   if( *a=='\n'  && a[1]!='\n' )
   {
     *a++=0;
     //b=strchr(a,'=');
     b=strpbrk(a,"=\n \t");
     if(!b)break;
     p=b;
     b=SkipSpace(b);
     if(!*b)break;
     if(*b!='='){
         //printf
         debug("**Lang file error at line %u near '%.20s'\n",line,b);
         a=b;
         if(*b=='\n')line++;
         continue ;
     }
     *p=0;
     b=SkipSpace(b+1);
     if((e=strstr(b,"\n\n"))) *e=0;

     for(i=0; LangData[i].name; i++)
     {
       if(!strcmp(a,LangData[i].name) )
       {
     //    debug("Found le %s=%.20s,%.20s\n",a,b,*LangData[i].t);
         p=*(LangData[i].t);

         if(!CheckLangItem(p, b))
         {
           debug("Lang element '%.20s' incompatible with source: original:'%.128s'; new:'%.128s'\n", a, p, b);
           goto  ExLP2;
         }

         for(cp=ConfigParams; cp->desc||cp->name;++cp)
         {
           if(cp->desc && cp->desc==p && (cp->desc<lang_data || cp->desc>b) ){cp->desc=b ;}
           if(cp->adv && cp->adv==p && (cp->adv<lang_data || cp->adv > b)){cp->adv=b ;}
         }
#ifndef VPNCLIENT_ONLY
         for(int j=0; anFnd[j] ; j++ )
            if(anFnd[j] == p)  anFnd[j]=b;
#endif
         *(LangData[i].t)=b;

         goto  ExLP2;
       }
     }
     debug("Lang element '%.20s' not found\n",a);

  ExLP2:
     if(!e)  goto  ExLP3;
     line+=CntLines(b,e)+2;
     *e=0;
     if(LangData[i].name)    FixSlach(b);
     a=e+1;
   }
   else
   {
     p=strstr(a+1,"\n\n");
     if(!p) break ;
     line+=CntLines(a,p)+2;
     a=p+1;
   }

 }

ExLP3:;
#endif
  return 0;

}

int onChangeM2b64(CfgParam *th)
{
  *(u64 *)th->vv = 1048576ll * *(uint *)th->v;
  return 1;
}

} // extern "C"



