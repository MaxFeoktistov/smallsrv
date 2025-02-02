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


#ifndef STRING_CONST_H
#include "g4strc.h"
#endif
#ifndef SRV_H
#define SRV_H

#if __SIZEOF_INT__ == 4 &&  __SIZEOF_POINTER__ == 8

#ifndef A_64
#define A_64 1
#endif

#endif

#define NEWSTRUCT 1

void dbgf(char *er,int s);
#define dbg(er) dbgf(er,s)
#define dbgs(er) dbgf(er,s)
void xdie(char *);
#define die(e) {xdie(e); return -1;}

//#define DEBUG_VERSION 1

#ifdef DEBUG_VERSION

#define DBGLS(a)  debug("%s:%u:%s %s\r\n",__FILE__ , __LINE__, __func__, a);
#define DBGL(a)  debug("%s:%u:%s " a "\r\n",__FILE__ , __LINE__, __func__ );
#define DBGLA(a,b...) debug("%s:%u:%s " a "\r\n",__FILE__ , __LINE__, __func__, ## b );
//printf("%s:%u:%s " a "\r\n",__FILE__ , __LINE__, __func__, b );
#else

#define DBGLS(a)
#define DBGL(a)
#define DBGLA(a,b...)

#endif

#if __BYTE_ORDER__  ==  __ORDER_BIG_ENDIAN__
#define BIG_ENDIAN 1
#else
#undef BIG_ENDIAN
#endif

#include "config.h"

#ifndef SYSUNIX

#include "to_win.h"

#else
#include "to_linux.h"
#endif

