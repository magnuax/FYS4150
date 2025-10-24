#include "arg_parser.hpp"

Args parse_args(int argc, char *argv[])
{
    Args args;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "--problem_8")
        {
            args.problem_8 = true;
        }
        else if (arg == "--problem_9")
        {
            args.problem_9 = true;
        }
        else if (arg == "--n_steps" && i + 1 < argc)
        {
            args.n_steps = std::stoi(argv[++i]);
        }
        else if (arg == "--n_particles" && i + 1 < argc)
        {
            args.n_particles = std::stoi(argv[++i]);
        }
        else if (arg == "--T" && i + 1 < argc)
        {
            args.T = std::stod(argv[++i]);
        }
        else if (arg == "--seed" && i + 1 < argc)
        {
            args.seed = std::stoi(argv[++i]);
        }
        else if (arg == "--non-interacting")
        {
            args.interacting = false;
        }
        else if (arg == "--amplitude" && i + 1 < argc)
        {
            args.amplitude = std::stod(argv[++i]);
        }
        else if (arg == "--freq_min" && i + 1 < argc)
        {
            args.freq_min = std::stod(argv[++i]);
        }
        else if (arg == "--freq_max" && i + 1 < argc)
        {
            args.freq_max = std::stod(argv[++i]);
        }
        else if (arg == "--freq_range" && i + 1 < argc)
        {
            args.freq_min = std::stod(argv[++i]);
            args.freq_max = std::stod(argv[++i]);
        }
        else if (arg == "--n_freq" && i + 1 < argc)
        {
            args.n_freq = std::stoi(argv[++i]);
        }
        else if (arg == "--help")
        {
            std::cout << "Usage:\n"
                      << "  ./program [options]\n"
                      << "\n"
                      << "Options:\n"
                      << "  --problem_8                 Run problem 8\n"
                      << "  --problem_9                 Run problem 9\n"
                      << "  --n_steps <int>             Number of time steps (default: 4000)\n"
                      << "  --T <double>                Total time (default: 50.0)\n"
                      << "  --n_particles <int>         Number of particles (default: 2)\n"
                      << "  --non-interacting           Disable particle interactions\n"
                      << "  --seed <int>                Random seed (default: 1234)\n"
                      << "  --help                      Show this help message\n"
                      << "\n"
                      << "Options for problem 9:\n"
                      << "  --amplitude <double>        Amplitude of the oscillating field (default: 0.1)\n"
                      << "  --freq_min <double>         Minimum frequency (default: 0.2)\n"
                      << "  --freq_max <double>         Maximum frequency (default: 2.5)\n"
                      << "  --n_freq <int>              Number of frequency values (default: 10)\n";
            exit(0);
        }
        else
        {
            std::cerr << "Unknown argument: " << arg << "\n";
            exit(1);
        }
    }

    return args;
}