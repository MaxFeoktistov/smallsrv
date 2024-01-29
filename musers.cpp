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



#ifndef SRV_H
#include "srv.h"
#endif
#ifdef SYSUNIX
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include <grp.h>
#endif


#ifndef A_64
char *User::dir(char *ps)
{
 if(*ps==1)return ps+19;
 else if(*ps==2)return ps+36;
 return memchr(ps,0,256)+1;
};
#endif

ulong OldRnd,RndCounter;
ulong Rnd()
{

 ulong r,v1,v2;
#ifndef SYSUNIX
 FILETIME ft;
 GetSystemTimeAsFileTime(&ft);

 v1=(GetTickCount()^(ulong)&r);
 v2=ft.dwHighDateTime * ft.dwLowDateTime;
 r=rol( (OldRnd^ft.dwLowDateTime)
       ^(v1<<(v2%24))+v2
       ,((OldRnd>>(v1&15))^ft.dwLowDateTime)+ ++RndCounter );
#else
 struct timeval tv;

 gettimeofday(&tv,0);


  v1=tv.tv_usec^(u_long)&r;
  v2=tv.tv_usec*tv.tv_sec;
  r=rol( (OldRnd^tv.tv_usec)
       ^(v1<<(v2%24))+v2
       ,((OldRnd>>(v1&15))^tv.tv_usec)+ ++RndCounter );

#endif
 OldRnd=r;
 return r;
};

#ifdef WITHMD5
void MD5UpdateL(MD5_CTX *c, char *u)
{
  if(u)
  {
    MD5Update (c, (uchar *)u, strlen ( (char *) u));
   // debug("MD5UL<<%s",u);
  }
  else
  {
    debug("MD5UL required variable is abcent");

  }

};




void ConvPwdMD5L4(uint *t4,char *u,char *pas, char *realmm)
{
  MD5_CTX context;

  MD5Init (&context);
  MD5UpdateL (&context, u);
  MD5Update(&context,(uchar *) ":", 1);
  MD5UpdateL (&context, realmm);
  MD5Update(&context,(uchar *) ":", 1);
  MD5UpdateL (&context, pas);
  MD5Final ((uchar *)t4, &context);
}

char* ConvPwdMD5(char *t,char *u,char *pas)
{
  uint dgt[6]      ;
  ConvPwdMD5L4(dgt,u,pas);
  return t+=sprintf(t,"~%8.8X.%8.8X.%8.8X.%8.8X",dgt[0],dgt[1],dgt[2],dgt[3]);
}

int IsPwdMD5C(char *p, char *pas,char *u)
{
  uint dgt[6]      ;
  int i;
  ConvPwdMD5L4(dgt,u,pas);
  p++;
  for(i=0;i<4;i++)
    if(DWORD_PTR(p[i<<2]) != dgt[i] ) return 0;
  return 1;
}


char *PrFinVar(char *s,const char *v)
{
  int l;
  char *r;
  char bb[64];
  l=sprintf(bb,"%s=",v);
  if(!(r=stristr(s,bb) )) return 0;
  if(r[l]=='"')l++;
  return r+l;
}
const char *digetvars[]=
{
  "username",
  "nonce",
  "uri",
  "qop",
  "nc",
  "cnonce",
  "response",
  "opaque",
  0
};

void CvtHex(uint  *cd, char * Hex    )
{
 sprintf(Hex,"%8.8x%8.8x%8.8x%8.8x",
#if __BYTE_ORDER__  ==  __ORDER_LITTLE_ENDIAN__
#ifdef A_64
   (uint) htonl(cd[0]),(uint) htonl(cd[1]),(uint) htonl(cd[2]),(uint) htonl(cd[3])
#else
   htonl(cd[0]),htonl(cd[1]),htonl(cd[2]),htonl(cd[3])
#endif

#else
   cd[0],cd[1],cd[2],cd[3]
#endif
   );
}

