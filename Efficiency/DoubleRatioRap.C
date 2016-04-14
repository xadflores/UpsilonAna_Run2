#include "effCommon.h"

double RError(double A, double eA, double B, double eB);
double PError(double A, double eA, double B, double eB);

const int  nRapBin = 2;
double m1S_low = 7.77;
double m1S_high = 10;
double m2S_low = 8.333;
double m2S_high = 10.563;


void DoubleRatioRap(){
        gROOT->Macro("logon.C+");
	cout<<"set styles"<<endl;

        double          RapBin[nRapBin] = {0.6,1.8};   //  RapBin
        double          RapBinErr[nRapBin] = {0.6,0.6};  // RapBinErr
        Float_t         RapBinEdges[nRapBin+1] = {0,1.2,2.4};

	TGraphAsymmErrors* hSingleRatioRapPbPbNum;

        TGraphAsymmErrors* hSingleRatioRapppDen;
	cout<<"declared variables"<<endl;
//        TH1D* hEffRatio = new TH1D("EffRatio", "", nRapBin, RapBinEdges);

        TGraphAsymmErrors* hEffDoubleRatio = new TGraphAsymmErrors(nRapBin);
	cout<<"Make new histogram"<<endl;
//	hEffRappp2S1S->Draw();


/////////// RapPbPb Single Ratio

        TFile* fEffSingleRatioRapPbPb = new TFile("EffSingleRatioRapPbPb.root", "Open");
        fEffSingleRatioRapPbPb->GetObject("hEffRapPbPb2S1S", hSingleRatioRapPbPbNum);

	cout<<"Loaded 2S efficiencies"<<endl;

////////// Rappp Single Ratio

        TFile* fEffSingleRatioRappp = new TFile("EffSingleRatioRappp.root", "Open");
        fEffSingleRatioRappp->GetObject("hEffRappp2S1S", hSingleRatioRapppDen);


///////// Double Ratio RapPbPb(2S/1S)/Rappp(2S/1S) Calculation

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


        for (Int_t i = 0; i < (nRapBin); i++){
		cout<<"Started loop: i = "<<i<<endl;
                EffNum = hSingleRatioRapPbPbNum->Eval(RapBin[i]);
                cout<<"Grabbed first bin of the numerator"<<endl;
		EffDen = hSingleRatioRapppDen->Eval(RapBin[i]);
                cout<<"Calculated individual efficiencies"<<endl;
		EffNumErrH = hSingleRatioRapPbPbNum->GetErrorYhigh(i);
                EffNumErrL = hSingleRatioRapPbPbNum->GetErrorYlow(i);
		EffDenErrH = hSingleRatioRapppDen->GetErrorYhigh(i);
		EffDenErrL = hSingleRatioRapppDen->GetErrorYlow(i);
		cout<<"Calculated individual errors"<<endl;
		EffRatio = EffNum / EffDen;
		cout<<"Calculated ratio of efficiencies"<<endl;
                EffRatioErrH = RError(EffNum, EffNumErrH, EffDen, EffDenErrH);  //Calculation for the combined efficiency	
                EffRatioErrL = RError(EffNum, EffNumErrL, EffDen, EffDenErrL); //typo EffL -> ErrL
		cout<<"Calculated error ratios"<<endl;

                hEffDoubleRatio->SetPoint((i), RapBin[i], EffRatio);
                hEffDoubleRatio->SetPointError((i), RapBinErr[i], RapBinErr[i], EffRatioErrL, EffRatioErrH);
        }



//	TFile* OutFile;
//        OutFile = new TFile("EffSingleRatioRappp.root", "Recreate");
//        hEffRappp2S1S->Write();
//        OutFile->Close();


        TCanvas* can1 = new TCanvas("can1", "Canvas with results1", 1000, 680);

	//adding a line
	TLine* line1 = new TLine(0,1,2.4,1);
        line1->SetLineStyle(kDashed);

        hEffDoubleRatio->SetMarkerSize(2.0);
        hEffDoubleRatio->SetMarkerColor(kRed);
        hEffDoubleRatio->SetMarkerStyle(21);
	hEffDoubleRatio->SetLineColor(kRed);
        hEffDoubleRatio->GetXaxis()->SetTitle("|y^{#mu+#mu-}|");
        hEffDoubleRatio->GetXaxis()->CenterTitle();

	hEffDoubleRatio->GetYaxis()->SetTitle("Efficiency[#varUpsilon(2S)]/Efficieny[#varUpsilon(1S)]_{PbPb}/Efficiency[#varUpsilon(2S)]/Efficiency[#varUpsilon(1S)]_{pp}");
	hEffDoubleRatio->GetYaxis()->SetRangeUser(0.5, 1.5);
//	hEffDoubleRatio->GetXaxis()->SetRangeUser(0.0, 30.0);
	hEffDoubleRatio->GetXaxis()->SetTitleSize(0.05);
	hEffDoubleRatio->GetXaxis()->SetTitleOffset(0.9);
	hEffDoubleRatio->GetYaxis()->SetTitleSize(0.03);
	hEffDoubleRatio->GetYaxis()->SetTitleOffset(1.0);
	hEffDoubleRatio->Draw("AP");
	line1->Draw("SAME");

        can1->Update();
        can1->SaveAs("DoubleRatioVsRap.png");
	  
        cout << "over" << endl;
	
	fEffSingleRatioRapPbPb->Close(); 
	fEffSingleRatioRappp->Close(); 

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


