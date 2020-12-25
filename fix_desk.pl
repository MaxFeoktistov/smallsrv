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




@opt=`./t/httpd.exe --help`;

$i=0;
foreach $s  (@opt)
{
#   @ar =  split (' - ',$s,2 );
#  $hopt[$a]=$b;
#  if($#ar >= 1)
   if( $s =~ /([^ ]+) - (.+)/ )
   {
     @ar=($1,$2);
     $ar[0] =~ s/[ \t]+//g;
     $key[$i]=$ar[0];
     if( $ar[0]=~/(.+)=/ )
     {
       $jkey[$i]=$1;
     }
     else
     {
       $jkey[$i]=$ar[0];
     }
     $dsc[$i]=$ar[1];
     $fnd[$i]=0;
     $i++;
   }
   else
   {
    print "$s";
   }

}

#print STDERR "Found $i keys\n@opt\n";
print STDERR "Found $i keys\n\n";
$n=$i;
exit -1 if(!$n);


open FILE, "<desc.htm" || die ("Cant open src\n");
@lines=<FILE>;
close FILE;
$nl=0;
$nlf=0;
$tt=0;
foreach $s (@lines)
{
 if($s =~ /\<\/td\>\<td\>\<b\>(.+)\<\/b\>\<\/td\>\<\/tr\>/ )
 {
   $x=$1;
   for($i=0;$i<$n;$i++)
   {
     $z=$jkey[$i];
     print "$z\n" if($tt++<$n);
     if($x =~ /$z/x )
     {
      $fnd[$i]=1;
     }
   
   }
      
 
 }
 $nl++;
}

print STDERR "$nl lines readed\n";

   for($i=0;$i<$n;$i++)
   {
     if(! $fnd[$i])
     {
       if( $dsc[$i] =~/([^.]+[.])(.*)/ )
       {
         ($a,$b)=($1,$2);
       }
       else
       {
         ($a,$b)=($dsc[$i],'');
       }
       printf '<tr><td><b>%s</b>%s</td><td><b>%s</b></td></tr>
',$a,$b,$key[$i];
     }
   
   }
      