void GenAPOP_dgst(char *pas,char *dgst,char *s,int ssize)
{
  MD5_CTX context;
  uint dgt[6];

  MD5Init (&context);
  MD5Update (&context, (uchar *)s, ssize);
  MD5UpdateL (&context, pas);
  MD5Final ((uchar *)dgt, &context);
  CvtHex(dgt,dgst);
}

int IsPwdAPOP(char *pas,char *dgst,char *s,int ssize)
{
  char dgHex[40];
  GenAPOP_dgst(pas,dgHex,s,ssize);
  return ! strcmp(dgHex,dgst);
}


void CalkPwdMD5D(char **dgv, uint *HA1,char *method, char *HA2Hex)
{
  uint HA2[6]      ;
//  ulong rt2[6]      ;
 // char HA2Hex[40]      ;
  char HA1Hex[40]      ;
//  char rez[40]      ;
#define rt2 HA2
#define rez HA2Hex
  MD5_CTX context;
  CvtHex(HA1, HA1Hex);
// calk HA2
  MD5Init (&context);
  MD5UpdateL (&context, method);
  MD5Update(&context,(uchar *) ":", 1);

  MD5UpdateL (&context, dgv[digtVar_uri]);
  MD5Final((uchar *)HA2, &context);
  CvtHex(HA2, HA2Hex);


  MD5Init (&context);

  MD5Update(&context,(uchar *) HA1Hex, 32);
  MD5Update(&context,(uchar *) ":", 1);
  MD5UpdateL (&context, dgv[digtVar_nonce]);
  MD5Update(&context, (uchar *)":", 1);
  if(dgv[digtVar_qop])
  {
          MD5UpdateL(&context, dgv[digtVar_nc]);
          MD5Update(&context, (uchar *)":", 1);
          MD5UpdateL(&context, dgv[digtVar_cnonce]);
          MD5Update(&context, (uchar *)":", 1);
          MD5UpdateL(&context, dgv[digtVar_qop]);
          MD5Update(&context, (uchar *)":", 1);
  }
  MD5Update(&context,(uchar *) HA2Hex, 32);
  MD5Final ((uchar *)rt2, &context);
  CvtHex(rt2, rez);

}

int IsPwdMD5D(char **dgv, uint *HA1,char *method)
{
  char HA2Hex[40];
  CalkPwdMD5D(dgv, HA1, method, HA2Hex);

//  debug("MD5 cmp:%s<>%s|  %s",rez,dgv[digtVar_response],HA1Hex);
  return !strcmp(HA2Hex,dgv[digtVar_response]) ;
#undef rez
#undef rt2
}
/*
void CalkHA1(char *u,char *pwd, uchar *HA1)
{
  MD5_CTX context;
// calk HA1
  MD5Init (&context);
  MD5UpdateL(&context, u);
  MD5Update(&context, (uchar *)":", 1);
  MD5UpdateL(&context, realm);
  MD5Update(&context,(uchar *) ":", 1);
  MD5UpdateL(&context, pwd);
  MD5Final ((uchar *)HA1, &context);
}
*/
int IsPwdMD5DD(char **dgv, char *u,char *pwd,char *method)
{
  uint HA1[6];
  ConvPwdMD5L4(HA1, u, pwd);

  //CalkHA1(u, pwd, (uchar *)HA1);
  return IsPwdMD5D(dgv, HA1,method);
}

#endif
char* ConvPwd(char *t,char *pas)
{
 ulong x,r;
 ulong aa[2];
  x=MkName(pas);
  do{
   r=Rnd();
   aa[0]=rol(x^r,*pas);
   aa[1]=ror(x+~r,pas[1]);
  }while(memchr(aa,0,8));

  return t+sprintf(t,"+%8.8XZ%8.8X",aa[0],aa[1]);
}

