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


#ifndef SRV_H
#include "srv.h"
#endif

#include <pty.h>

#if defined(TELNET) || ! defined(ONEFILE)

//char *telnet="/bin/sh";
ulong tlnt_timeout=100000;
#define TELNET_BFR_SIZE 512
int DONT_USE_PTY;

uchar echo_cmd[]={
  //0xFF, 0xFD, 0x2D
        0xFF,0xFB,0x03,                 /* IAC WILL Suppress Go Ahead */
        0xFF,0xFD,0x03,                 /* IAC DO Suppress Go Ahead */
        0xFF,0xFB,0x01,                 /* IAC WILL Echo */
        0xFF,0xFE,0x01,                 /* IAC DON'T Echo */
	//0xFF,0xFA,24,0,'V','T','1','0','0',0xFF,240, //	IAC	SB	Terminal Type	0	V	T	2	2	0	IAC	SE
        0xA,0xD
};
uchar term_cmd[]={
//	0xFF,0xFA,24,0,'V','T','1','0','0',0xFF,240, //	IAC	SB	Terminal Type	0	V	T	2	2	0	IAC	SE
	0xFF,0xFA,24,0,'V','T','1','0','0',0xFF,240 //	IAC	SB	Terminal Type	0	V	T	2	2	0	IAC	SE
};
/*
struct termios init_termp=
{
  
};

struct winsize winp=
{

};
*/
int edup(int od,int nd)
{
  debug("dup2 err %d",errno);
  close(nd);
  return dup(od);
}
// #define AddToLog(a...)
int Req::TReq()
{
 int phrd[2],phrdp[2],//phrde[2],
   child_pid,i,j;
 #define  hrd  (phrd[0])
 #define  hwr  (phrd[1])
// #define  hrde (phrde[0])
// #define  hwre (phrde[1])
 #define  hrdp (phrdp[0])
 #define  hwrp (phrdp[1])
 char bfr[TELNET_BFR_SIZE+20];
 char bfr2[TELNET_BFR_SIZE*2+20];
 union {
 char *argv[16];
 fd_set set;
 };
 timeval  TVal;
 char *p,*t,*e,*te;
 long ec,st_fl=0;
 long xtimlim =tlnt_timeout;

  AddToLog("Telnet income..\r\n",s,FmtShrtR);
  if( (!DONT_USE_PTY) && !openpty(phrd, phrd+1, 0,
                0,//   const struct termios *termp,
                0//   const struct winsize *winp
			    )
  )
 {
   phrdp[0]=phrd[1];
   phrdp[1]=phrd[0];
 }
 else
 {
    debug("Can't open pty;  using pipe...\r\n");
    if( pipe(phrdp) || pipe(phrd) ) //|| pipe(phrde) )
    {
    er000:;
      return HttpReturnError("Error. Can\'t run script\r\n");
    }
 }
  debug("pipes:%d %d %d %d\n",hrd,hwr,hrdp,hwrp );
  if( !(child_pid=vfork()) )
  {
    // Child
//    debug("Run %s %X\n",loc,req);
   if(hrdp)if(dup2(hrdp,0)<0)
   {
     edup(hrdp,0);
   }  ;
   if(hwr!=2)if(dup2(hwr,2)<0)
   {
     edup(hwr,2);
   }  ;
   if(hwr!=1)if(dup2(2,1)<0)
   {
     edup(hwr,1);
   }  ;

   /*
   close(hrd );
   close(hwr );

   if(hwrp!=hrd)
   {  
   //close(hrde);
    close(hwrp);
   //close(hwre);
    close(hrdp);
   }
  // */
  
   strncpy(bfr,tel_cmd,sizeof(bfr));
   i=0;
   p=bfr;

   do{
    argv[i++]=p;
    if(!(p=strchr(p,' ')))break;
    *p++=0;
    p=SkipSpace(p);
   }while(*p && i<16);
   argv[i]=0;
   printf("run %s\n",argv[0]); //,argv[1],argv[2]);
   execv(bfr,argv);
   printf("Can't exec %s\n",bfr);

   //execl(telnet,telnet,0);
   exit(0);
  }
  else if (child_pid>0)
  {
   ec=0;
   Send(echo_cmd,sizeof(echo_cmd));

   close(hwr );
   if(hwr!=hrdp)
  // close(hwre);
     close(hrdp);

   do{
    FD_ZERO(&set);
    //FD_SET(hrde,&set);
    FD_SET(s,&set);
    FD_SET(hrd,&set);
    //j=hrde;
    //if(s>j)
      j=s;
    if(hrd>j)j=hrd;
    TVal.tv_sec=1; //tlnt_timeout;
    TVal.tv_usec=0;
    if( select(j+1,&set,0,0,&TVal)<=0 )
    {
      
     ec=waitpid(child_pid,(int *)&ec,WNOHANG);
     if( (!is_no_exit) || (ec) || --xtimlim<=0 )
     {  
      if(xtimlim<=0)HttpReturnError("Timeout\r\n");
    ex:;
      kill(child_pid,9);
      sleep(3);
      waitpid(child_pid,(int *)&ec,WNOHANG);
      break;
     }
     continue;
    }
    xtimlim=tlnt_timeout;

    if(FD_ISSET(s,&set))
    { j=Recv(bfr,TELNET_BFR_SIZE);
      if(j<=0)
      {dbg("Connection aborted..\r\n ");
       goto ex;
      }
      p=bfr;
      t=bfr2;
      e=bfr+j;
      te=t+TELNET_BFR_SIZE*2;
      while(p<e && t<te)
      {
	if(st_fl)
	{
          if(st_fl&8) //0xFF0000)
	  {
	    /*
	    switch(*p)
	    {
	      case 24:
	    }
	    */
	    if(*p==24)
	    {
	      if(st_fl==0xFB0008)
	      {
		Send(term_cmd,sizeof(term_cmd));
	      }	
	    }
	    st_fl&=~0xFF0008;
	  }
	  
	  if(st_fl&1)
	  {
	    st_fl&=~1;
	    switch(* (uchar *) p)
	    {
	      case 0xff: *t++=0xFF;   //st_fl=0; break;
	      case 0xf0: st_fl=0; break;
	      case 0xf7: if(t>bfr2)--t; else { if(t<te)*t++=8; } break;
	      case 0xfa: st_fl=0xC; break;
	      case 0xFB: // will
	      case 0xFC: // wont
	      case 0xFD: // do
	      case 0xFE: // dont
		st_fl|= (((ulong) * (uchar *) p )<<16)|8;
		break;
	      default: 	st_fl|=8;


	    }
	    
	  }
	  if(st_fl&4 && (*(uchar *)p==255) )
	  {
	    st_fl|=1;
	    
	  }  
	  
	}
	else if(*(uchar *)p==255)
	{
	  debug("IAC %X %X %X %X %X\n",((uchar *)p)[1],((uchar *)p)[2],((uchar *)p)[3],((uchar *)p)[4],((uchar *)p)[5]);
	  //++p;
	  st_fl|=1;
	}
	//*
	else if( (*p=='\n' || *p=='\r'  ) && ( t<te ) )
	{
	  //*t++='\r';
	  *t++='\n';
	} //  */
	else
	  *t++=*p;

	++p;

	
      }
      j=(t-bfr2);
      write(hwrp,bfr2,j);
      //AddToLog("Telnet income..\r\n",s,FmtShrt);
      //if(s_flg&FL_FULLLOG)
      {
	if(j>64)
	{
	  DWORD_PTR(bfr2[64])=0x2E2E2E;
	}  
	else bfr2[j]=0;
	AddToLog(bfr2,s,FmtShortR);
      }

    }

    i=hrd;
   //do{
    if(FD_ISSET(i,&set))
    {
      j=read(i,bfr,TELNET_BFR_SIZE);
      if(j<=0)goto ex;
      p=bfr;
      t=bfr2;
      e=bfr+j;
      te=t+TELNET_BFR_SIZE*2;
      while(p<e && t<te)
      {
	if(hrd!=hwrp && (*p=='\r' || *p=='\n' )  )
	{
	  *t++='\r';
	  *t++='\n';
	}
	else *t++=*p;

	++p;
      }
      j=(t-bfr2);
      Send(bfr2,j);
	if(j>64)
	{
	  DWORD_PTR(bfr[64])=0x2E2E2E;
	}
	else bfr[j]=0;
	AddToLog(bfr,s,FmtShort);
    }
    //if(i==hrde)break;
    //i=hrde;
      //xchg(hrd,hrde);
   //}while(1);
    
    if(j!=TELNET_BFR_SIZE)ec=waitpid(child_pid,(int *)&ec,WNOHANG);
   }while(is_no_exit && !ec);
   if(!ec)
   {ec=waitpid(child_pid,(int *)&ec,WNOHANG);
    kill(child_pid,9);
   }

  }
  else
  {
    debug("can't vfork %d",errno);
  }

 close(hwrp);
 //close(hrdp);
 //close(hwr);
 if(hwrp!=hrd) close(hrd);
 //close(hwre);
 //close(hrde);
 AddToLog("End telnet sesion..\r\n",s,FmtShrt);
 return 1;

 #undef  hrd
 #undef  hwr
 #undef  hrdp
 #undef  hwrp

};


#endif

