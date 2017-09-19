
// Function which perfoms the three gauss fit
// Returns the final sigma
Float_t fitGauss(TH1F *histo)
{

	Float_t mean = histo->GetMean();
	Float_t rms = histo->GetRMS();
	g1 = new TF1("m1","gaus",mean-rms,mean+rms);
	histo->Fit(g1,"RQN");
	mean = g1->GetParameter(1);
	Float_t sigma = g1->GetParameter(2);
	g2 = new TF1("m2","gaus",mean-(1.5*sigma),mean+(1.5*sigma));
	histo->Fit(g2,"RQN");
	mean = g2->GetParameter(1);
	sigma = g2->GetParameter(2);
	g3 = new TF1("m3","gaus",mean-(1.5*sigma),mean+(1.5*sigma));
	histo->Fit(g3,"R");
	sigma = g3->GetParameter(2);
	return sigma;

}

// Function which perfoms the three gauss fit
// Returns the final mean
Float_t fitGauss_Mean(TH1F *histo)
{

	Float_t mean = histo->GetMean();
	Float_t rms = histo->GetRMS();
	g1 = new TF1("m1","gaus",mean-rms,mean+rms);
	histo->Fit(g1,"RQN");
	mean = g1->GetParameter(1);
	Float_t sigma = g1->GetParameter(2);
	g2 = new TF1("m2","gaus",mean-(1.5*sigma),mean+(1.5*sigma));
	histo->Fit(g2,"RQN");
	mean = g2->GetParameter(1);
	sigma = g2->GetParameter(2);
	g3 = new TF1("m3","gaus",mean-(1.5*sigma),mean+(1.5*sigma));
	histo->Fit(g3,"RQ");
	mean = g3->GetParameter(1);
	return mean;

}

// Function which perfoms the three gauss fit
void fitGauss2(TH1F *histo,Float_t *par)
{

	Float_t mean = histo->GetMean();
	Float_t rms = histo->GetRMS();
	g1 = new TF1("m1","gaus",mean-rms,mean+rms);
	histo->Fit(g1,"RQN");
	mean = g1->GetParameter(1);
	Float_t sigma = g1->GetParameter(2);
	g2 = new TF1("m2","gaus",mean-(1.5*sigma),mean+(1.5*sigma));
	histo->Fit(g2,"RQN");
	mean = g2->GetParameter(1);
	sigma = g2->GetParameter(2);
	g3 = new TF1("m3","gaus",mean-(1.5*sigma),mean+(1.5*sigma));
	histo->Fit(g3,"RQN");
	par[0] = g3->GetParameter(1);
	par[1] = g3->GetParameter(2);

	return;

}

                               
// Function which perfoms the three gauss fit
void fitGauss2(TH1F *histo)
{

	Float_t mean = histo->GetMean();
	Float_t rms = histo->GetRMS();
	g1 = new TF1("m1","gaus",mean-rms,mean+rms);
	histo->Fit(g1,"RQN");
	mean = g1->GetParameter(1);
	Float_t sigma = g1->GetParameter(2);
	g2 = new TF1("m2","gaus",mean-(1.5*sigma),mean+(1.5*sigma));
	histo->Fit(g2,"RQN");
	mean = g2->GetParameter(1);
	sigma = g2->GetParameter(2);
	g3 = new TF1("m3","gaus",mean-(1.5*sigma),mean+(1.5*sigma));
	histo->Fit(g3,"R");

	cout << g3->GetParameter(2)/TMath::Sqrt(2) << endl;

	return;
	
}

// Funtion which randomizes the TDC values
Float_t randomize(Float_t t) {

	TRandom q;
	Float_t ran=0;
	q.SetSeed(time(NULL));
	ran = 1 - 2*q.Rndm();
	t = t + ran;
	return t;

}

// Fuction which calculates position offset
// Returns the mean of the Left-Right time difference histogram changing its sign
Float_t getHistOffset(TH1F *histo)
{

	Float_t mean = histo->GetMean();
	Float_t rms  = histo->GetRMS();

	if(rms!=0 && rms<300)	{ 
		return -mean;
	} else { 
		return -9999.9; 
	}

}
	

