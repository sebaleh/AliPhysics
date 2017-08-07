  // $Id$
  //
  // Emcal Neutral Cluster analysis base task.
  //
  // 



#include "AliAODCaloTrigger.h"
#include "AliVCaloTrigger.h"
#include <TClonesArray.h>
#include <TChain.h>
#include <TList.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <THnSparse.h>
#include "AliAnalysisManager.h"
#include "AliCentrality.h"
#include "AliEMCALGeometry.h"
#include "AliESDEvent.h"
#include "AliAODEvent.h"
#include "AliLog.h"
#include "AliVCluster.h"
#include "AliVEventHandler.h"
#include "AliVParticle.h"
#include "AliClusterContainer.h"
#include "AliTrackContainer.h"
#include "AliVTrack.h"
#include "AliEmcalParticle.h"
#include "AliParticleContainer.h"
#include "AliAODCaloCluster.h"
#include "AliESDCaloCluster.h"
#include "AliVCaloCells.h"
#include "AliPicoTrack.h"
#include "AliAODMCParticle.h"
#include "AliAODMCHeader.h"
#include "AliEMCALRecoUtils.h"
#include "AliLog.h"
#include "TF1.h"
#include "Riostream.h"
#include "AliEMCALTriggerPatchInfo.h"
#include <TArrayI.h>
#include <AliEMCALTriggerBitConfig.h>
//#include <AliEmcalTriggerMakerTask.h>
#include <TSystem.h>
#include <TROOT.h>
#include "TRandom3.h"
#include "AliGenPythiaEventHeader.h"
#include <AliEMCALTriggerPatchInfo.h>
#include <AliTriggerClass.h>
#include <AliCDBManager.h>
#include <AliEmcalDownscaleFactorsOCDB.h>


#include "AliAnalysisTaskEMCALClusterTurnOn.h"

  /// \cond CLASSIMP
ClassImp(AliAnalysisTaskEMCALClusterTurnOn);
  /// \endcond

using std::cout;
using std::endl;
  //________________________________________________________________________
AliAnalysisTaskEMCALClusterTurnOn::AliAnalysisTaskEMCALClusterTurnOn() :
AliAnalysisTaskEmcal("AliAnalysisTaskEMCALClusterTurnOn",kTRUE),
  //fParticleCollArray(),
fAOD(0),
fAODEvent(0x0),
fVevent(0),
fEMCALRecoUtils(new AliEMCALRecoUtils),
  //fOutputList(0),
fIsoConeRadius(0.4),
fdetacut(0.025),
fdphicut(0.03),
fM02mincut(0.1),
fM02maxcut(0.3),
fQA(0),
fThn(0),
fNDimensions(0),
fClusDimensions(0),
fIsNLMCut(kFALSE),
fNLMCut(0),
fNLMmin(0),
fTMClusterRejected(kTRUE),
fTMClusterInConeRejected(kTRUE),
fTest1(0),
fBinsPt(),
fBinsPtCl(),
fBinsRejection(),
fBinsEnergy(),
fBinsEta(),
fBinsPhi(),
fBinsClEta(),
fBinsClPhi(),
fEtaPhiClus(0),
fPT(0),
fE(0),
fNLM(0),
fVz(0),
fEvents(0),
fPtaftTime(0),
fCelldist(0),
fPtaftCell(0),
fNLMdist(0),
fPtaftNLM(0),
fPtaftTM(0),
fDTBC(0),
fPtaftDTBC(0),
fPtaftFC(0),
fTriggerbit(0), 
fADCvsEonline(0),
hL0Amplitude(0),
hPatchADC(0),
hmaxADC(0),
hmaxL0ADC(0),
fL0triggered(0),
fEventsover10(0),
fL1triggered(0),
fADCvsEall(0),
fClusTime(0),
fPt_trig(0),
fM02cut(0),
fOutputTHnS(0),
hFastOrIndex(0),
fOutTHnS_Clust(0)
{

    // Default constructor.
  
    //fParticleCollArray.SetOwner(kTRUE);
    // for(Int_t i = 0; i < 12;    i++)  fGeomMatrix[i] =  0;
  
  SetMakeGeneralHistograms(kTRUE);
}

  //________________________________________________________________________
AliAnalysisTaskEMCALClusterTurnOn::AliAnalysisTaskEMCALClusterTurnOn(const char *name, Bool_t histo) :
AliAnalysisTaskEmcal(name, histo),
  //fParticleCollArray(),
fAOD(0),
fAODEvent(0x0),
fVevent(0),
fEMCALRecoUtils(new AliEMCALRecoUtils),
fIsoConeRadius(0.4),
fdetacut(0.025),
fdphicut(0.03),
fM02mincut(0.1),
fM02maxcut(0.3),
fQA(0),
fThn(0),
fNDimensions(0),
fClusDimensions(0),
fIsNLMCut(kFALSE),
fNLMCut(0),
fNLMmin(0),
fTMClusterRejected(kTRUE),
fTMClusterInConeRejected(kTRUE),
fTest1(0),
fBinsPt(),
fBinsPtCl(),
fBinsRejection(),
fBinsEnergy(),
fBinsEta(),
fBinsPhi(),
fBinsClEta(),
fBinsClPhi(),
fEtaPhiClus(0),
fPT(0),
fE(0),
fNLM(0),
fVz(0),
fEvents(0),
fPtaftTime(0),
fCelldist(0),
fPtaftCell(0),
fNLMdist(0),
fPtaftNLM(0),
fPtaftTM(0),
fDTBC(0),
fPtaftDTBC(0),
fPtaftFC(0),
fTriggerbit(0), 
fADCvsEonline(0),
hL0Amplitude(0),
hPatchADC(0),
hmaxADC(0),
hmaxL0ADC(0),
fL0triggered(0),
fEventsover10(0),
fL1triggered(0),
fADCvsEall(0),
fClusTime(0),
fPt_trig(0),
fM02cut(0),
fOutputTHnS(0),
hFastOrIndex(0),
fOutTHnS_Clust(0)
{

    // Standard constructor.
  
    //fParticleCollArray.SetOwner(kTRUE);
    //    for(Int_t i = 0; i < 12;    i++)  fGeomMatrix[i] =  0;
  
  SetMakeGeneralHistograms(kTRUE);
}

  //________________________________________________________________________
AliAnalysisTaskEMCALClusterTurnOn::~AliAnalysisTaskEMCALClusterTurnOn(){
    // Destructor
}


  //________________________________________________________________________
