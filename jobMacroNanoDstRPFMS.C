void jobMacroNanoDstRPFMS(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
//    AnRunDiffJetTreeMaker(fileList, outName,"fms");
    //AnRunDiffJetTreeMaker(fileList, outName,"eemc");
    AnRunDiffJetTreeMakerrun17RPFMS(fileList, outName,"rp");
//    AnRunDiffEmJetTreeMakerEEmcSmd(fileList, outName,"eemc");
//    RunFmsHotChQaMaker(fileList, outName,-1);
}
