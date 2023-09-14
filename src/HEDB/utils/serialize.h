#pragma once
#include <tfhe++.hpp>


namespace HEDB
{
    void generate_secret_key();

    void load_secret_key(TFHEpp::SecretKey &sk);

    void generate_bklvl01_key();

    void load_bklvl01_key(TFHEpp::EvalKey &ek);

    void generate_bklvl02_key();

    void load_bklvl02_key(TFHEpp::EvalKey &ek);

    void generate_bkfftlvl01_key();

    void load_bkfftlvl01_key(TFHEpp::EvalKey &ek);

    void generate_bkfftlvl02_key();

    void load_bkfftlvl02_key(TFHEpp::EvalKey &ek);

    void generate_bknttlvl01_key();

    void load_bknttlvl01_key(TFHEpp::EvalKey &ek);

    void generate_bknttlvl02_key();

    void load_bknttlvl02_key(TFHEpp::EvalKey &ek);

    void generate_iksklvl10_key();

    void load_iksklvl10_key(TFHEpp::EvalKey &ek);

    void generate_iksklvl11_key();

    void load_iksklvl11_key(TFHEpp::EvalKey &ek);

    void generate_iksklvl20_key();

    void load_iksklvl20_key(TFHEpp::EvalKey &ek);

    void generate_iksklvl21_key();

    void load_iksklvl21_key(TFHEpp::EvalKey &ek);

    void generate_iksklvl22_key();

    void load_iksklvl22_key(TFHEpp::EvalKey &ek);


} // namespace HEDB