void AliAnalysisTaskEMCALClusterTurnOn::UserCreateOutputObjects(){
    // Create ouput histograms and THnSparse and TTree
  
  AliAnalysisTaskEmcal::UserCreateOutputObjects();
    //printf("Up here all good");
  
  
  TString sIsoMethod="\0",sBoundaries="\0";
  
  sIsoMethod = "Clust";

  sBoundaries = "EMCAL Acceptance";

  fOutput = new AliEmcalList(); // RH: Leak? fOutput already exists in base class
  fOutput->SetOwner();
    //Initialize the common Output histograms
  TString sTitle;
  
  if(fThn){

    Int_t binPT = fBinsPt.size()-1;
    Int_t binPTCl = fBinsPtCl.size()-1;
    Int_t binRejection = fBinsRejection.size()-1;
    Int_t binEnergy = fBinsEnergy.size()-1;
    Int_t binetacell = fBinsEta.size()-1;
    Int_t binphicell = fBinsPhi.size()-1;
    Int_t binetacl = fBinsClEta.size()-1;
    Int_t binphicl = fBinsClPhi.size()-1;
    
    
    Int_t bins[] = {binPT, binRejection, binEnergy, binetacell, binphicell};


            fNDimensions = sizeof(bins)/sizeof(Int_t);
    const Int_t ndims =   fNDimensions;
    
    sTitle = Form("Direct Photons: p_{T} ,Reason for rejection, Cell Energy, #eta distr,#phi distr; p_{T} (GeV/c); Rejection ; E (GeV) ; #eta cell index; #phi cell index");
    
    fOutputTHnS =  new THnSparseF("fHnOutput",sTitle.Data(), ndims, bins);
    fOutputTHnS->SetBinEdges(0,fBinsPt.data());
    fOutputTHnS->SetBinEdges(1,fBinsRejection.data());
    fOutputTHnS->SetBinEdges(2,fBinsEnergy.data());
    fOutputTHnS->SetBinEdges(3,fBinsEta.data());
    fOutputTHnS->SetBinEdges(4,fBinsPhi.data());
    fOutputTHnS->Sumw2();
    
    fOutput->Add(fOutputTHnS);
    
    
//    Int_t binsCluster[] = {binPTCl, binRejection ,binetacl,binphicl};
//    
//    fClusDimensions = sizeof(binsCluster)/sizeof(Int_t);
//    const Int_t ndimsClus = fClusDimensions;
//    
//    fOutTHnS_Clust = new THnSparseF ("fOutTHnS_Clust","E_{T}, Reason for rejection, #eta_{clus}, #phi_{clus}; E_{T} (GeV/c); Rejection ; #eta_{clus}; #phi_{clus}",ndimsClus,binsCluster);
//    fOutTHnS_Clust->SetBinEdges(0,fBinsPtCl.data());
//    fOutTHnS_Clust->SetBinEdges(1,fBinsRejection.data());
//    fOutTHnS_Clust->SetBinEdges(2,fBinsClEta.data());
//    fOutTHnS_Clust->SetBinEdges(3,fBinsClPhi.data());
//    fOutTHnS_Clust->Sumw2();
//    fOutput->Add(fOutTHnS_Clust);
            

    hFastOrIndex = new TH2D("hFastOrIndex","FastOR index vs E_{Clus}; FastOR; #it{E}_{T} (GeV)",3101,-0.5,3100.5,70,0.,70.);
    hFastOrIndex->Sumw2();
    fOutput->Add(hFastOrIndex);
  }
        
    //Common histograms QA initialization
  if(fQA){
      //Include QA plots to the OutputList //DEFINE BETTER THE BINNING AND THE AXES LIMITS
    fClusTime = new TH1D("hClusTime_NC","Time distribution for Clusters",1800,-150.,150.);
    fClusTime->Sumw2();
    fOutput->Add(fClusTime);
    
    fCelldist = new TH1D("hCells","Cell distribution before cell cut",100,0.,100.);
    fCelldist->Sumw2();
    fOutput->Add(fCelldist);

    fDTBC = new TH1D("hDTBCaftTM","DTBC distribution after TM",50,0.,50.);
    fDTBC->Sumw2();
    fOutput->Add(fDTBC);

    fNLMdist = new TH1D("hNLMaftCell","NLM distribution after Cell cut",10,0.,10.);
    fNLMdist->Sumw2();
    fOutput->Add(fNLMdist);

    fPT = new TH1D("hPt_NC","P_{T} distribution for Neutral Clusters",400,0.,100.);
    fPT->Sumw2();
    fOutput->Add(fPT);

    fNLM = new TH2D("hNLM_NC","NLM distribution for Neutral Clusters",10,0.,10.,100,0.,100.);
    fNLM->Sumw2();
    fOutput->Add(fNLM);
  
    fPtaftTime = new TH1D("hPtaftTime_NC","p_{T} distribution for Clusters after cluster time cut",200,0.,100.);
    fPtaftTime->Sumw2();
    fOutput->Add(fPtaftTime);
    
    fPtaftCell = new TH1D("hPtaftCell_NC","p_{T} distribution for Clusters after Ncells cut",200,0.,100.);
    fPtaftCell->Sumw2();
    fOutput->Add(fPtaftCell);
    
    fPtaftNLM = new TH1D("hPtaftNLM_NC","p_{T} distribution for Clusters after NLM cut",200,0.,100.);
    fPtaftNLM->Sumw2();
    fOutput->Add(fPtaftNLM);
    
    fPtaftTM = new TH1D("hPtaftTM_NC","p_{T} distribution for Neutral Clusters",200,0.,100.);
    fPtaftTM->Sumw2();
    fOutput->Add(fPtaftTM);
    
    fPtaftDTBC = new TH1D("hPtaftDTBC_NC","p_{T} distribution for Neutral Clusters after DTBC cut",200,0.,100.);
    fPtaftDTBC->Sumw2();
    fOutput->Add(fPtaftDTBC);
    
    fPtaftFC = new TH1D("hPtaftFC_NC","p_{T} distribution for Clusters after fiducial cut",200,0.,100.);
    fPtaftFC->Sumw2();
    fOutput->Add(fPtaftFC);
  
//  fVz = new TH1D("hVz_NC","Vertex Z distribution",100,-50.,50.);
//  fVz->Sumw2();
//  fOutput->Add(fVz);
  }
    //   Initialization of all the Common THistos for the Three different outputs
  
  fE = new TH1D("hE_NC","E distribution for Clusters",200,0.,100.);
  fE->Sumw2();
  fOutput->Add(fE);
    
  fEvents = new TH1D("hEvents_NC","Events",100,0.,100.);
  fEvents->Sumw2();
  fOutput->Add(fEvents);
 
//  fTriggerbit = new TH2D("hTriggerbit","Events per Trigger Bit; trigger bit; max #it{E}_{T,Clus} (GeV); Events", 3,-0.5,2.5,100,0.,100.);
//  fTriggerbit->Sumw2();
//  fOutput->Add(fTriggerbit);

//  fADCvsEonline = new TH2D("hADCvsEonline","online ADC vs Patch Energy; ADC; E",400,0.,400.,200,0.,100.);
//  fADCvsEonline->Sumw2();
//  fOutput->Add(fADCvsEonline);

  hPatchADC = new TH1D("hPatchADC","L1 ADC per patch distribution; ADC", 500,0.,500.);
  hPatchADC->Sumw2();
  fOutput->Add(hPatchADC); 

  hmaxADC = new TH2D("hmaxPatchADC_E_L1","L1 max ADC vs patch energy distribution; ADC; E (GeV)", 1000,0.,2000.,100,0.,100.);
  hmaxADC->Sumw2();
  fOutput->Add(hmaxADC); 

  hmaxL0ADC = new TH2D("hmaxPatchADC_E_L0","L0 max ADC vs patch energy distribution; ADC; E (GeV)", 10000,100000.,400000.,80,0.,80.);
  hmaxL0ADC->Sumw2();
  fOutput->Add(hmaxL0ADC); 

  hL0Amplitude = new TH1D("hL0Amplitude","L0 Amplitudes for L0time = 8|9; Amplitude",5001,24999.5,30000.5);
  hL0Amplitude->Sumw2();
  fOutput->Add(hL0Amplitude);

  fL0triggered = new TH1D("hL0triggered","leading Cluster of L0 triggered Events;#it{E}_{T} (GeV);counts",200,0.,100.);
  fL0triggered->Sumw2();
  fOutput->Add(fL0triggered);

  fEventsover10 = new TH1D("hEventsover10", "Events with E_cluster > 10",100,0.,100.);
  fEventsover10->Sumw2();
  fOutput->Add(fEventsover10);

  fL1triggered = new TH1D("hL1triggered","leading Cluster of L1 triggered Events;#it{E}_{T} (GeV);counts",200,0.,100.);
  fL1triggered->Sumw2();
  fOutput->Add(fL1triggered);

  fADCvsEall = new TH2D("hADCvsEall","ADC vs Patch Energy; ADC; E",400,0.,400.,200,0.,100.);
  fADCvsEall->Sumw2();
  fOutput->Add(fADCvsEall);

  fPt_trig = new TH2D("hPt_trig",";#it{E}_{T} (GeV);trigger class;Rejection",100,0.,100.,1,0.,1.);
  #if ROOT_VERSION_CODE < ROOT_VERSION(6,4,2)
  fPt_trig->SetBit(TH1::kCanRebin);
  #else
  fPt_trig->SetCanExtend(TH1::kAllAxes);
  #endif
  fOutput->Add(fPt_trig);
  
  fEtaPhiClus = new TH2D ("hEtaPhiClusActivity","Events with E_{max Patch} < 8.5 GeV; #eta; #phi",250,-0.8,0.8, 250, 1.2, 3.4);
    //  fEtaPhiClus->Sumw2();
  fOutput->Add(fEtaPhiClus);
    

  
  
  PostData(1, fOutput);
    //     //   return;
}


  //________________________________________________________________________
