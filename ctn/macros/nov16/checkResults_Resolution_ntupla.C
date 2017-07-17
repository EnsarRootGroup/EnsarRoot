

void checkResults_Resolution_ntupla() {


	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE
	//char inputFile[250] = "/mnt/scratch/eli/outsim_iso.root";
	//char inputFile[250] = "/mnt/scratch/eli/outsim_W.root";
	char inputFile[250] = "outsim.root";                                         
	TFile *file1 = TFile::Open(inputFile);

	//READING TREE
	TTree* tree = (TTree*)file1->Get("ensartree");

	//HISTOGRAMS DEFINITION-----------------------------------------------------------//Change these maximum energies
    //TH2F* h_Cry_1 = new TH2F("h_Cry_1","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);
    
	
	

	//----   MCTrack (input)   -------------------------------------------------------
	TClonesArray* MCTrackCA;
	EnsarMCTrack** track;
	MCTrackCA = new TClonesArray("EnsarMCTrack",5);
	TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
	branchMCTrack->SetAddress(&MCTrackCA);
	
	//----- CALIFA detector--------------//
	//Crystal Points  
	TClonesArray* crystalHitCA;  
	R3BCaloCrystalHitSim** crystalHit;
	crystalHitCA = new TClonesArray("R3BCaloCrystalHitSim",5);
	TBranch *branchCrystalHit = tree->GetBranch("CrystalHitSim");
	branchCrystalHit->SetAddress(&crystalHitCA);


	//Variables definition ---------
	Int_t MCtracksPerEvent = 0;
	Int_t crystalHitsPerEvent = 0;

    TVector3 momentum;
    Double_t Theta=0., Phi=0.;
    Int_t CryID;
    
    Int_t Cry_ID;
    Double_t Cry_Theta;
    Double_t Cry_Phi;
   
    TNtuple *cryinfo = new TNtuple("cryinfo","Crystals information","Cry_ID:Cry_Theta:Cry_Phi");


	//TREE ENTRIES--------------------------------------------------------------------
	Long64_t nevents = tree->GetEntries();
        
	//LOOP IN THE EVENTS--------------------------------------------------------------
	for(int i=0;i<nevents;i++){
	    	if(i%1== 100) printf("Event:%i\n",i);

		tree->GetEvent(i);

		MCtracksPerEvent     = MCTrackCA->GetEntries();
		crystalHitsPerEvent  = crystalHitCA->GetEntries();

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

		
		//LOOP in MC mother tracks----------------------------------------------
		for(Int_t h=0;h<MCtracksPerEvent;h++){
			if(track[h]->GetMotherId()<0) { //Primary Particle is MotherId=-1
				
				track[h]->GetMomentum(momentum);
 				Theta = momentum.Theta();
               	Phi   = momentum.Phi();
                
                
                if (crystalHitsPerEvent){
                
                	CryID=crystalHit[0]->GetCrystalId(); //1 crystal signal
                
                	cryinfo->Fill(CryID, Theta, Phi);
                } 
			}

		if(MCtracksPerEvent)     delete[] track;
		if(crystalHitsPerEvent)  delete[] crystalHit;
		}
	}
	// END LOOP IN THE EVENTS---------------------------------------------------------

	//Save NTupla in a file
	TFile* Crystals_info=new TFile("Crystals_info.root","recreate");
	cryinfo->Write();
	Crystals_info->Close();

	// HISTOGRAMS--------------------------------------------------------------------- 
	
	/*TCanvas* c1 = new TCanvas("Theta vs Phi","Theta vs Phi",0,0,400,800);          
	c1->SetFillColor(0);       
	c1->SetFrameFillColor(0);           
	h_Cry_1->Draw("colz"); 
	h_Cry_1->GetXaxis()->SetTitle("Theta (rad)");
	h_Cry_1->GetYaxis()->SetTitle("Phi (rad)");*/

}
