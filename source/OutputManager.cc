#include "OutputManager.hh"

OutputManager* OutputManager::instance_ = nullptr;

OutputManager::OutputManager() 
: filename_("an_output.root") {
    output_directory_ = new G4UIdirectory("/output/");
    output_directory_->SetGuidance("Output control commands");

    set_filename_cmd_ = new G4UIcmdWithAString("/output/filename", this);
    set_filename_cmd_->SetGuidance("Sets the output filename");
    set_filename_cmd_->SetParameterName("Filename", false, false);
}

OutputManager::~OutputManager() {
    delete set_filename_cmd_;
    delete output_directory_;
}

void OutputManager::SetNewValue(G4UIcommand* cmd, G4String new_value) {
    if (cmd == set_filename_cmd_) {
        filename_ = new_value;
    }

    CreateFile();
}

OutputManager* OutputManager::Instance() {
    if (!instance_) {
        instance_ = new OutputManager();
    }
    
    return instance_;
}

void OutputManager::CreateFile() {
    file_ = new TFile(filename_.c_str(), "RECREATE");

    meta_tree_ = new TTree("meta_tree", "Meta tree");
    meta_tree_->Branch("detector_width", &detector_width, "detector_width/D");
    meta_tree_->Branch("detector_height", &detector_height, "detector_height/D");
    meta_tree_->Branch("detector_depth", &detector_depth, "detector_depth/D");

    event_tree_ = new TTree("event_tree", "Event tree");
    event_tree_->Branch("track_count", &track_count, "track_count/I");
    event_tree_->Branch("step_count", &step_count, "step_count/I");

    track_tree_ = new TTree("track_tree", "Track tree");
    track_tree_->Branch("parent_track_id", &parent_track_id, "parent_track_id/I");
    track_tree_->Branch("track_particle", &track_particle, "track_particle/I");
    track_tree_->Branch("track_position_x", &track_position_x, "track_position_x/D");
    track_tree_->Branch("track_position_y", &track_position_y, "track_position_y/D");
    track_tree_->Branch("track_position_z", &track_position_z, "track_position_z/D");
    track_tree_->Branch("track_momentum_x", &track_momentum_x, "track_momentum_x/D");
    track_tree_->Branch("track_momentum_y", &track_momentum_y, "track_momentum_y/D");
    track_tree_->Branch("track_momentum_z", &track_momentum_z, "track_momentum_z/D");
    track_tree_->Branch("track_time", &track_time, "track_time/D");
    track_tree_->Branch("track_total_energy", &track_total_energy, "track_total_energy/D");
    track_tree_->Branch("track_kinetic_energy", &track_kinetic_energy, "track_kinetic_energy/D");
    track_tree_->Branch("track_length", &track_length, "track_length/D");

    step_tree_ = new TTree("step_tree", "Step tree");
    step_tree_->Branch("step_track_id", &step_track_id, "step_track_id/I");
    step_tree_->Branch("step_length", &step_length, "step_length/D");
    step_tree_->Branch("step_energy_deposit", &step_energy_deposit, "step_energy_deposit/D");
    step_tree_->Branch("step_non_ionizing_energy_deposit", &step_non_ionizing_energy_deposit, "step_non_ionizing_energy_deposit/D");
    step_tree_->Branch("step_position_x", &step_position_x, "step_position_x/D");
    step_tree_->Branch("step_position_y", &step_position_y, "step_position_y/D");
    step_tree_->Branch("step_position_z", &step_position_z, "step_position_z/D");
    step_tree_->Branch("step_momentum_x", &step_momentum_x, "step_momentum_x/D");
    step_tree_->Branch("step_momentum_y", &step_momentum_y, "step_momentum_y/D");
    step_tree_->Branch("step_momentum_z", &step_momentum_z, "step_momentum_z/D");
    step_tree_->Branch("step_time", &step_time, "step_time/D");
    step_tree_->Branch("step_total_energy", &step_total_energy, "step_total_energy/D");
    step_tree_->Branch("step_kinetic_energy", &step_kinetic_energy, "step_kinetic_energy/D");
}

void OutputManager::CloseFile() {
    if (!file_) {
        return;
    }
    else {
        meta_tree_->Write();
        event_tree_->Write();
        track_tree_->Write();
        step_tree_->Write();

        file_->Close();
        std::cout << "-- Output done" << std::endl;
        delete file_;
    }
}

void OutputManager::RecordEntry(const G4Run* run) {
    if (!detector_solid_) {
		G4LogicalVolume* detector_logical = G4LogicalVolumeStore::GetInstance()->GetVolume("detector.logical");
		if (detector_logical) detector_solid_ = dynamic_cast<G4Box*>(detector_logical->GetSolid());
	}
    else if (detector_solid_) {
        detector_width = detector_solid_->GetXHalfLength() * 2.;
		detector_height = detector_solid_->GetYHalfLength() * 2.;
		detector_depth = detector_solid_->GetZHalfLength() * 2.;
	}
    else {
        detector_width = 0.;
        detector_height = 0.;
        detector_depth = 0.;
    }

    meta_tree_->Fill();
}

void OutputManager::RecordEntry(const G4Event* event) {
    event_tree_->Fill();

    track_count = 0;
    step_count = 0;
}

void OutputManager::RecordEntry(const G4Track* track) {
    parent_track_id = track->GetParentID();
    track_particle = track->GetParticleDefinition()->GetPDGEncoding();
    track_position_x = track->GetPosition().x();
    track_position_y = track->GetPosition().y();
    track_position_z = track->GetPosition().z();
    track_momentum_x = track->GetMomentum().x();
    track_momentum_y = track->GetMomentum().y();
    track_momentum_z = track->GetMomentum().z();
    track_time = track->GetGlobalTime();
    track_total_energy = track->GetTotalEnergy();
    track_kinetic_energy = track->GetKineticEnergy();
    track_length = track->GetTrackLength();

    track_tree_->Fill();

    track_count++;
}

void OutputManager::RecordEntry(const G4Step* step) {
    step_track_id = step->GetTrack()->GetTrackID();
    step_length = step->GetStepLength();
    step_energy_deposit = step->GetTotalEnergyDeposit();
    step_non_ionizing_energy_deposit = step->GetNonIonizingEnergyDeposit();
    step_position_x = step->GetPreStepPoint()->GetPosition().x();
    step_position_y = step->GetPreStepPoint()->GetPosition().y();
    step_position_z = step->GetPreStepPoint()->GetPosition().z();
    step_momentum_x = step->GetPreStepPoint()->GetMomentum().x();
    step_momentum_y = step->GetPreStepPoint()->GetMomentum().y();
    step_momentum_z = step->GetPreStepPoint()->GetMomentum().z();
    step_time = step->GetPreStepPoint()->GetGlobalTime();
    step_total_energy = step->GetPreStepPoint()->GetTotalEnergy();
    step_kinetic_energy = step->GetPreStepPoint()->GetKineticEnergy();
  
    step_tree_->Fill();

    step_count++;
}