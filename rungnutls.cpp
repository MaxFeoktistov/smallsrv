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



#ifdef  MINGW
extern "C" {

int DllMainCRTStartup(
   void* hinstDLL,
   long     fdwReason,
   void *   lpvReserved
)
{

    return 1;
};

}
#include <sys/types.h>
//#include <ctype.h>
#define inline  _cdecl
#define MINGWDLL
//#include "mstring1.h"
#include "mstring1.cpp"
#undef inline
void *  memmove(void *_s1, const void *_s2, size_t _n)
{
 if(_n)
   if(_s1>_s2) memcpy(_s1,_s2,_n);
   else if(_s1<_s2)memcpy_back( ((char *)_s1)+_n,((char *)_s2)+_n,_n);

 return _s1;
}

#define __STRALIGN_H_ 1
#include <windows.h>
#ifdef  DJGPP
extern "C" {
int PASCAL __WSAFDIsSet(int, fd_set *);
};
#endif

#undef FD_ZERO
#undef FD_SET
#undef FD_CLR
#undef FD_ISSET
#define FD_ZERO(set) ((set)->fd_count=0)
#define FD_SET(fd,set) ((set)->fd_array[(set)->fd_count++]=fd)
#define FD_ISSET(fd, set) __WSAFDIsSet((fd),(set))

#undef select

#else

#include "mstring1.h"

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#endif
#define GTLS


#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

//#include <stdio.h>

#ifdef USE_POOL
#include <poll.h>
#endif



#include "mdef.h"




//#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wwrite-strings"


#define  tbtAccept  0x2
#define  tbtAnon    0x1
#define  tbtVerfyRequired 0x4
#define  tbtDontVerfyTyme 0x8
#define  tbtDontVerfySigner 0x10
#define  tbtSSHstyleVerfy  0x20

#define DBGLS(a)  Fprintf("%s:%u:%s %s\r\n",__FILE__ , __LINE__, __func__, a);
#define DBGL(a)   Fprintf("%s:%u:%s " a "\r\n",__FILE__ , __LINE__, __func__ );
#define DBGLA(a,b...) Fprintf("%s:%u:%s " a "\r\n",__FILE__ , __LINE__, __func__, b );


