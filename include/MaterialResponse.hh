#ifndef MATERIAL_RESPONSE_HH
#define MATERIAL_RESPONSE_HH

class Excitation;

class Scintillation;
class Ionisation;

#include "Scintillation.hh"
#include "Ionisation.hh"

#include <string>
#include <vector>

class G4Step;

struct MaterialParameters {
    double loss_per_ionisation;
    double ionisation_threshold;
    double excitation_threshold;
    double thermal_loss;

    double fano_factor;
    double excitation_ratio;         

    double singlet_to_triplet;
    double singlet_lifetime;
    double triplet_lifetime;
};

struct EnergyDeposit {
    double visible;
    double linear_transfer;
    std::string particle_type;
    std::vector<double> position;
    double time;
};

class MaterialResponse {
public:
    static MaterialResponse* instance_;
    std::string material_;
    MaterialParameters material_properties_;

    MaterialResponse(std::string material);
    ~MaterialResponse();

    static MaterialResponse* Instance(std::string material);
    MaterialParameters initialise_parameters(std::string material);
    void process_step(G4Step* step);
    void create_response(const EnergyDeposit* energy_deposit);

    Scintillation* scintillation_;
    Ionisation* ionisation_;
};

#endif // MATERIAL_RESPONSE_HH

