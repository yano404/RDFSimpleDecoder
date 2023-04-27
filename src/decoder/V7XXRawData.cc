#include "V7XXRawData.h"

V7XXRawData::V7XXRawData(Int_t nch) {
    fNCh = nch;
    fData = std::vector<V7XXChData_t>(nch);
    Clear();
}

V7XXRawData::~V7XXRawData() {
    fData.clear();
}

UShort_t V7XXRawData::GetADC(Int_t ch) {
    return fData.at(ch).ADC;
}

Bool_t V7XXRawData::IsOV(Int_t ch) {
    return fData.at(ch).OV;
}

Bool_t V7XXRawData::IsUN(Int_t ch) {
    return fData.at(ch).UN;
}

void V7XXRawData::SetChData(Int_t ch, V7XXChData_t data) {
    fData.at(ch) = data;
}

void V7XXRawData::SetOV(Int_t ch, Bool_t ov) {
    fData.at(ch).OV = ov;
}

void V7XXRawData::SetUN(Int_t ch, Bool_t un) {
    fData.at(ch).UN = un;
}

void V7XXRawData::SetADC(Int_t ch, UShort_t adc) {
    fData.at(ch).ADC = adc;
}

void V7XXRawData::Clear() {
    for (auto itr=fData.begin(); itr!=fData.end(); ++itr) {
        (*itr).UN = kFALSE;
        (*itr).OV = kFALSE;
        (*itr).ADC = 0;
    }
}
