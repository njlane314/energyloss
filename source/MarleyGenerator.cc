#include "MarleyGenerator.hh"

MarleyGenerator::MarleyGenerator(std::string marley_source)
: source_(marley_source), delay_states_(0) {
    // https://www.sciencedirect.com/science/article/pii/S0090375217300169
    cascade_times_ = {
        { 0.0298299*MeV, 4.25*ns },
        { 0.800143*MeV, 0.26*ps },
        { 1.64364*MeV, 0.336*us },
        { 1.95907*MeV, 0.54*ps },
        { 2.010368*MeV, 0.32*ps },
    };
}

MarleyGenerator::~MarleyGenerator() {}

double MarleyGenerator::SampleDecayTime(double half_life) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<double> dist(std::log(2.0) / half_life);

    return dist(gen);
}

void MarleyGenerator::GeneratePrimaryVertex(G4Event* event) {
    marley::JSONConfig marley_config(source_);
    marley::Generator marley_generator = marley_config.create_generator();
    marley::Event marley_event = marley_generator.create_event();

    G4PrimaryVertex* primary_vertex = new G4PrimaryVertex(0,0,0,0.);

    double global_time = 0.;

    const auto& marley_cascades = marley_event.get_cascade_levels();
    const int marley_residue_pdg = marley_event.residue().pdg_code();

    int particle_idx = 0;
    int cascade_idx = 0;

    for (const auto& marley_particle : marley_event.get_final_particles()) {    
        if (particle_idx > 1 && marley_residue_pdg == 1000190400 && cascade_idx < marley_event.get_cascade_levels().size()) {
            const auto& excited_state = marley_cascades[cascade_idx]->energy();

            auto iter = cascade_times_.find(excited_state);
            if (iter != cascade_times_.end()) {
                double decay_time = SampleDecayTime(iter->second);
                primary_vertex->SetT0(global_time + decay_time);
                std::cout << "Decay time: " << decay_time << std::endl;
            }

            if (excited_state == 1.64364) {
                delay_states_++;
            }
            cascade_idx++;
        }

        G4PrimaryParticle* a_particle = new G4PrimaryParticle(marley_particle->pdg_code(), marley_particle->px(), marley_particle->py(), marley_particle->pz(), marley_particle->total_energy());
        a_particle->SetCharge(marley_particle->charge());
        
        primary_vertex->SetPrimary(a_particle);
        
        particle_idx++;
    }

    event->AddPrimaryVertex(primary_vertex);
    PrintEvent(marley_event);
}

void MarleyGenerator::PrintEvent(const marley::Event& event) {
    std::cout << "\nGenerated event data:" << std::endl;
    std::cout << "Projectile energy: " << event.projectile().total_energy() << std::endl;
    std::cout << "Initial excitation energy: " << event.Ex() << " MeV" << std::endl;
    std::cout << "Number of final particles: " << event.get_final_particles().size() << std::endl;
    std::cout << "Number of de-excitation levels: " << event.get_cascade_levels().size() << std::endl;
    std::cout << "Final particles:" << std::endl;
    
    int particle_idx = 1;
    int cascade_idx = 0;
    for (const auto& particle : event.get_final_particles()) {
        std::cout << "Particle " << particle_idx << ":" << std::endl;
        std::cout << "\tPDG code: " << particle->pdg_code() << std::endl;
        std::cout << "\tMomentum (px, py, pz): " << particle->px() << ", " << particle->py() << ", " << particle->pz() << " MeV/c" << std::endl;
        std::cout << "\tTotal energy: " << particle->total_energy() << " MeV" << std::endl;
        std::cout << "\tCharge: " << particle->charge() << std::endl;

        if (particle_idx > 1 && cascade_idx < event.get_cascade_levels().size()) {
        std::cout << "Cascade level: " << cascade_idx << ":" << std::endl;
        std::cout << "\tExcitation energy: " << event.get_cascade_levels()[cascade_idx]->energy() << " MeV" << std::endl;
        const auto& excited_state = event.get_cascade_levels()[cascade_idx]->energy();
        if (excited_state == 1.64364) {
            std::cout << "\tFound a delayed state with energy " << particle->total_energy() << " MeV" << std::endl;
        }
        cascade_idx++;
        }
        
        particle_idx++;
    }
    std::cout << "Number of delayed states: " << delay_states_ << std::endl;
    std::cout << std::endl;
}
