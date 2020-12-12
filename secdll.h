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


#endif

