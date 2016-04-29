#include "effCommon.h"

double RError(double A, double eA, double B, double eB);
double PError(double A, double eA, double B, double eB);

const int  nCenBin = 9;
const int nPtBin = 1;
double m1S_low = 7.77;
double m1S_high = 10;
double m2S_low = 8.333;
double m2S_high = 10.563;


void DoubleRatioCen(){
        gROOT->Macro("logon.C+");
	cout<<"set styles"<<endl;

        double          CenBin[nCenBin] = {5,15,30,50,70,90,110,130,150};
        double          CenBinErr[nCenBin] = {5,5,10,10,10,10,10,10,10};
        Float_t         CenBinEdges[nCenBin+1] = {0,10,20,40,60,80,100,120,140,160};

        double          ptBin[nPtBin] = {15.0};   //  RapBin
        double          ptBinErr[nPtBin] = {15.0};
        Float_t         ptBinEdges[nPtBin+1] = {0.0,30.0};

	TGraphAsymmErrors* hSingleRatioCenPbPbNum;

        TGraphAsymmErrors* hSingleRatioCenppDen;
	cout<<"declared variables"<<endl;
//        TH1D* hEffRatio = new TH1D("EffRatio", "", nPtBin, ptBinEdges);

        TGraphAsymmErrors* hEffDoubleRatio = new TGraphAsymmErrors(nPtBin);
	cout<<"Make new histogram"<<endl;
//	hEffCenpp2S1S->Draw();


/////////// CenPbPb Single Ratio

        TFile* fEffSingleRatioCenPbPb = new TFile("EffSingleRatioCenPbPb.root", "Open");
        fEffSingleRatioCenPbPb->GetObject("hEffCenPbPb2S1S", hSingleRatioCenPbPbNum);

	cout<<"Loaded 2S efficiencies"<<endl;

////////// Cenpp Single Ratio

        TFile* fEffSingleRatioCenpp = new TFile("EffSingleRatioCenpp.root", "Open");
        fEffSingleRatioCenpp->GetObject("hEffCenpp2S1S", hSingleRatioCenppDen);


///////// Double Ratio CenPbPb(2S/1S)/Cenpp(2S/1S) Calculation

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


        for (Int_t i = 0; i < (nCenBin); i++){
		cout<<"Started loop: i = "<<i<<endl;
                EffNum = hSingleRatioCenPbPbNum->Eval(CenBin[i]);
                cout<<"Grabbed first bin of the numerator"<<endl;
		EffDen = hSingleRatioCenppDen->Eval(ptBin[0]);
                cout<<"Calculated individual efficiencies"<<endl;
		EffNumErrH = hSingleRatioCenPbPbNum->GetErrorYhigh(i);
                EffNumErrL = hSingleRatioCenPbPbNum->GetErrorYlow(i);
		EffDenErrH = hSingleRatioCenppDen->GetErrorYhigh(0);
		EffDenErrL = hSingleRatioCenppDen->GetErrorYlow(0);
		cout<<"Calculated individual errors"<<endl;
		EffRatio = EffNum / EffDen;
		cout<<"Calculated ratio of efficiencies"<<endl;
                EffRatioErrH = RError(EffNum, EffNumErrH, EffDen, EffDenErrH);  //Calculation for the combined efficiency	
                EffRatioErrL = RError(EffNum, EffNumErrL, EffDen, EffDenErrL); //typo EffL -> ErrL
		cout<<"Calculated error ratios"<<endl;

                hEffDoubleRatio->SetPoint((i), CenBin[i], EffRatio);
                hEffDoubleRatio->SetPointError((i), CenBinErr[i], CenBinErr[i], EffRatioErrL, EffRatioErrH);
        }



//	TFile* OutFile;
//        OutFile = new TFile("EffSingleRatiopp.root", "Recreate");
//        hEffpp2S1S->Write();
//        OutFile->Close();


        TCanvas* can1 = new TCanvas("can1", "Canvas with results1", 1000, 680);

	//adding a line
	TLine* line1 = new TLine(0,1,160,1);
        line1->SetLineStyle(kDashed);

        hEffDoubleRatio->SetMarkerSize(2.0);
        hEffDoubleRatio->SetMarkerColor(kRed);
        hEffDoubleRatio->SetMarkerStyle(21);
	hEffDoubleRatio->SetLineColor(kRed);
        hEffDoubleRatio->GetXaxis()->SetTitle("Centrality^{#mu+#mu-}");
        hEffDoubleRatio->GetXaxis()->CenterTitle();

	hEffDoubleRatio->GetYaxis()->SetTitle("Efficiency[#varUpsilon(2S)]/Efficiency[#varUpsilon(1S)]_{PbPb}/Efficiency[#varUpsilon(2S)]/Efficiency[#varUpsilon(1S)]_{pp}");
	hEffDoubleRatio->GetYaxis()->SetRangeUser(0.5, 1.5);
	hEffDoubleRatio->GetXaxis()->SetRangeUser(0.0, 160.0);
	hEffDoubleRatio->GetXaxis()->SetTitleSize(0.05);
	hEffDoubleRatio->GetXaxis()->SetTitleOffset(0.9);
	hEffDoubleRatio->GetYaxis()->SetTitleSize(0.03);
	hEffDoubleRatio->GetYaxis()->SetTitleOffset(1.0);
	hEffDoubleRatio->Draw("AP");
	line1->Draw("SAME");

        can1->Update();
        can1->SaveAs("DoubleRatioVsCen.png");

        for (Int_t i = 0; i < (nCenBin); i++){
        cout << hEffDoubleRatio->Eval(CenBin[i]) << " , - " << hEffDoubleRatio->GetErrorYlow(i) << " , + " << hEffDoubleRatio->GetErrorYhigh(i) << endl;
        }
	  
        cout << "over" << endl;
	
	fEffSingleRatioCenPbPb->Close(); 
	fEffSingleRatioCenpp->Close(); 

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


