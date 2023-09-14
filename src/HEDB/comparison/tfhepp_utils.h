#pragma once
#include <cmath>
#include <limits>
#include "cloudkey.hpp"
#include "detwfa.hpp"
#include "keyswitch.hpp"
#include "params.hpp"
#include "trlwe.hpp"
#include "utils.hpp"
#include "gatebootstrapping.hpp"

namespace TFHEpp
{
    // c = (a, a * s + m + e)
    template <class P>
    TLWE<P> tlweSymInt32Encrypt(const typename P::T p, const double α, const double scale, const Key<P> &key)
    {
        std::uniform_int_distribution<typename P::T> Torusdist(0, std::numeric_limits<typename P::T>::max());
        TLWE<P> res = {};
        res[P::k * P::n] =
            ModularGaussian<P>(static_cast<typename P::T>(p * scale), α);
        // res[P::k * P::n] = static_cast<typename P::T>(p * scale);
        for (int k = 0; k < P::k; k++)
            for (int i = 0; i < P::n; i++) {
                res[k * P::n + i] = Torusdist(generator);
                // res[k * P::n + i] = 1;
                res[P::k * P::n] += res[k * P::n + i] * key[k * P::n + i];
            }
        return res;
    }

    template <class P>
    typename P::T tlweSymInt32Decrypt(const TLWE<P> &c, const double scale, const Key<P> &key)
    {
        typename P::T phase = c[P::k * P::n];
        typename P::T plain_modulus = (1ULL << (std::numeric_limits<typename P::T>::digits -1)) / scale;
        plain_modulus *= 2;
        for (int k = 0; k < P::k; k++)
            for (int i = 0; i < P::n; i++)
                phase -= c[k * P::n + i] * key[k * P::n + i];
        typename P::T res = 
        static_cast<typename P::T>(std::round(phase / scale)) % plain_modulus;
        return res;
    }

    // For debug
    template <class P>
    typename P::T tlweSymInt32Decrypt(const TLWE<P> &c, const double scale, const Key<P> &key, double &noise)
    {
        typename P::T phase = c[P::k * P::n];
        typename P::T plain_modulus = (1ULL << (std::numeric_limits<typename P::T>::digits -1)) / scale;
        plain_modulus *= 2;
        for (int k = 0; k < P::k; k++)
            for (int i = 0; i < P::n; i++)
                phase -= c[k * P::n + i] * key[k * P::n + i];
        typename P::T res = 
        static_cast<typename P::T>(std::round(phase / scale)) % plain_modulus;
        noise = (std::round(phase / scale) * scale > phase) ? std::round(phase / scale) * scale - phase : phase - std::round(phase / scale) * scale;
        return res;
    }

    template <class P>
    typename P::T tlweSymDecryptNoise(const TLWE<P> &c, typename P::T plain, const Key<P> &key, double &noise)
    {
        typename P::T phase = c[P::k * P::n];
        for (int k = 0; k < P::k; k++)
            for (int i = 0; i < P::n; i++)
                phase -= c[k * P::n + i] * key[k * P::n + i];
        noise = (phase > plain)? phase - plain : plain - phase;
        bool res =
        static_cast<typename std::make_signed<typename P::T>::type>(phase) > 0;
        return res;
    }

    // c = (a, as + m +e)
    template <class P>
    TRLWE<P> trlweSymInt32Encrypt(const array<typename P::T, P::n> &p, const double α, const double scale, const Key<P> &key)
    {
        TRLWE<P> c = trlweSymEncryptZero<P>(α, key);
        for (int i = 0; i < P::n; i++)
            c[P::k][i] += static_cast<typename P::T>(scale* p[i]);
        return c;
    }

