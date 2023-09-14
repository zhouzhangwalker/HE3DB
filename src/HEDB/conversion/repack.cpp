
#include "HEDB/utils/utils.h"
#include <algorithm>
#include "HEDB/conversion/repack.h"
namespace HEDB
{
    // std::vector<double> sin_coeff {-0.08105589007143076952, 0.00000000000000016223, -0.15541886507934937756, 0.00000000000000024884, -0.13376703618065691193, 
    //                                 -0.00000000000000056002, -0.09316754436003937823, -0.00000000000000046503, -0.02972723975943576333, 0.00000000000000055344, 
    //                                 0.05544950401713608784, 0.00000000000000042375, 0.14926508060315674120, -0.00000000000000005061, 0.22085729411616161766, 
    //                                 0.00000000000000024848, 0.22522386554948897741, -0.00000000000000054231, 0.12682986139811969206, -0.00000000000000056313, 
    //                                 -0.05631183175845096839, 0.00000000000000112350, -0.21883408765751022340, 0.00000000000000039318, -0.21004090439646547206, 
    //                                 -0.00000000000000025453, 0.01211138138241199536, -0.00000000000000025040, 0.23769325810615773986, -0.00000000000000078912, 
    //                                 0.15439588951304561437, -0.00000000000000093203, -0.17686921073803707483, -0.00000000000000015026, -0.21443193439394772626, 
    //                                 0.00000000000000142546, 0.17618928208093090348, 0.00000000000000123112, 0.19323621551361044868, 0.00000000000000071584, 
    //                                 -0.27189154606965287853, -0.00000000000000034348, -0.00866668616574352811, -0.00000000000000104915, 0.29380225208223392785, 
    //                                 -0.00000000000000158261, -0.37099499175094435843, -0.00000000000000041786, 0.28773107755644938832, 0.00000000000000094085, 
    //                                 -0.16712459036626514663, 0.00000000000000075438, 0.07834375197528484214, 0.00000000000000090609, -0.03070063303189948800, 
    //                                 -0.00000000000000141369, 0.00966894801058364173, 0.00000000000000016736};
    std::vector<double> sin_coeff {0.07043813426689680968, 0.00000000000000021373, 0.13648426242848080148, -0.00000000000000065511, 0.12251850008297035521, 
                                    0.00000000000000138338, 0.09687911813122730698, -0.00000000000000086808, 0.05705995636145275163, -0.00000000000000031285, 
                                    0.00190187312696625684, 0.00000000000000044354, -0.06571531807303646056, 0.00000000000000006187, -0.13574001749816311246, 
                                    0.00000000000000010202, -0.18904743240584598318, 0.00000000000000071687, -0.19987271926250091414, -0.00000000000000041061, 
                                    -0.14604929968900889903, -0.00000000000000043404, -0.02740391745089436684, 0.00000000000000064004, 0.11591361998332121164, 
                                    -0.00000000000000074717, 0.20462201735538801395, -0.00000000000000080824, 0.16189767223136858343, -0.00000000000000026978, 
                                    -0.01077097508287188157, -0.00000000000000003535, -0.18568750764013738919, 0.00000000000000053534, -0.18278566848347499452, 
                                    -0.00000000000000019242, 0.03006119366883018562, -0.00000000000000096166, 0.21645511738551495573, 0.00000000000000012574, 
                                    0.10356123542751023703, 0.00000000000000037192, -0.17891441905512733834, 0.00000000000000035345, -0.16355634150343503763, 
                                    -0.00000000000000053488, 0.16482549804489621259, -0.00000000000000118865, 0.16413921800264116846, 0.00000000000000026596, 
                                    -0.20888412226960773044, 0.00000000000000142608, -0.08344642688900480443, -0.00000000000000024583, 0.26869912801153833515, 
                                    0.00000000000000000804, -0.13548482981129844616, 0.00000000000000109860, -0.13729914741849616466, 0.00000000000000277430, 
                                    0.31392945514509801308, 0.00000000000000319002, -0.32761217572456985403, 0.00000000000000107757, 0.24389625486353291861, 
                                    0.00000000000000062732, -0.14493001079259371089, 0.00000000000000155720, 0.07233834282998567733, -0.00000000000000110679, 
                                    -0.03123261034259527599, -0.00000000000000262276, 0.01189279261890215621, -0.00000000000000323379, -0.00404956978046461594, 
                                    -0.00000000000000441352, 0.00124605558777123146, -0.00000000000000212377, -0.00034935166504105009, -0.00000000000000081908, 
                                    0.00008984823446769276, -0.00000000000000558015, -0.00002131648317308570, 0.00000000000000019052, 0.00000468762132811721, 
                                    0.00000000000000103557, -0.00000095941952191024, 0.00000000000000107394, 0.00000018342015938494, 0.00000000000000161096, 
                                    -0.00000003285866440681, 0.00000000000000201318, 0.00000000553158897361, 0.00000000000000262726, -0.00000000087731624365, 
                                    0.00000000000000194294, 0.00000000013139619346, 0.00000000000000145072, -0.00000000001866069215, -0.00000000000000088172, 
                                    0.00000000000282372220};


