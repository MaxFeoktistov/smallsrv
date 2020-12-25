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


extern "C" {
struct OpenSSLConnection
{int state;
 void *CallbackParam;
 void *con;
 void *io,*ssl_bio,*sbio;
};

typedef int (* TFprintf)(char *param,...);
typedef int (* TFtransfer)(void *param,void *bfr,int l);


int InitLib( TFprintf prnt,TFtransfer fsend,TFtransfer frecv,char *CApath,char *CAfile,
  char * s_cert_file,
  char * s_key_file
// ,char * s_dcert_file,
//  char * s_dkey_file
//  ,int s_server_verify
);

#ifdef  TLSWODLL
int InitLib( TFprintf prnt,TFtransfer fsend,TFtransfer frecv,
  char *CApath,char *CAfile,
  char * s_cert_file,char * s_key_file);
int SecAccept(OpenSSLConnection *s);
int SecRecv  (OpenSSLConnection *s,char *b,int l);
int SecSend  (OpenSSLConnection *s,char *b,int l);
int SecClose (OpenSSLConnection *s);
#else

extern int (*PInitLib)( TFprintf prnt,TFtransfer fsend,TFtransfer frecv,
         char *CApath,char *CAfile, char * s_cert_file,char * s_key_file);
extern int (*PSecAccept)(OpenSSLConnection *s);
extern int (*PSecRecv  )(OpenSSLConnection *s,char *b,int l);
extern int (*PSecSend  )(OpenSSLConnection *s,char *b,int l);
extern int (*PSecClose )(OpenSSLConnection *s);

typedef int (*TSecAccept)(OpenSSLConnection *s);
typedef int (*TSecRecv  )(OpenSSLConnection *s,char *b,int l);
typedef int (*TSecSend  )(OpenSSLConnection *s,char *b,int l);
typedef int (*TSecClose )(OpenSSLConnection *s);
typedef int (*TSetPriority )(char *s);
typedef int (*TInitLib  )( TFprintf prnt,TFtransfer fsend,TFtransfer frecv,
  char *CApath,char *CAfile, char * s_cert_file,char * s_key_file);


#define  SecAccept (PSecAccept)
#define  SecRecv   (PSecRecv  )
#define  SecSend   (PSecSend  )
#define  SecClose  (PSecClose )
#define  InitLib   (PInitLib  )

#endif


}


