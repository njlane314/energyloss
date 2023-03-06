#include "SteppingAction.hh"

SteppingAction::SteppingAction() 
: output_manager_(nullptr), verbose_level_(0) {
	output_manager_ = OutputManager::Instance();
}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {
	material_response_ = MaterialResponse::Instance();
    if (verbose_level_ > 2) {
		PrintStep(step);
	}

	material_response_->process_step(step);
	output_manager_->RecordEntry(step);
}

void SteppingAction::PrintStep(const G4Step* step) {}