    std::vector<double> coeff1 = {1.5, -0.5}; // 1.5 x  - 0.5 x ^ 3
    std::vector<double> coeff3 = {2.1875 , -2.1875 , 1.3125 , -0.3125};
    std::vector<double> coeff5 = {2.4609375/2, -3.28125/2, 2.953125/2, -1.40625/2, 0.2734375/2};
    // -0.2095 x ^ 15 + 1.692 x ^ 13 + -5.999 x ^ 11 + 12.22 x ^ 9 + -15.71 x ^ 7 + 13.2 x ^ 5 + -7.332 x ^ 3 + 3.142 x
    std::vector<double> coeff7 = {3.142 / 2, -7.332 / 2, 13.2 / 2, -15.71 / 2, 12.22 / 2, -5.999 / 2, 1.692 / 2, -0.2095 / 2}; 

    void LWEsToRLWEKeyGen(LTPreKey &eval_key, double scale, const seal::SecretKey &seal_key, const TFHESecretKey &tfhepp_key, size_t tfhe_n,
                          seal::CKKSEncoder &encoder, const seal::Encryptor &encryptor, const seal::SEALContext &context)
    {
        using namespace seal;
        size_t slot_count = encoder.slot_count();
        // size_t tfhe_n = Lvl1::n;
#ifdef HEDB_DEBUG
        if (slot_count < tfhe_n)
        {
            throw std::invalid_argument("ConvertKeyInit: degree is small.");
        }
#endif
        if (!is_metadata_valid_for(seal_key, context))
        {
            throw std::invalid_argument("ConvertKeyInit: invalid rlwe_sk");
        }

        std::vector<double> slots(tfhe_n, 0.);
        for (size_t i = 0; i < tfhe_n; ++i)
        {
            slots[i] = tfhepp_key.key.get<Lvl1>()[i] > 1 ? -1. : static_cast<double>(tfhepp_key.key.get<Lvl1>()[i]);
        }

        seal::Plaintext plain_sk;
        plain_sk.parms_id() = context.first_parms_id();
        pack_encode(slots, scale, plain_sk, encoder); // default in first_params_id
        encryptor.encrypt_symmetric(plain_sk, eval_key.key);

        size_t g = CeilSqrt(tfhe_n);
        eval_key.rotated_keys.resize(g);
        eval_key.rotated_keys[0] = eval_key.key;
        for (size_t j = 1; j < g; ++j)
        {
            std::rotate(slots.begin(), slots.begin() + 1, slots.end());
            pack_encode(slots, scale, plain_sk, encoder);
            encryptor.encrypt_symmetric(plain_sk, eval_key.rotated_keys[j]);
        }

        seal::util::seal_memzero(
            slots.data(),
            slots.size() * sizeof(slots[0])); // clean up secret material
    }

