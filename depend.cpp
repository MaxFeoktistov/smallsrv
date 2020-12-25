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


//#include "reg_usrs.cpp"

#ifndef FREEVER

#include "rulst.cpp"

#ifdef SYSUNIX

#define  _chk_code3  chk_code3
#define  _chk_code4  chk_code4
#define  _chk_code5  chk_code5
#define  _chk_code6  chk_code6

asm ("\n"
" _chk_code3=chk_code3\n"
" _chk_code4=chk_code4\n"
" _chk_code5=chk_code5\n"
" _chk_code6=chk_code6\n"
"");
#endif

void Crpt(char *s,char *t);
void Uncrpt(char *ss,char *t);
void  UpdAbout();

char *newabout;
//uchar reg_usr_hi[32];
//uchar reg_usr_lo[];
//uchar reg_usr_lo2[];
extern "C" ulong CheckShare(uchar *t)
{
 int f;
 f=0;
 ulong x=0x1230512;
 while(*t)
 {
  if( *t==' ' )if(f) goto mlx1; else f=1; else f=0;
  switch(*t&0xF)
  {
   case 0: x+=*t;break;
   case 1: x*=*t;break;
#ifdef  NOTINTEL

#else
    case 6:
           asm(" rolw %%cl,%%ax; ":"=a"(x)
                :"a"(x),"c"(*t)   );break;
   case 3:
           asm(" rorw %%cl,%%ax; ":"=a"(x)
                :"a"(x),"c"(*t)   );break;
   case 2:
           asm(" rorb %%cl,%%al; ":"=a"(x)
                :"a"(x),"c"(*t)   );break;
   case 5:
           asm(" rolb %%cl,%%ah; ":"=a"(x)
                :"a"(x),"c"(*t)   );break;
   case 4:
           asm(" rorl %%cl,%%eax; ":"=a"(x)
                :"a"(x),"c"(*t)   );break;
   case 7:
           asm(" roll %%cl,%%eax; ":"=a"(x)
                :"a"(x),"c"(*t)   );break;
#endif
           
   case 8: x^=*t;break;
   case 9: BYTE_PTR(x,1)^=*t;break;
   case 0xF: BYTE_PTR(x,2)^=*t;break;
   case 0xC: BYTE_PTR(x,2)-=*t;break;
   case 0xB: BYTE_PTR(x,2)+=*t;break;
   case 0xD: BYTE_PTR(x,1)+=*t;break;
   case 0xE: BYTE_PTR(x,1)-=*t;break;
   case 0xA: x^=WORD_PTR(*t);break;
  };
 mlx1:
  t++;
 }
 x&=0xFFFFFF;
 if(x<0x800000)x^=0xA5A5A5;
 return x;
}


uchar vcar[]={2,3,5,7 //,11
};
ulong  isMCName(ulong v)
{
  uint i;
  /*
  for(i=1000; i<v ;i*=10) ;
  i/=1000;
  v%=i;
  */
  v&=0x3FFFF;
  for(i=0;i<sizeof(vcar); i++ )
  {  while(v>300 &&  ! (v%vcar[i]) )v/=vcar[i];
  }
  if(!v) return 0;

  for(i=0; i<(sizeof(RusrLst)/4) ;++i )
  {
    if(! (RusrLst[i]%v) )return 1;
  }
  return 0;
}



int   CheckList(uchar *bfr)
{
 ulong n1,n2,n3;
 uchar *p;
#if 0
 n1=MkName((char *)bfr);
 n2=CheckShare((uchar *)bfr+1)+(n1>>16);
// n3=CheckShare(bfr);

 n1>>=8;
 for(n3=0;n1&0x1F;){n3+=reg_usr_hi[--n1&0x1F];}

 if( (reg_usr_hi[n3]) &&  (p=(uchar *)memchr(reg_usr_lo+n3,n2,reg_usr_hi[n3])) )
 { n1=p-reg_usr_lo;
   n3=(reg_usr_lo2[n1>>1]>>((n1&1)<<2));
   if( !(((n2>>8)^n3)&0xF) ) return 1;
 }
#else
 if(isMCName(CheckShare((uchar *)bfr)))return 1;
#endif

#ifndef SYSUNIX
//  DeleteFile("ucfshttp.zip");
//  DeleteFile("keygen.exe");
//  DeleteFile("ucf2000.nfo");
  DeleteFile("tsrh.nfo");
//  DeleteFile("10.03 Small HTTP Server v3.04.rar");
//  DeleteFile("keygen.exe");
//  DeleteFile("e-Lunatic_diz");
  MessageBox(0, sYOU_USED_
        ,"Sorry...",MB_OK|MB_ICONSTOP);
#else

#endif
  ExitProcess(0);

};

