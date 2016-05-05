//Making code a littler cleaner
#include "effCommon.h"








double RError(double A, double eA, double B, double eB);
double PError(double A, double eA, double B, double eB);

const int  nCenBin = 9;
const int  nPtBin = 3;
const int  nRapBin = 2;
const int  nIntBin = 1;


void doubleRatioEff(){
	cout<<"Started code"<<endl;
        gROOT->Macro("logon.C+");
	cout<<"set styles"<<endl;

        double          CenBin[nCenBin] = {5,15,30,50,70,90,110,130,150};
        double          CenBinErr[nCenBin] = {5,5,10,10,10,10,10,10,10};

        double          ptBin[nPtBin] = {2.5,8.5,21};
        double          ptBinErr[nPtBin] = {2.5,3.5,9};

        double          rapBin[nRapBin] = {0.6,1.8};
        double          rapBinErr[nRapBin] = {0.6,0.6};

        double          intBin[1] = {80};
        double          intBinErr[1] = {80};

	TGraphAsymmErrors* hEffCenSinglePbPb;
	TGraphAsymmErrors* hEffIntSinglePbPb;
	TGraphAsymmErrors* hEffPtSinglePbPb;
	TGraphAsymmErrors* hEffRapSinglePbPb;

        TGraphAsymmErrors* hEffIntSinglePP;
        TGraphAsymmErrors* hEffPtSinglePP;
        TGraphAsymmErrors* hEffRapSinglePP;


	cout<<"declared variables"<<endl;

        TGraphAsymmErrors* hEffCenDouble = new TGraphAsymmErrors(nCenBin);
        TGraphAsymmErrors* hEffPtDouble = new TGraphAsymmErrors(nPtBin);
        TGraphAsymmErrors* hEffRapDouble = new TGraphAsymmErrors(nRapBin);
        TGraphAsymmErrors* hEffIntDouble = new TGraphAsymmErrors(nIntBin);


/////////// pp 2S

        TFile* fEffSinglePbPb = new TFile("EffSingleRatio_PbPb.root", "Open");
	fEffSinglePbPb->GetObject("hEffCenSingle", hEffCenSinglePbPb);
	fEffSinglePbPb->GetObject("hEffIntSingle", hEffIntSinglePbPb);
	fEffSinglePbPb->GetObject("hEffPtSingle", hEffPtSinglePbPb);
	fEffSinglePbPb->GetObject("hEffRapSingle", hEffRapSinglePbPb);
	
        TFile* fEffSinglePP = new TFile("EffSingleRatio_PP.root", "Open");
	fEffSinglePP->GetObject("hEffIntSingle", hEffIntSinglePP);
	fEffSinglePP->GetObject("hEffPtSingle", hEffPtSinglePP);
	fEffSinglePP->GetObject("hEffRapSingle", hEffRapSinglePP);

///////// double Ratio 2S/1S Calculation

        double EffRatio;
        double EffNum;
        double EffNumErrH;
        double EffDenErrH;
        double EffNumErrL;
        double EffDenErrL;
        double EffDen;
	double EffRatioErrH;
        double EffRatioErrL;

//                hEffRatio->Divide(hEffNum, hEffDen);

        for (Int_t i = 0; i < nCenBin; i++){
		cout<<"Started loop: i = "<<i<<endl;
                EffNum = hEffCenSinglePbPb->Eval(CenBin[i]);
                cout<<"Grabbed first bin of the numerator"<<endl;
                EffDen = hEffIntSinglePP->Eval(intBin[0]);
                cout<<"Calculated individual efficiencies"<<endl;
                EffNumErrH = hEffCenSinglePbPb->GetErrorYhigh(i);
                EffNumErrL = hEffCenSinglePbPb->GetErrorYlow(i);
                EffDenErrH = hEffIntSinglePP->GetErrorYhigh(0);
                EffDenErrL = hEffIntSinglePP->GetErrorYlow(0);
		cout<<"Calculated individual errors"<<endl;
		EffRatio = EffNum / EffDen;
		cout<<"Calculated ratio of efficiencies"<<endl;
                EffRatioErrH = RError(EffNum, EffNumErrH, EffDen, EffDenErrH);  //Calculation for the combined efficiency	
                EffRatioErrL = RError(EffNum, EffNumErrL, EffDen, EffDenErrL); //typo EffL -> ErrL
		cout<<"Calculated error ratios"<<endl;

                hEffCenDouble->SetPoint(i, CenBin[i], EffRatio);
                hEffCenDouble->SetPointError(i, CenBinErr[i], CenBinErr[i], EffRatioErrL, EffRatioErrH);

		
	}

        EffRatio = 0;
        EffNum = 0;
        EffNumErrH = 0;
        EffDenErrH = 0;
        EffNumErrL = 0;
        EffDenErrL = 0;
	EffDen = 0;
	EffRatioErrH = 0;
        EffRatioErrL = 0;


        for (Int_t i = 0; i < nPtBin; i++){
		cout<<"Started loop: i = "<<i<<endl;
                EffNum = hEffPtSinglePbPb->Eval(ptBin[i]);
                cout<<"Grabbed first bin of the numerator"<<endl;
                EffDen = hEffPtSinglePP->Eval(ptBin[i]);
                cout<<"Calculated individual efficiencies"<<endl;
                EffNumErrH = hEffPtSinglePbPb->GetErrorYhigh(i);
                EffNumErrL = hEffPtSinglePbPb->GetErrorYlow(i);
                EffDenErrH = hEffPtSinglePP->GetErrorYhigh(i);
                EffDenErrL = hEffPtSinglePP->GetErrorYlow(i);
		cout<<"Calculated individual errors"<<endl;
		EffRatio = EffNum / EffDen;
		cout<<"Calculated ratio of efficiencies"<<endl;
                EffRatioErrH = RError(EffNum, EffNumErrH, EffDen, EffDenErrH);  //Calculation for the combined efficiency	
                EffRatioErrL = RError(EffNum, EffNumErrL, EffDen, EffDenErrL); //typo EffL -> ErrL
		cout<<"Calculated error ratios"<<endl;

                hEffPtDouble->SetPoint(i, ptBin[i], EffRatio);
                hEffPtDouble->SetPointError(i, ptBinErr[i], ptBinErr[i], EffRatioErrL, EffRatioErrH);

//		hist->SetBinContent(i, EffRatio);
//		hist->SetBinError

        }

        EffRatio = 0;
        EffNum = 0;
        EffNumErrH = 0;
        EffDenErrH = 0;
        EffNumErrL = 0;
        EffDenErrL = 0;
	EffDen = 0;
	EffRatioErrH = 0;
        EffRatioErrL = 0;

        for (Int_t i = 0; i < nRapBin; i++){
		cout<<"Started loop: i = "<<i<<endl;
                EffNum = hEffRapSinglePbPb->Eval(rapBin[i]);
                cout<<"Grabbed first bin of the numerator"<<endl;
                EffDen = hEffRapSinglePP->Eval(rapBin[i]);
                cout<<"Calculated individual efficiencies"<<endl;
                EffNumErrH = hEffRapSinglePbPb->GetErrorYhigh(i);
                EffNumErrL = hEffRapSinglePbPb->GetErrorYlow(i);
                EffDenErrH = hEffRapSinglePP->GetErrorYhigh(i);
                EffDenErrL = hEffRapSinglePP->GetErrorYlow(i);
		cout<<"Calculated individual errors"<<endl;
		EffRatio = EffNum / EffDen;
		cout<<"Calculated ratio of efficiencies"<<endl;
                EffRatioErrH = RError(EffNum, EffNumErrH, EffDen, EffDenErrH);  //Calculation for the combined efficiency	
                EffRatioErrL = RError(EffNum, EffNumErrL, EffDen, EffDenErrL); //typo EffL -> ErrL
		cout<<"Calculated error ratios"<<endl;

                hEffRapDouble->SetPoint(i, rapBin[i], EffRatio);
                hEffRapDouble->SetPointError(i, rapBinErr[i], rapBinErr[i], EffRatioErrL, EffRatioErrH);

//		hist->SetBinContent(i, EffRatio);
//		hist->SetBinError

        }
        EffRatio = 0;
        EffNum = 0;
        EffNumErrH = 0;
        EffDenErrH = 0;
        EffNumErrL = 0;
        EffDenErrL = 0;
	EffDen = 0;
	EffRatioErrH = 0;
        EffRatioErrL = 0;

        for (Int_t i = 0; i < nIntBin; i++){
		cout<<"Started loop: i = "<<i<<endl;
                EffNum = hEffIntSinglePbPb->Eval(intBin[i]);
                cout<<"Grabbed first bin of the numerator"<<endl;
                EffDen = hEffIntSinglePP->Eval(intBin[i]);
                cout<<"Calculated individual efficiencies"<<endl;
                EffNumErrH = hEffIntSinglePbPb->GetErrorYhigh(i);
                EffNumErrL = hEffIntSinglePbPb->GetErrorYlow(i);
                EffDenErrH = hEffIntSinglePP->GetErrorYhigh(i);
                EffDenErrL = hEffIntSinglePP->GetErrorYlow(i);
		cout<<"Calculated individual errors"<<endl;
		EffRatio = EffNum / EffDen;
		cout<<"Calculated ratio of efficiencies"<<endl;
                EffRatioErrH = RError(EffNum, EffNumErrH, EffDen, EffDenErrH);  //Calculation for the combined efficiency	
                EffRatioErrL = RError(EffNum, EffNumErrL, EffDen, EffDenErrL); //typo EffL -> ErrL
		cout<<"Calculated error ratios"<<endl;

                hEffIntDouble->SetPoint(i, intBin[i], EffRatio);
                hEffIntDouble->SetPointError(i, intBinErr[i], intBinErr[i], EffRatioErrL, EffRatioErrH);

//		hist->SetBinContent(i, EffRatio);
//		hist->SetBinError

        }


/*
	TFile* OutFile;
        OutFile = new TFile(Form("EffSingleRatio_%s.root",isPbPb ? "PbPb": "PP"), "Recreate");
        hEffCen2S1S->Write();
        hEffPt2S1S->Write();
        hEffRap2S1S->Write();
        hEffInt2S1S->Write();
        
        OutFile->Close();
*/
	
        TCanvas* c1 = new TCanvas("c1", "Canvas with results1", 600, 600);
        c1->cd();

	//adding a line
	TLine* line1 = new TLine(0,1,160,1);
        line1->SetLineStyle(kDashed);

        hEffCenDouble->SetMarkerSize(2.0);
        hEffCenDouble->SetMarkerColor(kRed);
        hEffCenDouble->SetMarkerStyle(21);
	hEffCenDouble->SetLineColor(kRed);
        hEffCenDouble->SetTitle("");
        hEffCenDouble->GetXaxis()->SetTitle("Centrality");
        hEffCenDouble->GetXaxis()->CenterTitle();
	hEffCenDouble->GetYaxis()->CenterTitle();
	hEffCenDouble->GetYaxis()->SetTitle("Efficiency[#varUpsilon(2S)]/Efficieny[#varUpsilon(1S)]_{PbPb}/Efficiency[#varUpsilon(2S)]/Efficiency[#varUpsilon(1S)]_{pp}");
	hEffCenDouble->GetYaxis()->SetRangeUser(0.5, 1.5);
	hEffCenDouble->GetXaxis()->SetRangeUser(0.0, 160);
        hEffCenDouble->Draw("AP");
	line1->Draw("");

        c1->SaveAs("DoubleRatioEff_Cent.png");	
        
	TCanvas* c2 = new TCanvas("c2", "Canvas with results1", 600, 600);
        c2->cd();

	TLine* line2 = new TLine(0,1,30,1);
        line2->SetLineStyle(kDashed);

        hEffPtDouble->SetMarkerSize(2.0);
        hEffPtDouble->SetMarkerColor(kRed);
        hEffPtDouble->SetMarkerStyle(21);
	hEffPtDouble->SetLineColor(kRed);
        hEffPtDouble->SetTitle("");
        hEffPtDouble->GetXaxis()->SetTitle("p_{T}");
        hEffPtDouble->GetXaxis()->CenterTitle();
	hEffPtDouble->GetYaxis()->CenterTitle();
	hEffPtDouble->GetYaxis()->SetTitle("Efficiency[#varUpsilon(2S)]/Efficieny[#varUpsilon(1S)]_{PbPb}/Efficiency[#varUpsilon(2S)]/Efficiency[#varUpsilon(1S)]_{pp}");
	hEffPtDouble->GetYaxis()->SetRangeUser(0.5, 1.5);
	hEffPtDouble->GetXaxis()->SetRangeUser(0.0, 30);
	hEffPtDouble->Draw("AP");
	line2->Draw("");

        c2->SaveAs("DoubleRatioEff_Pt.png");	

	TCanvas* c3 = new TCanvas("c3", "Canvas with results1", 600, 600);
        c3->cd();

	TLine* line3 = new TLine(0,1,2.4,1);
        line3->SetLineStyle(kDashed);

        hEffRapDouble->SetMarkerSize(2.0);
        hEffRapDouble->SetMarkerColor(kRed);
        hEffRapDouble->SetMarkerStyle(21);
	hEffRapDouble->SetLineColor(kRed);
        hEffRapDouble->SetTitle("");
        hEffRapDouble->GetXaxis()->SetTitle("|y|");
        hEffRapDouble->GetXaxis()->CenterTitle();
	hEffRapDouble->GetYaxis()->CenterTitle();
	hEffRapDouble->GetYaxis()->SetTitle("Efficiency[#varUpsilon(2S)]/Efficieny[#varUpsilon(1S)]_{PbPb}/Efficiency[#varUpsilon(2S)]/Efficiency[#varUpsilon(1S)]_{pp}");
	hEffRapDouble->GetYaxis()->SetRangeUser(0.5, 1.5);
	hEffRapDouble->GetXaxis()->SetRangeUser(0.0, 2.4);
	hEffRapDouble->Draw("AP");
	line3->Draw("");

        c3->SaveAs("DoubleRatioEff_Rap.png");	

	TCanvas* c4 = new TCanvas("c4", "Canvas with results1", 600, 600);
        c4->cd();

	TLine* line4 = new TLine(0,1,2.4,1);
        line4->SetLineStyle(kDashed);

        hEffIntDouble->SetMarkerSize(2.0);
        hEffIntDouble->SetMarkerColor(kRed);
        hEffIntDouble->SetMarkerStyle(21);
	hEffIntDouble->SetLineColor(kRed);
        hEffIntDouble->SetTitle("");
        hEffIntDouble->GetXaxis()->SetTitle("Integrated");
        hEffIntDouble->GetXaxis()->CenterTitle();
	hEffIntDouble->GetYaxis()->CenterTitle();
	hEffIntDouble->GetYaxis()->SetTitle("Efficiency[#varUpsilon(2S)]/Efficieny[#varUpsilon(1S)]_{PbPb}/Efficiency[#varUpsilon(2S)]/Efficiency[#varUpsilon(1S)]_{pp}");
	hEffIntDouble->GetYaxis()->SetRangeUser(0.5, 1.5);
	hEffIntDouble->GetXaxis()->SetRangeUser(0.0, 160);
	hEffIntDouble->Draw("AP");
	line4->Draw("");

        c4->SaveAs("DoubleRatioEff_Int.png");	



        cout <<"doing Centrality"<<endl;
        for (Int_t i = 0; i < (nCenBin); i++){
        cout << hEffCenDouble->Eval(CenBin[i]) << " , - " << hEffCenDouble->GetErrorYlow(i) << " , + " << hEffCenDouble->GetErrorYhigh(i) << endl;
        }
        cout <<"doing Int"<<endl;
        for (Int_t i = 0; i < (1); i++){
        cout << hEffIntDouble->Eval(intBin[i]) << " , - " << hEffIntDouble->GetErrorYlow(i) << " , + " << hEffIntDouble->GetErrorYhigh(i) << endl;
        }
        cout <<"doing PT"<<endl;
        for (Int_t i = 0; i < (nPtBin); i++){
        cout << hEffPtDouble->Eval(ptBin[i]) << " , - " << hEffPtDouble->GetErrorYlow(i) << " , + " << hEffPtDouble->GetErrorYhigh(i) << endl;
        }
        cout <<"doing Rap"<<endl;
        for (Int_t i = 0; i < (nRapBin); i++){
        cout << hEffRapDouble->Eval(rapBin[i]) << " , - " << hEffRapDouble->GetErrorYlow(i) << " , + " << hEffRapDouble->GetErrorYhigh(i) << endl;
        }

        cout << "over" << endl;
	
	fEffSinglePbPb->Close(); 
	fEffSinglePP->Close(); 





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


