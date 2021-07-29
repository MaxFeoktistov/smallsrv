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




@eopt=`cd o ; ./httpd.exe --help`;
@ropt=`cd ru ; ./httpd.exe --?`;


$i=0;

  %copt=ParseOpt(@eopt);
  $n=$i;
  exit -1 if(!$n);
  %rcopt=ParseOpt(@ropt);
  $rn=$i;



open FILE, "<desc.htm" || die ("Cant open src\n");
@lines=<FILE>;
close FILE;
open FILE, "<descr.htm" || die ("Cant open src\n");
@rlines=<FILE>;
close FILE;

$nl=0;
$nlf=0;
$tt=0;
$ii=0;

foreach $s (@rlines)
{
 $nl++;
 if($s =~ /\<\/td\>\<td\>\<b\>(.+)\<\/b\>\<\/td\>\<\/tr\>/ )
 {
   $x=$1;
   for($i=$n;$i<$rn;$i++)
   {
     $z=$jkey[$i];
#     print "$z\n" if($tt++<$n);
     if($x =~ /$z/x )
     {
      $fnd[$i]=1;
      $rnlines{$x}=$nl;
      $rnlines{$z}=$nl;
      $ii++;
     }
   
   }
      
 
 }
}


$i=1;
$prev='';
foreach $s (@lines)
{
 if($s =~ /\<\/td\>\<td\>\<b\>(.+)\<\/b\>\<\/td\>\<\/tr\>/ )
 {
   $x=$1;
   $nlines{$x}=$i++;
   if($x =~/([^=]+)=/)
   {
     $z=$1;
     $nlines{$z}=$i-1;
   }
   else
   {
    $z=$x;
   }
   
   $prevkey{$z}=$prev;
   $prev=$z;
   
 }
 
}

print STDERR "$nl lines readed, $ii key already present, $i eng keys\n";



$nfnds=0;
$foragaincnt=0;
$oforagaincnt=0;


   for($i=$n;$i<$rn;$i++)
   {
     if(! $fnd[$i])
     {
     
#        if( $dsc[$i] =~/([^.]+[.])(.*)/ )
#        {
#          ($a,$b)=($1,$2);
#        }
#        else
#        {
#          ($a,$b)=($dsc[$i],'');
#        }
#        printf '<tr><td><b>%s</b>%s</td><td><b>%s</b></td></tr>
# ',$a,$b,$key[$i];
 
       InsertLine($i);
     }
   
   }
   

while($foragaincnt && $oforagaincnt!=$foragaincnt)
{
 $oforagaincnt=$foragaincnt;
 for($j=0;$j<$foragaincnt; $j++)
 {
   if(InsertLine($foragain[$j]) )
   {
      splice @foragain, $j, 1;
      $foragaincnt--;
      $j--;
   }
 
 }


}


print STDERR "Inserted $nfnds lines, not found $foragaincnt lines\n";   
   
open FILE, ">t/descr.htm" || die ("Cant open dst\n");   
for($i=0;$i<=$#rlines; $i++)
{
  print FILE $rlines[$i] ;
}
close FILE;

print "\n\n";
foreach $x (@foragain)
{
  print MkLine($x);
}

exit 0;




sub  MkLine
{
 my ($i) = @_;
 my ($r);
        if( $dsc[$i] =~/([^.]+[.])(.*)/ )
        {
          ($a,$b)=($1,$2);
        }
        else
        {
          ($a,$b)=($dsc[$i],'');
        }
       $r= sprintf '<tr><td><b>%s</b>%s</td><td><b>%s</b></td></tr>
',$a,$b,$key[$i];

  $r;
}
      
sub  InsertLine
{
 my ($i) = @_;
 my ($pos,$npos,$j,$t,$k,$r) ;
 
 
 $r=0;
 
 $k=$jkey[$i];
 $t=$prevkey{$k};
 if($t)
 {
   $npos=$rnlines{$t};
        if(!$npos)
        {
          
         # print STDERR "Not found rus prev $t line $key[$i]\n";
         # print MkLine($i);
           if(!$oforagaincnt)
           {
            $foragain[$foragaincnt]=$i;
            $foragaincnt++;
           }
        }
        else
        {
          $t=  " " .
              MkLine($i);
          
#            if($rlines[$npos] !~ /^[\n]/m )
#            {
#              $t = "\n$t";
#            }
#            else
#            {
#              $rlines[$npos] =~ s/[\r\n]+/\n/gm;
#            }
#           $t .= "\n" if($rlines[$npos+1] !~ /^[\n]/m ) ;

          
          
          splice @rlines, $npos, 0, $t ;
          foreach $x (keys %rnlines)
          {
            $rnlines{$x}++ if( $rnlines{$x} > $npos );
          }
          $rnlines{$key[$i]} = $npos+1;
          $rnlines{$k} = $npos+1;
          $nfnds++;
          $r=1;

        } 
 }
 else
 {
  
   print STDERR "Not found prev line for $k\n";
   print MkLine($i);
    
 
 }
 
 $r;
} 
 
sub  InsertLine2
{
 my ($i) = @_;
 my ($pos,$npos,$j,$t,$r) ;
 
 $pos = $nlines{$key[$i]};
 $pos = $nlines{$jkey[$i]} if(! $pos);
 $r=0;
 if(!$pos)
 {
   print STDERR "Not found $key[$i]\n";
   print MkLine($i);
 }
 else
 {
   $pos--;
   
   for($j=1;$j<$n;$j++)
   {
     $t=$key[$j];
     if($pos == $nlines{$t})
     {
        $npos=$rnlines{$t};
        $npos=$rnlines{$jkey[$j]} if(!$npos);
        if(!$npos)
        {
          
         # print STDERR "Not found rus prev $t line $key[$i]\n";
         # print MkLine($i);
           if(!$oforagaincnt)
           {
            $foragain[$foragaincnt]=$i;
            $foragaincnt++;
           }
        }
        else
        {
          $t= MkLine($i);
          if($rlines[$npos] !~ /^[\n]/m )
          {
            $t = "\n$t";
          }
          else
          {
            $rlines[$npos] =~ s/[\r\n]+/\n/m;
          }
          $t .="\n" if($rlines[$npos+1] !~ /^[\n]/m ) ;
          
          splice @rlines, $npos, 0, $t ;
          foreach $x (keys %rnlines)
          {
            $rnlines{$x}++ if( $rnlines{$x} >= $npos );
          }
          $rnlines{$key[$i]} = $npos;
          $nfnds++;
          $r=1;

        }  
        goto L1;
     }
   
   }
   
   
   print STDERR "Not found prev line $key[$i]\n";
   print MkLine($i);
   
   
 L1:  ;
   
   
   
 }
 $r;
}

  
sub ParseOpt
{
  my( @opt ) = @_;
  my ($s,@ar,%r);
#$i=$first;

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
     $r{$ar[0]}=$ar[1];
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
#    else
#    {
#     print "$s";
#    }

  }

#print STDERR "Found $i keys\n@opt\n";
print STDERR "Found $i keys\n\n";
    %r ;
}

