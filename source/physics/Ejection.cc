#include "Ejection.hh"

#include "ThermalElectron.hh

Ejection::Ejection() {}

Ejection::~Ejection() {}

ThermalElectron Ejection::create_electron() {
    ThermalElectron an_electron;
    an_electron.set_energy(0);

    return an_electron;
}