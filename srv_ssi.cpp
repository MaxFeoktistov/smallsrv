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
//---
#define MAX_VAR_SIZE 512

#undef send
#undef recv
#define send(a,b,c,d) Send(b,c)
#define recv(a,b,c,d) Recv(b,c)
#define CSEND(b) Send(b,sizeof(b)-1)

#ifndef SYSUNIX

#undef FindFirstFile    
#undef _lopen           
#undef GetFileAttributes

#define FindFirstFile FindFirstFile_UTF
#define   _lopen               lopen_UTF 
#define   GetFileAttributes   GetFileAttributes_UTF

#endif

// Array with error strings
char *det_var_err[]={"",
 s__UNINSPE ,
 s__UNINSPE0 ,
 s__SSI_VAR ,
"",
0
};
//---
#ifndef SPECIAL
#include "regular.cpp"
#endif
/* Server Side Includes. */

#define err_ret(a){ dbg(a);return 0;}

// Get value of server variable from varlist.
// If variable is absent in list return NULL
char * GetVar(char **varlist,char *var);

// Maximum count of HTTP variables. Used to alloc memory
//#define MAX_HTTP_VARS 60

// Pointers to strings with GMT and local time.
char gtime_bbf[30],ltime_bbf[30];
char sprt80[6];
char sprt443[6];

char *SVR[]=
{"DATE_GMT",gtime_bbf,"DATE_LOCAL",ltime_bbf,
"QUERY_STRING",0,"SERVER_PORT",sprt80,
"REMOTE_ADDR",0,"REMOTE_USER",0,"AUTH_PASS",0,
"AUTH_TYPE","Basic",0,0,0,0
};

// Prepare list of HTTP variables from request
// This list content all incoming variables and DATE_GMT and DATE_LOCAL
// Name of vareables convert to upercase.
void Req::prepare_HTTP_var()
{SYSTEMTIME st;
 register int i;
 int j;
 char *t;

 t=rq;
 for(i=0;i<(MAX_HTTP_VARS-22);i+=2)
 {
  do{   
   if( !(t=strchr(t,'\r') ) )goto brk1;  //break;
   if(t[2]!='\t' && t[2]!=' ')break;
   ++t;
  }while(1);
  *t=0; t+=2;
  http_var[i]=t;
  for(;*t!=':'; t++)
   if( (*t >= 'a') && (*t <= 'z') ) *t&=0xDF;
   else if(*t=='-')*t='_'; else if(!*t)goto brk1;
  *t=0; do{++t;}while(*t==' ');
  http_var[i+1]=t;
 };
brk1:
 GetSystemTime(&st);
 memcpy(http_var+i,SVR,sizeof(SVR) );
 sprintf(gtime_bbf,"%2.2u/%2.2u/%2.2u %2.2u:%2.2u:%2.2u",st.wDay,st.wMonth,st.wYear,st.wHour,st.wMinute,st.wSecond);
 GetLocalTime(&st);
 sprintf(ltime_bbf,"%2.2u/%2.2u/%2.2u %2.2u:%2.2u:%2.2u",st.wDay,st.wMonth,st.wYear,st.wHour,st.wMinute,st.wSecond);
 http_var[i+5]=req;
 j=16;
 if( !(http_var[i+11]=CheckAuth(http_var[i+13]=rq+8192+256)))
 {memset(http_var+i+10,0,8*sizeof(char *));
  j=10;
 }
 else
 {
   if(strin(rq+8192+256,"Digest ") )
   {
      http_var[i+15]="Digest"; 
   }
 }    
 
 //else {  debug("|%s|%s|",http_var[i+11],http_var[i+13]);}
#ifndef CD_VER
 if(Snd==&TLSSend)http_var[i+7]=sprt443;
#endif
#ifdef USE_IPV6
  IP2S(http_var[i+9]=(char *) (http_var+MAX_HTTP_VARS+2),&sa_c);
#else
 sprintf(http_var[i+9]=(char *) (http_var+MAX_HTTP_VARS+2),"%u.%u.%u.%u",
#ifndef SYSUNIX
   sa_c.sin_addr.S_un.S_un_b.s_b1,  sa_c.sin_addr.S_un.S_un_b.s_b2,
   sa_c.sin_addr.S_un.S_un_b.s_b3,  sa_c.sin_addr.S_un.S_un_b.s_b4
#else
   sa_c.sin_addr.s_addr&0xFF, BYTE_PTR(sa_c.sin_addr.s_addr,1),
   BYTE_PTR(sa_c.sin_addr.s_addr,2),BYTE_PTR(sa_c.sin_addr.s_addr,3)
#endif
 );
#endif
#ifndef CD_VER
 i+=j;
 if(trn)
 { http_var[i++]="PATH_INFO";
   http_var[i++]=trn;
 }
 if( (s_flgs[1]&FL1_IPCNTR) && country_names)
 {http_var[i]="REMOTE_COUNTRY";
  http_var[i+1]=GetCntr(sa_c.sin_addr.s_addr);
 }
#endif


};
//---
// Get value of server variable from varlist.
// If variable is absent in list return NULL
char * GetVar(char **varlist,char *v)
{if((!varlist) || !*v)return 0;
 while(*varlist)
 {if(!stricmp(*varlist,v))return varlist[1];
  varlist+=2;
 };
 return 0;
};
//---
// Get index of server variable from varlist.
// If variable is absent in list return last index.
int GetIndVar(char **varlist,char *v)
{int i;
 for(i=0;varlist[i];i+=2)if(!strcmp(varlist[i],v))break;
 return i;
};

