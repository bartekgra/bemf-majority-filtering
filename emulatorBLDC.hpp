#include <iostream>
#include <list>
#include <math.h>

using namespace std;


class EmulatorBLDC{
    public:
    EmulatorBLDC(int numberOfSamples, int numberOfSamplesInPhase);

    void printPhases();

    void printAll();

    void setPhaseStates(int startPhase, int noiseProbInPercent);

    void BEMFFilter(void);

    private:
    const int numberOfPartPhase = 6;
    const static int numberOfPhases = 3;

    bool** bemfSignal;
    unsigned int* bemfSignalsABC;
    int numberOfSamples;
    int numberOfSamplesInPhase;
    unsigned int adcBackEMFFilter;
    unsigned int* adcBackEMFFilterArray;
    int commState;
    int* commStateArray;
    bool* logicOutputArra;

    list<int> zeroCrossingPointer;

    int noiseFlag[numberOfPhases] = {0, 0, 0};

    void PreCommState();

    void SetPhaseStateWithNoise(bool* phase, bool state, int noiseProbability);

    void SetPhaseStateWithNoise(bool* phase, int phaseIndex, bool state, int noiseProbability);

    const unsigned int ADC_MASK[8] = {0x0000, 0x0002, 0x0001, 0x0004,
        0x0002, 0x0001, 0x0004, 0x000};

    const unsigned int ADC_XOR[8] = {0x0000, 0x0000, 0xFFFF, 0x0000,
        0xFFFF, 0x0000, 0xFFFF, 0x0000};

    const unsigned char ADC_BEMF_FILTER[64]=
        {0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,
        0x18,0x1A,0x1C,0x1E,
        0x20,0x22,0x24,0x26,0x28,0x2A,0x2C,0x2E,0x01,0x01,0x01,0x36,
        0x01,0x3A,0x3C,0x3E,
        0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x01,0x01,0x01,0x16,
        0x01,0x1A,0x1C,0x1E,
        0x01,0x01,0x01,0x26,0x01,0x2A,0x2C,0x2E,0x01,0x01,0x01,0x36,
        0x01,0x3A,0x3C,0x3E};

};