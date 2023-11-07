void jobMacroNanoDstEEMC(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
//    AnRunDiffJetTreeMaker(fileList, outName,"fms");
    AnRunDiffJetTreeMaker(fileList, outName,"eemc");
//    AnRunDiffEmJetTreeMakerEEmcSmd(fileList, outName,"eemc");
//    RunFmsHotChQaMaker(fileList, outName,-1);
}
