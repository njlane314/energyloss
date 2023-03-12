#include "MaterialResponse.hh"
#include "Excitation.hh"

MaterialResponse* MaterialResponse::instance_ = nullptr;

MaterialResponse::MaterialResponse(std::string material) 
: material_(material) {
    material_properties_ = initialise_parameters(material);

    scintillation_ = new Scintillation();
    ionisation_ = new Ionisation();
}

MaterialResponse::~MaterialResponse() {}

MaterialResponse* MaterialResponse::Instance(std::string material) {
    if (!instance_) {
        instance_ = new MaterialResponse(material);
    }
    
    return instance_;
}

MaterialParameters MaterialResponse::initialise_parameters(std::string material) {
    MaterialParameters material_params;
    if (material == "lAr") {
        material_params.loss_per_ionisation = 23.6 * eV;
        material_params.ionisation_threshold = 15.4 * eV;
        material_params.excitation_threshold = 12.7 * eV;
        material_params.thermal_loss = 4.3 * eV;

        material_params.fano_factor = 0.107;
        material_params.excitation_ratio = 0.21;

        material_params.singlet_lifetime = 5 * ns;
        material_params.triplet_lifetime = 2100 * ns;
    } 

    return material_params;
}

void MaterialResponse::process_step(G4Step* step) {
    EnergyDeposit energy_deposit; 
    energy_deposit.visible = step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit();
    energy_deposit.linear_transfer = (step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit()) / step->GetStepLength();
    energy_deposit.particle_type = step->GetTrack()->GetDefinition()->GetParticleName();
    energy_deposit.position = step->GetPreStepPoint()->GetPosition();
    energy_deposit.time = step->GetPreStepPoint()->GetGlobalTime();

    create_response(&energy_deposit);
}

void MaterialResponse::create_response(const EnergyDeposit* energy_deposit) {
    Excitation excitation(&material_properties_);
    std::pair<ElectronCloud, PhotonRadiant> response = excitation.create_excitation(&material_properties_, energy_deposit);
    ionisation_->add_cloud(response.first);
    scintillation_->add_radiant(response.second);
}