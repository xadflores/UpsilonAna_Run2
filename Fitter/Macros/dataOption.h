//Parameters for fitting signal shapes from MC and 
//background from SS
//

const int PtBin = 3;
const int CentBin = 9;
const int RapBin = 2;

//pbpb MB
double turnOnPbPb = 7.8589;
double widthPbPb = 1.43669;
double decayPbPb = 6.0811;

//pbpb parameters kine
double turnOnPtPbPb[PtBin] = {8.49753,10.4025,22.2682};
double widthPtPbPb[PtBin] = {1.12846,18.9378,10.4544};
double decayPtPbPb[PtBin] = {4.48729,5.70287,3.13039};

double turnOnRapPbPb[RapBin] = {7.81916,8.1812};
double widthRapPbPb[RapBin] = {1.47851,1.69509};
double decayRapPbPb[RapBin] = {8.22981,2.97486};

//pp MB
double turnOnPP=8.51662;
double widthPP=1.80399;
double decayPP=7.95459;

//pp parameters kine
double turnOnPtPP[PtBin] = {8.97409,0.302983,0.00134883};
double widthPtPP[PtBin] = {1.37024,1.12331,24.9998};
double decayPtPP[PtBin] = {6.29136,24.6017,9.59006};

double turnOnRapPP[RapBin] = {8.53934,8.64054};
double widthRapPP[RapBin] = {2.08368,1.55561};
double decayRapPP[RapBin] = {11.2378,4.49366};

//PbPb MC MB
double alphaMBPbPb = 1.48444;
double sigmaMBPbPb = 0.0692881;
double npowMBPbPb = 3.74258;
double sigFracMBPbPb = 0.5734;
double scaleWidthMBPbPb = 1.90291;

double alphaMBPP = 1.52367;
double sigmaMBPP = 0.0674679;
double npowMBPP = 3.64936;
double sigFracMBPP = 0.555944;
double scaleWidthMBPP = 1.94018;


//PbPb MC kine
double alphaPtPbPb[PtBin] = {1.60086,1.34875,1.52342};
double sigmaPtPbPb[PtBin] = {0.0683047,0.0677385,0.0750181};
double npowPtPbPb[PtBin] = {2.47919,6.66898,3.60884};
double sigFracPtPbPb[PtBin] = {0.553312,0.562843,0.657418};
double scaleWidthPtPbPb[PtBin] = {1.90563,1.92393,1.90508};

double alphaRapPbPb[RapBin] = {1.8528,1.90506};
double sigmaRapPbPb[RapBin] = {0.0590767,0.109618};
double npowRapPbPb[RapBin] = {1.48856,2.10867};
double sigFracRapPbPb[RapBin] = {0.54841,0.617729};
double scaleWidthRapPbPb[RapBin] = {1.68978,1.55153};

//pp MC kine
double alphaPtPP[PtBin] = {1.55317,1.49047,1.55081};
double sigmaPtPP[PtBin] = {0.065411,0.0689875,0.0693828};
double npowPtPP[PtBin] = {3.56478,3.76049,3.39185};
double sigFracPtPP[PtBin] = {0.53645,0.581709,0.555974};
double scaleWidthPtPP[PtBin] = {1.99296,1.92588,1.85962};

double alphaRapPP[RapBin] = {1.82562,1.91518};
double sigmaRapPP[RapBin] = {0.061187,0.105424};
double npowRapPP[RapBin] = {1.71539,1.88793};
double sigFracRapPP[RapBin] = {0.641273,0.562814};
double scaleWidthRapPP[RapBin] = {1.71974,1.54627};