Double_t* AliAnalysisTaskEMCALClusterTurnOn::GenerateFixedBinArray(Int_t n, Double_t min, Double_t max) const
{
    // Generate the bin array for the ThnSparse
  
  Double_t *bins = new Double_t[n+1];
  
  Double_t binWidth = (max-min)/n;
  bins[0] = min;
  for (Int_t i = 1; i <= n; i++) {
    bins[i] = bins[i-1]+binWidth;
  }
  
  return bins;
}


  //________________________________________________________________________
void AliAnalysisTaskEMCALClusterTurnOn::ExecOnce()
{
    //   Init the analysis.
    //tracks for CT Matching
  AliTrackContainer *tracks = GetTrackContainer("tpconlyMatch");
    //  Printf("name of the first track container: %s", tracks->GetClassName().Data());
  if (!tracks) {
    AliError(Form("%s: This task needs a 1particle container!", GetName()));
    return;
  }
    //tracks for Isolation
  AliTrackContainer *tracksANA = GetTrackContainer("filterTracksAna");
    //  Printf("name of the second track container: %s", tracksANA->GetClassName().Data());
  
  if (!tracksANA) {
    AliError(Form("%s: This task needs a 2particle container!", GetName()));
    return;
  }
    //clusters
  AliClusterContainer *clusters = GetClusterContainer(0);
  if (!clusters) {
    AliError(Form("%s: This task needs a cluster container!", GetName()));
    return;
  }
  
    //Init the EMCAL Framework
  AliAnalysisTaskEmcal::ExecOnce();
  if (!fLocalInitialized) {
    
    AliError(Form("AliAnalysisTask not initialized"));
    return;
  }
}

  //______________________________________________________________________________________
