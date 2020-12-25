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

//gcc -m32 -O3 -s bin2s.cpp -o bin2s.exe

#include <stdio.h>
#include "mdef.h"

extern "C" {
#pragma GCC diagnostic ignored "-Wformat"
//#pragma GCC diagnostic ignored "-Wcomment"
//#pragma GCC diagnostic ignored "-Wwrite-strings"
//#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
//#pragma GCC diagnostic ignored "-Wunused-result"
//#pragma GCC diagnostic ignored "-Wwrite-strings"
//#pragma GCC diagnostic ignored "-Wnarrowing"
//#pragma GCC diagnostic ignored "-Wliteral-suffix"

#define die(a,b...) { printf(a,b); return 1;};

#define  PAGE_SIZE 4096
#define  lPAGE_SIZE 1024

static  union{
char tbc[PAGE_SIZE+8];
uint tbl[lPAGE_SIZE*2];
};

int main(int argc,char *argv[] )
{
FILE  *fout;
FILE  *fin;
int  b,bb,i,n=0;
  if(argc < 3)die("Command line: %s srcfile dstfile\r\n",argv[0]);
  if(! (fin=fopen(argv[1],"rb") ) )die("Could not open source %s...",argv[1]);
  if(! (fout=fopen(argv[2],"w")) )die("Could not open target %s...",argv[2]);

//  fprintf(fout,"%s" s_head);
  
  do
  {
   b=fread(tbc,1,PAGE_SIZE,fin);   
   if(b<=0)break;
   bb=(b+3)>>2;
   DWORD_PTR(tbc[b])=0;
   for(i=0;i<bb;++i)
   {  fprintf(fout,"%s 0x%8.8X",(i&0x7)?",":"\n.long "  ,tbl[i]);
   }    
  }while(b==PAGE_SIZE);
  fprintf(fout,"\n");
  
  fflush(fout);
  
  fclose(fout);
  
  fclose(fin);
//  puts("Right...\n");
  return 0;
};

}

