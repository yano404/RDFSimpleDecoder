#ifndef V7XXRAWDATA_H
#define V7XXRAWDATA_H

#include <Rtypes.h>

typedef struct {
    Bool_t UN;
    Bool_t OV;
    UShort_t ADC;
} V7XXChData_t;

class V7XXRawData {
    public:
        V7XXRawData(Int_t nch);
        ~V7XXRawData();
        Int_t GetNCh() {return fNCh;}
        UChar_t GetGeo() {return fGeo;}
        void SetGeo(UChar_t geo) {fGeo = geo;}
        UChar_t GetCrate() {return fCrate;}
        void SetCrate(UChar_t crate) {fCrate = crate;}
        UShort_t GetADC(Int_t ch);
        Bool_t IsOV(Int_t ch);
        Bool_t IsUN(Int_t ch);
        void SetChData(Int_t ch, V7XXChData_t data);
        void SetOV(Int_t ch, Bool_t ov=kTRUE);
        void SetUN(Int_t ch, Bool_t un=kTRUE);
        void SetADC(Int_t ch, UShort_t adc);
        UInt_t GetEventCount() {return fEventCount;}
        void SetEventCount(UInt_t count) {fEventCount = count;}
        void Clear();
    protected:
        Int_t fNCh;
        UChar_t fGeo;
        UChar_t fCrate;
        std::vector<V7XXChData_t> fData;
        UInt_t fEventCount;
};

#endif // V7XXRAWDATA_H
