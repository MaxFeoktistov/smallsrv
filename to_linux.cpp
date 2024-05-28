/*
 * Copyright (C) 1999-2022 Maksim Feoktistov.
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
#include "to_linux.h"
#endif
//#include "hton.h"

#include<pwd.h>
#include<grp.h>


extern "C" {
void MyLock(int &x);
#ifdef USE_FUTEX
void MyUnlock(int &x)
{
 x=0;
 futex((int *)&x,FUTEX_WAKE,1,0,0,0);
}
#else
inline void MyUnlock(int &x){x=0;}
#endif
/*
void* zalloc(uint a)
{
  char *r=(char *)  malloc(a);
  if(r && a)memset(r,0,a);
  return r;
}*/


} //extern C

int MemMtx;
char * Malloc(int c)
{
 register char *r;
 MyLock(MemMtx);
  if((r=(char *)malloc(c))){memset(r,0,c); }
 MyUnlock(MemMtx);
  return r;
};


void Sleep(uint ms_time)
{ulong t;
/*
 struct timeval t;
   t.tv_sec=ms_time/1000;
   t.tv_usec=(ms_time%1000)*1000+500;
   select(0,0,0,0,&t);
*/
 if(ms_time>=1000)sleep(ms_time/1000);
 if((t=(ms_time%1000)))usleep(t*1000+512);
};

/*
pthread_t MyCreateThread(int StackSize, void * ( *ThreadFunction)(void *), void *par )
{
  pthread_t result;
  pthread_attr_t attr;
  pthread_attr_init (&attr);
  pthread_attr_setstacksize (&attr,StackSize);
  pthread_create(&result,0,ThreadFunction,par);
  pthread_attr_destroy(&attr);
  return result;
};
*/

int RESelect1(long tv_sec,long tv_usec,int s)
{


#ifdef USE_POOL
   struct pollfd  pfd;
   pfd.fd=s;
   pfd.events=POLLIN
//#ifdef POLLRDHUP
    |POLLRDHUP|POLLHUP
//#endif
   ;
   pfd.revents=0;

   if( (! poll(&pfd,1,tv_sec*1000+((tv_usec+1023)>>10) )) ||
        ! (   pfd.revents&POLLIN )
     ) return 0;
   return 1;
#else
 fd_set set;
 if(s&0xFFFF0000)return -1;
 FD_ZERO(&set);
 FD_SET(s, &set);
 return select(s+1,&set,0,0,(timeval*)&tv_sec);
#endif
}
#ifdef USE_POOL
int REPool(ulong tv_msec,int s,int pflg)
{
   struct pollfd  pfd;
   pfd.fd=s;
   pfd.events=pflg;
   pfd.revents=0;

   if( (! poll(&pfd,1,tv_msec) ) || ! ( pfd.revents & pflg )
     ) return 0;
   return pfd.revents;
}
#endif
int RESelect2(long tv_sec,long tv_usec,int s,int s2)
{

#ifdef USE_POOL
   struct pollfd  pfd[2];
#ifndef POLLRDHUP
#define POLLRDHUP POLLHUP
#endif

   pfd[0].fd=s;
   pfd[0].events=POLLIN | POLLHUP| POLLRDHUP ;

   pfd[1].fd=s2;
   pfd[1].events=POLLIN | POLLHUP | POLLRDHUP;
   return  (poll(pfd,2,tv_sec*1000+((tv_usec+1023)>>10) ) < 0)? 0 :
     (pfd[0].revents&(POLLIN|POLLHUP|POLLRDHUP))? s  :
     (pfd[1].revents&(POLLIN|POLLHUP|POLLRDHUP))? s2 : 0
   ;
#else

 fd_set set;
 if((s|s2)&0xFFFF0000)return -1;
 FD_ZERO(&set);
 FD_SET(s, &set);
 FD_SET(s2, &set);
// if(s>s2)xchg(s,s2);

 return (select(((s>s2)?s:s2)+1,&set,0,0,(timeval*)&tv_sec)>0)? FD_ISSET(s,&set)?s:s2:0;
#endif
}


int FindNextFile(HANDLE h,WIN32_FIND_DATA *fd)
{
 char bfr[512],*t,*tt,*pp,*z,x;

cnt:
 z=0;
 if( (!h) || !(fd->pd=readdir((DIR *)h)))return 0;
 if(!(t=strrchr(fd->dir,'/')))t=&x;
 else
 {
  tt=t+1;
  pp=fd->pd->d_name;
//  debug("%s |%s| %s\n",fd->dir,tt,pp);
  do{
   if(*tt && (!*pp)  )
   { if(WORD_PTR(*tt)!='*')goto cnt;
     break;
   }
   if(*tt=='*')
   {
     z=++tt;
 lb_wld:
     while(*++pp!=*tt)if(!*pp)goto cnt;
   }
   else if(*tt=='?')
   {
     ++tt;
     ++pp;
     if(!*tt)
      if(!*pp)break;
      else goto cnt;
   }
   else if(*pp++!=*tt++)
        {
          if(z && *pp){tt=z; goto lb_wld;}
          goto cnt;
        }

  }while(*tt);

//  debug("Ok %s\n",fd->pd->d_name);

 }
 *t=0;
 sprintf(bfr,"%.255s/%.255s",fd->dir,fd->pd->d_name);
 *t='/';
 if(stat(bfr,&(fd->st))<0) lstat(bfr,&(fd->st));
 return 1;
};

