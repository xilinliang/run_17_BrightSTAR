void jobMacroNanoDstEEMCpi0(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
//    AnRunDiffJetTreeMaker(fileList, outName,"fms");
    AnRunEEMCpi0RpTreeMaker(fileList, outName);
//    AnRunDiffEmJetTreeMakerEEmcSmd(fileList, outName,"eemc");
//    RunFmsHotChQaMaker(fileList, outName,-1);
}