void UpdPwdCrypt(char *p)
{ulong a,b;
 if(*p=='+' && p[9]=='Z' && strlen(p)==18 && (a=atouix(p+1)) && (b=atouix(p+10)) )
 {
  *p=1;
  DWORD_PTR(p[1])=a;
  DWORD_PTR(p[5])=b;
 }
#ifdef WITHMD5
 else  if(*p=='~' && p[9]=='.' && strlen(p)==36 )
 {
   char *t;
   uint d[4];
    t=p;
    for(a=0; a<4; a++)
    {
      b=strtoul(t+1,&t,16);
      if( (*t!='.' && a!=3) )//|| (! (b&0xFF) ) || (! (b&0xFF00) ) || (! (b&0xFF0000) ) || (! (b&0xFF000000) ))
          goto exLp2;
      d[a]=b;
    }

    *p++ =2;
    memcpy(p,d,16);

  exLp2: ;
 }
#endif
}
//inline
int IsPwd(ulong a,ulong b, char *pas)
{
 ulong x;
 x=MkName(pas);
 a=ror(a,*pas);
 b=rol(b,pas[1]);
 return ((a^x)==~(b-x));
}
int IsPwdC(char *p, char *pas)
{
 if(*p==1)return IsPwd(DWORD_PTR(p[1]),DWORD_PTR(p[5]), pas);
 return !strcmp(p,pas);
}


/*
inline int  User::Convert(char *x)
{
 register char *pw=pasw();
 return sprintf(x,"%.32%s;%.32%s;%.256%s;%c%c%c%c%c;\n",
                    name,pw,dir(pw),
                  (state&UserPOP3)?'P':' ',(state&UserSMTP)?'S':' ',
                  (state&UserHTTP)?'H':' ',(state&UserFTP)?'F':' ',
                  (state&UserFTPW)?'W':' '
                  );
};
*/

#ifdef SYSUNIX
#include<pwd.h>
#endif


void User::MkDir()
{
char bfr[280];
char *d;
#ifdef SYSUNIX
struct passwd *pwd;
int i;
#endif
 if(state&(UserFTP|UserPOP3))
 {
  d=dir();
#ifdef SYSUNIX
  i=
#endif
  CreateDirectory(d,&secat);
#ifdef SYSUNIX
  if( (!i) && ((pwd=getpwnam(name)) || ( (state&UserFTP) && (pwd=getpwnam("ftp")))) )
  {chown(d,pwd->pw_uid,pwd->pw_gid); }
#endif
  if(state&UserPOP3)
  {sprintf(bfr,"%.255s" FSLUSHS "mbox",d);
#ifdef SYSUNIX
  i=
#endif
   CreateDirectory(bfr,&secat);
#ifdef SYSUNIX
   if( (!i) && pwd){chown(bfr,pwd->pw_uid,pwd->pw_gid); }
#endif
  }
 }
}

#ifdef AT_ARM

int User::Parse(char *x)
{
  char xbfr[256];
  char *e;
  int l;
  if( ((ulong)x)!= (ulong)(this) )
  {
    e=strpbrk(x,"\r\n ");
    if(e)*e=0;
    strncpy(xbfr,x,250);
    l=250;
    if(e)l=e-x;
    memcpy(//(char *) (void *) this
    name
    ,xbfr+5,l-5);
  }
  return Parse();


};

#endif
int User::Parse()
{

//user=name;password;home_dir;PSHF_flags
 char *t;
// char *n;
 char *x;
 char *pasw;
 char *dir;
 char bfr[264];
    if(state&UserPARSED)return 1;
  //  if((t=strchr(name,'\n')))*t=0;
  //  if( (t=strchr(name,'#')))*t=0;
    if((pasw=strchr(name,';')))
    {*pasw++=0;
     if((dir=strchr(pasw,';')))
     {  *dir++=0;
        state=0;
        if((x=strchr(dir,';')))
        {
         if(x[-1]==FSLUSH)x[-1]=0;
         *x++=0;
        do{
           switch(*x)
           {
            case 'P':state|=UserPOP3; break;
            case 'S':state|=UserSMTP; break;
            case 'H':state|=UserHTTP; break;
            case 'A':state|=UserADMIN;break;
            case 'N':state|=UserNOCGI;break;
            case 'W':state|=UserFTPW; break;
            case 'F':state|=UserFTPR; break;
          //  case ' ':
            default:
              goto l1;
           };
           x++;
          }while(1);
        l1:;
         state|=UserPARSED;
#ifdef A_64
        pwd=pasw;ddr=dir ;
#endif
         MkDir();
	// debug("user=%.15s pass=%.15s dir=%.20s ; %u\n",name,pasw,dir,offset(User,name));
         UpdPwdCrypt(pasw);

         return 1;
        }
     }
    };
    return 0;
};