// Fuction which calculates position offset
// Performs an algorithm for finding the edges of the Left-Right time difference histogram
// Returns the center of the distribution changing its sign
Float_t getHistogramMiddle(TH1F *histo)
{

	Int_t nbins = histo->GetNbinsX();
	Float_t val=0, prev_val=0;
	Float_t rate, prev_rate=0;
	Float_t cut=0.2;
	Float_t left_side=0, right_side=0;
	Float_t mean_val=0;

	for(Int_t i=0;i<nbins;i++) {

		val = histo->GetBinContent(i);
		if(val!=0) rate = (prev_val/val);
		if(prev_rate<cut && cut<rate)	left_side = histo->GetBinLowEdge(i);
		if(rate<cut && cut<prev_rate)	right_side = histo->GetBinLowEdge(i);
		if(left_side!=0 && right_side!=0) {
			mean_val = (left_side+right_side)/2;
			break;
		}
		prev_val = val;
		prev_rate = rate;
		
	}

  return -mean_val;
	
}
	

// Fuction which calculates T_left-T_right width 
// Performs an algorithm for finding the edges of the Left-Right time difference histogram
// Returns the width of the distribution divided by 2
Float_t getCellTimeWidth(TH1F *histo)
{

	Int_t nbins = histo->GetNbinsX();
	Float_t val=0;
	Float_t left_edge=0., right_edge=0.;
	Float_t width=0.;

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
			 	left_edge = histo->GetBinLowEdge(i);
			if(histo->GetBinContent(i+1)==0 && histo->GetBinContent(i+2)==0 && histo->GetBinContent(i-1)>0 && histo->GetBinContent(i-2)>0 && histo->GetBinContent(i-3)>0 && histo->GetBinContent(i-4)>0)
			 	right_edge = histo->GetBinLowEdge(i) + histo->GetBinWidth(i);
		}
		
		//cout << left_edge << " * " << right_edge << " * " << i << " * " << val << endl;

		if(fabs(left_edge)>0 && fabs(right_edge)>0) break;

	}

  width = fabs(right_edge-left_edge);

	if(width > 10000) width=0.; 
	
	return width;
	
}
	
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



// Funcion which calculates the Time_res vs ToT dependency (slewing correction)
// Returns the coeficients of the pol1 fit
void fitSlewingCorrection(TH2F *histo, Float_t *par_array) {

	TProfile *prof = new TProfile();
	prof = histo->ProfileX();
	gr1 = new TF1("gr1","pol1",650,830);
	prof->Fit(gr1,"RQNW");
	gr2 = new TF1("gr2","pol1",830,2000);
	prof->Fit(gr2,"RQNW");
	par_array[0] = gr1->GetParameter(0);
	par_array[1] = gr1->GetParameter(1);
	par_array[2] = gr2->GetParameter(0);
	par_array[3] = gr2->GetParameter(1);

	return;
}
	
// Funcion which calculates the Time_res vs ToT dependency (slewing correction)
// Returns the coeficients of the four parameter exponential fit
Double_t expo4(Double_t *x,Double_t *par) {
    Double_t p0  = par[0];
    Double_t p1  = par[1];
    Double_t p2  = par[2];
    Double_t p3  = par[3];
    Double_t var = x[0];

    return p0 + p1*var + TMath::Exp( p2 + p3*var ); 
    //return p0 + p1*var + p2*TMath::Exp(p3*var); 
}
Double_t expo4M(Double_t *x,Double_t *par) {
    Double_t p0  = par[0];
    Double_t p1  = par[1];
    Double_t p2  = par[2];
    Double_t p3  = par[3];
    Double_t var = x[0];

    return p0 + p1*var - TMath::Exp( p2 + p3*var ); 
    //return p0 + p1*var + p2*TMath::Exp(p3*var); 
}
Double_t func4(Double_t *x,Double_t *par) {
    Double_t p0  = par[0];
    Double_t p1  = par[1];
    Double_t p2  = par[2];
    Double_t p3  = par[3];
		p3 = 0.01;
    Double_t var = x[0];

    return p0 + p1*var + 1 / ( p2 + p3*var ); 
}
Double_t func3(Double_t *x,Double_t *par) {
    Double_t p0  = par[0];
    Double_t p1  = par[1];
    Double_t p2  = par[2];
		//p1 = 0.01;
    Double_t var = x[0];

    return p0 + p1*var + p2/sqrt(var); 
}
void fitSlewingCorrectionExpo(TH2F *histo, Float_t *par_array) {

	TProfile *prof = new TProfile();
	prof = histo->ProfileX();
	TF1 *func = new TF1("expo_4par",expo4,10,1200,4);
	//TF1 *func = new TF1("fun_4par",func4,10,1400,4);
  func->SetParNames("p0","p1","p2","p3");  
	//prof->Fit("fun_4par","RM");
	prof->Fit("expo_4par","RMW");
	par_array[0] = func->GetParameter(0);
	par_array[1] = func->GetParameter(1);
	par_array[2] = func->GetParameter(2);
	par_array[3] = func->GetParameter(3);

	return;
}
void fitSlewingCorrectionExpoM(TH2F *histo, Float_t *par_array) {

	TProfile *prof = new TProfile();
	prof = histo->ProfileX();
	TF1 *func = new TF1("expo_4parM",expo4M,20,1500,4);
	//TF1 *func = new TF1("fun_4par",func4,65,600,4);
  func->SetParNames("p0","p1","p2","p3");  
	prof->Fit("expo_4parM","RM");
	//prof->Fit("expo_4parM");
	//prof->Fit("fun_4par","RM");
	par_array[0] = func->GetParameter(0);
	par_array[1] = func->GetParameter(1);
	par_array[2] = func->GetParameter(2);
	par_array[3] = func->GetParameter(3);

	return;
}
void fitSlewingCorrectionInv(TH2F *histo, Float_t *par_array) {

	TProfile *prof = new TProfile();
	prof = histo->ProfileX();
	TF1 *func = new TF1("fun_4par",func4,65,450,4);
  func->SetParNames("p0","p1","p2","p3");  
	prof->Fit("fun_4par","RM");
	par_array[0] = func->GetParameter(0);
	par_array[1] = func->GetParameter(1);
	par_array[2] = func->GetParameter(2);
	par_array[3] = func->GetParameter(3);

	return;
}
	
