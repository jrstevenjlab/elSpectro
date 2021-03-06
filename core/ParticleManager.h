//////////////////////////////////////////////////////////////
///
///Class:		ParticleManager
///Description:
///            Class to manage particles
///            Should only be accessed via Manager::Particles()
///             1) Keep ownership of all particles
///             2) Construct particles via Create
///             3) Control output file particles
#pragma once

#include "DecayingParticle.h"
#include "Distribution.h"
#include <map>

namespace elSpectro{

  class Manager;
  
  using particle_ptrs= std::vector<Particle*> ;
  using decaying_ptrs= std::vector<DecayingParticle*> ;

  class ParticleManager{

  public:

    //take ownership of a particle
    //do not use p after calling this
    Particle* Take(Particle* p);

    void RegisterMassDistribution(int pdg, Distribution* dist){
      _massDist[pdg]=dist_uptr{dist};
    }
    /* void RegisterMassSquaredDistribution(int pdg, Distribution* dist){ */
    /*   _mass2Dist[pdg]=dist_uptr{dist}; */
    /* } */
    Distribution* GetMassDist(int pdg)const noexcept
    {return _massDist.at(pdg).get();}

    void BoostStable(const BetaVector& vboost ){
      std::for_each(_stables.begin(),_stables.end(),[&vboost](Particle* p){p->Boost(vboost);});
    }

    const decaying_ptrs UnstableParticles()const {return _unstables;}
    const particle_ptrs StableParticles()const {return _stables;}
    
  private:
    
    friend Manager; //only Manager can construct a ParticleManager
    ParticleManager();
    
    //all the particles in the generator
    std::vector<particle_uptr> _particles;
    
    particle_ptrs _stables; //products which are stable (to be detected)
    decaying_ptrs _unstables; //products which decay

    //distribitions for generating dynamic particle masses
    std::map<int,dist_uptr> _massDist; 
    // std::map<int,dist_uptr> _mass2Dist; 
    
    ClassDef(elSpectro::ParticleManager,1); //class ParticleManager
  };

}//namespace elSpectro
