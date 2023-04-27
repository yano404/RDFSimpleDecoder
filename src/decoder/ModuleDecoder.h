#ifndef MODULEDECODER_H
#define MODULEDECODER_H

#include <Rtypes.h>
#include <TString.h>

class ModuleDecoder {
    public:
        ModuleDecoder();
        ModuleDecoder(Int_t id, const char* name);
        ~ModuleDecoder();
        virtual Int_t GetID() {return fID;};
        virtual Int_t SetModuleName(const char* name);
        virtual TString GetModuleName() {return fModuleName;}
        virtual Int_t Decode(char* buffer, const int &size);
    protected:
        Int_t fID;
        TString fModuleName;
};

#endif // MODULEDECODER_H