#ifdef __cplusplus
extern "C"{
#endif


#ifdef MINGW
static void *id_heap;
//inline
void * malloc(size_t n){return HeapAlloc(id_heap,HEAP_ZERO_MEMORY,n);}
//inline
void * realloc(void *p,size_t n){return  HeapReAlloc(id_heap,HEAP_ZERO_MEMORY,p,n); }
//inline
void   free(void *p) { HeapFree(id_heap,0,p); }

//#define malloc WMALLOC
//#define free WFREE

static int  hstdout;
//=(int)GetStdHandle((ulong)STD_OUTPUT_HANDLE);
//static char ddbg[4512];

//#define  DBG_STEP() _hwrite(hstdout,ddbg,wsprintf(ddbg,"SSLDBG:%u\r\n",__LINE__ )); MessageBox(0,ddbg,"DBG",MB_OK);



typedef void (*tDebugFnc)(char *f,int l);
typedef void (_cdecl *tDebugPrintFnc)(char *f,...);

void  SetDfnc(tDebugFnc f,tDebugPrintFnc fp);

static void  DebugFnc(char *f,int l)
{
 // //_hwrite(hstdout,ddbg,wsprintf(ddbg,"SSLDBG:%.50s,%u\r\n",f,l ));
 // l=wsprintf(ddbg,"SSLDBG:%.50s,%u\r\n",(f)?f:"??",l );
 // _hwrite(hstdout,ddbg,l);
};
static int mem_count;
void  MDebugFnc(const char*t,const char *f,int l,ulong p,int n)
{
 //  l=wsprintf(ddbg,"SSLDBG:%s,%.50s,%u %X=%u  %u\r\n",t,(f)?f:"??",l,p,n,mem_count );
 // _hwrite(hstdout,ddbg,l);
};
#define MDebugFnc(a...)
void _cdecl DebugPrintFnc(char *a,...) ;

 //DBG_printf

#define  DBG_STEP()
//printf("tls:%u\n",__LINE__);
//DebugFnc("runssl",__LINE__);

// int CRYPTO_set_mem_ex_functions(void *(*m) (size_t, const char *, int),
//                                 void *(*r) (void *, size_t, const char *,
//                                             int), void (*f) (void *))


struct dbg_mem
{
 int check;
 const char *f;
 int l;
 int n;
};
void * dbg_malloc(size_t n,const char *f,int l){
    union{
     char *r;
     dbg_mem *d;
     ulong rl;
    };

    r=(char *)HeapAlloc(id_heap,HEAP_ZERO_MEMORY,n+sizeof(dbg_mem)+4);
    if(r)
    {
      mem_count+=n;
      MDebugFnc("malloc",f,l,rl,n);


      d->check=0x5A4B8F12;
      d->f=f;
      d->l=l;
      d->n=n;
      DWORD_PTR(r[n+sizeof(dbg_mem)])=0x5A4B7F12;

    }
    else
    {
      MDebugFnc("malloc FAIL!",f,l,0,n);

    }
    return r+sizeof(dbg_mem);

}
//inline
void * dbg_realloc(void *p,size_t n,const char *f,int l){
    union{
     char *r;
     dbg_mem *d;
     ulong rl;
    };
    int on;
    r=(char *)p;
    r-=sizeof(dbg_mem);
    if(d->check == 0x5A4B8F12)
    {
      MDebugFnc( (DWORD_PTR(r[d->n+sizeof(dbg_mem)])==0x5A4B7F12) ? "realloc1":"realloc check FAIL!",d->f,d->l,rl,d->n);
      on=d->n;

    }
    else
      MDebugFnc("realloc bad p",f,l,rl,n);

//    if(d->n > n)return r;

    r=(char *) HeapReAlloc(id_heap,HEAP_ZERO_MEMORY,r,n+sizeof(dbg_mem)+4);
    if(r)
    {
      mem_count+=n-on;
      MDebugFnc("realloc2",f,l,rl,n);

      d->check=0x5A4B8F12;
      d->f=f;
      d->l=l;
      d->n=n;
      DWORD_PTR(r[n+sizeof(dbg_mem)])=0x5A4B7F12;

      return r+sizeof(dbg_mem);
    }
    else
    {
      MDebugFnc("realloc FAIL!",f,l,(ulong)p,n);

    }
    return r;

}
//inline
void   dbg_free(void *p
//  ,const char *f,int l
) {
    union{
     char *r;
     dbg_mem *d;
     ulong rl;
    };
    if(!p)
    {
      return ;
    }
    r=(char *)p;
    r-=sizeof(dbg_mem);
    if(d->check == 0x5A4B8F12)
    {
      mem_count-=d->n;
      d->check = 0;
      MDebugFnc((DWORD_PTR(r[d->n+sizeof(dbg_mem)])==0x5A4B7F12)?"free" : "free check FAIL!",d->f,d->l,rl,d->n);
    }
    else
      MDebugFnc("free bad p","",0,rl,0);


    HeapFree(id_heap,0,r);

}

/*
void _cdecl DebugPrintFnc(char *a,...)
{
     int l;
     va_list v;
     va_start(v,a);
 //    _hwrite(hstdout,ddbg,wvsprintf(ddbg,a,(char *)(&a+1) ));
 //    _hwrite(hstdout,ddbg,wvsprintf(ddbg,a,v ));
    l=wvsprintf(ddbg,a,v);
    _hwrite(hstdout,ddbg,l);
    va_end(v);
}
// */
#define DebugPrintFnc(a...)

#else
#define  DBG_STEP()
//printf("tls:%u\n",__LINE__);
#endif

typedef int (* TFprintf)(char *param,...);

typedef int (* TFtransfer)(void *param,void *bfr,int l);
static TFtransfer FSend,FRecv;

struct OpenSSLConnection
{int state;
 void *CallbackParam;
#ifdef GTLS
 gnutls_session_t session;
#else
 SSL *con;
 BIO *io,*ssl_bio,*sbio;
#endif
};


static int XNullFunc(char * x,...){return 0;};
static TFprintf Fprintf=XNullFunc;
static ulong session_id_cntr,s_server_session_id_context;


inline
ulong rol(ulong a,int b){
#ifdef NOTINTEL
  return (a<<b)|(a>>(32-b) );
#else
 asm volatile(" roll  %%cl,%%eax ":"=&a"(a),"=&c"(b)
 :"0"(a),"1"(b)
 );
 return a;
#endif
};


static ulong OldRnd,RndCounter;
static ulong Rnd()
{
 ulong r,v1,v2;


 //FILETIME ft;
 //GetSystemTimeAsFileTime(&ft);
 //v1=(GetTickCount()^(ulong)&r);
 //v2=ft.dwHighDateTime * ft.dwLowDateTime;
  struct timeval tv;

#ifndef  MINGW

 gettimeofday(&tv,0);

#else
 SYSTEMTIME  stime;
 GetLocalTime(&stime);
 SystemTimeToFileTime(&stime, (FILETIME *)&tv);
#endif

 v2=tv.tv_sec+tv.tv_usec;
 r=rol( (OldRnd^tv.tv_usec)
       ^(v1<<(v2%24))+v2
       ,((OldRnd>>(v1&15))^(tv.tv_usec>>4))+ ++RndCounter );
 OldRnd=r;
 return r;
};


static int mRAND_bytes(unsigned char *buf, int num)
{int n;
 for(n=num;n>0;)
 {if(n>=4){DWORD_PTR(*buf)=Rnd(); n-=4; buf+=4; }
  else{*buf++=Rnd(); --n; }
 };
 return num;
}


static int pull_timeout_func(int *s, unsigned int ms)
{
  int r;
  DBG_STEP()
#ifndef MINGW //USE_POOL
   struct pollfd  pfd;
   pfd.fd=* (int *) s;
   pfd.events=POLLIN;
   pfd.revents=0;

   r=poll(&pfd,1,ms );
  // printf("poll(%d) %d %d ms\n",*s,r,ms);
   return r;
   //poll(&pfd,1,ms );
#else
//*
 fd_set set;
 timeval tv;
 int ss;

 ss=* (int *) s;
 FD_ZERO(&set);
 FD_SET(ss, &set);
 tv.tv_sec=ms/1000;
 tv.tv_usec=(ms%1000) * 1000;
 r=select(ss+1,&set,0,0,&tv);
 DebugPrintFnc("dbg TLS %X %u %X %d\r\n",s,ss,ms,r);
 DBG_STEP()
 return r;
// */
// return 1;
#endif

};
#ifdef MINGW

static int xFSend(void *p,char *b,int l)
{
 DebugPrintFnc("xFSend %X %X %d\r\n",p,b,l);
 return (FSend)(p,b,l);

}

static int xFRecv(void *p,char *b,int l)
{
 DebugPrintFnc("xFRecv %X %X %d\r\n",p,b,l);
 return (FRecv)(p,b,l);

}
#endif
char copyright[]=
"Securety library, using GnuTLS Library Copyright (c) 1998-2023 The GnuTLS Project\r\n";


static char *CApath, *CAfile, * s_cert_file,  * s_key_file, *priority_str;

#define MAX_CON 128
static int act_con_cnt,reinit_ctx_need;
//static time_t  con_time[MAX_CON];
static time_t  reinit_ctx_need_time;

static gnutls_certificate_credentials_t x509_cred;
static gnutls_priority_t priority_cache;
//static gnutls_session_t session;

static int REinitCTX()
{
 int r;
 char *crr;
 reinit_ctx_need=0;
#define CHECK(a)  if( (r=a)<0 ){ Fprintf("GNUTLS Eroror %d (%s) in " #a ,r,gnutls_strerror(r) );  return 0;}

       CHECK(gnutls_certificate_allocate_credentials(&x509_cred));

       if(CAfile && CAfile[0])
//        CHECK(gnutls_certificate_set_x509_trust_file(x509_cred, CAfile , GNUTLS_X509_FMT_PEM));
        if((r=gnutls_certificate_set_x509_trust_file(x509_cred, CAfile , GNUTLS_X509_FMT_PEM))<=0 )
        {
            Fprintf("Eroror %u (%s) in gnutls_certificate_set_x509_trust_file (%s)"  ,r,gnutls_strerror(r) ,CAfile) ;
            return 0;
        }

        if(CApath && CApath[0])
            gnutls_certificate_set_x509_trust_dir(x509_cred, CApath, GNUTLS_X509_FMT_PEM);
//             CHECK(gnutls_certificate_set_x509_crl_file(x509_cred, CApath,
//                                                    GNUTLS_X509_FMT_PEM));



        /* The following code sets the certificate key pair as well as,
         * an OCSP response which corresponds to it. It is possible
         * to set multiple key-pairs and multiple OCSP status responses
         * (the latter since 3.5.6). See the manual pages of the individual
         * functions for more information.
         */

       if(s_cert_file && s_cert_file[0])
        CHECK(gnutls_certificate_set_x509_key_file(x509_cred, s_cert_file,
                                                   s_key_file,
                                                   GNUTLS_X509_FMT_PEM));

//         CHECK(gnutls_certificate_set_ocsp_status_request_file(x509_cred,
//                                                               OCSP_STATUS_FILE,
//                                                               0));
       do{
        if(priority_str && priority_str[0])
        {
          crr=0;
          if(
#ifdef GNUTLS_PRIORITY_INIT_DEF_APPEND
              gnutls_priority_init2(&priority_cache,
                                //"%SERVER_PRECEDENCE",
                                priority_str,
               (const char **)  &crr, 0 // GNUTLS_PRIORITY_INIT_DEF_APPEND
                                  )
#else
              gnutls_priority_init(&priority_cache,
                                //"%SERVER_PRECEDENCE",
                                priority_str,
              (const char **) &crr)
#endif
              >=0)
          {
              break;

          }
          Fprintf("GNUTLS Eroror %d (%s) in gnutls_priority_init2 at '%.32s...'",r,gnutls_strerror(r), (crr)?crr:""  );
        }
        CHECK(gnutls_priority_init(&priority_cache, NULL, NULL));
       }while(0);
        /* Instead of the default options as shown above one could specify
         * additional options such as server precedence in ciphersuite selection
         * as follows:
         * gnutls_priority_init2(&priority_cache,
         *                       "%SERVER_PRECEDENCE",
         *                       NULL, GNUTLS_PRIORITY_INIT_DEF_APPEND);
	 */


#undef CHECK
  return 1;

}

void SetPriority(char *t)
{
  priority_str=t;
}

int InitLib( TFprintf prnt,TFtransfer fsend,TFtransfer frecv,char *lCApath,char *lCAfile,
  char * ls_cert_file,
  char * ls_key_file
//  ,char * s_dcert_file,
//  char * s_dkey_file
//  ,int s_server_verify
)
{
 int r;
#ifdef MINGW
 id_heap =   GetProcessHeap();
 hstdout=(int)GetStdHandle((ulong)STD_OUTPUT_HANDLE);

#endif
 if(prnt)
 {
  Fprintf=prnt;
  (prnt)(copyright);
 }
 DBG_STEP()
 FSend=fsend; FRecv=frecv;
 gnutls_global_init();
//TLS_RSA_WITH_AES_256_CBC_SHA
 CApath       =lCApath     ;
 CAfile       =lCAfile     ;
 s_cert_file  =ls_cert_file;
 s_key_file   =ls_key_file ;
 r=REinitCTX();
#if 0
 SSL_CTX_set_quiet_shutdown(ctx,1);
 DBG_STEP()
// SSL_CTX_set_options(ctx,0);
 SSL_CTX_set_options(ctx,SSL_OP_ALL|SSL_OP_CIPHER_SERVER_PREFERENCE); //SSL_OP_NO_TLSv1
 SSL_CTX_set_cipher_list(ctx,"ECDHE-RSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-RSA-AES256-SHA384:ALL:!DES:!3DES:!RC2");
 DBG_STEP()
// SSL_CTX_sess_set_cache_size(ctx,512);
// SSL_CTX_sess_set_cache_size(ctx,0);
 SSL_CTX_set_session_cache_mode(ctx,SSL_SESS_CACHE_OFF);

 DBG_STEP()

//*
 if ( CApath && CApath[0] && (!SSL_CTX_load_verify_locations(ctx,CAfile,CApath))
  //   ||  (!SSL_CTX_set_default_verify_paths(ctx))
  )
    {
      DBG_STEP()
//     BIO_printf(bio_err,"X509_load_verify_locations\n");
     (prnt)("X509 load verify locations");
     DBG_STEP()
    }
//*/
 X509_store = SSL_CTX_get_cert_store(ctx);
 DBG_STEP()
// X509_STORE_set_flags(X509_store,0);// vflags);
 if (!set_cert_stuff(ctx,s_cert_file,s_key_file))
 {
  Fprintf("OpenSSL error: Can't set certificates stuff");
  return 0;
 }
DBG_STEP()
// SSL_CTX_set_generate_session_id(ctx, generate_session_id);
/// set_cert_stuff(ctx,s_cert_file,s_key_file);

DBG_STEP()
// SSL_CTX_set_tmp_rsa_callback(ctx,tmp_rsa_cb);
// SSL_CTX_set_verify(ctx,s_server_verify,verify_callback);
// SSL_CTX_set_verify(ctx,1,verify_callback);
 SSL_CTX_set_session_id_context(ctx,(const unsigned char *)&s_server_session_id_context,
            sizeof s_server_session_id_context);
DBG_STEP()
 if (CAfile != NULL)
   SSL_CTX_set_client_CA_list(ctx,SSL_load_client_CA_file(CAfile));
DBG_STEP()
end:;
#endif


 return r;
}
#define BFRSIZE (16*1024)
int SecAccept(struct OpenSSLConnection *s);
/*
BIO *BIO_new(BIO_METHOD *method)
{
 BIO *ret=0;
 DBG_STEP()
 ret=(BIO *) malloc(sizeof(BIO));
 if (ret)
 {
   if(!BIO_set(ret,method))
   {free(ret);
    ret=0;
   }
 }
 return(ret);
}
// */
#ifdef MINGW

int __errno;
int * _errno(){__errno= GetLastError(); return &__errno ; };

#endif

static void PrepareSession(struct OpenSSLConnection *s)
{
  gnutls_transport_set_ptr(s->session, s->CallbackParam);

  DBG_STEP()
  #ifndef MINGW
  #define   xFSend    FSend
  #define   xFRecv    FRecv
  #endif
  gnutls_transport_set_push_function(s->session,(gnutls_push_func) xFSend );

  DBG_STEP()

  gnutls_transport_set_pull_function(s->session, (gnutls_pull_func) xFRecv);

  DBG_STEP()

  gnutls_transport_set_pull_timeout_function(s->session,(gnutls_pull_timeout_func)
  pull_timeout_func);
  DBG_STEP()

  gnutls_handshake_set_timeout(s->session,
                               GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);
  gnutls_record_set_timeout(s->session, 30000);
}

int SecAccept(struct OpenSSLConnection *s)
{
  int r;
  int err;
  DBG_STEP()
/*
  if(act_con_cnt<MAX_CON)
  {
    con_time[act_con_cnt]=time(0);
  }
*/
  act_con_cnt++;

#define CHECK(a)  if( (err = a ) < 0 ){ (Fprintf)("GNUTLS error at line:%u: %d:%s\r\n", __LINE__, err, gnutls_strerror(err)); return 0; }


                CHECK(gnutls_init(&s->session, GNUTLS_SERVER));
                CHECK(gnutls_priority_set(s->session, priority_cache));

                DBG_STEP()

                CHECK(gnutls_credentials_set(s->session, GNUTLS_CRD_CERTIFICATE,
                                             x509_cred));

                DBG_STEP()

                /* We don't request any certificate from the client.
                 * If we did we would need to verify it. One way of
                 * doing that is shown in the "Verifying a certificate"
                 * example.
                 */
                gnutls_certificate_server_set_request(s->session,
                                                      GNUTLS_CERT_IGNORE);


                DBG_STEP()
//                gnutls_transport_set_int(s->session, * (int *) s->CallbackParam);
                PrepareSession(s);
//*

                do {

                DBG_STEP()

                     r = gnutls_handshake(s->session);
                } while(r == GNUTLS_E_AGAIN || r == GNUTLS_E_INTERRUPTED);

                DBG_STEP()
                if(r<0)
                {
                    Fprintf( "GNUTLS: *** Handshake has failed (%s)\r\n\r\n",
                                gnutls_strerror(r));

                    gnutls_deinit(s->session);

                    DBG_STEP()

                    return 0;
                }

  DBG_STEP()
//  BIO_push(s->io,s->ssl_bio);
  return 1;

};

int SecUpdateCB(OpenSSLConnection *s)
{
  gnutls_transport_set_ptr(s->session, s->CallbackParam);
  return 1;
};


static gnutls_anon_client_credentials_t anoncred;
static gnutls_certificate_credentials_t xcred;
static int anon_inited;


static int verify_certificate_callback(gnutls_session_t session)
{
  unsigned int status;
  const gnutls_datum_t *cert_list;
  unsigned int cert_list_size;
  int ret;
  gnutls_certificate_type_t type;
  gnutls_datum_t out;
  const char *hostname;

  /* read hostname */
  hostname = (const char *)gnutls_session_get_ptr(session);

  /* This verification function uses the trusted CAs in the credentials
   * structure. So you must have installed one or more CA certificates.
   */
  DBGL()
  if( (ret=gnutls_certificate_verify_peers3(session, hostname, &status)) < 0 )
  {
    DBGLA("gnutls_certificate_verify_peers3 return %d status = %d %s", ret, status, gnutls_strerror(ret) )
    return ret;
  }

  type = gnutls_certificate_type_get(session);

  if (status == 0) /* Certificate is trusted */
  {
    DBGLS("Trasted sertificate")
    return 0;
  }

  /* Do SSH verification */
  cert_list = gnutls_certificate_get_peers(session, &cert_list_size);
  if (cert_list == NULL) {
    Fprintf("No certificate was found!\r\n");
    return GNUTLS_E_CERTIFICATE_ERROR;
  }

  /* service may be obtained alternatively using getservbyport() */
  ret = gnutls_verify_stored_pubkey(NULL, NULL, hostname, "https", type,
                                    &cert_list[0], 0);
  if (ret == GNUTLS_E_NO_CERTIFICATE_FOUND)
    Fprintf("Host %s is not known.\r\n", hostname);
  else if (ret == GNUTLS_E_CERTIFICATE_KEY_MISMATCH) {
    Fprintf("Error: host %s is known but has another key associated. "
            "It might be that the server has multiple keys, or you are under attack\r\n"
            "If you are sure that the sertificate is valid -- remove old info from $HOME/.gnutls/known_hosts\r\n",
            hostname);
    return GNUTLS_E_CERTIFICATE_ERROR;
  } else if (ret < 0) {
    Fprintf("gnutls_verify_stored_pubkey: %s\r\n",
           gnutls_strerror(ret));
    return ret;
  }

  if (ret != 0) {
    DBGLS("store!")
    if(gnutls_store_pubkey(NULL, NULL, hostname, "https", type,
                              &cert_list[0], 0, 0))
      Fprintf("Cant store Public Key info for sertificate...\r\n");
  }

  /* notify gnutls to continue handshake normally */
  return 0;
}


int SecConnect(struct OpenSSLConnection *s, int anon, char *verfyhost)
{
  int r;
  int err;


  DBG_STEP()
  act_con_cnt++;

  CHECK(gnutls_init(&s->session, GNUTLS_CLIENT));
  CHECK(gnutls_priority_set(s->session, priority_cache));

  DBG_STEP()

  if( anon & tbtAnon )
  {
    if(!anon_inited)
    {
      CHECK(gnutls_anon_allocate_client_credentials(&anoncred) );
      CHECK(gnutls_certificate_allocate_credentials(&xcred));
      if(CApath && CApath[0])
            gnutls_certificate_set_x509_trust_dir(xcred, CApath, GNUTLS_X509_FMT_PEM);
      anon_inited++;
    }

    CHECK(gnutls_credentials_set(s->session, GNUTLS_CRD_ANON, anoncred));
//     if( anon & tbtSSHstyleVerfy )
//     {
//       DBGLS("set verify_callback")
//       gnutls_certificate_set_verify_function(xcred, verify_certificate_callback);
//     }
    CHECK(gnutls_credentials_set(s->session, GNUTLS_CRD_CERTIFICATE, xcred));
  }
  else
  {
    CHECK(gnutls_credentials_set(s->session, GNUTLS_CRD_CERTIFICATE, x509_cred));
  }
  DBG_STEP()
  //gnutls_dh_set_prime_bits(s->session, 512);

  DBG_STEP()
  //                gnutls_transport_set_int(s->session, * (int *) s->CallbackParam);
  PrepareSession(s);

  if(verfyhost)
  {
    gnutls_server_name_set(s->session, GNUTLS_NAME_DNS,
				     verfyhost, strlen(verfyhost));
    r = GNUTLS_VERIFY_IGNORE_UNKNOWN_CRIT_EXTENSIONS;
    if(anon & tbtDontVerfyTyme) r = GNUTLS_VERIFY_DISABLE_TIME_CHECKS ;
    if(anon & tbtDontVerfySigner) r |= GNUTLS_VERIFY_DISABLE_CA_SIGN;
    gnutls_session_set_verify_cert(s->session, verfyhost, r);
    if( anon & tbtSSHstyleVerfy )
    {
      DBGLS("set verify_callback")
      //gnutls_certificate_set_verify_function(xcred, verify_certificate_callback);
      gnutls_session_set_verify_function(s->session, verify_certificate_callback);
      gnutls_session_set_ptr(s->session, verfyhost);
    }
  }
  //gnutls_set_default_priority(s->session);

  do {

    DBG_STEP()

    r = gnutls_handshake(s->session);
  } while(r == GNUTLS_E_AGAIN || r == GNUTLS_E_INTERRUPTED);

  DBG_STEP()
  if(r<0)
  {
    Fprintf( "GNUTLS: *** Handshake has failed (%d: %s)\r\n\r\n",
             r, gnutls_strerror(r));
    if (r == GNUTLS_E_CERTIFICATE_VERIFICATION_ERROR)
    {
      gnutls_datum_t out;
      gnutls_certificate_type_t type;
      unsigned status;
      /* check certificate verification status */
      type = gnutls_certificate_type_get(s->session);
      status = gnutls_session_get_verify_cert_status(s->session);
      if(gnutls_certificate_verification_status_print(status, type, &out, 0) >= 0 )
      {
        Fprintf("cert verify output: %s\r\n", out.data);
        gnutls_free(out.data);
      }
    }

    gnutls_deinit(s->session);

    DBG_STEP()

    return 0;
  }
  //Fprintf("GNUTLS: Connect Ok %d\r\n", r); ///!!!
  DBG_STEP()
  return 1;
};

int SecRecv(struct OpenSSLConnection *s,char *b,int l)
{
 int r;
 int try_again_limit = 16;
 DBG_STEP()
 do {
   r = gnutls_record_recv(s->session, b, l);
   if(r != GNUTLS_E_AGAIN) break;
 //  Fprintf("GNUTLS: recv %d E_AGAIN %lX %d\r\n", l, s->CallbackParam, *(int *) s->CallbackParam);
   pull_timeout_func( (int *) s->CallbackParam, 10000);
 }while(try_again_limit-- >0);

 if(r<0)
 {
    Fprintf( "GNUTLS: recv error (%d: %s)\r\n",
             r, gnutls_strerror(r));
 }

 return r;
}
int SecSend(struct OpenSSLConnection *s,char *b,int l)
{
 DBG_STEP()
 return gnutls_record_send(s->session, b, l);
}
//return BIO_write(s->io,b,l);}
int SecClose(struct OpenSSLConnection *s)
{
// BIO_flush(s->io);
 DBG_STEP()
 gnutls_bye(s->session, GNUTLS_SHUT_WR);
 gnutls_deinit(s->session);
 return 0;
}


#ifdef __cplusplus
}
#endif


