void jobMacroRun17QA(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    DiffJetAnaTreeQaRun17(fileList, outName,"fms");
    //AnRunDiffJetTreeMaker(fileList, outName,"fms");
//    AnRunDiffJetTreeMaker(fileList, outName,"eemc");
//    AnRunDiffEmJetTreeMakerEEmcSmd(fileList, outName,"eemc");
//    RunFmsHotChQaMaker(fileList, outName,-1);
}
