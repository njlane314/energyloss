
class OpticalPhoton {

public:
    OpticalPhoton(double time = 0.0, double wavelength = 0.0) : time_(time), wavelength_(wavelength) {}

    double get_time() const { return time_; }
    void set_time(double time) { time_ = time; }

    double get_wavelength() const { return wavelength_; }
    void set_wavelength(double wavelength) { wavelength_ = wavelength; }

private:
    double time_;
    double wavelength_;
};