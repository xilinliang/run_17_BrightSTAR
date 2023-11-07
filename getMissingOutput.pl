#!/usr/bin/perl
##use strict;
##use warnings;

my $folder=$ENV{'PWD'};
chomp($folder);
print "Please input the fill number: \n";
my $fill=<STDIN>;
chomp($fill);
#my $NanoDstFolder="/star/data01/pwg/liangxl/BrightSTAR_EMjet/condorjobResults/";
my $NanoDstFolder="/star/data01/pwg/liangxl/myBrightSTAR/NanoDst/run17FMSjets/";
my $NanoDstFilelist="$folder/All$fill";
my $NanoDstFile="$NanoDstFolder/NanoDiffJetTree_fill$fill"."_*.root";
if (-e $NanoDstFilelist) {unlink($NanoDstFilelist);}
system("ls $NanoDstFile > $NanoDstFilelist");

open(Input,"<$NanoDstFilelist") or die ("can not open $NanoDstFilelist");

@inputlist=<Input>;
$number_input = @inputlist;
print "Please input the total jobs: \n";

my $i;
my $totaljobs =<STDIN>;
chomp($totaljobs);

for ($i=0;$i<$totaljobs;$i++) {$checkfile[$i]=0;}

for ($i=0;$i<$number_input;$i++)
{

	chomp($inputlist[$i]);
	my $outputname = $inputlist[$i];
	my $outputname = substr($outputname,0,-5);
	my $outputid = substr($outputname,86,4);
	print "$outputid ";
	$checkfile[$outputid] = 1;

};

my $badlist;

for ($i=0;$i<$totaljobs;$i++)
{
	if ($checkfile[$i] eq 0) {$badlist=$badlist."$i,";}
}
chop($badlist);
print "badlist: $badlist \n";

print "good to continue? y/n \n";
my $judge=<STDIN>;
chomp($judge);

if ($judge eq "y")
{

	print "Please type the job name: \n";
	my $jobname=<STDIN>;
	chomp($jobname);
	my $resubmit="star-submit -r $badlist sched$jobname.session.xml \n";
	print "$resubmit";
	#system("chmod 744 $outputFile")==0 or die "can not chmod";

}

close(Input);

