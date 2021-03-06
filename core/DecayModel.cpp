#include "Manager.h"
#include "DecayModel.h"
#include "DecayingParticle.h"
#include <algorithm>

namespace elSpectro{

  ///////////////////////////////////////////////////////////////////////
  /// construct with particles, derived classes need the same constructor
  DecayModel::DecayModel( particle_ptrs ps, const std::vector<int> pdgs){

    //first add pre-existing particles
    std::copy(std::begin(ps), std::end(ps), std::back_inserter(_products));
    
    //now the non decaying particles
    auto& pman = Manager::Instance().Particles();
    for(const auto pdg : pdgs)
      _products.push_back( pman.Take( new Particle{pdg} ) );

    //store list of unstable particles which decay
    for(auto prod: _products){ 
      auto dp=dynamic_cast<DecayingParticle*>(prod);
      if(dp!=nullptr)
	_unstables.push_back(dp);
      else
	_stables.push_back(prod);
    }
	    
  }

  void DecayModel::PostInit(ReactionInfo* info){
    if(_unstables.empty()) return;
    for(auto& p:_unstables){
      std::cout<<"DecayModel::PostInit  "<<p<<" "<<_name<<std::endl;
      std::cout<<p->Pdg()<<std::endl;
      p->PostInit(info);
    }
  };

  void  DecayModel::GetStableMasses( std::vector<double >& masses) const{

    //if stable particle add its mass
    for(auto* entry:_stables)
      masses.push_back(entry->PdgMass()); 
    
    //if unstable particla add its child masses
    for(auto* entry:_unstables)
      entry->Model()->GetStableMasses(masses);
    
  }
  double DecayModel::PhaseSpaceWeightSq(double W){
    // std::cout<<GetName()<<" DecayModel::PhaseSpaceWeightSq "<<MinimumMassPossible()<<" "<<W<<std::endl;
    //Note use weight squared to reduce sqrt calls

    double result=1;
    
    double TCM=W;
    for(auto* p:_unstables){
      //This should be the only call to DetermineDynamicMass in the code.
      p->DetermineDynamicMass();
      TCM-=p->Mass();
    }
    for(auto* p:_stables)
      TCM-=p->Mass();
    
    //  std::cout<<GetName()<<" DecayModel::PhaseSpaceWeightSq "<<TCM<<std::endl;
     
    if(TCM<0)  return 0;//below threshold, start again
    
    result  *= kine::PDK2(W,_products[0]->Mass(),_products[1]->Mass());
    // std::cout<<GetName()<<" DecayModel::PhaseSpaceWeightSq result "<<result<<std::endl;
   
    for(auto* p:_unstables)
      result*=p->PhaseSpaceWeightSq();
    
    return result;
    
  }
  
  void DecayModel::Print() const{
    std::cout<<"DecayModel::Print() "<<" "<<GetName()<<std::endl;
    //Intensity();
    // std::cout<<"            Intensity= "<<_myInfo->_weight<<std::endl;
    for(const auto& p:_products){
      p->Print();
    }
  }

}
