#ifndef RECOMBINATION_HH
#define RECOMBINATION_HH

#include <vector>
#include <cmath>

#include "G4SystemOfUnits.hh"

class Recombination {
public:
    Recombination();
    ~Recombination();
    static std::pair<double, double> run_recombination(int ionisations, int excitations, double visible_linear_transfer, double electric_field = 0.5 * kilovolt/cm);

private:
    static double compute_recombination_factor(double visible_linear_transfer, double electric_field = 0.5 * kilovolt/cm);
    static double thermal_electrons_;
    static double optical_photons_;
};

#endif // RECOMBINATION_HH