//---
//Return count of variables in list
int  CountOfVar(char **varlist)
{int i;
 for(i=0;varlist[i];i+=2);
 return i;
};

unsigned long NullLong=0;
void Req::prepare_Req_var()
{int i;
 req_var=(char**) (void *) &NullLong;

 if(req[0])
 {char *t,**tt,*l;
  i=4;
  t=req;
  while(*t)if(*t++=='&')++i;
#ifdef x86_64 

  if(!(req_var=(char **) Malloc(i*16+(t-req)*sizeof(char *)+128 ) ) )return;
  *req_var = t = ( (char *)(void *)req_var )+i*16;

#else  
  if(!(req_var=(char **) new char [i*8+(t-req)*sizeof(char *)+64]))return;
  *req_var = t = ( (char *)(void *)req_var )+i*8;
#endif  
  

  req_var[1] =0;
  for(tt=req_var+2,l=req;*l;l++,t++)
  {switch(*l)
   {
    case '&':
     *t=0;
     if(l[1]!='&'){
     if(t[-1]==' ')t[-1]=0;
     *tt=t+1; tt++;
     *tt=0; tt++;
     break;
    case '=':
     tt[-1]=t+1; *t=0;
     break;
    case '+':
     *t=' '; break;
    case '%':
     if( (!*++l) || (!l[1]) ) goto lbExpl;
     *t=atouix2(l++);
     break;
    default:;
    }
    *t=*l;
   };
  };
 lbExpl:
  *t=0;
  *tt=tt[1]=0;
 };
}
#ifndef SPECIAL

// This method prepare all vars and call to SendSSIEx()
int Req::SendSSI()
{char *http_v[MAX_HTTP_VARS+12];
 int i;
 memcpy(http_v,http_var,sizeof(http_v));
 http_var=http_v;
 prepare_Req_var();
 //prepare_HTTP_var();
 SendSSIEx();
 if( (req_var!= (char**)  &NullLong))
#ifdef x86_64
     free(req_var);
#else     
     delete req_var;
#endif     
 return 0;
};
//---

/*
// Delete space from string to logical analyses
void DelSpace(char *s)
{register char  *t;
 for(t=s;(*t=*s);s++)if(!strchr(" \t\r\n",*t))t++;
};
*/
// Delete space from string to logical analyses
void DelSpaceRe(char *s)
{register char  *t;
 char b,c;   
 for(t=s;(*t=*s);s++)
 {
     if(*t=='\'' || *t=='\"' || (*t=='/' && t[-1]=='~' ))
     {
      dprint("Quote skip space %.64s\n",s); 
       b=*t++;
       do{
         c=*t++=*++s;           
         if(!c)return;
       }while( (c!=b || IsSeq(t-1) ));
       //s--;
       dprint("End Quote skip space %.64s %X %X\n",s,c,b); 
       
     }    
     else 
         if(!strchr(" \t\r\n",*t))t++;
 }   
};

