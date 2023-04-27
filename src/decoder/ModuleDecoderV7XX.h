#ifndef MODULEDECODERV7XX_H
#define MODULEDECODERV7XX_H

#include <Rtypes.h>
#include "ModuleDecoder.h"
#include "V7XXRawData.h"

class ModuleDecoderV7XX : public ModuleDecoder {
    public:
        ModuleDecoderV7XX(Int_t id, const char* name, Bool_t isN);
        ~ModuleDecoderV7XX();
        Int_t GetNCh() {return fNCh;}
        Bool_t IsN() {return fIsN;}
        UChar_t GetGeo() {return fData->GetGeo();}
        UChar_t GetCrate() {return fData->GetCrate();}
        Bool_t IsUN(Int_t ch) {return fData->IsUN(ch);}
        Bool_t IsOV(Int_t ch) {return fData->IsOV(ch);}
        UInt_t GetADC(Int_t ch) {return fData->GetADC(ch);}
        virtual Int_t Decode(char* buffer, const int &size);
        void Clear();
        static const UInt_t kMaskHeader       = 0x02000000;
        static const UInt_t kMaskEOB          = 0x04000000;
        static const UInt_t kMaskGeo          = 0xf8000000;
        static const UInt_t kMaskCrate        = 0x00ff0000;
        static const UInt_t kMaskUN           = 0x00002000;
        static const UInt_t kMaskOV           = 0x00001000;
        static const UInt_t kMaskChannel      = 0x001f0000;
        static const UInt_t kMaskADC          = 0x00000fff;
        static const UInt_t kMaskEventCounter = 0x00ffffff;
        static const UInt_t kShiftGeo = 27;
        static const UInt_t kShiftCrate = 16;
        static const UInt_t kShiftChannel = 16;
    protected:
        Int_t fID;
        Bool_t fIsN;
        Int_t fNCh;
        V7XXRawData *fData;
};

#endif // MODULEDECODERV7XX_H
