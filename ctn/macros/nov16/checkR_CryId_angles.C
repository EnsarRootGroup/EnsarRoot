////////////////////////////////////////////////////////////////////////////
////																	
////		--- Simulation of the Lisbon Nov-2016 setup ---				
////																	
////		Macro to analyze Crystal angles Petals						
////			theta&phi angles for each CryId							
////																	
////		Usage:														
////			1st: select the root file & the ranges (OPTIONAL)		
////			2nd: root -l checkR_CryId_angles.C						
////																	
////		Calculate: 													
////			-Crystal: ID, Theta, Phi and 							
////					there are some func(utilities.C) to calculate	
////					Mean, Leftedge, Center of the Histos 			
////																	
//// **elisabet.galiana@usc.es											
//// ** Universidad de Santiago de Compostela							
//// ** Dpto. Física de Partículas 										
////////////////////////////////////////////////////////////////////////////

//NOTE1: if you want to analyze the CrystalPoint
//      you have to activate it before to execute runsim.C,
//		in order to create its branch
//
//		How to activate it: comment/descomment these lines
//		Califa: calo/cal/R3BCalo.cxx
//				->FairRootManager::Instance()->Register("CrystalPoint", GetName(), fCaloCollection, kTRUE);
//		Then, you have to do "make" again in the EnsarRoot build directory and execute runsim.C  

//NOTE2: it is better to use a low energy gamma simulation (100-200 keV)
//		 because the gamma arrives and stops at the same crystal 

#include "TMath.h"
#include "utilities.C"

