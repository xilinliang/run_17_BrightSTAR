#!/usr/bin/perl
use strict;
use warnings;

#open my $eemcfill , '<' , "run15PhysFill_run.list" or die $!;
#my @allfill;
#my @allstart;
#my @allend;
#
#while (<$eemcfill>)
#{
#
#	my ($fill, $start, $end) = split;
#
#	push(@allfill,$fill);
#	push(@allstart,$start);
#	push(@allend,$end);
#
#	#print "fill = $allfill[$i] , start run = $allstart[$i] , end run = $allend[$i] \n";
#
#	#$i += 1;
#
#}
#
#print "Please input the run index: \n";
#my $i=<STDIN>;
#chomp($i);
#print "$allfill[$i]\n";
#close($eemcfill);

print "Please input the fill index: \n";
my $fill=<STDIN>;
chomp($fill);
print "Please input the start run: \n";
my $startrun=<STDIN>;
chomp($startrun);
print "Please input the end run: \n";
my $endrun=<STDIN>;
chomp($endrun);

#my $getfile='get_file_list.pl -keys \'path,filename\' -cond \'trgsetupname=production_pp200trans_2015, filetype=daq_reco_MuDst, filename~st_physics, storage!=hpss, runnumber[]'."$allstart[$i]-$allend[$i]".'\' -limit -1 -delim \'/\' > '."physics_fill_$allfill[$i]_filetmp.list";
my $getfile='get_file_list.pl -keys \'path,filename\' -cond \'trgsetupname=pp500_production_2017, filetype=daq_reco_MuDst, filename~st_rp, storage!=hpss, runnumber[]'."$startrun-$endrun".'\' -limit -1 -delim \'/\' > '."rp_fill_$fill"."_filetmp.list";
print "$getfile \n";
system($getfile)==0 or die "can not get file";

#my $addword = 'sed \'s/\/home\/starlib/root\:\/\/xrdstar.rcf.bnl.gov\:1095\/\/home\/starlib/g\''." physics_fill_$allfill[$i]_filetmp.list > run15EEMCfilelist/physics_fill_$allfill[$i]_file.list";
my $addword = 'sed \'s/\/home\/starlib/root\:\/\/xrdstar.rcf.bnl.gov\:1095\/\/home\/starlib/g\''." rp_fill_$fill"."_filetmp.list > run17RPfilelist/rp_fill_$fill"."_file.list";
print "$addword \n";
system($addword)==0 or die "can not add word";


my $command = "star-submit-template -template sumsNanoJobrun17RPEEMCtest.xml -entities fill=$fill\n";
print $command;
system($command)==0 or die "can not submit job";


