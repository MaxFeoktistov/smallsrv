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

#ifdef WITHMD5
void MD5UpdateL(MD5_CTX *c, char *u)
{
  if(u)
  {
    MD5Update (c, (uchar *)u, strlen ( (char *) u));
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

int FillMD5Pwd(char *t, uint *dgt)
{
#if defined(AT_ARM) || defined(ARCH_REQ_ALIGN)
  uint adgt[6];
  if( ((long)dgt) & 3){
    memcpy(adgt, dgt, 16);
    dgt = adgt;
  }
#endif
  return sprintf(t,"~%8.8X.%8.8X.%8.8X.%8.8X",dgt[0],dgt[1],dgt[2],dgt[3]);
}

char* ConvPwdMD5(char *t,char *u,char *pas)
{
  uint dgt[6];
  ConvPwdMD5L4(dgt,u,pas);
  return t + FillMD5Pwd(t, dgt);
}

int IsPwdMD5C(char *p, char *pas,char *u)
{
  uint dgt[6];
  int i;
  ConvPwdMD5L4(dgt,u,pas);
  p++;
  for(i=0;i<4;i++)
    if(DWORD_PTR(p[i<<2]) != dgt[i] ) return 0;
  return 1;
}

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


void CopyXor64(uchar *t, const char *s, uint pad)
{
  uchar *e=t+64;
  do{
    *t++ = *s ^ pad;
    if(*s) s++;
  }while(t<e);
}

void GenCRAM_dgst(char *pas,char *dgst,char *s,int ssize)
{
  MD5_CTX context;
  uchar tmp[68];
  uint dgt[6];

  CopyXor64(tmp, pas, 0x36);
  MD5Init (&context);
  MD5Update (&context, tmp, 64);
  MD5Update (&context, (uchar *)s, ssize);
  MD5Final ((uchar *)dgt, &context);
//  CvtHex(dgt,dgst);

  CopyXor64(tmp, pas, 0x5c);
  MD5Init (&context);
  MD5Update (&context, tmp, 64);
//  MD5Update (&context, (uchar *)dgst, 32);
  MD5Update (&context, (uchar *)dgt, 16);
  MD5Final ((uchar *)dgt, &context);
  CvtHex(dgt,dgst);
}

int IsPwdCRAM(char *pas,char *dgst,char *s,int ssize)
{
  char dgHex[40];
  GenCRAM_dgst(pas,dgHex,s,ssize);
  DBGLA("DEBUG  CRAM %s %s %s %s %u\r\n",pas,s,dgHex,dgst,ssize);
  return ! strcmp(dgHex,dgst);
}

void CalkPwdMD5D(char **dgv, uint *HA1,char *method, char *HA2Hex)
{
  uint HA2[6]      ;
  char HA1Hex[40]      ;
#define rt2 HA2
#define rez HA2Hex
  MD5_CTX context;
  CvtHex(HA1, HA1Hex);
// calk HA2
  DBGLA("method: %s digtVar_uri %s digtVar_nonce:%s", method, dgv[digtVar_uri], dgv[digtVar_nonce])

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
      DBGLA("qop: %s nc: %s cnonce:%s",dgv[digtVar_qop],dgv[digtVar_nc], dgv[digtVar_cnonce] )

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

  DBGLA("MD5 cmp:%s<>%s|  %s",rez,dgv[digtVar_response],HA2Hex);
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
