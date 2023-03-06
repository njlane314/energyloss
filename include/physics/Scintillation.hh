#include "Scintillation.hh"

#include <vector>
#include <iostream>

#include "OpticalPhoton.hh"

struct PhotonRadiant {
    std::vector<OpticalPhoton> photons_;
    std::vector<double> position_;
};

class Scintillation {

public:
    Scintillation() : scintillation_() {};

    void add_radiant(int radiant_size, const std::vector<doubble>& position) {
        Relaxation relaxation_generator();
        PhotonRadiant current_radiant_;

        current_radiant_.position_ = position;
        for (int i = 0; i < radiant_size; i++) {
            add_photon(relaxation_generator.create_photon());
        }

        scintillation_.push_back(current_radiant_);
    }

    void add_photon(OpticalPhoton aPhoton) { current_radiant_.photons_.push_back(aPhoton); };
    
    const std::vector<PhotonRadiant> get_scintillation() const { return scintillation_; }

    const std::PhotonRadiant get_current_radiant() const { return current_radiant_; }

    std::vector<double> get_emission_times() const {
        std::vector<double> emission_times;
        for (const auto& radiant : scintillation_) {
            for (const auto& photon : radiant.photons_) {
                emission_times.push_back(photon.get_time());
            }
        }
        return emission_times;
    }

    std::vector<double> get_radiant_positions() const {
        std::vector<double> radiant_positions;
        for (const auto& radiant : scintillation_) {
            for (const auto& pos : radiant.position_) {
                radiant_positions.push_back(pos);
            }
        }
        return radiant_positions;
    }

    void print_scintillation() const {
        std::cout << "Scintillation radiants:" << std::endl;
        for (const auto& radiant : scintillation_) {
            std::cout << "  Position: (";
            for (const auto& p : radiant.position_) {
                std::cout << p << ", ";
            }
            std::cout << "), photons: " << radiant.photons_.size() << std::endl;
        }
    }

private:
    std::vector<PhotonRadiant> scintillation_;
    PhotonRadiant current_radiant_;
}