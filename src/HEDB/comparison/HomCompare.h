#pragma once
#include "extract_msb.h"
#include "operators.h"

/***
 * A Greater than B
 * A Greater than or equal to B
 * A less than B
 * A less than or equal to B
 * A equal to B
***/
namespace HEDB
{
    // cipher1 > cipher2 <=> msb(cipher2 - cipher1) == 1
    template <typename P>
    void greater_than(TFHEpp::TLWE<P> &cipher1, TFHEpp::TLWE<P> &cipher2, TLWELvl1 &res, uint32_t plain_bits, TFHEEvalKey &ek, bool result_type)
    {
        TFHEpp::TLWE<P> sub_tlwe; 
        for (size_t i = 0; i <= P::k * P::n; i++)
        {
            sub_tlwe[i] = cipher2[i] - cipher1[i];
        }
        HomMSB(res, sub_tlwe, plain_bits + 1, ek, result_type);
    }

    // cipher1 >= cipher2 <=> NOT(msb(cipher1 - cipher2)) == 1
    template <typename P>
    void greater_than_equal(TFHEpp::TLWE<P> &cipher1, TFHEpp::TLWE<P> &cipher2, TLWELvl1 &res, uint32_t plain_bits, TFHEEvalKey &ek, bool result_type)
    {
        TFHEpp::TLWE<P> sub_tlwe; 
        for (size_t i = 0; i <= P::k * P::n; i++)
        {
            sub_tlwe[i] = cipher1[i] - cipher2[i];
        }
        HomMSB(res, sub_tlwe, plain_bits + 1, ek, LOGIC);
        HomNOT<Lvl1>(res, res);
        if (IS_ARITHMETIC(result_type)) TFHEpp::LOG_to_ARI(res, res, ek);
    }


    // cipher1 < cipher2 <=> msb(cipher1 - cipher2) == 1
    template <typename P>
    void less_than(TFHEpp::TLWE<P> &cipher1, TFHEpp::TLWE<P> &cipher2, TLWELvl1 &res, uint32_t plain_bits, TFHEEvalKey &ek, bool result_type)
    {
        TFHEpp::TLWE<P> sub_tlwe; 
        for (size_t i = 0; i <= P::k * P::n; i++)
        {
            sub_tlwe[i] = cipher1[i] - cipher2[i];
        }
        HomMSB(res, sub_tlwe, plain_bits + 1, ek, result_type);
    }
    
    // cipher1 <= cipher2 <=> NOT(msb(cipher2 - cipher1)) == 1
    template <typename P>
    void less_than_equal(TFHEpp::TLWE<P> &cipher1, TFHEpp::TLWE<P> &cipher2, TLWELvl1 &res, uint32_t plain_bits, TFHEEvalKey &ek, bool result_type)
    {
        TFHEpp::TLWE<P> sub_tlwe; 
        for (size_t i = 0; i <= P::k * P::n; i++)
        {
            sub_tlwe[i] = cipher2[i] - cipher1[i];
        }
        HomMSB(res, sub_tlwe, plain_bits + 1, ek, LOGIC);
        HomNOT<Lvl1>(res, res);
        if (IS_ARITHMETIC(result_type)) TFHEpp::LOG_to_ARI(res, res, ek);
    }

    template <typename P>
    void equal(TFHEpp::TLWE<P> &cipher1, TFHEpp::TLWE<P> &cipher2, TLWELvl1 &res, uint32_t plain_bits, TFHEEvalKey &ek, bool result_type)
    {
        TLWELvl1 greater_tlwe, less_tlwe;
        greater_than_equal<P>(cipher1, cipher2, greater_tlwe, plain_bits, ek, LOGIC);
        less_than_equal<P>(cipher1, cipher2, less_tlwe, plain_bits, ek, LOGIC);
        HomAND(res, greater_tlwe, less_tlwe, ek, result_type);
    }
} // namespace HEDB
