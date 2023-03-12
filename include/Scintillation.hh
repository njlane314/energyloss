#ifndef SCINTILLATION_HH
#define SCINTILLATION_HH

#include "OpticalPhoton.hh"
#include "Relaxation.hh"

#include <iostream>
#include <vector>

struct PhotonRadiant {
    std::vector<OpticalPhoton> photons_;
    std::vector<double> position_;
};

class Scintillation {

public:
    Scintillation();

    void add_radiant(int radiant_size, const std::vector<double>& position);
    void add_photon(OpticalPhoton a_photon);
    
    const std::vector<PhotonRadiant>& get_scintillation() const;
    const PhotonRadiant& get_current_radiant() const;
    std::vector<double> get_emission_times() const;
    std::vector<double> get_radiant_positions() const;

    void print_scintillation() const;

private:
    std::vector<PhotonRadiant> scintillation_;
    PhotonRadiant current_radiant_;
};

#endif
