#include "Ionisation.hh"

Ionisation::Ionisation() : ionisation_() {}

void Ionisation::add_cloud(int cloud_size, const std::vector<double>& position) {
    ElectronCloud this_cloud_;

    this_cloud_.position_ = position;
    for (int i = 0; i < cloud_size; i++) {
        add_electron(ThermalElectron());
    }

    ionisation_.push_back(this_cloud_);
}

void Ionisation::add_electron(ThermalElectron an_electron) { 
    this_cloud_.electrons_.push_back(an_electron); 
}

const std::vector<ElectronCloud> Ionisation::get_ionisation() const { 
    return ionisation_; 
}

void Ionisation::print_ionisation() const {
    std::cout << "ionisation clouds:" << std::endl;
    for (const auto& cloud : ionisation_) {
        std::cout << "  Position: (";
        for (const auto& p : cloud.position_) {
            std::cout << p << ", ";
        }
        std::cout << "), electrons: " << cloud.electrons_.size() << std::endl;
    }
}

std::vector<double> Ionisation::get_cloud_positions() const {
    std::vector<double> cloud_positions;
    for (const auto& cloud : ionisation_) {
        for (const auto& pos : cloud.position_) {
            cloud_positions.push_back(pos);
        }
    }
    return cloud_positions;
}