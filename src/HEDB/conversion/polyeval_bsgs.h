#pragma once

#include "HEDB/utils/utils.h"
#include <vector>
#include "seal/seal.h"
#include "HEDB/conversion/seal_util.h"

namespace HEDB
{

    /*
    Homomorphic Polynomial Evaluation using Baby-Step Giant-Step Algorithm.
    Based on Jean-Philippe Bossuat et al's paper 
    Efficient Bootstrapping for Approximate Homomorphic Encryption with Non-Sparse Keys (https://eprint.iacr.org/2020/1203)
    For a reference implementation of the algorithm, refer to the code in the following GitHub repository:
    https://github.com/ldsec/lattigo/blob/master/ckks/polynomial_evaluation.go
    https://github.com/Alibaba-Gemini-Lab/OpenPEGASUS/blob/master/pegasus/chevb_approximator.cc
    */

    template <typename T>
    inline uint64_t CeilLog2(T x) 
    {
        return static_cast<uint64_t>(std::ceil(std::log2(x)));
    }

    typedef struct ChebyshevPoly
    {
        ChebyshevPoly() {}

        explicit ChebyshevPoly(size_t degree)
        {
            coefficients.resize(degree + 1, 0.);
        }
        uint64_t degree()
        {
            return coefficients.empty() ? -1 : coefficients.size() - 1;
        }

        std::vector<double> &coeff()
        {
            return coefficients;
        }

        std::vector<double> coefficients;
    } ChebyshevPoly;


    void EvalChebyshevBaisc(std::vector<seal::Ciphertext> &T1, std::vector<seal::Ciphertext> &T2, seal::Ciphertext &x, uint64_t degree, 
                            seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys);
    
    void DivisionChebyshevLazy(ChebyshevPoly &f, uint64_t ChebyshevDegree, ChebyshevPoly &quotient, ChebyshevPoly &remainder);

    void EvalRecurseLazy(double target_scale, uint64_t m, uint64_t l, ChebyshevPoly &poly, 
                    std::vector<seal::Ciphertext> &T1, std::vector<seal::Ciphertext> &T2, seal::Ciphertext &result,
                    seal::SEALContext &context, seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys);
    
    void poly_evaluate_bsgs_lazy(seal::Ciphertext &result, double target_scale, seal::Ciphertext &x, ChebyshevPoly &poly, 
                            seal::SEALContext &context, seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys);
    
    void EvalPower(double target_scale, std::vector<double> coefficients, std::vector<seal::Ciphertext> &power_basis, seal::Ciphertext &result,
                    seal::SEALContext &context, seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys, seal::Decryptor &decryptor);
    
    void poly_evaluate_power(seal::Ciphertext &result, double target_scale, seal::Ciphertext &x, std::vector<double> &coefficients, 
                            seal::SEALContext &context, seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys, seal::Decryptor &decryptor);
} // namespace HEDB