extern "C"{

#include "mdef.h"
#include "hton.h"
#include "tlsdll.h"
#include "slib.h"
#include "regular.h"
#include "gz.h"
#include "mlist.h"
#include "dhhosts.h"
#include "bvprintf.h"
#include "fcgi.h"
#ifdef WITHMD5
#include "md5.h"
#endif
#ifdef MINGW
#undef fd_set
#define win_fd_set fd_set
#endif

struct LogInfo ;
struct MemList;

void debug(const char *a,...);
int InitDnsSrv();
DWORD SetDNSCall(void *);
struct StatLog;
typedef uint (*tfFind)(StatLog *psl,char *t);
struct Req;
typedef int (*tfSnd)(Req*,const void *,int);
typedef int (*tfRcv)(Req*,void *,int);
int JustSnd(Req *th,const void *b,int l);
int JustRcv(Req *th,void *b,int l);
int TLSSend(Req *th,const void *b,int l);
int TLSRecv(Req *th,void *b,int l);
int IsInIPRR(int,sockaddr_in *);

struct User;
struct host_dir;
struct WMail;

struct FTPSecCon;
struct FCGI_task;

#ifdef  x86_64
typedef unsigned long long arh_ulong;
#define ARH_MASK  0x3F
#define ARH_SHIFT 6
#define ARH_ONE   1ll
#else
typedef unsigned int arh_ulong;
#define ARH_MASK  0x1F
#define ARH_SHIFT 5
#define ARH_ONE   1
#endif


#ifdef USE_IPV6
#define  TSOCKADDR  sockaddr_in6
#else
#define  TSOCKADDR  sockaddr_in
#endif

typedef union{
#ifdef USE_IPV6
    sockaddr_in6 sa_c6;
#endif
    sockaddr_in sa_c;
    TSOCKADDR sa_c46;
} sockaddr46_t;

struct Req
{int s;
 tfSnd Snd;
 tfRcv Rcv;
 void *Adv;
 #ifdef USE_IPV6
 union{
    sockaddr_in6 sa_c6;
    sockaddr_in sa_c;
    TSOCKADDR sa_c46;
 };
 #else
 struct sockaddr_in sa_c;
 #define sa_c46 sa_c;
 #endif
 ulong tmout;
 time_t  KeepAliveExpired;

 union{
  uint fl;
  ushort flsrv[2];
 };
#define F_POST 1
#define F_KEEP_ALIVE  2
#define F_PHP 4
#define F_PASSWORD 4
#define F_GZ  0x8
//#define F_PERL 0x40
#define F_SKIPHEAD 0x10
#define F_EXE 0x20
#define F_UPCHUNKED 0x40
#define F_LASTLF 0x80
#define F_PRX 0x100
#define F_HTTP10 0x200
#define F_CHUNKED 0x400
#define F_DIGET_UNAVILABLE 0x800
#define F_STDERRSEL 0x1000
#define F_NAMEOUTED 0x2000
#define F_ERROUTED 0x4000
#define F_FCGI     0x8000

#define F_SERV_MASK 0xF0000

 #define F_JUSTPOOL  0x8000000


#define F_VPNTUN     0x1000000
#define F_VPNTAP     0x2000000
#define F_VPNANY     0x3000000

#define F_VPN_MACSET 0x20
#define F_VPN_IPSET  0x40
#define F_VPN_IP6SET  0x10
#define F_VPN_IP6SET2  0x80
#define F_VPN_LASTSINHERROR 0x200

#define F_SMTP_CL     0x4000000

#define MAX_SERV_MASK 0xF

 int  timout;
 char *loc, *rq;
 ulong freqcnt;

 //char *req,*pst,*trn,**http_var,**req_var,*dir;
 union{
  char *req;
  int  reqi;
 };
 union{
  char *pst;
  int  psti;
 };
 union{
  char *trn;
  int  trn_sock;
  Req  *next_doh;
 };
 char **http_var;
 union{
  char **req_var;
  int  req_vari;
 };
#ifdef SYSUNIX
  struct stat *fileStat;
#endif
 char *dir;
 int dirlen, ntsk, postsize;
#define dlNOW_SENDING_SIGN 0x12345678
 char *KeepAlive;
 u64 Tin,Tout;
 ulong bSpd;
 int   event;
 union {
   void *gz;
   int  pass_port;
 };
 union {
   host_dir *vhdir;
   User *a_user;
 };
 char inf[96];

#ifdef FIX_EXCEPT

 int thread_id;
 jmp_buf  jmp_env;

 #define  EXCEPT_TRY(th,x...)  setjmp( (th)->jmp_env ); if(!(th)->thread_id) {(th)->thread_id=GetCurrentThreadId();  }else{ CloseSocket((th)->s); x ; pthread_exit(0);  }
 #define  END_TRY(th)  (th)->thread_id=0;

#else
  #define  EXCEPT_TRY(th,x...)
  #define  END_TRY(th)
#endif


 int Send(const void *b,int l){return (Snd)(this,b,l);}
 int Recv(void *b,int l){return (Rcv)(this,b,l);}
 int JustSend(const void *b,int l){return JustSnd(this,b,l);}
 int JustRecv(void *b,int l){return JustRcv(this,b,l);}
 int SendChk(const char *b,int l);
 int ExecCGI();
 int SendSSI();
 int HttpReq();
 int TLSReq();
 int TReq();

#define  tbtAccept  0x2
#define  tbtAnon    0x1
#define  tbtVerfyRequired 0x4
#define  tbtDontVerfyTyme 0x8
#define  tbtDontVerfySigner 0x10
#define  tbtSSHstyleVerfy  0x20

 int TLSBegin(OpenSSLConnection *x, int type=tbtAccept, char *verfyhost = 0);
 //int TLSBeginClient(OpenSSLConnection *x, int anon);

 int IsInIPRange(int r){return IsInIPRR(r,&sa_c);};
 int RGetCMD(char *b);
 int HttpReturnError(char *err,int errcode=400);
 int MakeEnv(char *env, char *ee
#ifdef SYSUNIX
 ,char ***new_env
#endif
 );
 void prepare_HTTP_var();
 void prepare_Req_var();
 int ExecCGIEx();
 int ExecDllEx(char *e);
 int SendSSIEx();
 int ProxyReq();
 int SMTPReq();
 int POPReq();
 int FTPReq();
 int Admin();
 int SSLReq();
 char *CheckAuth(char * &p);
 void HTTPUserStat();
 int HTTPOutStatistics(char *bfr,char *log,int tbl,int dt);

 int HTTPFOutStatistics(char *bfr,char *log,int tbl,int dt);
 int HTTPIOutStatistics(char *bfr,char *log,int tbl,int dt);
 int HTTPLOutStatistics(char *bfr,LogInfo *l,int tbl,int dt);
 void OutBtbl(char *bfr,int prt,MemList *d,char *name);
 void OutStbl(char *bfr,int prt,MemList *d,char *name);
 void AddHack(ulong t,int v);

 void OutStatTblBody(StatLog *psl,tfFind fnd,//char *bfr
     BFILE *bf,
        const char *Name,int state_str=0);
 int OutActualConn(char *bfr);
 int OutVirusList(char *bfr);
 void HTTPOutCurentLog(char *);
 void HTTPAdminErrorMessage(char *msg,char *b)
 {Send(b,sprintf(b,"<hr><h1><br><i>Error: %s</i><br></h1><hr><br>",msg));}
 inline void PutFTPLine(char *ln,char *bfr);
 void OutDirTop();
 void OutDirBottom(char *bfr);
 int ChkValidPth(char *p,char *b);
 /*
 int HTTPOutCfg(char *bfr);
 int HTTPOutHosts(char *bfr);
 int HTTPUserAdd(char *bfr);
 void HTTPOutUsersPage(char *bfr){ HTTPUserAdd(bfr); HTTPOutHosts(bfr);}
 int HTTPMimeTypesOut(char *bfr);
 int HTTPCGIAppOut(char *bfr);
 */
 int HTTPOutCfg(BFILE *b);
 int HTTPOutHosts(BFILE *b);
 int HTTPUserAdd(BFILE *b);
 void HTTPOutUsersPage(BFILE *b){ HTTPUserAdd(b); HTTPOutHosts(b); b->fflush(); }
 int HTTPMimeTypesOut(BFILE *b);
 int HTTPCGIAppOut(BFILE *b);

 char* PutFTPDir(char *ln,char *bfr);
 void HTTPOutRegPage();
 void OutBaner(char *bfr);
 void p2p(int ss,char *in_buf);
 int CallUp(User *puser);
 int IsProxyRange(int a);
 int SleepSpeed();
 int IGZP();
 int IGZU();
 void GZEnd();
 void OutHackers();
 int OutSMTPLimit(char*,int);
 int ShowDHCP(char *b);
 int Wmail( User *usr=0);
 User * ChUser(int typ);
 void SendMsg(char *fromm, WMail *wm);
 int CheckDNSBL(char *t,struct sockaddr_in *sa);
 int SendDigestAuthReq(char *bfr);
 int CheckNonce(char *nonce,char *opaque);
 int IP2country();
 int ChkFTPSec(FTPSecCon *);
 void OutErr(char *t);

 int CallFCGI(char *name);
 int DoFCGI(FCGI_task *fcgi);
 int CheckEndChunked();
 int InsertVPNclient();

 int SendCMDsmtp(char *xxx,int lll);
 void Close();
 void Shutdown(){ shutdown(s, 2); } ;
 int ServerNumber(){return flsrv[1]&MAX_SERV_MASK; }
 int SendStr(char *b);
 int OutVPNLimit(char *bfr);

};


struct  maxFdSet
{
  int  max_fd;
  fd_set set;
  void Set(int fd);
  void Fix();
  void Clear(int fd);
};
extern maxFdSet maxKeepAliveSet;
#define keep_alive_max_fd maxKeepAliveSet.max_fd
#define KeepAliveSet maxKeepAliveSet.set

extern int maxKeepAlive;
extern Req **KeepAliveList;
extern int KeepAliveMutex;
extern int KeepAliveCount;
extern int TimeoutKeepAlive;
extern int keepalive_idle;


#ifdef USE_IPV6
int IsIPv6(sockaddr_in *sa);
uint IPv4addr(sockaddr_in *sa);
int CmpIP(TSOCKADDR *a, TSOCKADDR *b);
#else
#define IPv4addr(sa)  ((sa)->sin_addr. S_ADDR)
int inline CmpIP(TSOCKADDR *a, TSOCKADDR *b){return a->sin_addr. S_ADDR == b->sin_addr. S_ADDR; }
#endif
//int FndLimit(int lst,LimitCntr **ip, LimitCntr **net, sockaddr_in *sa );
int FndLimit(int lst,LimitBase **ip, LimitBase **net, sockaddr_in *sa );

#define FL_NOWINTYPES      0x2
#define FL_NOICON          0x10
#define FL_NOPROXY         0x20
#define FL_NOHTTP          0x40
#define FL_SSIHTM          0x80
#define FL_DNSMXA          0x100
#define FL_ANPROXY         0x200
#define FL_FORWARD         0x400
#define FL_FWDRUN          0x800
#define FL_DNSTCP          0x1000
#define FL_FTWOSPACE       0x2000
#define FL_NORUNHTM        0x4000
#define FL_HIDE            0x8000
#define F_PROXY            0x10000
#define F_FTP              0x20000
#define F_SMTP             0x30000
#define F_POP              0x40000
#define FL_NOBRKSMTP       0x80000
#define FL_NOSHARE         0x100000
#define FL_RADMIN          0x200000
#define FL_FULLLOG         0x400000
#define FL_LOGDAY          0x800000
#define FL_CFGUNSV         0x1000000
#define FL_SMTPANY         0x2000000
#define FL_IGNNOCH         0x4000000
#define FL_VHALIAS         0x8000000
#define FL_DNSUPLVL        0x10000000
#define FL_NODIR           0x20000000
#define FL_PRXUSER         0x40000000
#define FL_FTPVDIR         0x80000000u

#define AFL_NT      0x80000000u
#define AFL_ICO     1
#define AFL_HIDE    2
#define AFL_RESTART 4
#define AFL_EXIT    8
#define AFL_TLS     0x10
#define AFL_EX2     0x10

#define FL1_FTPSAME      0x1
#define FL1_SMTPTLS      0x2
#define FL1_FTPROXY      0x4
#define FL1_SMTPROXY     0x8
#define FL1_POPROXY      0x10
#define FL1_NOCHKBCK     0x20
#define FL1_SMTP_AVT     0x40
#define FL1_CRYPTPWD     0x80
#define FL1_ISAPI        0x100
#define FL1_ISAPIALL     0x200
#define FL1_BIGPROXY     0x400
#define FL1_LACPROXY     0x800

#define FL1_DNTSHOWHIDEN 0x1000
#define FL1_RUNSYSTEM    0x2000
#define FL1_DELPAS       0x4000
#define FL1_UPPRX        0x8000
#define FL1_GZ           0x10000
#define FL1_GZPRX        0x20000
#define FL1_ATLS         0x40000
#define FL1_NOLUS        0x80000
#define FL1_AUTOPTR      0x100000
#define FL1_CHKUSR       0x200000
#define FL1_NBRK         0x400000
#define FL1_BBPROXY      0x800000
#define FL1_DHCPDNS      0x1000000
#define FL1_DHCPLO       0x2000000
#define FL1_IPCNTR       0x4000000
#define FL1_2P           0x8000000
#define FL1_PRXHRD       0x10000000
#define FL1_CONFIRM      0x20000000
#define FL1_NOM          0x40000000
#define FL1_MHST         0x80000000u

#define FL2_CHKMX        0x100
#define FL2_NOMFTP       0x200
#define FL2_MX_GLIST     0x400
#define FL2_MLNOIP       0x800
#define FL2_DNSNO6       0x1000
#define FL2_WMAIL        0x2000
#define FL2_WMTLS       0x4000
#define FL2_UTF         0x8000

#define FL2_PAVHTML  0x10000
#define FL2_PAVALL  0x20000
#define FL2_WMUTF   0x40000
#define FL2_CHUNKED 0x80000
#define FL2_MD5PASS 0x100000
#define FL2_USEMD5D 0x200000
#define FL2_PARANOIDMD5 0x400000
#define FL2_IPCONTRY 0x800000
#define FL2_IPCNTRAUT 0x1000000
#define FL2_DOH 0x2000000
#define FL2_FTPTLS 0x4000000

#define FL2_NOERROUT    0x08000000
#define FL2_SEPARATELOG 0x10000000
#define FL2_DUBSTDERR   0x20000000
#define FL2_DEBUG_RE   0x40000000

#define FL3_FCGI_SI   0x00000001
#define FL3_FCGI_PHP  0x00000002
#define FL3_FTP_ALWPASS  0x00000004

#define FL3_VPN_TUN     0x00000008
#define FL3_VPN_TAP     0x00000010
#define FL3_VPN_CLIENT  0x00000020
#define FL3_TAP_CLIENT  0x00000040
#define FL3_VPN_PUBLIC  0x00000080
#define FL3_VPN_CHKTLS      0x00000100
#define FL3_VPN_TLSIGNTIME  0x00000200
#define FL3_VPN_TLSSSIGN    0x00000400
#define FL3_VPN_TLSSHSTYLE  0x00000800
#define FL3_VPN_SCRKEEP     0x00001000

#define FL3_SYS_USERS       0x00002000

#define FL3_SMTP_TLS        0x00004000
#define FL3_SMTP_CHKTLS     0x00008000
#define FL3_SMTP_TLSONLY    0x00010000
#define FL3_SMTP_AUTHSAME   0x00020000

#define FL3_VPN_ULIMIT      0x00040000
#define FL3_VPN_IPLIMIT     0x00080000


#define USE_TUN       (s_flgs[3] & FL3_VPN_TUN)
#define USE_TAP       (s_flgs[3] & FL3_VPN_TAP)
#define TAP_CLIENT    (s_flgs[3] & FL3_TAP_CLIENT)
//#define TAP_SEPARATE  (s_flgs[3] & FL3_VPN_CL_SEP)


//#define FL2_WMAILSENT   0x8000

#define MAX_VAR_SIZE 512

struct host_dir{
 host_dir *next;
 char *d;
#ifdef NEWSTRUCT
 char *h;
 int  flg;
#else
 char flg;
 char h[1];
#endif

} __attribute__ ((packed));

struct ntrd
{HANDLE htr;
 int ss,stt;
 ulong tm;
 int *sz;
};

struct xsrv
{int port,cnt;
 ntrd *nt;
};

extern const char FmtBasic[],FmtBasicC[],FmtShort[],FmtShortErr[],FmtShortR[],FmtShrt[],FmtShrtR[];
int InitSecDLL();
void AddToLog(char *t,int s,TSOCKADDR * psa,const char *fmt=FmtBasic,...);
int PrepCfg(char *fname);
void InitParam(char *cln);
typedef ulong WINAPI (*TskSrv)(void *);
int XRecv(int s1,char *b,ulong l,int f,int);

#define V_ACCESS        1
#define V_READ          2
#define V_WRITE         4
#define V_LIMITED       8
#define V_DIR           0x10
#define V_NOHTTP        0x20
#define V_NOFTP         0x80
#define V_RW            (V_READ|V_WRITE)
#define V_RWLIM         (V_LIMITED|V_READ|V_WRITE)

int IsInStrLst(char *pwd,char *t);
void PrintHelp();

#ifndef CD_VER

struct User
{User *next;
#define UserPOP3 1
#define UserSMTP 2
#define UserFTPR 4
#define UserFTPW 8
#define UserFTP 0xC
#define UserHTTP 0x10
#define UserADMIN 0x20
#define UserNOCGI 0x40
#define UserPARSED 0x80
#define UserSYSUSER 0x100
#define FindUserMD5digest 0x10000
#define FindUserMD5cram   0x20000

#ifdef NEWSTRUCT
 char *name;
 char *pwd,*ddr;
 int  state;

 char *pasw(){return pwd;};
 char *dir(char *ps){return ddr;};
 char *dir(){ return ddr;};
#else
 char state;
 char name[1];
 char *pasw(){return (char *) memchr(name,0,256)+1;};
 char *dir(char *ps);
 char *dir(){ return dir(pasw());};
#endif

 int Parse();
 int Parse(char *x);
 void MkDir();
 int Convert(char *x);
 int FlgString(char *bfr);
 int IsSysUser()
 {
#ifdef NEWSTRUCT
   return (state & UserSYSUSER);
#else
   return name[0] == 1;
#endif
 }

}PACKED ;

#ifdef SYSUNIX
struct SysUser: public User
{
#ifdef NEWSTRUCT
#else
 char *pwd,*ddr;

 char *pasw(){return pwd;};
 char *dir(char *ps){return ddr;};
 char *dir(){ return ddr;};
#endif
 char nname[1];

 //char *name() {return nname;}
 int IsPwd(char *pas);

};

extern int user_mutex;
#define N_ACESS_FLAGS 7
extern gid_t access_gids[N_ACESS_FLAGS];
extern const char* access_groups[N_ACESS_FLAGS];
extern SysUser* suserList;
void InitAccessGids();
SysUser* AllocSysUser(struct passwd* pEntry, char *pas);
SysUser* CheckSysPassword( const char* user, const char* password );

#else
struct SysUser;
#endif

#endif

extern const char *digetvars[];
#define digtVar_username  0
#define digtVar_nonce     1
#define digtVar_uri       2
#define digtVar_qop     3
#define digtVar_nc      4
#define digtVar_cnonce  5
#define digtVar_response 6
#define digtVar_opaque   7

#define  DIGT_MIN_REQUIRED (\
          (1<<digtVar_username)  | \
          (1<<digtVar_nonce   )  | \
          (1<<digtVar_uri     )  | \
          (1<<digtVar_response)  )


