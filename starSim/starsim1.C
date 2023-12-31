/**!
 * Example macro for running an event generator in standalone mode.
 *
 * Use cases:
 *   1) Generate input root files for geant simulation
 *   2) Generate root files for quick analysis.  
 *
 * NOTE:  By default, a number of particles are made stable (e.g. pi0, K0, etc...),
 *        to allow GEANT to handle the decays.  If this is not desired, change the
 *        flag below.
 *
 * Usage:
 *
 * root4star
 * .L standalone.pythia6.C
 * int nevents=100;
 * standalone( nevents )
 */

const bool GEANT_HANDLES_DECAYS = true;

class St_geant_Maker;
St_geant_Maker *geant_maker = 0;

class StarGenEvent;
StarGenEvent   *event       = 0;

class StarPrimaryMaker;
StarPrimaryMaker *primary = 0;

class StarPythia8;
StarPythia8* _pythia8;

//add 
class StarFilterMaker;
StarFilterMaker *filter = 0;

TString LHAPDF_DATA_PATH="./current/";

void geometry( TString tag, Bool_t agml=true )
{
  TString cmd = "DETP GEOM "; cmd += tag;
  if ( !geant_maker ) geant_maker = (St_geant_Maker *)chain->GetMaker("geant");
  geant_maker -> LoadGeometry(cmd);
  //  if ( agml ) command("gexec $STAR_LIB/libxgeometry.so");
}

void command( TString cmd )
{
  if ( !geant_maker ) geant_maker = (St_geant_Maker *)chain->GetMaker("geant");
  geant_maker -> Do( cmd );
}

