#ifndef MODULEDECODERV560_H
#define MODULEDECODERV560_H

#include "ModuleDecoder.h"
#include <Rtypes.h>
#include <TString.h>

class ModuleDecoderV560 : public ModuleDecoder {
    public:
        ModuleDecoderV560(Int_t id, const char* name, Int_t readmode=0);
        ~ModuleDecoderV560();
        virtual Int_t Decode(char* buffer, const int &size);
        Int_t GetNCh() {return kNCh;}
        UInt_t GetCount(Int_t ch) {return fData[ch];}
        Int_t GetReadMode() {return fReadMode;}
        void Clear();
        static const Int_t kNCh = 16;
    protected:
        Int_t fID;
        TString fModuleName;
        UInt_t fData[kNCh];
        Int_t fReadMode; // 0->Block, 1->Event by Event
};

#endif // MODULEDECODERV560_H
