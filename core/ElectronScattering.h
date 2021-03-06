//////////////////////////////////////////////////////////////
///
///Class:		ElectronScattering
///Description:
///            Class to manage intial electron scatter
///             1) Initial state particles
///             
///             2) DecayVectors for scattered electron
///                and g*p (e.g. ScatterEelectron_xy)
///             3) DecayModelQ2W for e'(g*p) production
///                to some final state e.g. e'rho+p
///                (e' probably remains unchanged)           

#pragma once
#include "ProductionProcess.h"
#include "PhaseSpaceDecay.h"

#include <TMath.h> //for Pi()

namespace elSpectro{
  

  class ElectronScattering : public ProductionProcess {

  public:
    
    //or if decayer already give required distribution
   
    ElectronScattering(double ep,double ionp,
		       double anglee,double anglep,
		       DecayVectors* decayer, DecayModel* model=new PhaseSpaceDecay{{},{11,-2211}},int ionpdg=2212);
    ElectronScattering(double ep,double ionp,
		       DecayVectors* decayer, DecayModel* model=new PhaseSpaceDecay{{},{11,-2211}},int ionpdg=2212);

    
    DecayStatus  GenerateProducts( ) override;

    // const CurrentEventInfo* EventInfo() const override{ return &_info; }

    void InitGen() override;

  private:
    ElectronScattering()=default;
 
    void SetBeamCondtion();
    
    double _pElectron; //nominal e- beam energy
    double _pIon; //nominal ion beam energy
    double _angleElectron; //nominal electron crossing angle
    double _angleIon;  //nominal proton crossing angle
    double _massIon;  //nominal proton crossing angle
    int _pdgIon; //species of ion
    

    LorentzVector _beamElec;
    LorentzVector _beamNucl;
    LorentzVector _nuclRestElec;
    LorentzVector _nuclRestNucl;

    DecayingParticle* _gStarN={nullptr}; 
    //PhotoProdInfo _info;
    ReactionElectroProd _reactionInfo;
    
    
    ClassDefOverride(elSpectro::ElectronScattering,1); //class ElectronScattering
 
  };


}//namespace elSpectro

