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



#define ADBG(a...)
//printf(a)
//debug(a)
//#define ALLOC malloc

const char DEF_IP_BASE_FN[] = "ipbase.bin";

char * ipbsfile=(char *)DEF_IP_BASE_FN;
/*
struct CntrCode{
 char nm[2];
 ushort ind;
 ulong  cnt;
 ulong  ip[1];

 CntrCode *Next(){ return (CntrCode *) (ip+cnt) ;}
 ulong FastFind(ulong s);


} PACKED;

*/

CntrCode  *cntr_dat,*cntr_dat_last;

ulong CntrCode::FastFind(ulong s)
{
 uint *first, *last,*mid,f,ff,l,ll,m,c,k;
  first=ip;
  last=ip+cnt-1;

  do{
   f=*first & ~0x1F;
//   debug("f=%X ",f);
   if(f>s)return 0;
   ff=f+(1<<(*first & 0x1F));
   if(s<ff)return f; //*first;
   ll=(l=*last & ~0x1F) + (1<<(*last & 0x1F) );
//   debug("ff=%X l=%X ll=%X s=%X c=%u\n",ff,l,ll,s,c);
   if(ll<s)return 0;
   if(s>l)return l;  // *last;
   c=(last-first)>>1;
   if(!c)return 0;
   mid=first+c;
   m=*mid & ~0x1F;
   if(s<=m)
   {
     last=mid;
   }
   else
   {
     first=mid;
   }
  }while(1);
}


CntrCode  * FindCntr(ulong ip)
{
 ulong best=0,b,db=0xFFFFFFFF,k;
 CntrCode  *p,*bp=0;
 for(p=cntr_dat;p<cntr_dat_last; p=p->Next() )
 {//debug("%2.2s %u %u\n",p->nm,p->ind,p->cnt);
  if( (b=p->FastFind(ip)) )
  {
   if((k=ip-b)<254)return p;
   if(db>k)
   { db=k;
     best=b;
     bp=p;
   }
  }
 }
 return bp;
}


struct CntrFile{
 uint sign;
 uint chk;
 uint cnt;
 uint offs;
 char names[1];


} PACKED ;

CntrFile *cntr_file;
char ** country_names;
void InitContries(CntrFile * m)
{
 int i;
 char *p=m->names;
 country_names= (char **) ALLOC( (m->cnt +4) * sizeof(void *)  );
 ADBG("m->cnt=%u\n",m->cnt);
 if(!country_names)
 {
   ADBG("Cant alloc %u %u for country_names\n",(m->cnt +4) * sizeof(void *) ,m->cnt );  
   return ;    
 }    
 m->cnt++;
 for(i=0; i< m->cnt ;  )
 {
  country_names[i++]=p;
  while(*p++);
 }
 //*
 if((p-m->names)!=m->offs)
 {
//   debug("Loading error! %u %u |%s| |%s|",(p-m->names),m->offs,country_names[0],country_names[i-1] );
   p=m->names+m->offs;
 }
// */
 cntr_dat=cntr_dat_last=(CntrCode*) p;

 CntrCode* x;

 for(i=0; i< m->cnt ; i++ )
 {
  ADBG("Country %2.2s %u %u \n",cntr_dat_last->nm, cntr_dat_last->ind, cntr_dat_last->cnt  );
  x=cntr_dat_last;
  cntr_dat_last=cntr_dat_last->Next();
 }
// debug("Country enumirate Ok %u Last=%s\n",x->ind, country_names[x->ind] );

}

#if 0
ndef FREEVER

int ProtBase(char *pkdat,int l)
{
 int i;
 ulong r,r1;
 char *p;
 r=0;
 r1=0;
 if(!user_name)return -1;
 p=user_name;
 l-=4;
 for(i=0;i<l;++i)
 {
  switch(*p&0x7)
  {
   case 0:  pkdat[i]^=*p; break;
   case 1:  pkdat[i]^=p[1]; break;
   case 2:  pkdat[i]^=r; break;
   case 3:  pkdat[i]^=r1; break;
   case 4:  pkdat[i]^=0x5A; break;
   case 5:  pkdat[i]^=0xAA; break;
   case 6:  pkdat[i]^=0x55; break;
  }
  r^=((uchar *)  pkdat)[i];
  r1+=((uchar *) pkdat)[i];
  ++p;
  if(!*p)p=user_name;
 }
 if(r1!=DWORD_PTR(pkdat[l]) )
 {
  debug("Bad file %X %X ",r1,DWORD_PTR(pkdat[l]));
/*
  i=_lcreat("xxx123",0);
  _lwrite(i,pkdat,l);
  _lclose(i);
// */
  return -1;
 }
 return 0;
}

#endif