struct CfgParam;
typedef int (*onCfgChange_t)(CfgParam *th);
typedef int (*onCfgToStr_t)(CfgParam *th, char *bfr);
struct CfgParam
{
 char *name;
 uint min,max,*v;
 char *desc,*adv;
 void *vv;
 onCfgChange_t fChange;
 onCfgToStr_t  fToString;
 char *comment;

 int HTMLFormString(char *bfr);
 int TextCfgString(char *bfr);
 int IsR();
};

int onChangeM2b64(CfgParam *th);
int onCfgToStrExt(CfgParam *th, char *bfr);
int onCfgToStrVHost(CfgParam *th, char *bfr);
int onCfgToStrUser(CfgParam *th, char *bfr);
int onCfgChangeUser(CfgParam *th);
int onCfgChangeVHost(CfgParam *th);
int onCfgChangeDisable(CfgParam *th);
int onCfgChangeFlag(CfgParam *th);
int onCfgChangeNoFlag(CfgParam *th);
int onCfgChangeExt(CfgParam *th);
int onCfgToStrExt(CfgParam *th, char *bfr);





int MyLock(volatile int &x);
#ifdef USE_FUTEX
void MyUnlock(int &x);
#else
inline void MyUnlock(int &x){x=0;}
#endif
void MyUnlockOwn(volatile int &x);

