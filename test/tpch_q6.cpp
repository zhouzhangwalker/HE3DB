#include "HEDB/comparison/comparison.h"
#include "HEDB/utils/utils.h"
#include "HEDB/conversion/repack.h"

using namespace HEDB;
using namespace std;

/***
 * TPC-H Query 6
 * select
        sum(l_extendedprice * l_discount) as revenue
    from
	    lineitem
    where
        l_shipdate >= date ':1'
        and l_shipdate < date ':1' + interval '1' year
        and l_discount between :2 - 0.01 and :2 + 0.01
        and l_quantity < :3;
    
    consider data \in [20200101~20221231]
*/


uint64_t generate_date(uint64_t down, uint64_t up)
{
    uint64_t dyear, dmonth, dday, uyear, umonth, uday;
    dyear = down / 10000;
    dmonth = (down / 100) % 100;
    dday = down % 100;
    uyear = up / 10000;
    umonth = (up / 100) % 100;
    uday = up % 100;
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());
    uniform_int_distribution<Lvl1::T> day_message(dday, uday);
    uniform_int_distribution<Lvl1::T> month_message(dmonth, umonth);
    uniform_int_distribution<Lvl1::T> year_message(dyear, uyear);
    return day_message(engine) + 100 * month_message(engine) + 10000 * year_message(engine);

}

