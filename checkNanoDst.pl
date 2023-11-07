#!/usr/bin/perl
use strict;
use warnings;

my $folder=$ENV{'PWD'};
chomp($folder);
print "Please input the fill number: \n";
my $fill=<STDIN>;
chomp($fill);
print "fill: $fill \n";
print "Please input the job name: \n";
my $jobname=<STDIN>;
chomp($jobname);


my $NanoDstFolder="/star/data01/pwg/liangxl/myBrightSTAR/NanoDst/run17RPFMS";
my $NanoDstFilelist="$folder/All$fill";
my $NanoDstFile="$NanoDstFolder/NanoDiffJetTree_fill$fill"."_*.root";
if (-e $NanoDstFilelist) {unlink($NanoDstFilelist);}
system("ls $NanoDstFile > $NanoDstFilelist");
my $checkcommand="root -b -q checkroot.C\'(\"$NanoDstFilelist\")\'";
print "$checkcommand \n";
system("$checkcommand");

my $badroot="$folder/badroot.csh";
open(BADINPUT,"<$badroot") or die ("can not open badroot.csh");
my @inputlist=<BADINPUT>;
my $number_input = @inputlist;
my $i;
my $submitcommand;
for ($i=1;$i<($number_input-1);$i++)
{

	chomp($inputlist[$i]);
	print("$inputlist[$i] \n");
        my $outputname = $inputlist[$i];
        $outputname = substr($outputname,0,-5);
        my $outputid = substr($outputname,89,4);
        print "$outputid \n";
	if ($i==1) {$submitcommand="star-submit -r $outputid";} else {$submitcommand=$submitcommand.",$outputid";}

}
$submitcommand=$submitcommand." sched$jobname".'.session.xml';	
print "$submitcommand \n";

print "Please input if it is good to continue: y or n \n";
my $judge=<STDIN>;
chomp($judge);
if ($judge eq "y") 
{

	print "continue.... \n";
	system('./badroot.csh');
	system("$submitcommand");

}
else
{
	print "end here \n";
}