    // Linear transform about A * s
    void LinearTranform(seal::Ciphertext &result, std::vector<std::vector<double>> &matrix, double scale, LTPreKey &eval_key,
                        seal::CKKSEncoder &encoder, seal::GaloisKeys &galois_keys, seal::Evaluator &evaluator)
    {
        size_t rows = matrix.size();
        size_t columns = matrix.front().size();
        size_t slot_counts = encoder.slot_count();
        if (columns > slot_counts)
        {
            throw std::invalid_argument("Convert LWE ciphers out of size.");
        }

        // BSGS Parameters
        size_t max_len = std::max(rows, columns);
        size_t min_len = std::min(rows, columns);
        size_t g_tilde = CeilSqrt(min_len);
        size_t b_tilde = CeilDiv(min_len, g_tilde);

        // Baby-Step
        if (eval_key.rotated_keys.size() < g_tilde)
        {
            std::cout << "LWEToRLWEKeyGen Error" << std::endl;
            eval_key.rotated_keys.resize(g_tilde);
            eval_key.rotated_keys[0] = eval_key.key;
            for (size_t i = 1; i < g_tilde; i++)
            {
                evaluator.rotate_vector(eval_key.rotated_keys[i - 1], 1, galois_keys, eval_key.rotated_keys[i]);
            }
        }

        // Giant-Step
        std::vector<double> diag(max_len);
        seal::Plaintext plain;
        plain.parms_id() = eval_key.rotated_keys[0].parms_id();
        for (size_t b = 0; b < b_tilde && g_tilde * b < min_len; b++)
        {
            seal::Ciphertext temp, sum;
            for (size_t g = 0; g < g_tilde && b * g_tilde + g < min_len; g++)
            {
                // Get diagonal
                size_t j = b * g_tilde + g;
                for (size_t r = 0; r < max_len; r++)
                {
                    diag[r] = matrix[r % rows][(r + j) % columns];
                }
                std::rotate(diag.rbegin(), diag.rbegin() + b * g_tilde, diag.rend());
                seal::pack_encode(diag, scale, plain, encoder);
                if (g == 0)
                {
                    evaluator.multiply_plain(eval_key.rotated_keys[g], plain, sum);
                }
                else
                {
                    evaluator.multiply_plain(eval_key.rotated_keys[g], plain, temp);
                    evaluator.add_inplace(sum, temp);
                }
            }
            if (b == 0)
            {
                result = sum;
            }
            else
            {
                evaluator.rotate_vector_inplace(sum, b * g_tilde, galois_keys);
                evaluator.add_inplace(result, sum);
            }
        }

        if (rows < columns)
        {
            size_t gama = std::log2(columns / rows);
            for (size_t j = 0; j < gama; j++)
            {
                seal::Ciphertext temp = result;
                evaluator.rotate_vector_inplace(temp, (1U << j) * rows, galois_keys);
                evaluator.add_inplace(result, temp);
            }
            
        }
    }


    ChebyshevPoly generate_mod_poly(uint32_t r)
    {
        ChebyshevPoly poly;
        poly.coeff() = sin_coeff;
        double cnst_scale = std::pow(0.5 / M_PI, 1. / (1 << r));
        std::transform(poly.coefficients.begin(), poly.coefficients.end(),
                 poly.coefficients.begin(), [&](double u) { return cnst_scale * u; });
        return poly;

    }

