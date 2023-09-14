#include "HEDB/conversion/polyeval_bsgs.h"

namespace HEDB
{
    void EvalChebyshevBaisc(std::vector<seal::Ciphertext> &T1, std::vector<seal::Ciphertext> &T2, seal::Ciphertext &x, uint64_t degree, 
                            seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys)
    {
#ifdef HEDB_DEBUG
        if (degree < 1)
        {
            throw std::invalid_argument("EvalChebyshevBaisc: degree is less than 1.");
        }
#endif
        uint64_t m = CeilLog2(degree + 1);
        uint64_t l = m / 2;
        T1.resize((1ULL << l) - 1);

        //Evaluate T_{1}, T_{2}, T_{3}, ..., T_{2^l - 1}
        T1[0] = x;
        for (size_t i = 2; i < (1ULL << l); i++)
        {
            // T_{i = a + b} = 2 * T_{a} * T_{b} - T_{|a - b|} 
            uint64_t a = (i + 1) / 2;
            uint64_t b = i - a;
            uint64_t c = a > b ? a - b : b - a;
            seal::multiply_and_relinearize(T1[a - 1], T1[b - 1], T1[i - 1], evaluator, relin_keys);
            evaluator.add_inplace(T1[i - 1], T1[i - 1]);
            

            if (c == 0)
            {
                // T_{i = a + b} = 2 * T_{a} * T_{b} - T_{0} 
                seal::add_scalar(T1[i - 1], -1., encoder, evaluator);
                evaluator.rescale_to_next_inplace(T1[i - 1]);
            }
            else
            {
                // T_{i = a + b} = 2 * T_{a} * T_{b} - T_{1} 
                seal::Ciphertext temp = T1[0];
                seal::multiply_scalar(temp, 1., T1[i -1].scale() / T1[0].scale(), encoder, evaluator);
                evaluator.mod_switch_to_inplace(temp, T1[i-1].parms_id());
                evaluator.sub_inplace(T1[i-1], temp);
                evaluator.rescale_to_next_inplace(T1[i-1]);
            }
        }
        // Evaluate T_{2^l}, T_{2^{l+1}}, ..., T_{2^{m-1}}
        T2.resize(m - l);
        seal::Ciphertext temp;
        temp = T1[(1ULL << (l -1 )) - 1];
        evaluator.mod_switch_to_inplace(temp, T1[(1ULL << l) - 2].parms_id());
        for (size_t i = 0; i < m - l; i++)
        {
            seal::multiply_and_relinearize(temp, temp, T2[i], evaluator, relin_keys);
            evaluator.add_inplace(T2[i], T2[i]);
            seal::add_scalar(T2[i], -1., encoder, evaluator);
            evaluator.rescale_to_next_inplace(T2[i]);
            temp = T2[i];
        }
        
    }

    void DivisionChebyshevLazy(ChebyshevPoly &f, uint64_t ChebyshevDegree, ChebyshevPoly &quotient, ChebyshevPoly &remainder)
    {
        if (f.degree() < ChebyshevDegree)
        {
            quotient = ChebyshevPoly();
            quotient.coefficients.resize(1, 0.);
            remainder = f;
        }
        else
        {
            remainder = ChebyshevPoly(ChebyshevDegree - 1);
            std::copy_n(f.coefficients.data(), ChebyshevDegree, remainder.coefficients.data());
            quotient = ChebyshevPoly(f.degree() - ChebyshevDegree);
            quotient.coefficients[0] = f.coefficients[ChebyshevDegree];

            for (size_t i = ChebyshevDegree + 1, j = 1; i <= f.degree(); i++, j++)
            {
                quotient.coefficients[i - ChebyshevDegree] = 2 * f.coefficients[i];
                remainder.coefficients[ChebyshevDegree - j] -= f.coefficients[i];
            }
        }
        
    }

