#include "emulatorBLDC.hpp"

using namespace std;

int main()
{
    int numberOfSamples = 280;
    int numberOfSamplesInPhase = 20;
    int noiseProbabilityInPercent = 50;
    int startPhase = 4;

    EmulatorBLDC bldc = EmulatorBLDC(numberOfSamples, numberOfSamplesInPhase);

    bldc.setPhaseStates(startPhase, noiseProbabilityInPercent);

    bldc.BEMFFilter();

    bldc.printAll();
}