    // Algotirhm 7 in Jean-Philippe Bossuat et al's paper
    // Predefined r = 2
    void HomMod(seal::Ciphertext &cipher, double scale, double q0,
                seal::CKKSEncoder &encoder, seal::Evaluator &evaluator, seal::RelinKeys &relin_keys,
                seal::SEALContext &context)
    {
        uint32_t r = 2;
        //Generate ChebyshevPoly
        ChebyshevPoly poly;
        poly = generate_mod_poly(r);
        uint32_t depth = CeilLog2(poly.degree()) + 1 + r;
        double K = 41;
        // if (cipher.coeff_modulus_size() <= depth + 1)
        // {
        //     throw std::invalid_argument("HomMod: level is small.");
        // }
        
        auto &context_data = *context.get_context_data(cipher.parms_id());
        auto &parms = context_data.parms();

        double target_scale = cipher.scale() *= std::round(q0 / scale);
        size_t output_level = cipher.coeff_modulus_size() - 1 - depth;

        for (size_t i = 1; i <= r; ++i) {
            uint64_t qi =  parms.coeff_modulus()[output_level + i].value(); 
            target_scale *= qi;
            target_scale = std::sqrt(target_scale);
        }
        cipher.scale() = target_scale;

        add_scalar(cipher, -0.25 / K, encoder, evaluator);
        seal::Ciphertext cipher_result;
        poly_evaluate_bsgs_lazy(cipher_result, target_scale, cipher, poly, context, encoder, evaluator, relin_keys);
        if (!seal::util::are_close(target_scale, cipher_result.scale())) 
        {
            throw std::invalid_argument("HomMod: scale mismatch.");
        }

        double power_r = std::pow(2., r);
        double theta = std::pow(0.5 / M_PI, 1. / power_r);
        cipher = cipher_result;
        for (size_t i = 0; i < r; ++i) {
            theta *= theta;
            multiply_and_relinearize(cipher, cipher, cipher, evaluator, relin_keys);
            evaluator.add_inplace(cipher, cipher);
            add_scalar(cipher, 0.-theta, encoder, evaluator);
            evaluator.rescale_to_next_inplace(cipher);
        }

        cipher.scale() /= std::round(q0 / scale);
    }

    void LWEsToRLWE(seal::Ciphertext &result, std::vector<TLWELvl1> &lwe_ciphers, LTPreKey &eval_key, double scale, double q0, double rescale,
                    seal::CKKSEncoder &encoder, seal::GaloisKeys &galois_keys, seal::RelinKeys &relin_keys,
                    seal::Evaluator &evaluator, seal::SEALContext &context)
    {
        // 1. Preprocess LWE Matrix
        // TODO: check the p/2 and multiplier
        size_t num_lwe_ciphers = lwe_ciphers.size();
        std::vector<std::vector<double>> A(num_lwe_ciphers);
        std::vector<double> b(num_lwe_ciphers);
        int64_t q_lwe = 1ULL << 32;
        int64_t q_lwe_half = 1ULL << 31;
        double K = 41;
        double multiplier = 1 / K;
        for (size_t i = 0; i < num_lwe_ciphers; i++)
        {
            TLWELvl1 negate_tlwe = lwe_ciphers[i];
            A[i] = std::vector<double>(Lvl1::n);
            // The default ciphertext format in TFHEpp is 
            // Change (a, a*s + m + e) to (-a, a*s + m + e), so the decryption is b + a * s
            for (size_t i = 0; i < Lvl1::k * Lvl1::n; i++)
            {
                negate_tlwe[i] = -negate_tlwe[i];
            }

            std::transform(negate_tlwe.begin(), negate_tlwe.end(), A[i].begin(), 
                            [K, rescale](uint32_t value) {return (static_cast<int32_t>(value)) * rescale / K ;});
            b[i] = static_cast<int32_t>(lwe_ciphers[i][Lvl1::n]) * multiplier * rescale;
        }

        // 2. Linear Transform A * s
        LinearTranform(result, A, 1.0, eval_key, encoder, galois_keys, evaluator);
        evaluator.rescale_to_next_inplace(result);
        result.scale() = 1.0;
        // 3. Perform A * s + b
        seal::Plaintext plain;
        plain.parms_id() = result.parms_id();
        seal::pack_encode_param_id(b, result.parms_id(), 1.0, plain, encoder);
        evaluator.add_plain_inplace(result, plain);



        // // 4. Perform modular reduction
        result.scale() = scale * rescale;
        // std::cout << result.coeff_modulus_size() << std::endl;
        HomMod(result, scale * rescale, q0 * rescale, encoder, evaluator, relin_keys, context);

    }

