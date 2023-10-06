#include <iostream>
#include <chrono>
#include <random>
#include "HEDB/comparison/comparison.h"
#include "HEDB/utils/utils.h"

using namespace HEDB;

// In comparison, if a and b are x bits, a - b is (x+1) bits
// If we define the input data is p bits, than the scale should be Q/2p
// The database is the same as comparison
// tlwelvl1_comparison_test include 1 - 9 bits, here the plain bits represent the range of input data.
void tlwelvl1_comparison_test(uint32_t plain_bits, int num_test)
{
    std::cout << "------ Test of TLWE lvl1param Comparison Function------" << std::endl;
    std::cout << "Test Time : " << num_test << std::endl;
    std::cout << "Plain bits : " << plain_bits << std::endl;
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());
    using P = Lvl1;
    TFHESecretKey sk;
    TFHEEvalKey ek;
    // load_secret_key(sk);
    // load_iksklvl10_key(ek);
    // load_bkfftlvl01_key(ek);
    ek.emplacebkfft<Lvl01>(sk);
    ek.emplaceiksk<Lvl10>(sk);
    uint32_t scale_bits;
    std::vector<uint32_t> error_time(5, 0);
    std::vector<double> comparison_time(5, 0.);
    // For simplicity，the input range is [0, 2^p -1]
    std::uniform_int_distribution<typename P::T> message(0, (1 << plain_bits) - 1);
    std::uniform_int_distribution<typename P::T> type(0, 1);
    scale_bits = std::numeric_limits<P::T>::digits - plain_bits - 1;
    typename P::T p0,p1, gres, geres, lres, leres, eres, dgres, dgeres, dlres, dleres, deres;
    TFHEpp::TLWE<P> c0,c1,c;
    TLWELvl1 cres;
    std::chrono::system_clock::time_point start, end;
    for (int test = 0; test < num_test; test++) 
    {
        
        p1 = message(engine);
        bool result_type = ARITHMETIC;
        if (p0 > p1) gres = 1;
        else gres = 0;
        if (p0 >= p1) geres = 1;
        else geres = 0;
        if (p0 < p1) lres = 1;
        else lres = 0;
        if (p0 <= p1) leres = 1;
        else leres = 0;
        if (p0 == p1) eres = 1;
        else eres = 0;
        c0 = TFHEpp::tlweSymInt32Encrypt<P>(p0, P::α, pow(2., scale_bits), sk.key.get<P>());
        c1 = TFHEpp::tlweSymInt32Encrypt<P>(p1, P::α, pow(2., scale_bits), sk.key.get<P>());

        //Greater than
        start = std::chrono::system_clock::now();
        greater_than<P>(c0, c1, cres, plain_bits, ek, result_type);
        end = std::chrono::system_clock::now();
        ARI_to_LOG(cres, cres, ek);
        result_type = LOGIC;
        comparison_time[0] += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(IS_ARITHMETIC(result_type)) dgres = TFHEpp::tlweSymInt32Decrypt<P>(cres, pow(2., 31), sk.key.lvl1);
        else dgres = TFHEpp::tlweSymDecrypt<P>(cres, sk.key.lvl1);
        if (gres != dgres) error_time[0] += 1;

        // Greater than or euqal to
        start = std::chrono::system_clock::now();
        greater_than_equal<P>(c0, c1, cres, plain_bits, ek, result_type);
        end = std::chrono::system_clock::now();
        comparison_time[1] += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(IS_ARITHMETIC(result_type)) dgeres = TFHEpp::tlweSymInt32Decrypt<P>(cres, pow(2., 31), sk.key.lvl1);
        else dgeres = TFHEpp::tlweSymDecrypt<P>(cres, sk.key.lvl1);
        if (geres != dgeres) error_time[1] += 1;

        // less than 
        start = std::chrono::system_clock::now();
        less_than<P>(c0, c1, cres, plain_bits, ek, result_type);
        end = std::chrono::system_clock::now();
        comparison_time[2] += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(IS_ARITHMETIC(result_type)) dlres = TFHEpp::tlweSymInt32Decrypt<P>(cres, pow(2., 31), sk.key.lvl1);
        else dlres = TFHEpp::tlweSymDecrypt<P>(cres, sk.key.lvl1);
        if (lres != dlres) error_time[2] += 1;

        //less than or equal to
        start = std::chrono::system_clock::now();
        less_than_equal<P>(c0, c1, cres, plain_bits, ek, result_type);
        end = std::chrono::system_clock::now();
        comparison_time[3] += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(IS_ARITHMETIC(result_type)) dleres = TFHEpp::tlweSymInt32Decrypt<P>(cres, pow(2., 31), sk.key.lvl1);
        else dleres = TFHEpp::tlweSymDecrypt<P>(cres, sk.key.lvl1);
        if (leres != dleres) error_time[3] += 1;

        //equal to
        start = std::chrono::system_clock::now();
        equal<P>(c0, c1, cres, plain_bits, ek, result_type);
        end = std::chrono::system_clock::now();
        comparison_time[4] += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(IS_ARITHMETIC(result_type)) deres = TFHEpp::tlweSymInt32Decrypt<P>(cres, pow(2., 31), sk.key.lvl1);
        else deres = TFHEpp::tlweSymDecrypt<P>(cres, sk.key.lvl1);
        if (eres != deres) error_time[4] += 1;
    }
    std::cout << "Test greater than, greater than or equal, less than, less than or equal and equal to" << std::endl;
    for (size_t i = 0; i < 5; i++)
    {
        std::cout << "Error time" << i << " : " << error_time[i] << std::endl;
        std::cout << "Time" << i << " : " << comparison_time[i] / num_test << "ms" << std::endl;
    }
    
    
}

