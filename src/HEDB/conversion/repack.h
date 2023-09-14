#pragma once

#include "HEDB/utils/utils.h"
#include <vector>
#include "seal/seal.h"
#include "HEDB/conversion/seal_util.h"
#include "HEDB/conversion/polyeval_bsgs.h"

namespace HEDB
{
    typedef struct LTPreKey
    {
        seal::Ciphertext key;
        std::vector < seal::Ciphertext > rotated_keys;
    } LTPreKey;
  
    // ceil(sqrt(n))
    template <typename T>
    static inline T CeilSqrt(T val) 
    {
        return static_cast<T>(std::ceil(std::sqrt(1. * val)));
    }

    template <typename T>
    static inline T CeilDiv(T a, T b) 
    {
        return (a + b - 1) / b;
    }


    void LWEsToRLWEKeyGen(LTPreKey &eval_key, double scale, const seal::SecretKey &seal_key, const TFHESecretKey &tfhepp_key, size_t tfhe_n,
                          seal::CKKSEncoder &encoder, const seal::Encryptor &encryptor, const seal::SEALContext &context);

    void LinearTranform(seal::Ciphertext &result, std::vector<std::vector<double>> &matrix, double scale, LTPreKey &eval_key,
                        seal::CKKSEncoder &encoder, seal::GaloisKeys &galois_keys, seal::Evaluator &evaluator);
    
    ChebyshevPoly generate_mod_poly(uint32_t r);

    void HomMod(seal::Ciphertext &cipher, double scale, double q0,
                seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys,
                seal::SEALContext &context);                

    void LWEsToRLWE(seal::Ciphertext &result, std::vector<TLWELvl1> &lwe_ciphers, LTPreKey &eval_key, double scale, double q0, double rescale,
                    seal::CKKSEncoder &encoder, seal::GaloisKeys &galois_keys, seal::RelinKeys &relin_keys,
                    seal::Evaluator &evaluator, seal::SEALContext &context);
    
    void HomRound(seal::Ciphertext &result, double scale,
                seal::CKKSEncoder &encoder, seal::RelinKeys &relin_keys,
                seal::Evaluator &evaluator, seal::Decryptor &decryptor, seal::SEALContext &context);

} // namespace HEDB
