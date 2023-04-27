#include "ModuleDecoderV560.h"

ModuleDecoderV560::ModuleDecoderV560(Int_t id, const char* name, Int_t readmode) {
    fID = id;
    fModuleName = name;
    fReadMode = readmode;
}

ModuleDecoderV560::~ModuleDecoderV560() {
}

Int_t ModuleDecoderV560::Decode(char* buffer, const int &size) {
    Int_t segid;

    UInt_t *evtdata = reinterpret_cast<UInt_t*>(buffer);
    UInt_t evtsize = size / sizeof(UInt_t);

    if (fReadMode) {
        segid = evtdata[0] & 0xffff0000;
        if (segid != fID) {
            return 0;
        }
        for (UInt_t i=1; i!=evtsize; i++) {
            fData[i-1] = ((evtdata[i]&0xffff0000)>>16) + ((evtdata[i]&0x0000ffff)<<16);
        }
    } else {
        for (UInt_t i=0; i!=evtsize; i++) {
            fData[i] = ((evtdata[i]&0xffff0000)>>16) + ((evtdata[i]&0x0000ffff)<<16);
        }
    }
    return 1;
}

void ModuleDecoderV560::Clear() {
    for (Int_t ch=0; ch!=kNCh; ch++) {
        fData[ch] = 0;
    }
}
