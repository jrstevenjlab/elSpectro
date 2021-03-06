//////////////////////////////////////////////////////////////
///
///Class:		DecayModel
///Description:
///             Control behaviour of Particle decay to Particle products
///             Defined by
///             1) list of Particle products
///             2) Intensity function given the produt LorentzVectors
///
///            Note drevied classes should include a constructor to initialise
///            DecayModel( particle_ptrs , const std::vector<int> pdgs );
#pragma once

#include "Particle.h"
#include "CurrentEventInfo.h"
#include "FunctionsForKinematics.h"
#include "ReactionInfo.h"
#include <TObject.h> //for ClassDef
#include <vector>
#include <string>
#include <numeric> //for accumulate

namespace elSpectro{

  class DecayingParticle;
  
  using particle_ptrs= std::vector<Particle*> ;
  using decaying_ptrs= std::vector<DecayingParticle*> ;
 
  class DecayModel {

  public:
    
    DecayModel()=default;
    //only declaring default constructor
    //so other 5 constructors also defaulted(rule of 5)

    //constructor to decay into particles
    DecayModel( particle_ptrs , const std::vector<int> pdgs );
    
    // Each model must define its intensity
    //    virtual double Intensity() const=0;
    virtual double  Intensity() const=0;

    const particle_ptrs& Products() const{ return _products;}
    const decaying_ptrs& UnstableProducts() const{ return _unstables;}

    virtual bool RegenerateOnFail() const noexcept =0;

    bool CheckThreshold() const{
      SumAllProducts();
      //  std::cout<<"CheckThreshold() "<<_sumOfMasses<<" "<<_parent.M()<<std::endl;
      if(_sumOfMasses>_parent.M()) return false;
      else return true;
    }
    void SumAllProducts( ) const {
      _parent.SetXYZT(0,0,0,0);
      _sumOfMasses=0;
      
      for(auto* entry:_products){
	_parent+=entry->P4();
	_sumOfMasses+=entry->P4().M();
      }
    }

    const LorentzVector& ParentVector()const {SumAllProducts( );return _parent;}

    double SumOfProductMasses()const {return _sumOfMasses;}

    virtual double MinimumMassPossible() const {
      double minmass=0;
      for(auto* entry:_products)
	minmass+=entry->MinimumMassPossible();
      return minmass;
    }
    void  GetStableMasses( std::vector<double >& masses) const;
    
    void Print() const;

    const std::string& GetName()const {return _name;}

    double PhaseSpaceWeightSq(double W);
    
    virtual void PostInit(ReactionInfo* info);
 
  protected:

    std::string _name;

  private:
     
    particle_ptrs _products;
    particle_ptrs _stables; //products which are stable
    decaying_ptrs _unstables; //products which decay

    mutable LorentzVector _parent;
    mutable double _sumOfMasses=0;

    
    ClassDef(elSpectro::DecayModel,1); //class DecayModel
    
  };//class DecayModel

}//namespace elSpectro
