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

//gcc -fpack-struct -fconserve-space -O2 -s getstr.cpp -o getstr.exe
//g++ -m32 -pipe -fpack-struct -fconserve-space -O2 -s getstr.cpp -o getstr

#include<stdio.h>
//#include "mstring1.h"
#include <string.h>
#include "mdef.h"


#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wunused-result"


#define debug printf

struct DfEntry
{
 DfEntry *next;
 char *name;
 char txt[1];
 void Set(char *t, char *n);
 void Write(FILE *fi);
} PACKED ;

void DfEntry::Set(char *t, char *n)
{
 char *p=txt;
 t++;
 while((*p=*t))
 {++t;
  if(*p=='\"' && p[-1]!='\\')
  {
   while(*t != '\"' )
   {
    if(!*t)goto exLp;
    ++t;
   }
   ++t;
  }
  else ++p;
 }
exLp:;
 *p++=0;
 next=(DfEntry *)(p+sprintf(name=p,n)+1);
};

void DfEntry::Write(FILE *fi)
{
 char *t,*p;
 int j;
 fprintf(fi,"#define %s ",name);
 t=txt;
 while((p=strstr(t,"\\n")))
 {
   j=p[2];
   if(!j)break;
   p[2]=0;
   fprintf(fi,"\"%s\"\\\n",t);
   p[2]=j;
   t=p+2;
 }
 j=50;
 while(strlen(t)>j)
 {
  if(!(p=strchr(t+j-10,' ')))break;
  *p=0;
  fprintf(fi,"\"%s\"\\\n",t);
  *p=' ';
  t=p;
  j=72;
 }
 fprintf(fi,"\"%s\"\n",t);
};


DfEntry df[0x20000];
char *fname="xstrc.h";
char *pref="";
char *bbuf;

int c_cnt,yes,noinc,rus;

inline int strin(char *a, char *b)
{
 while(*b && *a==*b){a++; b++;  }
 return !*b ;
}


ulong hash_back(char *t)
{
 ulong r=0,i;
 
 for(i=2; i<8 ; i++) if(strin(t-i,"CS("))
 {
  // printf("CS found %u\n",i);  
   t-=i+1;
 }
 
 i=1;
 while(t>bbuf && *t!='\n')
 {
   if(*t!=' ' && *t != '\t' && *(uchar *)t<128 )
   {
     r+=(*t-'!')*i;  
     i+=95;
     if(i>(0x7FFFffff/96))i=(i+1)%95;
   }
   --t;
 }
 
 return r;
    
    
};
    
