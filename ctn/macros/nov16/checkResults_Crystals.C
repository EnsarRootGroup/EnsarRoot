
void checkResults_Crystals() {

	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE  
	char inputFile[250] = "outsim.root";                                            
	TFile *file1 = TFile::Open(inputFile);

	//READING TREE
	TTree* tree = (TTree*)file1->Get("ensartree");

	//HISTOGRAMS DEFINITION-----------------------------------------------------------//Change this maximum energies
	TH2F* h_Cry_1 = new TH2F("h_Cry_1","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);	
	TH2F* h_Cry_2 = new TH2F("h_Cry_2","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);
	TH2F* h_Cry_3 = new TH2F("h_Cry_3","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);
	TH2F* h_Cry_4 = new TH2F("h_Cry_4","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);
	TH2F* h_Cry_5 = new TH2F("h_Cry_5","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);
	TH2F* h_Cry_6 = new TH2F("h_Cry_6","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);	
	TH2F* h_Cry_7 = new TH2F("h_Cry_7","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);
	TH2F* h_Cry_8 = new TH2F("h_Cry_8","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);
	
	TH2F* h_Cry_9 = new TH2F("h_Cry_9","Theta vs Phi both petals", 720,0.,3.6,720,-3.6,3.6);
	
	TH2F* h_Cry_10 = new TH2F("h_Cry_10","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);
	TH2F* h_Cry_11 = new TH2F("h_Cry_11","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);
	TH2F* h_Cry_12 = new TH2F("h_Cry_12","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);
	TH2F* h_Cry_13 = new TH2F("h_Cry_13","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);

	//----   MCTrack (input)   -------------------------------------------------------
	/*TClonesArray* MCTrackCA;
	EnsarMCTrack** track;
	MCTrackCA = new TClonesArray("EnsarMCTrack",5);
	TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
	branchMCTrack->SetAddress(&MCTrackCA);*/
	
	//----- HPGe detector--------------//

	//HPGe Hits 
	TClonesArray* hpgeHitCA;
	EnsarHPGeDetHit** hpgeHit;
	hpgeHitCA = new TClonesArray("EnsarHPGeDetHit",5);
	TBranch *branchEnsarHPGeDetHit = tree ->GetBranch("HPGeDetHit");
	branchEnsarHPGeDetHit->SetAddress(&hpgeHitCA );

	//HPGe Points
	/*TClonesArray* hpgePointCA;
	EnsarHPGeDetPoint** hpgePoint;
	hpgePointCA = new TClonesArray("EnsarHPGeDetPoint",5);
	TBranch *branchEnsarHPGeDetPoint = tree ->GetBranch("HPGeDetPoint");
	branchEnsarHPGeDetPoint->SetAddress(&hpgePointCA );*/
	
	
	//----- CALIFA detector--------------//
	//Crystal Hits  
	TClonesArray* crystalHitCA;  
	R3BCaloCrystalHitSim** crystalHit;
	crystalHitCA = new TClonesArray("R3BCaloCrystalHitSim",5);
	TBranch *branchCrystalHit = tree->GetBranch("CrystalHitSim");
	branchCrystalHit->SetAddress(&crystalHitCA);

	//Calo Hits
	TClonesArray* caloHitCA;  
	R3BCaloHitSim** caloHit;
	caloHitCA = new TClonesArray("R3BCaloHitSim",5);
	TBranch *branchCaloHit = tree->GetBranch("CaloHitSim");
	branchCaloHit->SetAddress(&caloHitCA);
	
	
	//Calo Points
	/*TClonesArray* crystalPointCA;  
	R3BCaloPoint** crystalPoint;
	crystalPointCA = new TClonesArray("R3BCaloPoint",5);
	TBranch *branchcrystalPoint = tree->GetBranch("CrystalPoint");
	branchcrystalPoint->SetAddress(&crystalPointCA);*/


	Int_t MCtracksPerEvent = 0;
	Int_t hpgeHitsPerEvent = 0;
	Int_t hpgePointsPerEvent = 0;
	Int_t crystalHitsPerEvent = 0;
	Int_t caloHitsPerEvent=0;
	Int_t crystalPointPerEvent=0;
    Double_t energyGe = 0.0;
    Double_t limit1 = 0.0, limit2=0.0;
    TVector3 momentum;
    
    //lower and upper limits Cascade Generator
    //limit1=1.779 - 0.01*1.779;//-1% of gamma energy=1.779
    //limit2=1.779 + 0.01*1.779;//+1%
    
    //lower and upper limits Co Cascade Generator
    limit1=1.172 - 0.01*1.172;//-1% of gamma energy=1.172
    limit2=1.172 + 0.01*1.172;//+1% 
    

	//TREE ENTRIES--------------------------------------------------------------------
	Long64_t nevents = tree->GetEntries();
   
        
	//LOOP IN THE EVENTS--------------------------------------------------------------
	for(int i=0;i<nevents;i++){
	    	if(i%1== 100) printf("Event:%i\n",i);
		energyGe = 0.;
		tree->GetEvent(i);

		//MCtracksPerEvent     = MCTrackCA->GetEntries();
		hpgeHitsPerEvent     = hpgeHitCA->GetEntries();
		//hpgePointsPerEvent   = hpgePointCA->GetEntries();
		crystalHitsPerEvent  = crystalHitCA->GetEntries();
		caloHitsPerEvent     = caloHitCA->GetEntries();
		//crystalPointPerEvent = crystalPointCA->GetEntries();

		/*if(MCtracksPerEvent>0) {
			track = new EnsarMCTrack*[MCtracksPerEvent];
			for(Int_t j=0;j<MCtracksPerEvent;j++){
				track[j] = new EnsarMCTrack;
				track[j] = (EnsarMCTrack*) MCTrackCA->At(j);
			}
		}*/
		if(hpgeHitsPerEvent>0) {
			hpgeHit = new EnsarHPGeDetHit*[hpgeHitsPerEvent];
			for(Int_t j=0;j<hpgeHitsPerEvent;j++){
				hpgeHit[j] = new EnsarHPGeDetHit;
				hpgeHit[j] = (EnsarHPGeDetHit*) hpgeHitCA->At(j);
			}
		}
		/*if(hpgePointsPerEvent>0) {
			hpgePoint = new EnsarHPGeDetPoint*[hpgePointsPerEvent];
			for(Int_t j=0;j<hpgePointsPerEvent;j++){
				hpgePoint[j] = new EnsarHPGeDetPoint;
				hpgePoint[j] = (EnsarHPGeDetPoint*) hpgePointCA->At(j);
			}
		}*/
		if(crystalHitsPerEvent>0) {
			crystalHit = new R3BCaloCrystalHitSim*[crystalHitsPerEvent];
			for(Int_t j=0;j<crystalHitsPerEvent;j++){
				crystalHit[j] = new R3BCaloCrystalHitSim;
				crystalHit[j] = (R3BCaloCrystalHitSim*) crystalHitCA->At(j);      
			}
		}
		if(caloHitsPerEvent>0) {
			caloHit = new R3BCaloHitSim*[caloHitsPerEvent];
			for(Int_t j=0;j<caloHitsPerEvent;j++){
				caloHit[j] = new R3BCaloHitSim;
				caloHit[j] = (R3BCaloHitSim*) caloHitCA->At(j);      
			}
		}
		/*if(crystalPointPerEvent>0) {
			crystalPoint = new R3BCaloPoint*[crystalPointPerEvent];
			for(Int_t j=0;j<crystalPointPerEvent;j++){
				crystalPoint[j] = new R3BCaloPoint;
				crystalPoint[j] = (R3BCaloPoint*) crystalPointCA->At(j);      
			}
		}*/
		
		//LOOP in Calo Hits-------
		for(Int_t i=0;i<caloHitsPerEvent;i++){
		
		 //LOOP in Crystal Hits----------------    
		 for(Int_t j=0;j<crystalHitsPerEvent;j++){
					//cout<<"CrystalId= "<<crystalHit[i]->GetCrystalId()<<endl;
					//cout<<"CrystalType= "<<crystalHit[i]->GetCrystalType()<<endl;
					//cout<<"CrystalCopy= "<<crystalHit[i]->GetCrystalCopy()<<endl;
			Double_t CryID=0.;
			CryID=crystalHit[j]->GetCrystalId();
					 	
			if (CryID>64 && CryID<73){
					 h_Cry_1->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
		    if (CryID>72 && CryID<81){
					 h_Cry_2->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
		    if (CryID>80 && CryID<89){
					 h_Cry_3->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
		    if (CryID>88 && CryID<97){
					 h_Cry_4->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
		    if (CryID>96 && CryID<105){
					 h_Cry_5->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
		    if (CryID>104 && CryID<113){
					 h_Cry_6->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
		    if (CryID>112 && CryID<121){
					 h_Cry_7->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
		    if (CryID>120 && CryID<129){
					h_Cry_8->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
		    
		    if (CryID==1|| CryID==2|| CryID==3|| CryID==4|| CryID==5|| CryID==6|| CryID==7|| CryID==8){
					h_Cry_10->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
		    if (CryID==9|| CryID==10|| CryID==11|| CryID==12|| CryID==13|| CryID==14|| CryID==15|| CryID==16){
					h_Cry_11->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
		    if (CryID==17|| CryID==18|| CryID==19|| CryID==20|| CryID==21|| CryID==22|| CryID==23|| CryID==24){
					h_Cry_12->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
		    if (CryID==25|| CryID==26|| CryID==27|| CryID==28|| CryID==29|| CryID==30|| CryID==31|| CryID==32){
					h_Cry_13->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		    }
					
			h_Cry_9->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());		
					
		}
	}
        
  			
       
		

		//if(MCtracksPerEvent)     delete[] track;
		if(hpgeHitsPerEvent)     delete[] hpgeHit;
		//if(hpgePointsPerEvent)   delete[] hpgePoint;
		if(crystalHitsPerEvent)  delete[] crystalHit;
		if(caloHitsPerEvent)     delete[] caloHit;
		//if(crystalPointPerEvent) delete[] crystalPoint;

	}
	// END LOOP IN THE EVENTS---------------------------------------------------------
	
	

	// HISTOGRAMS--------------------------------------------------------------------- 
	
	//CrystalHit
	TCanvas* c5 = new TCanvas("Crystal Identification","Crystal Identification",0,0,400,800);          
	c5->Divide(2,2);
	c5->SetFillColor(0);       
	c5->SetFrameFillColor(0);  

	c5->cd(1);
	h_Cry_1->Draw("colz"); 
	h_Cry_1->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_1->GetYaxis()->SetTitle("Phi (rad)");
	
	c5->cd(2);
	h_Cry_2->Draw("colz"); 
	h_Cry_2->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_2->GetYaxis()->SetTitle("Phi (rad)");
	
	c5->cd(3);
	h_Cry_3->Draw("colz"); 
	h_Cry_3->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_3->GetYaxis()->SetTitle("Phi (rad)");
	
	c5->cd(4);
	h_Cry_4->Draw("colz"); 
	h_Cry_4->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_4->GetYaxis()->SetTitle("Phi (rad)");
	        
	//-----              
	TCanvas* c2 = new TCanvas("Crystal Identification 2","Crystal Identification 2",0,0,400,800);          
	c2->SetFillColor(0);       
	c2->SetFrameFillColor(0);  
	c2->Divide(2,2);
	
	c2->cd(1);
	h_Cry_5->Draw("colz"); 
	h_Cry_5->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_5->GetYaxis()->SetTitle("Phi (rad)");
	
	c2->cd(2);
	h_Cry_6->Draw("colz"); 
	h_Cry_6->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_6->GetYaxis()->SetTitle("Phi (rad)");
	
	c2->cd(3);
	h_Cry_7->Draw("colz"); 
	h_Cry_7->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_7->GetYaxis()->SetTitle("Phi (rad)");
	
	c2->cd(4);
	h_Cry_8->Draw("colz"); 
	h_Cry_8->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_8->GetYaxis()->SetTitle("Phi (rad)");
	
	//-----
	TCanvas* c3 = new TCanvas("Crystal Identification both petals","Crystal Identification both petals",0,0,400,800);          
	c3->SetFillColor(0);       
	c3->SetFrameFillColor(0);  
	h_Cry_9->Draw("colz"); 
	h_Cry_9->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_9->GetYaxis()->SetTitle("Phi (rad)");
	
	//-----              
	TCanvas* c4 = new TCanvas("Crystal Identification detail","Crystal Identification detail",0,0,400,800);          
	c4->SetFillColor(0);       
	c4->SetFrameFillColor(0);  
	c4->Divide(2,2);
	
	c4->cd(1);
	h_Cry_10->Draw("colz"); 
	h_Cry_10->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_10->GetYaxis()->SetTitle("Phi (rad)");
	
	c4->cd(2);
	h_Cry_11->Draw("colz"); 
	h_Cry_11->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_11->GetYaxis()->SetTitle("Phi (rad)");
	
	c4->cd(3);
	h_Cry_12->Draw("colz"); 
	h_Cry_12->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_12->GetYaxis()->SetTitle("Phi (rad)");
	
	c4->cd(4);
	h_Cry_13->Draw("colz"); 
	h_Cry_13->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_13->GetYaxis()->SetTitle("Phi (rad)");

}

