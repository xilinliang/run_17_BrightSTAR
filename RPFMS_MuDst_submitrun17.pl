#!/usr/bin/perl
use strict;
use warnings;

open my $rpfill , '<' , "run17RPFill_run.list" or die $!;
my @allfill;
my @allstart;
my @allend;

while (<$rpfill>)
{

	my ($fill, $start, $end) = split;

	push(@allfill,$fill);
	push(@allstart,$start);
	push(@allend,$end);

	#print "fill = $allfill[$i] , start run = $allstart[$i] , end run = $allend[$i] \n";

	#$i += 1;

}

print "Please input the run index: \n";
my $i=<STDIN>;
chomp($i);
print "$allfill[$i]\n";
close($rpfill);
my $getfile='get_file_list.pl -keys \'path,filename\' -cond \'trgsetupname=pp500_production_2017, filetype=daq_reco_MuDst, filename~st_rp, storage!=hpss, runnumber[]'."$allstart[$i]-$allend[$i]".'\' -limit -1 -delim \'/\' > '."rp_fill_$allfill[$i]_filetmp.list";
print "$getfile \n";
system($getfile)==0 or die "can not get file";

my $addword = 'sed \'s/\/home\/starlib/root\:\/\/xrdstar.rcf.bnl.gov\:1095\/\/home\/starlib/g\''." rp_fill_$allfill[$i]_filetmp.list > run17RPfilelist/rp_fill_$allfill[$i]_file.list";
print "$addword \n";
system($addword)==0 or die "can not add word";

my $folder_dest = "/star/data01/pwg/liangxl/myBrightSTAR/BBCADCsumFiles/$allfill[$i]/";
if (not -d  $folder_dest) {mkdir  $folder_dest or die "can not mkdir  $folder_dest \n"};

my $command = "star-submit-template -template sumsNanoJobrun17RP.xml -entities fill=$allfill[$i]\n";
print $command;
system($command)==0 or die "can not submit job";