uint ChkFNC(uchar *f)
{
  uint r;
  r=0;
  do{
      if(0xCC==*f)++CCcnt;
      if(0xCD==*f)++CDcnt;
      r+=(*f*(CCcnt|1))^CDcnt;
      ++f;
      if( (WORD_PTR(*f)==0xC3C9) || (WORD_PTR(*f)==0xC2C9) )
      {
        C3C9cnt++;
        break;
      }
      if( (WORD_PTR(f[2])==0xE589) || (f>= (uchar *)hsdr.h ) )
      {
        E589cnt++;
        break;
      }
  }while(1);
  return r;
};
uint CheckShareTwo(uint r,uint x)
{
#ifdef  NOTINTEL

#else

 asm volatile("movzwl %%ax,%%eax\n"
"               orb  $1,%%cl\n"
"               rolw %%cl,%%ax\n"
"              ":"=&a" (r),"=&c"(x)
               :"0"(r),"1"(x)
//             :"%eax","%ecx"
              );
#endif
 return r&x;
};
int CheckReg(uchar * bfr,uint i,uint j,char *pp);

inline
ulong mulmod(ulong a,ulong b,ulong d)
{
   asm volatile("\n"
"     mull %%edx\n"
"\n"
"  3:\n"
"     cmpl %%ecx,%%edx\n"
"     ja  1f\n"
"     jecxz  4f\n"
"     pushl %%ecx\n"
"     shll  $16,%%ecx\n"
"     subl %%ecx,%%eax\n"
"     movzwl 2(%%esp),%%ecx\n"
"     sbbl %%ecx,%%edx\n"
"     popl %%ecx\n"
"     jmp  3b\n"
"  1:\n"
"     divl %%ecx\n"
"     movl %%edx,%%eax\n"
"  4:\n"
"   ":"=&a" (a),"=&d" (b) ,"=&c" (d)
    :"0" (a), "1"(b), "2"(d)

   );
 return a;
}
inline ulong DIVMOD(ulong a,ulong d,ulong &m)
{ asm volatile("\n"
"     xorl %%edx,%%edx\n"
"     divl %2\n"
"   ":"=&a" (a), "=&d" (m)
    :"0" (a),  "m"(d)
   );
 return a;
}

inline
ulong powmod(ulong a,ulong b,ulong d)
{
 ulong r[2];
 r[0]=1;
 r[1]=a;
 while(b)
 {
  if(b&1){r[0]=mulmod(r[0],r[1],d); }
  r[1]=mulmod(r[1],r[1],d);
  b>>=1;
 }
 return r[0];
}



