void jobMacroRun17RPFMSCorr(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    DiffJetRPFMSCorr(fileList, outName,"AN","fms");
}