// Lvl2 include 10 -24 bits
void tlwelvl2_comparison_test(uint32_t plain_bits, int num_test)
{
    std::cout << "------ Test of TLWE lvl2param Comparison Function------" << std::endl;
    std::cout << "Test Time : " << num_test << std::endl;
    std::cout << "Plain bits : " << plain_bits << std::endl;
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());
    using P = Lvl2;
    TFHESecretKey sk;
    TFHEEvalKey ek;
    // load_secret_key(sk);
    // load_bkfftlvl01_key(ek);
    // load_bkfftlvl02_key(ek);
    // load_iksklvl10_key(ek);
    // load_iksklvl20_key(ek);
    // load_iksklvl21_key(ek);
    
    ek.emplacebkfft<Lvl01>(sk);
    ek.emplacebkfft<Lvl02>(sk);
    ek.emplaceiksk<Lvl20>(sk);
    ek.emplaceiksk<Lvl10>(sk);
    ek.emplaceiksk<Lvl21>(sk);
    uint32_t scale_bits;
    std::vector<uint32_t> error_time(5, 0);
    std::vector<double> comparison_time(5, 0.);
    std::uniform_int_distribution<typename P::T> message(0, (1 << plain_bits) - 1);
    std::uniform_int_distribution<typename P::T> type(0, 1);
    scale_bits = std::numeric_limits<P::T>::digits - plain_bits - 1;
    typename P::T p0,p1, gres, geres, lres, leres, eres, dgres, dgeres, dlres, dleres, deres;
    TFHEpp::TLWE<P> c0,c1,c;
    TLWELvl1 cres;
    std::chrono::system_clock::time_point start, end;
    for (int test = 0; test < num_test; test++) 
    {
        p0 = message(engine);
        p1 = message(engine);
        bool result_type = LOGIC;
        if (p0 > p1) gres = 1;
        else gres = 0;
        if (p0 >= p1) geres = 1;
        else geres = 0;
        if (p0 < p1) lres = 1;
        else lres = 0;
        if (p0 <= p1) leres = 1;
        else leres = 0;
        if (p0 == p1) eres = 1;
        else eres = 0;
        c0 = TFHEpp::tlweSymInt32Encrypt<P>(p0, P::α, pow(2., scale_bits), sk.key.get<P>());
        c1 = TFHEpp::tlweSymInt32Encrypt<P>(p1, P::α, pow(2., scale_bits), sk.key.get<P>());

        //Greater than
        start = std::chrono::system_clock::now();
        greater_than<P>(c0, c1, cres, plain_bits, ek, result_type);
        end = std::chrono::system_clock::now();
        comparison_time[0] += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(IS_ARITHMETIC(result_type)) dgres = TFHEpp::tlweSymInt32Decrypt<Lvl1>(cres, pow(2., 31), sk.key.lvl1);
        else dgres = TFHEpp::tlweSymDecrypt<Lvl1>(cres, sk.key.lvl1);
        if (gres != dgres) error_time[0] += 1;

        // Greater than or euqal to
        start = std::chrono::system_clock::now();
        greater_than_equal<P>(c0, c1, cres, plain_bits, ek, result_type);
        end = std::chrono::system_clock::now();
        comparison_time[1] += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(IS_ARITHMETIC(result_type)) dgeres = TFHEpp::tlweSymInt32Decrypt<Lvl1>(cres, pow(2., 31), sk.key.lvl1);
        else dgeres = TFHEpp::tlweSymDecrypt<Lvl1>(cres, sk.key.lvl1);
        if (geres != dgeres) error_time[1] += 1;

        // less than 
        start = std::chrono::system_clock::now();
        less_than<P>(c0, c1, cres, plain_bits, ek, result_type);
        end = std::chrono::system_clock::now();
        comparison_time[2] += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(IS_ARITHMETIC(result_type)) dlres = TFHEpp::tlweSymInt32Decrypt<Lvl1>(cres, pow(2., 31), sk.key.lvl1);
        else dlres = TFHEpp::tlweSymDecrypt<Lvl1>(cres, sk.key.lvl1);
        if (lres != dlres) error_time[2] += 1;

        //less than or equal to
        start = std::chrono::system_clock::now();
        less_than_equal<P>(c0, c1, cres, plain_bits, ek, result_type);
        end = std::chrono::system_clock::now();
        comparison_time[3] += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(IS_ARITHMETIC(result_type)) dleres = TFHEpp::tlweSymInt32Decrypt<Lvl1>(cres, pow(2., 31), sk.key.lvl1);
        else dleres = TFHEpp::tlweSymDecrypt<Lvl1>(cres, sk.key.lvl1);
        if (leres != dleres) error_time[3] += 1;

        //equal to
        start = std::chrono::system_clock::now();
        equal<P>(c0, c1, cres, plain_bits, ek, result_type);
        end = std::chrono::system_clock::now();
        comparison_time[4] += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if(IS_ARITHMETIC(result_type)) deres = TFHEpp::tlweSymInt32Decrypt<Lvl1>(cres, pow(2., 31), sk.key.lvl1);
        else deres = TFHEpp::tlweSymDecrypt<Lvl1>(cres, sk.key.lvl1);
        if (eres != deres) error_time[4] += 1;
    }
    std::cout << "Test greater than, greater than or equal, less than, less than or equal and equal to" << std::endl;
    for (size_t i = 0; i < 5; i++)
    {
        std::cout << "Error time" << i << " : " << error_time[i] << std::endl;
        std::cout << "Time" << i << " : " << comparison_time[i] / num_test << "ms" << std::endl;
    }
    
    
}

int main()
{
    int num_test = 1000;

    // tlwelvl1_comparison_test(4, num_test);
    // tlwelvl2_comparison_test(16, num_test);
    // For overall multi-thread test
    // for (size_t i = 4; i <= 9; i++)
    // {
    //     multi_thread(tlwelvl1_comparison_test, 10, 10, i);
    //     // tlwelvl1_comparison_test(i);
    // }
    // for (size_t i = 4; i <= 9; i++)
    // {
    //     multi_thread(tlwelvl2_comparison_test, 10, 10, i, num_test);
    // }


    // For overall single-thread test
    for (size_t i = 4; i <= 9; i++)
    {
        tlwelvl1_comparison_test(i, num_test);
    }
    for (size_t i = 4; i <= 32; i++)
    {
        tlwelvl2_comparison_test(i, num_test);
    }
    
     
}