void predicate_evaluation(std::vector<TLWELvl1> &pred_cres, std::vector<uint32_t> &pred_res, size_t rows, 
    std::vector<uint32_t> quanlity_data, std::vector<uint32_t> discount_data, 
    std::vector<uint64_t> ship_data, TFHESecretKey &sk, double &filter_time)
{
    std::cout<< "Predicate evaluation: " << std::endl;
    using P = Lvl2;
    TFHEEvalKey ek;
    std::cout<< "Generating evaluation key..." << std::endl;
    ek.emplacebkfft<Lvl01>(sk);
    ek.emplacebkfft<Lvl02>(sk);
    ek.emplaceiksk<Lvl20>(sk);
    ek.emplaceiksk<Lvl10>(sk);
    ek.emplaceiksk<Lvl21>(sk);
    
    uint32_t quantity_bits = 6, ship_bits = 16, discount_bits = 4, quantity_scale_bits, ship_scale_bits, discount_scale_bits;

    quantity_scale_bits = std::numeric_limits<Lvl1::T>::digits - quantity_bits - 1;
    ship_scale_bits = std::numeric_limits<Lvl2::T>::digits - ship_bits - 1;
    discount_scale_bits = std::numeric_limits<Lvl1::T>::digits - discount_bits - 1;


    //Encrypt database
    std::cout<< "Encrypting Database..." << std::endl;
    std::vector<TLWELvl1> quanlity_ciphers(rows), discount_ciphers(rows);
    std::vector<TLWELvl2> ship_ciphers(rows);
    for (size_t i = 0; i < rows; i++)
    {
        quanlity_ciphers[i] = TFHEpp::tlweSymInt32Encrypt<Lvl1>(quanlity_data[i], Lvl1::α, pow(2., quantity_scale_bits), sk.key.get<Lvl1>());
        discount_ciphers[i] = TFHEpp::tlweSymInt32Encrypt<Lvl1>(discount_data[i], Lvl1::α, pow(2., discount_scale_bits), sk.key.get<Lvl1>());
        ship_ciphers[i] = TFHEpp::tlweSymInt32Encrypt<Lvl2>(ship_data[i], Lvl2::α, pow(2., ship_scale_bits), sk.key.get<Lvl2>());
    }
    
    //Encrypt Predicate values
    std::cout<< "Encrypting Predicate Values..." << std::endl;
    Lvl1::T pred3, pred4, pred5;
    Lvl2::T pred1 = 20101, pred2 = 21231;
    pred3 =  8;
    pred4 = pred3 + 2;
    pred5 = 32;

    std::vector<Lvl1::T> pred_res1(rows, 0), pred_res2(rows, 0), pred_res3(rows, 0), pred_res4(rows, 0), pred_res5(rows, 0);
    for (size_t i = 0; i < rows; i++)
    {
        pred_res1[i] = (ship_data[i] >= pred1) ? 1 : 0;
        pred_res2[i] = (ship_data[i] < pred2) ? 1 : 0;
        pred_res3[i] = (discount_data[i] >= pred3) ? 1 : 0;
        pred_res4[i] = (discount_data[i] <= pred4) ? 1 : 0;
        pred_res5[i] = (quanlity_data[i] < pred5) ? 1 : 0;
        pred_res[i] = pred_res1[i] * pred_res2[i] * pred_res3[i] * pred_res4[i] * pred_res5[i];
    }
    
    TLWELvl1 pred_cipher3, pred_cipher4, pred_cipher5;
    TLWELvl2 pred_cipher1, pred_cipher2;
    pred_cipher1 = TFHEpp::tlweSymInt32Encrypt<Lvl2>(pred1, Lvl2::α, pow(2., ship_scale_bits), sk.key.get<Lvl2>());
    pred_cipher2 = TFHEpp::tlweSymInt32Encrypt<Lvl2>(pred2, Lvl2::α, pow(2., ship_scale_bits), sk.key.get<Lvl2>());
    pred_cipher3 = TFHEpp::tlweSymInt32Encrypt<Lvl1>(pred3, Lvl1::α, pow(2., discount_scale_bits), sk.key.get<Lvl1>());
    pred_cipher4 = TFHEpp::tlweSymInt32Encrypt<Lvl1>(pred4, Lvl1::α, pow(2., discount_scale_bits), sk.key.get<Lvl1>());
    pred_cipher5 = TFHEpp::tlweSymInt32Encrypt<Lvl1>(pred5, Lvl1::α, pow(2., quantity_scale_bits), sk.key.get<Lvl1>());
    
    // Predicate Evaluation
    std::cout<< "Start Predicate Evaluation..." << std::endl;
    std::vector<TLWELvl1> pred_cres1(rows), pred_cres2(rows), pred_cres3(rows), pred_cres4(rows), pred_cres5(rows);
    std::chrono::system_clock::time_point start, end;
    start = std::chrono::system_clock::now();
    for (size_t i = 0; i < rows; i++)
    {
        greater_than_equal<Lvl2>(ship_ciphers[i], pred_cipher1, pred_cres1[i], ship_bits, ek, LOGIC);
        less_than<Lvl2>(ship_ciphers[i], pred_cipher2, pred_cres2[i], ship_bits, ek, LOGIC);
        HomAND(pred_cres[i], pred_cres1[i], pred_cres2[i], ek, LOGIC);
        greater_than_equal<Lvl1>(discount_ciphers[i], pred_cipher3, pred_cres3[i], discount_bits, ek, LOGIC);
        HomAND(pred_cres[i], pred_cres[i], pred_cres3[i], ek, LOGIC);
        less_than_equal<Lvl1>(discount_ciphers[i], pred_cipher4, pred_cres4[i], discount_bits, ek, LOGIC);
        HomAND(pred_cres[i], pred_cres[i], pred_cres4[i], ek, LOGIC);
        less_than<Lvl1>(quanlity_ciphers[i], pred_cipher5, pred_cres5[i], quantity_bits, ek, LOGIC);
        HomAND(pred_cres[i], pred_cres[i], pred_cres5[i], ek, ARITHMETIC);
    }
    end = std::chrono::system_clock::now();
    std::vector<uint32_t> pred_cres_de(rows), pred_cres1_de(rows), pred_cres2_de(rows), pred_cres3_de(rows), pred_cres4_de(rows), pred_cres5_de(rows);
    for (size_t i = 0; i < rows; i++)
    {
        pred_cres_de[i] = TFHEpp::tlweSymInt32Decrypt<Lvl1>(pred_cres[i], pow(2., 31), sk.key.get<Lvl1>());
        pred_cres1_de[i] =  TFHEpp::tlweSymDecrypt<Lvl1>(pred_cres1[i], sk.key.lvl1);
        pred_cres2_de[i] =  TFHEpp::tlweSymDecrypt<Lvl1>(pred_cres2[i], sk.key.lvl1);
        pred_cres3_de[i] =  TFHEpp::tlweSymDecrypt<Lvl1>(pred_cres3[i], sk.key.lvl1);
        pred_cres4_de[i] =  TFHEpp::tlweSymDecrypt<Lvl1>(pred_cres4[i], sk.key.lvl1);
        pred_cres5_de[i] =  TFHEpp::tlweSymDecrypt<Lvl1>(pred_cres5[i], sk.key.lvl1);
    }
    
    size_t error_time = 0;

    uint32_t rlwe_scale_bits = 29;
    for (size_t i = 0; i < rows; i++)
    {
        TFHEpp::ari_rescale(pred_cres[i], pred_cres[i], rlwe_scale_bits, ek);
    }
    for (size_t i = 0; i < rows; i++)
    {
        pred_cres_de[i] = TFHEpp::tlweSymInt32Decrypt<Lvl1>(pred_cres[i], pow(2., 29), sk.key.get<Lvl1>());
    }
    for (size_t i = 0; i < rows; i++) error_time += (pred_cres_de[i] == pred_res[i])? 0 : 1;
    // cout << "Predicate Evaluaton Time (s): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000 << std::endl;
    cout << "Predicate Error: " << error_time << std::endl;
    filter_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

}

