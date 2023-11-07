void jobMacroRun17Ana_WestRP(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    DiffJetXFCalcRun17_WestRP(fileList, outName,"AN","fms");
}
