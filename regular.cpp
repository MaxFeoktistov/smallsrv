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




#ifndef SRV_H
#include "srv.h"
#endif

#ifndef SYSUNIX
#include "strtoul.cpp"
#endif

#define DBG()
//debug("D %d", __LINE__ )


//#include <stdlib.h>



char *strchr_meta(char *p,int c)
{
 int i;
 --p;
 do{ if(! (p=strchr(p+1,c)) )return 0; i=1; while(p[-i]=='\\')++i; }while(!(i&1));
 return p;
}

int SlashSeq(char * &b)
{
 uint d=8,c;
 char *t;
  t=b+3;
  switch(*(uchar *)b)
  {
   case 'n': return '\n';
   case 'r': return '\r';
   case 't': return '\t';
   case 'e': return 27;
/*
   case 'I': st&=~0x20; if(0){
   case 'i': st|=0x20;       }
             ++b;
             return (*b++=='\\')?SlashSeq(b):b[-1];
*/
   case 'X':
   case 'x': ++b;  d+=6;
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9': d+=2;
   if(0) {
   case '0':
   case 'O':
   case 'o': ++b; ++t; };
     c=*t; *t=0;
     d=strtoul(b,&b,d);
     *t=c;
     if(!d)d=0xFF;
     --b;
     return d;
  }
  return *b; //++;
}

char *SkipSpace(char *a){
// while(*a && strchr(" \r\n\t",*a) )
 while( *a && *(uchar *)a <=' ' ) ++a;
 return a;
}

int IsInGrp(char *b,int c)
{
 int pre=0,st,a,d;
 st=0;
 if(*b=='^'){++b; ++st;};
 while(*b)
 {
  switch(a=*(uchar *)b)
  {
   case ']':
   case  0:
    return st;
   case '-':
    a=*++b;
    if(a=='\\')a=SlashSeq(++b);
    if(pre<=c && a>=c)return !st;
    break;
   case '\\': a=SlashSeq(++b);
   default:
    if(a==c)return !st;
  }
  pre=a;
  ++b;
 }
 return st;
}


int RegVars::isVar(char *&a,char **xx)
{
 char *p,*e,*t,b;
 int i,j;
 j=0; //st&RV_FLG_SUB;
 lb2:
 p=a;
 //debug("====%X %X {%X,%X,%X,%X} %X",p,xx,*xx,xx[1],xx[2],xx[3],((ulong *)&xx)[-3]);
 //debug("====%.5s====",p);
  if(*xx)
  {
   if(xx[2])
   {
    if(p==xx[1]) xx[2]=0;
    else {xx+=2; j=1; }
   }
   if( (!*p) || (p==xx[1]) )
   {
    a=*xx;
    *xx=0;
    return 2;
   }

   if(!j)
   {
    xx+=2;
    j=1;
    goto lb1;
   }
  }
  else
  {
 lb1:
   for(i=1;p[-i]=='\\';++i);
   if(!(i&1))return 0;
   if(*p=='$')
   {
    b=*++p;
    xx[1]=0;
    e=p+1;
    if( (b>='1' && b<='9') || b=='+' )
    {
      if(b=='+' && nrv)b=nrv-1;
      else b-='1';
      if(b>=nrv)
      {
       t="";
      }
      else
      {xx[1]=(t=rv[b].v)+rv[b].l;
      }

    }
    else if(b=='`'){
     if((!nrv) || !last_before)t="";
     else
     {
      b=nrv-1;
      t=last_str;
      xx[1]=rv[b].v;
     }
    }
    else if(b=='\''){
     if(!nrv)t="";
     else
     {
      b=nrv-1;
      t=rv[b].v+rv[b].l;
     }
    }
    else
    {
     if(b=='{')
     {if(! (e=strchr_meta(p,b='}')) )return 0;
      ++p;
      *e=0;
     }
     else
     {
      if((e=strpbrk(p," \t,;.+?*%^#@:/<>=&|!~\n\r$()[]{}~\\\"\'"))
         || (e=memchr(p,0,0x200000))
        )
      {b=*e;
       *e=0;
      }
     }

     if(! (t=GetVar(vl1,p) ) )
     if(! (t=GetVar(vl2,p) ) )
     if(! ( DWORD_PTR(p[1]) == 0x5F505454 x4CHAR("TTP_") ) )
      t=GetVar(vl1,p+5);

     if(e)*e=b;
     {
      if(p[-1]=='{')++e;
     }
    }
    if(!t)t="";
    *xx=e;
    a=t;
    if( (!*t) || !j)
    {

     goto lb2;
    }
    return 1;
   }
   else if(*p=='\\' && !j )
   {
     DWORD_PTR(xx[1])=SlashSeq(*xx=++p);
     ++*xx;
     a=(char *)(xx+1);
   }
  }
  return 0;
};

