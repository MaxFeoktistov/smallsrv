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
#ifdef SYSUNIX
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include <grp.h>
#endif

#ifndef NEWSTRUCT
char *User::dir(char *ps)
{
 if(*ps==1)return ps+19;
 else if(*ps==2)return ps+36;
 return memchr(ps,0,256)+1;
};
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

  return t+sprintf(t,"+%8.8X.%8.8X",aa[0],aa[1]);
}

void UpdPwdCrypt(char *p)
{
  ulong a,b;
  if(*p=='+' && (p[9]=='Z' || p[9]== '.')  && strlen(p)==18 && (a=atouix(p+1)) && (b=atouix(p+10)) )
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
      if( (*t!='.' && a!=3) )
        goto exLp2;
      d[a]=b;
    }

    *p++ =2;
    memcpy(p,d,16);

    exLp2: ;
  }
  #endif
}

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
    {
      sprintf(bfr,"%.255s" FSLUSHS "mbox",d);
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

  char *t;
  char *x;
  char *pasw;
  char *dir;
  char bfr[264];
  if(state&UserPARSED)return 1;
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
            default:
              goto l1;
          };
          x++;
        }while(1);
        l1:;
        state|=UserPARSED;
        #ifdef NEWSTRUCT
        pwd=pasw;
        ddr=dir;
        #endif
        MkDir();
        UpdPwdCrypt(pasw);
        return 1;
      }
    }
  };
  return 0;
};


//-------
char* IsSame(char *tt,char *pp)
{
  char *z = 0;
  uchar a,b;
  a = *tt;
  do{
    b = *pp++;

    DBGLA("%u %u %s %s\n",a,b,tt,pp-1)

    if(!b)
    {
     if(a >='-' && a!=':')return 0;
     return tt;
    }
    if(a=='*')
    {
      z=++tt;
    lb_wld:
      a=*tt;

      DBGLA("%X |%s| |%s|\n",a,tt,pp)
      while(b != a)
      {
        b = *pp++;
        if(!b)
        {
          if( a < '-' || a == ':' ) return tt;
          DBGLA("a=%X %s\n", a,tt)
          return 0;
        }
      }
    }
    else if(b != a)
    {
      if(z && b)
      {
        tt=z;
        goto lb_wld;
      }
      return 0;
    }
    a=*++tt;
  }while(a=='*' || (a >= '-' && a != ':') );
  DBGLA("%X |%s| |%s|\n",a,tt,pp)
  if(!*pp) return tt;
  return 0;
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
     }
   }
   else
   {
#else
     LimitCntr *lip;
#endif
     ulong ip4;
     ip4=IPv4addr(&sa_c);
     if(!(lip=hack.Find(ip4)))
     {
         lip=hack.Push(); lip->ip=ip4;
     }
#ifdef USE_IPV6
   }
#endif
   lip->first=t;
   lip->cnt+=v;
}

