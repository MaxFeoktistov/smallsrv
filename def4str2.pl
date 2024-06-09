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

 if($#ARGV<2)
 {
   printf "Usage: $0 prefix c_file header_file [--force]\n";
   exit(1);
 }

 $pref='STRx';
 $pref=$ARGV[0];
 $lpref="l$pref";
 $cfile = $ARGV[1];
 $hdr_file = $ARGV[2];
 $hdr_file1 = "g4strc.h";
 $lang_file = "en/shs_lang.cfg";
 $pref1='S2';
 $p_nm='';
 $quot='"';

############################################################

 $hdr_updated = 0;
# @new_nm=();

%strhdr = LoadHeader($hdr_file);
%strhdr1 = LoadHeader($hdr_file1);
%strlang = LoadLangFile($lang_file);

$cur_hsh=\%strhdr;


LoadCFile($cfile);
if( $hdr_updated || $strhdr{'_updated_'} || ($#ARGV>=3 && $ARGV[3] eq '--force') )
{
  delete $strhdr{'_updated_'};
  backup_rename($hdr_file);
  SaveHeader($hdr_file, $pref);
  $fnm="${pref}_lf.hh";
  backup_rename($fnm);
  SaveHeader2($fnm, $pref);

  print "$fnm updated\n";

  $fnm="${lpref}_lf.cfg";
  backup_rename($fnm);
  SaveCfg($fnm, $pref);

}
if( $strhdr1{'_updated_'})
{
  print "**Warning $hdr_file1 updated but unsaved\n";
}
if( $strlang{'_updated_'})
{
  print "**Warning $lang_file updated but unsaved\n";
}



exit(0);

sub dbg
{
  my ($s) = @_;
  print STDERR $s;
}

sub LoadHeader
{
  my ($hdrfile) = @_;
  my ($nm,$v,$vv);
  my %strhdr;
  my $cnt = 0;
  if( open( FILE, "<$hdrfile"  ) )
  {
    while(<FILE>)
    {
      s/[ \t\r\n]+$//;

      if(/^[ \t]*char ([A-Z0-9a-z_]+)\[\] = (\".*)$/)
      {
        ($nm,$v) = ($1,$2);
        while ( ($v !~ /[;][ \t]*$/m) && ($_=<FILE>) )
        {
          s/[ \t\r\n]+$//;
          s/^[ \t]+//;
          # s/^["]//;
          # $v =~ s/\"$//;
          $v .= "\n$_";

         # print STDERR "$nm: $v : $_\n";
        }
        $v =~ s/[; \t]+$//g;
        #$v =~ s/\"$//;
        $strhdr{$nm} = $v;
        $cnt++;

        #dbg("$cnt: $nm : $v\n");

      }
    }
    close FILE;
  }
  else
  {
    printf "Error. Can't open  $hdrfile for read; %s\n", $!;
  }
  dbg("Header $hdrfile loaded: $cnt records\n");
  return %strhdr;
}

sub SaveHeader
{
  my ($hdrfile, $prefix) =@_;
  my ($nm,$v,$t);

  if( open(FILE, ">$hdrfile") )
  {
    $t=$hdrfile;
    $t =~ s/^.*[\/]//;
    $t =~ s/[.]/_/;
    printf FILE '
/* Generated file. Do not edit it. */
#ifndef %s
#define %s
', $t,$t;

    foreach $nm ( keys(%strhdr) )
    {
      $v = $strhdr{$nm};
      $t = $nm;
      $t =~ s/^$prefix//;

      printf FILE '
#ifdef MAINSTR
 char %s[] = %s;
 char *l%s = %s;
#else
 extern char *l%s;
 extern char %s[];
#endif
#define %s l%s
#define x_%s %s

      ',$nm, $v, $nm, $nm, $nm, $nm,
      $t, $nm,
      $t, $nm
      ;
    }
    print FILE '
#endif
';
    close FILE;
  }
  else
  {
    printf "Error. Can't open  $hdrfile for write; %s\n", $!;
  }

}


sub SaveHeader2
{
  my ($hdrfile, $prefix) =@_;
  my ($nm,$v,$t);

  if( open(FILE, ">$hdrfile") )
  {
    $t=$hdrfile;
    $t =~ s/^.*[\/]//;
    $t =~ s/[.]/_/;
    printf FILE '
/* Generated file. Do not edit it. */
#ifndef %s
#define %s
', $t,$t;

    foreach $nm ( keys(%strhdr) )
    {
      $v = $strhdr{$nm};
      $t = $nm;
      $t =~ s/^$prefix//;

      printf FILE '{"%s", & l%s},
',$nm, $nm;
    }
    print FILE '
#endif
';
    close FILE;
  }
  else
  {
    printf "Error. Can't open  $hdrfile for write; %s\n", $!;
  }

}

sub SaveCfg
{
  my ($hdrfile, $prefix) =@_;
  my ($nm,$v,$t);

  if( open(FILE, ">$hdrfile") )
  {

    foreach $nm ( keys(%strhdr) )
    {
      $v = $strhdr{$nm};

        $v =~ s/\\"/__Q__/mg;
        $v =~ s/"//mg;
        $v =~ s/__Q__/"/mg;
        $v =~ s/\n([^ \t])/\n $1/g;
        printf  FILE '
%s=%s

', $nm, $v;
    }
    close FILE;
  }
  else
  {
    printf "Error. Can't open  $hdrfile for write; %s\n", $!;
  }

}

sub FindName
{
  my ($v) = @_;
  my $k;
  my $vv = RStr($v);

  foreach $cur_hsh (\%strhdr, \%strhdr1, \%strlang )
  {
    foreach $k (keys %{$cur_hsh} )
    {
      if( RStr( ${$cur_hsh}{$k} ) eq $vv )
      {
        return $k;
      }
    }
  }

  return 0;
}

sub StrByName
{
  my ($nm) = @_;
  my $v;

  foreach $cur_hsh (\%strhdr, \%strhdr1, \%strlang )
  {
    foreach $v ('', $pref, "${pref}t2", 't2', $pref1, "${pref1}t2")
    {
      $p_nm = "$v$nm";
      if( ${$cur_hsh}{$p_nm}  )
      {
        return ${$cur_hsh}{$p_nm};
      }
    }
  }

  return 0;
}


sub NewName
{
  my ($v) = @_;
  my $s=0;
  my ($i, $nm);
  for($i=0; $i<length($v); $i++ )
  {
    $s += ord( substr($v, $i, 1) ) << ($i&0xF);
  }
  do{
    $nm="${pref}T_$s";
    $s++;
  }while( $strhdr{$nm} || $strlang{$nm} );

  $strhdr{$nm} = $v;
  $hdr_updated++;

  print "New name $nm for $v\n";
  return $nm;
}

sub RStr
{
  my ($s) = @_;
  $s =~ s/^[ \t]*\/\/.*//gm;
  $s =~ s/[ \t]*\/\/[^"]*$//gm;
  $s =~ s/^[ \t]*\/\*.*?\*\/[ \t]*//gm;
  $s =~ s/[ \t]*\/\*[^"]*\*\/[ \t]*$//gm;

  $s =~ s/^[ \t]+$//gm;

  $s =~ s/^[ \t]*["]//mg;
  $s =~ s/["][ \t]*$//mg;
  $s =~ s/[\r\n]+//g;


  #$s =~ s/([^\\])["][ \t]*[\r\n]+[ \t]*["]/$1/g;

  return $s;
}

sub CheckIsChanged
{
  my ($nm,$v) = @_;
  my $ve = StrByName($nm);
  if( !$ve )
  {
    dbg("***Error. $nm not found\n");
    $strhdr{$nm} = $v;
    $hdr_updated++;
  }
  if( RStr($ve) ne RStr($v) )
  {
#    dbg("***Warning. $nm ($p_nm) changed -- \nold: ${$cur_hsh}{$p_nm}\nnew: $v\n");
    dbg("***Warning. $nm ($p_nm) changed -- \nold: $ve\nnew: $v\n");
    # exit(1);
    ${$cur_hsh}{$p_nm} = $v;
    ${$cur_hsh}{'_updated_'} = 1;
  }
}

sub LoadCFile
{
  my ($cfile) = @_;
  my ($nm, $vnm, $v, $ve, $t, $i, $j, $k, @lines,$fixed,$fixed_line);
  $fixed = 0;
  if( open(FILE, "<$cfile")  )
  {
    @lines = <FILE>;
    close FILE;
    dbg("****** $#lines lines loaded\n");

    for($i=0; $i<$#lines ; $i++)
    {
      $t = $lines[$i];
      if( $t !~ /^[ \t]*\/\// )
      {
        if($t =~ /CXS\(([A-Z0-9a-z_]+)[ ]*,[ \t]*(\".*[^\\]\")[ \t]*\)/)
        {
          $nm=$1;
          $v=$2;
          CheckIsChanged($nm,$v);
        }

        if($t =~ /CXS\(([A-Z0-9a-z_]+)[ ]*,(\".*[^\\]\")[ ]*$/)
        {
          $nm=$1;
          $v=$2;
          $k=1;

          for($j=$i+1; $k>0; $j++  )
          {
            $ve =  $lines[$j];
            $ve =~ s/[ \r\n]+$//;
            #$ve =~ s/ \/\*.*?\*\///;
            $ve =~ s/^[ \t]+//;
            $ve =~ s/\\$//;
            $ve =~ s/^[ \t]*\/\/.*$//;
            $ve =~ s/[ \t]*\/\/[^"]+$//;
            $ve =~ s/^[ \t]*\/\*.*?\*\/[ \t]*//;
            $ve =~ s/[ \t]*\/\*[^"]*?\*\/[ \t]*$//;
            if($ve =~ /[^\\]["][ \t]*\)/)
            {
              $k = 0;
              $ve =~ s/([^\\])["][ \t]*\).*/$1"/;
            }
            if($ve =~ /^[ \t]*\)/)
            {
              $k = 0;
            }
            else
            {
              $v .= "\n$ve" if($ve ne '');
            }
            # dbg("$i,$j:$v\n");
          }
          CheckIsChanged($nm,$v);
        }



        while($t =~ /CS\([ \t]*(\".*[^\\]\")[ \t]*\)/)
        {
          $v=$1;
          $v =~ s/[ \t]+$//;
          $fixed++;
          $nm = FindName($v);
          if(! $nm)
          {
            $nm = NewName($v);
#             $strhdr{$nm} = $v;
#             $hdr_updated++;
          }
          #   $t =~ s/CS\([ \t]*$v[ \t]*\)/CXS($nm, $v)/;
          $t =~ s/CS\([ \t]*/CXS($nm, /;
          $lines[$i] = $t;
          #   dbg("$v: $t\n");
        }


        if($t =~ /CS\([ \t]*(\".*[^\\]\"[LF\\ \t]*)$/)
        {
          $v=$1;
          $v =~ s/\\$//;
          $k=1;
          # dbg("$i:$v: $t\n");

          # && $lines[$j] =~/^([ \t]*\".*[^\\]\"[\);LF\\ \t]*)$/ ;

          for($j=$i+1; $k>0; $j++  )
          {
            $ve =  $lines[$j];
            $ve =~ s/[ \r\n]+$//;
            #$ve =~ s/ \/\*.*?\*\///;
            $ve =~ s/^[ \t]+//;
            $ve =~ s/\\$//;
            $ve =~ s/^[ \t]*\/\/.*$//;
            $ve =~ s/[ \t]*\/\/[^"]+$//;
            $ve =~ s/^[ \t]*\/\*.*?\*\/[ \t]*//;
            $ve =~ s/[ \t]*\/\*[^"]*?\*\/[ \t]*$//;
            if($ve =~ /[^\\]["][ \t]*\)/)
            {
              $k = 0;
              $ve =~ s/([^\\])["][ \t]*\).*/$1"/;
            }
            if($ve =~ /^[ \t]*\)/) {
              $k = 0;
            }
            else
            {
              $v .= "\n$ve" if($ve ne '');
            }
            # dbg("$i,$j:$v\n");
          }
          $fixed++;
          $nm = FindName($v);
          if(! $nm)
          {
            $nm = NewName($v);
         #   $strhdr{$nm} = $v;
         #   $hdr_updated++;
           # push(@new_nm,$nm);

          }
          $t =~ s/CS\(/CXS($nm, /;
          $lines[$i] = $t;
          $i = $j - 1;
        }

        if($t =~ /CS\([ \t]*([a-zA-Z0-9_]+)[ \t]*\)/)
        {
          $nm=$1;
          if( length($nm) > 3 )
          {

#             foreach $cur_hsh (\%strhdr, \%strhdr1, \%strlang)
#             {
#               foreach $ve ('', $pref, "${pref}t2", "t2", $pref1, "${pref1}t2")
#               {
#                 $vnm = $ve . $nm;
#                 dbg("vnm=$vnm cur_hsh=$cur_hsh\n");
#                 if( ${$cur_hsh}{$vnm}  )
#                 {
#                   $v = ${$cur_hsh}{$vnm};
#                   if($v !~ /^[ \t]*["]/) {
#                     $v =~ s/^(.*)$/$quot$1$quot/mg;
#                   }
#                   $t =~ s/CS\([ \t]*$nm[ \t]*/CXS($vnm, $v/;
#                   goto LhashFound;
#                 }
#               }
#             }
#
            $v = StrByName($nm);
            if($v) {
              if($v !~ /^[ \t]*["]/) {
                $v =~ s/^(.*)$/$quot$1$quot/mg;
              }
              $t =~ s/CS\([ \t]*$nm[ \t]*/CXS($p_nm, $v/;
            }
            else {
              print STDERR "$i:***Value for $nm not found. : $t \n";
              $t =~ s/CS\([ \t]*([a-zA-Z0-9_]+)[ \t]*/CXS($nm, $nm/;
            }
            # LhashFound:
            $lines[$i] = $t;
            $fixed++;

          }

        }


      }
    }

    # dbg("end loop\n");


    if($fixed) {
      backup_rename($cfile);
      if( open(FILE, ">$cfile")  )
      {
        print FILE @lines;
        #         foreach $_ (@lines)
        #         {
        #           print FILE $_;
        #         }
        close FILE;
      }
    }
  }
  else
  {
    printf "Error. Can't open  $hdrfile for read; %s\n", $!;
  }
  dbg("end load C\n");
#  return @lines;
}

sub backup_rename
{
  my ($cfile) = @_;
  my ($i, $j, $v);
  $i=0;
  $v = "$cfile.backup$i";
  while(-e $v)
  {
    $j = $i + 1;
    $v =~ s/backup$i$/backup$j/ ;
    $i++;
  }
  rename($cfile,$v);
}



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

 return %r ;

}