void fitSlewingCorrection3Par(TH2F *histo, Float_t *par_array) {

	TProfile *prof = new TProfile();
	prof = histo->ProfileX();
	TF1 *func = new TF1("fun_3par",func3,5,4000,3);
  func->SetParNames("p0","p1","p2");  
	prof->Fit("fun_3par","R");
	par_array[0] = func->GetParameter(0);
	par_array[1] = func->GetParameter(1);
	par_array[2] = func->GetParameter(2);

	return;
}
void fitSlewingCorrection3Par(TGraph *Gr, Float_t *par_array) {

	TF1 *func = new TF1("fun_3par",func3,5,12000,3);
  func->SetParNames("p0","p1","p2");  
	Gr->Fit("fun_3par","RM");
	par_array[0] = func->GetParameter(0);
	par_array[1] = func->GetParameter(1);
	par_array[2] = func->GetParameter(2);

	return;
}
	

// Funcion which calculates the Time_res vs Position dependency (position correction)
// Returns the coeficients of the pol2 fit
void fitPositionCorrection(TH2F *histo, Float_t *par_array) {

	TProfile *prof = new TProfile();
	prof = histo->ProfileX();
	gr = new TF1("gr","pol2");
	//gr = new TF1("gr","pol1");
	prof->Fit(gr,"M");
	par_array[0] = gr->GetParameter(0);
	par_array[1] = gr->GetParameter(1);
	par_array[2] = gr->GetParameter(2);

	return;
}

// Funcion which calculates the Time_res vs Position dependency (position correction)
// Returns the coeficients of the pol1 fit
void fitPositionCorrection1(TH2F *histo, Float_t *par_array) {

	TProfile *prof = new TProfile();
	prof = histo->ProfileX();
	gr = new TF1("gr","pol1");
	prof->Fit(gr,"M");
	par_array[0] = gr->GetParameter(0);
	par_array[1] = gr->GetParameter(1);

	return;
}


//Fit to a second order polynimium
void fitPol2(TH2F *histo, Float_t *par_array) {

	TProfile *prof = new TProfile();
	prof = histo->ProfileX();
	gr = new TF1("gr","pol2");
	prof->Fit(gr,"QN");
	par_array[0] = gr->GetParameter(0);
	par_array[1] = gr->GetParameter(1);
	par_array[2] = gr->GetParameter(1);

	return;
}

void printPar(Float_t *limits_array) {
	for(Int_t i=0;i<4;i++) cout << limits_array[i] << endl;
}

//Fit the histogram to three straight lines
void fit3StraightLines(TH2F *histo, Float_t *limits_array, Float_t *par_array) {

	//for(Int_t i=0;i<4;i++) cout << limits_array[i] << endl;

	//limits_array contains the limits of the fits
	//X0-X1,X1-X2,X2-X3
	//par_array stores the corresponding parameters
	TProfile *prof = new TProfile();
	prof = histo->ProfileX();
	f1 = new TF1("f1","pol1",limits_array[0],limits_array[1]);
	prof->Fit(f1,"R");
	f2 = new TF1("f2","pol1",limits_array[1],limits_array[2]);
	prof->Fit(f2,"R");
	f3 = new TF1("f3","pol1",limits_array[2],limits_array[3]);
	prof->Fit(f3,"R");
	par_array[0] = f1->GetParameter(0);
	par_array[1] = f1->GetParameter(1);
	par_array[2] = f2->GetParameter(0);
	par_array[3] = f2->GetParameter(1);
	par_array[4] = f3->GetParameter(0);
	par_array[5] = f3->GetParameter(1);

	return;
}

