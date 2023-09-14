#include "HEDB/comparison/tfhepp_utils.h"
#include "HEDB/conversion/conversion.h"
#include <chrono>

using namespace HEDB;

void repack_test(uint32_t scale_bits, size_t num_slots)
{
    std::cout << " Num slots: " << num_slots << std::endl;
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());
    using P = Lvl1;
    uint64_t modq_bits = 32;
    uint64_t modulus_bits = 45;
    TFHESecretKey tfhepp_secret_key;
    std::uniform_int_distribution<typename P::T> message(0, 1);
    std::vector<uint32_t> slots(num_slots);
    std::vector<TLWELvl1> tlwes(num_slots);
    for (size_t i = 0; i < num_slots; i++)
    {
        slots[i] = message(engine);
        // slots[i] = 1;
        tlwes[i] = TFHEpp::tlweSymInt32Encrypt<P>(slots[i], Lvl1::α, std::pow(2., scale_bits), tfhepp_secret_key.key.get<P>());
    }

    std::cout << "TFHEpp ciphertext generated." << std::endl;

    seal::EncryptionParameters parms(seal::scheme_type::ckks);
    size_t poly_modulus_degree = 65536;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(seal::CoeffModulus::Create(poly_modulus_degree, {45, 42, 42, 42, 42, 42, 42, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 59}));
    double scale = std::pow(2.0, scale_bits);

    //context instance
    seal::SEALContext context(parms, true, seal::sec_level_type::none);

    //key generation
    seal::KeyGenerator keygen(context);
    seal::SecretKey seal_secret_key = keygen.secret_key();
    seal::RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);
    seal::GaloisKeys galois_keys;
    keygen.create_galois_keys(galois_keys);
    

    //utils
    seal::Encryptor encryptor(context, seal_secret_key);
    seal::Evaluator evaluator(context);
    seal::Decryptor decryptor(context, seal_secret_key);

    //encoder
    seal::CKKSEncoder ckks_encoder(context);

    std::cout << "Parameters generated" << std::endl;

    //generate evaluation key
    LTPreKey pre_key;
    LWEsToRLWEKeyGen(pre_key, std::pow(2., modulus_bits), seal_secret_key, tfhepp_secret_key, P::n, ckks_encoder, encryptor, context);

    std::cout << "Conversion key generated" << std::endl;

    // conversion
    seal::Ciphertext result;
    std::chrono::system_clock::time_point start, end;
    start = std::chrono::system_clock::now();
    LWEsToRLWE(result, tlwes, pre_key, scale, std::pow(2., modq_bits), std::pow(2., modulus_bits - modq_bits), ckks_encoder, galois_keys, relin_keys, evaluator, context);
    HomRound(result, result.scale(), ckks_encoder, relin_keys, evaluator, decryptor, context);
    end = std::chrono::system_clock::now();
    std::cout << "Conversion Done" << std::endl;
    seal::Plaintext plain;
    std::vector<double> computed(num_slots);
    double run_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    decryptor.decrypt(result, plain);
    seal::pack_decode(computed, plain, ckks_encoder);

    double err = 0.;
    
    // std::cout << "Repack time: " << repack_time << std::endl;
    for (size_t i = 0; i < num_slots; ++i)
    {
        err += std::abs(computed[i] - slots[i]);
        if(i < 8) std::cout << "Initial: " << slots[i] << "; Repack: " << computed[i] << std::endl;
    }

    printf("repack average error = %f ~ 2^%.1f\n", err / num_slots, std::log2(err / num_slots));
    std::cout << "Run time : " << run_time << " ms" << std::endl;
    
    
}

int main()
{

    repack_test(29, 32768);

}