#ifndef RELAXATION_HH
#define RELAXATION_HH

class MaterialResponse;

#include "OpticalPhoton.hh"
#include "MaterialResponse.hh"

#include <random>

class Relaxation {
public:
    Relaxation(MaterialResponse::MaterialParameters material_properties);
    ~Relaxation();

    OpticalPhoton create_photon(double time);

private:
    double singlet_lifetime_;
    double triplet_lifetime_;
    double singlet_abundance_;
    double triplet_abundance_;
    
    std::mt19937 generator_;
    std::uniform_real_distribution<double> emission_distribution_;

    void sample_emission();
    double emission_probability(double t);
};

#endif // RELAXATION_HH