    // Lazy Algotirhm 2 in Jean-Philippe Bossuat et al's paper which does not use level optimization
    void EvalRecurseLazy(double target_scale, uint64_t m, uint64_t l, ChebyshevPoly &poly, 
                    std::vector<seal::Ciphertext> &T1, std::vector<seal::Ciphertext> &T2, seal::Ciphertext &result,
                    seal::SEALContext &context, seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys)
    {
        if (poly.degree() < 0)
        {
            result.release();
            return ;
        }
        uint64_t d = poly.degree();
        if (d < (1ULL << l))
        {
            if (d == 0)
            {
                if (std::abs(std::round(poly.coefficients[0] * target_scale)) > 1.)
                {
                    result.release();
                    result.resize(context, T1[0].parms_id(), 2);
                    result.scale() = target_scale;
                    result.is_ntt_form() = true;
                    seal::add_scalar(result, poly.coefficients[0], encoder, evaluator);
                   
                }
                else
                {
                    result.release();
                }
                return ;
            }

            // Evaluate Polynomial
            // result = \sum_{i=0}^{d} poly[i]*Enc(T_{i}(x))
            auto &context_data = *context.get_context_data(T1[d -1].parms_id());
            auto &parms = context_data.parms();
            double qT_d = parms.coeff_modulus()[T1[d-1].coeff_modulus_size() - 1].value();
            double cipher_scale = target_scale * qT_d;
            seal::Ciphertext temp;
            bool all_zero = true;
            // line 5 of Algorithm 2.
            result.release();
            result.resize(context, T1[d - 1].parms_id(), 2);
            result.scale() = cipher_scale;
            result.is_ntt_form() = true;
            // line 6 ~line 8 of Algorithm 2 .
            for (size_t i = d; i > 0; i--)
            {
                if (std::abs(std::round(poly.coefficients[i] * target_scale)) > 1.)
                {
                    temp = T1[i - 1];
                    evaluator.mod_switch_to_inplace(temp, T1[d-1].parms_id());
                    seal::multiply_scalar(temp, poly.coefficients[i], cipher_scale / T1[i-1].scale(), encoder, evaluator);
                    evaluator.add_inplace(result, temp);
                    all_zero = false;
                }
            }

            // line 5 of Algorithm
            if (std::abs(std::round(poly.coefficients[0] * target_scale)) > 1.)
            {
                seal::add_scalar(result, poly.coefficients[0], encoder, evaluator);
                all_zero = false;
            }
            

            if (all_zero)
            {
                result.release();
                return ;
            }
            else
            {
                evaluator.rescale_to_next_inplace(result);
                return ;
            }                   
        }
        // The const multiplication will consume 1 level.  
 
        uint64_t ChebyshevDegree = 1ULL << (m -1);
        uint64_t ChebyshevIndex = m - 1 - l;
        ChebyshevPoly quotient, remainder;
        DivisionChebyshevLazy(poly, ChebyshevDegree, quotient, remainder);

        size_t level = T2[ChebyshevIndex].coeff_modulus_size() - 1;
        auto &context_data = *context.get_context_data(context.first_parms_id());
        auto &parms = context_data.parms();
        double q = parms.coeff_modulus()[level].value();
        double quotient_scale = target_scale * q / T2[ChebyshevIndex].scale();

        seal::Ciphertext cipher_quotient, cipher_remainder;
        EvalRecurseLazy(target_scale, m -1, l, remainder, T1, T2, cipher_remainder, context, encoder, evaluator, relin_keys);
        EvalRecurseLazy(quotient_scale, m - 1, l , quotient, T1, T2, cipher_quotient, context, encoder, evaluator, relin_keys);

        if (cipher_quotient.size() && cipher_remainder.size())
        {
            // Both q(x) and r(x) are not zero
            seal::multiply_and_relinearize(cipher_quotient, T2[ChebyshevIndex], result, evaluator, relin_keys);
            if (result.coeff_modulus_size() <= cipher_remainder.coeff_modulus_size())
            {
                seal::multiply_scalar(cipher_remainder, 1., result.scale() / cipher_remainder.scale(), encoder, evaluator);
                evaluator.mod_switch_to_inplace(cipher_remainder, result.parms_id());
                evaluator.add_inplace(result, cipher_remainder);
                evaluator.rescale_to_next_inplace(result);
            }
            else
            {
                evaluator.rescale_to_next_inplace(result);
                if (!seal::util::are_close(result.scale(), cipher_remainder.scale())) 
                {
                    result.scale() = cipher_remainder.scale();
                }
                evaluator.mod_switch_to_inplace(result, cipher_remainder.parms_id());
                evaluator.add_inplace(result, cipher_remainder);
            }
        }
        else
        {
            if (cipher_quotient.size())
            {
                // If q(x) are not zero
                seal::multiply_and_relinearize(cipher_quotient, T2[ChebyshevIndex], result, evaluator, relin_keys);
                evaluator.rescale_to_next_inplace(result);
            }
            else if (cipher_remainder.size())
            {
                // If r(x) are not zero
                result = cipher_remainder;
            }
            else
            {
                // If q(x) and r(x) are zero
                result.release();
            }
        }
        return ;

    }

