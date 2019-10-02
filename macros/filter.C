
{
  // Create libraries
  gSystem->Load("libUniGen");
  gSystem->Load("libTasks");

  // Create manager
  UManager *mgr = new UManager();

  // Set in/out file names
  mgr->SetInputFile((TString(getenv("UNIGEN"))+"/data/urqmd/ug.root").Data());
  mgr->SetOutputFile("urqmd_filtered.root");

  // Create filter task
  UTaskFilter *task = new UTaskFilter("filter");
  task->AddParticle(211);
  task->AddParticle(-211);
  task->SetFilterMode(1);
  mgr->AddTask(task);

  // Initialisation
  mgr->Init();

  // Run
  mgr->Run();
}