#if 0
// Logical analyses of string, to detect false or true.
// string could include  next operations: >,<,==,!=,>=,<=,&&,||
// and ~ mean subinclude.  Sequense of operation could be change
// with help ().
int LogAn(char *ls,int c)
{int rval=0;
 char b,*t2;
 if(c>32)return 0;
 if(!c)DelSpace(ls);
 c++;
 if(*ls=='!')return ! LogAn(ls+1,c);
 if(*ls=='(')
 {if(!(t2=strchr(ls,')'))) return 0;
  *t2=0; t2++;
  ls++;
  b=*t2;
sw:
  rval=LogAn(ls,c);
  switch(b)
  {
   case '|': return rval || LogAn(t2+2,c);
   case '&': return rval && LogAn(t2+2,c);
   case '=': return (rval == LogAn(t2+1+(t2[1]=='='),c));
   case '!': return (rval != LogAn(t2+2,c));
  };
  return rval;
 }
 if( (t2=strpbrk(ls,"&|")) )
 {b=*t2; *t2=0;
  goto sw;
 };
 if((t2=strchr(ls,'>')))
 {*t2=0; t2++;
  if(*t2== '=')
  return strcmp(ls,t2+1) >=0;
  return strcmp(ls,t2) >0;
 }
 if( (t2=strchr(ls,'<')) )
 {*t2=0; t2++;
  if(*t2== '=')return strcmp(ls,t2+1) <=0;
  return strcmp(ls,t2) <0;
 }
 if( (t2=strchr(ls,'!') ) )
 {*t2=0;
  return strcmp(ls,t2+2) ;
 }
 if((t2=strchr(ls,'~')))
 {*t2=0;
  return (strstr(ls,t2+1)!=0);
 };
 if((t2=strchr(ls,'=')))
 {*t2=0;
  if(t2[1]== '=')t2++;
  return !strcmp(ls,t2+1) ;
 }
 return strcmp(ls,"undefined")&&*ls && WORD_PTR(*ls)!='0';
};
#endif
//----
char *NextIf(char *t,char *f,char *f1,char *f2) //="<!--#if",char *f2="<!--#endif " )
{char *r,*x,*ff;
 int i=0;
 do
 {
  if(!i)ff=f;
  while((r=strstr(t,ff)) && (x=strstr(t,f1)) && x<r)
  {++i; ff=f2; t=x+1;}
  t=r+1;
 }while(i-- > 0 && r);
 return r;
}

