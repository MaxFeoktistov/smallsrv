/*
 * Copyright (C) 1999-2026 Maksim Feoktistov.
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

// #define DEBUG_VERSION 1

#ifndef SRV_H
#include "srv.h"
#endif

#ifdef SEPLOG

#undef debug
#undef AddToLog

#define debug(a...)  sepLog[2]->Ldebug(a)
#define AddToLog(a...)  sepLog[2]->LAddToLog(a)

#endif

#ifndef SYSUNIX
#undef open
#define open lopen_UTF
#endif
extern "C" {
  int tftp_port = 69;
//char *tftp_addapter;
#define tftp_addapter bind_a[STFTP_IND]
  int max_tftp_files = 128;
  int opened_tftp_files;
  int tftp_timeout = 60;
  char *TFTPdir;

#define MAX_TFTP_PKT 1472
  int max_block_size = (MAX_TFTP_PKT - 4);

  struct TFTPReq : public Req {
    u32 block_size;
    u32 window_size;
    u32 pos;
    int fd;
    int eof;
    time_t expired;

//#define block_size postsize

    void Init() { block_size = 512; window_size = 1; }
    int  Ack();
//  void Err();
    void Done();
    TFTPReq *next() {return (TFTPReq *) Adv; }
  };


  struct TFTPDataPkt {
    u8 opcode0;
    u8 opcode;
    u16 block;
    u8  data[1];
  } PACKED;

  struct TFTPOpenPkt {
    u8 opcode0;
    u8 opcode;
    char fname[1];
  } PACKED;

  enum {
    OPRD   = 1, //     Read request (RRQ)
    OPWR   = 2, //     Write request (WRQ)
    OPDATA = 3, //     Data (DATA)
    OPACK  = 4, //     Acknowledgment (ACK)
    OPERR  = 5, //     Error (ERROR)
    OACK   = 6
  };

  TFTPReq * TFTPList;

  static union {
    TFTPDataPkt *TFTPbfr;
    TFTPOpenPkt *TFTPopen_bfr;
    char *TFTPbfr8;
  };
//int tftp_s;


  int TFTPSendErr(TSOCKADDR *to, int code, char *txt)
  {
    int l;
    TFTPbfr->opcode = OPERR;
    TFTPbfr->block = htons(code);
    l = sprintf((char *) TFTPbfr->data, "%s", txt);
    DBGLA("Send err %s", txt)
    return sendto(tftp_s, TFTPbfr8, l + 4, 0, (sockaddr *)to, SockAddrSize(to));
  }

  static char * ChkOpt(char *p, int lopt, u32 *val, int min, int max)
  {
    int v;
    char *rp;

    p += lopt;
    v = strtoul(p, &rp, 10);
    if(v < min) v = min;
    if(v > max) v = max;
    *val = v;
    if(rp) p = rp;
    return p;
  }

  TFTPReq *TFTPOpen(TFTPReq *r, int ll, TSOCKADDR *sa)
  {
    int fd;
    char fname[512];
    //int l = strlen(TFTPopen_bfr->fname);
    char *p = CheckBadName(TFTPopen_bfr->fname);
    char *ep;
    TFTPReq *rr = r;
    int opt = 0;

    if(!p) {
      AddToLog(0, -tftp_port, sa, ">Bad name: %.64s", TFTPopen_bfr->fname);
      TFTPSendErr(sa, 1, "Bad name");

      return 0;
    }

    if(opened_tftp_files >= max_tftp_files)
    {
      AddToLog(0, -tftp_port, sa, ">Too many opened files %.64s", TFTPopen_bfr->fname);
      TFTPSendErr(sa, 0, "Too many opened files");
      return 0;
    }
    // TODO: Check opened files from the same host

    sprintf(fname, "%.255s/%.255s", TFTPdir, TFTPopen_bfr->fname);
    DBGLA("%s", fname)
    fd = open(fname, O_RDONLY);
    if(fd < 0) {
      AddToLog(0, -tftp_port, sa, ">Not found: %.64s", TFTPopen_bfr->fname);
      TFTPSendErr(sa, 1, "Not found");
      return 0;
    }

    if(!rr)
      rr = (TFTPReq *) Malloc(sizeof(TFTPReq));

    if(!rr) {
      close(fd);
      AddToLog(0, -tftp_port, sa, ">Error open %.128s", TFTPopen_bfr->fname);
      return 0;
    }
    rr->Init();
    rr->fd = fd;
    memcpy(&rr->sa_c46, sa, SockAddrSize(sa));
    sprintf(rr->inf, "%.63", TFTPopen_bfr->fname);
    rr->tmout = GetTickCount();
    rr->expired = cur_time + tftp_timeout;

    if(r != rr)
    {
      rr->Adv = TFTPList;
      TFTPList = rr;
    }
    opened_tftp_files ++;

    ep = (char *)TFTPbfr8 + ll;
    for(p = TFTPopen_bfr->fname; p < ep; )
    {
      int l;
      l = strlen(p);
      p += l + 1;
      if(p >= ep) break;
      DBGLA("opt: %s", p)
      if(!strcmp(p, "blksize") )
      {
        p = ChkOpt(p, sizeof("blksize"), & rr->block_size, 512, max_block_size);
        opt |= 1;
      }
      else if(!strcmp(p, "windowsize") )
      {
        p = ChkOpt(p, sizeof("windowsize"), & rr->window_size, 1, 32);
        opt |= 2;
      }
    }

    AddToLog(0, -tftp_port, sa, ">Open %.128s %u %u opened_tftp_files: %d", TFTPopen_bfr->fname, rr->block_size, rr->window_size, opened_tftp_files);
    rr->pos = 0;
    TFTPbfr->block = 0;
    if(opt)
    {
      int l = 0;

      TFTPbfr->opcode = OACK;
      p = TFTPopen_bfr->fname;
      if(opt & 1) l = sprintf(p, "blksize\x00%u", rr->block_size) + 1;
      if(opt & 2) l += sprintf(p + l, "windowsize\x00%u", rr->window_size) + 1;
      sendto(tftp_s, TFTPbfr8, l + 2, 0, (sockaddr *)sa, SockAddrSize(sa));
    }
    else rr->Ack();

    return rr;
  }


  int TFTPReq::Ack()
  {
    int i;
    int l;
    u32 blk = htons(TFTPbfr->block);

    DBGLA("TFTPbfr->block: %u %u", blk, pos)
    if(blk != pos)
    {
      off_t p;

      pos = blk;
      if(lseek(fd, p = pos * block_size, SEEK_SET) != p)
      {
        //Err();
        TFTPSendErr((TSOCKADDR *) &sa_c, 0, "End of file");
        Done();
        return 0;
      }
    }
    else if(eof) {
      DBGLA("EOF")
      Done();
      return 0;
    }


    TFTPbfr->opcode = OPDATA;
    for(i = 0; i < window_size; i++)
    {
      TFTPbfr->block = htons(pos + 1);
      l = _hread(fd, (char *)TFTPbfr->data, block_size);
      if(l < 0) {
        l = 0;
      }
      if(sendto(tftp_s, TFTPbfr8, l + 4, 0, (sockaddr *) &sa_c, SockAddrSize((TSOCKADDR *) &sa_c) ) <= 0) {

        DBGLA("Send error %d " SER, WSAGetLastError() Xstrerror(errno))

        break;
      }
      Tout += l + 4;

      if(l != block_size) {
        eof = 1;
        DBGLA("l != block_size: %d %d", l, block_size)
        expired = cur_time + 5;
        break;
      }
      pos ++;
    }

    return 1;
  };

  void TFTPReq::Done()
  {
    DBGLA("fd: %d opened_tftp_files:%d", fd, opened_tftp_files)
    if(fd > 0)
    {
      close(fd);
      fd = -1;
      eof = 1;
      if(opened_tftp_files)
        opened_tftp_files--;
    }
  }

  TFTPReq *FindTFTP(TSOCKADDR *sa_c)
  {
    TFTPReq *r;
    TFTPReq **pr = &TFTPList;

    for(r = TFTPList; r; )
    {
      if(!memcmp(&r->sa_c46, sa_c, SockAddrSize(sa_c) ) )
      {
        r->expired = cur_time + tftp_timeout;
        return r;
      }
      if(r->expired < cur_time)
      {
        TFTPReq *rr;

        r->Done();
        *pr = rr = r->next();
        free(r);
        r = rr;
        continue;
      }

      pr = (TFTPReq **) &r->Adv;
      r = r->next();

    }
    return 0;
  }
  void TFTPRemove(TFTPReq *rr)
  {
    TFTPReq *r;
    TFTPReq **pr = &TFTPList;

    for(r = TFTPList; r; r = r->next())
    {
      if(r == rr) {
        r->Done();
        *pr = r->next();
        free(rr);
        return;
      }
      pr = (TFTPReq **) &r->Adv;
    }

    DBGLA("Not found req: %X", rr)

  };

  ulong WINAPI TFTPServer(void *)
  {
    TFTPReq *r;
    TSOCKADDR sa_c;
    int la;
    int l;

    tftp_s = UDPSrvSock46(tftp_port, tftp_addapter, (s_flgs[2] & FL2_TFTP_IPV6) ? AF_INET6 : AF_INET);

    if(tftp_s <= 0)
      return -1;

    TFTPbfr = (TFTPDataPkt *) malloc(MAX_TFTP_PKT + 8);

    while(is_no_exit)
    {
      if(RESelect1(3, 3, tftp_s))
      {
        la = sizeof(sa_c);
        if( (l = recvfrom(tftp_s, TFTPbfr8, MAX_TFTP_PKT, 0, (sockaddr *)& sa_c, &la)) < 0)
        {
          AddToLog("TFTP receive error", -tftp_port, &sa_c);
          break;
        }

        if(l > 2 && IsInIPRR(tftp_range, (sockaddr_in *) &sa_c)
            && IsInIPRR(ip_range, (sockaddr_in *) &sa_c)
          )
        {
          if(!ChkThread)
            cur_time = time(0);

          r = FindTFTP(&sa_c);

          //DBG_CODE( if(r) DBGLA("Found")  )

          switch(TFTPbfr->opcode)
          {
            case OPRD  :
              if(r) r->Done();
              if(!TFTPOpen(r, l, &sa_c))
              {
                if(r)
                  TFTPRemove(r);
              }
              break;
            case OPWR  : TFTPSendErr(&sa_c, 2, "Write not supported"); break;
            case OPACK :
              if(r) {
                if (!r->Ack())
                  TFTPRemove(r);
              }
              break;

            case OPERR :
              if(r) {
                r->Done();
                TFTPRemove(r);
              }
              break;

            case OPDATA:
            default:
              TFTPSendErr(&sa_c, 4, "Unsupported"); break;
          }
        }
      }
    }

    closesocket(tftp_s);
    return 0;

  }

} //extern "C"