Bool_t AliAnalysisTaskEMCALClusterTurnOn::Run()
{
    // Run the analysis.
    //vertex cuts
  AliTrackContainer *tracks = GetTrackContainer("tpconlyMatch");
  if(!tracks){
    Printf("Cannot find the tracks for CT Matching");
    return kFALSE;
  }
  
  if(tracks->GetTrackFilterType()!= AliEmcalTrackSelection::kTPCOnlyTracks){
    AliWarning(Form("CT matching NOT performed with TPCOnly Tracks"));
    AliWarning(Form("You better be sure of what you are doing"));
  }
  
  AliTrackContainer *tracksANA = GetTrackContainer("filterTracksAna");
  if(!tracksANA){
    Printf("Cannot find the tracks for Isolation");
    return kFALSE;
  }
  
    //  Printf("FilterType of the tracks for Analysis: %d \t(should be %d)", tracksANA->GetTrackFilterType(),AliEmcalTrackSelection::kHybridTracks);
  
    //    AliError(Form("\n\n\n\nGO CHECK the Settings!!!! Is Isolation calculated with filteredTracks?\n\n\n\n"));
  if(tracksANA->GetTrackFilterType()!= AliEmcalTrackSelection::kHybridTracks){
    AliWarning(Form("Isolation NOT calculated with HybridTracks"));
    AliWarning(Form("You better be sure of what you are doing"));
  }
  
  
  fVevent = dynamic_cast<AliVEvent*>(InputEvent());
  
    //  Printf("Vertex Z coordinate for M2: %f", fVertex[2]);
    //  Printf("Vertex Z coordinate for NF: %lf", fVertex[2]);
  
  if (fVertex[2]>10. || fVertex[2]<-10.) return kFALSE;
    //  AliError(Form("La task tourne bien"));
  
  AliClusterContainer* clusters = GetClusterContainer(0);
    ////Printf("Inside Run Method, Number of clusters for this event: %d",clusters->GetNAcceptedClusters());
  Int_t nbTracksEvent;
  nbTracksEvent =InputEvent()->GetNumberOfTracks();
  

    // Fill events number histogram
  fEvents->Fill(0);
//gROOT->LoadMacro(Form("%s/PWG/EMCAL/macros/AddTaskTriggerMakerNew.C", gSystem->ExpandPathName(gSystem->Getenv("ALICE_PHYSICS"))));
//AliEmcalTriggerMakerTask *triggermaker = AddTaskTriggerMakerNew();
//triggermaker->SelectCollisionCandidates(AliVEvent::kINT7 | AliVEvent::kEMC7 | AliVEvent::kEMCEJE | AliVEvent::kEMCEGA);
//  AliEmcalDownscaleFactorsOCDB *downscalehandler = AliEmcalDownscaleFactorsOCDB::Instance();
//  double ds = downscalehandler->GetDownscaleFactorForTriggerClass("CEMC7-S-NOPF-ALLNOTRD");
//  cout << "downscalefactor current run: " << ds << endl;
//  downscalehandler->SetRun(180500);
//  ds = downscalehandler->GetDownscaleFactorForTriggerClass("CEMC7-S-NOPF-ALLNOTRD");
//  cout << "downscalefactor 180500: " << ds << endl;

//  AliEMCALGeometry* geom = AliEMCALGeometry::GetInstance();
//  TClonesArray* fTriggerPatches = dynamic_cast<TClonesArray*>(InputEvent()->FindListObject("EmcalTriggers"));
//  if (fTriggerPatches) {
//    Int_t nPatches = fTriggerPatches->GetEntries();
//    for (Int_t i = 0; i < nPatches; i++) {
//      AliEMCALTriggerPatchInfo* patch = static_cast<AliEMCALTriggerPatchInfo*>(fTriggerPatches->At(i));
//      if(!patch) continue;
//      if(!patch->
//      printf("\ttrigger patch1 E=%1.2f\n",patch->GetPatchE());
//      cout << "trigger patch1 Et = " << patch->GetPatchET() << " Trigger Bits: " << patch->GetTriggerBits() << " PhiGeo1: " << patch->GetPhiGeo() << " PhiMax1: " << patch->GetPhiMax() << " EdgeCellX: " << patch->GetEdgeCellX()<<" Row start: " << patch->GetRowStart() << endl;
//      TArrayI Tarr(1024);
//      patch->GetCellIndices(geom,&Tarr);
////      Int_t nCells = Tarr.GetSize();
////      for(Int_t ii=0; i<nCells; i++){
////        cout << " cell number" << ii << ": " << Tarr.GetAt(ii);
////      }
//          
////      printf(" phiCM1: %f", patch->GetPhiCM());
////      printf(" etaCM1: %f", patch->GetEtaCM());
//      if(patch->IsRecalc()) printf("patch1 is recal");
//      if(patch->IsLevel0()) printf("patch1 is level0");
//    }
//  }
//  else printf("TClonesArray 1 not found!!!!");  


  
//  TClonesArray *triPatchInfo = dynamic_cast<TClonesArray*>(fVevent->FindListObject("EmcalTriggers")); 
  TClonesArray *triPatchInfo = dynamic_cast<TClonesArray*>(InputEvent()->FindListObject("EmcalTriggers")); 
  Bool_t isL1 = kFALSE;
  Bool_t isL0recalc = kFALSE;
  Int_t maxADC = 0;
  Int_t maxL0ADC = 0;
  Double_t E_of_maxADC = 0.;
  Double_t E_of_maxL0 = 0.;
  if(triPatchInfo){
    Int_t nPatch = triPatchInfo->GetEntries();
    for(Int_t ip = 0;ip<nPatch;ip++){
      AliEMCALTriggerPatchInfo *pti = static_cast<AliEMCALTriggerPatchInfo*>(triPatchInfo->At(ip));
      if(!pti) continue;
      if(!pti->IsEMCal()) continue;
      fADCvsEall->Fill(pti->GetADCAmp(),pti->GetPatchE());
      if(pti->IsLevel0Recalc() && maxL0ADC < pti->GetADCAmp()){
        maxL0ADC = pti->GetADCAmp();
        E_of_maxL0 = pti->GetPatchE();
      }
      if(pti->IsLevel0Recalc() && pti->GetADCAmp() > 106) isL0recalc = kTRUE;
      if(!pti->IsRecalcGamma()) continue;
//      fTriggerbit->Fill(0,veclclus.E());
//      if(pti->IsGammaHighRecalc()){
//        fADCvsEonline->Fill(pti->GetADCAmp(),pti->GetPatchE());
//        fTriggerbit->Fill(2,veclclus.E());
//        printf("\ttrigger patch2 E=%1.1f\n",pti->GetPatchE());
//        printf(" phiCM2: %f", pti->GetPhiCM());
//        printf(" etaCM2: %f", pti->GetEtaCM());
      hPatchADC->Fill(pti->GetADCAmp());
      if(maxADC<pti->GetADCAmp()){
        maxADC = pti->GetADCAmp();
        E_of_maxADC = pti->GetPatchE();
      }
      if(pti->GetADCAmp() > 130){  
//        AliEMCALTriggerBitConfig* triggerBitConfig = new AliEMCALTriggerBitConfigNew();
//        cout << "Gamma bit: " << triggerBitConfig->GetGammaHighBit() << " L0 bit: " << triggerBitConfig->GetLevel0Bit() << endl;
//        AliEMCALTriggerBitConfig* triggerBitConfigold = new AliEMCALTriggerBitConfigOld();
//        cout << "Gamma bit old: " << triggerBitConfigold->GetGammaHighBit() << " L0 bit old: " << triggerBitConfigold->GetLevel0Bit() << endl;
//        cout << "ADC Ampl L1: " << pti->GetADCAmp() << endl;
        isL1 = kTRUE;
//        break;
      }
//        if(pti->IsRecalc()) printf("patch2 is recal\n");
//        if(pti->IsLevel0()) printf("patch2 is level0");
//      if(pti->IsLevel0()) fTriggerbit->Fill(1,veclclus.E());
//    if(!pti->IsLevel0()) continue;
    }
  }                      
  else printf("TClonesArray 2 not found!!!!");
  hmaxADC->Fill(maxADC,E_of_maxADC);
  hmaxL0ADC->Fill(maxL0ADC,E_of_maxL0);
  cout << "MaxADCL0: " << maxL0ADC << " E_L0: " << E_of_maxL0 << endl;

  fAODEvent = dynamic_cast<AliAODEvent*> (InputEvent());  
  Int_t L0times[30], ntimes;
  Bool_t isL0 = kFALSE;
  AliAODCaloTrigger* AODtrigger   =fAODEvent->GetCaloTrigger("EMCAL");
  AODtrigger->Reset();
  while(AODtrigger->Next()){ 
    Int_t TriggerBits;
//    AODtrigger->GetTriggerBits(TriggerBits);
    AODtrigger->GetNL0Times(ntimes);
    AODtrigger->GetL0Times(L0times);
    Float_t Ampli;
    AODtrigger->GetAmplitude(Ampli);
    for(Int_t i = 0; i<ntimes; i++){
      if((L0times[i]==8 || L0times[i] == 9) && Ampli > 28710.){
//        cout << "L0time: " << L0times[i] << endl;
//        cout << "TriggerBit: " << TriggerBits << " Amplitude: " << Ampli << endl;
//        AliEMCALTriggerBitConfig* triggerBitConfig = new AliEMCALTriggerBitConfigNew();
//        cout << "Gamma bit: " << triggerBitConfig->GetGammaHighBit() << " L0 bit: " << triggerBitConfig->GetLevel0Bit() << endl;
//        AliEMCALTriggerBitConfig* triggerBitConfigold = new AliEMCALTriggerBitConfigOld();
//        cout << "Gamma bit old: " << triggerBitConfigold->GetGammaHighBit() << " L0 bit old: " << triggerBitConfigold->GetLevel0Bit() << endl;
        AODtrigger->Print();
        isL0 = kTRUE;
        hL0Amplitude->Fill(Ampli);
//        break;
      }
    }
  }
  TLorentzVector veclclus;
  for (auto it : clusters->accepted()){
    AliVCluster *coi = static_cast<AliVCluster*>(it);
    if(!coi) {
      AliError("No cluster found");
      return kFALSE;
    }
    TLorentzVector vecCOI;
    coi->GetMomentum(vecCOI,fVertex);
    Double_t coiTOF = coi->GetTOF()*1e9;
    if(coiTOF< -30. || coiTOF > 30.){
      continue;
    } 
    if(E_of_maxADC<8.5) fEtaPhiClus->Fill(vecCOI.Eta(),vecCOI.Phi());
    if(vecCOI.E()>veclclus.E()) veclclus = vecCOI;
  }
  if(veclclus.E() > 10.) fEventsover10->Fill(0);
//  if(isL0 || isL1) {


//    AliVCluster *coi = (clusters->GetLeadingCluster());
//    if(!coi){
//      
//      AliError(Form("No leading cluster"));
//    }
//    else{
//      TLorentzVector vecCOI;
//      coi->GetMomentum(vecCOI,fVertex);
    if(isL0){
      if(veclclus.E() > 10.) fEventsover10->Fill(2);
      fL0triggered->Fill(veclclus.E());
//      fL0triggered->Fill(vecCOI.Pt());
    }
    if(isL0recalc){
      if(veclclus.E() > 10.) fEventsover10->Fill(3);
    }
    if(isL1){
      if(veclclus.E() > 10.) fEventsover10->Fill(8);
      fL1triggered->Fill(veclclus.E());
//      fL1triggered->Fill(vecCOI.Pt());
    }
//  }
//  for(Int_t i = 1; i<=15; i++){
//    if(leadcoi->E() > i) fEvents->Fill(i);
//  }
    //AliError(Form("one event"));
  
  
    //Fill Vertex Z histogram
  if(fQA) fVz->Fill(fVertex[2]);
  
    // delete output USEFUL LATER FOR CONTAINER CREATION !!
    //fOutClusters->Delete();
  Int_t index=0;
  
  
      //get the entries of the Cluster Container
      //whatever is a RETURN in LCAnalysis here is a CONTINUE,
      //since there are more than 1 Cluster per Event
    
      //    Printf("Starting loop on clusters in RUN method");

  for (auto it : clusters->accepted()){
    AliVCluster *coi = static_cast<AliVCluster*>(it);
    if(!coi) {
      AliError("No cluster found");
      return kFALSE;
    }
      //
    
    index=coi->GetID();
    TLorentzVector vecCOI;
    coi->GetMomentum(vecCOI,fVertex);
    Double_t coiTOF = coi->GetTOF()*1e9;
    Double_t coiM02 = coi->GetM02();
            
    TObjArray* triggerClasses = InputEvent()->GetFiredTriggerClasses().Tokenize(" ");
    TIter next(triggerClasses);
    TObjString* triggerClass = 0;

//    Int_t TriggerBits = InputEvent()->GetTriggerBits();
//    cout << "TriggerBits: " << TriggerBits << endl;
//    AliCDBManager* man = AliCDBManager::Instance(); 
//    man->SetDefaultStorage("raw://"); 
//    man->SetRun(180044);
//    AliCDBPath* cdbpath = "GRP/CTP/Config";
//    AliCDBEntry* entry = man->Get(cdbpath);
//    AliTriggerConfiguration* cfg = (AliTriggerConfiguration*)entry->GetObject(); 
//    AliTriggerClass* triggerclass = cfg->GetClasses().FindObject("CEMC7-S-NOPF-ALLNOTRD");
//    Double_t ds;
//    triggerclass->GetDownscaleFactor(ds); 
//    cout << "downscaling factor = " << ds << endl;

//    TClonesArray* fTriggerPatchInfo;
//    TString fCaloTriggerPatchInfoName = "EmcalTriggers";
//  fTriggerPatchInfo= dynamic_cast<TClonesArray*>(fVevent->FindListObject(fCaloTriggerPatchInfoName ));
//  Int_t nPatch = fTriggerPatchInfo->GetEntries();
//  cout << "Patch Entries: " << nPatch << endl;
//  AliEMCALTriggerPatchInfo *patch;
////   if (nPatch> 0) {cout << "NEW Triggers in this event*********************************" << endl;}
//  for (Int_t iPatch = 0; iPatch < nPatch; iPatch++) {
//    patch = (AliEMCALTriggerPatchInfo*)fTriggerPatchInfo->At( iPatch );
//    Int_t triggerbits = patch->GetTriggerBits();
//    cout << "TriggerBits: " << triggerbits << endl;
//  }

    if(fQA) {
      fEtaPhiClus->Fill(vecCOI.Eta(),vecCOI.Phi());

      fPT->Fill(vecCOI.Pt());
    }
    fE->Fill(vecCOI.E());
    
    Double_t checktof = coi->GetTOF()*1e9;
    if(fQA) fClusTime->Fill(checktof);
    
    if(coiTOF< -30. || coiTOF > 30.){
      FillTHnSparse(coi,vecCOI, 1.5);
      continue;
    } 
    if(fQA) {
      fPtaftTime->Fill(vecCOI.Pt());
      fCelldist->Fill(coi->GetNCells());
    }
    
    if((coi->GetNCells() < 2)){
      FillTHnSparse(coi,vecCOI, 2.5);
      continue;
    }
    if(fQA) fPtaftCell->Fill(vecCOI.Pt());
    
    Int_t nlm=0;
    AliVCaloCells * fCaloCells =InputEvent()->GetEMCALCells();
    if(fCaloCells)
    {
      nlm = GetNLM(coi,fCaloCells);
      AliDebug(1,Form("NLM = %d",nlm));
      
      if(fQA){
        fNLMdist->Fill(nlm);
      }        
      
      if(fIsNLMCut && fNLMCut>0 && fNLMmin>0)
        if(nlm > fNLMCut || nlm < fNLMmin ){
          FillTHnSparse(coi,vecCOI, 3.5);
          continue;
        }
    }
    else
      AliDebug(1,Form("Can't retrieve EMCAL cells"));
    
    if(fQA) {
      fPtaftNLM->Fill(vecCOI.Pt());
      fDTBC->Fill(coi->GetDistanceToBadChannel());
    }
    
    if((coi->GetDistanceToBadChannel() < 2)){
      FillTHnSparse(coi,vecCOI, 4.5);
      continue;
    }
    
    if(fQA) fPtaftDTBC->Fill(vecCOI.Pt());
    if(fM02cut){
      if(coiM02 < 0.1) continue;
    }
    while (triggerClass = static_cast<TObjString*>(next())) fPt_trig->Fill(vecCOI.E(),triggerClass->GetString(),1);
    
    if(fTMClusterRejected)
    {
      if(ClustTrackMatching(coi)){
        FillTHnSparse(coi,vecCOI, 5.5);
        continue;
      }
    }
    if(fQA) fPtaftTM->Fill(vecCOI.Pt());
    if(coi->E()>=5. && coi->E()<70. && fQA)
      fNLM->Fill(nlm,coi->E());
    
    if(!CheckBoundaries(vecCOI)){
      FillTHnSparse(coi,vecCOI, 6.5);
      continue;
    }
    
    if(fQA){
      fPtaftFC->Fill(vecCOI.Pt());
    }
    FillTHnSparse(coi,vecCOI, 7.5);
  
    if(vecCOI.Pt()<5.) continue;
    
  }
    
  return kTRUE;
}

  //____________________________________________________________________
