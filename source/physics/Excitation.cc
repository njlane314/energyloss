#include "Excitation.hh"

Excitation::Excitation() {}

Excitation::~Excitation() {}

std::pair<ElectronCloud, PhotonRadiant> Excitation::create_excitation(MaterialParameters* material_properties, EnergyDeposit* energy_deposit)
: material_properties_(material_properties) {
    Relaxation relaxation_generator_(material_properties);
    Ejection ejection_generator_(material_properties);

    double expected_ionisations = energy_deposit.visible / material_properties_.loss_per_ionisation;

    if (expected_ionisations > 10) {
        double resolution = material_properties.fano_factor * std::sqrt(expected_ionisations);
        int num_ionisations = random_.Gaus(expected_ionisations, resolution);
    } else {
        int num_ionisations = random_.Poisson(expected_ionisations);
    }

    double excitation_energy = energy_deposit.visible - num_ionisations * (material_properties_.ionisation_threshold + material_properties_.thermal_loss);
    int num_excitations = excitation_energy / material_properties_.excitation_threshold;

    std::tie(num_thermal_electrons, num_optical_photons) = Recombination::run_recombination(num_ionisations, num_excitations, energy_deposit.linear_transfer);

    return std::make_pair(create_electron_cloud(num_thermal_electrons, energy_deposit.position), create_photon_radiant(num_optical_photons, energy_deposit.position));
}

PhotonRadiant Excitation::create_photon_radiant(int radiant_size, const std::vector<doubble>& position) {
    PhotonRadiant photon_radiant_;

    photon_radiant_.position_ = position;
    for (int i = 0; i < radiant_size; i++) {
        photon_radiant_.photons_.push_back(relaxation_generator_.create_photon());
    }

    return photon_radiant_;
}

ElectronCloud Excitation::create_electron_cloud(int cloud_size, const std::vector<doubble>& position) {
    Ejection ejection_generator();
    ElectronCloud electron_cloud_;

    electron_cloud_.position_ = position;
    for (int i = 0; i < cloud_size; i++) {
        electron_cloud_.electrons_.push_back(ejection_generator_.create_electron());
    }

    return electron_cloud_;
}