
#ifdef __CINT__


#pragma link C++ function FmsSimRunStarsimAndBfc;                // Run Pythia + Geant3 (starsim) + BFC for FMS Jet Simulation
#pragma link C++ function RunFmsJetFinderPro;                    // Run FMS Jet Finder for Simulated Data (used for analysis)
#pragma link C++ function FmsSimMakeResponseMatrix;              // Generate Response Matrix or 2D hist for particle vs detector level quantities
#pragma link C++ function FmsSimParticleCheck;                   // Make QA plots for the simulation events only in particle level simulation
#pragma link C++ function FmsSimJetQa;                   // Make QA plots for the simulation events for EM-jets


#endif