// ----------------------------------------------------------------------------
void trig( Int_t n=1 )
{
  for ( Int_t i=0; i<n; i++ ) {
    chain->Clear();
    chain->Make();
  }
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Pythia6( TString mode="pp:W", Int_t tune=320 )
{
  
  //  gSystem->Load( "libStarGeneratorPoolPythia6_4_23.so" );
  gSystem->Load( "libPythia6_4_28.so");

  StarPythia6 *pythia6 = new StarPythia6("pythia6");
  if ( mode=="pp:W" )
  {
    pythia6->SetFrame("CMS", 510.0 );
    pythia6->SetBlue("proton");
    pythia6->SetYell("proton");
    if ( tune ) pythia6->PyTune( tune );

    // Setup pythia process
    PySubs_t &pysubs = pythia6->pysubs();
    int& msel = pysubs.msel;
    msel = 12;
    
    // Setup other common block variables / array elements
    float& ckin3 = pysubs.ckin(3); 
    ckin3 = 4.0;

    //
    // Set particles to be stable so that the decay manager
    // can handle them in the starsim phase
    //
    pythia6 -> SetDecayFlag( +24, 0 ); // W+
    pythia6 -> SetDecayFlag( -24, 0 ); // W-
    pythia6 -> SetDecayFlag( +23, 0 ); // Z0
    pythia6 -> SetDecayFlag( -23, 0 ); // Z0
    pythia6 -> SetDecayFlag( +15, 0 ); // tau+
    pythia6 -> SetDecayFlag( -15, 0 ); // tau-
    

  }
  if ( mode == "pp:minbias" )
  {
    pythia6->SetFrame("CMS", 510.0 );
    pythia6->SetBlue("proton");
    pythia6->SetYell("proton");
    if ( tune ) pythia6->PyTune( tune );
  }
  if ( mode == "ep" )
  {
    Double_t pblue[]={0.,0.,30.0};
    Double_t pyell[]={0.,0.,-320.0};
    pythia6->SetFrame("3MOM", pblue, pyell );
    pythia6->SetBlue("e-");
    pythia6->SetYell("proton");
    if ( tune ) pythia6->PyTune( tune );
  }
    
  primary->AddGenerator(pythia6);
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Pythia8( TString config="pp:W", const char* _library="libPythia8_1_62.so" )
{
  gSystem->Load( _library );

  //
  // Create the pythia 8 event generator and add it to 
  // the primary generator
  //
  StarPythia8 *pythia8 = new StarPythia8();    
  _pythia8=pythia8;
  if ( config=="pp:W" )
    {
      pythia8->SetFrame("CMS", 510.0);
      pythia8->SetBlue("proton");
      pythia8->SetYell("proton");
      
      pythia8->Set("WeakSingleBoson:all=off");
      pythia8->Set("WeakSingleBoson:ffbar2W=on");
      pythia8->Set("24:onMode=0");              // switch off all W+/- decaus
      pythia8->Set("24:onIfAny 11 -11");        // switch on for decays to e+/-
      
    }
  if ( config=="pp:minbias" )
    {
      pythia8->SetFrame("CMS", 510.0);
      pythia8->SetBlue("proton");
      pythia8->SetYell("proton");            

      //pythia8->Set("SoftQCD:minBias = on");
      //pythia8->Set("SoftQCD:elastic = on");
      pythia8->Set("SoftQCD:singleDiffractive = on");
	//pythia8->Set("Diffraction:doHard = on");
    }

  primary -> AddGenerator( pythia8 );
  
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void starsim1( Int_t nevents=1000, UInt_t rngSeed = 12345 )
{ 

  gROOT->ProcessLine(".L bfc.C");
  
//    TString simple = "tables nodefault";
    TString simple = "y2017 geant gstar agml usexgeom ";
    //TString simple = "y2011b geant gstar agml usexgeom -db -detdb";
    bfc(0, simple );
  

  gSystem->Load( "libVMC.so");
  gSystem->Load( "St_g2t.so" );
  gSystem->Load( "St_geant_Maker.so" );
 
  gSystem->Load( "StarGeneratorUtil.so" );
  gSystem->Load( "StarGeneratorEvent.so" );
  gSystem->Load( "StarGeneratorBase.so" );
  gSystem->Load( "StarGeneratorFilt.so" );

  gSystem->Load( "libMathMore.so"   );  
  gSystem->Load( "xgeometry.so"     );

  //
  // Create the primary event generator and insert it
  // before the geant maker
  //
  //  StarPrimaryMaker *
  primary = new StarPrimaryMaker();
  {
    primary -> SetFileName( Form("pythia8_%i.starsim.root",rngSeed));
      chain -> AddBefore( "geant", primary );
  }

  //
  // Setup an event generator
  //
  Pythia8( "pp:minbias" );


  //
  // Initialize random number generator
  //
  StarRandom &random = StarRandom::Instance();
  random.capture(); // maps all ROOT TRandoms to StarRandom
  random.seed( rngSeed );


  //
  // Setup cuts on which particles get passed to geant for
  //   simulation.  (To run generator in standalone mode,
  //   set ptmin=1.0E9.)
  //                    ptmin  ptmax
  primary->SetPtRange  (0.0,  -255.0);         // GeV
  //                    etamin etamax
  //primary->SetEtaRange ( 2.5, 4.0 );
  //                    phimin phimax
  primary->SetPhiRange ( 0., TMath::TwoPi() );
  
  
  // 
  // Fixed x, y, z vertex
  // 
  primary->SetVertex( 0., 0., 0. );
  primary->SetSigma( 0., 0.1, 0. );

  //
  // Initialize primary event generator and all sub makers
  //
  primary -> Init();

  //
  // By default we have configured pythia8 such that the following
  // particles are stable, so that geant is responsible for decaying
  // them.  If you need pythia to decay these for analysis, enable
  // the following code block...
  //
  if ( GEANT_HANDLES_DECAYS ) {
    _pythia8->Set("111:onMode=1"); // pi0 
    _pythia8->Set("211:onMode=1"); // pi+/-                         
    _pythia8->Set("221:onMode=1"); // eta                              
    _pythia8->Set("321:onMode=1"); // K+/-                             
    _pythia8->Set("310:onMode=1"); // K short                                               
    _pythia8->Set("130:onMode=1"); // K long                                               
    _pythia8->Set("3122:onMode=1"); // Lambda 0                                             
    _pythia8->Set("3112:onMode=1"); // Sigma -                                              
    _pythia8->Set("3222:onMode=1"); // Sigma +                                              
    _pythia8->Set("3212:onMode=1"); // Sigma 0                                              
    _pythia8->Set("3312:onMode=1"); // Xi -                                                 
    _pythia8->Set("3322:onMode=1"); // Xi 0                                                 
    _pythia8->Set("3334:onMode=1"); // Omega -              
  }

  geometry("y2017a");
  command("gkine -4 0");
  command(Form("gfile o pythia8_%i.starsim.fzd",rngSeed));


  //
  // Trigger on nevents
  //
  //trig( nevents );

// ----------------------------------------------------------------------------
  StarGenEvent* event = primary->event();
  for ( Int_t i=0; i<nevents; i++ ) {
      chain->Clear();
      chain->Make();

  }
  command("call agexit");  // Make sure that STARSIM exits properly

}
