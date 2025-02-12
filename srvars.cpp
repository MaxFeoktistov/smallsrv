/*
 * Copyright (C) 1999-2021 Maksim Feoktistov.
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


char *last_cfg;
char *cmdline;
host_dir hsdr={0,0,0};
//char b_prot[0x4580];
char efix[0x100];
int count_of_tr;
int post_limit=0x30000,DnsTms=800;
int soc_port[]={80,3128,21,25,110,443,23};
int up_proxy_port=3128,max_cont_st=8;
char  *bind_a[MAX_SERV];
char *up_user;
char *ftp_upload;
ulong max_msg_size=0x100000;
char *smtp_name="shttp.srv";
char *dns_server_for_mail="127.0.0.1";
#ifdef TELNET
char *tel_cmd="/bin/bash -i -s";
#endif

char *out_path=
#ifndef SYSUNIX
   "c:\\"
#endif
   "outbox";
char *err_path="err";
char *antiv,*antispam,*smtproxy,*spamfltr,*dnsbl;

#ifndef CD_VER
CheckFileTime smtp_chk=
{0, //CheckProxyTime;
 0x6000,//proxy_time;
 "msg",//ext[4];
 0//proxy_dir;
};
#endif

int FTPTimeout=60;
int POPTimeout=60;
int PRXTimeout=120;
char *up_proxy;
u32  ip_cach[33];
int  iip_cach;
int  ip_cach_mtx;
ulong *Range[MAX_SERV*2];
const char full_rng[]=
#ifndef CD_VER
"1.0.0.1-254.254.254.254";
#else
"127.0.0.1" ;
#endif
char *Rangec[MAX_SERV*2]={
(char *)full_rng,"",  // http
(char *)full_rng,"",  // proxy
(char *)full_rng,"",  // ftp
"127.0.0.1","",  // smtp
(char *)full_rng,"",  // pop
(char *)full_rng,"",  // ssl
(char *)full_rng,"",  // ip
(char *)full_rng,"",  // dns
"127.0.0.1","",  // adm
(char *)full_rng,"",  // tel
};
#ifdef USE_IPV6
const char full_rng6[]="::1-FFFF:FFFF:FFFF:FFFF::";
char *Rangec6[MAX_SERV*2]={
(char *)full_rng6,"",  // http
(char *)"FE80::-FEFF::","",  // proxy
(char *)full_rng6,"",  // ftp
"::1","",  // smtp
(char *)full_rng6,"",  // pop
(char *)full_rng6,"",  // ssl
(char *)full_rng6,"",  // ip
(char *)full_rng6,"",  // dns
"::1","",  // adm
(char *)full_rng6,"",  // tel

};
in6_addr *Range6[MAX_SERV*2];
#endif

char *header="Server: SHS";
#ifndef G4STRING_CONST_H
const char AuthErr[]=AUTH_ERR;
#else
#define AuthErr AUTH_ERR
#endif

#ifndef CD_VER

CheckFileTime proxy_chk=
{0,//CheckProxyTime
 0x6000,//proxy_time
 "asi",//ext[4]
 0//proxy_dir
};
#endif

#define proxy_dir  (proxy_chk.dir)
#define proxy_time (proxy_chk.time)
#define CheckProxyTime proxy_chk.CheckProxyTime
ulong last_file=0;
THREADHANDLE HSmtpSendThrd;
int SMTPCounter;
ulong ltime[10],ip_limit[10]={0x1000000};
ulong limit[10]={0x1000000};
ulong net_limit[10]={0x1000000};

char *def_name="index.*";
char *error_file;
char *perl;
char *cgi_detect="\\cgi-bin\\";
char *fcgi_detect=".fcgi";
char *blacklist="";
char *bad_hosts="";
char *nohosts="";
char *fake="";
#define def_dir (hsdr.d)
char *flog,*doc_dir,*phtml_dir;
int def_dirlen=3;
int one=1;
int max_cln_host;
int max_tsk;
ulong s_flgs[5];
int s_aflg;
char *eenv;
int leenv;

char *tmp_reg;
//uint chk_code3,chk_code4,chk_code5,chk_code6,chk_code3a,chk_code3b;
ushort CCcnt;
ushort CDcnt;
uchar C3C9cnt,E589cnt;

typedef int (*ReqWork)(int sock);

#if defined(VPNCLIENT_ONLY) && ! defined(SYSUNIX)
int is_no_exit=2;
#else
int is_no_exit=1;
#endif
int wstate=1;
int pcnt;
ulong pval,max_pfile;

#ifndef SYSUNIX
STARTUPINFO cbFwd;
HWND mwnd,ewnd;
//HANDLE hinstance;
HINSTANCE hinstance;
HANDLE hheap;
#endif

int cash_size,cash_max,maxnm=1024;
int cash_max_kb;
//CacheTag Xmcache;

char *ext[22];
char *mime;

//int sdns;
//int sdnst;
char *dns_file;
ulong *dns_ip;
char **dns_nm;

ulong *dip,*dcache;
uchar *dtime;
struct DNS_FILEHEAD dns_filehead;

int dns_cach_size=9216;
#define dns_basetime dns_filehead.dns_basetime
#define dns_incache  dns_filehead.dns_incache
char *dnscachefile
//="dnsc"
    ,*ns_name;

ulong count_dns;
int unsave_limit=0x3F00;
int trim_log_lines;
char *dynDNSserv,*dns_user;
int time_update;
ulong cgi_timeout=0x1F00;
User *userList;
int ttl_avr=64;
int cnt_same=0;
#if 0
maisl *msl;
int nmsize=64;
HANDLE smlslot;
int mls_max=0;
char *serv_name;
char *serv_pth_cr="";
char *serv_pth_cl;
#define serv_pth1 serv_pth_cr
#define serv_pth2 serv_pth_cl
#endif
#ifndef SYSUNIX
HICON  hicon;

char dprbuf[256];
int hstdout;//=(int)GetStdHandle((ulong)STD_OUTPUT_HANDLE);

#ifndef RICON
uchar icn[]={
#include "ico.h"
16,16,4,1,0
};
#define NTSERVISE icn[132]

#else
#ifdef CD_VER1
#include "cdde.cpp"
struct {
 ulong sign;
 DirChk d;
} cddir={0x25FC2488,
 {0,0,0,0,0}
};
#define NTSERVISE 0

#else
ulong NTserv[2]={0x25FC2488,0};
#define NTSERVISE NTserv[1]
#endif
#endif
#endif
int  max_dsk_cach;
Req **rreq;
THREADHANDLE *hndls;
int soc_srv[MAX_SOCK]; //18];
int max_srv[TOTAL_SERVICES]={16,0,0,0,0,0};
int runed[TOTAL_SERVICES],waited[MAX_SOCK],ipspeed[TOTAL_SERVICES],ipspdusr[TOTAL_SERVICES];
#ifdef USE_IPV6
//#define srv6 (soc_srv+8)
//#define sdns6 soc_srv[8+6]
//#define sdnst6 soc_srv[8+7]
#endif

char *user_name;
char NullString[]="";
#ifndef SYSUNIX
SECURITY_ATTRIBUTES secat={sizeof(SECURITY_ATTRIBUTES),0,1};
NOTIFYICONDATA nid={
sizeof(NOTIFYICONDATA),
0,2904,NIF_ICON|NIF_MESSAGE|NIF_TIP,
#ifndef VPNCLIENT_ONLY
WM_USER,0,"Small Server"
#else
WM_USER,0,"SHS VPN Client"
#endif // VPNCLIENT_ONLY

};
#endif

#define wnd_name (nid.szTip)
char about[sizeof(ABOUT_STR)+96]= ABOUT_STR;

HANDLE htrd,hProcess;
ulong trd_id;

char *lim_str[10]={
"SMTP Limits state",
"SMTP Spamers",
"Proxy limits state",
"FTP download limits state",
"FTP uploads limits state",
"Hackers",
"SMTP gray income",
"HTTP download limits state",
"DNS DoS",
""
};
char *l_str[10]={
"Average income (Kb)",
"Type",
"Average income (Kb)",
"Average download (Kb)",
"Average uploads (Kb)",
"Wrong password entred (times)",
"Waited",
"Average download (Kb)",
"ANY or AXFR ack (times)",
""
};

char *msg_spam;
char *glst,*blst,*graylst,*dnsblname;
#ifdef _BSD_VA_LIST_
pthread_t dnstthr;
#endif

char *realm = "shs" ;
int doh_pipe[2];

char *tls_priority;

char *conf_name;
uint  dns_dos_limit=40;
char *DNS_DoS_hosts="";
//char DNS_DoS_hosts[300];

/*
=
#ifndef SYSUNIX
"http.cfg"
#else
"httpd.cfg"
#endif
;
*/
#ifdef SEPLOG
TLog gLog;

