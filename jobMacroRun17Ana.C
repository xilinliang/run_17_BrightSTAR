void jobMacroRun17Ana(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    DiffJetXFCalcRun17(fileList, outName,"AN","fms");
}
