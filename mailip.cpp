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

#define DEBUG_VERSION 1

#ifndef SRV_H
#include "srv.h"
#endif


#ifdef SEPLOG

#undef debug
#undef AddToLog

#define debug(a...)  sepLog[3]->Ldebug(a)
#define AddToLog(a...)  sepLog[3]->LAddToLog(a)

#endif

#define  rtypeA_BE    0x100
#define  rtypeMX_BE   0xF00
#define  rtypeTXT_BE  0x1000
#define  rtypeSPF_BE  0x6300
#define  rtypePTR_BE  0xC00
#define  rtypeAAAA_BE 0x1C00


//char *dns_server_for_mail=0;
struct d_msgt {
  ushort id,
         flags,
         qdcount,
         ancount,
         nscount,
         arcount;
}
 msg_reqmx={0x100,0x1,0x100,0,0,0};


char* CopyDName(char *t, char *s)
{
 char *tt,*t1;
 strcpy(t+1,s);
 tt=t;
 do{
    t1=strchr(tt+1,'.');
    if(!t1)
    {
     *tt=strlen(tt+1);
     break;
    }
    *tt=t1-tt-1;
    tt=t1;
   }while((t-tt)<255);
 return  tt + (*tt)+2;
};


void  MHinfo::InitHst(uchar *t1)
{
 char  b[80];
// uchar *t1;
 int   tl[8];
 int i,j,k;
 memset(ta,0,sizeof(ta)+20);
// t1=beg+4;
 for(j=i=0;j++ <7;++i)
 {
  t1=(uchar *)DecodeName(b //t2
    ,(char *) t1, (char *) beg);
//  debug("%s %X",b,DWORD_PTR(*t1));
  if(  DWORD_PTR(*t1)==0x01000F00 )
  {
   ta[i]=(char *) t1 +12;
   tl[i]=htons(WORD_PTR(t1[10]));
   FirstIP=t1=(uchar *)DecodeName(b,(char *) t1 +12, (char *) beg);
  }
  else
  {
    if(  DWORD_PTR(*t1)==0x01000100 || ! (t1[1]) ) break ;
    FirstIP=t1=(uchar *)DecodeName(b,(char *) t1 +10, (char *) beg);
    --i;
  }

//  debug("MH:%s",b);
  //if(!t1) break;
 }
 nhst_hst=i;
 LastIP=FirstIP;
 for(k=0; k<i; ++k)
   for(j=k+1;j<i;++j)
    if(tl[j]<tl[k])
    {xchgp(ta[k],ta[j]);
     xchg(tl[k],tl[j]);
    }

}
uchar *MHinfo::GetNextMH(char *t)
{
 char  b[80];
 char *t1;
 //debug("==========%X %X %X",LastIP,ta[0],ta[1]);
 if(DWORD_PTR(LastIP)==1)
 {
  //debug("!!!!!! %X %X",ta[0],ta[1]);
  strcpy(t,ta[0]);
  LastIP=0;
  t1=((char *)(ta+1))-10;
  goto lbIP;
 }
tryNext: ;
 if(cur_hst>=nhst_hst) return 0;
 if( ((ulong) (ta[cur_hst]-(char *) beg)) >500ul )
 {
  // debug("MH error %X %X %u",ta[cur_hst],(char *) beg,cur_hst);
   return 0;
 }
 DecodeName(t,ta[cur_hst],(char *) beg);
 sip[0]=0;
 if(FirstIP)
 {
   if(!LastIP)LastIP=FirstIP;
//   t[68]=0;
   do
   {
    t1=DecodeName(b,(char *)LastIP,(char *) beg);
//debug("FMH |%s| %X %s",b,DWORD_PTR(*t1),t);
    if(WORD_PTR(*t1)!=0x0100) break;
    LastIP=(uchar *) (t1+10+t1[9]);
    if( DWORD_PTR(*t1)==0x01000100 &&
        ! stricmp(t,b)
      )
    {
 lbIP:
     sprintf(sip,"%u.%u.%u.%u",((uchar *)t1)[10],((uchar *)t1)[11],((uchar *)t1)[12],((uchar *)t1)[13]);
//     debug("MHIP:%s %s",t,sip);
     lIP=DWORD_PTR(t1[10]);
     ++cur_ip;
     return (uchar *)t;
    }
   }while(1);
   ++cur_hst;
   LastIP=FirstIP;
   if(cur_ip)
   {cur_ip=0;
    goto tryNext;
   }
 }
 else ++cur_hst;

 return (uchar *)t;
}