    void poly_evaluate_bsgs_lazy(seal::Ciphertext &result, double target_scale, seal::Ciphertext &x, ChebyshevPoly &poly, 
                            seal::SEALContext &context, seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys)
    {
        uint64_t degree = poly.degree();
        uint64_t m = CeilLog2(degree + 1);
        uint64_t l = m / 2;
        std::vector<seal::Ciphertext> T1,T2;
        EvalChebyshevBaisc(T1, T2, x, degree, encoder, evaluator, relin_keys);
        EvalRecurseLazy(target_scale, m, l, poly, T1, T2, result, context, encoder, evaluator, relin_keys);

    }

    void EvalPower(double target_scale, std::vector<double> coefficients, std::vector<seal::Ciphertext> &power_basis, seal::Ciphertext &result,
                    seal::SEALContext &context, seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys, seal::Decryptor &decryptor)
    {
        if (coefficients.size() == 1)
        {
            if (std::abs(std::round(coefficients[0] * target_scale)) > 1.)
            {
                auto &context_data = *context.get_context_data(power_basis[0].parms_id());
                auto &parms = context_data.parms();
                double qT_d = parms.coeff_modulus()[power_basis[0].coeff_modulus_size() - 1].value();
                double cipher_scale = target_scale * qT_d;
                result = power_basis[0];
                seal::multiply_scalar(result, coefficients[0], cipher_scale / power_basis[0].scale(), encoder, evaluator);
                evaluator.rescale_to_next_inplace(result);
                return ;
            }
            else
            {
                result.release();
                return ;
            }
        }
        std::vector<double> quotient, remainder;
        uint64_t degree = 2 * coefficients.size() - 1;
        uint64_t m = CeilLog2(degree + 1);
        remainder.resize((1 << (m-1)) / 2);
        quotient.resize(coefficients.size() - remainder.size());
        for (size_t i = 0; i < remainder.size(); i++)
        {
            remainder[i] = coefficients[i];
        }
        for (size_t i = 0; i < quotient.size(); i++)
        {
            quotient[i] = coefficients[i + remainder.size()]; 
        }

        auto &context_data = *context.get_context_data(power_basis[m-1].parms_id());
        auto &parms = context_data.parms();
        double qT_d = parms.coeff_modulus()[power_basis[m-1].coeff_modulus_size() - 1].value();
        seal::Ciphertext cipher_quotient, cipher_remainder;
        double quotient_scale = target_scale * qT_d / power_basis[m-1].scale();
        double remainder_scale = target_scale;
        EvalPower(quotient_scale, quotient, power_basis, cipher_quotient, context, encoder, evaluator, relin_keys, decryptor);
        EvalPower(remainder_scale, remainder, power_basis, cipher_remainder, context, encoder, evaluator, relin_keys, decryptor);
        seal::multiply_and_relinearize(cipher_quotient, power_basis[m-1], result, evaluator, relin_keys);
        evaluator.rescale_to_next_inplace(result);
        // seal::Plaintext plain;
        // std::vector<double> computed;
        // decryptor.decrypt(result, plain);
        // encoder.decode(plain, computed);
        // for (size_t i = 0; i < 9; i++)
        // {
        //     std::cout << "Computed1: " << computed[i] << std::endl;
        // }
        evaluator.mod_switch_to_inplace(cipher_remainder, result.parms_id());
        // decryptor.decrypt(cipher_remainder, plain);
        // encoder.decode(plain, computed);
        // for (size_t i = 0; i < 9; i++)
        // {
        //     std::cout << "Computed2: " << computed[i] << std::endl;
        // }
        cipher_remainder.scale() = result.scale();
        evaluator.add_inplace(result, cipher_remainder);
        
        return ;
    }

    // Evaluate polynomial in a power basis, in the form of a_1 x + a_3 x ^ 3 + a_5 x ^ 5 + a_7 x ^ 7 + ...
    void poly_evaluate_power(seal::Ciphertext &result, double target_scale, seal::Ciphertext &x, std::vector<double> &coefficients, 
                            seal::SEALContext &context, seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys, seal::Decryptor &decryptor)
    {
        uint64_t degree = coefficients.size() * 2 - 1;
        uint64_t m = CeilLog2(degree + 1);
        std::vector<seal::Ciphertext> power_basis(m);
        power_basis[0] = x;
        for (size_t i = 1; i < m; i++)
        {
            seal::multiply_and_relinearize(power_basis[i-1], power_basis[i-1], power_basis[i], evaluator, relin_keys);
            evaluator.rescale_to_next_inplace(power_basis[i]);
        }

        EvalPower(target_scale, coefficients, power_basis, result, context, encoder, evaluator, relin_keys, decryptor);
    }

} // namespace HEDB