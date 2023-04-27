#ifndef RDFEVENTSTORE_H
#define RDFEVENTSTORE_H
#include <fstream>
#include <Rtypes.h>
#include <TString.h>

class RDFEventStore {
    public:
        RDFEventStore(const char* filename);
        ~RDFEventStore();
        virtual Bool_t Open();
        virtual void Close();
        virtual Bool_t GetNextBlock();
        virtual Bool_t GetNextEvent();
        virtual Bool_t GetNextSegment();
        static const Int_t kBlockSize = 16384; // 1block 16384 bytes
        static const UInt_t kHeaderFlag = 0x00000001;
        static const UInt_t kEventFlag  = 0x00000000;
        static const UInt_t kEnderFlag  = 0x0000ffff;
        static const UInt_t kEOBFlag = 0xffffffff;
        static const Int_t kRunNumberIdx = 24;
        static const Int_t kRunNumberCharLength = 4;
        static const Int_t kRunStartTimeIdx = 39;
        static const Int_t kRunStartTimeCharLength = 8;
        static const Int_t kRunStopTimeIdx = 57;
        static const Int_t kRunStopTimeCharLength = 8;
        static const Int_t kPrintTimeIdx = 77;
        static const Int_t kPrintTimeCharLength = 8;
        static const Int_t kPrintDateIdx = 87;
        static const Int_t kPrintDateCharLength = 9;
        static const Int_t kRunHeaderIdx = 100;
        static const Int_t kRunHeaderCharLength = 80;
        static const Int_t kRunEnderIdx = 180;
        static const Int_t kRunEnderCharLength = 80;
        static const Int_t kEventDataStartIdx = 8;
        static const Int_t kSegmentStartIdx = 6;
        static const UInt_t kEventSizeMask = 0x00000fff;
        static const UInt_t kEventFIDMask = 0xffff0000;
        static const UInt_t kEventIDMask = 0x0000ffff;
        static const UInt_t kSegmentSizeMask = 0x0000ffff;
        static const UInt_t kSegmentIDMask = 0xffff0000;
        static const UInt_t k2BytesShift = 16;
        Int_t GetScaNCh() {return fScaNCh;}
        void SetScaNCh(Int_t nch) {fScaNCh = nch;}
        Int_t GetRunNumber() {return fRunNumber;}
        TString GetStartTime() {return fStartTime;}
        TString GetStopTime() {return fStopTime;}
        TString GetPrintTime() {return fPrintTime;}
        TString GetPrintDate() {return fPrintDate;}
        TString GetRunHeader() {return fRunHeader;}
        TString GetRunEnder() {return fRunEnder;}
        char fCurrentBlock[kBlockSize];
        char fCurrentEvent[kBlockSize];
        char fCurrentSegment[kBlockSize];
        TString GetCurrentBlockType() {return fCurrentBlockType;}
        UInt_t GetBlockCount() {return fBlockCounter;}
        UInt_t GetEventCount() {return fEventCounter;}
        Bool_t IsEOB() {return fIsEOB;}
        Bool_t IsEOF() {return fIsEOF;}
        virtual Bool_t DecodeHeader();
        virtual Bool_t DecodeEnder();
        UInt_t GetCurrentEventSize() {return fCurrentEventSize;}
        UInt_t GetEventSegmentID() {return fCurrentEventID;}
        UInt_t GetCurrentSegmentSize() {return fCurrentSegmentSize;}
        UInt_t GetCurrentSegmentID() {return fCurrentSegmentID;}
    protected:
        TString fFileName;
        std::ifstream fFileStream;
        Int_t fRunNumber;
        TString fStartTime;
        TString fStopTime;
        TString fPrintDate;
        TString fPrintTime;
        TString fRunHeader;
        TString fRunEnder;
        TString fCurrentBlockType;
        UInt_t fBlockCounter;
        UInt_t fEventCounter;
        Bool_t fIsEOB;
        Bool_t fIsEOF;
        UInt_t fHead;
        UInt_t fSegmentHead;
        UInt_t fCurrentEventSize;
        UInt_t fCurrentEventFID;
        UInt_t fCurrentEventID;
        UInt_t fCurrentSegmentSize;
        UInt_t fCurrentSegmentID;
        Int_t fScaNCh = 16;
};

#endif // RDFEVENTSTORE_H
