#include "emulatorBLDC.hpp"

using namespace std;

EmulatorBLDC::EmulatorBLDC(int numberOfSamples, int numberOfSamplesInPhase):adcBackEMFFilter(0),commState(0){
    this->numberOfSamples = numberOfSamples;
    this->numberOfSamplesInPhase = numberOfSamplesInPhase;

    bemfSignal = new bool*[this->numberOfSamples];
    for(int i = 0; i < this->numberOfSamples; i++){
        *(bemfSignal + i) = new bool[numberOfPhases];
    }

    bemfSignalsABC = new unsigned int[this->numberOfSamples];
    adcBackEMFFilterArray = new unsigned int[this->numberOfSamples];
    commStateArray = new int[this->numberOfSamples];
    logicOutputArra = new bool[this->numberOfSamples];


    for(int samp = 0; samp < this->numberOfSamples; samp++){
        for(int i = 0; i < numberOfPhases; i++){
            bemfSignal[samp][i] = 1;
        }
    }
}

void EmulatorBLDC::printPhases(){
    for(int i = 0; i < numberOfSamples; i++){
        if(i < 10)
        cout << i << "   |  " << bemfSignal[i][0] << " " << bemfSignal[i][1] << " " << bemfSignal[i][2] << "\n"; 
        else if(i < 100)
        cout << i << "  |  " << bemfSignal[i][0] << " " << bemfSignal[i][1] << " " << bemfSignal[i][2] << "\n"; 
        else if(i < 1000)
        cout << i << " |  " << bemfSignal[i][0] << " " << bemfSignal[i][1] << " " << bemfSignal[i][2] << "\n"; 
    }
}

void EmulatorBLDC::printAll(){
    int numberOfEmptySpaces = log10(numberOfSamples);
    int tmpLog10;

    cout << "No. | Sig. ABC|Log|Com|Filter\n";

    for(int i = 0; i < numberOfSamples; i++){
        cout << i;

        i == 0 ? tmpLog10 = 0 : tmpLog10 = log10(i);
        for(int j = 0; j <= (numberOfEmptySpaces - tmpLog10); j++){
            cout << " ";
        }

        cout << "|  " << ((bemfSignalsABC[i] >> 2) & 1)  << " " << ((bemfSignalsABC[i] >> 1) & 1) << " " << (bemfSignalsABC[i] & 1) << "  | "
        << logicOutputArra[i] << " | " << commStateArray[i] << " | " << adcBackEMFFilterArray[i];

        if(zeroCrossingPointer.size() > 0 && zeroCrossingPointer.front() == i){
            zeroCrossingPointer.pop_front();
            cout << "  <- zero crossing event";
        }
        cout << "\n";
    }
}

void EmulatorBLDC::setPhaseStates(int startPhase, int noiseProbInPercent){
    bool tmpStatePhases[3];
    for(int samp = 0; samp < numberOfSamples; samp++){

        switch((samp / numberOfSamplesInPhase + startPhase) % numberOfPartPhase){
            case 5:
                tmpStatePhases[0] = 1;
                tmpStatePhases[1] = 0;
                tmpStatePhases[2] = 0;
            break;
            case 4:
                tmpStatePhases[0] = 1;
                tmpStatePhases[1] = 1;
                tmpStatePhases[2] = 0;
            break;
            case 3:
                tmpStatePhases[0] = 0;
                tmpStatePhases[1] = 1;
                tmpStatePhases[2] = 0;
            break;
            case 2:
                tmpStatePhases[0] = 0;
                tmpStatePhases[1] = 1;
                tmpStatePhases[2] = 1;
            break;
            case 1:
                tmpStatePhases[0] = 0;
                tmpStatePhases[1] = 0;
                tmpStatePhases[2] = 1;
            break;
            case 0:
                tmpStatePhases[0] = 1;
                tmpStatePhases[1] = 0;
                tmpStatePhases[2] = 1;
            break;

        }

        bemfSignalsABC[samp] = 0;
        for(int i = 0; i < 3; i++){
            SetPhaseStateWithNoise(bemfSignal[samp], i, tmpStatePhases[i], noiseProbInPercent);

            if(bemfSignal[samp][i]){
                bemfSignalsABC[samp] |= 1 << (2 - i);
            }
            
        }
    }

}

void EmulatorBLDC::BEMFFilter(void){
    for(int samp = 0; samp < numberOfSamples; samp++){
        if((bemfSignalsABC[samp] ^ ADC_XOR[commState + 1]) & ADC_MASK[commState + 1]){
            adcBackEMFFilter |= 0x01;
            logicOutputArra[samp] = true;
        }else{
            logicOutputArra[samp] = false;
        }
        
        //Majority detection filter
        adcBackEMFFilter = ADC_BEMF_FILTER[adcBackEMFFilter];
        adcBackEMFFilterArray[samp] = adcBackEMFFilter;
        if (adcBackEMFFilter & 0b00000001){
            PreCommState();
            zeroCrossingPointer.push_back(samp);
        }
        commStateArray[samp] = commState;
    }
}

void EmulatorBLDC::PreCommState(){
    commState = commState + 1;
    commState %= numberOfPartPhase;
}

void EmulatorBLDC::SetPhaseStateWithNoise(bool* phase, bool state, int noiseProbability){
    if((rand() % 100) < noiseProbability){
        *phase = !state;
    }else{
        *phase = state;
    }
}

void EmulatorBLDC::SetPhaseStateWithNoise(bool* phase, int phaseIndex, bool state, int noiseProbability){
    if((rand() % 100) < noiseProbability && noiseFlag[phaseIndex] == 0){
        *(phase + phaseIndex) = !state;
        noiseFlag[phaseIndex] = 2;
    }else{
        *(phase + phaseIndex) = state;
        if(noiseFlag[phaseIndex])
            noiseFlag[phaseIndex]--;
    }
}