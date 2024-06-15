# Exam_Lib

## Overview

`Exam_Lib` is a C++ library for simulating stochastic reactions, specifically designed to model systems such
as the SEIHR (Susceptible-Exposed-Infectious-Hospitalized-Recovered) model for epidemiology and the
circadian rhythm genetic oscillator.

## Project Structure
project_root/
├── CMakeLists.txt 

├── matplotlib-cpp/

├── include/ 

│ ├── Reaction.h 

│ ├── Species.h 

│ ├── SystemState.h 

│ ├── Simulator.h 

│ ├── Vessel.h 

│ ├── Observer.h 

│ └── SymbolTable.h 

├── src/ 

│ ├── main.cpp 

│ ├── Reaction.cpp 

│ ├── Species.cpp 

│ ├── SystemState.cpp 

│ ├── Simulator.cpp 

│ ├── Vessel.cpp 

│ └── benchmark.cpp 

└── test/ 

├── test_main.cpp 

└── test_symbol_table.cpp 

## Requirements

- CMake 3.27 or higher
- C++23 compatible compiler
- Google Test library (for unit testing)
- Matplotlib-cpp (for plotting)

## Building the Project

### 1. Clone the Repository ###

```sh
git clone git@github.com:mksm20/CPP_Exam.git
cd project_root
```

### 2. Create a Build Directory ###
```sh
mkdir build
cd build
```

### 3. Generate Build Files ###
```sh
cmake ..
```

### 4. Build the Project ###
```sh
make
```

### Running the Project ###
```sh
./Exam_Lib
```

### Running Unit Tests ###
```sh
./runUnitTests
```

### Usage ###
**SEIHR Simulation Example**
Define Vessel
```c++
Vessel seihr(uint32_t N, std::shared_ptr<SystemState> state) {
    auto v = Vessel{"COVID19 SEIHR: " + std::to_string(N), state};
    const auto eps = 0.0009; // initial fraction of infectious
    const auto I0 = size_t(std::round(eps * N)); // initial infectious
    const auto E0 = size_t(std::round(eps * N * 15)); // initial exposed
    const auto S0 = N - I0 - E0; // initial susceptible
    const auto R0 = 2.4; // initial basic reproductive number
    const auto alpha = 1.0 / 5.1; // incubation rate (E -> I) ~5.1 days
    const auto gamma = 1.0 / 3.1; // recovery rate (I -> R) ~3.1 days
    const auto beta = R0 * gamma; // infection/generation rate (S+I -> E+I)
    const auto P_H = 0.9e-3; // probability of hospitalization
    const auto kappa = gamma * P_H * (1.0 - P_H); // hospitalization rate (I -> H)
    const auto tau = 1.0 / 10.12; // removal rate in hospital (H -> R) ~10.12 days
    const auto S = v.add("S", S0); // susceptible
    const auto E = v.add("E", E0); // exposed
    const auto I = v.add("I", I0); // infectious
    const auto H = v.add("H", 0); // hospitalized
    const auto R = v.add("R", 0); // removed/immune (recovered + dead)
    v.add(S >> (beta / N) >>= E); // susceptible becomes exposed
    v.add((S + I) >> (beta / N) >>= CombinedSpecies({E, I})); // infectious infects susceptible
    v.add(E >> alpha >>= I); // exposed becomes infectious
    v.add(I >> gamma >>= R); // infectious becomes removed
    v.add(I >> kappa >>= H); // infectious becomes hospitalized
    v.add(H >> tau >>= R); // hospitalized becomes removed
    return v;
}
```
Run simulation
```c++
#include "Species.h"
#include "Reaction.h"
#include "SystemState.h"
#include "Simulator.h"
#include "SymbolTable.h"
#include "Observer.h"
#include <vector>
#include <iostream>
#include <map>

int main() {
    auto vessel = seihr(1000);
    auto state = std::make_shared<sim::SystemState>();
    sim::Simulator simulator(vessel, state, 100.0);

    simulator.run();

    return 0;
}

```

**Circadian Rhythm Simulation Example**

Define Vessel
```c++
Vessel circadian_rhythm(std::shared_ptr<SystemState> state) {
    const auto alphaA = 50;
    const auto alpha_A = 500;
    const auto alphaR = 0.01;
    const auto alpha_R = 50;
    const auto betaA = 50;
    const auto betaR = 5;
    const auto gammaA = 1;
    const auto gammaR = 1;
    const auto gammaC = 2;
    const auto deltaA = 1;
    const auto deltaR = 0.2;
    const auto deltaMA = 10;
    const auto deltaMR = 0.5;
    const auto thetaA = 50;
    const auto thetaR = 100;


    auto v = Vessel{"Circadian Rhythm", state};
    const auto DA = v.add("DA", 1);
    const auto D_A = v.add("D_A", 0);
    const auto DR = v.add("DR", 1);
    const auto D_R = v.add("D_R", 0);
    const auto MA = v.add("MA", 0);
    const auto MR = v.add("MR", 0);
    const auto A = v.add("A", 0);
    const auto R = v.add("R", 0);
    const auto C = v.add("C", 0);

    v.add((A + DA) >> gammaA >>= D_A);
    v.add(D_A >> thetaA >>= DA + A);
    v.add((A + DR) >> gammaR >>= D_R);
    v.add(D_R >> thetaR >>= DR + A);
    v.add(D_A >> alpha_A >>= MA + D_A);
    v.add(DA >> alphaA >>= MA + DA);
    v.add(D_R >> alpha_R >>= MR + D_R);
    v.add(DR >> alphaR >>= MR + DR);
    v.add(MA >> betaA >>= MA + A);
    v.add(MR >> betaR >>= MR + R);
    v.add((A + R) >> gammaC >>= C);
    v.add(C >> deltaA >>= R);
    v.add(A >> deltaA >>= Species("env", 0, true)); 
    v.add(R >> deltaR >>= Species("env", 0, true));
    v.add(MA >> deltaMA >>= Species("env", 0, true));
    v.add(MR >> deltaMR >>= Species("env", 0, true));

    return v;
}
```
Run simulation
```c++
#include "Species.h"
#include "Reaction.h"
#include "SystemState.h"
#include "Simulator.h"
#include "SymbolTable.h"
#include "Observer.h"
#include <vector>
#include <iostream>
#include <map>

int main() {
    auto vessel = circadian_rhythm();
    auto state = std::make_shared<sim::SystemState>();
    sim::Simulator simulator(vessel, state, 100.0);

    simulator.run();

    return 0;
}

```

**Using the Visualizer**
```c++
    GraphVisualizer visualizer(vessel.getSpecies(), vessel.getReactions());
    visualizer.generateGraph("name.png");
```

**Using Matplotlib plt**
After running the simulations, you can use Matplotlib to plot the results over time. 
Below is an example of how to plot the results of a simulation:

```c++
namespace plt = matplotlibcpp;

void plotResults(const SystemState& systemState, const Vessel& vessel) {
    const auto& results = systemState->getTrajectory();
    const auto& timePoints = systemState->getTimePoints();

    for (const auto& species : vessel.getSpecies()) {
        plt::named_plot(species.getName(), timePoints, results.at(species.getName()));
    }
    plt::legend();
    plt::show();
```