int LoadContries()
{
 int h,l;
// struct stat st;
// CntrFile *cf;
 char *pkdat;
  if(cntr_file)return 1;
  if( !ipbsfile)
  {   
      return 0;
  }//  else 
  if((h=_lopen(ipbsfile,0))<0)
  {
   if( DEF_IP_BASE_FN != ipbsfile)debug("Can\'t open %s",ipbsfile);
   return 0;
  }
  //fstat(h,&st);
  //printf("Load begin st.st_size=%u\n",st.st_size);
  if(
     ((l=FileSize(h))<0x100) ||
     (!(pkdat=(char *) ALLOC(l+16) )
     )
    )
  {
//   printf("No memory cf=%X %d\n",cntr_file,GetLastError() );
   //exit(0);
 lbErr:
   debug("Error loading %s",ipbsfile);
   _lclose(h);
   return 0;
  }
  if( _hread(h,pkdat,l)<l) goto lbErr3;
  if(DWORD_PTR(pkdat[0]) == 0x4D6559)
  {
   cntr_file=(CntrFile *)pkdat;

  }
  else
  {
   ADBG("Bad sign %u\n",DWORD_PTR(pkdat[0]))   ;
   if(
#ifndef FREEVER
//     ProtBase(pkdat,l) ||
#endif
     (DWORD_PTR(pkdat[0]) != 0x4D6558) ||
     ! (cntr_file=(CntrFile *) ALLOC(DWORD_PTR(pkdat[4])+0x2008)
       )

   )
   {
   lbErr3:
    ADBG("Bad file, or no memory");
//   debug("Error loading %s, bad file %X",ipbsfile,DWORD_PTR(pkdat[0]));
   lbErr2:
    FREE(pkdat);
    goto lbErr;
   }
  // printf("Bunpack... %u\n",DWORD_PTR(pkdat[4]));
  
#if 0
//ndef NOTINTEL
   if(!LZZUnpk((uchar *)cntr_file,(uchar *)pkdat+8) )
   {
//   debug("IP base loading error");
    FREE(cntr_file);
    cntr_file=0;
    goto lbErr2;
   }
#endif
  }
  _lclose(h);

  ADBG("Init begin cf=%X\n",cntr_file);
  InitContries(cntr_file);
  debug("IP - country base loaded");
  return 1;
}




uint fFindCountry(StatLog *psl,char *bfr)
{
 CntrCode  *cc;
 char *t;


 if( cntr_dat &&
     http_port==psl->port
   )
 {
   t="unknown";
   if((cc=FindCntr(htonl(psl->ip))))
   {t=country_names[cc->ind];
   }
 }
 else return 0;
 sprintf(bfr,"%s",t);
 return 1; //bfr;
}

char *GetCntr(ulong ip)
{CntrCode  *cc;;
 if((cc=FindCntr(htonl(ip))))
 {return country_names[cc->ind];
 }
 return (char *) &NullLong;
}

int Req::IP2country()
{
  char *t,*tc,*l;
  ulong ip=0;  
  char *p,*cn;
  CntrCode  *cc;
  char ipaddr[64];
  
  if(s_flgs[2]& FL2_IPCNTRAUT)
  {

   if( ! (ChUser(0x80)) )
   {
#ifdef WITHMD5
    SendDigestAuthReq(loc); 
#else
    Send(AuthErr, strlen(AuthErr)  ) ;// sizeof(AuthErr)-1); 
#endif   
    return -1;
   }
       
       
  }
  
  prepare_Req_var();  
  t=GetVar(req_var,"ip");
  l=GetVar(req_var,"l");
      
  if(t)ip=ConvertIP(tc=t);
  else
  {
    t=ipaddr;  
    IP2S(ipaddr,&sa_c);
    ip=(sa_c6.sin6_family==AF_INET6)? 
        (
          sa_c6.sin6_addr.s6_addr32[2]==0xFFFF0000?
          sa_c6.sin6_addr.s6_addr32[3]:0
        ):
        sa_c.sin_addr.s_addr;

  }    
  p=loc+8192+260;
  
  cn="??";
  tc="unknown";
  if(cntr_dat && ip &&  (cc=FindCntr(htonl(ip))) )
  {
    cn=cc->nm;
    tc=country_names[cc->ind];     
  }
  
  return Send(p,sprintf(p, 
                        (l && *l=='j')?
                        "HTTP/1.0 200\r\nContent-Type: application/x-javascript\r\n\r\n//<!-- \nvar county_code=\"%2.2s\",country=\"%s\",country_ip=\"%s\";\n// -->\n"
                        :(l && *l=='h')?
                        "HTTP/1.0 200\r\nContent-Type: text/html\r\n\r\n<center> <h1>This page convert IP to country.</h1></center> <form method=get> <br> County code:%2.2s<br>Country=%s<BR>IP:<input type=text  name=ip value=\"%s\" ><input type=hidden name=l value=h > <input type=submit value=\"Get country\" ></form>\r\n"                       
                        :"HTTP/1.0 200\r\nnContent-Type: text/plain\r\n\r\n%2.2s,%s\r\n",cn,tc,t));     
    
}


