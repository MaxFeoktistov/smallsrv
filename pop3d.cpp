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

#ifdef SEPLOG

#undef debug
#undef AddToLog

#define debug(a...)  sepLog[4]->Ldebug(a)
#define AddToLog(a...)  sepLog[4]->LAddToLog(a)

#endif




int Req::POPReq()
{char bfr[520],pth[512],subdir[128],tmp[80];
 WIN32_FIND_DATA fnds;
 User *puser;
 ulong mboxLen,mboxCount;
 char *t,*d,*tt,*p,*sf;
 HANDLE hdl;
 int h,i,j,n,l;
 char *rcode;
 int rcodeLen;
 char *bb=0;
 PopName *pn=0;
 int cmd=0,status=0;
 OpenSSLConnection otls;

#undef SendCMD
#undef SendConstCMD
#define SendCMD(xxb,xxl) rcode=xxb; rcodeLen=xxl;
#define SendConstCMD(b)  rcode=b; rcodeLen=sizeof(b)-1;
#ifdef WITHMD5
 struct timeval tpwd;
    if(  ! (s_flgs[1]&FL1_CRYPTPWD) )
    {    
          gettimeofday(&tpwd,0);
          rcodeLen=sprintf(rcode=bfr,"+OK POP3 server ready <%u.%u@%.64s>\r\n",tpwd.tv_sec,tpwd.tv_usec,smtp_name?smtp_name:"shs");
    }
    else
#endif 
   { SendConstCMD( "+OK POP3 server ready\r\n"); }
 timout=POPTimeout;
//  debug("!POP:%s\n",rcode) ;    

 do{
//  debug("POP:%s\n",rcode) ;    
  if(Send(rcode,rcodeLen)<0)
  {dbg("Can\'t send..");goto ex1;};
  if(s_flg&FL_FULLLOG)AddToLog(rcode,s,FmtShrt);
  if(cmd==0x74697571)break;
 lcnt:
  if((cmd=RGetCMD(bfr))<0) goto ex1;
  if( (! (status&1)) && cmd!=0x72657375 x4CHAR("user")
      && cmd!=0x61706163 x4CHAR("capa")
      && cmd!=0x73736170 x4CHAR("pass")
#ifdef WITHMD5
      && cmd!=0x706F7061 x4CHAR("apop")  
#endif      
      && cmd!=0x736C7473 x4CHAR("stls"))goto lerr;
  tt=SkipSpace(bfr+4);
  switch(cmd)
  {
   case 0x61706163 x4CHAR("capa"):
/*       
    if(s_aflg&AFL_TLS)
    {
      SendConstCMD( "+OK\r\nUSER\r\nUIDL\r\nLIST\r\nSTLS\r\nTOP\r\n.\r\n");
    }
    else
    {
     SendConstCMD( "+OK\r\nUSER\r\nUIDL\r\nLIST\r\nTOP\r\n.\r\n");
    }
*/

   rcodeLen=sprintf(rcode=bfr,"+OK\r\nUSER\r\nUIDL\r\nLIST\r\nTOP\r\n");
   if(s_aflg&AFL_TLS)
      rcodeLen+=sprintf(rcode+rcodeLen,"STLS\r\n");
#ifdef WITHMD5
   if(  ! (s_flgs[1]&FL1_CRYPTPWD) )
   {    
      rcodeLen+=sprintf(rcode+rcodeLen,"APOP\r\n");
   }
      
#endif      
   rcodeLen+=sprintf(rcode+rcodeLen,".\r\n");

   break;
#ifdef WITHMD5
   case 0x706F7061 x4CHAR("apop") :
    if(  ! (s_flgs[1]&FL1_CRYPTPWD) )
    {    
       t=strpbrk(tt," \t");
       if(!t)    goto lerr;      
       *t++=0;    
       if(!CheckBadName(tt))goto lerr;
       t=SkipSpace(t);
       postsize=sprintf(pst=pth,"<%u.%u@%.128s>",tpwd.tv_sec,tpwd.tv_usec,smtp_name);
       
       if((puser=FindUser(tt,UserPOP3|FindUserMD5digest,t,this))) goto lbAuthOk;
        
    }
    goto lerr;      
#endif      
   case 0x736C7473 x4CHAR("stls"):
    if(s_aflg&AFL_TLS)
    {Send("+OK\r\n",sizeof("+OK\r\n")-1);
     debug("**%snit TLS connection",TLSBegin(&otls)?"I":"Error to i");
     goto lcnt;
    }
    goto lerr;
   case 0x72657375 x4CHAR("user")://22:
    strncpy(pth,tt,128);
    lok:
   case 0x74697571 x4CHAR("quit")://12:
   case 0x706F6F6E x4CHAR("noop")://11:
    SendConstCMD( "+OK\r\n");
    break;
   case 0x73736170 x4CHAR("pass")://17: //Pasword
    subdir[0]=0;
    if( (sf=strpbrk(pth,"@#/") ) )
    {
      if(*sf=='/')
      {
        *sf++=0;
        if(!CheckBadName(sf))goto lerr;
        sprintf(subdir,"%.120s" FSLUSHS ,sf);
      }
      /*
      else if(*sf!='@' || !FindUser(pth,UserPOP3,0,0)) )
      {
       sf=0;
      }*/
      else sf=0;
    }
    if((puser=FindUser(pth,UserPOP3,tt,this)))
    {
   lbAuthOk:     
     status|=1;
     if(IsInIPRange(smtp_range)<=0) //,htonl(sa_c.sin_addr. S_ADDR)))) //saddr[ntsk]))))
     {POP3usr[iPOP3usr]=sa_c.sin_addr. S_ADDR; //saddr[ntsk];
      iPOP3usr=(++iPOP3usr)&0xF;
      POP3usrTime=GetTickCount();
     }
     if(fl&F_PRX)
     {if((h=CallUp(puser))<0)goto ex1x;
      pn= (PopName *) new char[0x8800];
      p2p(h,(char *)pn);
      //shutdown(h,2);
      //closesocket(h);
      CloseSocket(h);
      sprintf((char *)pn,"Proxy in:%u out:%u %s\r\n",Tin,Tout,puser->name);
      AddToLog((char *)pn,s,FmtShort);
      goto ex1;
     }
      bfr[0]=0;
     // if(sf) {    sprintf(bfr, "%.255s" FSLUSHS,sf);    }
     sprintf(pth,"%.255s" FSLUSHS "mbox" FSLUSHS "%s*.msg",d=dir,subdir);
     pn= new PopName[0x3028];
     mboxLen=0;
     mboxCount=0;
     if( (hdl=FindFirstFile(pth,&fnds))!=INVALID_HANDLE_VALUE )
     {do{
       if(fnds.nFileSizeLow && !(fnds.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
       {if(fnds.cFileName[8]=='.')
        {pn[mboxCount].name=atouix(fnds.cFileName);
         pn[mboxCount].size=fnds.nFileSizeLow;
         mboxLen+=fnds.nFileSizeLow;
         if((++mboxCount)>=0x3000)break;
        }
       }
      }while(FindNextFile(hdl,&fnds));
      FindClose(hdl);
     }
     if(mboxCount<0x2000)bb=(char *) (pn+mboxCount+2);
     else bb=new char[0x8100];
     sprintf(bb,"POP: login in %.20s;\r\n",puser->name);
     AddToLog(bb,s);
   case 0x74617473 x4CHAR("stat")://19:
     rcodeLen=sprintf(rcode=bb,"+OK %u %u\r\n",mboxCount,mboxLen);
   }
   else
   {ex1x: cmd=0x74697571;
  lerr:;
   default:
     SendConstCMD( "-ERR\r\n");
   }
   break;
  case 0x6C646975 x4CHAR("uidl")://21:
  case 0x7473696C x4CHAR("list")://16:
   if(bfr[4]==' ')
   {if( ((i=atoui(tt))>mboxCount) || !i )goto lerr;
    rcodeLen=sprintf(rcode=bb,"+OK %u %u\r\n",i,(cmd==0x7473696C x4CHAR("list"))?pn[i-1].size:pn[i-1].name);
   }else
   {rcodeLen=sprintf(rcode=bb,"+OK %u %u\r\n",mboxCount,mboxLen);
    for(i=0;i<mboxCount;++i)if( (rcodeLen+=sprintf(bb+rcodeLen,
       "%u %u\r\n",i+1,(cmd==0x7473696C x4CHAR("list"))?pn[i].size:pn[i].name))>0x7C00)break;
    DWORD_PTR(bb[rcodeLen])=0x0A0D2E;  //".\r\n"
    rcodeLen+=3;
   }
   break;
  case 0x20706F74 x4CHAR("top ")://20:
  // --tt;
  case 0x656C6564 x4CHAR("dele")://15:
  case 0x72746572 x4CHAR("retr")://18:
   if(((i=atouis(tt))>mboxCount) || !i)goto lerr;
   sprintf(bb,"%.255s" FSLUSHS "mbox" FSLUSHS "%s%8.8X.msg",d,subdir,pn[i-1].name );
   if(cmd==0x656C6564 x4CHAR("dele"))
   {DeleteFile(bb);
    goto lok;
   }
   if((h=_lopen(bb,0))<=0)
   {
    debug("Can't open %s %s",bb,strerror(errno));

    goto lerr;
   }
   l=rcodeLen=sprintf(bb,"+OK %u    bytes for %s\r\n",pn[i-1].size,puser->name);
   do{
    rcodeLen+=i=_hread(h,t=bb+rcodeLen,0x8000);
    if(cmd==0x20706F74 )
    {j=atoui(tt+1);
     DWORD_PTR(t[i])=0;
     if( (tt=strstr(t,"\r\n\r\n") ) )
     {tt+=4;
      while((--j)>=0 )
      {if(! ((tt=strstr(tt,"\r\n"))) ){tt=t+i; break;}
       tt+=2;
      }
     }else tt=t+i;
     rcodeLen=tt-bb;
     bb[(i=sprintf(bb+4,"%u   ",tt-t))+4]=' ';
    }
    if(i<0x8000)
    {if(WORD_PTR(bb[rcodeLen-2])!=0x0A0D)
     {WORD_PTR(bb[rcodeLen])=0x0A0D;
      rcodeLen+=2;
     }
     DWORD_PTR(bb[rcodeLen])=0x0A0D2E;
     rcodeLen+=3;
    }
    if(Send(bb,rcodeLen)<=0)goto ex1;
    if(l){bb[l]=0; l=0; AddToLog(bb,s,FmtShrt);}
    rcodeLen=0;
   }while(i==0x8000);
   _lclose(h);
   goto lcnt;
  };
 }while(1);
 ex1:;
  if(Snd==&TLSSend)SecClose(&otls);
  if(pn)
  {if(mboxCount>=0x2000)delete bb;
   delete pn;
  }
  return 0;
};


