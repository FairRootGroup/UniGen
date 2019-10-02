{
  // Load library
  gSystem->Load("libUniGen");
  gSystem->Load("libTasks");

  // Create manager
  UManager *mgr = new UManager();

  // Open output file
  char* unigen = getenv("UNIGEN");
  TString fileName = (TString(unigen)+"/data/test.root");
  mgr->SetOutputFile(fileName.Data());

  // Simple Generator
  UTaskWrite *task = new UTaskWrite("write", 1);
  mgr->AddTask(task);

  mgr->Init();

  mgr->Run(4000);
}
