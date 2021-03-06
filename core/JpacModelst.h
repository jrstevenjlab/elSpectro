//////////////////////////////////////////////////////////////
///
///Class:		JpacModelst
///Description:
///             Control behaviour of Particle decay to Particle products
///             Defined by
///             1) preconfigured jpacPhoto amplitude
///             2) it decay as a function of s and t JpacDecayst
///
///            Note derived classes should include a constructor to initialise
///            JpacModelst( particle_ptrs , const std::vector<int> pdgs );
#pragma once

#include "DecayModel.h"
#include "FunctionsForElectronScattering.h"
#include "amplitudes/amplitude.hpp"

namespace elSpectro{

  using jpacAmp_ptr = jpacPhoto::amplitude*;

  class JpacModelst : public DecayModel {

  public:
    
    JpacModelst()=delete;
    //constructor giving jpac amplitude pointer (which we will now own)
    //and decay particles 
    JpacModelst( jpacAmp_ptr amp, particle_ptrs parts,
		  const std::vector<int> pdgs  );
    
    //
    double Intensity() const override;

    void PostInit(ReactionInfo* info) override;

    bool RegenerateOnFail() const noexcept override {return true;};

    const Particle* GetMeson() const noexcept{return _meson; }
    const Particle* GetBaryon() const noexcept{return _baryon; }
 
  private:
    // mutable CurrentEventInfo _myInfo;//!

    jpacAmp_ptr _amp={nullptr}; //I am not the owner

    ReactionElectroProd* _prodInfo={nullptr};
 
    Particle* _baryon={nullptr};
    Particle* _meson={nullptr};
    LorentzVector _target={0,0,0,escat::M_pr()};
    
    mutable double _max={0};

    ClassDefOverride(elSpectro::JpacModelst,1); //class JpacModelst
    
  };//class JpacModelst

}//namespace elSpectro