extern int FTPTimeout,POPTimeout,PRXTimeout;
int GetCMD(int s,char *b,int timo=POPTimeout);
char *SaveCfgFile(char *b);
void SaveCfgIfNeed();

int HttpReturnError(int s,char *err);
int InitApplication();
void CreatCfgWindow();
void OkCfgWindow();
void RestartServer(char *u,int cnt);
void Restart();
void CheckValidCGIIdent();
ulong ConvertIP(char * &t);
#undef inet_addr
#define inet_addr ConvertIP
void CheckValidCGIIdent();
void InitParam(char *cln);
int PrepCfg(char *fname);
void SaveConfigFile(char *bfr,char *fnm);
char *SaveCfgFile(char *b);
int CreateSrv(int);
void CloseServer();
void CloseService();
void StopSocket();

int CrThreadFunc(TskSrv Fnc, Req *par);
int CrThread(uint fnc);
int FreeThreads();
void AddKeepAlive(Req* preq);
int TryToAddKeepAlive(Req *req);
void RemoveKeepAlive(int i);
int WINAPI KeepAliveWike(Req *preq);
void FixMaxKeepAliveFD();
void DeleteKeepAlive(Req* preq);
// Function return -1, if removed anything, othervise index of oldest
int RemoveExpired();
void RemoveAndDelKeepAlive(int i);
void SetKeepAliveSock(int s);
int WINAPI KeepAliveThread(void *);
char* IsSame(char *tt,char *pp);