//-------
int IsSame(char *tt,char *pp)
{
 char *z;
  z=0;
  do{
   if(*tt && !*pp)return 0;
   if(*tt=='*')
   { z=++tt;
 lb_wld:
     while(*++pp!=*tt) if(!*pp)return 0;
   }
   else if(*pp++!=*tt++)
   {if(z && *pp){tt=z; goto lb_wld;}
    return 0;
   }
  }while(*tt);
 return !*pp;
}

void Req::AddHack(ulong t,int v)
{

#ifdef USE_IPV6
  union{
     LimitCntr *lip;
     LimitCntrIPv6 *lip6;


   };
   if(IsIPv6(&sa_c)) //sa_c.sin_family==AF_INET6)
   {
     if(!(lip6=hack6.Find(sa_c6.sin6_addr)))
     {
      lip6=hack6.Push();
      lip6->Set(sa_c6.sin6_addr);
      /*
      lip6->ip.ip.s6_addr32[0] =  sa_c6.sin6_addr.s6_addr32[0];
      lip6->ip.ip.s6_addr32[1] =  sa_c6.sin6_addr.s6_addr32[1];
      lip6->ip.ip.s6_addr32[2] =  sa_c6.sin6_addr.s6_addr32[2];
      lip6->ip.ip.s6_addr32[3] =  sa_c6.sin6_addr.s6_addr32[3];
      */
     }


   }
   else
   {
#else
     LimitCntr *lip;
#endif
     ulong ip4;
     ip4=IPv4addr(&sa_c);
     if(!(lip=hack.Find(ip4))) // sa_c.sin_addr. S_ADDR)))
     {
         lip=hack.Push(); lip->ip=ip4; // sa_c.sin_addr. S_ADDR;
     }
#ifdef USE_IPV6
   }
#endif
   lip->first=t;
   lip->cnt+=v;
}

