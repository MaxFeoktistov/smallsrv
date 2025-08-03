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


#ifndef _SECDLL_H_
#define _SECDLL_H_

typedef struct _SecConnection
{int state;
 void *CallbackParam;   // Callback param for F_send, F_recv functions
 void *con;
 void *io,*ssl_bio,*sbio;
} SecConnection;

#ifndef  STDCALL
#define STDCALL
#endif

int STDCALL InitLib(
   int  (* F_printf)(char *fmt,...), // Callback to printf like C printf
   int  (* F_send)(void  *param,void  *bfr,int len), // Callback function for send
   int  (* F_recv)(void  *param,void  *bfr,int len), // Callback function for receive
   char  *CApath,       // patch for  CA-files
   char *CAfile,        // CAfile
   char * s_cert_file,  // cert file
   char * s_key_file    // key file
   ); // This function return 1 if success, 0 if fail

int STDCALL SecAccept(SecConnection *s);  // In this function DLL
// allocate memory for new connection  This function return 1 if success, 0 if fail.


int STDCALL SecRecv  (SecConnection *s,char *bfr,int l); // This function
// encode bfr data and send it to user trough F_send callback.
// s->callback must be set to param for F_send, F_recv functions
// Function return nuber of sent bytes , 0 if connection closed, negative if error.
int STDCALL SecSend  (SecConnection *s,char *bfr,int l); // This function
// receive data  trough F_recv callback decode it to the bfr.
// Function return number of bytes placed to the bfr, 0 if connection closed, negative if error.
int STDCALL SecClose (SecConnection *s );
//This function release memory allocated for connection.

int STDCALL SecConnect(OpenSSLConnection *s, int flags, char *verfyhost); // This function prepare client TLS connection
// Flags:
#define  tbtAnon    0x1            /* Don't use server certificate */
#define  tbtVerfyRequired 0x4      /* Verify hostname */
#define  tbtDontVerfyTyme 0x8      /* GNUTLS only. Don't verify timestamp */
#define  tbtDontVerfySigner 0x10   /* GNUTLS only. Don't verify issuer */
#define  tbtSSHstyleVerfy  0x20    /* GNUTLS only. SSH style. First time new host will be valid, and public key will be stored */
// verfyhost must be NULL if don't verfy, or name of remote host for verification.
// This function return 1 if success, 0 if fail.

int STDCALL SecUpdateCB(OpenSSLConnection *s); // Function that must be call when address of caller object changed.


#endif

