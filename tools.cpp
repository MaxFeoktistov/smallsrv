/*
 * Copyright (C) 1999-2023 Maksim Feoktistov.
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


#ifndef SELECT1
#ifdef MINGW
#undef fd_set
#endif
int RESelect(long tv_sec,long tv_usec,int n,...)
{return select(0,(fd_set *)&n,0,0,(timeval*)&tv_sec);};
int WESelect(long tv_sec,long tv_usec,int n,...)
{return select(0,0,(fd_set *)&n,0,(timeval*)&tv_sec);};

#else


int RESelect1(long tv_sec,long tv_usec,int s)
{
#ifdef MINGW
#undef fd_set
#define win_fd_set fd_set
#endif
 fd_set t;

 t.fd_count=1;
 t.fd_array[0]=s;
 return select(0,(fd_set *)&t,0,0,(timeval*)&tv_sec);
};
int RESelect2(long tv_sec,long tv_usec,int s1,int s2)
{
 fd_set t;
 int r;

 t.fd_count=2;
 t.fd_array[0]=s1;
 t.fd_array[0]=s2;
 if( ! (r=select(0,(fd_set *)&t,0,0,(timeval*)&tv_sec)) )return 0;
 if(r>1)return s1;
 return FD_ISSET(s1,&t)? s1:s2;
};
//#define RESelect(a,b,c,d) RESelect1(a,b,d)
#endif

#ifdef MINGW
#undef fd_set
#define fd_set win_fd_set

void win_fd_clr(int s, fd_set *set)
{
  for(int i=0; i<set->fd_count; i++)
  {
    if(set->fd_array[i] == s)
    {
      set->fd_count --;
      if(set->fd_count != i) set->fd_array[i] = set->fd_array[set->fd_count];
    }
  }
}

#endif


#endif


void maxFdSet::Set(int fd)
{
  FD_SET(fd, &set);
  if(max_fd < fd) max_fd = fd;
}

void maxFdSet::Fix()
{
  while(max_fd>0)
  {
    max_fd --;
    if(FD_ISSET(max_fd, &set) ) break;
  }
}

void maxFdSet::Clear(int fd)
{
  FD_CLR(fd, &set);
  if(max_fd == fd) Fix();
}

void CloseSocket(int s)
{
  #undef shutdown
  shutdown(s,2);
  closesocket(s);
}

