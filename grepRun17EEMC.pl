#!/usr/bin/perl
##use strict;
##use warnings;

my $folder=$ENV{'PWD'};
chomp($folder);
my $dest_folder = "$folder/run17EEMCdataset/P1/";
my $runlist = "$folder/Run17pp500Runlist.txt";
my $oldfilelist = "$folder/run17EEMCdataset/run17EEMCP1.list";

open(RUN,"<$runlist");
my @runlist = <RUN>;
my $totalrun = @runlist;
print "$totalrun\n";

for ($i = 0; $i < $totalrun; $i++)
#for ($i = 0; $i < 5; $i++)
{

	chomp($runlist[$i]);
	print "$runlist[$i] \n";
	my $outname = "$dest_folder/run$runlist[$i].list";
	if (-f $outname) {system("rm $outname");}
	my $commend = "grep -ir \"$runlist[$i]\" $oldfilelist > $outname";
	system("grep -ir \"$runlist[$i]\" $oldfilelist > $outname");
	if (not -s $outname) {print "empty file! \n"; system("rm $outname");}

}

