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


#include "pe_m.h"

struct Rres{
ulong  Flags;
ulong  TimeDateStamp;
ulong  Version;
ushort NameEntry;
ushort ID_NumEntry;
ulong  RVA[3];
} __attribute__ ((packed));


uchar *RCBase;
//#define RCBase  ((uchar *)(((PEAOUTHDR *)(0x400074+FILHSZ))->DataDirectory[2][0]+0x400000))
//end
long DeltaRes;
void UpdateRES(Rres *r)
{
 int i;
 i=(r->NameEntry+r->ID_NumEntry)*2 +1;
 r->TimeDateStamp=0;
 r->Version=0;
 while(i>2)
 {i-=2;
  if(r->RVA[i]&0x80000000)
  {UpdateRES((Rres *) (RCBase+(r->RVA[i]&0xFFFF) ) ); }
  else
  {//r->RVA[i]+=DeltaRes;
   DWORD_PTR(RCBase[r->RVA[i]])+=DeltaRes;
  }
 }
}


int BindRes(PEAOUTHDR *PE1,PEAOUTHDR *PE2,FILHDR *FH2,int l)
{
 int i1,i2,j;

 SCNHDR *SH1,*SH2;
 SH1=(SCNHDR *) &(PE1[1]);
 SH2=(SCNHDR *) &(PE2[1]);
 j=SH2->s_vaddr+ (SH2->s_paddr+0xFFF)&0xFFFFf000;
 memcpy(++SH2,SH1+1,sizeof(SCNHDR));
 SH2->s_vaddr=j;
 FH2->f_nscns++;
 DeltaRes=j-SH1[1].s_vaddr;
 UpdateRES((Rres *)RCBase);
 PE2->DataDirectory[2][0]=j;
 PE2->DataDirectory[2][1]=PE1->DataDirectory[2][1];
// if(SH2->s_size > SH2->s_paddr)SH2->s_size=(SH2->s_paddr+0x1FF)&0xFFFFE00;
 PE2->SizeOfImage+=(SH2->s_size+0xFFF)&0xFFFFf000;
 SH2->s_scnptr=l;
 SH2->s_flags=0x40000040;
 return SH2->s_size;
}

