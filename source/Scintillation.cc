#include "Scintillation.hh"

Scintillation::Scintillation() : scintillation_() {}

void Scintillation::add_radiant(int radiant_size, const std::vector<double>& position) {
    Relaxation relaxation_generator;
    PhotonRadiant current_radiant;

    current_radiant.position_ = position;
    for (int i = 0; i < radiant_size; i++) {
        add_photon(relaxation_generator.create_photon());
    }

    scintillation_.push_back(current_radiant);
}

void Scintillation::add_photon(OpticalPhoton a_photon) {
    current_radiant_.photons_.push_back(a_photon);
}

const std::vector<PhotonRadiant>& Scintillation::get_scintillation() const {
    return scintillation_;
}

const PhotonRadiant& Scintillation::get_current_radiant() const {
    return current_radiant_;
}

std::vector<double> Scintillation::get_emission_times() const {
    std::vector<double> emission_times;
    for (const auto& radiant : scintillation_) {
        for (const auto& photon : radiant.photons_) {
            emission_times.push_back(photon.get_time());
        }
    }
    return emission_times;
}

std::vector<double> Scintillation::get_radiant_positions() const {
    std::vector<double> radiant_positions;
    for (const auto& radiant : scintillation_) {
        for (const auto& pos : radiant.position_) {
            radiant_positions.push_back(pos);
        }
    }
    return radiant_positions;
}

void Scintillation::print_scintillation() const {
    std::cout << "Scintillation radiants:" << std::endl;
    for (const auto& radiant : scintillation_) {
        std::cout << "  Position: (";
        for (const auto& p : radiant.position_) {
            std::cout << p << ", ";
        }
        std::cout << "), photons: " << radiant.photons_.size() << std::endl;
    }
}