{
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////																										////
////							--- Simulation of the Lisbon Nov-2016 setup ---								////
////																										////
////		This Macro analyze the Crystal Response of the Petals.							 				////
////																										////
////		It study the ratio of an isotropic and a correlated distribution in simulation					////
////		and the polar angle (theta) for each Crystal.													////
////		The Crystals could be identified by their CrystalId:											////
////				-Petal at 90º w.r.t.HPGe: CryId from 1 to 64											////
////				-Petal at 180º w.r.t.HPGe: CryId from 65 to 128											////
////		Then, it calculates the Normalized theoretical angular correlations function					////
////		to check the agreement between them.															////
////																										////
////		Usage:																							////
////			1st: (OPTIONAL) change the data files & data file paths,									////
////				  A2&A4 AngularCorrelations coefficients, Normalization of the function					////
////			2nd: root -l Crystal_Response.C																////
////																										////
////		This Macro reads:																				////
////		 -the CryId and its Theta angle from the CrystalId_Theta.dat data file							////
////		 -Num.gammas for each CryId of an isotropic distribution from the NumGammas_Cry_isotropic.dat	////
////		 -Num.gammas for each CryId of an AngularCorr distribution from the NumGammas_Cry_AngularC.dat	////
////																										////
////		Calculate:																						////
////		 -Ratio gammas R= Num.gammas isotropic / Num.gammas with AngularCorr, for each CrystalId		////
////		 -Normalized theoretical angular correlations function											////
////																										////
////		Graph representation: Theta angle vs Ratio gammas for each CrystalId							////
////							  Normalized theoretical angular correlations function						////
////																										////
//// **elisabet.galiana@usc.es																				////
//// ** Universidad de Santiago de Compostela																////
//// ** Dpto. Física de Partículas 																			////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 	gROOT->SetStyle("Plain");
	gStyle->SetOptFit(111);
	gStyle->SetPalette(1);

	//OPEN FILES
	
	//FILE 1 CrystalId and Theta angle
	//ifstream *File1 = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/Crystal_Resolution/CrystalId_Theta.dat");
	ifstream *File1 = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/ENSARRoot/EnsarRoot_source/ctn/macros/nov16/CrystalId_Theta.dat");

	const Int_t nLevel=128;
	Double_t CryId[nLevel];
	Double_t Theta[nLevel];
    
   
    File1->ignore(256,'\n');
	File1->ignore(256,'\n');
	File1->ignore(256,'\n');
	File1->ignore(256,'\n');
	
	for(Int_t i = 0; i < nLevel; i++){ 
	 	*File1 >> CryId[i] >> Theta[i];
	 	cout<<"CryId= "<< CryId[i]<<"  Theta= " << Theta[i]<<endl;
    }
    //fclose(File1);
    
    //FILE 2 Number of gammas have been arrived to each Crystal -> Isotropic distribution
	//ifstream *File2 = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/Crystal_Resolution/iso_200keV/NumGammas_Cry_isotropic.dat");
	ifstream *File2 = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/ENSARRoot/EnsarRoot_source/ctn/macros/nov16/NumGammas_Cry_isotropic.dat");

	Double_t CryId2[nLevel];
	Double_t Gammas_iso[nLevel];
    
   
    File2->ignore(256,'\n');
	File2->ignore(256,'\n');
	File2->ignore(256,'\n');
	File2->ignore(256,'\n');
	
	for(Int_t i = 0; i < nLevel; i++){ 
	 	*File2 >> CryId2[i] >> Gammas_iso[i];
    }
    
    //FILE 3 Number of gammas have been arrived to each Crystal -> Angular Correlated distribution
	//ifstream *File3 = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/Crystal_Resolution/W_200keV_2/NumGammas_Cry_AngularC.dat");
	ifstream *File3 = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/ENSARRoot/EnsarRoot_source/ctn/macros/nov16/NumGammas_Cry_AngularC.dat");

	Double_t CryId3[nLevel];
	Double_t Gammas_W[nLevel];
    
   
    File3->ignore(256,'\n');
	File3->ignore(256,'\n');
	File3->ignore(256,'\n');
	File3->ignore(256,'\n');
	
	for(Int_t i = 0; i < nLevel; i++){ 
	 	*File3 >> CryId3[i] >> Gammas_W[i];
    }
   
   
   //CALCULATION
   Double_t Gammas[nLevel];
   Double_t x, y, frac;
   x=0.;
   y=0.;
   frac=0.;
   
   for(Int_t i = 0; i < nLevel; i++){ 
   
   		x= Gammas_iso[i];
	 	y= Gammas_W[i];
	 	frac=y/x;
	 	Gammas[i]=frac;
	 	
	 	cout<<"y="<<y<<" x="<<x<<endl;
	 	cout<<"frac= "<<frac<<endl;
    }
    
   //FUNCTION
   //The Theoretical Angular Correlations function is		W=1+A2cos^2(theta)+A4cos^4(theta)
   // 														where the A2, A4 Angular Correlation coefficients depends on 
   // 														the spin-parity of the transition in the gamma cascade
   
   //Normalized theoretical angular correlations function
   TF1 *fa1 = new TF1("fa1","(1+[0]*pow((TMath::Cos(x)),2)+[0]*pow((TMath::Cos(x)),4))/[1]",0,2.15);
   fa1->SetParameter(0, 0.95); //change 	this parameter represents the A2,A4 Angular Correlations coefficients
   fa1->SetParameter(1, 1.5);  //change		normalization parameter
   
   //Integral of the theoretical angular correlations function (not used)
   TF1 *fa2 = new TF1("fa2","(7/8)*[0]*x +([0]*TMath::Sin(4*x))/32+[0]*TMath::Sin(x)*TMath::Cos(x)+x",0,2.15);
   fa2->SetParameter(0, 0.95);
   //fa2->Draw("");
  
    
   //CHANGE THETA UNITS grad->rad 
   Double_t Theta_rad[nLevel];
   for(Int_t i = 0; i < nLevel; i++){ 
	 	Double_t T_rad, T_grad;
	 	T_grad=0.;
	 	T_rad=0.;
	 	
	 	T_grad=Theta[i];
	 	T_rad =T_grad*TMath::Pi()/180.;
	 	Theta_rad[i]=T_rad;
    }
   
    
    
   //GRAPH
   TGraph *gr = new TGraph(nLevel,Theta_rad,Gammas);
   gr->SetLineColor(2);
   gr->SetLineWidth(4);
   gr->SetMarkerColor(4);
   //gr->SetMarkerStyle(21);
   gr->SetTitle("Ratio gammas vs Theta angle for each CryId");
   gr->GetXaxis()->SetTitle("Theta (rad)");
   gr->GetYaxis()->SetTitle("Counts Gamma_W/Gamma_iso");
   //gr->Draw("AP");
   //gr->Draw("A*");
   //fa1->Draw("same");
   
   //CANVAS
   /*TCanvas *c1 = new TCanvas("c1","A Simple",200,10,700,500);
   c1->SetFillColor(0);       
   c1->SetFrameFillColor(0); 
   c1->Divide(1,2); 

   c1->cd(1); 
   fa2->Draw();
   fa2->GetXaxis()->SetTitle("Theta (rad)");
   fa2->GetYaxis()->SetTitle("Integral of W(theta)");
   
   c1->cd(2); 
   gr->Draw("A*");
   fa1->Draw("same");*/
   
   TCanvas *c1 = new TCanvas("c1","A Simple",200,10,700,500);
   c1->SetFillColor(0);       
   c1->SetFrameFillColor(0); 
   gr->Draw("A*");
   fa1->Draw("same");
}
