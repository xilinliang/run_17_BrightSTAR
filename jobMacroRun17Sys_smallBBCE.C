void jobMacroRun17Sys_smallBBCE(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    DiffJetXFCalcRun17(fileList, outName,"smallBBCE","fms");
}
