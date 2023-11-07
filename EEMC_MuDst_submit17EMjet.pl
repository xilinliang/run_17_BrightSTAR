#!/usr/bin/perl
use strict;
use warnings;

my $folder=$ENV{'PWD'};
chomp($folder);

open my $eemcrun , '<' , "run17EEMCRunlistP1.list" or die $!;
my @allrun;

while (<$eemcrun>)
{

	my ($run) = split;

	push(@allrun,$run);

	#print "run = $run";
	#print "fill = $allfill[$i] , start run = $allstart[$i] , end run = $allend[$i] \n";

	#$i += 1;

}
close($eemcrun);

print "Please input the start run index: \n";
my $start=<STDIN>;
chomp($start);
print "Please input the end run index: \n";
my $end=<STDIN>;
chomp($end);

my $i;
for ($i = $start; $i <= $end ; $i++)
{
	print "run number: $allrun[$i]\n";

	my $outfolder = "$folder/NanoDst/run17EEMCjets/$allrun[$i]/";
	if (not -d $outfolder) {mkdir $outfolder or die "can not mkdir $outfolder";}
	my $command = "star-submit-template -template sumsNanoJobEEMCrun17test.xml -entities run=$allrun[$i]\n";
	print $command;
	system($command)==0 or die "can not submit job";

}


