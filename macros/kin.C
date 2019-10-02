
{
  // Create libraries
  gSystem->Load("libUniGen");
  gSystem->Load("libTasks");

  // Create manager
  UManager *mgr = new UManager();

  // Set in/out file names
  mgr->SetInputFile("/d/cbm02/kresan/unigen/urqmd/auau/25gev/centr\
/urqmd.auau.25gev.centr.0000.root");
  mgr->SetOutputFile((TString(getenv("UNIGEN"))+"/data/kin.root").Data(),
		     kFALSE);

  // Create and add task
  UTaskKin *task = new UTaskKin("kinematics");
  mgr->AddTask(task);

  // Initialisation
  mgr->Init();

  // Run
  mgr->Run();
}

