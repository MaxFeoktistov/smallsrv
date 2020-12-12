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


 
 if($#ARGV<1)
 {
   die("Comand line:  src_lang_pack1.cfg src_lang_pack1.cfg >dst_lang_pack.cfg") 
 }
  
#  open SRC1, "<$ARGV[0]" || die("Can't open $ARGV[0]");
#  open SRC2, "<$ARGV[1]" || die("Can't open $ARGV[1]");
 %src1 = LoadLang($ARGV[0]);
 %src2 = LoadLang($ARGV[1]);

# open DST, ">$ARGV[2]" || die("Can't open $ARGV[2]");
 
 if($#ARGV >=2 )
 {
   open SRC, "<$ARGV[2]" || die("Can't open $ARGV[0]");
   $cnt=0;
   $cnt2=0;
   while(<SRC>)
   {
     $z=$_;
     L1: foreach $k ( keys(%src1) )
     {
        $y= substr($k,2); 
        if(  (   ! $src2{$k}) &&   
       # $z =~  # /$k/xi )
           /(.+)$y/x ) 
        {
          $n=$1;
#          $pref=substr($k,0,2);
          $x= mk_hash_back($n);
          
          for($i=1; $i<=3 ; $i++)
          {
            $xx="S$i" . "t2T_$x";
            if($src2{$xx})  
            {
                print "$k=$src2{$xx}\n\n";
                $dst{$k} = $src2{$xx} ;
             #   print STDERR " Found!!! $k=$xx $src2{$xx} ==  $src1{$k}\n";
                $cnt2++;    
                goto L2;
            }
          }
          print STDERR " Not found $k $xx $y $src1{$k}\n";
           
   L2:
          $cnt++;
          last L1;
        }
   
      }   
   
   }
   print STDERR " total $cnt vars in file found $cnt2\n";

 }

 
 $cnt = 0; 
 foreach $k ( keys(%src1) )
 {
   if($src2{$k})
   {
     $dst{$k} = $src2{$k} ; 
   }
   elsif( !  $dst{$k} )
   {
   #  print "$k=$src1{$k}\n\n";
     $dst{$k} = $src1{$k} ;
     $cnt++;
   }

   print "$k=$dst{$k}\n\n";
   
 }
 print  STDERR "\n\n $cnt vars absent ($#ARGV)\n\n";
 
 
 sub LoadLang
 {
  my ($file) = @_ ;
  my (%lf );
  my ($name,$value,$cnt);
  
  $cnt=0;
   open SRC1, "<$file" || die("Can't open $file");
  $name='';
  $value='';
  while(<SRC1>)
  {
    s/[\r\n]+//g;
    if( $name ne '' )
    {
      if($_ eq '')
      {
        $lf{$name}=$value;
        $name='';
        $value='';
        $cnt++;
      }
      else
      {
       $value .= "\n$_";
      }
    }
    elsif(/([^ \t=]+)=(.+)/)
    {
      ($name,$value)=($1,$2);
    
    }
    
    
  }
  
  close SRC1;
  print STDERR  "$file: $cnt vars loaded\n"   ;
  %lf ;
 }  
 
 
 sub mk_hash_back
 {
  my ($str) = @_ ;
  
  my($r,$i,$p,$t) ;
  $r=0;
  $i=1;
  $p= length($str) - 1;
 while($p>=0 )
 {
   $t = ord( substr($str,$p,1) ) ;
#   if( $t != ' ' && $t != '\t' && $t<128 )
   if( $t > 0x20 &&  $t<128 )
   {
     $r+=($t-0x21)*$i;  
     $i+=95;
     if($i> 22369621 ) #(0x7FFFffff/96))
     {
       $i=($i+1)%95;
     }
   }
   $p--;
  }
  
  $r;
 }