#ifndef CD_VER

struct CheckFileTime
{ulong CheckProxyTime;
 ulong time;
 char ext[4];
 char *dir;
 void CheckProxy();
 void CheckDir(char *bfr,char *pdir,long d);
 void CheckDDir(char *bfr,char *pdir,long d);
};
extern CheckFileTime proxy_chk;
#define proxy_dir (proxy_chk.dir)
#define proxy_time (proxy_chk.time)
#define CheckProxyTime proxy_chk.CheckProxyTime
extern CheckFileTime smtp_chk;

#endif
extern CfgParam ConfigParams[];
int anychars(char *name);

extern char *tmp_reg,*cmdline;
extern uint chk_code3,chk_code4,chk_code5,chk_code6,chk_code3a,chk_code3b;
extern ushort CCcnt,CDcnt;
extern uchar C3C9cnt,E589cnt;

extern host_dir hsdr;
#define  LOG_SIZE 0x8000
extern char *smtp_name,*dns_server_for_mail,*out_path,*err_path
 ,*def_name,*error_file,*perl,*cgi_detect,*blacklist,*bad_hosts,*flog,*doc_dir,*phtml_dir,*nohosts,*fake,
  end,*last_cfg,*eenv,*dnsblname;
extern char *fcgi_detect;
extern char *srv_str[];
extern ulong max_msg_size,last_file,max_pfile,tmSpd;
#define sent_path (smtp_chk.dir)

inline void Free_if_heap(char *a){if(a>&end)delete a;}
inline void Free_if_heap2(char *a){if(a>last_cfg && a>&end)delete a;}
#define FREE_IF_HEAP(a) Free_if_heap((char *)(a))
#define FREE_IF_HEAP2(a) Free_if_heap2((char *)(a))

extern THREADHANDLE HSmtpSendThrd;
#define def_dir (hsdr.d)
extern int def_dirlen,one,zero,max_cln_host,max_tsk,s_aflg,
 is_no_exit,wstate,pcnt,cash_size,cash_max,maxnm,cash_max_kb,leenv,CurPrxCnt,
 max_clndns,dns_cach_size,ttl_avr,cnt_same,unsave_limit,time_update,iip_cach,
 ip_cach_mtx,dns_s,addr_dns,up_proxy_port,max_cont_st,SMTPCounter,
 count_of_tr,post_limit,DnsTms,time_btw,proxy_antivirus_port,trim_log_lines;

#define s_flg s_flgs[0]
#define s_flg1 s_flgs[1]
typedef int (*ReqWork)(int s);
extern char *ext[22],*mime,*dns_file,**dns_nm,*primary_dns,*secondary_dns,
 *dnscachefile,*ns_name,*up_user,*ftp_upload,*up_proxy,*dynDNSserv,*dns_user,
 *antiv,*antispam,*smtproxy,*spamfltr,*dnsbl,*proxy_antivirus;

extern char *realm;
extern char *charset;
extern char *vpn_name;
extern char *vpncln_name;

extern uint limitPeriods[3];
extern uint VPNInLimitMb[3];
extern uint VPNOutLimitMb[3];
extern u64 VPNInLimit[3];
extern u64 VPNOutLimit[3];


extern ulong  s_flgs[5],count_dns,cgi_timeout;
extern u32 ip_cach[];
extern User *userList;
int IsPwd(ulong a,ulong b, char *pas);
void GenAPOP_dgst(char *pas,char *dgst,char *s,int ssize);
int IsPwdAPOP(char *pas,char *dgst,char *s,int ssize);
void CalkPwdMD5D(char **dgv, uint *HA1,char *method, char *HA2Hex);
//void CalkHA1(char *u,char *pwd, uchar *HA1);
void ConvPwdMD5L4(uint *t4,char *u,char *pas, char *realmm=realm);
char* ConvPwdMD5(char *t,char *u,char *pas);

int FillMD5Pwd(char *t, uint *dgt);
int IsPwdMD5DD(char **dgv, char *u,char *pwd,char *method);
int IsPwdMD5D(char **dgv, uint *HA1,char *method);
int IsPwdCRAM(char *pas,char *dgst,char *s,int ssize);
void GenCRAM_dgst(char *pas,char *dgst,char *s,int ssize);
void CopyXor64(uchar *t, const char *s, uint pad);
int IsPwdMD5C(char *p, char *pas,char *u);
void CvtHex(uint  *cd, char * Hex    );
void ConvPwdMD5L4(uint *t4,char *u,char *pas, char *realmm);
void MD5UpdateL(MD5_CTX *c, char *u);

int Pass2Txt(char *pbfr, char *t);
void UpdPwdCrypt(char *p);

