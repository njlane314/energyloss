#ifndef IONISATION_HH
#define IONISATION_HH

#include <vector>
#include <iostream>
#include "ThermalElectron.hh"

struct ElectronCloud {
    std::vector<ThermalElectron> electrons_;
    std::vector<double> position_;
};

class Ionisation {
public:
    Ionisation();

    void add_cloud(int cloud_size, const std::vector<double>& position);

    void add_electron(ThermalElectron anElectron);

    const std::vector<ElectronCloud> get_ionisation() const;

    void print_ionisation() const;

    std::vector<double> get_cloud_positions() const;

private:
    std::vector<ElectronCloud> ionisation_;
    ElectronCloud this_cloud_;
};

#endif // IONISATION_HH