    void LWEsToRLWE(seal::Ciphertext &result, std::vector<TLWELvl2> &lwe_ciphers, LTPreKey &eval_key, double scale, double q0,
                    seal::CKKSEncoder &encoder, seal::GaloisKeys &galois_keys, seal::RelinKeys &relin_keys,
                    seal::Evaluator &evaluator, seal::SEALContext &context)
    {
        // 1. Preprocess LWE Matrix
        // TODO: check the p/2 and multiplier
        size_t num_lwe_ciphers = lwe_ciphers.size();
        std::vector<std::vector<double>> A(num_lwe_ciphers);
        std::vector<double> b(num_lwe_ciphers);
        int64_t q_lwe = 1ULL << 32;
        int64_t q_lwe_half = 1ULL << 31;
        double K = 41;
        double multiplier = 1 / K;
        for (size_t i = 0; i < num_lwe_ciphers; i++)
        {
            TLWELvl2 negate_tlwe = lwe_ciphers[i];
            A[i] = std::vector<double>(Lvl2::n);
            // The default ciphertext format in TFHEpp is 
            // Change (a, a*s + m + e) to (-a, a*s + m + e), so the decryption is b + a * s
            for (size_t i = 0; i < Lvl2::k * Lvl2::n; i++)
            {
                negate_tlwe[i] = -negate_tlwe[i];
            }

            std::transform(negate_tlwe.begin(), negate_tlwe.end(), A[i].begin(), 
                            [K](uint64_t value) {return (static_cast<int64_t>(value)) / K ;});
            b[i] = static_cast<int64_t>(lwe_ciphers[i][Lvl2::n]) * multiplier;
        }

        // 2. Linear Transform A * s
        LinearTranform(result, A, 1.0, eval_key, encoder, galois_keys, evaluator);
        evaluator.rescale_to_next_inplace(result);
        result.scale() = 1.0;
        // 3. Perform A * s + b
        seal::Plaintext plain;
        plain.parms_id() = result.parms_id();
        seal::pack_encode_param_id(b, result.parms_id(), 1.0, plain, encoder);
        evaluator.add_plain_inplace(result, plain);



        // // 4. Perform modular reduction
        result.scale() = scale;
        // std::cout << result.coeff_modulus_size() << std::endl;
        HomMod(result, scale, q0, encoder, evaluator, relin_keys, context);

    }

    void HomRound(seal::Ciphertext &cipher, double scale,
                seal::CKKSEncoder &encoder, seal::RelinKeys &relin_keys,
                seal::Evaluator &evaluator, seal::Decryptor &decryptor, seal::SEALContext &context)
    {
        auto &context_data = *context.get_context_data(cipher.parms_id());
        auto &parms = context_data.parms();
        auto &coeff_modulus = parms.coeff_modulus();
        // x = 2 * x - 1
        seal::multiply_scalar(cipher, 2.0, 1.0, encoder, evaluator);
        seal::add_scalar(cipher, -1.0, encoder, evaluator);
        poly_evaluate_power(cipher, scale, cipher, coeff1, context, encoder, evaluator, relin_keys, decryptor);
        // poly_evaluate_power(cipher, scale, cipher, coeff3, context, encoder, evaluator, relin_keys, decryptor);
        poly_evaluate_power(cipher, scale, cipher, coeff5, context, encoder, evaluator, relin_keys, decryptor);
        seal::add_scalar(cipher, 0.5, encoder, evaluator);
        
    }

    
} // namespace HEDB
