/*#include "TObject.h"

    class CryInfo:public TObject{
    	public:
    	CryInfo();
    	~CryInfo();
    	
    	Int_t Cry_ID;
    	Double_t Cry_Theta;
    	Double_t Cry_Phi;	
        
        ClassDef(CryInfo,1);
    };

	CryInfo::CryInfo():TObject(){Cry_ID=0;};
	CryInfo::~CryInfo(){};*/
	
void checkResults_Resolution_struct() {


	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE
	//char inputFile[250] = "/mnt/scratch/eli/outsim_iso.root"; 
	//char inputFile[250] = "/mnt/scratch/eli/outsim_W.root"; 
	char inputFile[250] = "outsim.root";                                         
	//char outFile[250] = "outInfo.root";                                         	
	TFile *file1 = TFile::Open(inputFile);
	//READING TREE
	TTree* tree = (TTree*)file1->Get("ensartree");
	//OUT TREE
	TFile* fileOut = new TFile("outinfo.root","RECREATE");
	TTree* treeOut = new TTree("INFO","INFO");   	
	
	Int_t Cry_ID;
    Double_t Cry_Theta;
    Double_t Cry_Phi;	
	
	TBranch *branch_Cry_ID = treeOut->Branch("branch_Cry_ID",&Cry_ID,"Cry_ID");
	TBranch *branch_Cry_Theta = treeOut->Branch("branch_Cry_Theta",&Cry_Theta,"Cry_Theta");
	TBranch *branch_Cry_Phi = treeOut->Branch("branch_Cry_Phi",&Cry_Phi,"Cry_Phi");
	/*CryInfo* Crystals = new CryInfo();
	//treeOut->Branch("CryInfo", &Crystals);	
	TBranch *branchOut = treeOut->Branch("bname", &Crystals);*/

	//TClonesArray* CryInfoCA;
	//CryInfoCA = new TClonesArray("CryInfo",2); 
	//treeOut->Branch("INFO",&CryInfoCA;
	//------
	//TClonesArray* padSignalCA;
 	//padSignalCA = new TClonesArray("ActarPadSignal",50);
	//digiTree->Branch("padSignals",&padSignalCA);


	//HISTOGRAMS DEFINITION-----------------------------------------------------------//Change these maximum energies
    TH2F* h_Cry_1 = new TH2F("h_Cry_1","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);
    
	
	

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
    
    /*CryInfo :: TObject{
    	Int_t Cry_ID;
    	Double_t Cry_Theta;
    	Double_t Cry_Phi;
    };*/
    
   /* struct CryInfo{
    	Int_t Cry_ID;
    	Double_t Cry_Theta;
    	Double_t Cry_Phi;
    };
    */

   

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

		//CryInfoCA->Clear();
		
		//LOOP in MC mother tracks----------------------------------------------
		for(Int_t h=0;h<MCtracksPerEvent;h++){
			if(track[h]->GetMotherId()<0) { //Primary Particle is MotherId=-1
				
				track[h]->GetMomentum(momentum);
 				Theta = momentum.Theta();
               	Phi   = momentum.Phi();
                
                
                if (crystalHitsPerEvent==1){
                
                	CryID=crystalHit[0]->GetCrystalId(); //1 crystal signal
                
                	//Crystals->Cry_ID=CryID;
					//Crystals->Cry_Theta=Theta;
					//Crystals->Cry_Phi=Phi;
					
					Cry_ID=CryID;
					Cry_Theta=Theta;
					Cry_Phi=Phi;
					
					
                }
		       
              
		}
		
		branch_Cry_ID->Fill();
					branch_Cry_Theta->Fill();
					branch_Cry_Phi->Fill();
	}
	//treeOut->Fill();
	//cout << "event"<<endl;	
	
	/*branch_Cry_ID->Fill();
	branch_Cry_Theta->Fill();
	branch_Cry_Phi->Fill();*/
	
	if(MCtracksPerEvent)     delete[] track;
	if(crystalHitsPerEvent)  delete[] crystalHit;
}

	fileOut->Write();
	fileOut->Close();
 
// END LOOP IN THE EVENTS---------------------------------------------------------


	// HISTOGRAMS--------------------------------------------------------------------- 
	
	
}
