#include "HEDB/conversion/seal_util.h"

namespace seal
{
    void pack_encode(std::vector<double> &input, double scale, Plaintext &plain, CKKSEncoder &ckks_encoder)
    {
        size_t slot_count = ckks_encoder.slot_count();
        size_t input_size = input.size();
        if (input_size <= slot_count)
        {
            int step_size = slot_count / input_size;
            std::vector<double> plain_input(slot_count, 0.);
            for (size_t i = 0; i < slot_count; i++)
            {
                plain_input[i] = input[i % input_size];
            }
            ckks_encoder.encode(plain_input, scale, plain);
        }
        else
        {
            throw std::invalid_argument("Out of size.");
        }
    }

    void pack_encode_param_id(std::vector<double> &input, seal::parms_id_type param_id, 
                                double scale, Plaintext &plain, CKKSEncoder &ckks_encoder)
    {
        size_t slot_count = ckks_encoder.slot_count();
        size_t input_size = input.size();
        if (input_size <= slot_count)
        {
            int step_size = slot_count / input_size;
            std::vector<double> plain_input(slot_count, 0.);
            for (size_t i = 0; i < slot_count; i++)
            {
                plain_input[i] = input[i % input_size];
            }
            ckks_encoder.encode(plain_input, param_id, scale, plain);
        }
        else
        {
            throw std::invalid_argument("Out of size.");
        }
    }

    void pack_decode(std::vector<double> &result, Plaintext &plain, CKKSEncoder &ckks_encoder)
    {
        size_t result_size = result.size();
        size_t slot_count = ckks_encoder.slot_count();
        if (result_size <= slot_count)
        {
            int step_size = slot_count / result_size;
            std::vector<double> plain_output(slot_count, 0.);
            ckks_encoder.decode(plain, plain_output);
            for (size_t i = 0; i < result_size; i++)
            {
                result[i] = plain_output[i];
            }
        }
        else
        {
            throw std::invalid_argument("Out of size.");
        }
    }

    void add_scalar(Ciphertext &result, double scalar, CKKSEncoder &ckks_encoder, Evaluator &evaluator)
    {
        Plaintext plain;
        Ciphertext cipher;
        ckks_encoder.encode(scalar, result.parms_id(), result.scale(), plain);
        evaluator.add_plain_inplace(result, plain);
    }

    void multiply_scalar(Ciphertext &result, double scalar, double scale, CKKSEncoder &ckks_encoder, Evaluator &evaluator)
    {
        Plaintext plain;
        Ciphertext cipher;
        ckks_encoder.encode(scalar, result.parms_id(), scale, plain);
        evaluator.multiply_plain_inplace(result, plain);
    }

    void multiply_and_relinearize(Ciphertext &cipher1, Ciphertext &cipher2, Ciphertext &result, Evaluator &evaluator, RelinKeys &relin_keys)
    {
        if (cipher1.coeff_modulus_size() > cipher2.coeff_modulus_size())
        {
            evaluator.mod_switch_to_inplace(cipher1, cipher2.parms_id());
        }
        else
        {
            evaluator.mod_switch_to_inplace(cipher2, cipher1.parms_id());
        }
        evaluator.multiply(cipher1, cipher2, result);
        evaluator.relinearize_inplace(result, relin_keys);
    }

} // namespace seal