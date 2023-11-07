void jobMacroRun17Sys_Esum(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    DiffJetXFCalcRun17(fileList, outName,"Esum","fms");
}