void  AliAnalysisTaskEMCALClusterTurnOn::FillTHnSparse(AliVCluster *coi,TLorentzVector vecCOI, Double_t RejectedAt){

  if(fThn){
    const Int_t ndims =   fNDimensions;
    const Int_t ndimsClus = fClusDimensions;
    Double_t outputValues[ndims];
    Double_t outputValues_clus[ndimsClus];
    AliEMCALGeometry* geom = AliEMCALGeometry::GetInstance();
    Int_t nSupMod, nModule, nIphi, nIeta, iphi, ieta;
    Int_t c_eta = 0;
    Int_t c_phi = 0;
//    outputValues[0] = coi->E();
//    outputValues[1] = RejectedAt;
//    outputValues_clus[0] = vecCOI.Et();
//    outputValues_clus[1] = RejectedAt;
//    outputValues_clus[2] = vecCOI.Eta();
//    outputValues_clus[3] = vecCOI.Phi();
//    fOutTHnS_Clust->Fill(outputValues_clus);
    
    AliVCaloCells * fCaloCells =InputEvent()->GetEMCALCells();
    
    Int_t cellnumber = coi->GetNCells();
    Int_t IDs[cellnumber];
    Float_t energy = 0;
    Int_t FastOrIndex = 0;
    for (Int_t cellcounter = 0; cellcounter<cellnumber; cellcounter++)
    {
      IDs[cellcounter] = coi->GetCellsAbsId()[cellcounter];        
      Int_t cellID = IDs[cellcounter];
      geom->GetFastORIndexFromCellIndex(cellID,FastOrIndex);
      hFastOrIndex->Fill(FastOrIndex,vecCOI.Et());
      geom->GetCellIndex(IDs[cellcounter],nSupMod, nModule, nIphi, nIeta);
      geom->GetCellPhiEtaIndexInSModule(nSupMod, nModule, nIphi, nIeta, iphi, ieta);
      c_eta = 0; 
      if(!(nSupMod%2 == 0)) c_eta = 1;
      c_phi = nSupMod/2;
      outputValues[3] = ieta + c_eta*48;
      outputValues[4] = iphi + c_phi*24; 
//      cout << "SuperModul Nummer: " << nSupMod << " eta: " << outputValues[3] << " phi: " << outputValues[4] << endl;
      energy = fCaloCells->GetCellAmplitude(IDs[cellcounter]);
      outputValues[2] = energy;
      fOutputTHnS -> Fill(outputValues);
      
    }
  }
}      
  //___________________________________________________________________________________
