/*****************
* Copyright (c) 1995 Process Software Corporation
* Copyright (c) 1995 Microsoft Corporation
* Module Name : HttpExt.h
* Abstract : This module contains the structure definitions and
* prototypes for the HTTP Server Extension interface.
******************/
// Edited by Max Feoktistov for Small HTTP server project.  
// Copyright (C) 1999-2020 Maksim Feoktistov.

#ifndef _HTTPEXT_H
#define _HTTPEXT_H
#define HSE_VERSION_MAJOR 1 // major version number of this spec
#define HSE_VERSION_MINOR 0 // minor version number of this spec
#define HSE_LOG_BUFFER_LEN 80
#define MAX_EXT_DLL_NAME_LEN 256

#ifdef SYSUNIX


typedef void *  LPVOID;
typedef char *  LPSTR;
typedef char *  LPTR;
typedef uchar * LPBYTE;
typedef ulong * LPDWORD;

#undef BOOL
#define BOOL int

#undef CHAR
#define CHAR char

#undef  DWORD
#define DWORD unsigned long

#define WINAPI
struct Req;

#define TRUE 1
#define FALSE 0

#endif

typedef LPVOID HCONN;
// status codes returned by the Extension DLL
#define HSE_STATUS_SUCCESS 1
#define HSE_STATUS_SUCCESS_AND_KEEP_CONN 2
#define HSE_STATUS_STATUS_PENDING 3
#define HSE_STATUS_STATUS_ERROR 4
// request services values for ServerSupportFunction.
// Values from 0 to 1000 are reserved for future versions
#define HSE_REQ_BASE 0
#define HSE_REQ_SEND_URL_REDIRECT_RESP ( HSE_REQ_BASE + 1 )
#define HSE_REQ_SEND_URL ( HSE_REQ_BASE + 2 )
#define HSE_REQ_SEND_RESPONSE_HEADER ( HSE_REQ_BASE + 3 )
#define HSE_REQ_DONE_WITH_SESSION ( HSE_REQ_BASE + 4 )
#define HSE_REQ_END_RESERVED 1000

// passed to GetExtensionVersion
typedef struct _HSE_VERSION_INFO {
 DWORD dwExtensionVersion; // Hiword = Major
 // Loword = minor
 char lpszExtensionDesc[MAX_EXT_DLL_NAME_LEN];
 // Null terminated name of max 256 chars
} HSE_VERSION_INFO, *LPHSE_VERSION_INFO;

// passed to extension procedure on a new request
typedef  BOOL ( *TGetServerVariable)( HCONN hConn,LPSTR lpszVariableName,LPVOID lpvBuffer,LPDWORD lpdwSize ) WINAPI;
typedef  BOOL ( *TWriteClient)( HCONN ConnID,LPVOID Buffer,LPDWORD lpdwBytes,DWORD dwReserved ) WINAPI;
typedef  BOOL ( *TReadClient)( HCONN ConnID,LPVOID lpvBuffer,LPDWORD lpdwSize ) WINAPI;
typedef  BOOL ( *TServerSupportFunction)( HCONN hConn,DWORD dwHSERRequest,LPVOID lpvBuffer,LPDWORD lpdwSize,LPDWORD lpdwDataType ) WINAPI;

typedef struct _EXTENSION_CONTROL_BLOCK
{
 DWORD cbSize; // size of this struct.
 DWORD dwVersion ; // version info of this spec
 HCONN ConnID; // Context number DO NOT CHANGE!
 DWORD dwHttpStatusCode; // HTTP Status code
 char lpszLogData[HSE_LOG_BUFFER_LEN];
 // null-terminated log info specific
 // to this Extension DLL
 LPSTR lpszMethod; // REQUEST_METHOD
 LPSTR lpszQuery_string; // QUERY_STRING
 LPSTR lpszPathInfo; // PATH_INFO
 LPSTR lpszPathTranslated; // PATH_TRANSLATED
 DWORD cbTotalBytes; // Total bytes available
 DWORD cbAvailable; // available number of bytes
 LPBYTE lpbData; // pointer to client data of
 // cbAvailable number of bytes
 LPSTR lpszContentType; // Content data type
 TGetServerVariable     GetServerVariable;
 TWriteClient           WriteClient;
 TReadClient            ReadClient;
 TServerSupportFunction ServerSupportFunction;

//--- My extention-- (M.F.)
 Req *sock;
// int sock;
 int state;
 char **hv;
 char *snm;
 char *bf;

} EXTENSION_CONTROL_BLOCK, *LPEXTENSION_CONTROL_BLOCK;
//
// these prototypes must be exported from the extension DLL
//
BOOL WINAPI GetExtensionVersion( HSE_VERSION_INFO *pVer );
DWORD WINAPI HttpExtensionProc( EXTENSION_CONTROL_BLOCK *pECB );
// type declarations for the server side
typedef int (* PFN_GETEXTENSIONVERSION)( HSE_VERSION_INFO *pVer )  WINAPI ;
typedef int (* PFN_HTTPEXTENSIONPROC )( EXTENSION_CONTROL_BLOCK *pECB) WINAPI ;
typedef int (* PFN_DllRegisterServer )() WINAPI ;
#endif // end definition _HTTPEXT_H_
// the following additional functions are specific to this WEB
#define HSE_GET_COUNTER_FOR_GET_METHOD 1001
#define HSE_GET_COUNTER_FOR_POST_METHOD 1002
#define HSE_GET_COUNTER_FOR_HEAD_METHOD 1003
#define HSE_GET_COUNTER_FOR_ALL_METHODS 1004
/***
dwHSERequest = HSE_GET_COUNTER_FOR_GET_METHOD
lpvBuffer --- contains a null terminated string of the server
name as returned by the CGI variable SERVER_NAME.
lpdwSize --- size of the string pointed by lpvBuffer
lpdwDataType -- contains a DWORD of the total number of GET
methods served by the server mentioned in lpvBuffer.
dwHSERequest = HSE_GET_COUNTER_FOR_GET_METHOD
Exactly same as above except that this function returns
the number of POST methods serviced by the server mentioned in
lpvBuffer.
dwHSERequest = HSE_GET_COUNTER_FOR_HEAD_METHOD
Exactly same as above except that this function returns
the number of HEAD methods serviced by the server mentioned in
lpvBuffer.
dwHSERequest = HSE_GET_COUNTER_FOR_ALL_METHODS
Exactly same as above except that this function returns
the total number of requests serviced by the server mentioned in
lpvBuffer.
*****/