int RegVars::IsRegular(char *s0,char *p)
{
 register char *s=s0;
 int lastst,a,bracket,i,min,max,j,k;
 char *t,*lsb=0,*lb,*e,*bst;

//debug("e %.12s =~ /%.12s/ %X",s0,p,((ulong *)&s0)[-2] );

 if(++req_counter>32)
 {
debug("**Expresion: Too many condition: %.12s =~ /%.12s/ ",s0,p);
  goto exLp0;//  return 0;
 }
 lastst=1;
 bracket=0;
// debug("IRE  |%s| =~ |%s|",s,p);
 while(1)
 {
   if(*p!='\\')isVar(p,bb);
   if(!(a=*(uchar *)p))break;
   isVar(s,aa);
   //if(!*s){
   //if(a == '*' ||  (a=='+' && lastst)   )break;
   // goto exLp0;
   //}
   if(bb[0])     // If this part of patern is part of variable
                 // then don't interpolate it.
   {
    if(*(uchar *)s!=a && (*(uchar *)s!=((st&0x20)^a))) goto exLp0;//return 0;
    s++;
   }
   else switch(a)
   {
    case '^':
      if( s[-1]!='\n' && s[-1] ) goto lbNE;
    case '?':
      break;
    case '.':
      if( *s=='\n' || *s=='\r' || !*s ) goto lbNE;
      ++s;
      break;
    case '|':
     if(lastst)
     {// Skip next expresion
      ++p;
      if( (a=*p=='[') || *p=='(' )
      {
       if(! (p=strchr_meta(p,1+a+*p)) )
       {
        // unclosed bracked
        goto exLp0;//return 0;
       }
      }
      else if( *p=='{' )
      {
       if(! (p=strchr_meta(p,'}')) )
       {
        // unclosed bracked {}
        goto exLp0;//return 0;
       }
      }
      else
      { if(!(t=strpbrk(p,"|[]{}().*+^$"))) goto exLp;
        p=t-1;
      }
      break;
     }
     lastst=1;
     break;
    case '[':
     lsb=++p;
     if(! (p=strchr_meta(p,']')) )goto exLp0;//return 0;
     if(!IsInGrp(lsb,*s))  goto lbNE;
     ++s;
     break;
    case ')': goto exLp;
    case '(':
     lb=++p;
     if(! (p=strchr_meta(p,')')) )goto exLp0;//return 0;
//     *p=0;
DBG();
     i=IsRegular(s,lb);
//     *p=')';
//  debug("() %d s='%.12s' lb='%.12s' ns='%.12s'",i,s,lb,s+i);
     if(!i) goto lbNE;
    if(!(st&RV_FLG_SUB))
    {rv[nrv].v=s;
     rv[nrv].l=i;
//     last_str=s0;
//     last_before=s-last_str;
     last_before=pos;
     if(nrv<9)++nrv;
    }
     s+=i;
     //++p;
     break;
    case '{':
      if(! (t=strchr_meta(p,'}')) ) goto exLp0;//return 0;
      e=p;
      max=min=strtoul(p+1,&p,10);
      if(*p=',')
      {max=strtoul(p+1,&p,10);
       if(!max)max=0x7FFFffff;
      }
      p=t;
    if(0){
    case '+': min=1; if(0){
    case '*':
      min=0; } max=0x7FFFffff;
      e=p;
    }
    a=*(uchar *)e;
//if(lsb)debug("**** |%.10s| |%.10s|",lsb-1,e-1);
    *e=0;
    t=e-1;
    if(*t==')')t=lb-1;
    if(*t==']')t=lsb-1;
//debug("**** |%.10s| ****",t);
//    if(p[1]=='?')st|=;else st&=~;



    for(j=lastst;j<min;++j)
    {
DBG();
     if(! (i=IsRegular(s,t)))
     {*e=a;
      if(p[1]=='|')lastst=0;
      else if(p[1]!='*' && p[1]!='?' ) goto exLp0;//return 0;
     }
     s+=i;
    }
    lastst=1;
    bst=0;
    if((!min) && j && p[-1]!=')' )
    {--s; }

    while(j<max)
    {if(!*s)break;
//debug("j=%d %X %d |%.12s| |%.12s| |%.12s|",j,s,s-s0,t,s,p+2);
     i=(p[1]=='?')+1;
     k=st;
     st|=RV_FLG_SUB;
     i=IsRegular(s,p+i+(p[i]==')') ) ;
     st=k;
     if(i){ //s+=i;
//debug("fff %i '%.12s' '%.12s'",i,s,s-1);
     //goto exLp;
      if(p[1]=='?') break;
      bst=s;
     }
     if(! (i=IsRegular(s,t)))break;
//debug("fff %d '%.12s' '%.12s'",i,s,s+i);
     s+=i;
     ++j;
    }

    *e=a;
    if(bst)s=bst;
//    ++p;
    if(p[1]=='?')++p;
    break;
    case '\\': if(!*++p)goto exLp;
     a=SlashSeq(p);
    lbDef:
    default:
     if(*(uchar *)s!=a && (*(uchar *)s!=((st&0x20)^a)))
     {
      if((t=strpbrk(p+1,"|[]{}().*+^$")) && *t == '|')p=t-1;
    lbNE:
//debug("No equ:%u '%c' %.50s %.50s",nrv,a,s,p);
       if(p[1]=='|' || p[1]=='{' || p[1]=='*' )lastst=0;
       else if( /* p[1]!='*' && */  p[1]!='?' )goto exLp0;// return 0;
//debug("Still:%.50s %.50s",s,p);

       break;
     }
     if(*s)s++;
   };
  p++;
 }
exLp:
 --req_counter;
 return s-s0;
exLp0:
 --req_counter;
 return 0;
}