Bool_t AliAnalysisTaskEMCALClusterTurnOn::ClustTrackMatching(AliVCluster *clust) {
    // Check if the cluster match to a track
  
  AliTrackContainer* tracks = GetTrackContainer(0);
  AliVTrack* mt = 0;
  TLorentzVector vecClust;
  clust->GetMomentum(vecClust,fVertex);
  
  Int_t nbMObj = clust -> GetNTracksMatched();
  if(tracks->GetTrackFilterType()!=AliEmcalTrackSelection::kTPCOnlyTracks)  AliError(Form("NO TPC only tracks"));
  
  Double_t distCT=0.;
  
  if (nbMObj == 0) return kFALSE;
  
  for(Int_t i=0;i<nbMObj;i++){
    
    if (fIsEsd) {
      Int_t imt = clust->GetTrackMatchedIndex(0);
      if (imt >= 0) mt = static_cast<AliVTrack*>(tracks->GetAcceptParticle(imt));
    }
    else {
      mt = static_cast<AliVTrack*>(clust->GetTrackMatched(i));
      UInt_t rejectionReason = 0;
      if (!tracks->AcceptParticle(mt, rejectionReason)) mt = 0;
    }
      //  Int_t imt = partC->GetMatchedObjId(i);
    
    if(!mt) continue;
    
      //    printf("Cluster ID %d matched with track ID %d with pT %.3f",clust->GetID(),mt->GetID(),mt->Pt());
    
    
    Double_t deta = 999;
    Double_t dphi = 999;
    
    Double_t veta = mt->GetTrackEtaOnEMCal();
    Double_t vphi = mt->GetTrackPhiOnEMCal();
    
    Float_t pos[3] = {0};
    clust->GetPosition(pos);
    TVector3 cpos(pos);
    Double_t ceta     = cpos.Eta();
    Double_t cphi     = cpos.Phi();
    deta=veta-ceta;
    dphi=TVector2::Phi_mpi_pi(vphi-cphi);
      //    printf("distant deta %.3f and dphi %.3f from the cluster",deta, dphi);
    distCT=TMath::Sqrt(deta*deta+dphi*dphi);
    
    if(TMath::Abs(dphi)<fdphicut && TMath::Abs(deta)<fdetacut){
      return kTRUE;
    }
  }
  return kFALSE;
}

  //_____________________________________________________________________________________________