    template <class P>
    Polynomial<P> trlweSymInt32Decrypt(const TRLWE<P> &c, double scale, const Key<P> &key)
    {
        Polynomial<P> phase = c[P::k];
        typename P::T plain_modulus = (1ULL << (std::numeric_limits<typename P::T>::digits -1)) / scale;
        plain_modulus *= 2;
        for (int k = 0; k < P::k; k++) {
            Polynomial<P> mulres;
            std::array<typename P::T, P::n> partkey;
            for (int i = 0; i < P::n; i++) partkey[i] = key[k * P::n + i];
            PolyMul<P>(mulres, c[k], partkey);
            for (int i = 0; i < P::n; i++) phase[i] -= mulres[i];
        }

        Polynomial<P> p;
        for (int i = 0; i < P::n; i++)
            p[i] = static_cast<typename P::T>(std::round(phase[i] / scale)) % plain_modulus;
        return p;
    }
    
    template <class P>
    constexpr Polynomial<P> μ_polygen(typename P::T μ)
    {
        Polynomial<P> poly;
        for (typename P::T &p : poly) p = -μ;
        return poly;
    }

    template <class P>
    Polynomial<P> gpolygen(uint32_t plain_bits, uint32_t scale_bits)
    {
        Polynomial<P> poly;
        uint32_t padding_bits = P :: nbit - plain_bits;
        for (int i = 0; i< P::n; i++) poly[i] = (1ULL << scale_bits) * (i >> padding_bits);
        return poly;
    }


    template <class P>
    void TLWEAdd(TLWE<P> &ca, TLWE<P> &cb, TLWE<P> &res)
    {
        for (int i = 0; i <= P::k * P::n; i++) res[i] = ca[i] + cb[i];
        
    }

    template <class P>
    void TLWESub(TLWE<P> &ca, TLWE<P> &cb, TLWE<P> &res)
    {
        for (int i = 0; i <= P::k * P::n; i++) res[i] = ca[i] - cb[i];
        
    }

    template <class P>
    void TRLWEAdd(TRLWE<P> &ca, TRLWE<P> &cb, TRLWE<P> &res)
    {
        for (int k = 0; k <= P :: k; k++)
        {
            for (int i = 0; i < P::n; i++) res[k][i] = ca[k][i] + cb[k][i];
        }     
    }

    template <class P>
    void TRLWESub(TRLWE<P> &ca, TRLWE<P> &cb, TRLWE<P> &res)
    {
        for (int k = 0; k <= P :: k; k++)
        {
            for (int i = 0; i < P::n; i++) res[k][i] = ca[k][i] - cb[k][i];
        }
        
    }

    template <class P>
    void TLWEToTRLWE(TLWE<P> &c, TRLWE<P> &res)
    {
        res[0][0] = c[0];
        for (int i = 1; i < P::n; i++)
        {
            res[0][i] = -c[P::n - i];
        }
        res[P::k][0] = c[P::n];
    }

    void ARI_to_LOG(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, const EvalKey &ek);

    void LOG_to_ARI(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, const EvalKey &ek);

    void log_rescale(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, uint32_t scale_bits, const EvalKey &ek);

    void ari_rescale(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, uint32_t scale_bits, const EvalKey &ek);

    void MSBGateBootstrapping(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, const EvalKey &ek, bool result_type);

    void MSBGateBootstrapping(TLWE<lvl1param> &res, const TLWE<lvl2param> &tlwe, const EvalKey &ek, bool result_type);
    
    void MSBGateBootstrapping(TLWE<lvl2param> &res, const TLWE<lvl2param> &tlwe, const EvalKey &ek, bool result_type);

    void IdeGateBootstrapping(TLWE<lvl1param> &res, const TLWE<lvl1param> &tlwe, uint32_t scale_bits, const EvalKey &ek);

    void IdeGateBootstrapping(TLWE<lvl1param> &res, const TLWE<lvl2param> &tlwe, uint32_t scale_bits, const EvalKey &ek);

    void IdeGateBootstrapping(TLWE<lvl2param> &res, const TLWE<lvl2param> &tlwe, uint32_t scale_bits, const EvalKey &ek);
} // namespace TFHEpp
