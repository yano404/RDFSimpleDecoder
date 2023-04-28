#include "ModuleDecoderV7XX.h"

ModuleDecoderV7XX::ModuleDecoderV7XX(Int_t id, const char* name, Bool_t isN){
    fID = id;
    fModuleName = name;
    fIsN = isN;
    fNCh = ((isN)? 16 : 32);
    fData = new V7XXRawData(fNCh);
}

ModuleDecoderV7XX::~ModuleDecoderV7XX() {}

Int_t ModuleDecoderV7XX::Decode(char* buffer, const int &size) {
    Int_t segid;
    UChar_t geo, crate;
    UInt_t evtcount;
    Int_t channel;
    UInt_t adc;

    UInt_t *evtdata = reinterpret_cast<UInt_t*>(buffer);
    UInt_t evtsize = size / sizeof(UInt_t);

    // Get Segment ID
    segid = (evtdata[0] & 0xffff0000)>>16;

    if (segid != fID) {
        return 0;
    }

    // Decode
    Clear();
    for (UInt_t i=1; i!=evtsize; i++) {
         if (evtdata[i] & kMaskHeader) {
             geo = (evtdata[i] & kMaskGeo) >> kShiftGeo;
             crate = (evtdata[i] & kMaskCrate) >> kShiftCrate;
             fData->SetGeo(geo);
             fData->SetCrate(crate);
         } else if (evtdata[i] & kMaskEOB) {
             evtcount = (evtdata[i] & kMaskEventCounter);
             fData->SetEventCount(evtcount);
         } else {
             channel = (evtdata[i] & kMaskChannel) >> kShiftChannel;
             if (fIsN) channel /= 2;
             if (evtdata[i] & kMaskUN) fData->SetUN(channel);
             if (evtdata[i] & kMaskOV) fData->SetOV(channel);
             adc = evtdata[i] & kMaskADC;
             fData->SetADC(channel, adc);
         }
    }
    return 1;
}

void ModuleDecoderV7XX::Clear() {
    fData->Clear();
}