Int_t AliAnalysisTaskEMCALClusterTurnOn::GetNLM(AliVCluster *coi, AliVCaloCells* cells){
    // find the number of local maxima of a cluster adapted from AliCalorimeterUtils
  
  const Int_t   nc = coi->GetNCells();
  
  Int_t   absIdList[nc];
  Float_t maxEList[nc];
  
  Int_t nMax = GetNLM(coi, cells, absIdList, maxEList);
  
  return nMax;
}

  //_____________________________________________________________________________________________________________________________
Int_t AliAnalysisTaskEMCALClusterTurnOn::GetNLM(AliVCluster* coi, AliVCaloCells* cells, Int_t *absIdList, Float_t *maxEList) {
    // find the cluster number of local maxima adapted from AliCalorimeterUtils
  
  Int_t iDigitN = 0 ;
  Int_t iDigit  = 0 ;
  Int_t absId1 = -1 ;
  Int_t absId2 = -1 ;
  const Int_t nCells = coi->GetNCells();
  
    // Printf("Cluster Energy Before Recalculation: %.4f",coi->E());
  Float_t eCluster = RecalEnClust(coi, cells);// recalculate cluster energy, avoid non lin correction.
  Float_t localMaxCutE = 0.1;
  Float_t locMaxCutEDiff = 0.0;
  
  Float_t emax  = 0;
  Int_t   idmax =-1;
  for(iDigit = 0; iDigit < nCells ; iDigit++)
  {
    absIdList[iDigit] = coi->GetCellsAbsId()[iDigit]  ;
    Float_t en = cells->GetCellAmplitude(absIdList[iDigit]);
      //    Printf("Cell Energy Before Recalculation: %.4f",en);
    RecalAmpCell(en,absIdList[iDigit]);
    
    if( en > emax )
    {
      emax  = en ;
      idmax = absIdList[iDigit] ;
    }
  }
  for(iDigit = 0 ; iDigit < nCells; iDigit++)
  {
    if( absIdList[iDigit] >= 0 )
    {
      absId1 = coi->GetCellsAbsId()[iDigit];
      
      Float_t en1 = cells->GetCellAmplitude(absId1);
        //      Printf("Cell-1 Energy Before Recalculation: %.4f",en1);
      RecalAmpCell(en1,absId1);
      
      
      for(iDigitN = 0; iDigitN < nCells; iDigitN++)
      {
        absId2 = coi->GetCellsAbsId()[iDigitN] ;
        
        if(absId2==-1 || absId2==absId1) continue;
        
          //printf("\t %d : absIDj %d\n",iDigitN, absId2);
        
        Float_t en2 = cells->GetCellAmplitude(absId2);
          //        Printf("Cell-2 Energy Before Recalculation: %.4f",en2);
        RecalAmpCell(en2,absId2);
        
        
        if ( AreNeighbours(absId1, absId2) )
        {
            // printf("\t \t Neighbours \n");
          if ( en1 > en2 )
          {
            absIdList[iDigitN] = -1 ;
              //printf("\t \t indexN %d not local max\n",iDigitN);
              // but may be digit too is not local max ?
            if(en1 < en2 + locMaxCutEDiff) {
                //printf("\t \t index %d not local max cause locMaxCutEDiff\n",iDigit);
              absIdList[iDigit] = -1 ;
            }
          }
          else
          {
            absIdList[iDigit] = -1 ;
              //printf("\t \t index %d not local max\n",iDigitN);
              // but may be digitN too is not local max ?
            if(en1 > en2 - locMaxCutEDiff)
            {
              absIdList[iDigitN] = -1 ;
                //printf("\t \t indexN %d not local max cause locMaxCutEDiff\n",iDigit);
            }
          }
        } // if Are neighbours
          //else printf("\t \t NOT Neighbours \n");
      } // while digitN
    } // slot not empty
  } // while digit
  
  iDigitN = 0 ;
  for(iDigit = 0; iDigit < nCells; iDigit++)
  {
    if( absIdList[iDigit] >= 0 )
    {
      absIdList[iDigitN] = absIdList[iDigit] ;
      
      Float_t en = cells->GetCellAmplitude(absIdList[iDigit]);
        // Printf("NlocMax Cell Energy Before Recalculation: %.4f",en);
      RecalAmpCell(en,absIdList[iDigit]);
        // Printf("NlocMax Cell Energy After Recalculation: %.4f",en);
      
        //      if(fMCECellClusFracCorrOn)
        //        en*=GetMCECellClusFracCorrection(en,eCluster)/simuTotWeight;
      
      if(en < localMaxCutE) continue; // Maxima only with seed energy at least
      
      maxEList[iDigitN] = en ;
      
        //printf("Local max %d, id %d, en %f\n", iDigit,absIdList[iDigitN],en);
      iDigitN++ ;
    }
  }
  
  if ( iDigitN == 0 ){
    AliDebug(1,Form("No local maxima found, assign highest energy cell as maxima, id %d, en cell %2.2f, en cluster %2.2f",
                    idmax,emax,coi->E()));
    iDigitN      = 1     ;
    maxEList[0]  = emax  ;
    absIdList[0] = idmax ;
  }
  
  AliDebug(1,Form("In coi E %2.2f (wth non lin. %2.2f), M02 %2.2f, M20 %2.2f, N maxima %d",
                  coi->E(),eCluster, coi->GetM02(),coi->GetM20(), iDigitN));
  return iDigitN ;
}

  //__________________________________________________________________________________________
