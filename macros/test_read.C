{
  // Load library
  gSystem->Load("libUniGen");
  gSystem->Load("libTasks");

  // Create the Manager
  UManager *mgr = new UManager();

  // Open the file
  char* unigen = getenv("UNIGEN");
  TString fileName = TString(unigen)+"/data/test.root";
  mgr->SetInputFile(fileName.Data());

  UTaskRead *task = new UTaskRead("read");
  mgr->AddTask(task);

  mgr->Init();

  mgr->Run();
}

