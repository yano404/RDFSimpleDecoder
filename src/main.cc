/*
 * RDF Decoder for P4
 */

#include <iostream>
#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <TArrayL.h>
#include "RDFEventStore.h"
#include "ModuleDecoderV7XX.h"
#include "ModuleDecoderV560.h"

int main(int argc, char *argv[]){
    static const Int_t NCH_V7XX = 32;
    static const Int_t NCH_V7XXN = 16;
    static const Int_t NCH_V560 = 16;
    static const UInt_t UN_VALUE = 0x0;
    static const UInt_t OV_VALUE = 0x1fff;

    if (argc != 3) {
        return 1;
    }

    // Read command line args
    char* rdffilename = argv[1];
    char* outfilename = argv[2];

    std::cout << "Input  : " << rdffilename << std::endl;
    std::cout << "Output : " << outfilename << std::endl;

    // Initialize
    // RDF event store
    auto *eventstore = new RDFEventStore(rdffilename);
    
    // Module decoders
    ModuleDecoderV7XX decoderV775 = ModuleDecoderV7XX(1, "V775N", kTRUE);
    ModuleDecoderV7XX decoderV792 = ModuleDecoderV7XX(2, "V792N", kTRUE);
    ModuleDecoderV7XX decoderV785 = ModuleDecoderV7XX(3, "V785N", kTRUE);
    ModuleDecoderV560 decoderV560 = ModuleDecoderV560(4, "V560", 0); // read scaler block by block
    
    // Data array
    Int_t tdc[NCH_V7XXN];
    Int_t qdc[NCH_V7XXN];
    Int_t adc[NCH_V7XXN];
    Int_t sca[NCH_V560];
    char scablkbuffer[NCH_V560*4];

    // Open Root file
    TFile *fout = TFile::Open(outfilename, "RECREATE");
    // Create tree & branch
    TTree *tree = new TTree("tree", "tree");
    tree->Branch("tdc", tdc, Form("tdc[%d]/I", NCH_V7XXN));
    tree->Branch("qdc", qdc, Form("qdc[%d]/I", NCH_V7XXN));
    tree->Branch("adc", adc, Form("adc[%d]/I", NCH_V7XXN));
    tree->Branch("sca", sca, Form("sca[%d]/I", NCH_V560));

    // Scaler array (for block read mode)
    TArrayL scablk = TArrayL(NCH_V560);
    
    // Open RDF file
    if (!eventstore->Open()) {
        // Fail to open the file
        return 1;
    }
    
    // Decode
    while (!eventstore->IsEOF()) {
        // Read block
        while (eventstore->GetNextBlock()) {
            // Success to get next block
            while (eventstore->GetNextEvent()) {
                // Success to get next event
                if (!eventstore->IsEOB()) {
                    // Decode Event
                    while (eventstore->GetNextSegment()) {
                        // Decode Segment
                        decoderV775.Decode(eventstore->fCurrentSegment, eventstore->GetCurrentSegmentSize());
                        decoderV792.Decode(eventstore->fCurrentSegment, eventstore->GetCurrentSegmentSize());
                        decoderV785.Decode(eventstore->fCurrentSegment, eventstore->GetCurrentSegmentSize());
                    }
                    // Fill data
                    for (Int_t ch=0; ch!=NCH_V7XXN; ch++) {
                        // SegID=1 V775
                        if (decoderV775.IsOV(ch)) {
                            tdc[ch] = OV_VALUE;
                        } else if (decoderV775.IsUN(ch)) {
                            tdc[ch] = UN_VALUE;
                        } else {
                            tdc[ch] = decoderV775.GetADC(ch);
                        }
                        // SegID=2 V792
                        if (decoderV792.IsOV(ch)) {
                            qdc[ch] = OV_VALUE;
                        } else if (decoderV792.IsUN(ch)) {
                            qdc[ch] = UN_VALUE;
                        } else {
                            qdc[ch] = decoderV792.GetADC(ch);
                        }
                        // SegID=3 V785
                        if (decoderV785.IsOV(ch)) {
                            adc[ch] = OV_VALUE;
                        } else if (decoderV785.IsUN(ch)) {
                            adc[ch] = UN_VALUE;
                        } else {
                            adc[ch] = decoderV785.GetADC(ch);
                        }
                        // SegID=4 V560
                        //sca[ch] = decoderV560.GetCount(ch);
                    }
                    tree->Fill();
                } else {
                    // End of block
                    // Read scaler
                    memcpy(scablkbuffer, &(eventstore->fCurrentBlock[eventstore->kBlockSize - NCH_V560*4]), NCH_V560*4);
                    decoderV560.Decode(scablkbuffer, NCH_V560*4);
                    for (Int_t ch=0; ch!=NCH_V560; ch++) {
                        scablk[ch] = decoderV560.GetCount(ch);
                    }
                }
            }
        }
    }
    // Write to the file
    tree->Write();
    fout->WriteObject(&scablk, "scaler");
    fout->Write();
    fout->Close();

    // Print Runinfo
    std::cout << "Run    : "; 
    printf("%04d", eventstore->GetRunNumber());
    std::cout << std::endl;
    std::cout << "Date   : " << eventstore->GetPrintDate() << std::endl;
    std::cout << "Start  : " << eventstore->GetStartTime() << std::endl;
    std::cout << "Stop   : " << eventstore->GetStopTime() << std::endl;
    std::cout << "Header : ";
    std::cout << eventstore->GetRunHeader() << std::endl;
    std::cout << "Ender  : ";
    std::cout << eventstore->GetRunEnder() << std::endl;
    std::cout << "NEvent : " << eventstore->GetEventCount() << std::endl;

    return 0;
}
