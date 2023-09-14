#include "tfhepp_utils.h"
#include "HEDB/utils/types.h"

namespace TFHEpp
{

    // In arithmetic mode, 1 -> 1 *scale, 0 -> 0 * scale
    // In logic mode, 1 -> q/8, 0 -> -q/8
    void ARI_to_LOG(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, const EvalKey &ek)
    {
        lvl1param::T μ = lvl1param::μ;
        TLWE<lvl1param> tlweoffset = tlwe;
        tlweoffset[lvl1param::k * lvl1param::n] += μ;
        TLWE<lvl0param> tlwelvl0;
        IdentityKeySwitch<lvl10param>(tlwelvl0, tlweoffset, *ek.iksklvl10);
        GateBootstrappingTLWE2TLWEFFT<lvl01param>(res, tlwelvl0, *ek.bkfftlvl01, μ_polygen<lvl1param>(μ));
    }

    void LOG_to_ARI(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, const EvalKey &ek)
    {
        lvl1param::T μ = lvl1param::μ;
        μ = μ << 1;
        TLWE<lvl0param> tlwelvl0;
        IdentityKeySwitch<lvl10param>(tlwelvl0, tlwe, *ek.iksklvl10);
        GateBootstrappingTLWE2TLWEFFT<lvl01param>(res, tlwelvl0, *ek.bkfftlvl01, μ_polygen<lvl1param>(-μ));
        res[lvl1param::k * lvl1param::n] += lvl1param::μ;
    }

    void log_rescale(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, uint32_t scale_bits, const EvalKey &ek)
    {
        lvl1param::T μ = 1ULL << (scale_bits - 1);
        TLWE<lvl0param> tlwelvl0;
        IdentityKeySwitch<lvl10param>(tlwelvl0, tlwe, *ek.iksklvl10);
        GateBootstrappingTLWE2TLWEFFT<lvl01param>(res, tlwelvl0, *ek.bkfftlvl01, μ_polygen<lvl1param>(-μ));
        res[lvl1param::k * lvl1param::n] += μ;
    }

    void ari_rescale(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, uint32_t scale_bits, const EvalKey &ek)
    {
        lvl1param::T μ = 1ULL << (scale_bits - 1);
        constexpr uint64_t offset = 1ULL << (std::numeric_limits<lvl1param::T>::digits - 6);
        TLWE<lvl1param> tlweoffset = tlwe;
        tlweoffset[lvl1param::k * lvl1param::n] += offset;
        TLWE<lvl0param> tlwelvl0;
        IdentityKeySwitch<lvl10param>(tlwelvl0, tlweoffset, *ek.iksklvl10);
        GateBootstrappingTLWE2TLWEFFT<lvl01param>(res, tlwelvl0, *ek.bkfftlvl01, μ_polygen<lvl1param>(μ));
        res[lvl1param::k * lvl1param::n] += μ;
    }

    // ARI 0 / 1/2, LOG 1/8 , -1/8
    void MSBGateBootstrapping(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, const EvalKey &ek, bool result_type)
    {
        lvl1param::T μ = lvl1param::μ;
        if (IS_ARITHMETIC(result_type)) μ = μ << 1;  
        constexpr uint64_t offset = 1ULL << (std::numeric_limits<lvl1param::T>::digits - 6);
        TLWE<lvl1param> tlweoffset = tlwe;
        tlweoffset[lvl1param::k * lvl1param::n] += offset;
        TLWE<lvl0param> tlwelvl0;
        IdentityKeySwitch<lvl10param>(tlwelvl0, tlweoffset, *ek.iksklvl10);
        GateBootstrappingTLWE2TLWEFFT<lvl01param>(res, tlwelvl0, *ek.bkfftlvl01, μ_polygen<lvl1param>(μ));
        if (IS_ARITHMETIC(result_type)) res[lvl1param::k * lvl1param::n] += μ;
    }