int main(int argc, char *argv[])
{
 int i,k,b,l,c,v,ll,oldsect,ii;
 ulong  j=0;
 FILE *fi,*fo;
 char bname[256];
 char *t,*tt,*p,*t1,*t2,*nofix=0;
 DfEntry *pdf=df, *tdf;

   if( argc<=2 )
   { printf("Command line:\n%s [-f out_filename.h] [-y] [-r] [-n] prefix {src}",argv[0]);
     return -1;
   }
 i=1;
 while(argc >3 && argv[i][0]=='-' ) 
 {
   if(argv[i][1]=='f')  
   {    
    i++;   
    fname=argv[i];
     //i+=2;
   }
   else    if(argv[i][1]=='y')  
   {
     yes=1;       
   }
   else    if(argv[i][1]=='n')  
   {
     noinc=1;       
   }
   else    if(argv[i][1]=='r')  
   {
     rus=1;       
   }
   i++;   
 }
 pref=argv[i];

// sprintf(bname,"%sstrc.h",argv[i]);
 sprintf(bname,"%s",fname);
 if( ! (fo = fopen(fname,"ab")))
 { printf("Unable to open %s\n",fname);
   //if( ! (fo = fopen("strc.h","ab")))
       return -1;
 }
 fprintf(fo,"\n#define STRING_CONST_H_%s\n\n",pref);

 v=i+1;
 do{
   
  if( ! (fi = fopen(argv[v],"r")))
  { printf("Unable to open %s\n",argv[v]);
    continue;
  }

  fseek(fi,0,2);
  k=ftell(fi);
  fseek(fi,0,0);

  t1=t=bbuf=new char[k*2+32];

  ll=fread(bbuf,1,k,fi);
  bbuf[k]=0;
  fclose(fi);

  sprintf(bname,"%s%s",pref,argv[v]);
  if( ! (fi = fopen(bname,"wb")))
  { printf("Unable to open %s\n",bname);
  }
  else
  {
   printf("\n\n File %s %u\n\n",bname,rus);
   
   fprintf(fi,"\n//  GENERATED FILE, DON'T  EDIT THIS!  !!!  Edit %s instead...\n\n",bname);
   if(!noinc)
     fprintf(fi,"\n#ifndef STRING_CONST_H_%s\n#include \"%s\"\n#endif\n\n",pref,fname);


   while(*t)
   {
    if( WORD_PTR(*t)==0x2F2F )
    {
      nofix=strstr(t,"Dont_fix_var");  
      if(!(t=strchr(t,'\n')))break;
      ++t;
      if(nofix > t )nofix=0;
      continue;
    }
    if( WORD_PTR(*t)==0x2A2F )
    {++t;
     do{
      if(!*t) goto exLp1;
      ++t;
     }while(WORD_PTR(*t)!=0x2F2A );
     ++t;
     continue;
    }

    if(*t!='\"' || t[-1]=='\\' || t[-1]=='\'' ){++t; continue;}
    p=t+1;
    while((p=strchr(p,'\"')))
    {
      //if(p[-1]=='\\' && (p[-2]!='\\' || p[-3]=='\\' ) )
      ii=1;
      while( p[-ii]=='\\' )++ii;
      if(!(ii&1) ){++p; continue;}
      t2=p+1;
   //   debug("found end ..%.8s ..\n",p);
/*
 "Only test "
 "constant"
 " 123"
 " end big line to test "
 "how it will be work in the real substitute. Any other world... "
 "Ok. it's a"
"ll";

 "Only test "
 "constant"
 " 123"
 " end big line to test "
 "but it is other";

 "Only test "
 "constant"
 " 123"
 " end big line to test "
 "how it will be work in the real substitute. Any other world... "
 "Ok. it's all";
*/
       while(strchr(" \t\r\n\\LF",*t2) ) ++t2;

//       do{   
//        while(*t2 == ' ' || *t2 == '\r' || *t2 == '\n' || *t2 == '\t' || *t2 == '\\' ) ++t2;
//        if(WORD_PTR(*t2)!= (((uint)'F') | (((uint)'L')<<8)  )   //C4CHAR("LF") 
//          ) break;
//        t2+=2;
//       }while(1); 
      if(*t2!='\"')break;
      p=t2+1;
   //   debug("continue  ..%.20s ..\n",t2);
    }
    if(!p)break;
    *p=0;
    i=1;
    j=0;
    bname[0]='s';
    for(j=0;t[i] && i<24; ++i)
    {
     c=((uchar *)t)[i];
     if( c>127 && !rus )goto lbCnt;
     if(nofix)
     {
        nofix=0;   
        goto lbCnt;
     }   
     if( (c>='a' && c<='z') || (c>='A' && c<='Z') || (rus && c>=128 ) )
     {
      if(c>=128 )j|=5;//j|=0x8;   
      c&=~0x20;
      ii=((uchar *)t)[i-1]|0x20;
      //if(t[i-1]>='A' && t[i-1]<='z' )
      if( (ii>='a' && ii<='z' ) || (rus && ii>=128) )
      {
          j|=1;
          if(j&2)j|=4;
      }
     }
     else {if(c==' ')j|=2; c='_';  }
     if(i<10)bname[i]=c;
    }
    if(!rus)while(t[i]){ if( ((uchar *)t)[i]>127)goto lbCnt; ++i; }
    if(j==7)
    { if(i>10)i=10;
      j=0;
//      i=sprintf(bname,"%sT_%u",pref,++c_cnt);
      ++c_cnt;
      i=sprintf(bname,"%sT_%u",pref,hash_back(t-1));
      DWORD_PTR(bname[i])=0;
      for(tdf=df;tdf!=pdf;tdf=tdf->next)
      { if(!strcmp(tdf->name,bname) )
        {if(!j){bname[i++]='0'; ++j;}
         else if(bname[i-1]++ == ':')
         {bname[i]='0'; bname[i++]='0';}
        }
      }

      pdf->Set(t,bname);

      for(tdf=df;tdf!=pdf;tdf=tdf->next)
        if(!strcmp(tdf->txt,pdf->txt) )
        {
     lbStr:
         *t=0;
         fprintf(fi,"%s %s ",t1,tdf->name);
         t1=p+1;
         goto lbCnt;
        }
     if(yes) c='y';
     else     
     {
      printf("\n%s\n%s>[y],n,q,x?",pdf->name,pdf->txt);
      fflush(stdin);
      if((c=getchar()|0x20)=='q')break;
      if(c=='x'){v=0x7FFFFF; break;};
     } 
      if(c!='n')
      {
       tdf=pdf;
       tdf->Write(fo);
       pdf=pdf->next;
       goto lbStr;
      }
      
      c&=~0x20;
      while(c!='\n' && c!='\r' )c=getchar();
    }
lbCnt:;
    t=tt=p+1;
    *p='\"';

   }
 exLp1:
   fprintf(fi,"%s",t1);
   fclose(fi);
  }
  delete bbuf;
 }while(++v<argc);

 fclose(fo);

};
