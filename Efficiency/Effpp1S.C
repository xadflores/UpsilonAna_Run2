//Making code a little cleaner
#include "effCommon.h"


//Macro to get Efficiency of 1S vs. pt in pp. 
//run as follows:
//> root -l
//> .L trigEff.C+
//> trigEff()
//
//Too see all the branches and declaration for a root file of this type
//OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_ptUps03_noCUT.root
//Do the following:
//root -l rootFileName.root
//hionia->cd() //for pp files
//myTree->MakeClass("myClass")
//.q
//look in myClass.h and myClass.C to learn
//


bool IsAccept(TLorentzVector *Muon);
double FindCenWeight(int Bin);
double RError(double A, double eA, double B, double eB);
double PError(double A, double eA, double B, double eB);
bool PtCut(TLorentzVector* Muon);
bool MassCut(TLorentzVector* DiMuon, double LowM, double HighM);
double PtReweight(TLorentzVector* DiMuon, double coefficient, double constant);


const int  nPtBin = 3;
const double muonPtCut = 4.0;
double m1S_low = 7.77;
double m1S_high = 10;
double m2S_low = 8.333;
double m2S_high = 10.563;
double pp1S_coefficient = -0.022;
double pp1S_constant = 1.09;


void Effpp1S(){

	gStyle->SetOptStat(0);
        gROOT->Macro("logon.C+");
	TChain myTree("hionia/myTree");
	myTree.Add("/scratch_menkar/CMS_Trees/OniaTrees_2015_5TeV/pp_MC_Official/OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");


	Float_t         muMiDxy;
	Float_t         muMiDz;
	Int_t           muMiNPxlLayers;
	Int_t           muMiNTrkLayers;
	Bool_t          muMiGoodMu;
	Float_t         muPlDxy;
	Float_t         muPlDz;
	Int_t           muPlNPxlLayers;
	Int_t           muPlNTrkLayers;
	Bool_t          muPlGoodMu;
	Float_t         vProb;

	double          ptBin[nPtBin] = {2.5,8.5,21};   //  RapBin
        double          ptBinErr[nPtBin] = {2.5,3.5,9}; 
        Float_t         ptBinEdges[nPtBin+1] = {0.0,5.0,12.0,30.0};
        double          ptReweight=0;

	Int_t           Centrality; 
	ULong64_t       HLTriggers;
	Int_t           Reco_QQ_size;
	Int_t           Reco_QQ_sign[36];   //[Reco_QQ_size]
	TClonesArray    *Reco_QQ_4mom;
	TClonesArray    *Reco_QQ_mupl_4mom;
	TClonesArray    *Reco_QQ_mumi_4mom;
	ULong64_t       Reco_QQ_trig[36];   //[Reco_QQ_size]
	Float_t         Reco_QQ_VtxProb[36];   //[Reco_QQ_size]
	Bool_t          Reco_QQ_mupl_isGoodMuon[36];   //[Reco_QQ_size]
	Bool_t          Reco_QQ_mumi_isGoodMuon[36];   //[Reco_QQ_size]
	Int_t           Reco_QQ_mupl_nPixWMea[36];   //[Reco_QQ_size]
	Int_t           Reco_QQ_mumi_nPixWMea[36];   //[Reco_QQ_size]
	Int_t           Reco_QQ_mupl_nTrkWMea[36];   //[Reco_QQ_size]
	Int_t           Reco_QQ_mumi_nTrkWMea[36];   //[Reco_QQ_size]
	Float_t         Reco_QQ_mupl_dxy[36];   //[Reco_QQ_size]
	Float_t         Reco_QQ_mumi_dxy[36];   //[Reco_QQ_size]
	Float_t         Reco_QQ_mupl_dz[36];   //[Reco_QQ_size]
	Float_t         Reco_QQ_mumi_dz[36];   //[Reco_QQ_size]



	Int_t           Gen_QQ_size;
	Int_t           Gen_QQ_sign[36];   //[Gen_QQ_size]
	TClonesArray    *Gen_QQ_4mom;
	TClonesArray    *Gen_QQ_mupl_4mom;
	TClonesArray    *Gen_QQ_mumi_4mom;
	Float_t         Gen_QQ_VtxProb[36];   //[Gen_QQ_size]
	Bool_t          Gen_QQ_mupl_isGoodMuon[36];   //[Gen_QQ_size]
	Bool_t          Gen_QQ_mumi_isGoodMuon[36];   //[Gen_QQ_size]
	Int_t           Gen_QQ_mupl_nPixWMea[36];   //[Gen_QQ_size]
	Int_t           Gen_QQ_mumi_nPixWMea[36];   //[Gen_QQ_size]
	Int_t           Gen_QQ_mupl_nTrkWMea[36];   //[Gen_QQ_size]
	Int_t           Gen_QQ_mumi_nTrkWMea[36];   //[Gen_QQ_size]
	Float_t         Gen_QQ_mupl_dxy[36];   //[Gen_QQ_size]
	Float_t         Gen_QQ_mumi_dxy[36];   //[Gen_QQ_size]
	Float_t         Gen_QQ_mupl_dz[36];   //[Gen_QQ_size]
	Float_t         Gen_QQ_mumi_dz[36];   //[Gen_QQ_size]





	TBranch        *b_Centrality;   //!
	TBranch        *b_HLTriggers;   //!
	TBranch        *b_Reco_QQ_size;   //!
	TBranch        *b_Reco_QQ_sign;   //!
	TBranch        *b_Reco_QQ_4mom;   //!
	TBranch        *b_Reco_QQ_mupl_4mom;   //!
	TBranch        *b_Reco_QQ_mumi_4mom;   //!
	TBranch        *b_Reco_QQ_trig;   //!
	TBranch        *b_Reco_QQ_VtxProb;   //!
	TBranch        *b_Reco_QQ_mupl_isGoodMuon;   //!
	TBranch        *b_Reco_QQ_mumi_isGoodMuon;   //!
	TBranch        *b_Reco_QQ_mupl_nPixWMea;   //!
	TBranch        *b_Reco_QQ_mumi_nPixWMea;   //!
	TBranch        *b_Reco_QQ_mupl_nTrkWMea;   //!
	TBranch        *b_Reco_QQ_mumi_nTrkWMea;   //!
	TBranch        *b_Reco_QQ_mupl_dxy;   //!
	TBranch        *b_Reco_QQ_mumi_dxy;   //!
	TBranch        *b_Reco_QQ_mupl_dz;   //!
	TBranch        *b_Reco_QQ_mumi_dz;   //!


	TBranch        *b_Gen_QQ_size;   //!
//	TBranch        *b_Gen_QQ_sign;   //!
	TBranch        *b_Gen_QQ_4mom;   //!
	TBranch        *b_Gen_QQ_mupl_4mom;   //!
	TBranch        *b_Gen_QQ_mumi_4mom;   //!

	//Set object pointer, Initialize
	Reco_QQ_4mom = 0;
	Reco_QQ_mupl_4mom = 0;
	Reco_QQ_mumi_4mom = 0;	
	
	Gen_QQ_4mom = 0;
	Gen_QQ_mupl_4mom = 0;
	Gen_QQ_mumi_4mom = 0;	


	myTree.SetBranchAddress("Centrality", &Centrality, &b_Centrality);
	myTree.SetBranchAddress("HLTriggers", &HLTriggers, &b_HLTriggers);
	myTree.SetBranchAddress("Reco_QQ_size", &Reco_QQ_size, &b_Reco_QQ_size);
	myTree.SetBranchAddress("Reco_QQ_sign", Reco_QQ_sign, &b_Reco_QQ_sign);
	myTree.SetBranchAddress("Reco_QQ_4mom", &Reco_QQ_4mom, &b_Reco_QQ_4mom);
	myTree.SetBranchAddress("Reco_QQ_mupl_4mom", &Reco_QQ_mupl_4mom, &b_Reco_QQ_mupl_4mom);
	myTree.SetBranchAddress("Reco_QQ_mumi_4mom", &Reco_QQ_mumi_4mom, &b_Reco_QQ_mumi_4mom);
	myTree.SetBranchAddress("Reco_QQ_trig", Reco_QQ_trig, &b_Reco_QQ_trig);
	myTree.SetBranchAddress("Reco_QQ_VtxProb", Reco_QQ_VtxProb, &b_Reco_QQ_VtxProb);
	myTree.SetBranchAddress("Reco_QQ_mupl_isGoodMuon", Reco_QQ_mupl_isGoodMuon, &b_Reco_QQ_mupl_isGoodMuon);
	myTree.SetBranchAddress("Reco_QQ_mumi_isGoodMuon", Reco_QQ_mumi_isGoodMuon, &b_Reco_QQ_mumi_isGoodMuon);
	myTree.SetBranchAddress("Reco_QQ_mupl_nPixWMea", Reco_QQ_mupl_nPixWMea, &b_Reco_QQ_mupl_nPixWMea);
	myTree.SetBranchAddress("Reco_QQ_mumi_nPixWMea", Reco_QQ_mumi_nPixWMea, &b_Reco_QQ_mumi_nPixWMea);
	myTree.SetBranchAddress("Reco_QQ_mupl_nTrkWMea", Reco_QQ_mupl_nTrkWMea, &b_Reco_QQ_mupl_nTrkWMea);
	myTree.SetBranchAddress("Reco_QQ_mumi_nTrkWMea", Reco_QQ_mumi_nTrkWMea, &b_Reco_QQ_mumi_nTrkWMea);
	myTree.SetBranchAddress("Reco_QQ_mupl_dxy", Reco_QQ_mupl_dxy, &b_Reco_QQ_mupl_dxy);
	myTree.SetBranchAddress("Reco_QQ_mumi_dxy", Reco_QQ_mumi_dxy, &b_Reco_QQ_mumi_dxy);
	myTree.SetBranchAddress("Reco_QQ_mupl_dz", Reco_QQ_mupl_dz, &b_Reco_QQ_mupl_dz);
	myTree.SetBranchAddress("Reco_QQ_mumi_dz", Reco_QQ_mumi_dz, &b_Reco_QQ_mumi_dz);



	myTree.SetBranchAddress("Gen_QQ_size", &Gen_QQ_size, &b_Gen_QQ_size);
//	myTree.SetBranchAddress("Gen_QQ_sign", Gen_QQ_sign, &b_Gen_QQ_sign);
	myTree.SetBranchAddress("Gen_QQ_4mom", &Gen_QQ_4mom, &b_Gen_QQ_4mom);
	myTree.SetBranchAddress("Gen_QQ_mupl_4mom", &Gen_QQ_mupl_4mom, &b_Gen_QQ_mupl_4mom);
	myTree.SetBranchAddress("Gen_QQ_mumi_4mom", &Gen_QQ_mumi_4mom, &b_Gen_QQ_mumi_4mom);



	TH1D  *hMassL1=new TH1D("hMassL1","Reconstructed", nPtBin, ptBinEdges);

	TH1D  *hgenMassL1=new TH1D("hgenMassL1","Generated", nPtBin, ptBinEdges);



	hMassL1->Sumw2();
        hgenMassL1->Sumw2();

	TH1D* hEff = new TH1D("Eff", "", nPtBin, ptBinEdges);





	Long64_t nentries = myTree.GetEntries();
	cout << nentries<<endl;

	for (Long64_t jentry=0; jentry<nentries; jentry++){
		myTree.GetEntry(jentry);

		//looping over all the dimuons 
		for (int iQQ=0; iQQ<Reco_QQ_size;iQQ++){
			TLorentzVector *qq4mom = (TLorentzVector*) Reco_QQ_4mom->At(iQQ);
			TLorentzVector *mumi4mom = (TLorentzVector*) Reco_QQ_mumi_4mom->At(iQQ);
			TLorentzVector *mupl4mom = (TLorentzVector*) Reco_QQ_mupl_4mom->At(iQQ);

			//--Muid cuts for muon minus
			muMiDxy=Reco_QQ_mumi_dxy[iQQ];
			muMiDz=Reco_QQ_mumi_dz[iQQ];
			muMiNPxlLayers=Reco_QQ_mumi_nPixWMea[iQQ];
			muMiNTrkLayers=Reco_QQ_mumi_nTrkWMea[iQQ];
			muMiGoodMu = Reco_QQ_mumi_isGoodMuon[iQQ];

			//--Muid cuts for muon plus
			muPlDxy=Reco_QQ_mupl_dxy[iQQ];
			muPlDz=Reco_QQ_mupl_dz[iQQ];
			muPlNPxlLayers=Reco_QQ_mupl_nPixWMea[iQQ];
			muPlNTrkLayers=Reco_QQ_mupl_nTrkWMea[iQQ];
			muPlGoodMu = Reco_QQ_mupl_isGoodMuon[iQQ];
			vProb = Reco_QQ_VtxProb[iQQ];

			bool mupl_cut = 0;
			bool mumi_cut = 0;
			bool acceptMu = 0;
			bool trigL1Dmu = 0;
//			bool trigL3Dmu = 0;
			bool PtCutPass = 0;
			bool MassCutPass = 0;

			//--Muon id cuts
			if( (muPlGoodMu==1) && muPlNTrkLayers> 5 &&  muPlNPxlLayers > 0 && TMath::Abs(muPlDxy) < 0.3 && TMath::Abs(muPlDz) < 20 && vProb > 0.01){mupl_cut = 1;}    
			if( (muMiGoodMu==1) && muMiNTrkLayers> 5 &&  muMiNPxlLayers > 0 && TMath::Abs(muMiDxy) < 0.3 && TMath::Abs(muMiDz) < 20 ){mumi_cut = 1;}

			//check if muons are in acceptance
			if(IsAccept(mupl4mom) && IsAccept(mumi4mom)){acceptMu = 1;}
			if (PtCut(mupl4mom) && PtCut(mumi4mom)){ PtCutPass = 1; }
			MassCutPass = MassCut(qq4mom, m1S_low, m1S_high);

			//check if trigger bit is matched to dimuon
			if((HLTriggers&1)==1 && (Reco_QQ_trig[iQQ]&1)==1){trigL1Dmu = 1;}
//			if((HLTriggers&262144)==262144 && (Reco_QQ_trig[iQQ]&262144)==262144){trigL3Dmu = 1;}

			//weights only needed for PbPb
			 double weight = 0;
			// ptWeight=0;
			// centWeight = FindCenWeight(Centrality);
			// if(qq4mom->Pt()<=3){ptWeight = ptWeightArr[0];}
			// if(qq4mom->Pt()>3){ptWeight = ptWeightArr[1];}
			// weight = centWeight*ptWeight;
                         if(qq4mom->Pt()<=30){ptReweight = PtReweight(qq4mom, pp1S_coefficient, pp1S_constant);}
                         weight = ptReweight;

			bool L1Pass=0;
//			bool L3Pass=0;
			if (Reco_QQ_sign[iQQ]==0 && acceptMu && mupl_cut && mumi_cut && trigL1Dmu){L1Pass=1;}
//			if (Reco_QQ_sign[iQQ]==0 && acceptMu && mupl_cut && mumi_cut && trigL3Dmu){L3Pass=1;}

			if(TMath::Abs(qq4mom->Rapidity())<2.4){
			for(int i = 0; i<nPtBin;i++){
				if(qq4mom->Pt()>(ptBin[i]-ptBinErr[i]) && qq4mom->Pt()<(ptBin[i]+ptBinErr[i])){
					if(L1Pass == 1 && PtCutPass ==1 && MassCutPass == 1){hMassL1->Fill(qq4mom->Pt(),weight);}
				}
			}    // */
			// if(L1Pass == 1 && PtCutPass ==1 && MassCutPass == 1){hMassL1->Fill(qq4mom->Pt(),weight);}
			}
		}

		//Dinonminator loop
		for (int iQQ=0; iQQ<Gen_QQ_size;iQQ++){
			TLorentzVector *qq4mom = (TLorentzVector*) Gen_QQ_4mom->At(iQQ);
			TLorentzVector *mumi4mom = (TLorentzVector*) Gen_QQ_mumi_4mom->At(iQQ);
			TLorentzVector *mupl4mom = (TLorentzVector*) Gen_QQ_mupl_4mom->At(iQQ);


			bool mupl_cut = 0;
			bool mumi_cut = 0;
			bool acceptMu = 0;
			bool trigL1Dmu = 0;
//			bool trigL3Dmu = 0;
			bool PtCutPass = 0;
			bool MassCutPass = 0;

			//--Muon id cuts
			if( (muPlGoodMu==1) && muPlNTrkLayers> 5 &&  muPlNPxlLayers > 0 && TMath::Abs(muPlDxy) < 0.3 && TMath::Abs(muPlDz) < 20 && vProb > 0.01){mupl_cut = 1;}
			if( (muMiGoodMu==1) && muMiNTrkLayers> 5 &&  muMiNPxlLayers > 0 && TMath::Abs(muMiDxy) < 0.3 && TMath::Abs(muMiDz) < 20 ){mumi_cut = 1;}

			//check if muons are in acceptance
			if(IsAccept(mupl4mom) && IsAccept(mumi4mom)){acceptMu = 1;}
			if (PtCut(mupl4mom) && PtCut(mumi4mom)){ PtCutPass = 1; }
			MassCutPass = MassCut(qq4mom, m1S_low, m1S_high);


			//weights only needed for PbPb
			 double weight = 0;
			// ptWeight=0;
			// centWeight = FindCenWeight(Centrality);
			// if(qq4mom->Pt()<=3){ptWeight = ptWeightArr[0];}
			// if(qq4mom->Pt()>3){ptWeight = ptWeightArr[1];}
			// weight = centWeight*ptWeight;
                         if(qq4mom->Pt()<=30){ptReweight = PtReweight(qq4mom, pp1S_coefficient, pp1S_constant);}
                         weight = ptReweight;
 

			if(TMath::Abs(qq4mom->Rapidity())<2.4){
			for(int i = 0; i<nPtBin;i++){
				if(qq4mom->Pt()>(ptBin[i]-ptBinErr[i]) && qq4mom->Pt()<(ptBin[i]+ptBinErr[i])){
					if(acceptMu == 1 && PtCutPass == 1 && MassCutPass == 1){hgenMassL1->Fill(qq4mom->Pt(), weight);}
				}
			}    //*/
			//  if(acceptMu == 1 && PtCutPass == 1 && MassCutPass == 1){hgenMassL1->Fill(qq4mom->Pt(),weight);}
			}
		}


	}


TCanvas *c1 = new TCanvas("c1","c1",600,600);


//From Ota

         hEff->Divide(hMassL1, hgenMassL1);
//	 hEff->Draw();



   ///////////
 TFile* MyFileEff;
//  if (Switch_1S2S == 1){
//          MyFileEff = new TFile("PbPbEff1S.root", "Recreate");
//  }
//  if (Switch_1S2S == 2){
//          MyFileEff = new TFile("PbPbEff2S.root", "Recreate");
//  }
//  if (Switch_1S2S == 3){
          MyFileEff = new TFile("ppEff1S.root", "Recreate");
//  }
//  if (Switch_1S2S == 4){
//          MyFileEff = new TFile("ppEff2S.root", "Recreate");
//  }
  hMassL1->Write();
  hgenMassL1->Write();
  hEff->Write();


  MyFileEff->Close();

//////////// */ //



//	TCanvas *c1 = new TCanvas("c1","c1",600,400);
	// Will use TGraphAsymmErrors
	TGraphAsymmErrors *TrigEff = new TGraphAsymmErrors(hEff);
//	TGraphAsymmErrors *TrigEff = new TGraphAsymmErrors(nPtBin);
//	TrigEff->Divide(hMassL1, hgenMassL1, "cl=0.683 b(1,1) mode");

	TrigEff->SetMarkerSize(1.0);
        TrigEff->SetMarkerColor(kBlue);
        TrigEff->SetMarkerStyle(20);
        TrigEff->SetLineColor(kBlue);

	TrigEff->SetTitle("");
//	TrigEff->SetMarkerStyle(21);
//	TrigEff->SetMarkerColor(2);
	TrigEff->GetYaxis()->SetTitle("Efficiency[#varUpsilon(1S)]_{pp}");
	TrigEff->GetYaxis()->SetTitleOffset(1.5);
	TrigEff->GetYaxis()->SetRangeUser(0,1);
	TrigEff->GetXaxis()->SetTitle("p^{#mu+#mu-}_{T}");
	TrigEff->GetXaxis()->SetTitleOffset(1.5);
	TrigEff->GetXaxis()->CenterTitle();
	TrigEff->GetXaxis()->SetRangeUser(0,30);

	TrigEff->Draw("AP");	// */
	c1->Update();
	c1->SaveAs("EfficiencyVsPtUpsilonpp1S.png");

}




