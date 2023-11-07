void jobMacroRun17Sys_smallBBC(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    DiffJetXFCalcRun17(fileList, outName,"smallBBC","fms");
}
