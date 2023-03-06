#include "MaterialResponse.hh"

#include "Excitation.hh"
#include "TrackStructure.hh"

#include "Scintillation.hh"
#include "Ionisation.hh"

struct MaterialResponse::EnergyDeposit {
    double visible;
    double linear_transfer;
    std::string particle_type;
    std::vector<double> position;
    double time;
}

struct MaterialResponse::MaterialParameters {
    double loss_per_ionisation;
    double ionisation_threshold;
    double excitation_threshold;
    double thermal_loss;

    double fano_factor;
    double excitation_ratio;

    double singlet_to_triplet;
    double singlet_lifetime;
    double triplet_lifetime;
}

MaterialResponse* MaterialResponse::instance_ = nullptr;

MaterialResponse::MaterialResponse(std::string material = 'lAr') 
: material_(material) {
    material_properties_ = initialise_parameters(material_);

    scintillation_ = Scintillation();
    ionisation_ = Ionisation();
}

MaterialResponse::~MaterialResponse() {
    scintillation_.~Scintillation();
    ionisation_.~Ionisation();
}

MaterialResponse* MaterialResponse::Instance() {
    if (!instance_) {
        instance_ = new MaterialResponse();
    }
    
    return instance_;
}

MaterialParameters MaterialResponse::initialise_parameters(std::string material) {
    MaterialResponse material_params;
    if (material == "lAr") {
        material_params.loss_per_ionisation = 23.6 * eV;
        material_params.ionisation_threshold = 15.4 * eV;
        material_params.excitation_threshold = 12.7 * eV;
        material_params.thermal_loss = 4.3 * eV;

        material_params.fano_factor = 0.107;
        material_params.excitation_ratio = 0.21;

        material_params.singlet_to_triplet = x;
        material_params.singlet_lifetime = x;
        material_params.triplet_lifetime = x;
    } 

    return material_params;
}

void MaterialResponse::prcoess_step(G4Step* step) {
    EnergyDeposit energy_deposit;
    energy_deposit.visible = step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit();
    energy_deposit.linear_transfer = (step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit()) / step->GetStepLength();
    energy_deposit.particle_type = step->GetTrack()->GetDefinition()->GetParticleName();
    energy_deposit.position = step->GetPreStepPoint()->GetPosition();
    energy_deposit.time = step->GetPreStepPoint()->GetGlobalTime();

    create_response(energy_deposit);
}

void MaterialResponse::create_response(EnergyDeposit* energy_deposit) {
    ElectronCloud electron_cloud_; PhotonRadiant photon_radiant_;

    std::tie(electron_cloud_, photon_radiant_) = Excitation::create_excitation(MaterialParams* material_properties_, EnergyDeposit* energy_deposit);

    ionisation_.add_cloud(electron_cloud_);
    scintillation_.add_radiant(photon_radiant_);
}