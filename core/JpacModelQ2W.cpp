#include "JpacModelQ2W.h"
#include "TwoBody_stu.h"
#include "FunctionsForJpac.h"
#include "Interface.h" //for generator

namespace elSpectro{
  ///////////////////////////////////////////////////////
  ///constructor includes subseqent decay of g*N system
  ///this class controls decay of e+p ->e' + g*N
  JpacModelQ2W::JpacModelQ2W( jpacPhoto::amplitude* amp ,
			      particle_ptrs parts, const std::vector<int> pdgs):
    _amp{amp},
    DecayModelQ2W{0, new JpacModelst{amp, parts, pdgs},
	new TwoBody_stu{0,1,0.5,0,0} }
    // ->   DecayModel{{ new DecayingParticle{-2211,Decay_st} },{11}}
  {
    _name={"JpacModelQ2W"};

   
  }
  ///////////////////////////////////////////////////////
  ///constructor includes subseqent decay of Ngamma* system with s and t channel
  JpacModelQ2W::JpacModelQ2W( jpacPhoto::amplitude* amp ,
			      particle_ptrs parts, const std::vector<int> pdgs,
			      double s_strength,
			      double t_strength,double t_slope):
    _amp{amp},
    DecayModelQ2W{0, new JpacModelst{amp, parts, pdgs},
	new TwoBody_stu{s_strength,t_strength, t_slope,0,0} }
    // ->   DecayModel{{ new DecayingParticle{-2211,Decay_st} },{11}}
  {
    _name={"JpacModelQ2W_given_s_and_t"};
  }

  ////////////////////////////////////////////////////////
  void JpacModelQ2W::PostInit(ReactionInfo* info){

    DecayModelQ2W::PostInit(info);

    //declare the model for controlling phase space
    //this is the decay of the overall hadronic system
    generator().SetModelForMassPhaseSpace(GetGammaN()->Model());
    
    //JPAC model is for photoproduction
    //Leave Q2 dependence for now
    //W dependence requires integration over W
    //Do we need this or just evaluate at each s and t via JpacModelst
    auto prodInfo=ProdInfo();
    double maxW = ( *(prodInfo->_target) + *(prodInfo->_ebeam) ).M();

    std::cout<<"JpacModelQ2W::PostInit generating total cross section, may take some time... "<<std::endl;
    TH1D hist("Wdist","Wdist",100,0,maxW);
    jpacFun::HistProbabilityDistribution_s(_amp,hist);
    
    auto maxVal= hist.GetMaximum();
    //add additional 10% of max to raise envelope
    for(int ibin=1;ibin<=hist.GetNbinsX();ibin++)
      hist.SetBinContent(ibin,hist.GetBinContent(ibin)+maxVal*0.1);
    
    _W_Dist.reset( new DistTH1(hist) );
    
    /* std::cout<<"JpacModelQ2W check interpolation "<<std::endl;
    for(int iW=0;iW<1000;iW++){
      double Wval=double(iW)/1000*maxW;
      std::cout<<Wval<<" "<<_W_Dist->GetWeightFor( Wval )<<" or "<<hist.Interpolate(Wval)<<std::endl;

    }
    exit(0);
    */
  }
  double JpacModelQ2W::Intensity() const{
    //  std::cout<<"DecayModelQ2W::Intensity "<<MinimumMassPossible()<<" "<<GetGammaN()->MinimumMassPossible()<<" "<<ProdInfo()->_photoN->M()<<" meson "<<GetDecayMeson()->Mass()<<std::endl;
    if(CheckThreshold()==false){
      return 0.;
    }
    // std::cout<<"Mass check "<<GetGammaN()->P4().M() <<" "<< GetGammaN()->MinimumMassPossible()<<std::endl;
    if(GetGammaN()->P4().M()<GetGammaN()->MinimumMassPossible()) return 0;
    
    //    if(GetGammaN()->P4().M() < GetGammaN()->MinimumMassPossible())
    // std::cout<<"DecayModelQ2W::Intensity "<<MinimumMassPossible()<<" passed "<<std::endl;
 
    auto prodInfo=ProdInfo(); 
    //if(prodInfo->_photoN->M()<25)return 0;
 
    _gamma = *(prodInfo->_ebeam) - *(prodInfo->_scattered);

    if(_W_Dist.get()==nullptr) return 1.;
    
    double weight=_W_Dist->GetWeightFor( prodInfo->_photoN->M() );
    // std::cout<<"         JpacModelQ2W::Intensity()  "<<prodInfo->_photoN->M()<<" "<<weight<<std::endl;
    prodInfo->_sWeight=weight*1; //might be used in s and t
    
    return weight;
  }
  
  
}