char * RegVars::FindRegular(char *s,char *p)
{
 char *t,*x[4];
// debug("|||%X|||%.128s %.128s",st,s,p);
 if( (t=FindEnd(p)))
 {
  while(*t && strchr("gismxvc",*++t) )
  {
   if(*t=='i'){st|=0x20;}
   if(*t=='v'){st&=~RV_FLG_NEW;}
   if(*t=='c'){nrv=0;}
  }
 };
 if(st&RV_FLG_NEW)
 {nrv=0;
  st&=~RV_FLG_NEW;
 }
 last_str=s;
 pos=0;
 last_before=0;
 do
 {isVar(s,aa);
  if(!*s)break;
  memcpy(x,aa,sizeof(x));
  if(IsRegular(s,p))return s;
  memcpy(aa,x,sizeof(x));
  ++s;
  ++pos;
 }while(1);
 return 0;
}

ulong isInt(char *a, char &i)
{
 ulong r;
  r=strtoul(a,&a,0);
  if(!a){i=0; return 0xFFFFffff;}
  while(*a == ' ' )++a;
//  return (*a)?0xFFFFffff:r;
  if(*a)
  {
    i=0;
    return 0xFFFFffff;    
  }    
  i=1;
  return r;
}

char* RegVars::FindEnd(char *&t)
{
 register char *a=t,*b=0;
// while(*a ==' ' )++a;
 a=SkipSpace(a);
 if( strchr("/\'\"`",*a) )
 { if((b=strchr_meta(a+1,*a)))
   {last_char=*a++;
    *b=0;
    end=b;
   }
 }
// else { }
 t=a;
 return b;
}


int RegVars::cmp(char *a,char *b)
{
 int r;
 ulong la,lb;
 char lai,lbi;
//  while(*a ==' ' )++a;
//  while(*b ==' ' )++b;
  a=SkipSpace(a);
  b=SkipSpace(b);
  la=isInt(a,lai);
  lb=isInt(b,lbi);
// debug("cmp |%s|<=>|%s| %d %d",a,b,la,lb);
//  if( (la!=0xFFFFffff) && (lb!=0xFFFFffff) )return la-lb;
  if( lai && lbi )return la-lb;
  do{
   isVar(a,aa);
   isVar(b,bb);
   r=*a-*b;
   if(*a && *a<' ')r+=' '-*a;
   if(*b && *b<' ')r-=' '-*b;
   if( r )
   {
    if(!*a)
    {
     while( *b<=' ' )
     {
      if(!*b)return 0;
      isVar(++b,bb);
     }

    }
    if(!*b)
    {
     while(*a <=' ' )
     {
      if(!*a)return 0;
      isVar(++a,aa);
     }
    }
    return r;
   }
   if(!*a)return 0;
   if(!*b)return 0;
   ++a;
   ++b;
  }while(1);
};