extern int dos_protect_speed;

ulong DTick(ulong tick1, ulong tick2);
ulong Rnd();

void ShowProt();

char **CGIListNext(char **a,char *bfr);
char *MimeListNext(char *a,char *bfr);
User *UserListNext(User *tuser,char *bfr);
host_dir *HostListNext(host_dir *a,char *bfr);

int IsUsHost(char *t);
User *FindUser(char *bfr,int typ,char *pwd=0,Req *r=0);
ulong GetMsgName(SYSTEMTIME &stime);
void AddSendMessage(int i);
int LogAn(char *ls,int c);
void DelSpace(char *s);
void DelSpaceRe(char *s);
int IsSeq(char *p);
int SecConnectAbcent(OpenSSLConnection *s, int anon, char *);

int call_socket(char *hostname, int portnum);
int call_socket2(char *hostname, int portnum);
ulong MkName(char *url,int brk=' ');
#ifndef CD_VER
char* GetMailHost(char *adr,d_msg *dmm,int jchk=0);
uchar* GetNextMH(uchar *beg);
#else
#define OutBaner()
extern char *phtml_ini;
#endif

extern uchar icn[];
#define MAX_ADAPT  5
#define MAX_SERV   10

#define MAX_SOCK  (MAX_ADAPT*MAX_SERV+12)

#define TOTAL_SERVICES 7
extern int max_dsk_cach,first_pass_port,
 soc_port[TOTAL_SERVICES],soc_srv[MAX_SOCK],max_srv[TOTAL_SERVICES],runed[TOTAL_SERVICES],waited[MAX_SOCK],ipspeed[TOTAL_SERVICES],ipspdusr[TOTAL_SERVICES];
extern Req **rreq;
extern THREADHANDLE *hndls;
extern char  *bind_a[MAX_SERV];
#define SMTP_N 3
#define max_cln    (max_srv[0])
#define max_prx    (max_srv[1])
#define max_ftp    (max_srv[2])
#define max_smtp   (max_srv[3])
#define max_pop    (max_srv[4])
#define max_ssl    (max_srv[5])
#define max_tel    (max_srv[6])
#define sd         (soc_srv[0])
#define sdp        (soc_srv[1])
#define sdftp      (soc_srv[2])
#define sdsmtp     (soc_srv[3])
#define sdpop      (soc_srv[4])
#define sdssl      (soc_srv[5])
#define sdns       (soc_srv[7])
#define sdnst      (soc_srv[8])
#define dhcpd_s    (soc_srv[9])
#define dhcpd_so   (soc_srv[9+MAX_SERV])

#define SDNS_IND 7
#define SDHCP_IND 9

#define http_port  (soc_port[0])
#define proxy_port (soc_port[1])
#define ftp_port   (soc_port[2])
#define smtp_port  (soc_port[3])
#define pop_port   (soc_port[4])
#define ssl_port   (soc_port[5])
#define tel_port   (soc_port[6])

#define  SRV_HTTP      0
#define  SRV_PROXY     1
#define  SRV_FTP       2
#define  SRV_SMTP      3
#define  SRV_POP       4
#define  SRV_SSL       5
#define  SRV_TEL       6
#define  SRV_SDNS      7
#define  SRV_SDNST     8
#define  SRV_DHCPD     9

#define  F_SERV_HTTP      (SRV_HTTP  <<16)
#define  F_SERV_PROXY     (SRV_PROXY <<16)
#define  F_SERV_FTP       (SRV_FTP   <<16)
#define  F_SERV_SMTP      (SRV_SMTP  <<16)
#define  F_SERV_POP       (SRV_POP   <<16)
#define  F_SERV_SSL       (SRV_SSL   <<16)
#define  F_SERV_TEL       (SRV_TEL   <<16)
#define  F_SERV_SDNS      (SRV_SDNS  <<16)
#define  F_SERV_SDNST     (SRV_SDNST <<16)
#define  F_SERV_DHCPD     (SRV_DHCPD <<16)

#define   SRV_HTTP_MSK      (1<<SRV_HTTP )
#define   SRV_PROXY_MSK      (1<<SRV_PROXY)
#define   SRV_FTP_MSK      (1<<SRV_FTP  )
#define   SRV_SMTP_MSK      (1<<SRV_SMTP )
#define   SRV_POP_MSK      (1<<SRV_POP  )
#define   SRV_SSL_MSK      (1<<SRV_SSL  )
#define   SRV_TEL_MSK      (1<<SRV_TEL  )
#define   SRV_SDNS_MSK      (1<<SRV_SDNS )
#define   SRV_SDNST_MSK      (1<<SRV_SDNST)
#define   SRV_DHCPD_MSK      (1<<SRV_DHCPD)

#define  CONID_KEEPALIVE_MASK 0x100000
#define  CONID_VPN_MASK 0x200000
#define  CONID_VPNCL_VALUE 0x2FFFFF

extern char *user_name,NullString[],about[],*wkday[],*month[],
 *CApath,*CAfile,*s_cert_file,*s_key_file,*TLSLibrary,*loc_sent,*loc_draft,*loc_trash,*tls_priority;
extern HANDLE htrd;
extern ulong trd_id;
extern int iofs,no_close_req,MutexEr;
extern int close_wait;
extern CfgParam ConfigParams[];
extern CfgParam ConfigParams2[];
extern short MnuOffset[20];
extern const ulong *FTPcmd;
extern const char HTMLDirBody2[];
extern ulong lastbadip[24],nextbadip;
extern unsigned long NullLong;
char* DecodeName(char *tt,char *s,char *dm);
char* StrVar(char *p,char *n);
char* Encode64(char *t,char *s,int cnt);
char* Decode64(char *t, char *s, int max_size);
int split(char *src, char *separators, char **result, int max_result);

