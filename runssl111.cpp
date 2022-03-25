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
#define __TIME_H_ 1

#include <windows.h>

#else

#include "mstring1.h"

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#define ioctlsocket ioctl

#endif


#include <openssl/e_os2.h>
/*
typedef unsigned int u_int;
#include <openssl/lhash.h>
#include <openssl/bn.h>
#include <openssl/err.h>
*/
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <openssl/ossl_typ.h>
#include <openssl/err.h>

#include "mdef.h"





#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wwrite-strings"


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
static char ddbg[4512];

//#define  DBG_STEP() _hwrite(hstdout,ddbg,wsprintf(ddbg,"SSLDBG:%u\r\n",__LINE__ )); MessageBox(0,ddbg,"DBG",MB_OK);



typedef void (*tDebugFnc)(char *f,int l);
typedef void (_cdecl *tDebugPrintFnc)(char *f,...);

void  SetDfnc(tDebugFnc f,tDebugPrintFnc fp);



void  DebugFnc(char *f,int l)
{
  // _hwrite(hstdout,ddbg,wsprintf(ddbg,"SSLDBG:%.50s,%u\r\n",f,l ));  
  // l=wsprintf(ddbg,"SSLDBG:%.50s,%u\r\n",(f)?f:"??",l );
  //_hwrite(hstdout,ddbg,l);  
};
static int mem_count,mem_n;
void  MDebugFnc1(const char*t,const char *f,int l,ulong p,int n)
{
  // l=wsprintf(ddbg,"SSLDBG:%s,%.50s,%u %X=%u  %u,%u\r\n",t,(f)?f:"??",l,p,n,mem_count,mem_n );
  // _hwrite(hstdout,ddbg,l);  
};
#define MDebugFnc(a...)

void _cdecl DebugPrintFnc(char *a,...) ;

#define  DBG_STEP() 
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
    if(!n)return 0; 
    r=(char *)HeapAlloc(id_heap,HEAP_ZERO_MEMORY,n+sizeof(dbg_mem)+4);
    if(r)
    {
      mem_count+=n;  
      mem_n++;
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

void   dbg_free(void *p
  ,const char *f,int l   
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
      mem_n--;
      d->check = 0;
      MDebugFnc((DWORD_PTR(r[d->n+sizeof(dbg_mem)])==0x5A4B7F12)?"free" : "free check FAIL!",d->f,d->l,rl,d->n);
    }
    else
    {    
      MDebugFnc("free bad p","",0,rl,0);
      return ;
    }  

  
    HeapFree(id_heap,0,r); 
    
}

