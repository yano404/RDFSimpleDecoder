#include "ModuleDecoder.h"

ModuleDecoder::ModuleDecoder() : fID(0), fModuleName("module") {
}

ModuleDecoder::ModuleDecoder(Int_t id, const char* name) : fID(id), fModuleName(name) {
}

ModuleDecoder::~ModuleDecoder() {
}

Int_t ModuleDecoder::SetModuleName(const char* name) {
    fModuleName = name;
    return 0;
}

Int_t ModuleDecoder::Decode(char* buffer, const int &size) {
    return 1;
}
