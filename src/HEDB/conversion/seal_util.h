#include "seal/seal.h"

namespace seal
{
    void pack_encode(std::vector<double> &input, double scale, Plaintext &plain, CKKSEncoder &ckks_encoder);

    void pack_encode_param_id(std::vector<double> &input, seal::parms_id_type param_id, 
                                double scale, Plaintext &plain, CKKSEncoder &ckks_encoder);

    void pack_decode(std::vector<double> &result, Plaintext &plain, CKKSEncoder &ckks_encoder);

    void add_scalar(Ciphertext &result, double scalar, CKKSEncoder &ckks_encoder, Evaluator &evaluator);

    void multiply_scalar(Ciphertext &result, double scalar, double scale, CKKSEncoder &ckks_encoder, Evaluator &evaluator);

    void multiply_and_relinearize(Ciphertext &cipher1, Ciphertext &cipher2, Ciphertext &result, Evaluator &evaluator, RelinKeys &relin_keys);

    void encrypt_no_noise(Ciphertext result, Plaintext &plain, Encryptor &encryptor);

} // namespace seal
