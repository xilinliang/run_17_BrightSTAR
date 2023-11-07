void jobMacroNanoDstFMS_zerobias(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    AnRunDiffJetTreeMakerrun17RPFMS_zerobias(fileList, outName,"fms","zerobias");
//    AnRunDiffJetTreeMaker(fileList, outName,"eemc");
//    AnRunDiffEmJetTreeMakerEEmcSmd(fileList, outName,"eemc");
//    RunFmsHotChQaMaker(fileList, outName,-1);
}
