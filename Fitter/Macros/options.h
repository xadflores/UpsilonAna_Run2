//binning options set

const int nPtBin = 3;
const int nCentBin = 4;
const int nCentPerBin = 9;
const int nRapBin = 2;

double DPtMinArr[nPtBin] = {0.0,5.0,12.0};
double DPtMaxArr[nPtBin] = {5.0,12.0,30.0};
double DRapMinArr[nRapBin] = {0,1.2};
double DRapMaxArr[nRapBin] = {1.2,2.4};
int CentMinArr[nCentPerBin] = {0,10,20,40,60,80,100,120,140};
int CentMaxArr[nCentPerBin] = {10,20,40,60,80,100,120,140,160};

int CentMinArrUL[nCentBin] = {0,20,60,100};
int CentMaxArrUL[nCentBin] = {20,60,100,160};


