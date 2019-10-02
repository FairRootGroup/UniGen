
{
  // Create libraries
  gSystem->Load("libUniGen");
  gSystem->Load("libTasks");

  // Create manager
  UManager *mgr = new UManager();

  // Set in/out file names
  mgr->SetInputFile("epos.root");
  mgr->SetOutputFile("epos_cm.root");

  // Create and add task
  UTaskLab2Cm *task = new UTaskLab2Cm("LAB2CM");
  mgr->AddTask(task);

  // Initialisation
  mgr->Init();

  // Run
  mgr->Run();
}