void checkR_CryId_angles() {

	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE  
	char inputFile[250] = "outsim.root"; //select root file                                      
	//char inputFile[250] = "/mnt/scratch/eli/outsim_gamma_200kev.root"; //		Isotropic simulation
	//char inputFile[250] = "/mnt/scratch/eli/outsim_g_200keV_W2.root";	 //		Angular Correlations simulation
	TFile *file1 = TFile::Open(inputFile);

	//READING TREE
	TTree* tree = (TTree*)file1->Get("ensartree");

	//HISTOGRAMS DEFINITION--------------------------------------------------------------------//Change these ranges
	//TH1F* h_Cry_1 = new TH1F("h_Cry_1","Primary Gammas Hits of each Crystal",130.,0.,130.);	
	//TH2F* h_MC_2 = new TH2F("h_MC_2","Theta_MC of each Crystal", 128.,0.,128.,360,0.,3.6);
	//TH2F* h_MC_3 = new TH2F("h_MC_3","Phi_MC of each Crystal", 128.,0.,128.,720,-3.6,3.6);
	
	const Int_t n=128; //CryId total number
	char hname[20];
	
	TH1F *histos[n];
	
	for(Int_t i=0;i<n;i++){
		
		//sprintf(hname,"hTheta_%d",i+1);
		sprintf(hname,"hPhi_%d",i+1);
		//histos[i]= new TH1F(hname,"Theta Angle for CryId",1800.,0.,180.);//180*100=18000 Theta angle
		histos[i]= new TH1F(hname,"Phi Angle for CryId",3600.,-180.,180.);//   Phi angle
	}
	
	//----   MCTrack (input)   -------------------------------------------------------
	TClonesArray* MCTrackCA;
	EnsarMCTrack** track;
	MCTrackCA = new TClonesArray("EnsarMCTrack",5);
	TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
	branchMCTrack->SetAddress(&MCTrackCA);
	
	
	
	//----- CALIFA detector--------------//
	//Crystal Hits  
	TClonesArray* crystalHitCA;  
	R3BCaloCrystalHitSim** crystalHit;
	crystalHitCA = new TClonesArray("R3BCaloCrystalHitSim",5);
	TBranch *branchCrystalHit = tree->GetBranch("CrystalHitSim");
	branchCrystalHit->SetAddress(&crystalHitCA);

	//Calo Hits
	/*TClonesArray* caloHitCA;  
	R3BCaloHitSim** caloHit;
	caloHitCA = new TClonesArray("R3BCaloHitSim",5);
	TBranch *branchCaloHit = tree->GetBranch("CaloHitSim");
	branchCaloHit->SetAddress(&caloHitCA);*/
	
	
	//Crystal Points
	/*TClonesArray* crystalPointCA;  
	R3BCaloPoint** crystalPoint;
	crystalPointCA = new TClonesArray("R3BCaloPoint",5);
	TBranch *branchcrystalPoint = tree->GetBranch("CrystalPoint");
	branchcrystalPoint->SetAddress(&crystalPointCA);*/


	Int_t MCtracksPerEvent = 0;
	Int_t crystalHitsPerEvent = 0;
	//Int_t caloHitsPerEvent=0;
	//Int_t crystalPointPerEvent=0;
	
	const Double_t pi  = TMath::Pi();
    Int_t CryId=0.;
	TVector3 momentum;
	Double_t Theta=0.;
	Double_t Theta2=0.;
	Double_t Phi=0.;
	Double_t Phi2=0.;
   
    

	//TREE ENTRIES--------------------------------------------------------------------
	Long64_t nevents = tree->GetEntries();
   
        
	//LOOP IN THE EVENTS--------------------------------------------------------------
	for(int i=0;i<nevents;i++){
	    	if(i%1== 100) printf("Event:%i\n",i);
		energyGe = 0.;
		tree->GetEvent(i);

		MCtracksPerEvent     = MCTrackCA->GetEntries();
		crystalHitsPerEvent  = crystalHitCA->GetEntries();
		//caloHitsPerEvent     = caloHitCA->GetEntries();
		//crystalPointPerEvent = crystalPointCA->GetEntries();

		if(MCtracksPerEvent>0) {
			track = new EnsarMCTrack*[MCtracksPerEvent];
			for(Int_t j=0;j<MCtracksPerEvent;j++){
				track[j] = new EnsarMCTrack;
				track[j] = (EnsarMCTrack*) MCTrackCA->At(j);
			}
		}
		if(crystalHitsPerEvent>0) {
			crystalHit = new R3BCaloCrystalHitSim*[crystalHitsPerEvent];
			for(Int_t j=0;j<crystalHitsPerEvent;j++){
				crystalHit[j] = new R3BCaloCrystalHitSim;
				crystalHit[j] = (R3BCaloCrystalHitSim*) crystalHitCA->At(j);      
			}
		}
		/*if(caloHitsPerEvent>0) {
			caloHit = new R3BCaloHitSim*[caloHitsPerEvent];
			for(Int_t j=0;j<caloHitsPerEvent;j++){
				caloHit[j] = new R3BCaloHitSim;
				caloHit[j] = (R3BCaloHitSim*) caloHitCA->At(j);      
			}
		}*/
		/*if(crystalPointPerEvent>0) {
			crystalPoint = new R3BCaloPoint*[crystalPointPerEvent];
			for(Int_t j=0;j<crystalPointPerEvent;j++){
				crystalPoint[j] = new R3BCaloPoint;
				crystalPoint[j] = (R3BCaloPoint*) crystalPointCA->At(j);      
			}
		}*/
		

	
		
		 //LOOP in Crystal Hits----------------    
		 for(Int_t j=0;j<crystalHitsPerEvent;j++){
					//cout<<"CrystalId= "<<crystalHit[i]->GetCrystalId()<<endl;
					//cout<<"CrystalType= "<<crystalHit[i]->GetCrystalType()<<endl;
					//cout<<"CrystalCopy= "<<crystalHit[i]->GetCrystalCopy()<<endl;
			
			CryId=crystalHit[j]->GetCrystalId();		
		}
				
		//LOOP in MC mother tracks----------------
		for(Int_t i=0;i<MCtracksPerEvent;i++){
			if(track[i]->GetMotherId()<0) { //Primary Particle is MotherId=-1
		
				track[i]->GetMomentum(momentum);
 				Theta = momentum.Theta();
 				Theta2= Theta*180./pi;
               	Phi   = momentum.Phi();
               	Phi2  = Phi*360./(2*pi);
                                
                
			}
		}
		
		//Fill the Array of Histograms
		if (crystalHitsPerEvent){
                
        	histos[CryId-1]->Fill(Theta2);
        	//histos[CryId-1]->Fill(Phi2);
        
        } 
		

		if(MCtracksPerEvent)     delete[] track;
		if(crystalHitsPerEvent)  delete[] crystalHit;
		//if(caloHitsPerEvent)     delete[] caloHit;
		//if(crystalPointPerEvent) delete[] crystalPoint;

	}
	// END LOOP IN THE EVENTS---------------------------------------------------------
	
	
	//FUNCTIONS to find the Center and the LeftEdge of each crystal
	
	cout<<"CryId  Mean  Center  CrystalWidth"<<endl;
	
	for(Int_t i=0;i<n;i++){
		
		//cout << i+1 << "\t" << getCenter(histos[i]) << endl;
		Float_t LeftEdge;
		Float_t Center;
		Float_t CryWidth;
		//Float_t Mean;
		
		//Float_t Mean = histos[i]->GetMean();
		
		//LeftEdge= getLeftEdge(histos[i]);
		//Center  = - getCenter(histos[i]);
		//CryWidth= Center - LeftEdge;
		//cout<<i+1<<"**    "<<Mean<<" ***   "<<"    "<<Center<<"  ****  "<<CryWidth<<endl;
	}
	
	
/********************************************************************
	
// Fuction which calculates T_left-T_right histogram center 
// Performs an algorithm for finding the edges of the Left-Right time difference histogram
// Returns the sum of the edges divided by 2 changing its sign
Float_t getCenter(TH1F *histo)
{

	Int_t nbins = histo->GetNbinsX();
	Float_t val=0;
	Float_t left_edge=0., right_edge=0.;
	Float_t center=0.;

	for(Int_t i=1;i<nbins;i++) {

		Float_t max = histo->GetBinContent(histo->GetMaximumBin());
		Float_t con = histo->GetBinContent(i);
		histo->SetBinContent(i,con-(max/5));
		if(histo->GetBinContent(i)<0) histo->SetBinContent(i,0);

	}

	for(Int_t i=1;i<nbins;i++) {

		val = histo->GetBinContent(i);

		if(val>0) {
			if(histo->GetBinContent(i-1)==0 && histo->GetBinContent(i+1)>0 && histo->GetBinContent(i+2)>0 && histo->GetBinContent(i+3)>0 && histo->GetBinContent(i+4)>0)
					//&& histo->GetBinContent(i+5)>0 && histo->GetBinContent(i+6)>0	&& histo->GetBinContent(i+7)>0 && histo->GetBinContent(i+8)>0)
				left_edge = histo->GetBinLowEdge(i);
			if(histo->GetBinContent(i+1)==0 && histo->GetBinContent(i-1)>0 && histo->GetBinContent(i-2)>0 && histo->GetBinContent(i-3)>0 && histo->GetBinContent(i-4)>0)
					//&& histo->GetBinContent(i-5)>0 && histo->GetBinContent(i-6)>0	&& histo->GetBinContent(i-7)>0 && histo->GetBinContent(i-8)>0)
				right_edge = histo->GetBinLowEdge(i) + histo->GetBinWidth(i);
		}

		if(fabs(left_edge)>0 && fabs(right_edge)>0) break;

	}

	center = (right_edge + left_edge)/2;

	return -center;

}

// Fuction which calculates T_left-T_right histogram edges 
// Performs an algorithm for finding the edges of the Left-Right time difference histogram
// Returns the left edge
Float_t getLeftEdge(TH1F *histo)
{

  Int_t nbins = histo->GetNbinsX();
  Float_t val=0;
  Float_t left_edge=0.;

	for(Int_t i=1;i<nbins;i++) {

		Float_t max = histo->GetBinContent(histo->GetMaximumBin());
		Float_t con = histo->GetBinContent(i);
		histo->SetBinContent(i,con-(max/8));
		if(histo->GetBinContent(i)<0) histo->SetBinContent(i,0);

	}

  for(Int_t i=1;i<nbins;i++) {

    val = histo->GetBinContent(i);

    if(val>0) {
      if(histo->GetBinContent(i-1)==0 && histo->GetBinContent(i+1)>0 && histo->GetBinContent(i+2)>0 && histo->GetBinContent(i+3)>0
          && histo->GetBinContent(i+4)>0 && histo->GetBinContent(i+5)>0)
        left_edge = histo->GetBinLowEdge(i);
    }

    if(fabs(left_edge)>0) break;

  }

  return left_edge;

}
***************************************************/
	

	// HISTOGRAMS--------------------------------------------------------------------- 
	
	Int_t j;
	j=10; //change it!
	
	//CrystalHit
	TCanvas* c1 = new TCanvas("Theta Angle for CryId","Theta Angle for CryId",0,0,1000,1000);          
	c1->SetFillColor(0);       
	c1->SetFrameFillColor(0); 
	c1->Divide(3,2); 
	
	c1->cd(1); 
	histos[j]->Draw();
	
	c1->cd(2); 
	histos[j+1]->Draw();
	
	c1->cd(3); 
	histos[j+2]->Draw();
	
	c1->cd(4); 
	histos[j+3]->Draw();
	
	c1->cd(5); 
	histos[j+4]->Draw();
	
	c1->cd(6); 
	histos[j+5]->Draw();
	

	TCanvas* c2 = new TCanvas("Angle for CryId","Angle for CryId",0,0,1000,1000);          
	c2->SetFillColor(0);       
	c2->SetFrameFillColor(0); 
	c2->Divide(3,2); 
	
	c2->cd(1); 
	histos[j+6]->Draw();
	
	c2->cd(2); 
	histos[j+7]->Draw();
	
	c2->cd(3); 
	histos[j+8]->Draw();
	
	c2->cd(4); 
	histos[j+9]->Draw();
	
	c2->cd(5); 
	histos[j+10]->Draw();
	
	c2->cd(6); 
	histos[j+11]->Draw();
	

}