#define USER_FOUND ((SysUser *) 1)
//-------
//ulong lastbadip[24],nextbadip;
User   *FindUser(char *bfr,int typ,char *pwd /*=0*/,Req *r) //=0)
{
  union {
    User *tuser;
    SysUser *suser;
  };
 char *t,*t1,*t2;
 ulong ip; //,*pip=0;
 union{
 LimitCntr *lip;
 LimitCntrIPv6 *lip6;
 };
 int  cc;
#ifdef WITHMD5
 int md5pwd=typ&FindUserMD5digest;
// ulong dgt[4];
 char *dgtvars[10];

// ConvPwdMD5L4(dgt,bfr,pwd);
// typ&=0x7F;

 if( //(typ != (UserPOP3|FindUserMD5cram)) &&
     //(! (typ & (UserPOP3|UserSMTP)) ) &&
     pwd && ( strin(pwd,"Digest") || md5pwd ) )
 {

   for(cc=0;digetvars[cc];cc++)
   {
     if((dgtvars[cc]=  PrFinVar(pwd,digetvars[cc])) )
        md5pwd|=1<<cc;
   }
//debug("TTT digest %X %s %X",md5pwd,bfr,typ);
   if( (md5pwd &  DIGT_MIN_REQUIRED) != DIGT_MIN_REQUIRED )
   {
     debug("Absent DIGEST_MIN_REQUIRED %X %X",md5pwd&0xFFFF,DIGT_MIN_REQUIRED);
     md5pwd=0;
   }
   else
   {
    for(cc=0;digetvars[cc];cc++)
    {
     if(dgtvars[cc] && (t=strpbrk(dgtvars[cc],"\t\r\n \",")) )*t=0;
    }
    bfr=dgtvars[digtVar_username];
    if( (!r)  || ! (r->CheckNonce(dgtvars[digtVar_nonce],dgtvars[digtVar_opaque])) ) return 0;
   }
 }
 //typ&=~FindUserMD5cram;
#endif
//debug("TTT %s %X %X",bfr,typ,userList);
 cc=0;
 for(tuser=userList;tuser;tuser=tuser->next)
 {
  cc++;
// debug("%s %s %X %X",tuser->name,bfr,tuser->state,typ);
  if( (tuser->state & UserPARSED) && (tuser->state&typ) && IsSame(tuser->name,bfr)
  )
  {

   if(pwd)
   {
#ifndef WITHMD5
    if( *(t=tuser->pasw()) && ! IsPwdC(t,pwd)  )goto lbBad;
#else
    if( *(t=tuser->pasw())  )
    {
   //   debug("%s %s %X %X",tuser->name,t,tuser->state,typ);
     if(*t==1 && ( md5pwd || (typ & (FindUserMD5cram|FindUserMD5digest) ) ) )
     {
       r->fl|=F_DIGET_UNAVILABLE;
 //      debug("Digest unavilable for %s",tuser->name);
       return 0;
     }
     if( typ & FindUserMD5cram ) // typ == (UserPOP3|FindUserMD5cram) || typ == (UserSMTP | FindUserMD5cram) )
     {
        if(! IsPwdAPOP(t,pwd,r->pst,r->postsize)) goto lbBad;
     }
     else
     {
        t1="GET";
        if(r->fl&F_POST)t1="POST";
        if(DWORD_PTR(r->loc) == 0x4E4E4F43 x4CHAR("CONN")  ) t1="CONNECT";
        if(DWORD_PTR(r->loc) == 0x44414548 x4CHAR("HEAD")  ) t1="HEAD";
        if( !(
           (md5pwd)?
            (
              (*t==2)?
                    IsPwdMD5D(dgtvars,(uint *)(t+1),t1) :
                    IsPwdMD5DD(dgtvars,tuser->name,t,t1)

            )
        :
        (*t==2)?
        IsPwdMD5C(t,pwd,tuser->name)
        :
        IsPwdC(t,pwd)  //strcmp(t,pwd)
        ) )goto lbBad;
     }
    }
#endif
    r->dir=tuser->dir(t);

//    if( (pip=memchr4(lastbadip, r->sa_c.sin_addr. S_ADDR ,8)))//saddr[r->ntsk],8)) )
//    {if(!(pip[8]&=~((0x501*cc)<<5)))*pip=0;
//    }
  lbFound:;
#ifdef USE_IPV6
  if( IsIPv6( & r->sa_c ) ) //sa_client.sin_family==AF_INET6)
  {
     if( (lip6=hack6.Find(r->sa_c6.sin6_addr ) ) )
     { if(!(lip6->cnt&=~ ((0x501*cc)<<5))) hack6.Del(lip6); }
  }
  else
#endif
    if( (lip=hack.Find( IPv4addr(& r->sa_c ) ) ) ) //r->sa_c.sin_addr. S_ADDR)))
    { if(!(lip->cnt&=~((0x501*cc)<<5)))hack.Del(lip); }

   }
   return tuser;
  }
 }
#ifdef USE_SYSPASS
#ifdef SYSUNIX
 DBGL("")
 if((!tuser) && (FL3_SYS_USERS & s_flgs[3]) )
 {
    DBGL("SYS")

    for(suser=suserList;suser;suser=(SysUser *) suser->next)
    {
      if( !strcmp(suser->nname,bfr) )
      {
        if(md5pwd)
        {
          r->fl|=F_DIGET_UNAVILABLE;
          return 0;
        }

        if(! suser->IsPwd(pwd)) goto lbBad;
        break;
      }
    }
    if(!tuser) suser = CheckSysPassword(bfr,pwd);
    DBGLA("SYS %lX", (long)suser)
    if(suser == USER_FOUND)
    {
      if(md5pwd)
      {
        r->fl|=F_DIGET_UNAVILABLE;
        return 0;
      }
      goto lbBad;
    }
    if(!suser) goto lbBad;
    if( suser->state & typ)
    {
      if(r)
      {
        r->dir = suser->ddr;
        goto lbFound;
      }
      return tuser;
    }
 }
#endif
#endif

#if !defined(CD_VER)
//debug("U1 %X %X %X",s_flgs[1],(1<<(BYTE_PTR(r->fl,2)&0xE)),s_flgs[1]&(1<<(BYTE_PTR(r->fl,2)&0xE)) );
  if(r && (r->Snd!=&TLSSend)
    && (t=strpbrk(bfr,"@#")) &&
    (s_flgs[1]&(1<<(BYTE_PTR(r->fl,2)&0xE)))
    &&
     //IsInIPRange(proxy_range,htonl( r->sa_c.sin_addr. S_ADDR )) //saddr[r->ntsk]))
     r->IsInIPRange(proxy_range)>0
    )
  {
//debug("U2 %s",t);
//    if( (t1=strpbrk(++t,"@#") ) )
    ++t;
    if( (t1=strchr(t,t[-1]) ) )
    {++t1;
     if( !(t2=strpbrk(pwd,"@#") ) )
     //t2=strpbrk(t1,"@#");
       t2=strchr(t1,t[-1]);
     if(t2)
     {t[-1]=0;
      *t2++=0;
      t1[-1]=0;
//debug("U3 %s %s",bfr,pwd);

      if((tuser=FindUser(bfr,typ,pwd,r)))
      {r->fl|=F_PRX;
       r->req=t;
       r->pst=t1;
       r->trn=t2;
//debug("U4 %s %s %s",r->req,r->pst,r->trn);

      };
      return tuser;
     }
    }
  }

#endif
 lbBad:
  if(r)
  {
#ifdef SEPLOG
    sepLog[r->ServerNumber()]->LAddToLog(0, r->s, & r->sa_c46, " Try to login failed %.20s;%.127s\r\n",bfr,pwd?pwd:"");
#else
    AddToLog(0, r->s, & r->sa_c46, "Try to login failed %.20s;%.127s\r\n",bfr,pwd?pwd:"");
#endif
    //sprintf(bfr+24," Try to login failed %.20s;%.127s\r\n",bfr,pwd?pwd:"");
    //AddToLog(bfr+24,r->s,&r->sa_c46);
#if defined(SPECIAL) || !defined(CD_VER)
#ifdef USE_IPV6
    if(IsIPv6(&r->sa_c) )//sa_client.sin_family==AF_INET6)
    {
        if( (lip6=hack6.Find( r->sa_c6.sin6_addr ) ) ) goto lbHF;
        lip6=hack6.Push(); lip6->Set( r->sa_c6.sin6_addr );  // r->sa_c.sin_addr. S_ADDR;
    }
    else
#endif
    if( (lip=hack.Find(IPv4addr(& r->sa_c ) ) ) ) //r->sa_c.sin_addr. S_ADDR)))
    {
     lbHF:
        if(pwd &&  lip->first !=(ip=MkName(pwd)) ){lip->cnt++; lip->first=ip;}
    }else
    {
        lip=hack.Push(); lip->ip=IPv4addr(& r->sa_c );  // r->sa_c.sin_addr. S_ADDR;

    }
    if(tuser)
    {lip->cnt|=(0x501*cc)<<5;  }

#endif
  }
 return 0;
};
//-----
#ifndef CD_VER
uchar StdPorts[]={80,88,21,25,110};
int Req::CallUp(User *puser)
{
 char tmp[512];
 int h;
 sprintf(inf,"%.94s",pst);
 if((h=call_socket2(pst,StdPorts[(fl>>16)&7]))>=0)
 {
  GetCMD(h,tmp,0);
  send(h,tmp,sprintf(tmp,"USER %.64s\r\n",req),0);
  GetCMD(h,tmp,0);
  send(h,tmp,sprintf(tmp,"PASS %.64s\r\n",trn),0);
  sprintf(tmp,"X-PROXY: login in %.64s (to %.64s)\r\n",puser->name,pst);
  AddToLog(tmp,s,&sa_c46);
 }
 return h;
}
#endif

