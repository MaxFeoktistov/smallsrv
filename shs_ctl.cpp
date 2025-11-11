/*
 * Copyright (C) 1999-2025 Maksim Feoktistov.
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

#ifdef SYSUNIX
#include <sys/ipc.h>
#include <sys/shm.h>
#endif
#include "srv.h"


SHMdata *shm;
#ifdef SYSUNIX
int shm_id;
int shmkey = 0x545F597;
#else
char *shmkey="SHSrv";
HANDLE hMapFile;
#endif

char *SrvNameSufix[]={
  "http",       //     0
  "proxy",      //     1
  "ftp",        //     2
  "smtp",       //     3
  "pop",        //     4
  "ssl",        //     5
  #ifndef TELNET
  "http.err",   //     10
  #else
  "telnet",   //     6
  #endif
  "dns",        //     7
  "",            //     8
  "dhcp"        //     9
  #ifdef TELNET
  ,"http.err"   //     10
  #endif
  #ifdef TLSVPN
  , "vpn"
  #endif
};

const struct timespec timeout_50ms={0, 50000000};

int OpenShmem()
{

  if (shm)
    return 0;

#ifdef SYSUNIX
  if((shm_id=shmget(shmkey, 0, 0660))==-1)
    return -1;
  //printf("SHM found");

  if ( (shm=(SHMdata *)shmat(shm_id, 0, 0)) != ((SHMdata *) -1) && shm )
#else
  if( (hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,                 // do not inherit the name
        shmkey)               // name of mapping object
      ) &&
      (shm = (SHMdata  *) MapViewOfFile(hMapFile, // handle to map object
                                           FILE_MAP_ALL_ACCESS,  // read/write permission
                                           0, 0, 0)
      )
  )
#endif
    return 0;
  return -1;
};

void  CloseShm()
{
  if(shm) {
#ifdef SYSUNIX
    shmdt(shm);
#else
    UnmapViewOfFile(shm);
    CloseHandle(hMapFile);
#endif
    shm = 0;
  }
}


typedef int (*func_t)(long par, char **arg);

struct ArgFunc
{
  const char *opt;
  const char *short_opt;
  int   n_args;
  func_t fnc;
  long par;
  const char *usage;
};

void OpenShmemExit()
{
  OpenShmem();
  if(!shm)
  {
    if (errno == 13)
      fprintf(stderr, "(%d) %s\nYou haven't permission for server control. Try to use \"sudo !!\" \n", errno, strerror(errno));
    else
      fprintf(stderr, "Server don't started or shared memory control disabled (%d) %s\n", errno, strerror(errno));
    exit(1);
  }

};


int DoCmd(int cmd)
{
  OpenShmemExit();

  shm->cmd = cmd;
  shm->reply = 0;
  kill(shm->pid, SIGUSR2);
#ifdef USE_FUTEX
  futex((int *)&shm->reply, FUTEX_WAIT, 0, &timeout_50ms, 0, 0);
  if(cmd == CMD_EXIT)
  {
    int timeout = 50;
    while(shm->reply == REPLY_INPROGRESS && (shm->status & SHMST_RUN) )
    {
      if (--timeout < 0) goto exLoop;
      futex((int *)&shm->status, FUTEX_WAIT, shm->status, &timeout_50ms, 0, 0);
    }
  }
  printf("Server stopped\n");
exLoop:;
#else
  usleep(50000);
#endif
  if (! shm->reply)
    return -2;

  return 0;
}

int DoCmdF(long cmd, char **args)
{
  return DoCmd(cmd);
}

int UsageF(long cmd, char **args);


void ShowLog(int n)
{
  const char *name = "";

  if(shm->sepLog[n].idx < ARRAY_SIZE(SrvNameSufix)) {
    name = SrvNameSufix[shm->sepLog[n].idx];
  }
  printf("### %s Log %u\n\n%s\n\n", name, n, shm->sepLog[n].lb_prot);
}

int ShowLogF(long cmd, char **args)
{
  int n = 0;

  OpenShmemExit();

  if(args && args[0]) {
    if(args[0][0] <= '9')
      n = atoi(args[0]);
    else {
      int i;
      for(i=0; i<ARRAY_SIZE(SrvNameSufix); i++)
      {
        if(!stricmp(args[0], SrvNameSufix[i])) {
          for(n=0; n < shm->n_log; n++)
          {
            if(i == shm->sepLog[n].idx)
              goto lbLogFound;
          }

          break;
        }
      }
      fprintf(stderr, "Log for \"%s\" not found\n", args[0]);
      return -1;
    }
  }

  if(n >= shm->n_log)
  {
    fprintf(stderr, "Only %u log present\n", shm->n_log);
    return -1;
  }

  lbLogFound:

  ShowLog(n);

  return 0;
}

int ShowAllLogF(long cmd, char **args)
{
  OpenShmemExit();

  for(int n=0; n < shm->n_log; n++)
    ShowLog(n);

  return 0;
}

const ArgFunc argfnc[] = {
 { "exit","e", 0, DoCmdF, CMD_EXIT       , " \t\t\tClose server"},
 { "save-log","s", 0, DoCmdF, CMD_SAVELOG , " \t\tFlash log"   },
 { "start-vpncl","v", 0, DoCmdF, CMD_START_VPNCL, " \t\tStart VPN client"},
 { "stop-vpncl","t", 0, DoCmdF, CMD_STOP_VPNCL , " \t\tStop VPN client"},
 { "send-mail","m", 0, DoCmdF, CMD_SEND_MAIL , " \t\tSend waited mail" },
 { "load-domain","l", 0, DoCmdF, CMD_LOAD_DOMENS , " \t\tReload domain configuration file"},
 { "print-log","p", 1, ShowLogF, 0 , "[#N | name] \t\tPrint current log number N or log by name"},
 { "print-all","a", 0, ShowAllLogF, 0 , " \t\tPrint all current logs"},
 { "help","h", 1, UsageF, 0 , " \t\t\tPrint this help"},
 { 0  }
};

int UsageF(long cmd, char **args)
{
  printf("Extended control for Small HTTP server.\nUsage: shs_ctl <command> [args]\ncommands:\n");
  for (const ArgFunc * p = argfnc; p->opt; p++ )
  {
    printf(" --%s, -%s %s\n", p->opt, p->short_opt, p->usage);
  }
  printf("\n");
  exit(1);
}


extern "C" int main(int argc, char *argv[])
{
  int ret = 0;

  if(argc < 2)
    UsageF(0, 0);

  for(int i=1; i<argc; i++)
  {
    for (const ArgFunc * p = argfnc; p->opt; p++ )
    {
      if(argv[i][0] == '-')
      {
        if(strstr(argv[i], p->opt) || !strcmp(argv[i]+1, p->short_opt))
        {
          printf("%s\n", p->opt);
          ret = p->fnc(p->par, argv + i + 1);
          i += p->n_args;
          goto lb_cont;
        }
      }
    }
    UsageF(0, 0);
   lb_cont:;
  }
  printf("\n");
  CloseShm();
  exit(ret);
}


