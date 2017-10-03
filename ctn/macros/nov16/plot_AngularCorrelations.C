{
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////																										
////							--- Simulation of the Lisbon Nov-2016 setup ---					
////			
////			The plot of differents de-excitations (gamma-cascades) with differents spin-parities (J^P)
////			
////			Transitions(J3->J2->J1 ground state):
////													1->2->0   
////													2->2->0
////													2->3->0
////													2->4->0
////													2->0->0
////													2->1->0
////			The A2&A4 angular correlated coeficients are theoretical: 	A2=SetParameter(0)	A4=SetParameter(1)
////
//// **elisabet.galiana@usc.es													
//// ** Universidad de Santiago de Compostela									
//// ** Dpto. Física de Partículas 												
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



 	gROOT->SetStyle("Plain");
	gStyle->SetOptFit(111);
	gStyle->SetPalette(1);

	
	
   //FUNCTION
   //The Theoretical Angular Correlations function is		W=1+A2cos^2(theta)+A4cos^4(theta)
   // 														where the A2, A4 Angular Correlation coefficients depends on 
   // 														the spin-parity of the transition in the gamma cascade
   
   //Theoretical angular correlations function 1->2->0
   TF1 *fa1 = new TF1("fa1","(1+[0]*pow((TMath::Cos(x)),2)+[1]*pow((TMath::Cos(x)),4))",0.,3.1416);
   fa1->SetParameter(0, -0.33); //	this parameter represents the A2,A4 Angular Correlations coefficients
   fa1->SetParameter(1, 0.);
   fa1->Draw("");
   
   //Theoretical angular correlations function 2->2->0
   TF1 *fa2 = new TF1("fa2","(1+[0]*pow((TMath::Cos(x)),2)+[1]*pow((TMath::Cos(x)),4))",0.,3.1416);
   fa2->SetParameter(0, 0.429); //	this parameter represents the A2,A4 Angular Correlations coefficients
   fa2->SetParameter(1, 0.);
   
   //Theoretical angular correlations function 2->3->0
   TF1 *fa3 = new TF1("fa3","(1+[0]*pow((TMath::Cos(x)),2)+[1]*pow((TMath::Cos(x)),4))",0.,3.1416);
   fa3->SetParameter(0, 0.); //	this parameter represents the A2,A4 Angular Correlations coefficients
   fa3->SetParameter(1, -0.33);
     
   //Theoretical angular correlations function 2->4->0
   TF1 *fa4 = new TF1("fa4","(1+[0]*pow((TMath::Cos(x)),2)+[1]*pow((TMath::Cos(x)),4))",0.,3.1416);
   fa4->SetParameter(0, 0.125); //	this parameter represents the A2,A4 Angular Correlations coefficients
   fa4->SetParameter(1, 0.0417);
   
   //Theoretical angular correlations function 2->0->0
   TF1 *fa5 = new TF1("fa5","(1+[0]*pow((TMath::Cos(x)),2)+[1]*pow((TMath::Cos(x)),4))",0.,3.1416);
   fa5->SetParameter(0, -3.); //	this parameter represents the A2,A4 Angular Correlations coefficients
   fa5->SetParameter(1, 4.);
   
   //Theoretical angular correlations function 2->1->0
   TF1 *fa6 = new TF1("fa6","(1+[0]*pow((TMath::Cos(x)),2)+[1]*pow((TMath::Cos(x)),4))",0.,3.1416);
   fa6->SetParameter(0, 5.); //	this parameter represents the A2,A4 Angular Correlations coefficients
   fa6->SetParameter(1, -5.33);
   
   
    
   TCanvas *c1 = new TCanvas("c1","A Simple",200,10,700,500);
   c1->SetFillColor(0);       
   c1->SetFrameFillColor(0); 
   
   fa6->SetLineColor(3);
   fa6->Draw("");
   //fa6->GetXaxis()->->SetRange(22,23);
   fa6->GetYaxis()->SetRangeUser(0.4,2.2);
   fa6->GetXaxis()->SetTitle("#theta");
   fa6->GetYaxis()->SetTitle("W(#theta)");
   fa6->SetTitle("Angular Correlations functions");
   
   fa2->SetLineColor(5);
   fa2->Draw("same");
   
   fa1->SetLineColor(4);
   fa1->Draw("same");
   
   fa3->SetLineColor(7);
   fa3->Draw("same");
   
   fa4->SetLineColor(1);
   fa4->Draw("same");
    
   fa5->SetLineColor(2);
   fa5->Draw("same");
   
   
   
	TLegend *leg = new TLegend(0.5,0.7,0.8,0.8);
	leg->AddEntry(fa1, "1->2->0", "l");
	leg->SetFillColor(0);
	leg->Draw();
	leg->AddEntry(fa2, "2->2->0", "l");
	leg->AddEntry(fa3, "2->3->0", "l");
	leg->AddEntry(fa4, "2->4->0", "l");
	leg->AddEntry(fa5, "2->0->0", "l");
	leg->AddEntry(fa6, "2->1->0", "l");
	
    
   //GRAPH
   /*TGraph *gr = new TGraph(nLevel,Theta_rad,Gammas);
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
   TCanvas *c1 = new TCanvas("c1","A Simple",200,10,700,500);
   c1->SetFillColor(0);       
   c1->SetFrameFillColor(0); 
   c1->Divide(1,2); 

   c1->cd(1); 
   fa2->Draw();
   fa2->GetXaxis()->SetTitle("Theta (rad)");
   fa2->GetYaxis()->SetTitle("Integral of W(theta)");
   
   c1->cd(2); 
   gr->Draw("A*");
   fa1->Draw("same");
   
   TCanvas *c1 = new TCanvas("c1","A Simple",200,10,700,500);
   c1->SetFillColor(0);       
   c1->SetFrameFillColor(0); 
   gr->Draw("A*");
   fa1->Draw("same");*/
}
