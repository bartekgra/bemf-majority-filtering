#include "emulatorBLDC.hpp"

using namespace std;

int main()
{
    int numberOfSamples = 180;
    int numberOfSamplesInPhase = 30;
    int noiseProbabilityInPercent = 20;
    int startPhase = 4;

    EmulatorBLDC bldc = EmulatorBLDC(numberOfSamples, numberOfSamplesInPhase);

    bldc.setPhaseStates(startPhase, noiseProbabilityInPercent);

    bldc.BEMFFilter();

    bldc.printAll();
}