//Returns a boolean for muon in acceptance
bool IsAccept(TLorentzVector *Muon){
	return (
			(( fabs(Muon->Eta())>=0.0 && fabs(Muon->Eta())<1.0 ) && Muon->Pt()>3.4) ||
			(( fabs(Muon->Eta())>=1.0 && fabs(Muon->Eta())<1.5 ) && Muon->Pt()>(5.8-2.4*fabs(Muon->Eta())) ) ||
			(( fabs(Muon->Eta())>=1.5 && fabs(Muon->Eta())<2.4 ) && Muon->Pt()>(3.4-0.78*fabs(Muon->Eta())) )
	       );
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



bool PtCut(TLorentzVector* Muon){
        if (Muon->Pt() < muonPtCut){ return false; }
        else return true;
}



bool MassCut(TLorentzVector* DiMuon, double LowM, double HighM){
        if (DiMuon->M() < LowM){ return false; }
        if (DiMuon->M() > HighM){ return false; }
        return true;
}

double PtReweight(TLorentzVector* DiMuon, double coefficient, double constant){
        double f = coefficient*(DiMuon->Pt()) + constant;
        return f;
}




//this re weights centrality dist. 
double FindCenWeight(int Bin) {
	const int nbins = 200;
	const double Ncoll[nbins] = {1976.95, 1944.02, 1927.29, 1891.9, 1845.3, 1807.2, 1760.45, 1729.18, 1674.8, 1630.3, 1590.52, 1561.72, 1516.1, 1486.5, 1444.68, 1410.88, 1376.4, 1347.32, 1309.71, 1279.98, 1255.31, 1219.89, 1195.13, 1165.96, 1138.92, 1113.37, 1082.26, 1062.42, 1030.6, 1009.96, 980.229, 955.443, 936.501, 915.97, 892.063, 871.289, 847.364, 825.127, 806.584, 789.163, 765.42, 751.187, 733.001, 708.31, 690.972, 677.711, 660.682, 640.431, 623.839, 607.456, 593.307, 576.364, 560.967, 548.909, 530.475, 519.575, 505.105, 490.027, 478.133, 462.372, 451.115, 442.642, 425.76, 416.364, 405.154, 392.688, 380.565, 371.167, 360.28, 348.239, 340.587, 328.746, 320.268, 311.752, 300.742, 292.172, 281.361, 274.249, 267.025, 258.625, 249.931, 240.497, 235.423, 228.63, 219.854, 214.004, 205.425, 199.114, 193.618, 185.644, 180.923, 174.289, 169.641, 161.016, 157.398, 152.151, 147.425, 140.933, 135.924, 132.365, 127.017, 122.127, 117.817, 113.076, 109.055, 105.16, 101.323, 98.098, 95.0548, 90.729, 87.6495, 84.0899, 80.2237, 77.2201, 74.8848, 71.3554, 68.7745, 65.9911, 63.4136, 61.3859, 58.1903, 56.4155, 53.8486, 52.0196, 49.2921, 47.0735, 45.4345, 43.8434, 41.7181, 39.8988, 38.2262, 36.4435, 34.8984, 33.4664, 31.8056, 30.351, 29.2074, 27.6924, 26.7754, 25.4965, 24.2802, 22.9651, 22.0059, 21.0915, 19.9129, 19.1041, 18.1487, 17.3218, 16.5957, 15.5323, 14.8035, 14.2514, 13.3782, 12.8667, 12.2891, 11.61, 11.0026, 10.3747, 9.90294, 9.42648, 8.85324, 8.50121, 7.89834, 7.65197, 7.22768, 6.7755, 6.34855, 5.98336, 5.76555, 5.38056, 5.11024, 4.7748, 4.59117, 4.23247, 4.00814, 3.79607, 3.68702, 3.3767, 3.16309, 2.98282, 2.8095, 2.65875, 2.50561, 2.32516, 2.16357, 2.03235, 1.84061, 1.72628, 1.62305, 1.48916, 1.38784, 1.28366, 1.24693, 1.18552, 1.16085, 1.12596, 1.09298, 1.07402, 1.06105, 1.02954};
	return Ncoll[Bin];
}