//Fit the graph to three straight lines
/*void fit3StraightLinesGraph(TGraph *gr, Float_t *limits_array, Float_t *par_array) {

	//for(Int_t i=0;i<4;i++) cout << limits_array[i] << endl;

	//limits_array contains the limits of the fits
	//X0-X1,X1-X2,X2-X3
	//par_array stores the corresponding parameters
	gr->Fit("pol1","+","",limits_array[0],limits_array[1]);
	par_array[0] = pol1->GetParameter(0);
	par_array[1] = pol1->GetParameter(1);
	gr->Fit("pol1","+","",limits_array[1],limits_array[2]);
	par_array[2] = pol1->GetParameter(0);
	par_array[3] = pol1->GetParameter(1);
	gr->Fit("pol1","+","",limits_array[2],limits_array[3]);
	par_array[4] = pol1->GetParameter(0);
	par_array[5] = pol1->GetParameter(1);

	return;
}

//Fit the graph to four straight lines
void fit4StraightLinesGraph(TGraph *gr, Float_t *limits_array, Float_t *par_array) {

	//for(Int_t i=0;i<4;i++) cout << limits_array[i] << endl;

	//limits_array contains the limits of the fits
	//X0-X1,X1-X2,X2-X3
	//par_array stores the corresponding parameters
	gr->Fit("pol1","+","",limits_array[0],limits_array[1]);
	par_array[0] = pol1->GetParameter(0);
	par_array[1] = pol1->GetParameter(1);
	gr->Fit("pol1","+","",limits_array[1],limits_array[2]);
	par_array[2] = pol1->GetParameter(0);
	par_array[3] = pol1->GetParameter(1);
	gr->Fit("pol1","+","",limits_array[2],limits_array[3]);
	par_array[4] = pol1->GetParameter(0);
	par_array[5] = pol1->GetParameter(1);
	gr->Fit("pol1","+","",limits_array[3],limits_array[4]);
	par_array[6] = pol1->GetParameter(0);
	par_array[7] = pol1->GetParameter(1);

	return;
}

//Fit the profile to four straight lines
void fit4StraightLinesProfile(TProfile *prof, Float_t *limits_array, Float_t *par_array) {

	//for(Int_t i=0;i<4;i++) cout << limits_array[i] << endl;

	//limits_array contains the limits of the fits
	//X0-X1,X1-X2,X2-X3
	//par_array stores the corresponding parameters
	prof->Fit("pol1","W+","",limits_array[0],limits_array[1]);
	par_array[0] = pol1->GetParameter(0);
	par_array[1] = pol1->GetParameter(1);
	prof->Fit("pol1","W+","",limits_array[1],limits_array[2]);
	par_array[2] = pol1->GetParameter(0);
	par_array[3] = pol1->GetParameter(1);
	prof->Fit("pol1","W+","",limits_array[2],limits_array[3]);
	par_array[4] = pol1->GetParameter(0);
	par_array[5] = pol1->GetParameter(1);
	prof->Fit("pol1","W+","",limits_array[3],limits_array[4]);
	par_array[6] = pol1->GetParameter(0);
	par_array[7] = pol1->GetParameter(1);

	return;
}

//Fit the profile to three straight lines
void fit3StraightLinesProfile(TProfile *prof, Float_t *limits_array, Float_t *par_array) {

	//for(Int_t i=0;i<4;i++) cout << limits_array[i] << endl;

	//limits_array contains the limits of the fits
	//X0-X1,X1-X2,X2-X3
	//par_array stores the corresponding parameters
	prof->Fit("pol1","W+","",limits_array[0],limits_array[1]);
	par_array[0] = pol1->GetParameter(0);
	par_array[1] = pol1->GetParameter(1);
	prof->Fit("pol1","W+","",limits_array[1],limits_array[2]);
	par_array[2] = pol1->GetParameter(0);
	par_array[3] = pol1->GetParameter(1);
	prof->Fit("pol1","W+","",limits_array[2],limits_array[3]);
	par_array[4] = pol1->GetParameter(0);
	par_array[5] = pol1->GetParameter(1);

	return;
}*/
