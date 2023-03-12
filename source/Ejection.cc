#include "Ejection.hh"

Ejection::Ejection() {}

Ejection::~Ejection() {}

ThermalElectron* Ejection::create_electron() {
    auto an_electron = new ThermalElectron();
    an_electron->set_energy(0);

    return an_electron;
}