#pragma once
#include "cloudkey.hpp"
#include "HEDB/utils/types.h"

namespace HEDB
{
    /***
     * The input of Hom Gate must be logic cipher type
    ***/

    // c0 + c1 - 1/8
    void HomAND(TLWELvl1 &res, const TLWELvl1 &ca, const TLWELvl1 &cb, const TFHEEvalKey &ek, bool result_type);

    void HomOR(TLWELvl1 &res, const TLWELvl1 &ca, const TLWELvl1 &cb, const TFHEEvalKey &ek, bool result_type);

    // The output of Not gate is logic type
    template <typename P>
    inline void HomNOT(TFHEpp::TLWE<P> &res, const TFHEpp::TLWE<P> &tlwe)
    {
        for (int i = 0; i <= P::k * P::n; i++) res[i] = -tlwe[i];
    }

} // namespace HEDB
