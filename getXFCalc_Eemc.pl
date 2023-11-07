#!/usr/bin/perl
##use strict;
##use warnings;

my $current_folder=$ENV{'PWD'};
chomp($current_folder);
my $folder='/star/data01/pwg/liangxl/BrightSTAR_EMjet';
$inputFile=$folder.'/EemcFillList.in';
open(Input,"<$inputFile") or die ("can not open $inputFile");

@inputlist=<Input>;
$number_input = @inputlist;
#my $totaljobs = 54;
my $nfillPerRun = 2;

for ($i=0;$i<$number_input;$i++)
{

	if ($i % $nfillPerRun ==0)
	{
		$file_id = $i / $nfillPerRun;
		$outputFile=$current_folder."/EemcXFCalc_P$file_id".".C";
		$outputFilename="EemcXFCalc_P$file_id";
		print $outputFile;
		open(Output,"+>$outputFile") or die ("can not open $outputFile");
		print Output "void $outputFilename".'()'."\n";
		
		print Output '{'."\n";
	}

	chomp($inputlist[$i]);
	my $fill = $inputlist[$i];
	#my $inputFilePrint = $folder."/condorjobResults/DiffJetNanoDstfms_0124fill/NanoDiffJetTree_fill$fill.root";
	my $inputFilePrint = $folder."/condorjobResults/DiffJetNanoDstEemc_060722fill_pT1/NanoDiffJetTree_fill$fill.root";
	my $outputFilePrint = $current_folder."/root_result/EemcANana/DiffJetXFCalc$fill"."_0622.root";

	print Output 'DiffJetXFCalc("'."$inputFilePrint".'","'."$outputFilePrint".'","eemc");'."\n";

	if (($i % $nfillPerRun ==($nfillPerRun - 1)) or ($i == ($number_input -1)))
	{
		print Output '}';
		close(Output);
		system("root4star -b -q $outputFile \n");
	}

};

close(Input);
close(Output);

