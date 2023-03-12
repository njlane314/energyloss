#ifndef THERMAL_ELECTRON_HH
#define THERMAL_ELECTRON_HH

class ThermalElectron {
    
public:
    ThermalElectron() {}

    double get_energy() const { return energy_; }
    void set_energy(double energy) { energy_ = energy; }

private:
    double energy_;
};

#endif // THERMAL_ELECTRON_HH