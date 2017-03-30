

void mcrd(const char* MCper,const char* anchper,const char* anchpass){

gROOT->LoadMacro("tpcMCValidationStandardQA.C+");
cout<<"mcrd.C: INITIALIZING TPC MC Validation"<<endl;
InitTPCMCValidation(MCper,"passMC",anchper, anchpass,0,0);
//InitTPCMCValidation("LHC15k1a1","passMC","LHC15o", "pass3_lowIR_pidfix",0,0);

Double_t cRange[4]={0.13,0.01,0.5,0.35};
Double_t cRange2[4]={0.13,0.01,0.5,0.3};
Double_t cRange5[4]={0.13,0.01,0.8,0.3};
TMultiGraph *graph=0,*lines=0;




trendingDraw->fWorkingCanvas->Clear();  
TLegend *legend2 = new TLegend(cRange2[0],cRange2[1],cRange2[2],cRange2[3],"Number of clusters: MC/Anchor"); legend2->SetBorderSize(0);
legend2->SetNColumns(2);
graph = TStatToolkit::MakeMultGraph(treeMC,"","meanTPCncl;TPC.Anchor.meanTPCncl:run","1","25;21","2;4",1,0.75,5,legend2);
TStatToolkit::DrawMultiGraph(graph,"alp");
legend2->Draw();
trendingDraw->AppendStatusPad(0.3, 0.4, 0.05);
trendingDraw->fWorkingCanvas->SaveAs("meanTPCNclMCtoAnchor.png"); 
//trendingDraw->fWorkingCanvas->Draw();


trendingDraw->fWorkingCanvas->Clear(); 
TLegend *legend = new TLegend(cRange5[0],cRange5[1],cRange5[2],cRange5[3],"Matching efficiency: MC/Anchor"); legend->SetBorderSize(0);
legend->SetNColumns(5);
graph = TStatToolkit::MakeMultGraph(treeMC,"","QA.TPC.tpcItsMatchA;QA.TPC.tpcItsMatchC;QA.ITS.EffTOTPt02;QA.ITS.EffTOTPt1;QA.ITS.EffTOTPt10;TPC.Anchor.tpcItsMatchA;TPC.Anchor.tpcItsMatchC;ITS.Anchor.EffTOTPt02;ITS.Anchor.EffTOTPt1;ITS.Anchor.EffTOTPt10:run","1","21;24;25;27;28;21;24;25;27;28","2;2;2;2;2;4;4;4;4;4",1,1.5,5,legend);
TStatToolkit::DrawMultiGraph(graph,"alp");
legend->Draw(); 
trendingDraw->AppendStatusPad(0.3, 0.4, 0.05);
trendingDraw->fWorkingCanvas->SaveAs("matchingTPC-ITSEffe.png");    
//trendingDraw->fWorkingCanvas->Draw(); 
}