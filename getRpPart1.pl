#!/usr/bin/perl
##use strict;
##use warnings;

my $current_folder=$ENV{'PWD'};
chomp($current_folder);

my $totaljobs = 2000;
my $NjobsPerFile = 20;


for ($i=1000;$i<$totaljobs;$i++)
#for ($i=2000;$i<4000;$i++)
{

	if ($i % $NjobsPerFile ==0)
	{
		$file_id = $i / $NjobsPerFile;
		$outputFile=$current_folder."/FmsSimParticleCheck_P$file_id".".C";
		$outputFilename="FmsSimParticleCheck_P$file_id";
		print "$outputFile \n";
		open(Output,"+>$outputFile") or die ("can not open $outputFile");
		print Output "void $outputFilename".'()'."\n";

                print Output '{'."\n";	
	}

	print Output "FmsSimParticleCheck(-1,$i); \n";

	if ($i % $NjobsPerFile == ($NjobsPerFile - 1))
	{
		print Output '}';
		close(Output);
		system("root4star -b -q $outputFile \n");
	}

};

close(Input);
close(Output);

