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
 
 while(<STDIN>)
 {
  
    if(/^[ \t]+\/\*/ )
    {
      $coment=1;
    }
    
    if(/^[ \t]+\/\*/ )
    {
      $coment=0;
    }
    

   if( (!$coment) && ! /^[ \t]*\/\// )
   {
    if( /"(.+)"(.*)/  )
    {
      ($a,$b)=($1,$2);
      
      while($b ne '"' )
      {
        $a .= '\\' if($a =~ /\\$/);
        $c=<STDIN>;
        exit 0 if(! $c) ;
        if( $c  =~ /(.*)([\r\n"])/  )
        {
          $b .= "\n $c";
          
        }
        else
        {
            $b .= "\n$c\n";
        }
        $c =~ s/[\r\n]+//gm;
      }

      $b=~ s/\\\n/\n/gm;
      print "#ifdef MAINSTR\n char $pref$a\[\] = $b;\n" ;
      
      if($b =~ /[ \t\-]/)
      {
      
        
        print "char * $lpref$a = $pref$a;\n #else\n extern char * $lpref$a;\n #endif\n" ;
        
        printf  LANGFILEC '{"%s",& %s},
' , "$pref$a" , "$lpref$a";

        $b =~ s/\\"/__Q__/mg;
        $b =~ s/"//mg;
        $b =~ s/__Q__/"/mg;
        printf  LANGCFG '
%s=%s
',"$pref$a",$b ;      
      $_ = "#define $a $lpref$a\n"; 
      }
      else
      {
        print "#else\nextern char *$pref$a;\n#endif\n" ;
        $_ = "#define $a $pref$a\n"; 
      
      }
    }
   }
   
   print $_;
 }
 
 close LANGFILEC ;
 close LANGCFG ;
 
 