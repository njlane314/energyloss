
class ThermalElectron {
    
public:
    ThermalElectron(double energy) : energy_(energy) {}

    double get_energy() const { return energy_; }
    void set_energy(double energy) { energy_ = energy; }

private:
    double energy_;
};
