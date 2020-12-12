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



struct DirChk
{
 ulong DirChkSum,DirChkSum2,
// DirChkSumS,DirChkSum2S,
 DirChkSumX,DirChkSum2X,
 DirChkLngh;
};
DirChk dirchk;
void CalcDirChkSum(char *bb)
{
 WIN32_FIND_DATA fnds;
 HANDLE hdl;
 int i;
 char *p;

 p=bb+strlen(bb);
 strcpy(p,"\\*.*");
 if( ( hdl=FindFirstFile(bb,&fnds) )!=INVALID_HANDLE_VALUE )
 {
  do
  {
   if( WORD_PTR(fnds.cFileName[0])!=0x2E && WORD_PTR(fnds.cFileName[0])!=0x2E2E )
   {
    for(i=0;fnds.cFileName[i];++i)
    { dirchk.DirChkSum ^= ((1+i)<<5) * (fnds.cFileName[i]&~0x20);
      dirchk.DirChkSum2^=((ulong)(fnds.cFileName[i]&~0x20))<<(i%25);
      if(i<6)
      { dirchk.DirChkSumX ^=(fnds.cFileName[i]&~0x20)* ((i<<7)+1);
        dirchk.DirChkSum2X^=(fnds.cFileName[i]&~0x20)<<(i<<4);
      }
    }
/*
    for(i=0;fnds.cAlternateFileName[i];++i)
    { dirchk.DirChkSumS +=(fnds.cAlternateFileName[i]&~0x20)* (1+(i<<5));
      dirchk.DirChkSum2S^=(fnds.cAlternateFileName[i]&~0x20)<<(i%25);
    }
*/
    if( !(fnds.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
     dirchk.DirChkLngh+=fnds.nFileSizeLow;
    else
    { wsprintf(p,"\\%s",fnds.cFileName);
     CalcDirChkSum(bb);
     *p=0;
    }
   }
  }while(FindNextFile(hdl,&fnds));
  FindClose(hdl);
 }
 *p=0;

}