char * RegVars::isFunc(char *ls)
{
  char *e1,*e2,e,*lss;
  int  r1,v;
  char bb[512];
#ifdef SYSUNIX
  struct stat st;  
#else
  WIN32_FIND_DATA fnds;  
#endif
  lss=ls;
  if(strin(ls,"exist(") || strin(ls,"fsize(")  || strin(ls,"ftime(")  || strin(ls,"fmode(") )
  {
   ls+=sizeof("exist(") -1;
     
   e=')';
   if(*ls == '"'){ls++ ; e='"'; }   
   e1=strchr_meta(ls,e);
   if(!e1)
   {
  lbExistErr:     
      debug("***Error in script. Unclosed function \"%.6s\"...",lss);
      return ls;
   }   
   *e1=0;
   if(req && *ls!='/' && (e2=strrchr(req->loc,FSLUSH)) )
   {
         
        *e2=0;    
        sprintf(bb,"%.255s" FSLUSHS "%.255s" , req->loc, ls);
        *e2=FSLUSH;    
   }    
   else
     sprintf(bb,"%.255s" FSLUSHS "%.255s" , req->dir, ls+1);
   
   if(!CheckBadName(bb))
   {
       debug("***Error in script. Bad filename in \"%.6s\"...",lss);
       return ls;
   }    
   
#ifndef SYSUNIX
   
   r1=     //  (int)(GetFileAttributes(bb));
       (int)FindFirstFile(bb,&fnds);
   if(r1>=0)FindClose((HANDLE)r1);    
#else
   r1=       stat(bb,&st );
#endif 

    ls=e1; 
    *e1=e;
 
    if(e!=')')
    {
     e1=strchr_meta(ls+1,')');
     if(!e1)goto lbExistErr;
     ls=e1;    
    } ;
    
    e2="%u";
    switch(lss[1])
    {
        case 'm':  if(r1>=0){ e2="0%9.9o"; 
#ifdef SYSUNIX            
            v=st.st_mode;  
#else
            v= fnds.dwFileAttributes;
#endif            
            
            goto Lc;
        case 's':  if(r1>=0){ 
#ifdef SYSUNIX            
            v= st.st_size;
#else
            v= fnds.nFileSizeLow;
#endif            
             goto Lc;
        case 't':  if(r1>=0){ 
#ifdef SYSUNIX            
            v= st.st_mtime; 
#else
            v= FileTime2time(fnds.ftLastWriteTime);
#endif            
        Lc:
         r1=sprintf(bb,e2,v);
         ls=e1-r1+1;
         if(ls<lss)ls=lss;
         memcpy(ls,bb,r1);
         break;
        }  }  }
      default:    
    *e1= (r1>=0)?'1':'0';
    }
         
  }    
  return ls;  
}

int RegVars::LogAn(char *ls)
{
 union {   
 int rval;
 char *prval;
 };
 char b,*t2;

 rval=0;

 if( (req_counter)>=32)
 {debug("**Expresion: Too many condition %.20s..",ls);
  return 0;
 }
 if(!req_counter)
 {
  st|=RV_FLG_NEW;
  memset(aa,0,16);
  memset(bb,0,16);
 }
 ++req_counter;
 FindEnd(ls);
 ls=SkipSpace(ls);
// debug("%u %u<<<<#%s#>>>>",req_counter,nrv,ls);

 if(*ls=='!'){--req_counter; return ! LogAn(ls+1);}
 if(*ls=='(')
 {b=1;
  t2=ls;
  do{
   if(!(t2=strpbrk(t2+1,"()")))
   {--req_counter;
    debug("***Error in script. Unclosed bracket %d",b);
    return 0;
   }
   if(*t2==')')--b; else ++b;
  }while(b>0);
  *t2=0;
  //do{t2++;}while(*t2 ==' ' );
  t2=SkipSpace(t2+1);
  ls++;
  b=*t2;
sw:
  rval=LogAn(ls);
  *t2=b;
  switch(b)
  {
   case '|': rval = rval || LogAn(t2+2); break;
   case '&': rval = rval && LogAn(t2+2); break;
   case '=': rval = (rval == LogAn(t2+1+(t2[1]=='='))); break;
   case '!': rval = (rval != LogAn(t2+2)); break;
   case '>': if(t2[1]=='=') ++rval; rval = (rval > LogAn(t2+2)); break;
   case '<': if(t2[1]=='=') --rval; rval = (rval < LogAn(t2+2)); break;
  };
 }
 else
 {
  ls=isFunc(ls);   
//  debug("  %u %u<<<<#%s#>>>>",req_counter,nrv,ls);
  t2=ls-1;
  while( (t2=strpbrk(t2+1,"|&") ) )
  {
   b=1; while(t2[-b]=='\\')++b;
   if(t2[1]==*t2 && (b&1))
   {b=*t2;
    *t2=0;
    goto sw;
   }
  }
  if( (t2=strpbrk(ls,"<=>~") ) )
  {b=*t2;
   *t2=0;
   if(b=='~' || t2[1]=='~')
   {
    if(t2[1]=='~')++t2;
    else
    { bb[0]="";
      bb[1]=0;
    }
//    while(*t2==' ')++t2;
//    rval=(int) FindRegular(ls,t2+1);
    prval=FindRegular(ls,t2+1);
   }
   else
   { if(t2[-1]=='!' && b=='=') {t2[-1]=0;}
//     rval=cmp(ls,t2+1+(t2[1]=='=') );
     rval=cmp(ls,isFunc(SkipSpace(t2+1+(t2[1]=='='))) );
     
//  debug("|%s| |%s| %u",ls,t2+1+(t2[1]=='='),rval);
     if(!rval)
     {if( t2[1]=='=' || b=='='  )rval=1;}
     else if(rval<0)
     { if(b!='<' )rval=0; }
     else if( b!='>' )rval=0;
     if(!(t2[-1]) )
     {t2[-1]='!';
      rval=!rval;
     }
   }
  }
  else
  { isVar(ls,aa);
    rval = isInt(ls,b)  ;
//    if( (rval = isInt(ls)) ==0xFFFFffff ) rval =cmp(ls,"undefined");
    if( !b ) rval =cmp(ls,"undefined");
  }
 }
 --req_counter;
 //debug("## ret %d $$",rval);
 return rval;

};