char * Rnames[]={"port","dnscache","ipv6","hosts","bind",0};
char * BRnames[]={"bad_","nouph","upproxy","dos_h",0};
int CfgParam::IsR()
{int i;
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

#ifdef USE_SYSPASS
#ifdef SYSUNIX

void InitAccessGids()
{
  int i;
  struct group *g;
  for(i=0;i<N_ACESS_FLAGS; i++)
  {
    g = getgrnam(access_groups[i]);
    if(g)
      access_gids[i] = g->gr_gid;
  }
}

SysUser* AllocSysUser(struct passwd* pEntry, char *pas)
{
  int l1 = strlen(pEntry->pw_name);
  int l2 = strlen(pEntry->pw_dir);
  int l3 = strlen(pas);
  SysUser *ret;
  int ngroups = 128;
  gid_t gids[128];
  int  i,j;

  ret = (SysUser* )malloc(l1+l2+l3+ 3 + sizeof(SysUser) );
  if(ret)
  {
#ifdef A_64
    ret->name = ret->nname;
    ret->state = UserSYSUSER | UserPARSED;
#else
    ret->state = UserPARSED;
#endif
    strcpy(ret->nname,pEntry->pw_name);
    strcpy(ret->ddr=ret->nname + l1 + 1, pEntry->pw_dir);
    strcpy(ret->pwd=ret->ddr + l2 + 1, pas);

    if(getgrouplist(pEntry->pw_name, pEntry->pw_gid,
                        gids, &ngroups)>0)
    {
      for(i=0; i<ngroups; i++)
        for(j=0; j<N_ACESS_FLAGS; j++)
          if(gids[i] == access_gids[j]) ret->state |= (1<<j);
    }
    ret->state ^= UserNOCGI;
    ret->next = (User*) suserList;
    suserList = ret;
  }
  return ret;
}

// @return 0 - password is correct, otherwise no
SysUser* CheckSysPassword( const char* user, const char* password )
{
    struct passwd* pEntry;
    SysUser *ret=0;

    DBGLA("SYS PAS %s %s",user,password)

    MyLock(user_mutex);
    pEntry = getpwnam( user );
    if ( pEntry )
    {
      ret = USER_FOUND;

      DBGLA("SYS PAS %s %s",user,pEntry->pw_passwd)

      if ( WORD_PTR(pEntry->pw_passwd[0]) != 'x' )
      {
        if(! strcmp( pEntry->pw_passwd, crypt( password, pEntry->pw_passwd ) ) )
        {
          ret = AllocSysUser(pEntry, pEntry->pw_passwd);
        }
      }
      else
      {
        DBGL("password is in shadow file")
        // password is in shadow file
        struct spwd* shadowEntry = getspnam( user );

        DBGLA("SYS PAS %s %s",user,(shadowEntry)? shadowEntry->sp_pwdp: "")

        if ( shadowEntry && strcmp( shadowEntry->sp_pwdp, crypt( password, shadowEntry->sp_pwdp ) ) )
        {
          DBGL("ok")
          ret = AllocSysUser(pEntry, shadowEntry->sp_pwdp);
        }
      }
    }
    MyUnlock(user_mutex);
    return ret;

}

int SysUser::IsPwd(char *pas)
{
  int ret;
  MyLock(user_mutex);
  ret = ! strcmp(pwd, crypt(pas, pwd));
  MyUnlock(user_mutex);
  DBGLA("%d %s %s", ret, pas, pwd)
  return ret;
}
#endif
#endif
