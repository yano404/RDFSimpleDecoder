#include "RDFEventStore.h"

RDFEventStore::RDFEventStore(const char* filename) {
    fFileName = filename;
    fBlockCounter = 0;
    fEventCounter = 0;
    fHead = 0;
    fCurrentEventSize = 0;
    fIsEOB = kTRUE;
    fIsEOF = kFALSE;
    //Open();
}

RDFEventStore::~RDFEventStore() {
    Close();
}

Bool_t RDFEventStore::Open() {
    fFileStream.open(fFileName.Data(), std::ios_base::in|std::ios_base::binary);
    if (fFileStream.fail()) {
        return kFALSE;
    }
    return kTRUE;
}

void RDFEventStore::Close() {
    fFileStream.close();
}

Bool_t RDFEventStore::GetNextBlock() {
    if (fIsEOF) {
        // EOF
        return kFALSE;
    }
    if (!fIsEOB) {
        // Unexpectedly called.
        return kFALSE;
    }
    fFileStream.read(fCurrentBlock, kBlockSize);
    if (!fFileStream) {
        fIsEOF = fFileStream.eof();
        return kFALSE;
    }
    fIsEOB = kFALSE;
    fIsEOF = fFileStream.eof();
    // Detect block type.
    UInt_t blockheader;
    memcpy(&blockheader, fCurrentBlock, sizeof(UInt_t));
    switch(blockheader) {
        case kHeaderFlag:
            fCurrentBlockType = "Header";
            DecodeHeader();
            fIsEOB = kTRUE;
            break;
        case kEventFlag:
            fCurrentBlockType = "Event";
            fHead = 0;
            fIsEOB = kFALSE;
            break;
        case kEnderFlag:
            fCurrentBlockType = "Ender";
            DecodeEnder();
            fIsEOB = kTRUE;
            break;
        default:
            return kFALSE;
    }
    fBlockCounter++;
    return kTRUE;
}

Bool_t RDFEventStore::GetNextEvent() {
    if (fCurrentBlockType != "Event") {
        return kFALSE;
    }
    if (fIsEOB) {
        return kFALSE;
    }

    UInt_t buffer;

    if (fHead == 0) {
        fHead = kEventDataStartIdx;
    } else {
        fHead += fCurrentEventSize;
    }
    // Fetch the first 4bytes from HEAD
    memcpy(&buffer, &(fCurrentBlock[fHead]), 4);
    if (buffer==kEOBFlag) {
        // EOB
        fIsEOB = kTRUE;
    } else {
        // Read Event Size & FID
        fCurrentEventSize = 2*(buffer & kEventSizeMask);
        fCurrentEventFID = (buffer & kEventFIDMask)>>k2BytesShift;
        // Read Event ID
        memcpy(&buffer, &(fCurrentBlock[fHead+4]), 4);
        fCurrentEventID = (buffer & kEventIDMask);
        fEventCounter++;
        memcpy(fCurrentEvent, &(fCurrentBlock[fHead]), fCurrentEventSize);
    }
    fSegmentHead = 0;
    return kTRUE;
}

Bool_t RDFEventStore::GetNextSegment() {
    if (fCurrentBlockType != "Event") {
        return kFALSE;
    }
    if (fIsEOB) {
        return kFALSE;
    }
    if (fSegmentHead==0) {
        fSegmentHead = kSegmentStartIdx;
    } else {
        fSegmentHead += fCurrentSegmentSize;
    }
    if (fSegmentHead >= fCurrentEventSize) {
        return kFALSE;
    }
    
    UInt_t buffer;
    memcpy(&buffer, &(fCurrentEvent[fSegmentHead]), 4);
    fCurrentSegmentSize = 2*(buffer & kSegmentSizeMask);
    fCurrentSegmentID = (buffer & kSegmentIDMask)>>k2BytesShift;
    memcpy(fCurrentSegment, &(fCurrentEvent[fSegmentHead]),fCurrentSegmentSize);
    return kTRUE;
}

Bool_t RDFEventStore::DecodeHeader() {
    if (fCurrentBlockType != "Header") {
        return kFALSE;
    }
    char runnumber[]="xxxx";
    char starttime[]="xxxxxxxx";
    char runheader[kRunHeaderCharLength];
    memcpy(runnumber, &(fCurrentBlock[kRunNumberIdx]), kRunNumberCharLength);
    memcpy(starttime, &(fCurrentBlock[kRunStartTimeIdx]), kRunStartTimeCharLength);
    memcpy(runheader, &(fCurrentBlock[kRunHeaderIdx]), kRunHeaderCharLength);
    fRunNumber = atoi(runnumber);
    fStartTime = TString(starttime);
    fRunHeader = runheader;
    return kTRUE;
}

Bool_t RDFEventStore::DecodeEnder() {
    if (fCurrentBlockType != "Ender") {
        return kFALSE;
    }
    char stoptime[]="xxxxxxxx";
    char printtime[]="xxxxxxxx";
    char printdate[]="xxxxxxxxx";
    char runender[kRunEnderCharLength];
    memcpy(stoptime, &(fCurrentBlock[kRunStopTimeIdx]), kRunStopTimeCharLength);
    memcpy(printtime, &(fCurrentBlock[kPrintTimeIdx]), kPrintTimeCharLength);
    memcpy(printdate, &(fCurrentBlock[kPrintDateIdx]), kPrintDateCharLength);
    memcpy(runender, &(fCurrentBlock[kRunEnderIdx]), kRunEnderCharLength);
    fStopTime = stoptime;
    fPrintTime = printtime;
    fPrintDate = printdate;
    fRunEnder = runender;
    return kTRUE;
}
