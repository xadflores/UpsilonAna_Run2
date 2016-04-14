//Making code a littler cleaner
#include "effCommon.h"








double RError(double A, double eA, double B, double eB);
double PError(double A, double eA, double B, double eB);

const int  nRapBin = 2;
double m1S_low = 7.77;
double m1S_high = 10;
double m2S_low = 8.333;
double m2S_high = 10.563;


void SingleRatioRapPbPb2S1S(){
	cout<<"Started code"<<endl;
        gROOT->Macro("logon.C+");
	cout<<"set styles"<<endl;

        double          RapBin[nRapBin] = {0.6,1.8};   //  RapBin
        double          RapBinErr[nRapBin] = {0.6,0.6};  // RapBinErr
        Float_t         RapBinEdges[nRapBin+1] = {0,1.2,2.4};


	TH1D* hRecoNum;
	TH1D* hGenNum;
	TH1D* hEffNum;

        TH1D* hRecoDen;
        TH1D* hGenDen;
        TH1D* hEffDen;
	cout<<"declared variables"<<endl;
//        TH1D* hEffRatio = new TH1D("EffRatio", "", nPtBin, ptBinEdges);

        TGraphAsymmErrors* hEffRapPbPb2S1S = new TGraphAsymmErrors(nRapBin);
	hEffRapPbPb2S1S->SetName("hEffRapPbPb2S1S");
	cout<<"Make new histogram"<<endl;


/////////// pp 2S

        TFile* fRapPbPbEff2S = new TFile("RapPbPbEff2S.root", "Open");
	fRapPbPbEff2S->GetObject("Eff", hEffNum);

     //   fppEff2S->Close(); //closing before using. deleting from memory?
	cout<<"Loaded 2S efficiencies"<<endl;

////////// pp 1S

	TFile* fRapPbPbEff1S = new TFile("RapPbPbEff1S.root", "Open");
        fRapPbPbEff1S->GetObject("Eff", hEffDen);

       // fppEff1S->Close(); //closing before using. deleting from memory?


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


        for (Int_t i = 1; i < (nRapBin+1); i++){
		cout<<"Started loop: i = "<<i<<endl;
                EffNum = hEffNum->GetBinContent(i);
                cout<<"Grabbed first bin of the numerator"<<endl;
		EffDen = hEffDen->GetBinContent(i);
                cout<<"Calculated individual efficiencies"<<endl;
		EffNumErrH = hEffNum->GetBinErrorUp(i);
                EffNumErrL = hEffNum->GetBinErrorLow(i);
		EffDenErrH = hEffDen->GetBinErrorUp(i);
		EffDenErrL = hEffDen->GetBinErrorLow(i);
		cout<<"Calculated individual errors"<<endl;
		EffRatio = EffNum / EffDen;
		cout<<"Calculated ratio of efficiencies"<<endl;
                EffRatioErrH = RError(EffNum, EffNumErrH, EffDen, EffDenErrH);  //Calculation for the combined efficiency	
                EffRatioErrL = RError(EffNum, EffNumErrL, EffDen, EffDenErrL); //typo EffL -> ErrL
		cout<<"Calculated error ratios"<<endl;

                hEffRapPbPb2S1S->SetPoint((i - 1), RapBin[i - 1], EffRatio);
                hEffRapPbPb2S1S->SetPointError((i - 1), RapBinErr[i - 1], RapBinErr[i - 1], EffRatioErrL, EffRatioErrH);

//		hist->SetBinContent(i, EffRatio);
//		hist->SetBinError

        }



	TFile* OutFile;
        OutFile = new TFile("EffSingleRatioRapPbPb.root", "Recreate");
        hEffRapPbPb2S1S->Write();

        OutFile->Close();


        TCanvas* can1 = new TCanvas("can1", "Canvas with results1", 1000, 680);

	//adding a line
	TLine* line1 = new TLine(0,1,2.4,1);
        line1->SetLineStyle(kDashed);

        hEffRapPbPb2S1S->SetMarkerSize(2.0);
        hEffRapPbPb2S1S->SetMarkerColor(kRed);
        hEffRapPbPb2S1S->SetMarkerStyle(21);
	hEffRapPbPb2S1S->SetLineColor(kRed);
        hEffRapPbPb2S1S->GetXaxis()->SetTitle("#eta");
        hEffRapPbPb2S1S->GetXaxis()->CenterTitle();

	hEffRapPbPb2S1S->GetYaxis()->SetTitle("Efficiency[#varUpsilon(2S)/#varUpsilon(1S)]_{PbPb}");
	hEffRapPbPb2S1S->GetYaxis()->SetRangeUser(0.5, 1.5);
//	hEffRapPbPb2S1S->GetXaxis()->SetRangeUser(0.0, 2.5);
	hEffRapPbPb2S1S->GetXaxis()->SetTitleSize(0.05);
	hEffRapPbPb2S1S->GetXaxis()->SetTitleOffset(0.9);
	hEffRapPbPb2S1S->GetYaxis()->SetTitleSize(0.05);
	hEffRapPbPb2S1S->GetYaxis()->SetTitleOffset(0.9);
	hEffRapPbPb2S1S->Draw("AP");
	line1->Draw("SAME");
	  
        cout << "over" << endl;
	
	fRapPbPbEff1S->Close(); 
	fRapPbPbEff2S->Close(); 

/*
        TCanvas* can2 = new TCanvas("can2", "Canvas with results2", 1000, 680);
        hgEffpp2S->SetMarkerSize(2.0);
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


