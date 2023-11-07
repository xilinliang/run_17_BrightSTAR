void jobMacroReco(int cycle, int events)
{

	gROOT->Macro("rootlogon.C");
	RunFmsJetFinderPro(cycle, events);

}