//#define HTML_LN "\n"
#define HTML_LN
char* CheckBadName(char *in_buf);
int IsCGI(char *bb,int j=5);
char *GetVar(char **varlist,char *var);
char *GetVarS(char **varlist,char *var);
char *PrFinVar(char *s,const char *v);
int GetIndVar(char **varlist,char *v);
int  CountOfVar(char **varlist);

#define HTMLOutBottom OutBaner
int IsInIPRange(int lst,ulong ip);
StatLog *ParseFile(char *log,char *b=0);
extern ulong *Range[MAX_SERV*2];
extern char *Rangec[MAX_SERV*2],*header,*msg_spam,*ipbsfile,**country_names;
#ifdef USE_IPV6
extern char *Rangec6[MAX_SERV*2];
extern in6_addr *Range6[MAX_SERV*2];

#endif
extern char *det_var_err[];

char* IPv6Addr(ushort *t,char *s);
int IP2S(char *addr6,sockaddr_in* xsa);
int IPv6S(char *addr6,in6_addr &sin6_addr);


#define http_range  0
#define proxy_range 2
#define ftp_range   4
#define smtp_range  6
#define pop_range   8
#define ssl_range   10
#define ip_range    12
#define dns_range   14
#define adm_range   16
#define tel_range   18
//extern const char AuthErr[sizeof(AUTH_ERR)];
#define AuthErr AUTH_ERR

char *NextIf(char *t,char *f,char *f1="<!--#if",char *f2="<!--#endif ");

#define MAX_HTTP_VARS 60
#define DEBUGVAR(a)
void SRegHTM();
char* ConvPwd(char *t,char *pas);

extern char sprt80[6],sprt443[6];
#define NS_HSH_MAX 0x2000
#define NS_HSH_MAX_MX 0x800
#define NS_HSH_MAX_TXT 0x4000
#define NS_HSH_SHIFT 0x4800

struct NS_hash
{ulong sign;
 ulong hshbtime;
 ulong cn,cns,cmx,cptr,ctxt;
 ulong n[NS_HSH_MAX],ns[NS_HSH_MAX],mx[NS_HSH_MAX_MX],a[NS_HSH_MAX],
  nsa[NS_HSH_MAX],mxa[NS_HSH_MAX_MX],ptr[NS_HSH_MAX_MX],ptrh[NS_HSH_MAX_MX];
 //ushort
  ulong
     ttl[NS_HSH_MAX],nsttl[NS_HSH_MAX],mxttl[NS_HSH_MAX_MX];
 char txt[NS_HSH_MAX_TXT+64];
 void AddA(ulong xhst,ulong IP,ulong tim,ulong shift,ulong &ccc, ulong max);
 void UpdateBTime();
 void AddToCashPtr(ulong xhst,ulong tim,char *t,uchar typ,uchar l);
 int FindDNSinPTR(ulong xhst,int typ, int first=0);
};

extern NS_hash *pns_hash;
int GZSnd(Req *th,const void *b,int l);
int GZSndSkip(Req *th,uchar *b,int l);
int GZRcv(Req *th,void *b,int l);
int DelStr(char *s,char *b,int l);
extern struct linger lngr;
extern char *lim_str[10],*l_str[10],*glst,*blst,*graylst,*dhcp_bcast,*dhcp_addr;
int BSend(int s,char *b,int l);
void LoadDomainM();
void CloseSocket(int);
char* OutLL(long x0, long x1,char *t);


void AddToLogDNS(const char *t,int n,TSOCKADDR *sa,char *ad="");
int FNCATRIBUTE LZZUnpk(uchar *t,uchar *s0);
int LoadContries();
char *GetCntr(ulong ip);
extern int doh_pipe[2];
#define doh_r doh_pipe[0]
#define doh_w doh_pipe[1]
void DelDOHReq(Req *rq);

extern char *anFnd[];
//------------------------------

struct MHinfo
{
 uchar beg[512];
 uchar name[68];
 char sip[20];
 char  *ta[8];
 uint   cur_hst;
 uint   cur_ip;
 uint   nhst_hst;
 uchar *FirstIP;
 uchar *LastIP;
 ulong  lIP;

 void InitHst(uchar *t1);
 uchar * GetNextMH(char *t);
 int  CheckIP(ulong ip);
};

inline int CheckMHIP(uchar *beg,ulong ip) { return  ((MHinfo *)beg)->CheckIP(ip);}
inline uchar * GetNextMH(uchar *beg) //,uchar *t1,char *t2)
{ return  ((MHinfo *)beg)->GetNextMH((char *)beg+512);}

#define LINESIZE 2048
struct FileLine
{int h,l;
 char *p;
 char b[LINESIZE+1];
 int  Open(char *n){ p=b; l=0; return h=_lopen(n,0); }
 void Close(){ _lclose(h); }
 int  ReadLine();
};

char * GetNextBindAddr(char *p, void *sa_server);
char* html2text(char *t, char *s);
char *SkipSpace(char *a);
char *strchr_meta(char *p,int c);
extern char *tel_cmd;

int LoadLangCfg(char *fname);



#define CRLF  "\r\n"
#ifdef SYSUNIX
#define LF  "\n"
#else
#define LF  "\r\n"
#endif

struct CntrCode{
 char nm[2];
 ushort ind;
 uint  cnt;
 uint  ip[1];

 CntrCode *Next(){ return (CntrCode *) (ip+cnt) ;}
 ulong FastFind(ulong s);


} PACKED;

extern CntrCode  *cntr_dat,*cntr_dat_last;
CntrCode  * FindCntr(ulong ip);

extern char *lang_data;
extern char *conf_name;

int ChkWaitBind();

extern const uchar isCountAr[];
extern char NSTypes[];
extern char *NSTypesTXT[];
extern char *DNS_DoS_hosts;
extern int   pid_to_wait;

void dec_no_close_req();