void aggregation(std::vector<TLWELvl1> &pred_cres, std::vector<uint32_t> &pred_res, size_t tfhe_n,
            std::vector<double> &extendedprice_data, std::vector<double> &discount_data_double,
             size_t rows, TFHESecretKey &sk, double &aggregation_time)
{
    std::cout << "Aggregation :" << std::endl;
    uint64_t scale_bits = 29;
    uint64_t modq_bits = 32;
    uint64_t modulus_bits = 45;
    uint64_t repack_scale_bits = modulus_bits + scale_bits - modq_bits;
    uint64_t slots_count = pred_cres.size();
    std::cout << "Generating Parameters..." << std::endl;
    seal::EncryptionParameters parms(seal::scheme_type::ckks);
    size_t poly_modulus_degree = 65536;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(seal::CoeffModulus::Create(poly_modulus_degree, {59, 42, 42, 42, 42, 42, 42, 42, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 59}));
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

    

    //generate evaluation key
    std::cout << "Generating Conversion Key..." << std::endl;
    LTPreKey pre_key;
    LWEsToRLWEKeyGen(pre_key, std::pow(2., modulus_bits), seal_secret_key, sk, tfhe_n, ckks_encoder, encryptor, context);


    // conversion
    std::cout << "Starting Conversion..." << std::endl;
    seal::Ciphertext result;
    std::chrono::system_clock::time_point start, end;
    start = std::chrono::system_clock::now();
    LWEsToRLWE(result, pred_cres, pre_key, scale, std::pow(2., modq_bits), std::pow(2., modulus_bits - modq_bits), ckks_encoder, galois_keys, relin_keys, evaluator, context);
    HomRound(result, result.scale(), ckks_encoder, relin_keys, evaluator, decryptor, context);
    end = std::chrono::system_clock::now();
    aggregation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    seal::Plaintext plain;
    std::vector<double> computed(slots_count);
    decryptor.decrypt(result, plain);
    seal::pack_decode(computed, plain, ckks_encoder);

    double err = 0.;
    
    for (size_t i = 0; i < slots_count; ++i)
    {
        err += std::abs(computed[i] - pred_res[i]);
    }

    printf("Repack average error = %f ~ 2^%.1f\n", err / slots_count, std::log2(err / slots_count));


    // Filter result * data
    std::vector<double> price_discount(extendedprice_data.size());
    seal::Ciphertext price_discount_cipher;
    for (size_t i = 0; i < rows; i++)
    {
        price_discount[i] = extendedprice_data[i] * discount_data_double[i];
    }
    double qd = parms.coeff_modulus()[result.coeff_modulus_size() - 1].value();
    seal::pack_encode(price_discount, qd, plain, ckks_encoder);
    encryptor.encrypt_symmetric(plain, price_discount_cipher);

    std::cout << "Aggregating price and discount .." << std::endl;
    start = std::chrono::system_clock::now();
    seal::multiply_and_relinearize(result, price_discount_cipher, result, evaluator, relin_keys);
    evaluator.rescale_to_next_inplace(result);
    int logrow = log2(rows);
    
    seal::Ciphertext temp;
    for (size_t i = 0; i < logrow; i++)
    {
        temp = result;
        size_t step = 1 << (logrow - i - 1);
        evaluator.rotate_vector_inplace(temp, step, galois_keys);
        evaluator.add_inplace(result, temp);
    }
    end = std::chrono::system_clock::now();
    aggregation_time += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::vector<double> agg_result(slots_count);
    decryptor.decrypt(result, plain);
    seal::pack_decode(agg_result, plain, ckks_encoder);
    double plain_result = 0;
    for (size_t i = 0; i < rows; i++)
    {
        plain_result += extendedprice_data[i] * discount_data_double[i] * pred_res[i];
    }
    cout << "Plain_result: " << plain_result << endl;
    cout << "Encrypted query result: " << std::round(agg_result[0]) <<endl;

}

