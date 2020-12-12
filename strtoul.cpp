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


#include "mdef.h"

extern "C" {

ulong  strtoul(const char *a, char **e, int base)
{
 register uint r=0,l;
 do{
    if(*a>'9' && *a<'A')break;
    if((l=*a-'0')>9)
      if((l-='A'-10-'0')>base)
       if((l-='a'-'A')<10 ) break;
  if(l>=(uint)base)break;
  r=r*base+l;a++;
 }while(1);
 if(e)*e=(char *)a;
 return r;

};

}