#ifdef USEVALIST
#define mva_list va_list
#else
#undef  mva_list
#define mva_list void **
#endif

#define  HTTP_HEAD_BEGIN "HTTP/1.1 200 Ok\r\nConnection: close\r\n"
extern const char ChunkedHead[];
extern const int ChunkedHeadSize;

#ifdef SEPLOG

struct TLog
{
  const char *suffix;
  char *lpprot,*lf_prot,*loldprot;
  int  lpcnt;
  int  msk,llastday;
  char lb_prot[LOG_SIZE];
  char aabfr[0x1000];

  void RelProt(SYSTEMTIME *stime);
  void RelProt();
  void LAddToLog(char *t,int s, TSOCKADDR *psa, const char *fmt=FmtBasic,...);
  void LAddToLogDNS(const char *t,int n,TSOCKADDR  *sa,char *ad="");
  void MkFileName(char *t,int n);
  void Init(int n);
  //{lpprot=lf_prot=lb_prot; suffix=s ;  lb_prot[0]=0; lpcnt=0; msk=1<<n;};
  void GetProt();
void ShowProt();
void Ldebug(const char *a,...);
void Lvdebug(const char *a,  mva_list v);


};

extern TLog gLog;

#ifdef TLSVPN
#define VPN_N_LOG 1
#else
#define VPN_N_LOG 0
#endif

#ifdef TELNET
#define TELNET_N_LOG  1
#define CGI_ERR_LOG 10
#else
#define TELNET_N_LOG  0
#define CGI_ERR_LOG 6
#endif

#define N_LOG  (10 + TELNET_N_LOG + VPN_N_LOG)
#define VPN_LOG (10 + TELNET_N_LOG)

extern TLog *sepLog[N_LOG];
extern TLog  *shown_log;

extern int oldchecked,mutex_pcnt;
extern uint logsigmsk;

void InitSepLog();
void PreInitSepLog();
void DoneSepLog();
void UDoneSepLog();
extern "C" void edebug(const char *a,...);
extern "C" void tlsdebug(const char *a,...);


TLog *GetLogS(int s);
TLog *GetLogR(Req *r,int def=0);
//TLog *GetLogN(int n){ return (s_flgs2[2]&FL2_SEPARATELOG)?gLog:gLog[n]  };
//inline TLog *GetLogN(int n){ return sepLog[n];  };


#define debug(a...)  gLog.Ldebug(a)
#define AddToLog(a...)  gLog.LAddToLog(a)
#define AddToLogDNS(a...)  gLog.LAddToLogDNS(a)
extern char *b_prot,*pprot;
extern char *SrvNameSufix[];
#undef dbg
#define  dbg(t) OutErr(t)


#else

#define  InitSepLog()
#define  DoneSepLog()  RelProt()
#define  UDoneSepLog()  RelProt()
extern char b_prot[],*pprot,*f_prot;

inline void GetProt(){MyLock(pcnt);
#ifdef SYSUNIX
 oldprot=pprot;
#endif
};


#endif

struct FCGI_req
{
  int s;
  Req *req;
  int state;
#define fcgistCONNECTED   1
#define fcgistCONT_RECV   2
#define fcgistWAIT_RECV   4
#define fcgistRECV_READY  8
#define fcgistPART_HEAD   0x10
#define fcgistPART_BODY   0x20
#define fcgistSKIP_PADD   0x40
#define fcgistHDR_SEND    0x80

  int  recv_len_left;
  int  padd;
  /*
  union {
    FCGI_Header last_header;
    char bfr[1];
  };
  */
  int FCGIDoInput(char *bfr);
  int AppWrite(int type, int l, const void *data);
//  int AppRead(int max_l, void *data);
  int Param(const char *name, const char *val);

};

struct FCGI_task
{
  FCGI_task *next;
  char app_name[256];
  int  listen_fd;
  //int  fdw, fdr;
  //int phrd[2],phrdp[2],phrde[2]
  int child_pid;
#ifndef SYSUNIX
  PROCESS_INFORMATION pi;
#endif
//  uint state;


  int  lock;
  int  r_lock;
  int  n_wait;
//  int  last_id;
  int  sockaddr_len;
#ifdef SYSUNIX
  union {
#endif
    struct sockaddr_in sa;
#ifdef SYSUNIX
    struct sockaddr_un sau;
  };
#endif
#ifdef FCGI_THREAD

 // arh_ulong reqs[1];
  FCGI_req reqs[1];
#endif

  //Req  *rwait[1];
  //fd_set connected_socket;

  int  Open(char *name);
  void Close();
//  int  AppWrite(int idx, int type, int l, const void *data);
//  int  AppRead(int idx, int max_l, void *data);

//  void AddRequest(Req *req);
//  void Run(Req *req);

  int  CheckLive();
  int  Connect(FCGI_req *f_req);
//  void EndAllReq();

};
int FCGI_thread(void *);
void CloseFCGI_tasks();


extern FCGI_task * fcgi_list;
//extern int FCGI_thread_runed;
extern char *fcgi_upath;
extern uint fcgi_group;
#define FCGI_MODE_BIT  (1<<15)


ulong AddrHash(sockaddr_in *sa);

int SrvEventWait(int *event, uint timeout);
inline void SendEvent(int &event, int val)
{
  event = val;
#ifdef USE_FUTEX
  futex((int *)&event,FUTEX_WAKE,1,0,0,0);
#endif
};

#define STRUCT_OFFSET(s, field) ((unsigned long) (void *) & (((s *)0)->field))
void* RemoveFromList(void **list, void *object, int next_offset);

};  // extern "C"

#ifndef SEPLOG

void RelProt(SYSTEMTIME *stime);
void RelProt();
#define tlsdebug debug

#endif

char* CopyBB(char *y,char *t);

extern uint  dns_dos_limit;

#define CS(a) a
#define CXS(a,b) a

#endif