//inline uchar * GetNextMH(uchar *beg) //,uchar *t1,char *t2)
//{ return  ((MHinfo *)beg)->GetNextMH((char *)beg+512);}

int  MHinfo::CheckIP(ulong ip)
{
 struct hostent *hp;
 ulong **p;

  while(GetNextMH((char *) name) )if(sip[0])
  {
   if(lIP ==ip )return 1;
  }
  else
  {
    if( (hp=gethostbyname((char *) name)) )
    {
     for( p=(ulong **)  hp->h_addr_list; *p ; ++p )if(*p[0]==ip)return 1;
    }
  }
  return 0;
}

//inline int CheckMHIP(uchar *beg,ulong ip) { return  ((MHinfo *)beg)->CheckIP(ip);}

char *askDNS(char *adr, d_msg  *dmm, int type_be)
{
  int dns_s;
  //int addr_dns;
  struct sockaddr_in sa;
  int i;
  int j=4;
  char *t;



  if( (!dns_server_for_mail) ||
    (dns_s= socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0)
  {
    return 0;
  }
  #ifdef SYSUNIX
  fcntl(dns_s, F_SETFD, fcntl(dns_s, F_GETFD) | FD_CLOEXEC);
  #endif
  //addr_dns
  if(!dns_for_mail_ip)
    dns_for_mail_ip=ConvertIP(t=dns_server_for_mail);

  DBGLA("%s %X", adr, type_be)

  memset((char *)&sa, 0, sizeof(sa));
  sa.sin_addr.s_addr = dns_for_mail_ip;
  sa.sin_family=AF_INET;
  sa.sin_port=0x3500;  //htons(53);
  ++msg_reqmx.id;
  memcpy(dmm, &msg_reqmx, sizeof(msg_reqmx));
  t=CopyDName(dmm->buf, adr);
  WORD_PTR(*t)= type_be;     //type MX
  WORD_PTR(t[2])=0x0100;   //class IN
  t += 4;

  do{
    i=-1;

    if( (i=sendto(dns_s,(char *)dmm,t - ((char *)dmm),0,(sockaddr *)&sa ,sizeof(sockaddr) )) <=0)
    {
      debug("SMTP DNS error send...");
      break;
    }
    if(RESelect(2+j,0,1,dns_s)>0)
    {
      i=recvfrom(dns_s,(char *)dmm,512,0,(sockaddr *)&sa, &(i=sizeof(sockaddr)));
      break;
    }
  }while(--j>0);

  CloseSocket(dns_s);
  //debug("MAILIP: %d %X %u %X %X adr %X",i,dmm->flags,jchk,s_flgs[1]&FL1_MHST,t+4,adr);
  if( i<=0 )
  {
    //dbg2("Resv error...");
    return 0;
  }
  DWORD_PTR( ((char*)dmm)[i] ) = 0;

  if((dmm->flags&0xF00) || !dmm->ancount) {
    DBGLA("Fail %s %X flags: %X ancount %d", adr, type_be, dmm->flags, dmm->ancount)
    return 0;
  }

  return t;
}


//---------
char* GetMailHost(char *adr, d_msg  *dmm,int jchk)
{
 //d_msg  dm;
#define dm  (*dmm)
 char *t;
 //debug("x %X,",(dmm->flags&0xF00));
 if(! dns_server_for_mail)  // (dmm->flags&0xF00) || ! (dmm->ancount) )
 {
   struct hostent *hp;
   //debug("|%s| %X %u",adr,!(s_flgs[1]&FL1_MHST),jchk);

   if( (jchk)   ||
     ((s_flgs[1]&FL1_MHST)==0ul)
     ||  !(hp=gethostbyname(adr)) )
     return 0;
   //||
   ;

   //debug("hp=%X %X",hp,hp?*(ulong *)(hp->h_addr):0);
   // if( !(hp)  )return 0;

   memset(((MHinfo *)dmm)->ta,0,sizeof(((MHinfo *)dmm)->ta)+20);
   ((MHinfo *)dmm)->ta[0]=adr;
   DWORD_PTR(((MHinfo *)dmm)->ta[1])=*(ulong *)(hp->h_addr);
   DWORD_PTR(((MHinfo *)dmm)->LastIP)=1;
 }
 else
 {
   t = askDNS(adr, dmm, rtypeMX_BE); //type MX

   if(!t)
     return 0;

   if(!jchk)
   {
     //!!! DWORD_PTR( ((char*)dmm)[i] )=0;
     // ((char*)dmm)[0]=0;
     ((MHinfo *)dmm)->InitHst((uchar *)t);  // !!! +4);
   }
 }
 return t; //!!! t+4;
};

char* DNS_RR_ParseHelper::FindInfo()
{
  if(next_rr && *next_rr)
  {
    union {
      char * r;
      RR_info *rr;
    };

    r = DecodeName(host, (char *)next_rr, beg);
    if(r)
    {
      next_info = rr;
      next_l = htons(rr->rdlength);
      if( (rr->rdata + next_l - beg) < 512)
        return rr->rdata;
    }

  }
  next_info = 0;
  return 0;
}

int DNS_RR_ParseHelper::Next()
{
  if(!next_info)
  {
     if(!FindInfo())
       return 0;
  }
  next_rr = next_info->rdata + next_l;
  next_info = 0;
  if((next_rr - beg) >  512)
  {
    next_rr = 0;
    return 0;
  }
  return 1;
}

void  DNS_RR_ParseHelper::NormalezeText()
{
  char *p;
  saved_next = * (p = (next_info->rdata + next_l));
  *p = 0;
}
void  DNS_RR_ParseHelper::RestoreText()
{
  next_info->rdata[next_l] = saved_next;
}

int DNS_RR_ParseHelper::FindRR(int type_be, void *par, dns_rr_cmp_func_t cmp_func)
{
  while(FindInfo())
  {
    if(next_info->type == type_be)
    {
      if(! CMP_FUNC(this, par))
        return SPF_OK;
    }

    if(!Next())
      break;
  }

  return SPF_NOT_FOUND;
}

#ifdef DONT_USE_CPP_METHOD_POINTER

int cmpAAAA(DNS_RR_ParseHelper *th, void *b)
{
  return memcmp(th->next_info->rdata, b, 16);
}

int cmpA(DNS_RR_ParseHelper *th, u32 *a)
{
  DBGLA("A %X <> %X", DWORD_PTR(th->next_info->rdata[0]), (u32) (long) a)
  return  DWORD_PTR(th->next_info->rdata[0]) - (u32) (long) a;
}

#define  P_DNS_RR

#else

#define  P_DNS_RR &DNS_RR_ParseHelper::

#endif

int DNS_RR_ParseHelper::FindA(char *t, TSOCKADDR *sa_c)
{

  next_rr = t;

  if(!IsIPv6((sockaddr_in *) sa_c))
  {
    unsigned long ip4 = IPv4addr((sockaddr_in *) sa_c);

    return FindRR(rtypeA_BE, (void *) ip4, (dns_rr_cmp_func_t) P_DNS_RR cmpA);
  }

  return FindRR(rtypeAAAA_BE, ((sockaddr_in6 *)sa_c)->sin6_addr.s6_addr32, P_DNS_RR cmpAAAA);
}

int CmpIPforHost(char *host, TSOCKADDR *sa_c)
{
  d_msg dmm;
  int typA_be = IsIPv6((sockaddr_in *) sa_c)? rtypeAAAA_BE : rtypeA_BE;
  char *t = askDNS(host + 2, &dmm, typA_be);

  DBGLA("MX %s %u", host, (int) !!t)

  if(t)
  {
    DNS_RR_ParseHelper parser(&dmm, t);
    return ! parser.FindA(t, sa_c);
  }
  return -1;
}
#ifdef DONT_USE_CPP_METHOD_POINTER

int cmpMX(DNS_RR_ParseHelper *th, TSOCKADDR *sa_c)
{
  DecodeName(th->host, th->next_info->rdata + 2, th->beg);
  return CmpIPforHost(th->host, sa_c);
}

int cmpPTR(DNS_RR_ParseHelper *th, char *hst)
{
  DecodeName(th->host, th->next_info->rdata + 2, th->beg);

  DBGLA("PTR %.64s %.64s", hst, th->host)

  return stricmp(th->host, hst);
}

#else // USE_CPP_METHOD_POINTER

int DNS_RR_ParseHelper::cmpAAAA(void *b)
{
  DBGLA("cmp AAAA")
  return memcmp(next_info->rdata, b, 16);
}

int DNS_RR_ParseHelper::cmpA(u32 *a)
{
  DBGLA("A %X <> %X", DWORD_PTR(next_info->rdata[0]), (u32) (long) a)
  return  DWORD_PTR(next_info->rdata[0]) - (u32) (long) a;
}

int DNS_RR_ParseHelper::cmpMX(TSOCKADDR *sa_c)
{
  DecodeName(host, next_info->rdata + 2, beg);
  return CmpIPforHost(host, sa_c);
}

int DNS_RR_ParseHelper::cmpPTR(char *hst)
{
  DecodeName(host, next_info->rdata + 2, beg);

  DBGLA("PTR %.64s %.64s", hst, host)

  return stricmp(host, hst);
}

#endif // DONT_USE_CPP_METHOD_POINTER


int find_spf_patern(char *s, char *pat, char* prefix)
{
  int l = strlen(pat);
  char *r;

  while( (r = stristr(s, pat)) )
  {
    if(r[l] < '0' && r[l] !=':' && strchr(prefix, r[-1]))
      return 1;

    s = r + 1;
  }

  return 0;
}

int CheckSPF(char *host, TSOCKADDR *sa_c, d_msg *dmm, int type_be)
{
  char *t;
  int ret = SPF_NOT_FOUND;
  int inc_limit = 5;

  if (type_be & 0xFFFF0000)
  {
    inc_limit = type_be >> 16;
    type_be &= 0xFFFF;
  }

  t = askDNS(host, dmm, type_be);

  if(t)
  {
    DNS_RR_ParseHelper parser(dmm, t);
    char ip[64];
    char *found;
    char *rd;
    bool is6 = IsIPv6((sockaddr_in *) sa_c);

    IP2S(ip, (sockaddr_in *)sa_c);

    debug("find SPF for %s - %s\r\n", host, ip);

    while(parser.FindInfo())
    {
      DBGLA("type: %X len: %X %d", parser.next_info->type, parser.next_info->rdlength, parser.next_l)

      if(parser.next_info->type == type_be)
      {
        parser.NormalezeText();

        DBGLA("%.64s ", parser.next_info->rdata)

        if(stristr(rd = (char *) parser.next_info->rdata, "v=spf1") )
        {
          u32  flags = 0;
          enum PresentFlags {
            HAVE_A = 1,
            HAVE_MX = 2,
            HAVE_PTR = 4,
            HAVE_INCLUDE = 8,
            HAVE_MX2 = 0x10,
            HAVE_A_MX = 3,
            HAVE_A_MX_PTR = 7,
          };

          ret = SPF_IP_EXCLUDED;
          if(stristr(rd, "-all")) ret = SPF_HARD;
          if(stristr(rd, "~all")) ret = SPF_SOFT;

          if(find_spf_patern(rd, "a", "+ \t\r\n"))   flags |= HAVE_A;
          if(find_spf_patern(rd, "mx", "+ \t\r\n"))  flags |= HAVE_MX;
          if(find_spf_patern(rd, "ptr", "+ \t\r\n")) flags |= HAVE_PTR;
          if(stristr(rd, "include:"))   flags |= HAVE_INCLUDE;
          if(stristr(rd, " mx:"))   flags |= HAVE_MX2;

          /*
          if(stristr(rd, " a "))   flags |= 1;
          if(stristr(rd, " mx "))  flags |= 2;
          if(stristr(rd, " ptr ")) flags |= 4;
          */
#ifdef SUPPORT_EXISTS_SPF
          if(stristr(rd, " exists ")) flags |= 0x20;
#endif

          DBGLA("Found %s (ret:%d flags:%X inc_left:%d)\r\n", rd, ret, flags, inc_limit)

          found = strstr(rd, ip);
          if(found)
            return SPF_OK;

          do {
            found = stristr(rd, is6 ? "ip6:":"ip4:");
            if(!found)
              break;

            found += 4;

            t = strpbrk(found, "/ \t\r\n");
            if(t && *t == '/')
            {
              int nmsk = atoui(t+1);

              DBGLA("net %d (%.16s)", nmsk, found)
#ifdef USE_IPV6
              if(is6)
              {
                addr_v6_t net;
                int i = 0;

                IPv6Addr(net.us, found);

                for(i = 0; i<4; i++)
                {
                  if(nmsk > 32)
                  {
                    if(net.ui[i] != ((sockaddr_in6 *)sa_c)->sin6_addr.s6_addr32[i])
                      break;
                  }
                  else
                  {
                    u32 msk = htonl((~0) << (32 - nmsk));

                    if( ! ((net.ui[i] ^ ((sockaddr_in6 *)sa_c)->sin6_addr.s6_addr32[i]) & msk))
                      return SPF_OK;

                    break;
                  }

                  nmsk -= 32;
                }
              }
              else
#endif
              {
                u32 msk = htonl((~0) << (32 - nmsk));
                u32 net = ConvertIP(found);

                DBGLA("net %X %X", net, msk)

                if(! ((IPv4addr((sockaddr_in *) sa_c) ^ net) & msk)) {

                  DBGLA("into net %X", net)
                  return SPF_OK;
                }

                DBGLA("outside net %X", net)
              }
            }

            rd = found + 1;
          }while(1);


          for(found = (char *) parser.next_info->rdata ; (found = stristr(found, " a:")) ; )
          {
            found+=3;
            t = strpbrk(found, " \t\r\n");
            if(t) *t = 0;
            if(!CmpIPforHost(found, sa_c))
              return SPF_OK;
            if(t) *t = ' ';
          }
          if(flags)
          {
            d_msg *dmm2 = dmm;
            DNS_RR_ParseHelper *pparser = &parser;
#if __cplusplus >= 201103L && ! defined(DONT_USE_LAMBDA)
            auto alloc_dmm2 = [&]()
            {
              dmm2 = (d_msg *) malloc(sizeof(d_msg) + 4 + sizeof(DNS_RR_ParseHelper));
              if(!dmm2) dmm2 = dmm;
              else
              {
                pparser = (DNS_RR_ParseHelper *) (dmm2 + 1);
                pparser->dmm = dmm2;
              }
            };
#else
#define alloc_dmm2()  { dmm2 = (d_msg *) malloc(sizeof(d_msg) + 4 + sizeof(DNS_RR_ParseHelper)); \
                        if(!dmm2) dmm2 = dmm; \
                        else { \
                          pparser = (DNS_RR_ParseHelper *) (dmm2 + 1); \
                          pparser->dmm = dmm2; \
                        }  \
                      }
#endif

            //int typA_be = is6 ? rtypeAAAA_BE : rtypeA_BE;

            if(flags & HAVE_A)
            {
              DBGLA("Check A")
              if(!CmpIPforHost(host, sa_c))
                return SPF_OK;
              /*
              t = askDNS(host, dmm, typA_be);
              if(!t) return ret;

              if(parser.FindA(t, sa_c)) return SPF_OK;
              */
            }

            if(flags & HAVE_MX)
            {

              DBGLA("Check MX")
              if(flags & ~ HAVE_A_MX_PTR) alloc_dmm2();

              t = askDNS(host, dmm2, rtypeMX_BE);
              if(t)
              {

                if(pparser->FindA(t, sa_c))
                {
                  DBGLA("Found A in MX reply")
                  ret_ok:
                  if(dmm2 != dmm) free(dmm2);
                  return SPF_OK;
                }

                pparser->next_rr = t;
                if(pparser->FindRR(rtypeMX_BE, sa_c, (dns_rr_cmp_func_t) P_DNS_RR cmpMX))
                  goto ret_ok;
              }

            }

            if(flags & HAVE_PTR)
            {
              if(!is6)
              {
                union
                {
                  u32 ip4;
                  u8  ip4b[4];
                };

                ip4 = IPv4addr((sockaddr_in *) sa_c);
                if(dmm2 == dmm && (flags & ~ HAVE_A_MX_PTR))
                  alloc_dmm2();

                char *ptr = ((char *)dmm2) + 256;

                sprintf(ptr, "%u.%u.%u.%u.IN-ADDR.ARPA",
                        ip4b[3], ip4b[2], ip4b[1], ip4b[0]);

                t = askDNS(ptr, dmm2, rtypePTR_BE);
                if(t)
                {
                  pparser->next_rr = t;
                  if(pparser->FindRR(rtypePTR_BE, host, (dns_rr_cmp_func_t) P_DNS_RR  cmpPTR ))
                    goto ret_ok;
                }
              }
            }

            if(flags & HAVE_MX2)
            {
              if(dmm2 == dmm)
                alloc_dmm2();

              for(found = (char *) parser.next_info->rdata ; (found = stristr(found, " mx:")) ; )
              {
                char *tt;

                found += 4;
                tt = strpbrk(found, " \t\r\n");
                if(tt) *tt = 0;

                t = askDNS(found, dmm2, rtypeMX_BE);
                if(t)
                {
                  if(pparser->FindA(t, sa_c))
                  {
                    DBGLA("Found A in MX2 reply")
                    goto ret_ok;
                  }

                  pparser->next_rr = t;
                  if(pparser->FindRR(rtypeMX_BE, sa_c, (dns_rr_cmp_func_t) P_DNS_RR cmpMX))
                    goto ret_ok;
                }
                if(tt) *tt = ' ';
              }
            }

            if(dmm2 == dmm) return ret;
            free(dmm2);
          }

          if((found = stristr((char *) parser.next_info->rdata, "include:")))
          {
            found += sizeof("include:") - 1;

            DBGLA("Found include %.64s (%d)", found, inc_limit)

            if(inc_limit > 1)
            {
              char *p = stristr((char *) parser.next_info->rdata, "redirect=");

              if(p)
              {
                char *pp = found  - (sizeof("include:") - 1);
                found = p + sizeof("redirect=") - 1;
                p = pp;
              }
              else
                p = stristr(found, "include:");

              if(p)
              {
                d_msg *dmm2;

                DBGLA("More include %.64s", p)

                dmm2 = (d_msg *) malloc(sizeof(d_msg) + 4);
                if(dmm2)
                {
                  do
                  {
                    p += sizeof("include:") - 1;
                    t = strpbrk(p, " \t\r\n");
                    if(t) *t = 0;
                    if(CheckSPF(p, sa_c, dmm2, type_be | ((inc_limit - 1) << 16) ) == SPF_OK)
                    {
                      free(dmm2);
                      return SPF_OK;
                    }

                    if(t) *t = ' ';

                    p = stristr(p, "include:");
                  } while(p);

                  free(dmm2);
                }
              }
            }
          }
          else if((found=stristr((char *) parser.next_info->rdata, "redirect=")))
            found += sizeof("redirect=") - 1;

          if(found)
          {
            if( --inc_limit < 0  )
            {
              debug("Too many SPF includes\r\n");
              break;
            }
            if((t=strpbrk(found, " \t") ))
              *t = 0;

            if(!stricmp(host, found))
            {
              debug("SPF include loop detected (%s)\r\n", found);
              break;
            }
            debug("SPF try include %s\r\n", found);

            t = askDNS(found, dmm, type_be);
            if(t)
            {
              parser.next_rr = t;
              continue;
            }
            break;
          }

        }
        parser.RestoreText();
      }

      if(!parser.Next())
        break;
    }

  }
  DBGLA("SFP check return %d", ret)
  return ret;

}

int CheckSPF_TXT(char *host, TSOCKADDR *sa_c, d_msg *dmm)
{
  int r = CheckSPF(host, sa_c, dmm, rtypeSPF_BE);
  if(!r) r = CheckSPF(host, sa_c, dmm, rtypeTXT_BE);

  return r;
}
