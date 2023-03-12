#ifndef EXCITATION_HH
#define EXCITATION_HH

#include <utility> 
#include <vector>

#include "Recombination.hh"

#include "Ejection.hh"
#include "Relaxation.hh"

#include "Ionisation.hh"
#include "Scintillation.hh"

#include <random>

class MaterialResponse;

class ElectronCloud;
class PhotonRadiant;

class Excitation {
public:
    Excitation(MaterialResponse::MaterialParameters material_properties);
    ~Excitation();
    std::pair<ElectronCloud, PhotonRadiant> create_excitation(MaterialResponse::MaterialParameters material_properties, MaterialResponse::EnergyDeposit energy_deposit);

private:
    MaterialResponse::MaterialParameters material_properties_;
    std::mt19937 random_;

    PhotonRadiant create_photon_radiant(Relaxation* relaxation_generator, MaterialResponse::EnergyDeposit energy_deposit, int radiant_size);
    ElectronCloud create_electron_cloud(Ejection* ejection_generator, MaterialResponse::EnergyDeposit energy_deposit, int cloud_size);
};

#endif  // EXCITATION_HH

