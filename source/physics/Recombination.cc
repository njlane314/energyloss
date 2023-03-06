#include "Recombination.hh"

Recombination::Recombination() {}

Recombination::~Recombination() {}

static std::pair<double, double> Recombination::run_recombination(int ionisations, int excitations, double visible_linear_transfer, double electric_field) {
    double recombination_factor = calculate_recombination_factor(visible_linear_transfer, electric_field); 

    thermal_electrons_ = random_.Gaus(ionisations * recombination_factor, ionisations * recombination_factor * (1 - recombination_factor));
    optical_photons_ = excitations + (ionisations - thermal_electrons_);
}

double Recombination::compute_recombination_factor(double visible_linear_transfer, double electric_field)
{
    auto BirksRecomb = [&]() -> G4double {
        G4double ARecomb = 0.800;
        G4double kRecomb = 0.0486;

        return ARecomb / (1. + visible_linear_transfer * kRecomb / electric_field);
    };

    auto EscapeRecomb = [&]() -> G4double {
        auto EscapingFraction = [visible_linear_transfer]() -> G4double {
            G4double larqlChi0A = 0.00338427;
            G4double larqlChi0B = -6.57037;
            G4double larqlChi0C = 1.88418;
            G4double larqlChi0D = 0.000129379;

            return larqlChi0A / (larqlChi0B + exp(larqlChi0C + larqlChi0D * visible_linear_transfer));
        };

        auto FieldCorrection = [&]() -> G4double {
            if (visible_linear_transfer < 1) {
                return 0;
            }
            else {
                G4double larqlAlpha = 0.0372;
                G4double larqlBeta = 0.0124;

                return std::exp(-electric_field  / (larqlAlpha * std::log(visible_linear_transfer) + larqlBeta));
            }
        };

        return EscapingFraction() * FieldCorrection();
    };

    return BirksRecomb() + EscapeRecomb();
}