Bool_t AliAnalysisTaskEMCALClusterTurnOn::AreNeighbours(Int_t absId1, Int_t absId2 ) const
{
    // check if two cells are neighbour (adapted from AliCalorimeterUtils)
  
  Bool_t areNeighbours = kFALSE ;
  
  Int_t iSupMod1 = -1, iTower1 = -1, iIphi1 = -1, iIeta1 = -1, iphi1 = -1, ieta1 = -1;
  Int_t iSupMod2 = -1, iTower2 = -1, iIphi2 = -1, iIeta2 = -1, iphi2 = -1, ieta2 = -1;
  
  Int_t phidiff =  0, etadiff =  0;
  
    //first cell
  fGeom->GetCellIndex(absId1,iSupMod1,iTower1,iIphi1,iIeta1);
  fGeom->GetCellPhiEtaIndexInSModule(iSupMod1,iTower1,iIphi1, iIeta1,iphi1,ieta1);
  
    // second cell
  fGeom->GetCellIndex(absId2,iSupMod2,iTower2,iIphi2,iIeta2);
  fGeom->GetCellPhiEtaIndexInSModule(iSupMod2,iTower2,iIphi2, iIeta2,iphi2,ieta2);
  
  
  if(iSupMod1!=iSupMod2){
      // In case of a shared cluster, index of SM in C side, columns start at 48 and ends at 48*2-1
      // C Side impair SM, nSupMod%2=1; A side pair SM nSupMod%2=0
    if(iSupMod1%2) ieta1+=AliEMCALGeoParams::fgkEMCALCols;
    else           ieta2+=AliEMCALGeoParams::fgkEMCALCols;
  }
  
  phidiff = TMath::Abs( iphi1 - iphi2 ) ;
  etadiff = TMath::Abs( ieta1 - ieta2 ) ;
  
    //if (( coldiff <= 1 )  && ( rowdiff <= 1 ) && (coldiff + rowdiff > 0))
  if ((etadiff + phidiff == 1 ))
    areNeighbours = kTRUE ;
  
  return areNeighbours;
}
  //_____________________________________________________________________________________________
  /// Recalculate cell energy if recalibration factor.
  //_____________________________________________________________________________________________
void AliAnalysisTaskEMCALClusterTurnOn::RecalAmpCell(Float_t & amp, Int_t id) const
{
  Int_t iSupMod = -1, iTower = -1, iIphi = -1, iIeta = -1, iphi = -1, ieta = -1;
  Float_t ampold=amp;
  fGeom->GetCellIndex(id,iSupMod,iTower,iIphi,iIeta);
  fGeom->GetCellPhiEtaIndexInSModule(iSupMod,iTower,iIphi, iIeta,iphi,ieta);
  
  amp *= fEMCALRecoUtils->GetEMCALChannelRecalibrationFactor(iSupMod,ieta,iphi);
    // if(ampold!=amp)
    //  Printf("Inside cell id %d\t\tOld Amp %.4f and recalculated amplitude: %.4f",id, ampold, amp);
  
}

  //__________________________________________________________________________
  /// Recalibrate the cluster energy, considering the recalibration map and the energy of the cells that compose the cluster.
  //__________________________________________________________________________
Float_t AliAnalysisTaskEMCALClusterTurnOn::RecalEnClust(AliVCluster * coi,
                                                          AliVCaloCells * cells)
{
    //Printf("Inside clust Recal");
    // Initialize some used variables
  Float_t frac  = 0., energy = 0.;
  
  if(cells)
  {
      //Get the cluster number of cells and list of absId, check what kind of cluster do we have.
    
    UShort_t * index    = coi->GetCellsAbsId() ;
    Double_t * fraction = coi->GetCellsAmplitudeFraction() ;
    
    Int_t ncells     = coi->GetNCells();
    
    
      // Loop on the cells, get the cell amplitude and recalibration factor, multiply and and to the new energy
    for(Int_t icell = 0; icell < ncells; icell++)
    {
      Int_t absId = index[icell];
      
      frac =  fraction[icell];
      if(frac < 1e-3) frac = 1; //in case of EMCAL, this is set as 0, not used.
      
      Float_t amp = cells->GetCellAmplitude(absId);
      RecalAmpCell(amp, absId);
      
        //Printf("Recalibrate cell: EMCAL, cell fraction %f, cell energy: before cal %f; after cal %f",frac,cells->GetCellAmplitude(absId),amp);
      
      energy += amp*frac;
    }
    
    AliDebug(1,Form("Energy before %f, after %f",coi->E(),energy));
    
  } // cells available
  else
  {
    AliFatal("Cells pointer does not exist!");
  }
    //Printf("recalculated energy: %.4f",energy);
  return energy;
}

Bool_t AliAnalysisTaskEMCALClusterTurnOn::CheckBoundaries(TLorentzVector vecCOI){
    // Check if the cone around the considered cluster is in EMCAL acceptance
    //AliInfo("Inside CheckBoundaries\n");
  
  Double_t minPhiBound= 0. , minEtaBound= 0., maxPhiBound= 0., maxEtaBound= 0.;
  Bool_t isINBoundaries;
  
    minEtaBound = -0.67+fIsoConeRadius; // ""
    maxEtaBound = 0.67-fIsoConeRadius; // ""
    
      minPhiBound = 1.798;
      maxPhiBound = 2.740; // normally 110° but shorter cut to avoid EMCAL border
  
  if(vecCOI.Eta() > maxEtaBound || vecCOI.Eta() < minEtaBound || vecCOI.Phi() > maxPhiBound || vecCOI.Phi() <minPhiBound)
    isINBoundaries=kFALSE;
  else
    isINBoundaries=kTRUE;
  
  return isINBoundaries;
}
