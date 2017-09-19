{
 
 	gROOT->SetStyle("Plain");
	gStyle->SetOptFit(111);
	gStyle->SetPalette(1);

	//OPEN FILES
	
	//FILE 1 CrystalId and Theta angle
	ifstream *File1 = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/Crystal_Resolution/CrystalId_Theta.dat");

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
    
    //FILE 2 Counts of an isotropic distribution
	ifstream *File2 = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/Crystal_Resolution/iso_200keV/ResolutionCrystals_iso.dat");

	Double_t CryId2[nLevel];
	Double_t Gammas_iso[nLevel];
    
   
    File2->ignore(256,'\n');
	File2->ignore(256,'\n');
	File2->ignore(256,'\n');
	File2->ignore(256,'\n');
	
	for(Int_t i = 0; i < nLevel; i++){ 
	 	*File2 >> CryId2[i] >> Gammas_iso[i];
    }
    
    //FILE 3 Counts of an Angular Correlated distribution
	ifstream *File3 = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/Crystal_Resolution/W_200keV_2/ResolutionCry_W2.dat");

	Double_t CryId3[nLevel];
	Double_t Gammas_W[nLevel];
    
   
    File3->ignore(256,'\n');
	File3->ignore(256,'\n');
	File3->ignore(256,'\n');
	File3->ignore(256,'\n');
	
	for(Int_t i = 0; i < nLevel; i++){ 
	 	*File3 >> CryId3[i] >> Gammas_W[i];
    }
   
   
   //CALCUL
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
    
   //FUNCTION ANGULAR CORRELATIONS 
   TF1 *fa1 = new TF1("fa1","(1+[0]*pow((TMath::Cos(x)),2)+[0]*pow((TMath::Cos(x)),4))/[1]",0,2.15);
   fa1->SetParameter(0, 0.95);
   fa1->SetParameter(1, 1.5);
   
   TF1 *fa2 = new TF1("fa2","(7/8)*[0]*x +([0]*TMath::Sin(4*x))/32+[0]*TMath::Sin(x)*TMath::Cos(x)+x",0,2.15);
   fa2->SetParameter(0, 0.95);
   //fa2->Draw("");
  
    
   //PASS grad->rad 
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
