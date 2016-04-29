//Making code a littler cleaner
#include "effCommon.h"








double RError(double A, double eA, double B, double eB);
double PError(double A, double eA, double B, double eB);

const int  nPtBin = 1;
double m1S_low = 7.77;
double m1S_high = 10;
double m2S_low = 8.333;
double m2S_high = 10.563;


void SingleRatioCenppFixing2S1S(){
	cout<<"Started code"<<endl;
        gROOT->Macro("logon.C+");
	cout<<"set styles"<<endl;

        double          ptBin[nPtBin] = {15.0};   //  RapBin
        double          ptBinErr[nPtBin] = {15.0};
        Float_t         ptBinEdges[nPtBin+1] = {0.0,30.0};


	TH1D* hRecoNum;
	TH1D* hGenNum;
	TGraphAsymmErrors* hEffNum;

        TH1D* hRecoDen;
        TH1D* hGenDen;
        TGraphAsymmErrors* hEffDen;
	cout<<"declared variables"<<endl;
//        TH1D* hEffRatio = new TH1D("EffRatio", "", nPtBin, ptBinEdges);

        TGraphAsymmErrors* hEffCenpp2S1S = new TGraphAsymmErrors(nPtBin);
	hEffCenpp2S1S->SetName("hEffCenpp2S1S");
	cout<<"Make new histogram"<<endl;
//	hEffCenpp2S1S->Draw();


/////////// Cenpp 2S

        TFile* fCenppEff2S = new TFile("CenppEff2S.root", "Open");
        fCenppEff2S->GetObject("Reco", hRecoNum);
        fCenppEff2S->GetObject("Gen", hGenNum);
	fCenppEff2S->GetObject("Eff", hEffNum);

     //   fCenppEff2S->Close(); //closing before using. deleting from memory?
	cout<<"Loaded 2S efficiencies"<<endl;

////////// Cenpp 1S

	TFile* fCenppEff1S = new TFile("CenppEff1S.root", "Open");
        fCenppEff1S->GetObject("Reco", hRecoDen);
        fCenppEff1S->GetObject("Gen", hGenDen);
        fCenppEff1S->GetObject("Eff", hEffDen);

       // fCenppEff1S->Close(); //closing before using. deleting from memory?


///////// Single Ratio 2S/1S Calculation

        double EffRatio;
        double EffNum;
        double EffNumErrH;
        double EffDenErrH;
        double EffNumErrL;
        double EffDenErrL;
        double EffDen;
        //previously not defined
	double EffRatioErrH;
        double EffRatioErrL;

//                hEffRatio->Divide(hEffNum, hEffDen);


        for (Int_t i = 0; i < (nPtBin); i++){
		cout<<"Started loop: i = "<<i<<endl;
                EffNum = hEffNum->Eval(ptBin[i]);
                cout<<"Grabbed first bin of the numerator"<<endl;
                EffDen = hEffDen->Eval(ptBin[i]);
                cout<<"Calculated individual efficiencies"<<endl;
                EffNumErrH = hEffNum->GetErrorYhigh(i);
                EffNumErrL = hEffNum->GetErrorYlow(i);
                EffDenErrH = hEffDen->GetErrorYhigh(i);
                EffDenErrL = hEffDen->GetErrorYlow(i);
		cout<<"Calculated individual errors"<<endl;
		EffRatio = EffNum / EffDen;
		cout<<"Calculated ratio of efficiencies"<<endl;
                EffRatioErrH = RError(EffNum, EffNumErrH, EffDen, EffDenErrH);  //Calculation for the combined efficiency	
                EffRatioErrL = RError(EffNum, EffNumErrL, EffDen, EffDenErrL); //typo EffL -> ErrL
		cout<<"Calculated error ratios"<<endl;

                hEffCenpp2S1S->SetPoint((i), ptBin[i], EffRatio);
                hEffCenpp2S1S->SetPointError((i), ptBinErr[i], ptBinErr[i], EffRatioErrL, EffRatioErrH);

//		hist->SetBinContent(i, EffRatio);
//		hist->SetBinError

        }



	TFile* OutFile;
        OutFile = new TFile("EffSingleRatioCenpp.root", "Recreate");
        hEffCenpp2S1S->Write();

        OutFile->Close();


        TCanvas* can1 = new TCanvas("can1", "Canvas with results1", 1000, 680);

	//adding a line
	TLine* line1 = new TLine(0,1,30,1);
        line1->SetLineStyle(kDashed);

        hEffCenpp2S1S->SetMarkerSize(2.0);
        hEffCenpp2S1S->SetMarkerColor(kRed);
        hEffCenpp2S1S->SetMarkerStyle(21);
	hEffCenpp2S1S->SetLineColor(kRed);
        hEffCenpp2S1S->GetXaxis()->SetTitle("p^{#mu+#mu-}_{T}");
        hEffCenpp2S1S->GetXaxis()->CenterTitle();

	hEffCenpp2S1S->GetYaxis()->SetTitle("Efficiency[#varUpsilon(2S)/#varUpsilon(1S)]_{pp}");
	hEffCenpp2S1S->GetYaxis()->SetRangeUser(0.5, 1.5);
//	hEffCenpp2S1S->GetXaxis()->SetRangeUser(0.0, 30.0);
	hEffCenpp2S1S->GetXaxis()->SetTitleSize(0.05);
	hEffCenpp2S1S->GetXaxis()->SetTitleOffset(0.9);
	hEffCenpp2S1S->GetYaxis()->SetTitleSize(0.05);
	hEffCenpp2S1S->GetYaxis()->SetTitleOffset(0.9);
	hEffCenpp2S1S->Draw("AP");
	line1->Draw("SAME");
	  
        cout << "over" << endl;
	
	fCenppEff1S->Close(); 
	fCenppEff2S->Close(); 

/*
        TCanvas* can2 = new TCanvas("can2", "Canvas with results2", 1000, 680);
        hgEffCenpp2S->SetMarkerSize(2.0);
        hgEffpp2S->SetMarkerColor(kBlue);
        hgEffpp2S->SetMarkerStyle(20);
        hgEffpp2S->SetLineColor(kBlue);
        hgEffpp2S->GetXaxis()->SetTitle("p_{T}");
        hgEffpp2S->GetYaxis()->SetTitle("Efficiency");
        hgEffpp2S->GetYaxis()->SetRangeUser(0, 1);
        hgEffpp2S->GetXaxis()->SetTitleSize(0.05);
        hgEffpp2S->GetXaxis()->SetTitleOffset(0.9);
        hgEffpp2S->GetYaxis()->SetTitleSize(0.05);
        hgEffpp2S->GetYaxis()->SetTitleOffset(0.9);
        hgEffpp2S->Draw("AP");

        hgEffpp1S->SetMarkerSize(2.0);
        hgEffpp1S->SetMarkerColor(kRed);
        hgEffpp1S->SetMarkerStyle(21);
        hgEffpp1S->SetLineColor(kRed);
        hgEffpp1S->Draw("P");
// */


}




//Ratio Error Propogation
double RError(double A, double eA, double B, double eB){
	 double f=A/B;
	 double fA=eA/A;
	 double fB=eB/B;
	 double eR=  f*sqrt( (fA*fA + fB*fB )) ;
	 return eR;
}


//Product Error Propogation
double PError(double A, double eA, double B, double eB){
	 double f=A*B;
	 double fA=eA/A;
	 double fB=eB/B;
	 double eR=  f*sqrt( (fA*fA + fB*fB )) ;
	 return eR;
}