    void MSBGateBootstrapping(TLWE<lvl1param> &res, const TLWE<lvl2param> &tlwe, const EvalKey &ek, bool result_type)
    {
        lvl1param::T μ = lvl1param::μ;
        if (IS_ARITHMETIC(result_type)) μ = μ << 1;
        constexpr uint64_t offset = 1ULL << (std::numeric_limits<lvl2param::T>::digits - 6);
        TLWE<lvl2param> tlweoffset = tlwe;
        tlweoffset[lvl2param::k * lvl2param::n] += offset;
        TLWE<lvl0param> tlwelvl0;
        IdentityKeySwitch<lvl20param>(tlwelvl0, tlweoffset, *ek.iksklvl20);
        GateBootstrappingTLWE2TLWEFFT<lvl01param>(res, tlwelvl0, *ek.bkfftlvl01, μ_polygen<lvl1param>(μ));
        if (IS_ARITHMETIC(result_type)) res[lvl1param::k * lvl1param::n] += μ;
    }
    
    void MSBGateBootstrapping(TLWE<lvl2param> &res, const TLWE<lvl2param> &tlwe, const EvalKey &ek, bool result_type)
    {
        lvl2param::T μ = lvl2param::μ;
        if (IS_ARITHMETIC(result_type)) μ = μ << 1;
        constexpr uint64_t offset = 1ULL << (std::numeric_limits<lvl2param::T>::digits - 7);
        TLWE<lvl2param> tlweoffset = tlwe;
        tlweoffset[lvl2param::k * lvl2param::n] += offset;
        TLWE<lvl0param> tlwelvl0;
        IdentityKeySwitch<lvl20param>(tlwelvl0, tlweoffset, *ek.iksklvl20);
        GateBootstrappingTLWE2TLWEFFT<lvl02param>(res, tlwelvl0, *ek.bkfftlvl02, μ_polygen<lvl2param>(μ));
        if (IS_ARITHMETIC(result_type)) res[lvl2param::k * lvl2param::n] += μ;
    }

    void IdeGateBootstrapping(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, uint32_t scale_bits, const EvalKey &ek)
    {
        constexpr uint64_t offset = 1ULL << (std::numeric_limits<lvl1param::T>::digits - 6);
        TLWE<lvl1param> tlweoffset = tlwe;
        tlweoffset[lvl1param::k * lvl1param::n] += offset;
        constexpr uint32_t plain_bits = 4;
        TLWE<lvl0param> tlwelvl0;
        IdentityKeySwitch<lvl10param>(tlwelvl0, tlweoffset, *ek.iksklvl10);
        GateBootstrappingTLWE2TLWEFFT<lvl01param>(res, tlwelvl0, *ek.bkfftlvl01, gpolygen<lvl1param>(plain_bits, scale_bits));
    }

    void IdeGateBootstrapping(TLWE<lvl1param> &res, const TLWE<lvl2param> &tlwe, uint32_t scale_bits, const EvalKey &ek)
    {
        constexpr uint64_t offset = 1ULL << (std::numeric_limits<lvl2param::T>::digits - 6);
        TLWE<lvl2param> tlweoffset = tlwe;
        tlweoffset[lvl2param::k * lvl2param::n] += offset;
        constexpr uint32_t plain_bits = 4;
        TLWE<lvl0param> tlwelvl0;
        IdentityKeySwitch<lvl20param>(tlwelvl0, tlweoffset, *ek.iksklvl20);
        GateBootstrappingTLWE2TLWEFFT<lvl01param>(res, tlwelvl0, *ek.bkfftlvl01, gpolygen<lvl1param>(plain_bits, scale_bits - 32));
    }

    void IdeGateBootstrapping(TLWE<lvl2param> &res, const TLWE<lvl2param> &tlwe, uint32_t scale_bits, const EvalKey &ek)
    {
        constexpr uint64_t offset = 1ULL << (std::numeric_limits<lvl2param::T>::digits - 7);
        TLWE<lvl2param> tlweoffset = tlwe;
        tlweoffset[lvl2param::k * lvl2param::n] += offset;
        constexpr uint32_t plain_bits = 5;
        TLWE<lvl0param> tlwelvl0;
        IdentityKeySwitch<lvl20param>(tlwelvl0, tlweoffset, *ek.iksklvl20);
        GateBootstrappingTLWE2TLWEFFT<lvl02param>(res, tlwelvl0, *ek.bkfftlvl02, gpolygen<lvl2param>(plain_bits, scale_bits));
    }
} // namespace TFHEpp
