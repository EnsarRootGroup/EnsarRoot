void checkResults_ResolutionCry_angulos() {

	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE  
	char inputFile[250] = "outsim.root";                                            
	//char inputFile[250] = "/mnt/scratch/eli/outsim_gamma_200kev.root"; 
	//char inputFile[250] = "/mnt/scratch/eli/outsim_g_200keV_W.root";	
	TFile *file1 = TFile::Open(inputFile);

	//READING TREE
	TTree* tree = (TTree*)file1->Get("ensartree");

	//HISTOGRAMS DEFINITION-----------------------------------------------------------
	TH1F* h_Cry_1 = new TH1F("h_Cry_1","Primary Gammas Hits of each Crystal",130.,0.,130.);	
	
	//TH2F* h_Calo_2 = new TH2F("h_Calo_2","Theta of each Crystal", 128.,0.,128.,360,0.,3.6);
	//TH2F* h_Calo_3 = new TH2F("h_Calo_3","Phi of each Crystal", 128.,0.,128.,720,-3.6,3.6);
	
	TH2F* h_MC_2 = new TH2F("h_MC_2","Theta_MC of each Crystal", 128.,0.,128.,360,0.,3.6);
	TH2F* h_MC_3 = new TH2F("h_MC_3","Phi_MC of each Crystal", 128.,0.,128.,720,-3.6,3.6);
	
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
	
	
	//Calo Points
	/*TClonesArray* crystalPointCA;  
	R3BCaloPoint** crystalPoint;
	crystalPointCA = new TClonesArray("R3BCaloPoint",5);
	TBranch *branchcrystalPoint = tree->GetBranch("CrystalPoint");
	branchcrystalPoint->SetAddress(&crystalPointCA);*/


	Int_t MCtracksPerEvent = 0;
	Int_t crystalHitsPerEvent = 0;
	//Int_t caloHitsPerEvent=0;
	//Int_t crystalPointPerEvent=0;
    
   
    

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
		

		Double_t CryID=0.;
		
		 //LOOP in Crystal Hits----------------    
		 for(Int_t j=0;j<crystalHitsPerEvent;j++){
					//cout<<"CrystalId= "<<crystalHit[i]->GetCrystalId()<<endl;
					//cout<<"CrystalType= "<<crystalHit[i]->GetCrystalType()<<endl;
					//cout<<"CrystalCopy= "<<crystalHit[i]->GetCrystalCopy()<<endl;
			
			CryID=crystalHit[j]->GetCrystalId();		 	
			h_Cry_1->Fill(CryID);		
					
		}
		
		//LOOP in calorimeter Hits----------------
		/*for(Int_t h=0;h<caloHitsPerEvent;h++){
			h_Calo_2->Fill(CryID,caloHit[h]->GetTheta()); //rad
			h_Calo_3->Fill(CryID,caloHit[h]->GetPhi());	  //rad
		} */
		
			
		
		//LOOP in MC mother tracks----------------
		for(Int_t i=0;i<MCtracksPerEvent;i++){
			if(track[i]->GetMotherId()<0) { //Primary Particle is MotherId=-1
				
				TVector3 momentum;
				Double_t Theta=0.;
				Double_t Phi=0.;
		
				track[i]->GetMomentum(momentum);
 				Theta = momentum.Theta();
               	Phi   = momentum.Phi();
                                
                if (crystalHitsPerEvent){
                	h_MC_2->Fill(CryID,Theta); //rad
					h_MC_3->Fill(CryID,Phi);   //rad
                } 
			}
		}
		

		if(MCtracksPerEvent)     delete[] track;
		if(crystalHitsPerEvent)  delete[] crystalHit;
		//if(caloHitsPerEvent)     delete[] caloHit;
		//if(crystalPointPerEvent) delete[] crystalPoint;

	}
	// END LOOP IN THE EVENTS---------------------------------------------------------
	
	

	// HISTOGRAMS--------------------------------------------------------------------- 
	
	//CrystalHit
	TCanvas* c1 = new TCanvas("Primary Gammas Hits in each Crystal","Primary Gammas Hits in each Crystal",0,0,400,800);          
	c1->SetFillColor(0);       
	c1->SetFrameFillColor(0);  
	h_Cry_1->Draw();
	h_Cry_1->SetLineColor(9); 
	h_Cry_1->GetXaxis()->SetTitle("Crystal_ID");
	h_Cry_1->GetYaxis()->SetTitle("Number Primary Gammas Hits");
	
	//CaloHit
	/*TCanvas* c2 = new TCanvas("Angles of each Crystal","Angles of each Crystal",0,0,400,800);
	c2->Divide(1,2);           
	c2->SetFillColor(0);       
	c2->SetFrameFillColor(0);
	
	c2->cd(1);                 
	h_Calo_2->Draw("colz");
	//h_Calo_2->SetLineColor(4); 
	h_Calo_2->GetXaxis()->SetTitle("Crystal_ID");
	h_Calo_2->GetYaxis()->SetTitle("Theta (rad)"); 
	
	c2->cd(2);                 
	h_Calo_3->Draw("colz");
	//h_Calo_3->SetLineColor(6); 
	h_Calo_3->GetXaxis()->SetTitle("Crystal_ID");
	h_Calo_3->GetYaxis()->SetTitle("Phi (rad)"); */
	
	//MCTracks
	TCanvas* c3 = new TCanvas("Angles_MC of each Crystal","Angles_MC of each Crystal",0,0,400,800);
	c3->Divide(1,2);           
	c3->SetFillColor(0);       
	c3->SetFrameFillColor(0);
	
	c3->cd(1);                 
	h_MC_2->Draw("colz"); 
	h_MC_2->GetXaxis()->SetTitle("Crystal_ID");
	h_MC_2->GetYaxis()->SetTitle("Theta_MC (rad)"); 
	
	c3->cd(2);
	h_MC_3->Draw("colz"); 
	h_MC_3->GetXaxis()->SetTitle("Crystal_ID");
	h_MC_3->GetYaxis()->SetTitle("Phi_MC (rad)");

}