int CheckCode(uchar *bfr,uint i,uint j)
{
 char ttt[80];

 uint chk,oCC,oCD,a,b,r,ii;
 chk=0;
 char cname[32];
 oCC=CCcnt;
 oCD=CDcnt;

 CCcnt=0;
 CDcnt=0;

 chk^=ChkFNC((uchar*)(void *) CheckCode );
 chk^=ChkFNC((uchar*)(void *) CheckShareTwo );
 chk^=ChkFNC((uchar*)(void *) CheckReg );
#ifndef SYSUNIX
 chk^=ChkFNC((uchar*)(void *) DefProc );
 chk^=ChkFNC((uchar*)(void *) WinMain );
#else
// chk^=ChkFNC((uchar*)(void *) main );
#endif
 chk^=ChkFNC((uchar*)(void *) InitApplication );
 chk^=ChkFNC((uchar*)(void *) CheckShare );
 if((j*i)==chk_code4 )
 {chk_code4=chk;
#ifndef SYSUNIX
  if( (!GetVolumeInformation("C:\\",cname,24,(ulong *)&chk_code5,(ulong *)cname,(ulong *)cname,0,0))  || !chk_code5)chk_code5=12345678;
#else
  chk_code5=12345678;
#endif
//  chk_code5=powmod(chk_code5,chk_code4,0xC0000001);
//MessageBox(0,"j*i==","Ok",MB_OK);

 }
 if(i && bfr)
 {
//  sprintf(ttt,"%.50s %X",bfr, );




 /*
  asm volatile("\n"
"       roll %%cl,%%eax\n"
"       xorl %%edx,%%eax\n"
"       shrl $5,%%ecx\n"
"       rorl %%cl,%%eax\n"
"       shrl $5,%%ecx\n"
"       addl %%ecx,%%eax\n"
"       xorl $0x15FC2487,%%eax\n"
"       jnz  1f\n"
"       xorl $0x15FC2487,%%eax\n"
"       inc %%eax\n"
"       roll $1,%%eax\n"
"       inc %%eax\n"
" 1:\n"
"     ":"=&a" (chk_code6),"=&c"(i),"=&d"(i)
      :"0"(i),"1"(chk_code5),"2"(chk_code4)
//      :"%eax","%ecx"

      );

// sprintf(ttt,"%X %X| %X %X %X ",chk_code6,CheckShare(bfr),chk_code5,chk_code4,i);
// MessageBox(0,"i & bfr",ttt,MB_OK);

  if(chk_code6==CheckShare(bfr)  )
*/

//debug("|%s| %X %X %X %X",bfr,chk_code3a,chk_code5,chk_code3b,CheckShare(bfr));
  if(IsPwd(chk_code3a^chk_code5,chk_code3b^CheckShare(bfr),(char *) bfr))
  {
   ii=sprintf(user_name=about+sizeof(ABOUT_STR)-1, sREGISTRED ,(char *)bfr);
   user_name+=15;
   SRegHTM();
#ifndef SYSUNIX
   nid.szTip[12]=0;
   mnu2[ (sizeof(mnu2)/sizeof(mrc_mnu))-4].id=0;
   if(hmnu){
     DeleteMenu(hmnu,353,MF_BYCOMMAND);
     DeleteMenu(hmnu,134,MF_BYCOMMAND);
     DeleteMenu(hmnu,135,MF_BYCOMMAND);
     SetWindowText(mwnd,(char *)nid.szTip);
   }
#endif
   newabout=about+(sizeof(ABOUT_STR)+96)/2;

   if( (user_name+ii)  >=  newabout  ) newabout = new char [(user_name+ii) -about + 160 ];
   Crpt(about,newabout);


  }
  else
  { user_name=0;
  //debug("NO chk pwd!!!!!");
  }
 }

 return (CCcnt^oCC)+(CDcnt^oCD)+(chk^chk_code4);
};
int CheckReg(uchar * bfr,uint i,uint j,char *pp)
{
 uint k;
  k=CheckShare((uchar *)bfr);
  //debug("%u %s",k,bfr);
  if(i==k)
  {
 // debug("CheckReg(%s,%u,%u,%s) k=%u %u %u, %c%c%c",bfr,i,j,pp,k,chk_code3,
 //      CheckShareTwo(j,chk_code3),(j%26)+'a',((j^*pp)%26ul)+'a',((j+*pp)%26ul)+'a');
   if((( (CheckShareTwo(j,chk_code3) == chk_code3)
          && pp
          && (*pp==(j%26)+'a')
#ifndef SYSUNIX
          && pp[1]==(((j^*pp)%26ul)+'a')
          && pp[2]==(((j+*pp)%26ul)+'a')
#endif
       ) || CheckList(bfr)
      )
      && i!=8545217 && i!=12057788  && i!= 8923899
     )
   {
   //  debug("Reg Ok");
#ifdef  NOTINTEL

#else

  asm volatile("\n"
"       movl %%eax,%%edx\n"
"       roll %%cl,%%eax\n"
"       xorl _chk_code4,%%eax\n"
"       shrl $5,%%ecx\n"
"       rorl %%cl,%%eax\n"
"       shrl $5,%%ecx\n"
"       addl %%ecx,%%eax\n"
"       xorl _chk_code6,%%eax\n"
"       xorl %%edx,%%eax\n"
"       subl %%ecx,%%eax\n"
"       movl _chk_code5,%%ecx\n"
"       shrl $5,%%ecx\n"
"       roll %%cl,%%eax\n"
"       movl _chk_code5,%%ecx\n"
"       xorl _chk_code4,%%eax\n"
"       rorl %%cl,%%eax\n"
"      "
       :"=&a"(chk_code3),"=&c"(k)
       :"0"(i),"1"(chk_code5)
//       :"%eax","%ecx"
);
  
#endif
  
     return CheckCode(bfr,chk_code3,0);
   }
  }
 return 1;
}

#if 0
extern "C" {
void WINAPI DbgMemError(char *a,int b)
{
 char bb[128];
 sprintf(bb,"MemError %X[%X-8]=%X",a,b,DWORD_PTR(a[b-8]) );
 MessageBox(0,bb,"Error",MB_OK);
}
}

#endif

void Crpt(char *s,char *t)
{ int b=0x55;
  while(*s)
  { *t=*s^b;  b+=*s;  ++t; ++s;  }
  DWORD_PTR(*t)=0;
}

void Uncrpt(char *s,char *t)
{ int b=0x55,c;
//  char *s=ss;
  while(DWORD_PTR(*s))
  { *t=*s^b;  b+=*t; ++t; ++s; }
  DWORD_PTR(*t)=0;
//  return s-ss;
}

void  UpdAbout()
{
  if(newabout)
    Uncrpt(newabout,about);
  else DWORD_PTR(about[sizeof(ABOUT_STR)-1])=0;

}

/*

int IsInRegList(ulong v)
{
 ulong i,j;
  j=v;
  for(i=0; i<6; ++i)
  {
    if(!() )
    {

    }


  }

}

 */

#endif