char * RegVars::SubstVar(char *t,char *s)
{
 char *t2,*tm=t+MAX_VAR_SIZE;
// if(!(t2=strchr_meta(s,'\"')))return det_var_err[1];
 t2=strchr_meta(s,'\"');
 aa[0]=0;
 aa[2]=0;

 do{
  isVar(s,aa);
  if( (s==t2) || !*s)break;

  //*t++=(*s=='\\')?SlashSeq((++s)):*s++;
/*
  if(*s=='\\')
  {++s;
debug("@%X %s",s,s);
   *t++=SlashSeq(s);
debug("@!%X %s",s,s);
  }else
*/
  *t++=*s++;

 }while(t<tm);
 *t=0;

//debug(")))|%s| |%s|",t,s);

 return t2?0:det_var_err[1];
};


int OutVarTbl(Req *s,char **tv,char *xbbf)
{
 char *pbbf=xbbf;
 for(;*tv;tv+=2)
 { pbbf+=sprintf(pbbf,"%s=%s<br>",*tv,tv[1]?tv[1]:""); }
 return s->Send(xbbf,pbbf-xbbf);
}
void RegVars::PrintEnv(Req *s)
{ char *xbbf,*t;
  int i;
  if((xbbf=new char[0x3000]))
  {
    s->Send("<br><b>HTTP:</b><br>",sizeof("<br><b>HTTP:</b><br>")-1);
    OutVarTbl(s,vl1,xbbf);
    if(*vl2)
    {
     s->Send("<br><b>Request:</b><br>",sizeof("<br><b>Request:</b><br>")-1);
     OutVarTbl(s,vl2,xbbf);
    }
    if(nrv)
    {s->Send("<br><b>Expresion:</b>",sizeof("<br><b>Expresion:</b>")-1);
     for(i=0;i<nrv;++i)
     {
//      debug("^^^ %u %X %d",nrv,rv[i].v,rv[i].l);
//      send(s,xbbf,sprintf(xbbf,"<br>$%u=",i+1),0);
//      send(s,rv[i].v,rv[i].l,0);

      sprintf(xbbf,"<br>%u=$%u",i+1,i+1);
      SubstVar(xbbf+20,xbbf);
      s->Send(xbbf+20,strlen(xbbf+20));
     }
     SubstVar(xbbf,"<br>\\$`=$`<br>\\$'=$'<br>");
     s->Send(xbbf,strlen(xbbf));

/*
     send(s,"<br>$`=",sizeof("<br>$`=")-1,0);
     SubstVar(xbbf,last_str);
     if(last_before>0) send(s,xbbf,last_before,0);
//debug("%X %X %u",rv[nrv-1].v,last_str,i);
//debug("%s",xbbf);


     send(s,"<br>$'=",sizeof("<br>$'=")-1,0);
     if((i=strlen(xbbf)-last_before-rv[nrv-1].l)>0)
       send(s,xbbf+last_before+rv[nrv-1].l,i,0);

     send(s,"<br>",sizeof("<br>")-1,0);
*/
    }
   delete xbbf;
  }
};