void query_evaluation(size_t rows)
{
    TFHESecretKey sk;
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());
     // Generate database
    std::vector<uint32_t> quanlity_data(rows), discount_data(rows);
    std::vector<uint64_t> ship_data(rows);
    uint32_t quantity_bits = 6, ship_bits = 16, discount_bits = 4, quantity_scale_bits, ship_scale_bits, discount_scale_bits;
    uniform_int_distribution<Lvl1::T> quantity_message(0, (1 << quantity_bits) - 1);
    uniform_int_distribution<Lvl1::T> discount_message(0, (1 << discount_bits) - 1);
    quantity_scale_bits = std::numeric_limits<Lvl1::T>::digits - quantity_bits - 1;
    ship_scale_bits = std::numeric_limits<Lvl2::T>::digits - ship_bits - 1;
    discount_scale_bits = std::numeric_limits<Lvl1::T>::digits - discount_bits - 1;
    for (size_t i = 0; i < rows; i++)
    {
        quanlity_data[i] = quantity_message(engine);
        discount_data[i] = discount_message(engine);
        ship_data[i] = generate_date(10101, 21230);
    }

    std::vector<double> extendedprice_data(rows), discount_data_double(rows);
    std::uniform_int_distribution<uint64_t> extendedprice_message(1, 10);
    for (size_t i = 0; i < rows; i++)
    {
        extendedprice_data[i] = (double)extendedprice_message(engine);
        discount_data_double[i] = (double)discount_data[i];
    }

    quanlity_data[0] = 1;
    discount_data[0] = 9;
    ship_data[0] = 21215;
    double filter_time, aggregation_time;
    std::vector<TLWELvl1> pred_cres(rows);
    std::vector<uint32_t> pred_res(rows, 0);
    predicate_evaluation(pred_cres, pred_res, rows, quanlity_data, discount_data, ship_data, sk, filter_time);
    aggregation(pred_cres, pred_res, Lvl1::n, extendedprice_data, discount_data_double, rows, sk, aggregation_time);
    cout << "End-to-End Time: " << (filter_time + aggregation_time) / 1000 << " s" << endl;

}

int main()
{
    query_evaluation(16);
}



