void jobMacroRun17Sys_Ecorr(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    DiffJetXFCalcRun17(fileList, outName,"Ecorr","fms");
}
