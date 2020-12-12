#!/usr/bin/perl

#
# Copyright (C) 1999-2020 Maksim Feoktistov.
#
# This file is part of Small HTTP server project.
# Author: Maksim Feoktistov 
#
#
# Small HTTP server is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License.
#
# Small HTTP server is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see https://www.gnu.org/licenses/ 
#
# Contact addresses for Email:  support@smallsrv.com
#
# 
#


 $coment=0;
 
 $pref='STRx';
 $pref=$ARGV[0] if($#ARGV>=0) ; 
 $lpref="l$pref";
 
 ;
 
 if($#ARGV>=1)
 {
  open LANGFILEC, ">$ARGV[1]\.hh" || die("Can't open ${pref}_lf\.hh");
  open LANGCFG, ">$ARGV[1].cfg" || die("Can't open $ARGV[1]");
 }
 else
 {
  open LANGFILEC, ">${pref}_lf\.hh" || die("Can't open ${pref}_lf\.hh");
  open LANGCFG, ">${lpref}_lf\.cfg" || die("Can't open ${lpref}_lf\.cfg");
 }
 $tif='';
 $eif=0;
 while(<STDIN>)
 {
#  
#    if(/^[ \t]+\/\*/ )
#    {
#      $coment=1;
#    }
#    
#    if(/^[ \t]+\/\*/ )
#    {
#      $coment=0;
#    }
#    

   if(/^#if/)
   {
     $tif.=$_;    
     $eif++;
   }
   elsif(/^(#else|#endif)/)
   { 
     $taa=$1;
  #   $tif=$_ if($tif eq '');
     if($tif eq '' && $eif>0)
     {
  #     printf  LANGFILEC  $_ ;
       $eif-- if($taa eq '#endif');
     }
     else
     {
       $tif='';    
       $eif--;
     }
     $eif=0 if($eif<0);

   }
   elsif( (! /^[ \t]*\/\//)  && ! /DONT_AUTO_CHANGE/ )
   {
    if( /^[ \t]*#define[ \t]+([A-z_0-9]+)[ \t]+(".+)[\r\n]/  )
    {
      ($a,$b)=($1,$2);
      
      while($b =~ /\\$/ )
      {
        $c=<STDIN>;
        exit 0 if(! $c) ;
        $c =~ s/[\r\n]+//gm;
        $b .= "\n $c";
      }

      $b=~ s/\\\n/\n/gm;
      print "#ifdef MAINSTR\n char $pref$a\[\] = $b;\n" ;
      
      if($b =~ /[ \t\-]/)
      {
      
        
        print " char *$lpref$a = $pref$a;\n#else\n extern char *$lpref$a;\n extern char $pref$a\[\];\n#endif\n" ;
        
        printf  LANGFILEC '%s{"%s",& %s},
',$tif , "$pref$a" , "$lpref$a";
         $tif='';

        $b =~ s/\\"/__Q__/mg;
        $b =~ s/"//mg;
        $b =~ s/__Q__/"/mg;
        printf  LANGCFG '
%s=%s
',"$pref$a",$b ;      
      $_ = "#define $a $lpref$a\n#define x_$a $pref$a\n\n"; 
      }
      else
      {
        print "#else\nextern char *$pref$a;\n#endif\n" ;
        $_ = "#define $a $pref$a\n#define x_$a $b\n"; 
      
      }
    }
   }
   
   print $_;
 }

 
 close LANGFILEC ;
 close LANGCFG ;
 
 exit(0);
 