int Req::SendSSIEx()
{char *p,*t,*t1,*t2,*t3,*t4;
 int h,l,i;
 int loc_var=CountOfVar(http_var);
 int lvar=loc_var;
 char *bbf,*bbf1,*vdir,*fend;
 host_dir *a;
 char fname[768];
 Req r1;
 RegVars rv;
 if((freqcnt&0x1F)>9)err_ret( sTO_MANY_I );
 rv.Init(http_var,req_var,this);
 memcpy(&r1,this, sizeof(r1) );
#define SubstVar(a,b,c,d) rv.SubstVar(a,b)
 if((h=_lopen(loc,0))<0)
  if(!(freqcnt&0x1F)) return HttpReturnError( sERROR__FI );
  else
  {if(send(s,loc,strlen(loc),0)<0)goto er1;
   if(CSEND( s__FILE_NO )<0)goto er1;
   return 1;
  };
 l=FileSize(h);
#ifdef x86_64
 p=(char *)Malloc(l+48+MAX_VAR_SIZE*4);
#else 
 p=new char[l+24+MAX_VAR_SIZE*4];
#endif 
 if(!p)return HttpReturnError( sERROR__NO );
 bbf1=(bbf=p+l+12)+MAX_VAR_SIZE;
 _hread(h,p,l);
 _lclose(h);
 fend=p+l;
 *fend=0;
 if(!(freqcnt&0x1F))
 {
  i=sprintf(bbf,"HTTP/1.0 %u Ok\r\nContent-Type: text/html\r\n"
  "%s\r\n"
  "\r\n",(loc!=error_file)?200:404,header);
  if( (fl&(F_GZ)) &&  l>GZSizeLow  && IGZP() )
  {memcpy(bbf+i-2,gz_head,sizeof(gz_head));
   i+=sizeof(gz_head)-2;
  }
  else fl&=~(F_GZ);
  if( (s_flgs[2]&FL2_CHUNKED) && Snd!=&TLSSend && ! (fl&F_HTTP10) )
  {
     bbf[7]='1';
     i+=sprintf(bbf+i-2,"%sTransfer-Encoding: chunked\r\n\r\n",(KeepAlive)?"Connection: Keep-Alive\r\n":"" )-2;

    //debug("HTTP 1.1 %X %X",fl,(fl&F_HTTP10));
  }
  if(send(s,bbf,i,0) < 0 )
  {er1: 
#ifdef x86_64
     free(p);
#else     
     delete p; 
#endif     
      
      err_ret("Connection reset by peer...");
  };
  if( (s_flgs[2]&FL2_CHUNKED) && Snd!=&TLSSend && ! (fl&F_HTTP10) ) fl|=F_CHUNKED;
 }
 if(gz)
 {Snd= GZSnd;
  r1.Snd= GZSnd;
  r1.gz=gz;
 }

 for(t=p;(t1=strstr(t,"<!--#"));)
 {a=0;
  if(t!=t1){ if(Send(t,t1-t)<0)goto er1; };
  t1+=5;
  if((!(t=strstr(t1,"-->"))) /* || (t-t1) > */ )
  {
    er2:
#ifdef x86_64
     free(p);
#else     
     delete p; 
#endif     
      err_ret( sUNINSPECT );
  };
  *t=0;
  if(strin(t1,"fsize "))
  {
   i=1;
   goto lb1;
  }
  if(strin(t1,"lastmod ") || strin(t1+1,"lastmod ")  )
  {i=2;
   goto lb1;
  }
  if(strin(t1,"include ") || strin(t1,"exec ") )
  {i=0;
  lb1:
   a=0;
   if((t2 = strstr(t1,"file=\"")))
   {t2+=6;
   lf1:
    fname[0]=0;
    if(*t2!='/' && *t2!='\\' /* && t2[1]!=':' */ )
    { if( (t3=strrchr(loc,FSLUSH)) )strncpy(fname,loc,t3-loc+1 ); }
   }
   else if((t2=strstr(t1,"virtual=\"")))
   {t2+=9;
    for(a=hsdr.next;a;a=a->next)if(a->h[0]=='/')
     if((vdir=stristr(t2,a->h)))
     {t2=vdir+strlen(a->h);
      strcpy(fname,a->d);
      goto vdirfnd;
     }
    strcpy(fname,dir);
    if(t2[0]!='/')goto lf1;
   }
   else if( ( t2 = strstr(t1,"cgi=\"") ) || ( t2 = strstr(t1,"cmd=\"") ) )
   {t2+=5;
    if( fname[(l=sprintf(fname,"%.255s"
#ifndef SYSUNIX
     FSLUSHS "%.255s",dir,
      cgi_detect+(cgi_detect[0]==FSLUSH)))-1] !=FSLUSH )
#else
    ,dir))-1] !=FSLUSH )
#endif
     DWORD_PTR(fname[l])= FSLUSH;
   }
   else{dbg( sUNINSPECT0 ); goto cnt;}
vdirfnd:
   if((t3=SubstVar(bbf,t2,http_var,req_var))){ send(s,t3,strlen(t3),0); goto cnt; }
   t4=0;
   for(t3=bbf;WORD_PTR(*t3)==0x2e2e;t3+=3)
   {do{if(!(t4=strrchr(fname,FSLUSH)))goto lbBN; *t4=0;}while(!(t4[1]));}
   if(t4)WORD_PTR(*t4)='/';
   strncat(fname,t3,255);

