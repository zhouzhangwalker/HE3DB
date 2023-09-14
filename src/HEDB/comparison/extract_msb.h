#pragma once
#include "tfhepp_utils.h"
#include "HEDB/utils/types.h"

namespace HEDB
{
    // Extract msb 1~5
    void ExtractMSB5(TLWELvl1 &res, const TLWELvl1 &tlwe, const TFHEEvalKey &ek, bool result_type);

    //Extract msb 6~9
    void ExtractMSB9(TLWELvl1 &res, const TLWELvl1 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);

    //Extract msb 10
    void ExtractMSB10(TLWELvl1 &res, const TLWELvl1 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);

    //Extract msb 11
    void ExtractMSB11(TLWELvl2 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);
    
    //Extract msb 12~16
    void ExtractMSB16(TLWELvl2 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);

    //Extract msb 17~21
    void ExtractMSB21(TLWELvl2 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);

    void ImExtractMSB5(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);
    //Improved Extract msb 10-14
    void ImExtractMSB9(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);

    void ImExtractMSB14(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);

    //Improved Extract msb 15-19
    void ImExtractMSB19(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);

    void ImExtractMSB24(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);

    void ImExtractMSB29(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);

    void HomMSB(TLWELvl1 &res, const TLWELvl1 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);

    void HomMSB(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type);

    void HomMSB(std::vector<TLWELvl1> &res, const TRLWELvl1 &trlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type, size_t num_threads);

    void HomMSB(std::vector<TLWELvl1> &res, const TRLWELvl2 &trlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type, size_t num_threads);
    
} // namespace HEDB
