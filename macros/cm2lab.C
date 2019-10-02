
{
  // Create libraries
  gSystem->Load("libUniGen");
  gSystem->Load("libTasks");

  // Create manager
  UManager *mgr = new UManager();

  // Set in/out file names
  mgr->SetInputFile((TString(getenv("UNIGEN"))+"/data/urqmd/ug.root").Data());
  mgr->SetOutputFile("urqmd_lab.root");

  // Create and add task
  UTaskCm2Lab *task = new UTaskCm2Lab("CM2LAB");
  mgr->AddTask(task);

  // Initialisation
  mgr->Init();

  // Run
  mgr->Run();
}

