#!/usr/bin/perl

# hex,pl, written by Hexacorn.com, 2014-2017
#
# This is a simple hexdump script that shows the standard
# hexadecimal output including offset and binary data
# but also decimal offset and strings extracted from
# a current row (both ANSI and Wide)
#
# Usage:
#      perl hex.pl <filename>
# or if you want to see strings:
#      perl hex.pl -s <filename>
#      perl hex.pl -S <filename>
#      -s - extract strings
#      -S - extract strings, and skip output lines w/o strings

use strict;
use warnings;

my $f=shift || die ("\n
Usage:
 perl hex.pl <filename>
 perl hex.pl -s <filename>
 perl hex.pl -S <filename>
");

my $opt_s=0;
if ($f eq '-s'||$f eq '-S')
{
  $opt_s=1 if ($f eq '-s');
  $opt_s=2 if ($f eq '-S');
  $f=shift;
}

open    F,"<$f" || die("\nCan't open file '$f'!\n");
binmode F;

my $fs=-s $f;

my $dol=length($fs);
my $hol=length(sprintf("%X",$fs));

my $ofs=0;
while (1)
{
    seek (F, $ofs, 0);
    my $nor = read(F,my $buf,16+64) || last;

    my $s='';
    if ($opt_s!=0)
    {
       while ($buf =~ /([ -~]{3,}|([ -~]\x00){3,}|[a-z0-9]{2}|([a-z0-9]\x00){2})/sgi)
       {
         my $t=$1;
         if (((pos $buf)-length($t))<16)
         {
           $t =~ s/\x00//gs if $t =~ /([ -~]\x00){2,}/s;
           $s=$s.' ' if ($s ne '');
           $t=~s/^\s*(.+)\s*$/$1/;
           $s.=' '.$t;
         }
       }
    }
    my $ls = "";
    my $rs = "";
    $buf=substr($buf,0,16);
    pos $buf=0;
    foreach my $i ($buf =~ m/./gs)
    {
        $ls.=sprintf(" %02X",ord($i));
        if ($i=~m/[ -~]/) { $rs.= $i; } else { $rs.= "."; }
    }
    $ofs+=16;

    next if ($opt_s==2&&$s eq '');
    if (($ofs & 0xFF) == 0)
    {
      print ("\n".(" " x $hol)."   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 0123456789ABCDEF\n".("-" x (71+$dol+$hol))."\n");
    }
    printf ("%0".$hol."X :%-48s %-16s %0".$dol."s",$ofs-16,$ls,$rs,$ofs-16);
    print ("$s") if ($opt_s!=0);
    print ("\n");
}

close F;