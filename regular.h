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


#ifndef REGULAR_H
#define REGULAR_H


struct RegVar
{
 char *v;
 int l;
};

struct Req;
struct RegVars
{
 RegVar rv[11];
 int nrv;
 char *last_str;
 int  last_before;
 char **vl1;
 char **vl2;
 int req_counter;
 int st;
#define RV_FLG_NEW   0x2000
#define RV_FLG_SUB   0x1000
#define RV_FLG_ICASE 0x20

 char *aa[4];
 char *bb[4];
 char *end;
 int pos;
 char last_char;
 Req *req;


 int IsRegular(char *s0,char *p);
 char *FindRegular(char *s,char *pat);
 int LogAn(char *ls);
 int cmp(char *a,char *b);
 int isVar(char *&a,char **xx);
 void Init(char **v1,char **v2,Req *rr)
 {memset(this,0,sizeof(*this) ); /*st|=0x20;*/ vl1=v1; vl2=v2; req=rr; }
 char * SubstVar(char *t,char *s);
 char * FindEnd(char * &t);
 void PrintEnv(Req* s);
 char * isFunc(char *ls);
};

char *strchr_meta(char *p,int c);
int SlashSeq(char * &b);
int IsInGrp(char *b,int c);
int OutVarTbl(int s,char **tv,char *xbbf);
char *SkipSpace(char *a);

#endif
