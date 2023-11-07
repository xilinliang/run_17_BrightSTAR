
#ifdef __CINT__


#pragma link C++ function AnRunDiffJetTreeMaker;                 // Diffractive Jet     Tree Maker for EM jet
#pragma link C++ function AnRunDiffJetTreeMakerGetBBC;                 // Diffractive Jet   Tree Maker for EM jet (get BBC from st_rp stream, run 17)
#pragma link C++ function AnRunDiffJetTreeMakerrun17RPEEMC;                 // Diffractive Jet     Tree Maker for run 17 EEMC EM jet with st_rp stream
#pragma link C++ function AnRunDiffJetTreeMakerrun17RPFMS;                 // Diffractive Jet     Tree Maker for run 17 FMS EM jet with st_rp stream
#pragma link C++ function AnRunDiffJetTreeMakerrun17RPFMS_zerobias;                 // Diffractive Jet     Tree Maker for run 17 zerobias
#pragma link C++ function AnRunFmsPi0RpTreeMaker;                 // Diffractive Jet (FMS + RP) Tree Maker for pi0
#pragma link C++ function AnRunEEMCpi0RpTreeMaker;                 // Diffractive Jet (EEMC + RP) Tree Maker for pi0
#pragma link C++ function AnRun17RPTreeMaker;                 // Diffractive Jet RP Tree Maker for run 17 RP stream only

#pragma link C++ function DiffJetGetRunList;                 // Diffractive Jet Get run list

#pragma link C++ function DiffJetRpCheck;                 // Diffractive Jet RP check for run 17 RP stream only
#pragma link C++ function DiffJetAnaTreeQa;                 // Diffractive Jet basic QA
#pragma link C++ function DiffJetXFCalc;                 // Diffractive Jet XF Calc for run 15
#pragma link C++ function DiffJetXFANCalc;                 // Diffractive Jet XF A_N Calc for run 15
#pragma link C++ function DiffJetXFANCalc_5pt;                 // Diffractive Jet XF A_N Calc for run 17 [0.1,0.15], [0.15,0.2] , [0.2, 0.25], [0.25,0.3], [0.3,0.45]
#pragma link C++ function DiffJetXFANCalc_4pt;                 // Diffractive Jet XF A_N Calc for run 17 [0.1,0.15], [0.15,0.2] , [0.2, 0.25], [0.25,0.45]
#pragma link C++ function DiffJetXFANCalc_4pt_bkg;                 // Diffractive Jet XF A_N Calc for run 17 [0.1,0.15], [0.15,0.2] , [0.2, 0.25], [0.25,0.45] (bkg)
#pragma link C++ function DiffJetXFANCalc_12np_7pt;                 // Diffractive Jet XF A_N Calc for run 15 (1 & 2 photon multi)
#pragma link C++ function DiffJetXFANCalc_12np_5pt;                 // Diffractive Jet XF A_N Calc for run 17 [0.1,0.15], [0.15,0.2] , [0.2, 0.25], [0.25,0.3], [0.3,0.45] (1 & 2 photon multi)
#pragma link C++ function DiffJetXFANCalc_12np_4pt;                 // Diffractive Jet XF A_N Calc for run 17 [0.1,0.15], [0.15,0.2] , [0.2, 0.25], [0.25,0.45] (1 & 2 photon multi) measured AN
#pragma link C++ function DiffJetXFANCalc_12np_4pt_bkg;                 // Diffractive Jet XF A_N Calc for run 17 [0.1,0.15], [0.15,0.2] , [0.2, 0.25], [0.25,0.45] (1 & 2 photon multi) bkg AN


#pragma link C++ function DiffJetAnaTreeQaRun17;                 // Diffractive Jet basic QA for run 17
#pragma link C++ function DiffJetXFCalcRun17;                 // Diffractive Jet XF Calc for run 17
#pragma link C++ function DiffJetXFCalcRun17_WestRP;                 // Diffractive Jet XF Calc for run 17 (1 west RP track case)
#pragma link C++ function DiffJetRPFMSCorr;                 // Diffractive Jet Roman Pot FMS correlation for run 17


#endif