#ifndef SYSUNIX
//   for(t3=fname;*t3;t3++)if(*t3=='/')*t3=FSLUSH;
#endif
   if(! (t3=strchr(fname,'?'))) t3=req;
   else{*t3++=0;}
   if(!i)
   {i=GetIndVar(http_var,"QUERY_STRING");
    if(http_var[i])http_var[i+1]=t3;
    r1.loc=fname;
    r1.req=t3;
    r1.Tout=0;
    r1.Tin =0;
    if( CheckBadName(fname) && ((striin(fname,a?a->d:dir))))
    {
     if( (i=IsCGI(fname,5))&1 )
     {
      r1.fl=(fl)|F_SKIPHEAD;
      r1.freqcnt=freqcnt;
      if( !(r1.ExecCGIEx()) )goto er1;
     }
     else
     {
       r1.fl=fl;
       r1.freqcnt=freqcnt+1;
#ifdef SYSUNIX
       if( !(i&4) )
       {
        if( Send(loc,strlen(loc))<0)goto er1;
        if(CSEND( s__FILE_NO )<0)goto er1;
       }
       else
#endif


       if(
#ifndef SYSUNIX
        (
         (s_flgs[1]&(FL1_RUNSYSTEM|FL1_DNTSHOWHIDEN)) &&
         !(i&4)
        )  ||
#endif
           !(r1.SendSSIEx()) )goto er1;
      }
     Tout+=r1.Tout;
     Tin +=r1.Tin;
    }
    else{lbBN: CSEND( s___BAD_NA );}
    if(http_var[i])http_var[i+1]=req;
   }else
   {
       
#ifndef SYSUNIX 
union{ 
#endif    
 WIN32_FIND_DATA fd;
#ifndef SYSUNIX 
 WIN32_FIND_DATAW fdw;
};
#endif    
    HANDLE hf;
    if((hf=FindFirstFile(fname,&fd) )!=INVALID_HANDLE_VALUE)
    {FindClose(hf);
     if(i==1)
     {
       i=
         strstr(t1," Kb")? sprintf(fname,"%uK",(ulong)(fd.nFileSizeLow+512)>>10 ) :
         strstr(t1," Mb")? sprintf(fname,"%uM",(ulong)(fd.nFileSizeLow+0x80000)>>20 ) :
         sprintf(fname,"%u",(ulong)fd.nFileSizeLow);
     }    
     else
     {SYSTEMTIME st;
      FileTimeToSystemTime(
#ifdef ANDROID
     (time_t *)
#endif
      &fd.ftLastWriteTime,&st);
      if( (t3=strstr(t1," format=\"") ))
      {
        i=0;
        t3+=sizeof(" format=\"")-1;
        while(i<128 && *t3!='"')
        {
          t4="%2.2u";   
          switch(*t3)
          {
              
              case 'a':  fname[i++]=(st.wHour<12)?'a':'p' ;  fname[i++]='m'; break;
              case 'A':  fname[i++]=(st.wHour<12)?'A':'P' ;  fname[i++]='M'; break;

                  
              case 'j':  t4="%u";
              case 'd':  h= st.wDay; if(0){ 
              case 'n':  t4="%u";    
              case 'm':  h= st.wMonth; if(0){ 
              case 'Y':    
              case 'o':  h= st.wYear; if(0){ 
              case 'y':  h= (st.wYear)%100; if(0){ 
              case 'g':  t4="%u";    
              case 'h':  h= (st.wHour<=12)?st.wHour:st.wHour-12; if(0){ 
              case 'G':  t4="%u";    
              case 'H':  h= st.wHour; if(0){ 
              case 'i':  h= st.wMinute; if(0){ 
              case 's':  h= st.wSecond; 
              }            }}}}}}
              i+=sprintf(fname+i,t4,h); break;
              
              default: fname[i++]=*t3;
          }
          t3++;
        }    
      }
      else
        i=sprintf(fname,"%2.2u/%2.2u/%2.2u %2.2u:%2.2u:%2.2u",st.wDay,st.wMonth,st.wYear,
        st.wHour,st.wMinute,st.wSecond);
     };
     if(send(s,fname,i,0)<0)goto er1;
    };
   }
  }
  else if( strin(t1,"echo ") )
  {if( (t2=strstr(t1,"var=\"") ) )
   {t2+=5;
    if( (t3=SubstVar(bbf,t2,http_var,req_var))){ send(s,t3,strlen(t3),0); goto cnt; };

    if(!strchr_meta(t2,'$'))
    {if(!(t3=GetVar(http_var,bbf)))
     if(!(t3=GetVar(req_var,bbf)))
     if(!( (DWORD_PTR(bbf[1]) == 0x5F505454 x4CHAR("TTP_")) &&
        (t3=GetVar(http_var,t2+5)))
       )t3="undefined";
    }
    else t3=bbf;
    if(t3 && (i=strlen(t3))) if(send(s,t3,i,0)<0)goto er1;
   }
  }else if(strin(t1,"printenv "))
  {
#if 0
   char *xbbf, *pbbf, **tv;
   if((xbbf=new char[0x3000]))
   {strcpy(xbbf,"<br><b>HTTP:</b><br>");
    pbbf=xbbf+22;
    for(tv=http_var;*tv;tv+=2)pbbf+=sprintf(pbbf,"%s=%s<br>",*tv,tv[1]);
    if(i=send(s,xbbf,pbbf-xbbf,0)>0)
    if(*req_var)
    {pbbf=xbbf+sprintf(xbbf,"<br><b>Request:</b><br>");;
     for(tv=req_var;*tv;tv+=2)
      pbbf+=sprintf(pbbf,"%s=%s<br>",*tv,tv[1]);
     i=send(s,xbbf,pbbf-xbbf,0);
    };
    delete xbbf;
    if(i<0)goto er1;
   }
#else
   rv.PrintEnv(this);
#endif
  }else if( (DWORD_PTR(*t1)&0xFFFFFF) == 0x206669 x4CHAR("if ") )  //strin(t1,"if ") )
  {
  lif:
   if( (t2=strstr(t1,"expr=\"") ) )
   {t2+=6;
//    if( (t3=SubstVar(bbf,t2,http_var,req_var))){ send(s,t3,strlen(t3),0); goto cnt; };
//    if(! LogAn(bbf,0))
    if(! rv.LogAn(t2-1))
    {*t='-';
      if( ! (t3=NextIf(t,"<!--#endif ") ) )
      {if( CSEND( s___ENDIF_ )<0)goto er1;
       goto exdoc; //cnt;
      }
     if( (!(t4=NextIf(t,"<!--#el"))) || t4>t3  )t4=t3;
     t4+=7;
     if( ! (t=strstr(t4,"-->")) ) goto er2;
     if( WORD_PTR(*t4)==0x6669 )//elif
     {t1=t4; goto lif;}
    }
   }
  }else if( t1[4]==' '&&
   ( DWORD_PTR(*t1) == 0x65736C65 x4CHAR("else") || DWORD_PTR(*t1) == 0x66696C65 x4CHAR("elif")
   )  ) //(strin(t1,"else ") ) || (strin(t1,"elif "))  )
  {*t='-';
//   if( !(t3=strstr(t,"<!--#endif ") ) )
   if( !(t3=NextIf(t,"<!--#endif ") ) )
   {if(CSEND( s___ENDIF_ )<0)goto er1; //goto exdoc;
   }
   else{if( !(t=strstr( t3 ,"-->" ) ) )goto er2; }
  }
  else if( DWORD_PTR(*t1) == 0x20746573 x4CHAR("set ") ) //strin(t1,"set ") )
  {if( (t2=strstr(t1, "var=\"") ) )
   {t2+=5;
    if( (t3=SubstVar(bbf,t2,http_var,req_var))){ send(s,t3,strlen(t3),0); goto cnt; };
    i=GetIndVar(http_var,bbf);
    if( !http_var[i])
    {if( (i>MAX_HTTP_VARS-4) ||
         ( ( t3=  bbf1 + strlen(bbf)  ) >= (bbf + MAX_VAR_SIZE*3) )
       )
     {er3: CSEND( s__TO_MANY ); goto cnt;};
     strcpy( (http_var[i]=bbf1),bbf);
     bbf1=t3+1;
     http_var[++lvar]=0;
     http_var[++lvar]=0;
    }
    if( (t2=strstr(t1, "value=\"") ) )
    {t2+=7;
     if( (t3=SubstVar(bbf,t2,http_var,req_var))){send(s,t3,strlen(t3),0); goto cnt; };

     if( (t3=bbf1+strlen(bbf)) >= (bbf + MAX_VAR_SIZE*3) )goto er3;
     strcpy( (http_var[i+1]=bbf1),bbf);
     bbf1=t3+1;
    };
   }
  }
  else if( strin(t1,"break ") )goto exdoc;
 cnt:
  *t='-';
  t+=3;
 }
 if((l=fend-t)>0) send(s,t,l,0);
exdoc:
#ifdef x86_64
     free(p);
#else     
     delete p; 
#endif     
 
 http_var[loc_var]=0;
 return 1;
};
#endif
#undef send
#undef recv
#undef SubstVar

#ifndef SYSUNIX

#undef   _lopen            
#undef   GetFileAttributes
#undef   FindFirstFile    

#define   _lopen              lopen           
#define   GetFileAttributes   GetFileAttributesA
#define   FindFirstFile       FindFirstFileA

#endif
