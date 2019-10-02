// Example function for steering the decay with UManager. The
// CGenesisDecayer is wrapped by UGenesisTaskDecay to accomplish this
// functionality.
void task(const char *filename)
{
  // Load library
  gSystem->Load("libUniGen");
  gSystem->Load("libGenesis");
  gSystem->Load("libTasksGenesis");

  // Create the Manager
  UManager *mgr = new UManager();

  // Open the file
  mgr->SetInputFile(filename);
  mgr->SetOutputFile("umanager.root");

  // Conversion from UniGen to Genesis
  UGenesisTask *ug2gen = new UGenesisTask("ug2gen", 1);
  mgr->AddTask(ug2gen);

  // Genesis decayer, back conversion
  UGenesisTaskDecay *genesisDecayer =
    new UGenesisTaskDecay(ug2gen->GetGenesisEvent());
  mgr->AddTask(genesisDecayer);

  mgr->Init();

  mgr->Run();

  mgr->CloseInputFile();
  mgr->CloseOutputFile();
}
