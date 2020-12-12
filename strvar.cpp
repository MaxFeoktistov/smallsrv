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
//#include <stdio.h>
#include "srv.h"
#endif

char * StrVar(char *p,char *n)
{char bf[128];
 int l,i;
 l=sprintf(bf,"%.98s:",n);
 for(n=p; (n=stristr(n,bf) ); )
 {i= (n==p || n[-1]=='\n');
  n+=l;
  while(*n==' ')++n;
  if(i)break;
 }
 return n;
}