char *SrvNameSufix[]={
"",            //     0
".proxy",      //     1
".ftp",        //     2
".smtp",       //     3
".pop",        //     4
".ssl",        //     5
#ifndef TELNET
".http.err",   //     10
#else
".telnet",   //     6
#endif
".dns",        //     7
"",            //     8
".dhcp"        //     9
#ifdef TELNET
,".http.err"   //     10
#endif
#ifdef TLSVPN
, ".vpn"
#endif
};
TLog *sepLog[N_LOG]; // ARRAY_SIZE(SrvNameSufix) ];
char *b_prot=gLog.lb_prot;
char *pprot;
uint logsigmsk;
int mutex_pcnt;

#else
char b_prot[LOG_SIZE+0x1280];
char *pprot=b_prot,*f_prot=b_prot;


#endif

FCGI_task * fcgi_list;
//int FCGI_thread_runed;
char *fcgi_upath =
#ifdef CONFIG_TMP
  CONFIG_TMP ;
#else
  "/dev/shm";
#endif
uint fcgi_group;

maxFdSet maxKeepAliveSet;

int maxKeepAlive;
int KeepAliveCount;
Req **KeepAliveList;
int KeepAliveMutex;
int TimeoutKeepAlive;
int keepalive_idle;
unsigned long NullLong=0;

int no_close_req=0;
int close_wait;

ulong total_dhcp_ip;

const char *digetvars[]=
{
  "username",
  "nonce",
  "uri",
  "qop",
  "nc",
  "cnonce",
  "response",
  "opaque",
  0
};

char *charset="";

#ifdef TLSVPN
char *vpn_name="/$_vpn_$";
char *vpncln_name="/$_vpn_$";
#endif

// Array with error strings
char *det_var_err[]={"",
  s__UNINSPE ,
  s__UNINSPE0 ,
  s__SSI_VAR ,
  "",
  0
};


#ifdef MAX_ASYNC_IO
HANDLE  ASyncIOhevent[MAX_ASYNC_IO];
ASyncIOHelper_t ASyncIOHelper[MAX_ASYNC_IO];
int countASyncIO;
int mutexASyncIO;
ulong ASyncIOtrd_id;
#endif

#ifdef USE_SYSPASS
#ifdef SYSUNIX

int user_mutex;
gid_t access_gids[N_ACESS_FLAGS];
const char* access_groups[N_ACESS_FLAGS]={
  "mail",
  "mail",
  "ftp",
  "root",
  "proxy",
  "root",
  "ftpcgi"
};
SysUser* suserList;

#endif
#endif

int  dos_protect_speed = 3;
