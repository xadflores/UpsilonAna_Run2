#include "effCommon.h"


double GetWeight(int numTree,int oniaMode);
bool IsAccept(TLorentzVector *Muon);
double FindCenWeight(int Bin);
double RError(double A, double eA, double B, double eB);
double PError(double A, double eA, double B, double eB);
bool PtCut(TLorentzVector* Muon);
bool MassCut(TLorentzVector* DiMuon, double LowM, double HighM);
double PtReweight(TLorentzVector* DiMuon, double coefficient, double constant);



const int  nPtBin = 3;     
const int  nRapBin = 2;     
const int nCenBin = 9;
const double muonPtCut = 4.0;

double m1S_low = 8.0;
double m1S_high = 10.0;
double m2S_low = 8.563;
double m2S_high = 10.563;


void dimuEff(
	int oniaMode = 2, //1 = 1S and 2 = 2S
	bool isPbPb = true //true = PbPb and false = pp
	){      // Change function name


	gROOT->Macro("logon.C+");

	TChain myTree("hionia/myTree");   // Change source of tree


	if (oniaMode == 2 && !isPbPb){
		myTree.Add("files/OniaTree_Ups2SMM_5p02TeV_TuneCUETP8M1_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");
	}

	if (oniaMode == 1 && !isPbPb){
		myTree.Add("files/OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");
	}


	if (oniaMode == 2 && isPbPb){
		myTree.Add("files/OniaTree_Pythia8_Ups2SMM_ptUps2S_00_03_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");   //  Change tree being added (Different for pp1S, pp2S, PbPb1S, PbPb2S)
		myTree.Add("files/OniaTree_Pythia8_Ups2SMM_ptUps2S_03_06_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
		myTree.Add("files/OniaTree_Pythia8_Ups2SMM_ptUps2S_06_09_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
		myTree.Add("files/OniaTree_Pythia8_Ups2SMM_ptUps2S_09_12_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
		myTree.Add("files/OniaTree_Pythia8_Ups2SMM_ptUps2S_12_15_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
		myTree.Add("files/OniaTree_Pythia8_Ups2SMM_ptUps2S_15_inf_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
	}

	if (oniaMode == 1 && isPbPb){
		myTree.Add("files/OniaTree_Pythia8_Ups1SMM_ptUps_00_03_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");   //  Change tree being added (Different for pp1S, pp2S, PbPb1S, PbPb2S)
		myTree.Add("files/OniaTree_Pythia8_Ups1SMM_ptUps_03_06_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
		myTree.Add("files/OniaTree_Pythia8_Ups1SMM_ptUps_06_09_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
		myTree.Add("files/OniaTree_Pythia8_Ups1SMM_ptUps_09_12_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
		myTree.Add("files/OniaTree_Pythia8_Ups1SMM_ptUps_12_15_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
		myTree.Add("files/OniaTree_Pythia8_Ups1SMM_ptUps_15_30_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root");
	}




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

	float          ptWeight;
	float          centWeight;
	float          ptBin[nPtBin] = { 2.5, 8.5, 21 };
	float          ptBinEdges[nPtBin + 1] = { 0, 5, 12, 30 };
	float          rapBin[nPtBin] = { 0.6, 1.8 };
	float          rapBinEdges[nPtBin + 1] = { 0, 1.2, 2.4 };

	float           CenBin[nCenBin] = { 5, 15, 30, 50, 70, 90, 110, 130, 150 };
	float    	CenBinEdges[nCenBin + 1] = { 0, 10, 20, 40, 60, 80, 100, 120, 140, 160 };

	float           IntBin[1] = { 160 };
	float		IntBinEdges[2] = { 0, 160 };
	float          ptReweight = 0.0;

	float 		massLow = 0;
	float 		massHigh = 0;

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


	TBranch        *b_Gen_QQ_size;   //
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
	myTree.SetBranchAddress("Gen_QQ_4mom", &Gen_QQ_4mom, &b_Gen_QQ_4mom);
	myTree.SetBranchAddress("Gen_QQ_mupl_4mom", &Gen_QQ_mupl_4mom, &b_Gen_QQ_mupl_4mom);
	myTree.SetBranchAddress("Gen_QQ_mumi_4mom", &Gen_QQ_mumi_4mom, &b_Gen_QQ_mumi_4mom);

	myTree.SetBranchStatus("*", 0);


	myTree.SetBranchStatus("Centrality", 1);
	myTree.SetBranchStatus("HLTriggers", 1);
	myTree.SetBranchStatus("Reco_QQ_size", 1);
	myTree.SetBranchStatus("Reco_QQ_sign", 1);
	myTree.SetBranchStatus("Reco_QQ_4mom", 1);
	myTree.SetBranchStatus("Reco_QQ_mupl_4mom", 1);
	myTree.SetBranchStatus("Reco_QQ_mumi_4mom", 1);
	myTree.SetBranchStatus("Reco_QQ_trig", 1);
	myTree.SetBranchStatus("Reco_QQ_VtxProb", 1);
	myTree.SetBranchStatus("Reco_QQ_mupl_isGoodMuon", 1);
	myTree.SetBranchStatus("Reco_QQ_mumi_isGoodMuon", 1);
	myTree.SetBranchStatus("Reco_QQ_mupl_nPixWMea", 1);
	myTree.SetBranchStatus("Reco_QQ_mumi_nPixWMea", 1);
	myTree.SetBranchStatus("Reco_QQ_mupl_nTrkWMea", 1);
	myTree.SetBranchStatus("Reco_QQ_mumi_nTrkWMea", 1);
	myTree.SetBranchStatus("Reco_QQ_mupl_dxy", 1);
	myTree.SetBranchStatus("Reco_QQ_mumi_dxy", 1);
	myTree.SetBranchStatus("Reco_QQ_mupl_dz", 1);
	myTree.SetBranchStatus("Reco_QQ_mumi_dz", 1);



	myTree.SetBranchStatus("Gen_QQ_size", 1);
	myTree.SetBranchStatus("Gen_QQ_4mom", 1);
	myTree.SetBranchStatus("Gen_QQ_mupl_4mom", 1);
	myTree.SetBranchStatus("Gen_QQ_mumi_4mom", 1);



	TH1D  *RecoEvents = new TH1D("RecoEvents", "Reconstructed", isPbPb ? nCenBin : 1, isPbPb ? CenBinEdges : IntBinEdges);
	TH1D  *GenEvents = new TH1D("GenEvents", "Generated", isPbPb ? nCenBin : 1, isPbPb ? CenBinEdges : IntBinEdges);

	TH1D  *RecoEventsInt = new TH1D("RecoEventsInt", "Reconstructed", 1, IntBinEdges);
	TH1D  *GenEventsInt = new TH1D("GenEventsInt", "Generated", 1, IntBinEdges);

	TH1D  *RecoEventsPt = new TH1D("RecoEventsPt", "Reconstructed", nPtBin, ptBinEdges);
	TH1D  *GenEventsPt = new TH1D("GenEventsPt", "Generated", nPtBin, ptBinEdges);

	TH1D  *RecoEventsRap = new TH1D("RecoEventsRap", "Reconstructed", nRapBin, rapBinEdges);
	TH1D  *GenEventsRap = new TH1D("GenEventsRap", "Generated", nRapBin, rapBinEdges);

	TH1D  *hCentrality = new TH1D("hCentrality", "Centrality distribution", 202, -1, 201);
	TH1D  *hCrossCheck = new TH1D("hCrossCheck", "Checking number of events", 2, 0, 2);

	TH1D  *hRecoEventsD = new TH1D("hRecoEventsD", "Reconstructed", isPbPb ? nCenBin : 1, isPbPb ? CenBinEdges : IntBinEdges);
	TH1D  *hGenEventsD = new TH1D("hGenEventsD", "Generated", isPbPb ? nCenBin : 1, isPbPb ? CenBinEdges : IntBinEdges);

	RecoEvents->Sumw2();
	GenEvents->Sumw2();
	RecoEventsInt->Sumw2();
	GenEventsInt->Sumw2();
	RecoEventsPt->Sumw2();
	GenEventsPt->Sumw2();
	RecoEventsRap->Sumw2();
	GenEventsRap->Sumw2();

	//weights for 1S are used for 2S as well
	TF1* f1SAA;
	TF1* f2SAA;
	TF1* f1Spp;
	TF1* f2Spp;
	TFile* ReweightFunctions = new TFile("dNdpT_ratio_tsallis_June7.root", "Open");

	f1SAA = (TF1*)ReweightFunctions->Get("f1sraa_test");
	f2SAA = (TF1*)ReweightFunctions->Get("f2sraa_test");
	f1Spp = (TF1*)ReweightFunctions->Get("f1srpp_test");
	f2Spp = (TF1*)ReweightFunctions->Get("f2srpp_test");

	//f1SAA->SetNpx(100000);
	//f2SAA->SetNpx(100000);
	//f1SAA->SetRange(0,40);
	//f2SAA->SetRange(0,40);



	if (oniaMode == 1){
		massLow = m1S_low;
		massHigh = m1S_high;
	}
	else{
		massLow = m2S_low;
		massHigh = m2S_high;
	}

	Long64_t nentries = myTree.GetEntries();
	cout << nentries << endl;

	for (Long64_t jentry = 0; jentry < nentries; jentry++){
		myTree.GetEntry(jentry);

		//Numerator Loop RECO
		for (int iQQ = 0; iQQ < Reco_QQ_size; iQQ++){
			hCrossCheck->Fill(1);
			TLorentzVector *qq4mom = (TLorentzVector*)Reco_QQ_4mom->At(iQQ);
			TLorentzVector *mumi4mom = (TLorentzVector*)Reco_QQ_mumi_4mom->At(iQQ);
			TLorentzVector *mupl4mom = (TLorentzVector*)Reco_QQ_mupl_4mom->At(iQQ);

			//--Muid cuts for muon minus
			muMiDxy = Reco_QQ_mumi_dxy[iQQ];
			muMiDz = Reco_QQ_mumi_dz[iQQ];
			muMiNPxlLayers = Reco_QQ_mumi_nPixWMea[iQQ];
			muMiNTrkLayers = Reco_QQ_mumi_nTrkWMea[iQQ];
			muMiGoodMu = Reco_QQ_mumi_isGoodMuon[iQQ];

			//--Muid cuts for muon plus
			muPlDxy = Reco_QQ_mupl_dxy[iQQ];
			muPlDz = Reco_QQ_mupl_dz[iQQ];
			muPlNPxlLayers = Reco_QQ_mupl_nPixWMea[iQQ];
			muPlNTrkLayers = Reco_QQ_mupl_nTrkWMea[iQQ];
			muPlGoodMu = Reco_QQ_mupl_isGoodMuon[iQQ];
			vProb = Reco_QQ_VtxProb[iQQ];

			bool mupl_cut = 0;
			bool mumi_cut = 0;
			bool acceptMu = 0;
			bool trigL1Dmu = 0;
			bool PtCutPass = 0;
			bool MassCutPass = 0;

			//--Muon id cuts
			if ((muPlGoodMu == 1) && muPlNTrkLayers > 5 && muPlNPxlLayers > 0 && TMath::Abs(muPlDxy) < 0.3 && TMath::Abs(muPlDz) < 20 && vProb > 0.01){ mupl_cut = 1; }
			if ((muMiGoodMu == 1) && muMiNTrkLayers > 5 && muMiNPxlLayers > 0 && TMath::Abs(muMiDxy) < 0.3 && TMath::Abs(muMiDz) < 20){ mumi_cut = 1; }

			//check if muons are in acceptance
			if (IsAccept(mupl4mom) && IsAccept(mumi4mom)){ acceptMu = 1; }
			if (PtCut(mupl4mom) && PtCut(mumi4mom)){ PtCutPass = 1; }
			MassCutPass = MassCut(qq4mom, massLow, massHigh);

			//check if trigger bit is matched to dimuon
			if ((HLTriggers & 1) == 1 && (Reco_QQ_trig[iQQ] & 1) == 1){ trigL1Dmu = 1; }

			//weights only needed for PbPb
			float weight = 0;
			ptWeight = 0;
			centWeight = 0;
			centWeight = FindCenWeight(Centrality);
			ptReweight = 0;

			//getting reco pt
			float ptReco = 0;
			float rapReco = 0;
			ptReco = qq4mom->Pt();
			rapReco = TMath::Abs(qq4mom->Rapidity());

			//getting the tree weight from pt generated MC bins

			//reweight from dn/dpt function
			int tNum = -1;
			//total weighting factor
			if (isPbPb){
				if (oniaMode == 1){ ptReweight = (f1SAA->Eval(ptReco)); }
				if (oniaMode == 2){ ptReweight = (f2SAA->Eval(ptReco)); }
				tNum = myTree.GetTreeNumber();
				//ptReweight = 1;
				ptWeight = GetWeight(tNum, oniaMode);
				weight = centWeight*ptWeight*ptReweight;
			}
			else {
				if (oniaMode == 1){ ptReweight = (f1Spp->Eval(ptReco)); }
				if (oniaMode == 2){ ptReweight = (f2Spp->Eval(ptReco)); }
				//ptReweight = 1;
				weight = ptReweight;
			}

			bool recoPass = 0;

			if (Reco_QQ_sign[iQQ] == 0 && acceptMu && mupl_cut && mumi_cut && trigL1Dmu){ recoPass = 1; }

			//filling RecoEvent Histo if passing
			if (rapReco < 2.4 && ptReco < 30 && Centrality < 160){
				if (recoPass == 1 && PtCutPass == 1 && MassCutPass == 1){
					RecoEvents->Fill(Centrality, weight);
					hRecoEventsD->Fill(Centrality, weight);
					RecoEventsInt->Fill(Centrality, weight);
					RecoEventsPt->Fill(ptReco, weight);
					RecoEventsRap->Fill(rapReco, weight);
					hCentrality->Fill(Centrality, weight);
				}
			}


		}


		//Denominator loop  GEN
		for (int iQQ = 0; iQQ < Gen_QQ_size; iQQ++){

			hCrossCheck->Fill(0);
			TLorentzVector *g_qq4mom = (TLorentzVector*)Gen_QQ_4mom->At(iQQ);
			TLorentzVector *g_mumi4mom = (TLorentzVector*)Gen_QQ_mumi_4mom->At(iQQ);
			TLorentzVector *g_mupl4mom = (TLorentzVector*)Gen_QQ_mupl_4mom->At(iQQ);

			bool acceptMu = 0;
			bool PtCutPass = 0;
			bool MassCutPass = 0;


			//check if muons are in acceptance
			if (IsAccept(g_mupl4mom) && IsAccept(g_mumi4mom)){ acceptMu = 1; }
			if (PtCut(g_mupl4mom) && PtCut(g_mumi4mom)){ PtCutPass = 1; }
			MassCutPass = MassCut(g_qq4mom, massLow, massHigh);



			//weights only needed for PbPb
			float weight = 0;
			ptWeight = 0;
			centWeight = 0;
			ptReweight = 0;
			centWeight = FindCenWeight(Centrality);

			//getting a pt gen value 
			float ptGen = 0;
			float rapGen = 0;
			ptGen = g_qq4mom->Pt();
			rapGen = TMath::Abs(g_qq4mom->Rapidity());



			int tNum = -1;
			//getting the tree pt mc weighting from generation
			if (isPbPb){
				if (oniaMode == 1){ ptReweight = (f1SAA->Eval(ptGen)); }
				if (oniaMode == 2){ ptReweight = (f2SAA->Eval(ptGen)); }
				tNum = myTree.GetTreeNumber();
				//ptReweight = 1;
				ptWeight = GetWeight(tNum, oniaMode);
				weight = centWeight*ptWeight*ptReweight;
			}
			else{
				if (oniaMode == 1){ ptReweight = (f1Spp->Eval(ptGen)); }
				if (oniaMode == 2){ ptReweight = (f2Spp->Eval(ptGen)); }
				//ptReweight = 1;
				weight = ptReweight;
			}

			//fill GenEvent Histo Denominator if passing 
			if (rapGen < 2.4 && ptGen < 30 && Centrality < 160){
				if (acceptMu == 1 && PtCutPass == 1 && MassCutPass == 1){
					GenEvents->Fill(Centrality, weight);
					hGenEventsD->Fill(Centrality, weight);
					GenEventsInt->Fill(Centrality, weight);
					GenEventsPt->Fill(ptGen, weight);
					GenEventsRap->Fill(rapGen, weight);

				}
			}

		}


	}



//------Cent---------       
//dividing the RecoEvents by GenEvents 
TGraphAsymmErrors *EffCent = new TGraphAsymmErrors(nCenBin);
EffCent->BayesDivide(RecoEvents, GenEvents);
EffCent->SetName("EffCent");

if(isPbPb){
TCanvas *c1 = new TCanvas("c1","c1",600,600);
c1->cd();
//EffCent->SetMarkerSize(1.0);
EffCent->SetMarkerColor(kRed);
EffCent->SetMarkerStyle(20);

EffCent->SetTitle("");
EffCent->GetYaxis()->SetTitle(Form("Efficiency[#varUpsilon(%dS)]_{%s}",oniaMode, isPbPb ? "PbPb" : "PP"));
EffCent->GetXaxis()->SetTitle(Form("%s",isPbPb ? "Centrality" : "Integrated Bin"));
EffCent->GetYaxis()->SetRangeUser(0,1);
EffCent->GetXaxis()->SetRangeUser(0.0, 160.0);
EffCent->GetXaxis()->CenterTitle();
EffCent->GetYaxis()->CenterTitle();

EffCent->Draw("AP");

c1->SaveAs(Form("EfficiencyCent_%dS_%s.png",oniaMode,isPbPb ? "PbPb" : "PP"));
}
//----------Pt
/*TCanvas *cpt = new TCanvas("cpt","cpt",600,600);
cpt->cd();
RecoEventsPt->SetMarkerColor(kBlue);
GenEventsPt->SetMarkerColor(kRed);

GenEventsPt->Draw();
RecoEventsPt->Draw("SAME");
*/
TCanvas *c2 = new TCanvas("c2","c2",600,600);
c2->cd();

TGraphAsymmErrors *EffPt = new TGraphAsymmErrors(nPtBin);
EffPt->BayesDivide(RecoEventsPt, GenEventsPt);
EffPt->SetName("EffPt");

//EffPt->SetMarkerSize(1.0);
EffPt->SetMarkerColor(kRed);
EffPt->SetMarkerStyle(20);

EffPt->SetTitle("");
EffPt->GetYaxis()->SetTitle(Form("Efficiency[#varUpsilon(%dS)]_{%s}",oniaMode, isPbPb ? "PbPb" : "PP"));
EffPt->GetXaxis()->SetTitle("p_{T}");
EffPt->GetYaxis()->SetRangeUser(0,1);
EffPt->GetXaxis()->SetRangeUser(0.0, 30.0);
EffPt->GetXaxis()->CenterTitle();
EffPt->GetYaxis()->CenterTitle();

EffPt->Draw("AP");

c2->SaveAs(Form("EfficiencyPt_%dS_%s.png",oniaMode,isPbPb ? "PbPb" : "PP"));



//------------Rap
TCanvas *c3 = new TCanvas("c3","c3",600,600);
c3->cd();

TGraphAsymmErrors *EffRap = new TGraphAsymmErrors(nRapBin);
EffRap->BayesDivide(RecoEventsRap, GenEventsRap);
EffRap->SetName("EffRap");

//EffRap->SetMarkerSize(1.0);
EffRap->SetMarkerColor(kRed);
EffRap->SetMarkerStyle(20);

EffRap->SetTitle("");
EffRap->GetYaxis()->SetTitle(Form("Efficiency[#varUpsilon(%dS)]_{%s}",oniaMode, isPbPb ? "PbPb" : "PP"));
EffRap->GetXaxis()->SetTitle("|y|");
EffRap->GetYaxis()->SetRangeUser(0,1);
EffRap->GetXaxis()->SetRangeUser(0.0,2.4);
EffRap->GetXaxis()->CenterTitle();
EffRap->GetYaxis()->CenterTitle();

EffRap->Draw("AP");

c3->SaveAs(Form("EfficiencyRap_%dS_%s.png",oniaMode,isPbPb ? "PbPb" : "PP"));

//------------Int
TCanvas *c4 = new TCanvas("c4","c4",600,600);
c4->cd();

TGraphAsymmErrors *EffInt = new TGraphAsymmErrors(1);
EffInt->BayesDivide(RecoEventsInt, GenEventsInt);
EffInt->SetName("EffInt");

//EffInt->SetMarkerSize(1.0);
EffInt->SetMarkerColor(kRed);
EffInt->SetMarkerStyle(20);

EffInt->SetTitle("");
EffInt->GetYaxis()->SetTitle(Form("Efficiency[#varUpsilon(%dS)]_{%s}",oniaMode, isPbPb ? "PbPb" : "PP"));
EffInt->GetXaxis()->SetTitle("Integrated bin");
EffInt->GetYaxis()->SetRangeUser(0,1);
EffInt->GetXaxis()->SetRangeUser(0.0,160);
EffInt->GetXaxis()->CenterTitle();
EffInt->GetYaxis()->CenterTitle();

EffInt->Draw("AP");
c4->SaveAs(Form("EfficiencyInt_%dS_%s.png",oniaMode,isPbPb ? "PbPb" : "PP"));

/*
//drawing the number RecoEvents        
TCanvas *c2 = new TCanvas("c2","c1",600,600);
c2->cd();
RecoEvents->GetXaxis()->SetTitle("RECO (numerator)");
RecoEvents->Draw();

//drawing the denominator GenEvents
TCanvas *c3 = new TCanvas("c3","c1",600,600);
c3->cd();
GenEvents->GetXaxis()->SetTitle("GEN (denominator)");
GenEvents->Draw();
*/



TFile* MyFileEff;
MyFileEff = new TFile(Form("Eff_%s_%dS.root",isPbPb ? "PbPb" : "PP",oniaMode), "Recreate");
if(isPbPb){
	EffCent->Write();
	GenEvents->Write();
	RecoEvents->Write();
}

hGenEventsD->Write();
hRecoEventsD->Write();
RecoEventsInt->Write();
RecoEventsPt->Write();
RecoEventsRap->Write();
GenEventsInt->Write();
GenEventsPt->Write();
GenEventsRap->Write();
hCentrality->Write();
hCrossCheck->Write();
EffPt->Write();
EffRap->Write();
EffInt->Write();
MyFileEff->Close();

	if(isPbPb){
        for (Int_t i = 0; i < (nCenBin); i++){
        cout << EffCent->Eval(CenBin[i]) << " , - " << EffCent->GetErrorYlow(i) << " , + " << EffCent->GetErrorYhigh(i) << endl;
        }
	}

        ReweightFunctions->Close();

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


double GetWeight(int numTree,int oniaMode){
  double weight1[6] = {3.10497,4.11498,2.2579,1.2591,0.567094,0.783399};
  double weight2[6] = {5.89168,9.08207,3.106,1.10018,0.534916,0.776183};
  if(oniaMode ==1)return weight1[numTree];
  else return weight2[numTree];
}

//this re weights centrality dist. 
double FindCenWeight(int Bin) {
	const int nbins = 200;
	const double Ncoll[nbins] = {1976.95, 1944.02, 1927.29, 1891.9, 1845.3, 1807.2, 1760.45, 1729.18, 1674.8, 1630.3, 1590.52, 1561.72, 1516.1, 1486.5, 1444.68, 1410.88, 1376.4, 1347.32, 1309.71, 1279.98, 1255.31, 1219.89, 1195.13, 1165.96, 1138.92, 1113.37, 1082.26, 1062.42, 1030.6, 1009.96, 980.229, 955.443, 936.501, 915.97, 892.063, 871.289, 847.364, 825.127, 806.584, 789.163, 765.42, 751.187, 733.001, 708.31, 690.972, 677.711, 660.682, 640.431, 623.839, 607.456, 593.307, 576.364, 560.967, 548.909, 530.475, 519.575, 505.105, 490.027, 478.133, 462.372, 451.115, 442.642, 425.76, 416.364, 405.154, 392.688, 380.565, 371.167, 360.28, 348.239, 340.587, 328.746, 320.268, 311.752, 300.742, 292.172, 281.361, 274.249, 267.025, 258.625, 249.931, 240.497, 235.423, 228.63, 219.854, 214.004, 205.425, 199.114, 193.618, 185.644, 180.923, 174.289, 169.641, 161.016, 157.398, 152.151, 147.425, 140.933, 135.924, 132.365, 127.017, 122.127, 117.817, 113.076, 109.055, 105.16, 101.323, 98.098, 95.0548, 90.729, 87.6495, 84.0899, 80.2237, 77.2201, 74.8848, 71.3554, 68.7745, 65.9911, 63.4136, 61.3859, 58.1903, 56.4155, 53.8486, 52.0196, 49.2921, 47.0735, 45.4345, 43.8434, 41.7181, 39.8988, 38.2262, 36.4435, 34.8984, 33.4664, 31.8056, 30.351, 29.2074, 27.6924, 26.7754, 25.4965, 24.2802, 22.9651, 22.0059, 21.0915, 19.9129, 19.1041, 18.1487, 17.3218, 16.5957, 15.5323, 14.8035, 14.2514, 13.3782, 12.8667, 12.2891, 11.61, 11.0026, 10.3747, 9.90294, 9.42648, 8.85324, 8.50121, 7.89834, 7.65197, 7.22768, 6.7755, 6.34855, 5.98336, 5.76555, 5.38056, 5.11024, 4.7748, 4.59117, 4.23247, 4.00814, 3.79607, 3.68702, 3.3767, 3.16309, 2.98282, 2.8095, 2.65875, 2.50561, 2.32516, 2.16357, 2.03235, 1.84061, 1.72628, 1.62305, 1.48916, 1.38784, 1.28366, 1.24693, 1.18552, 1.16085, 1.12596, 1.09298, 1.07402, 1.06105, 1.02954};
	return Ncoll[Bin];
}
