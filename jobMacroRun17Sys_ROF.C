void jobMacroRun17Sys_ROF(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    DiffJetXFCalcRun17(fileList, outName,"ROF","fms");
}
