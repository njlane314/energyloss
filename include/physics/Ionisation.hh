#include <vector>
#include <iostream>

#include "ThermalElectron.hh"

struct ElectronCloud {
    std::vector<Electron> electrons_;
    std::vector<double> position_;
};

class Ionisation {

public:
    Ionisation() : ionisation_() {};

    void add_cloud(int cloud_size, const std::vector<double>& position) {
        ElectronCloud this_cloud_;

        this_cloud_.position_ = position;
        for (int i = 0; i < cloud_size; i++) {
            add_electron();
        }

        ionisation_.push_back(this_cloud_);
    }

    void add_electron(Electron anElectron) { this_cloud_.electrons_.push_back(anElectron); };
    
    const std::vector<ElectronCloud> get_ionisation() const { return ionisation_; }

    void print_ionisation() const {
        std::cout << "ionisation clouds:" << std::endl;
        for (const auto& cloud : ionisation_) {
            std::cout << "  Position: (";
            for (const auto& p : cloud.position_) {
                std::cout << p << ", ";
            }
            std::cout << "), electrons: " << cloud.electrons_.size() << std::endl;
        }
    }

    std::vector<double> get_cloud_positions() const {
        std::vector<double> cloud_positions;
        for (const auto& cloud : ionisation_) {
            for (const auto& pos : cloud.position_) {
                cloud_positions.push_back(pos);
            }
        }
        return cloud_positions;
    }

private:
    std::vector<ElectronCloud> ionisation_;
    ElectronCloud this_cloud_;
};