HANDLE FindFirstFile(char *p,WIN32_FIND_DATA *fd)
{

// int h;
 DIR * h;
 char *t;
 if(strchr(p,'*') )
 {
  if((t=strrchr(p,'/')))
  {
   *t=0;
   h= opendir(p);
   *t='/';
 //  fd->s=t;
   if(!h)return (HANDLE)-1;
   fd->pd=0;
   strncpy(fd->dir,p,sizeof(fd->dir)-1);
   if(!FindNextFile((HANDLE)h,fd))
   { closedir((DIR *)h);
     return (HANDLE)-1;
   }
   return (HANDLE)h;
  }
  return (HANDLE)-1;
 }
 else if(stat(p,&(fd->st))!=-1)
 {
  //fd->pd=&(fd->d);
  //strncpy(fd->d.d_name,(t=strrchr(p,'/'))?t+1:p,sizeof(fd->d.d_name)-1);
  fd->pd=(dirent *)(((t=strrchr(p,'/'))?t+1:p) - offset(dirent,d_name));
  return 0;
 }
 return (HANDLE)-1;
};

int FileSize(int i)
{
 struct stat st;
 fstat(i,&st);
 return st.st_size;
}

int GetFileAttributes(char *i)
{
 struct stat st;
 stat(i,&st);
 return st.st_mode;
}

//void GetFileTime(HANDLE h,FILETIME *a,FILETIME *b,FILETIME *c)
void GetFileTime(int h,FILETIME *a,FILETIME *b,FILETIME *c)
{
 struct stat st;
 fstat(h,&st);
 if(a)*a=st.st_mtime;
 if(b)*b=st.st_atime;
 if(c)*c=st.st_ctime;
}
ulong GetTickCount()
{
 struct  timeval t;
 gettimeofday(&t,0);
/*
 asm volatile(
   "shrd $10,%%edx,%%eax"
   : "=&a" (t.tv_usec), "=&d" (t.tv_sec)
   : "0" (t.tv_usec), "1" (t.tv_sec)
   :"%ecx"
 );
 return t.tv_usec;
*/
 return  (t.tv_usec>>10ul)|(t.tv_sec<<10);
};


int IsAssess(char *fname,int uid,int gid,int type,char *uname)
{
 struct stat stt;
 struct group *g;
 struct passwd *pwd;
 int i;
 char *p;

 if( stat(fname,&stt)>=0 || lstat(fname,&stt)>=0 )
 {
lbChk:
  if( (stt.st_mode&type) // mode for all
    || uid==stt.st_uid) return 1;
  if( stt.st_mode&(type<<3) ) //// mode for group
  {
   if(gid==stt.st_gid) return 1;
   // Check group
   if( (uname || (pwd=getpwuid(uid)))     &&
       (g=getgrgid(stt.st_gid))
     )
   {
    if(!uname)uname= pwd->pw_name;
    for(i=0;g->gr_mem[i];++i) if(!strcmp(g->gr_mem[i],uname) ) return 1;
   }
  }
  return 0;
 }
 else if(!(type&2) )return -1;
 if((p=strrchr(fname,'/')))
 {
  *p=0;
  if( (i=stat(fname,&stt) )<0 ) i=lstat(fname,&stt);
  *p='/';
  type=2; // Now check for write only
  if(i>=0)goto lbChk;
 }
 return -1;




}


#ifdef CD_VER
int GetCDLabel(char *fn,char *tg)
{
 char bfr[0x100];
 char dev[0x82];
 FILE *f;
 int i,j,l;
 char *s,*t;
 l=0;

  if((f=fopen("/etc/mtab", "rb")))
  {
   while(fgets(bfr,255,f))
   {
    if((s=strchr(bfr,' ')))
    {*s=0;
     j=s-bfr;
     if(j>l)
     {
      if(!strncmp(bfr,fn,j))
      {
       if((s=strstr(s+1,"dev=")))
       {l=j;
        s+=4;
        if((t=strchr(s,' ')))*t=0;
        strncpy(dev,s,127);
       }
      }
     }
    }
   }
   fclose(f);
  }
  else
  {
    printf("Can't open /etc/mtab\n");
    return 0;
  }
  if((f=fopen(dev, "rb")))
  {
   if(fseek(f,0x8000,SEEK_SET))
   {
//    printf("Can't seek %d\n",errno);
    for(j=0;j<129;++j)i=fread(bfr,1,0x100,f);
   }
   else
   {
    i=fread(bfr,1,0x100,f);
   }
//   printf("%d bytes has been readed (%d)\n",i,errno);
   if(i>0)
   {
//    printf("Label is '%s'\n",bfr+0x28);
     strncpy(tg,bfr+0x28,48);
   }
   fclose(f);
  }
  else
  {
   printf("Can't open CD device %s\n",dev);
   return 0;
  }
  return 1;
}



#endif

/*
void * operator new[](unsigned int s)
{
  char *p=malloc(s);
  if(p)memset(p,0,s);
  return p;
}
void operator delete[](void *a)
{
  free(a);
}

void operator delete(void *a)
{
  free(a);
}

void operator delete(void *a,unsigned int x)
{
  free(a);
}
*/
