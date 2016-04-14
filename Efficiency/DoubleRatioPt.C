#include "effCommon.h"

double RError(double A, double eA, double B, double eB);
double PError(double A, double eA, double B, double eB);

const int  nPtBin = 3;
double m1S_low = 7.77;
double m1S_high = 10;
double m2S_low = 8.333;
double m2S_high = 10.563;


void DoubleRatioPt(){
        gROOT->Macro("logon.C+");
	cout<<"set styles"<<endl;

        double          ptBin[nPtBin] = {2.5,8.5,21};   //  RapBin
        double          ptBinErr[nPtBin] = {2.5,3.5,9};
        Float_t         ptBinEdges[nPtBin+1] = {0.0,5.0,12.0,30.0};

	TGraphAsymmErrors* hSingleRatioPbPbNum;

        TGraphAsymmErrors* hSingleRatioppDen;
	cout<<"declared variables"<<endl;
//        TH1D* hEffRatio = new TH1D("EffRatio", "", nPtBin, ptBinEdges);

        TGraphAsymmErrors* hEffDoubleRatio = new TGraphAsymmErrors(nPtBin);
	cout<<"Make new histogram"<<endl;
//	hEffpp2S1S->Draw();


/////////// PbPb Single Ratio

        TFile* fEffSingleRatioPbPb = new TFile("EffSingleRatioPbPb.root", "Open");
        fEffSingleRatioPbPb->GetObject("hEffPbPb2S1S", hSingleRatioPbPbNum);

	cout<<"Loaded 2S efficiencies"<<endl;

////////// pp Single Ratio

        TFile* fEffSingleRatiopp = new TFile("EffSingleRatiopp.root", "Open");
        fEffSingleRatiopp->GetObject("hEffpp2S1S", hSingleRatioppDen);


///////// Double Ratio PbPb(2S/1S)/pp(2S/1S) Calculation

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
                EffNum = hSingleRatioPbPbNum->Eval(ptBin[i]);
                cout<<"Grabbed first bin of the numerator"<<endl;
		EffDen = hSingleRatioppDen->Eval(ptBin[i]);
                cout<<"Calculated individual efficiencies"<<endl;
		EffNumErrH = hSingleRatioPbPbNum->GetErrorYhigh(i);
                EffNumErrL = hSingleRatioPbPbNum->GetErrorYlow(i);
		EffDenErrH = hSingleRatioppDen->GetErrorYhigh(i);
		EffDenErrL = hSingleRatioppDen->GetErrorYlow(i);
		cout<<"Calculated individual errors"<<endl;
		EffRatio = EffNum / EffDen;
		cout<<"Calculated ratio of efficiencies"<<endl;
                EffRatioErrH = RError(EffNum, EffNumErrH, EffDen, EffDenErrH);  //Calculation for the combined efficiency	
                EffRatioErrL = RError(EffNum, EffNumErrL, EffDen, EffDenErrL); //typo EffL -> ErrL
		cout<<"Calculated error ratios"<<endl;

                hEffDoubleRatio->SetPoint((i), ptBin[i], EffRatio);
                hEffDoubleRatio->SetPointError((i), ptBinErr[i], ptBinErr[i], EffRatioErrL, EffRatioErrH);
        }



//	TFile* OutFile;
//        OutFile = new TFile("EffSingleRatiopp.root", "Recreate");
//        hEffpp2S1S->Write();
//        OutFile->Close();


        TCanvas* can1 = new TCanvas("can1", "Canvas with results1", 1000, 680);

	//adding a line
	TLine* line1 = new TLine(0,1,30,1);
        line1->SetLineStyle(kDashed);

        hEffDoubleRatio->SetMarkerSize(2.0);
        hEffDoubleRatio->SetMarkerColor(kRed);
        hEffDoubleRatio->SetMarkerStyle(21);
	hEffDoubleRatio->SetLineColor(kRed);
        hEffDoubleRatio->GetXaxis()->SetTitle("p^{#mu+#mu-}_{T}");
        hEffDoubleRatio->GetXaxis()->CenterTitle();

	hEffDoubleRatio->GetYaxis()->SetTitle("Efficiency[#varUpsilon(2S)/#varUpsilon(1S)]_{PbPb}/Efficiency[#varUpsilon(2S)/#varUpsilon(1S)]_{pp}");
	hEffDoubleRatio->GetYaxis()->SetRangeUser(0.5, 1.5);
	hEffDoubleRatio->GetXaxis()->SetRangeUser(0.0, 30.0);
	hEffDoubleRatio->GetXaxis()->SetTitleSize(0.05);
	hEffDoubleRatio->GetXaxis()->SetTitleOffset(0.9);
	hEffDoubleRatio->GetYaxis()->SetTitleSize(0.04);
	hEffDoubleRatio->GetYaxis()->SetTitleOffset(0.9);
	hEffDoubleRatio->Draw("AP");
	line1->Draw("SAME");
	  
        cout << "over" << endl;
	
	fEffSingleRatioPbPb->Close(); 
	fEffSingleRatiopp->Close(); 

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


