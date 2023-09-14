#pragma once
#include <tfhe++.hpp>

#define LOGIC true
#define ARITHMETIC false
#define ASCENDING 1
#define DESCENDING 0
#define MINIMUM 1
#define MAXIMUM 0
#define IS_LOGIC(a) a
#define IS_ARITHMETIC(a) !a

namespace HEDB
{
    using Lvl0 = TFHEpp::lvl0param;
    using Lvl1 = TFHEpp::lvl1param;
    using Lvl2 = TFHEpp::lvl2param;
    using Lvl01 = TFHEpp::lvl01param;
    using Lvl02 = TFHEpp::lvl02param;
    using Lvl10 = TFHEpp::lvl10param;
    using Lvl11 = TFHEpp::lvl11param;
    using Lvl20 = TFHEpp::lvl20param;
    using Lvl21 = TFHEpp::lvl21param;
    using Lvl22 = TFHEpp::lvl22param;
    using TLWELvl0 = TFHEpp::TLWE<Lvl0>;
    using TLWELvl1 = TFHEpp::TLWE<Lvl1>;
    using TLWELvl2 = TFHEpp::TLWE<Lvl2>;
    using TRLWELvl0 = TFHEpp::TRLWE<Lvl0>;
    using TRLWELvl1 = TFHEpp::TRLWE<Lvl1>;
    using TRLWELvl2 = TFHEpp::TRLWE<Lvl2>;
    using TFHEEvalKey = TFHEpp::EvalKey;
    using TFHESecretKey = TFHEpp::SecretKey;
    using TFHETLWE2TRLWEIKSKey11 = TFHEpp::TLWE2TRLWEIKSKey<TFHEpp::lvl11param>;
    using TFHETLWE2TRLWEIKSKey22 = TFHEpp::TLWE2TRLWEIKSKey<TFHEpp::lvl22param>;

} // namespace HEDB
