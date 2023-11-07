void jobMacroSimAndBfc(int cycle, int events)
{

	gROOT->Macro("rootlogon.C");
	FmsSimRunStarsimAndBfc(cycle, events);

}
