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
   die  "Command line:\nadd_to_langcfg.pl srcfile.cfg dstfile.cfg [outfile.cfg]\n";
 }

 %src= LoadLangFile($ARGV[0]);
 %dst= LoadLangFile($ARGV[1]);
 $ofile=$ARGV[1];
 $ofile=$ARGV[2]  if($#ARGV>=2);
 

 open FILE , ">>$ofile" || die ("Cant open dst $ofile\n");
 
 $i=0;
 foreach $t (keys %src)
 {
   if(! $dst{$t} )
   {
   
     print FILE "$t=$src{$t}\n\n";
     $i++;
   }
  
 }


 close FILE;
 print STDERR "Added $i lines\n";
 exit 0;

sub LoadLangFile
{
  my ($fname) = @_;
  my($t,%r,$x,$y,$l,$n);
  
  open FILE, "<$fname" || die ("Cant open src $fname\n");

  $t=''; 
  $l=1;
  $n=0;
  while(<FILE>)
  {
   s/[\r\n]+//g;
   s/^[ \t]+//g;
    
   
   
   
   if($_ eq '')
   {
     
#     if($t =~ /^([A-Za-z_0-9]+)=()/)
    
     ($x,$y)=split(/=/,$t,2);
     
     if($y ne '' && $x ne '')
     {
      $r{$x}=$y;
      $n++;
     }
     elsif($t ne '')
     {
       print STDERR "$fname:Bad line $l: $t\n";
     }
   
     $t=''; 
   
   }
   else
   {
    $t .= "\n " if($t ne '');
    $t .= $_;
   }
   
   $l++; 
  }



 close FILE;

 print STDERR "$fname: $l lines, $n strings loaded\n";
 
 %r ;

}


# 
# sub SaveLangFile
# {
#   my ($fname) = @_;
#   my($t,$y);
#   
#   open FILE, ">$fname" || die ("Cant open dst $fname\n");
# 
#   close FILE;
# 
# }