#define USER_FOUND ((SysUser *) 1)
//-------
User   *FindUser(char *bfr,int typ,char *pwd /*=0*/,Req *r) //=0)
{
  union {
    User *tuser;
    SysUser *suser;
  };
 char *t,*t1,*t2;
 ulong ip;
 union{
 LimitCntr *lip;
 LimitCntrIPv6 *lip6;
 };
 int  cc;
#ifdef WITHMD5
 int md5pwd=typ&FindUserMD5digest;
 char *dgtvars[10];


 if( pwd && ( strin(pwd,"Digest") || md5pwd ) )
 {
   for(cc=0;digetvars[cc];cc++)
   {
     if((dgtvars[cc]=  PrFinVar(pwd,digetvars[cc])) )
       md5pwd|=1<<cc;
   }
   DBGLA("DEBUG  digest %X %s %X",md5pwd,bfr,typ);
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
#endif
 DBGLA("DEBUG  %s %X %X",bfr,typ,userList);
 cc=0;
 for(tuser=userList;tuser;tuser=tuser->next)
 {
  cc++;
  DBGLA("DEBUG 3 %s %s %X %X",tuser->name,bfr,tuser->state,typ);
  if( (tuser->state & UserPARSED) && (tuser->state&typ) )
  {
    if( ! strcmp(tuser->name,bfr)  )
    {
      if(pwd && r)
      {
        #ifndef WITHMD5
        if( *(t=tuser->pasw()) && ! IsPwdC(t,pwd)  )goto lbBad;
        #else

        if( *(t=tuser->pasw())  )
        {
          DBGLA("DEBUG U %s %s %X %X",tuser->name,t,tuser->state,typ);

          if(*t==1)
            r->fl|=F_DIGET_UNAVILABLE;

          if( *t==1 && (md5pwd || (typ & (FindUserMD5cram|FindUserMD5digest) ) ) )
          {
            debug("Cram and digest unavilable for %s",tuser->name);
            return 0;
          }
          if( typ == (UserPOP3|FindUserMD5cram) )
          {
            if(! IsPwdAPOP(t,pwd,r->pst,r->postsize)) goto lbBad;
          }
          else if( typ == (UserSMTP|FindUserMD5cram) )
          {
            if(! IsPwdCRAM(t,pwd,r->pst,r->postsize) ) goto lbBad;
          }
          else
          {
            if(md5pwd) {
              t1="GET";
              if(r->fl&F_POST)t1="POST";
              if(r->loc) {
                if(DWORD_PTR(r->loc[0]) == 0x4E4E4F43 x4CHAR("CONN")  ) t1="CONNECT";
                if(DWORD_PTR(r->loc[0]) == 0x44414548 x4CHAR("HEAD")  ) t1="HEAD";
              }
            }
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
              IsPwdC(t,pwd)
            ) )goto lbBad;
          }
        }
        #endif
        if (r)
          r->dir=tuser->dir(t);
        lbFound:;
        #ifdef USE_IPV6
        if( IsIPv6( & r->sa_c ) )
        {
          if( (lip6=hack6.Find(r->sa_c6.sin6_addr ) ) )
          { if(!(lip6->cnt&=~ ((0x501*cc)<<5))) hack6.Del(lip6); }
        }
        else
          #endif
          if( (lip=hack.Find( IPv4addr(& r->sa_c ) ) ) )
          { if(!(lip->cnt&=~((0x501*cc)<<5)))hack.Del(lip); }
      }
      return tuser;
    }
  }
 }
#if defined(USE_SYSPASS) && defined(SYSUNIX)
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

        if(! suser->IsPwd(pwd)) {
          r->fl|=F_DIGET_UNAVILABLE;
          goto lbBad;
        }
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

#if !defined(CD_VER)
  if(r && (r->Snd!=&TLSSend)
    && (t=strpbrk(bfr,"@#")) &&
    (s_flgs[1]&(1<<(BYTE_PTR(r->fl,2)&0xE)))
    &&
     r->IsInIPRange(proxy_range)>0
    )
  {
    ++t;
    if( (t1=strchr(t,t[-1]) ) )
    {++t1;
     if( !(t2=strpbrk(pwd,"@#") ) )
       t2=strchr(t1,t[-1]);
     if(t2)
     {t[-1]=0;
      *t2++=0;
      t1[-1]=0;

      if((tuser=FindUser(bfr,typ,pwd,r)))
      {r->fl|=F_PRX;
       r->req=t;
       r->pst=t1;
       r->trn=t2;

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
#if defined(SPECIAL) || !defined(CD_VER)
#ifdef USE_IPV6
    if(IsIPv6(&r->sa_c) )
    {
        if( (lip6=hack6.Find( r->sa_c6.sin6_addr ) ) ) goto lbHF;
        lip6=hack6.Push(); lip6->Set( r->sa_c6.sin6_addr );
    }
    else
#endif
    if( (lip=hack.Find(IPv4addr(& r->sa_c ) ) ) )
    {
     lbHF:
        if(pwd &&  lip->first !=(ip=MkName(pwd)) ){lip->cnt++; lip->first=ip;}
    }else
    {
        lip=hack.Push(); lip->ip=IPv4addr(& r->sa_c );
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
#ifdef NEWSTRUCT
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
