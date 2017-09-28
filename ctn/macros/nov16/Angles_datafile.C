{

////////////////////////////////////////////////////////////////////////////
////																	////
////		--- Simulation of the Lisbon Nov-2016 setup ---				////
////																	////
////		Macro to calculate the Theta&Phi angles of each Crystal		////
////																	////
////		It use the CrystalId_Theta_Phi.dat data file which 			////
////		the origin of this data is MCTrack.							////
////																	////
////		Usage:														////
////			1st: select the root file and the root file path		////
////			2nd: root -l Angles_datafile.C							////
////																	////
//// **elisabet.galiana@usc.es											////
//// ** Universidad de Santiago de Compostela							////
//// ** Dpto. Física de Partículas 										////
////////////////////////////////////////////////////////////////////////////


 	gROOT->SetStyle("Plain");
	gStyle->SetOptFit(111);
	gStyle->SetPalette(1);

	
	//FILE 1 CrystalId & Theta angle  &  Phi angle
	//ifstream *File1 = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/Crystal_Resolution/CrystalId_Theta_Phi.dat");
	ifstream *File1 = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/ENSARRoot/EnsarRoot_source/ctn/macros/nov16/CrystalId_Theta_Phi.dat");

	const Int_t nLevel=128;
	Double_t CryId[nLevel];
	Double_t Theta[nLevel];
	Double_t Phi[nLevel];
    
   
    File1->ignore(256,'\n');
	File1->ignore(256,'\n');
	File1->ignore(256,'\n');
	File1->ignore(256,'\n');
	
	for(Int_t i = 0; i < nLevel; i++){ 
	 	*File1 >> CryId[i] >> Theta[i] >> Phi[i];
	 	cout<<"CryId= "<< CryId[i]<<"  Theta= " << Theta[i]<<"Phi= "<<Phi[i]<<endl;
    }
    
   //GRAPH
   TGraph *gr = new TGraph(nLevel,Theta,Phi);
   gr->SetLineColor(2);
   gr->SetLineWidth(4);
   gr->SetMarkerColor(4);
   //gr->SetMarkerStyle(21);
   gr->SetTitle("Theta vs Phi for each CryId");
   gr->GetXaxis()->SetTitle("Theta (#circ)");
   gr->GetYaxis()->SetTitle("Phi (#circ)");
   //gr->Draw("AP");
   gr->Draw("A*");

}