void * dbg_realloc(void *p,size_t n,const char *f,int l){
    union{
     char *r;
     dbg_mem *d;
     ulong rl;
    };
    int on;
    if(!p)
    {
        MDebugFnc( "realloc with zero pointer",f,l,0,n);
        return dbg_malloc(n,f,l);
    }
    
    if(!n) { dbg_free(p,f,l); return 0; }
    
    r=(char *)p;
    r-=sizeof(dbg_mem);
    if(d->check == 0x5A4B8F12)
    {
      MDebugFnc( (DWORD_PTR(r[d->n+sizeof(dbg_mem)])==0x5A4B7F12) ? "realloc1":"realloc check FAIL!",d->f,d->l,rl,d->n);
      on=d->n;  

    }
    else
    {
        MDebugFnc("realloc bad p",f,l,rl,n);
        return 0;
    }    

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


#else

#define  DBG_STEP()

#endif

#define  DebugPrintFnc(a...)
#define  DebugFnc(a...)


typedef int (* TFprintf)(char *param,...);

typedef int (* TFtransfer)(void *param,void *bfr,int l);
static TFtransfer FSend,FRecv;

struct OpenSSLConnection
{int state;
 void *CallbackParam;
 SSL *con;
 BIO *io,*ssl_bio,*sbio;
};


static int XNullFunc(char * x,...){return 0;};
static TFprintf Fprintf=XNullFunc;
static SSL_CTX *ctx;

char FMT[]="%s";
#define  BIO_printf(a,b...) (Fprintf)(b)
#define ERR_print_errors(a) (Fprintf)("OpenSSL error")

static X509_STORE *X509_store;

#ifdef OSSL111
#define  BIODATA(t)   BIO_get_data(t)
#else
#define  BIODATA(t)  (t->ptr)
#endif

static int bwrite(BIO *t, const char *b, int l)
{ //(Fprintf)("BIO write %u",l);
 DebugPrintFnc("\r\nBwrite %X %u\r\n",b,l);   
 return (FSend)(BIODATA(t),(void *)b,l); 
}
static int bread(BIO *t, char *b, int l)
{
 //(Fprintf)("BIO read %u",l);
 void *v=BIODATA(t);
 DebugPrintFnc("\r\nBread %X %X %u\r\n",v,b,l);   
 return (FRecv)(v //BIODATA(t)
           ,b,l);
}

static int bputs  (BIO *t, const char *b)
{
 int i,j;
  if(!b)return 0;
  j=strlen(b);
  DebugPrintFnc("\r\nBputs %X %.64s %u\r\n",b,j);   
  ((char *)b)[j]='\n';
  i=(FSend)(BIODATA(t),(void *)b,j+1);
  ((char *)b)[j]=0;
  return i;
};


static int bgets  (BIO *t, char *b, int l)
{
 int i;
 for(i=0;i<l;++i)
 { if((FRecv)(BIODATA(t),b,1)<=0)break;
   if(*b++=='\n'){
       if(!i)++i; 
       break;
    }
 }
 *b=0;
 return i;
};

static long ctrl  (BIO *p, int a, long b, void *c)
{
// (Fprintf)("BIO CTRL %u %u %X",a,b,c);
  DebugFnc("BIO CTRL ",a);  
 int *s;
 long l; 
// DBG_STEP()   
 switch (a)
 {
  case BIO_CTRL_DUP:
  case BIO_CTRL_FLUSH:
  case BIO_CTRL_SET:
          return 1;      
  case BIO_CTRL_GET: return BIO_TYPE_CIPHER;
      
  case BIO_CTRL_WPENDING: return 0x2000;
  case BIO_CTRL_PENDING:
          s=(int *)  BIODATA(p);
          l=0;
          ioctlsocket(*s,FIONREAD,(ulong *)&l);
          return l;
/*
  case BIO_CTRL_RESET:
  case BIO_C_FILE_SEEK:
  case BIO_C_FILE_TELL:
  case BIO_CTRL_INFO:
  case BIO_C_SET_FD:
  case BIO_C_GET_FD:
  case BIO_CTRL_GET_CLOSE:
  case BIO_CTRL_SET_CLOSE:
  case BIO_CTRL_PENDING:
  case BIO_CTRL_WPENDING:
  default:
    return 0;
*/
     }
 return 0;
};
static int create (BIO *x){
// (Fprintf)("BIO CREATE");
 return 1;};
static int destroy(BIO *x){
// (Fprintf)("BIO Destroy");
 return 1;};
static long callback_ctrl(BIO *x, int z, bio_info_cb *y)
{
// (Fprintf)("BIO Callback CTRL");
return 0;};

#ifdef  OSSL111
static BIO_METHOD *srv_meth;

int CRYPTO_set_mem_functions(
        void *(*m)(size_t, const char *, int),
        void *(*r)(void *, size_t, const char *, int),
        void (*f)(void *, const char *, int));
#else
static BIO_METHOD srv_meth=
{
 BIO_TYPE_CIPHER, // int type;
 "Server",          // const char *name;
 bwrite      ,// int (*bwrite)(BIO *, const char *, int);
 bread,       // int (*bread)(BIO *, char *, int);
 bputs,       // int (*bputs)(BIO *, const char *);
 bgets,       // int (*bgets)(BIO *, char *, int);
 ctrl,       // long (*ctrl)(BIO *, int, long, void *);
 create,      // int (*create)(BIO *);
 destroy,     // int (*destroy)(BIO *);
 callback_ctrl // long (*callback_ctrl)(BIO *, int, bio_info_cb *);
};
#endif

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


static int generate_session_id(const SSL *ssl, unsigned char *id,
                        unsigned int *id_len)
{
 mRAND_bytes(id,*id_len);
#if 0
 unsigned int i,tk,o;
 ulong *a;

 a=(ulong *)&ssl;
 o=tk=GetTickCount();
 for(i=*id_len;i>4;++i)
 {--i;
  id[i]=(tk>>(i&7))+(a[1]*a[2]);
  if(o&1) id[i]^=a[0];
  if(o&2) id[i]^=~tk>>12;
  if(o&4) id[i]+=~tk>>20;
  o=id[i]^i;
 }

 DWORD_PTR(*id)=++session_id_cntr^(tk<<8);
// memcpy(id, session_id_prefix,  (strlen(session_id_prefix) < *id_len) ? strlen(session_id_prefix) : *id_len);
#endif

 return 1;
}

static int set_cert_stuff(SSL_CTX *ctx, char *cert_file, char *key_file)
{
 if (cert_file != NULL)
 {
  DBG_STEP()
  if(SSL_CTX_use_certificate_file(ctx,cert_file, SSL_FILETYPE_PEM) <= 0)
  {
   DBG_STEP()   
   BIO_printf(bio_err,"unable to get certificate from '%s'",cert_file);
   ERR_print_errors(bio_err);
   return(0);
  }
  DBG_STEP()
  if (key_file == NULL)
  {lb1:
   DBG_STEP()     
   key_file=cert_file;
  }
  DBG_STEP()
  if (SSL_CTX_use_PrivateKey_file(ctx,key_file, SSL_FILETYPE_PEM) <= 0)
  {
   BIO_printf(bio_err,"unable to get private key from '%s'",key_file);
   ERR_print_errors(bio_err);
   if(key_file!=cert_file)goto lb1;
   return(0);
  }
  DBG_STEP()

  
 /* If we are using DSA, we can copy the parameters from
  * the private key */


 /* Now we know that a key and cert have been set against
  * the SSL context */
  if (!SSL_CTX_check_private_key(ctx))
  {
   BIO_printf(bio_err,"Private key does not match the certificate public key\n");
   return(0);
  }

  DBG_STEP()

 }
 return(1);
}


char copyright[]=
"\r\nSecurety library, based on OpenSSL Library Copyright (c) 1998-2019 The OpenSSL Project\r\n"
"This product includes software developed by the OpenSSL Projec Copyright (c) 1995-1998 Eric A. Young, Tim J. Hudsont for use in the OpenSSL Toolkit. (http://www.openssl.org/)\r\n"
"This product includes cryptographic software written by Eric Young (eay@cryptsoft.com).\r\n"
"This product includes software written by Tim Hudson (tjh@cryptsoft.com).\r\n"
"This product includes software written by Dr Stephen N Henson (shenson@bigfoot.com).\r\n"
"\r\n"
;

static RSA *RSA_key;


inline
RSA *tmp_rsa_cb(SSL *s,int is_export, int keylength) //WINAPI
{
 if(!RSA_key)RSA_key=RSA_generate_key(keylength,RSA_F4,NULL,NULL);
 return RSA_key;
}

#if 0
int verify_callback(int ok, X509_STORE_CTX *ctx)
{
 char buf[256];
 X509 *err_cert;
 int err,depth;
#if 0
 err_cert=X509_STORE_CTX_get_current_cert(ctx);
 err=    X509_STORE_CTX_get_error(ctx);
 depth=  X509_STORE_CTX_get_error_depth(ctx);

 X509_NAME_oneline(X509_get_subject_name(err_cert),buf,sizeof(buf));
 BIO_printf(bio_err,"depth=%d %s\n",depth,buf);
 if (!ok)
 {
  BIO_printf(bio_err,"verify error:num=%d:%s\n",err,
          X509_verify_cert_error_string(err));
  if (verify_depth >= depth)
  {
   ok=1;
   verify_error=X509_V_OK;
  }
  else
  {
   ok=0;
   verify_error=X509_V_ERR_CERT_CHAIN_TOO_LONG;
  }
 }
 switch (ctx->error)
         {
 case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
         X509_NAME_oneline(X509_get_issuer_name(ctx->current_cert),buf,sizeof buf);
         BIO_printf(bio_err,"issuer= %s\n",buf);
         break;
 case X509_V_ERR_CERT_NOT_YET_VALID:
 case X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD:
         BIO_printf(bio_err,"notBefore=");
         ASN1_TIME_print(bio_err,X509_get_notBefore(ctx->current_cert));
         BIO_printf(bio_err,"\n");
         break;
 case X509_V_ERR_CERT_HAS_EXPIRED:
 case X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD:
         BIO_printf(bio_err,"notAfter=");
         ASN1_TIME_print(bio_err,X509_get_notAfter(ctx->current_cert));
         BIO_printf(bio_err,"\n");
         break;
         }
 BIO_printf(bio_err,"verify return:%d\n",ok);
 return(ok);
#endif
 return 1;
}

#endif
static char *CApath, *CAfile, * s_cert_file,  * s_key_file;

#define MAX_CON 128
static int act_con_cnt,reinit_ctx_need;
//static time_t  con_time[MAX_CON];
static time_t  reinit_ctx_need_time;

char *priority_str="ECDHE-RSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-RSA-AES256-SHA384:ALL:!DES:!3DES:!RC2";

void SetPriority(char *t)
{
  priority_str=t;    
}


int REinitCTX()
{

 reinit_ctx_need=0;  
 
 SSL_CTX_set_quiet_shutdown(ctx,1);
 DBG_STEP()
// SSL_CTX_set_options(ctx,0);
 SSL_CTX_set_options(ctx,SSL_OP_ALL|SSL_OP_CIPHER_SERVER_PREFERENCE); //SSL_OP_NO_TLSv1
 SSL_CTX_set_cipher_list(ctx,priority_str);
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
     (Fprintf)("X509 load verify locations");
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
    
  return 1;  
    
}

int OsslErrCb(const char *str, size_t len, void *u)
{
    
    (Fprintf)("OpenSSL:%.128s",(str)?str:"null");
//    if(len && str)_hwrite(hstdout,str,len);
    return len;
    
}
#define  DBG_ERR(a)  OsslErrCb(a,sizeof(a),0)

int InitLib( TFprintf prnt,TFtransfer fsend,TFtransfer frecv,char *lCApath,char *lCAfile,
  char * ls_cert_file,
  char * ls_key_file
//  ,char * s_dcert_file,
//  char * s_dkey_file
//  ,int s_server_verify
)
{
    
#ifdef MINGW 
 id_heap =  HeapCreate(0,0x1000,0x2000000);
 if(!id_heap)
 {
   id_heap=GetProcessHeap(); 
   (prnt)("Cant create heap\r\n");
 }    
 
 hstdout=(int)
      GetStdHandle((ulong)STD_OUTPUT_HANDLE);  
//      GetStdHandle((ulong)STD_ERROR_HANDLE);  
//  if(hstdout<0)
//  {
//    hstdout=_lcreat("err_ssl.txt",0);    
//  }     
 MDebugFnc("test",0,0,0,0);
#endif   
 if(prnt)
 {
  Fprintf=prnt;
  (prnt)(copyright);
 }
 DBG_STEP()
 FSend=fsend; FRecv=frecv;
 
//  ERR_print_errors_cb(int (*cb)(const char *str, size_t len, void *u), void *u)

#ifdef  OSSL111
 /*
  if(!CRYPTO_set_mem_functions( dbg_malloc,dbg_realloc,dbg_free))
  {
     (prnt)("Cant set mem functions\r\n");   
  }
  */
  srv_meth = BIO_meth_new(BIO_TYPE_CIPHER, "Server");
  if(!srv_meth)
  {
      DBG_ERR("BIO_meth_new return 0");
      return 0;
  }    
  BIO_meth_set_write(srv_meth,  bwrite) ;
  BIO_meth_set_read(srv_meth,   bread) ;
  BIO_meth_set_puts(srv_meth,   bputs);
  BIO_meth_set_gets(srv_meth,   bgets);
  BIO_meth_set_ctrl(srv_meth,   ctrl);
  BIO_meth_set_create(srv_meth, create);
  BIO_meth_set_destroy(srv_meth,destroy);
 //callback_ctrl
  
  //CRYPTO_set_mem_debug(1);
 // CRYPTO_set_mem_ex_functions( dbg_malloc,dbg_realloc,dbg_free);
 // CRYPTO_set_locked_mem_ex_functions( dbg_malloc,dbg_realloc,dbg_free);
#else 

 #ifdef  MINGW
// SetDfnc(DebugFnc,DebugPrintFnc);
 SetDfnc(DebugFnc,(tDebugPrintFnc)prnt);
 
 CRYPTO_set_mem_ex_functions( dbg_malloc,dbg_realloc,dbg_free);
 //CRYPTO_set_mem_functions( dbg_malloc,dbg_realloc,dbg_free);

#endif

#endif 
  ERR_print_errors_cb(OsslErrCb, 0);
 
///! 
 OpenSSL_add_ssl_algorithms();
 DBG_STEP()
// ctx=SSL_CTX_new(SSLv23_server_method());
// DBG_STEP()
//  ctx=SSL_CTX_new(TLSv1_server_method());
  ctx=SSL_CTX_new(TLS_server_method());
//  DBG_STEP()
 if(!ctx)
 {
   DBG_STEP()    
   return 0;    
 }
//TLS_RSA_WITH_AES_256_CBC_SHA   
 CApath       =lCApath     ;
 CAfile       =lCAfile     ;
 s_cert_file  =ls_cert_file;
 s_key_file   =ls_key_file ;
 REinitCTX();
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


 return 1;
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
//void OPENSSL_cleanse(void *x,int l)
// void OPENSSL_cleanse(void* x , size_t l)
// {
//   if(x && l) memset(x,0,l);
// }

/* This version of _reent is laid out with "int"s in pairs, to help
 * ports with 16-bit int's but 32-bit pointers, align nicely.  */
#if 0
struct _reent
{
  /* As an exception to the above put _errno first for binary
     compatibility with non _REENT_SMALL targets.  */
  int _errno;			/* local copy of errno */
  /* FILE is a big struct and may change over time.  To try to achieve binary
     compatibility with future versions, put stdin,stdout,stderr here.
     These are pointers into member __sf defined below.  */
  void *_stdin, *_stdout, *_stderr;	/* XXX */
  int  _inc;			/* used by tmpnam */
  char *_emergency;
  int __sdidinit;		/* 1 means stdio has been init'd */
  int _current_category;	/* unused */
  char *_current_locale;	/* unused */
  //struct _mprec
  void *_mp;
//  void _EXFNPTR(__cleanup, (struct _reent *));
  void (*__cleanup)(struct _reent *r);
  int _gamma_signgam;
  /* used by some fp conversion routines */
  int _cvtlen;			/* should be size_t */
  char *_cvtbuf;
//  struct _rand48 *_r48;
  void *_r48;
  void *_localtime_buf;
//  struct __tm *_localtime_buf;
  char *_asctime_buf;
  /* signal info */
  void (**(_sig_func))(int);
# if 0 //ndef _REENT_GLOBAL_ATEXIT
  /* atexit stuff */
  struct _atexit *_atexit;
  struct _atexit _atexit0;
# endif
//  struct _glue __sglue;			/* root of glue chain */
//  __FILE *__sf;			        /* file descriptors */
  void *__sf;			        /* file descriptors */
//  struct _misc_reent *_misc;            /* strtok, multibyte states */
  void *_misc;            /* strtok, multibyte states */
  char *_signal_buf;                    /* strsignal */
} _impure,
  * _impure_ptr = &_impure;

  unsigned short * * _imp___ctype_ ;
  
//   {
//     0,0,0,0,0,0,0
//   };
  //__ctype_arr;
#endif  
  
#if 1

#ifdef   _CRTIMP
#define DLLIMPORT 
  //_CRTIMP
  //__attribute__((dllimport))
#else

#define DLLIMPORT

#endif
  
  
  static int tbl_days[]=
  {
   0,                       // Jan
   31,                      // Feb
   31+28,                   // Mar
   31+28+31,                // Apr
   31+28+31+30,             // May
   31+28+31+30+31,          // Jun
   31+28+31+30+31+30,       // Jul
   31+28+31+30+31+30+31,    // Aug
   31+28+31+30+31+30+31+31,  // Sep
   31+28+31+30+31+30+31+31+30,  // Oct
   31+28+31+30+31+30+31+31+30+31,  // Nov
   31+28+31+30+31+30+31+31+30+31+30  // Dec
  };
  
struct tm *gmtime_r(const time_t *timep, struct tm *result)
{
   union{   
   FILETIME FileTime;
   long long  tt;
   };
   SYSTEMTIME   SystemTime;
   if(result)
   { 

        tt= (10000000ll * (*timep)) + 0x14F373BFDE04000ll;
            
        FileTimeToSystemTime( &FileTime,&SystemTime);
        
        result->tm_sec  = SystemTime. wSecond ;
        result->tm_min  = SystemTime. wMinute ;
        result->tm_hour = SystemTime. wHour ;
        result->tm_mday = SystemTime.wDay  ;
        result->tm_mon  = SystemTime. wMonth-1 ;
        result->tm_year = SystemTime. wYear - 1900;
        result->tm_wday = SystemTime. wDayOfWeek ;
        result->tm_yday = SystemTime.wDay +  tbl_days[result->tm_mon];
        if(result->tm_mon>1 && ! (result->tm_year&3) )result->tm_yday++;
    
        // result->tm_isdst = SystemTime. wHour>7;
   }
    return result;
};
int TZ_add_sec;
struct tm *localtime_r(const time_t *timep, struct tm *result)
{
  time_t mtime=*timep+TZ_add_sec;  
  return  gmtime_r(&mtime, result) ;
    
};
 
 int getuid(){return 100;};
 int geteuid(){return 100;};
 int getgid(void){return 100;};
 int getegid(void){return 100;};
 int tcgetattr(int fd, struct termios *termios_p){return 0;};
 int tcsetattr(int fd, int optional_actions,
                     const struct termios *termios_p){return 0;};
 int sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact){return 0;};    
 

 typedef void (*sighandler_t)(int);                    
 sighandler_t signal(int signum, sighandler_t handler)
 { return 0; };                     
 
 char* getenv(const char *name){return 0;};   
 DLLIMPORT int _cdecl isspace(int a){return (int) strchr(" \t\v\f\r\n",a); }
 DLLIMPORT int _cdecl isalnum(int a){ return (a>='A' && a<='Z') || (a>='a' && a<='z') || (a>='0' && a<='9') ; }
//  int __mingw_vsprintf(char *fmt,void **)
/* */                    
  int __mingw_vprintf(const char*fmt, char*l)
  {
    return Fprintf("OPENSSL:%s",fmt);    
  }
  int __mingw_vfprintf(FILE *f, const char*fmt, char*l)
  {
    return Fprintf("OPENSSL:%s",fmt);    
  }
  int __mingw_vsprintf(char *t,const char*fmt, char*l)
  {
      return wvsprintf(t,fmt,l);    
  }
  
// */ 
//static 
int PerSecond1E7=10000000l;
inline
void gettimeofday(struct timeval *x,...)
{
 SYSTEMTIME  stime;
 GetLocalTime(&stime);
 SystemTimeToFileTime(&stime, (FILETIME *)x);

//debug("TIME %X %X",x->tv_sec,x->tv_usec);
 
 asm volatile(
    " subl  $0xFDE04000 ,%%eax \n"
    " sbbl  $0x14F373B ,%%edx\n"
    "        cmpl  _PerSecond1E7,%%edx\n"
    "        jae   1f\n"
    "        divl _PerSecond1E7\n"
    "        imul $51,%%edx\n"
    "        shrl $9,%%edx\n"
    "       1:\n"
     :"=&a"(x->tv_sec),"=&d"(x->tv_usec)
      :"0"(x->tv_sec),"1"(x->tv_usec)
    );

}
#if defined(_USE_32BIT_TIME_T)
#define time _time32
#endif
time_t __cdecl time(time_t* y)
{
 struct timeval x;
 gettimeofday(&x);
 if(y)*y=x.tv_sec;
 return x.tv_sec;
}


#endif
// void   OPENSSL_cpuid_setup(){};
 
 
//#undef ERR_print_errors
// void ERR_print_errors(BIO *bp)
// { (Fprintf)("OpenSSL error");}
//#define ERR_print_errors(a) (Fprintf)("OpenSSL error")
//*
void ERR_put_error1(int lib, int func,int reason,const char *file,int line)
{ 
    
    (Fprintf)("OpenSSL error: lib=%d func=%d reason=%d %s line=%d ",lib,func,reason,file?file:"*",line);
   // DebugPrintFnc("OpenSSL error: lib=%d func=%d reason=%d %s line=%d\n",lib,func,reason,file?file:"*",line);
    
};
// */

int shs_print_cb(const char *str, size_t len, void *bp)
{
  //DebugPrintFnc("OpenSSL error:%s\n",str);  
  return  (Fprintf)("OpenSSL error:%s",str);
}                     
#endif

int SecAccept(struct OpenSSLConnection *s)
{
 // SSL_CIPHER *c;
  int r;
  DBG_STEP()
/*
  if(act_con_cnt<MAX_CON)
  {
    con_time[act_con_cnt]=time(0);
  }
*/
  act_con_cnt++;

//  SSL_CTX_flush_sessions(ctx,time(0));
  
  if ((s->con=SSL_new(ctx)) == NULL)
  {
   DBG_STEP()  
   return 0;
  }
  
//  (Fprintf)("SSL Accept %X",s->con);
  SSL_clear(s->con);
   
//  if(!(buf=malloc(BFRSIZE)))return(0);

//  s->io=BIO_new(BIO_f_buffer());
//  BIO_set_write_buffer_size(s->io,BFRSIZE);

  s->ssl_bio=BIO_new(BIO_f_ssl());
  
#ifdef  OSSL111
  s->sbio=BIO_new(srv_meth);
  BIO_set_data(s->sbio,s->CallbackParam);
  BIO_set_init(s->sbio, 1);
#else  
  s->sbio=BIO_new(&srv_meth);

  s->sbio->ptr=s->CallbackParam;
  s->sbio->init=1;
  s->sbio->flags=0;
#endif  
  SSL_set_bio(s->con,s->sbio,s->sbio);
  SSL_set_accept_state(s->con);
  BIO_set_ssl(s->ssl_bio,s->con,BIO_CLOSE);
#if 0  
  if( (r=SSL_do_handshake( s->con ) ) <0 )
  {
     SecClose(s);
     if(!act_con_cnt)
     {
        (Fprintf)("TLS handshake error %d %d reinit",r,SSL_get_error(s->con,r) );
        REinitCTX();    
       
     }
     else
     {
       (Fprintf)("TLS handshake error %d %d ; Now opened  %d TLS thread.  Reinit need %d",r,SSL_get_error(s->con,r),act_con_cnt,       reinit_ctx_need );
       reinit_ctx_need++;    
       if(reinit_ctx_need==1)  
       {
         reinit_ctx_need_time=time(0)+30;
         
       }
       else
       {
                if(reinit_ctx_need_time < time(0) )
                {
                    REinitCTX();    
                }    
       }    
     }    
     return 0; 
  }
#endif

  DBG_STEP()   
//  BIO_push(s->io,s->ssl_bio);
  return 1;
};

int SecRecv(struct OpenSSLConnection *s,char *b,int l)
{
 DBG_STEP()
 return BIO_read(s->ssl_bio,b,l);
}
int SecSend(struct OpenSSLConnection *s,char *b,int l)
{
 DBG_STEP()   
 return BIO_write(s->ssl_bio,b,l);
}
//return BIO_write(s->io,b,l);}
int SecClose(struct OpenSSLConnection *s)
{
// BIO_flush(s->io);
 
 DBG_STEP()   
 BIO_flush(s->ssl_bio);
 DBG_STEP()   
 if(!SSL_get_current_cipher(s->con))
 {
   Fprintf("TLS no chipper. Reinit CTX need");  
   reinit_ctx_need++;    
 } 
 else reinit_ctx_need=0;
 
// BIO_free(s->ssl_bio);
// BIO_free(s->io);
// BIO_free_all(s->io);
 SSL_free (s->con);
// BIO_free(s->sbio);
 if( (--act_con_cnt) < 0)act_con_cnt=0;
 if(reinit_ctx_need  )
 {
     if((!act_con_cnt))
     {    
   
       REinitCTX();    
     }
     else
     {
       (Fprintf)("TLS handshake error ; Now opened  %d TLS thread.  Reinit need %d",act_con_cnt,       reinit_ctx_need );
       if(reinit_ctx_need==1)  
       {
         reinit_ctx_need_time=time(0)+30;
         
       }
       else
       {
                if(reinit_ctx_need > 2 && reinit_ctx_need_time < time(0) )
                {
                    (Fprintf)("TLS reinit " );
                    REinitCTX();    
                }    
       }    
     }
 }    
 
}


#ifdef __cplusplus
}
#endif


