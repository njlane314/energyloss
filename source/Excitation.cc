#include "Excitation.hh"

Excitation::Excitation(MaterialResponse::MaterialParameters* material_properties)
: material_properties_(material_properties), random_(std::random_device{}()) {}

Excitation::~Excitation() {}

std::pair<ElectronCloud, PhotonRadiant> Excitation::create_excitation(MaterialResponse::MaterialParameters* material_properties, MaterialResponse::EnergyDeposit* energy_deposit) {
    Relaxation relaxation_generator(material_properties);
    Ejection ejection_generator;

    double expected_ionisations = energy_deposit->visible / material_properties->loss_per_ionisation;

    int num_ionisations;
    if (expected_ionisations > 10) {
        double resolution = material_properties->fano_factor * std::sqrt(expected_ionisations);
        std::normal_distribution<double> dist(expected_ionisations, resolution);
        num_ionisations = static_cast<int>(std::round(dist(random_)));
    } else {
        std::poisson_distribution<int> dist(expected_ionisations);
        num_ionisations = dist(random_);
    }

    double excitation_energy = energy_deposit->visible - num_ionisations * (material_properties->ionisation_threshold + material_properties->thermal_loss);
    int num_excitations = excitation_energy / material_properties->excitation_threshold;

    int num_thermal_electrons, num_optical_photons;
    std::tie(num_thermal_electrons, num_optical_photons) = Recombination::run_recombination(num_ionisations, num_excitations, energy_deposit->linear_transfer);

    return std::make_pair(create_electron_cloud(&ejection_generator, energy_deposit, num_thermal_electrons), create_photon_radiant(&relaxation_generator, energy_deposit, num_optical_photons));
}

PhotonRadiant Excitation::create_photon_radiant(Relaxation* relaxation_generator, MaterialResponse::EnergyDeposit* energy_deposit, int radiant_size) {
    PhotonRadiant photon_radiant_;

    photon_radiant_.position_ = energy_deposit->position;
    for (int i = 0; i < radiant_size; i++) {
        photon_radiant_.photons_.push_back(relaxation_generator->create_photon(energy_deposit->time));
    }

    return photon_radiant_;
}

ElectronCloud Excitation::create_electron_cloud(Ejection* ejection_generator, MaterialResponse::EnergyDeposit* energy_deposit, int cloud_size) {
    ElectronCloud electron_cloud_;

    electron_cloud_.position_ = energy_deposit->position;
    for (int i = 0; i < cloud_size; i++) {
        electron_cloud_.electrons_.push_back(*ejection_generator->create_electron());
    }

    return electron_cloud_;
}