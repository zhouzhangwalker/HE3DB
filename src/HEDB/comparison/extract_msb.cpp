#include "extract_msb.h"
#include "HEDB/utils/utils.h"
namespace HEDB
{
    void ExtractMSB5(TLWELvl1 &res, const TLWELvl1 &tlwe, const TFHEEvalKey &ek, bool result_type)
    {
        MSBGateBootstrapping(res, tlwe, ek, result_type);
    }

    void ExtractMSB9(TLWELvl1 &res, const TLWELvl1 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TLWELvl1 shift_tlwe, sign_tlwe5;
        uint32_t scale_bits = std::numeric_limits<Lvl1::T>::digits - plain_bits;
        for (size_t i = 0; i <= Lvl1 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] << (plain_bits - 5);
        }
        MSBGateBootstrapping(sign_tlwe5, shift_tlwe, ek, ARITHMETIC);
        for (size_t i = 0; i <= Lvl1 :: n; i++)
        {
            shift_tlwe[i] = shift_tlwe[i] - sign_tlwe5[i];
        }
        IdeGateBootstrapping(shift_tlwe, shift_tlwe, scale_bits, ek);
        for (size_t i = 0; i <= Lvl1 :: n; i++)
        {
            res[i] = tlwe[i] - shift_tlwe[i];
        }
        ExtractMSB5(res, res, ek, result_type);
    }

    void ExtractMSB10(TLWELvl1 &res, const TLWELvl1 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TLWELvl1 shift_tlwe, sign_tlwe5;
        uint32_t scale_bits = std::numeric_limits<Lvl1::T>::digits - plain_bits;
        for (size_t i = 0; i <= Lvl1 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] << (plain_bits - 5);
        }
        MSBGateBootstrapping(sign_tlwe5, shift_tlwe, ek, ARITHMETIC);
        for (size_t i = 0; i <= Lvl1 :: n; i++)
        {
            shift_tlwe[i] = shift_tlwe[i] - sign_tlwe5[i];
        }
        IdeGateBootstrapping(shift_tlwe, shift_tlwe, scale_bits, ek);
        for (size_t i = 0; i <= Lvl1 :: n; i++)
        {
            res[i] = tlwe[i] - shift_tlwe[i];
        }
        ExtractMSB9(res, res, plain_bits - 4, ek, result_type);
    }

    void ExtractMSB11(TLWELvl2 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TLWELvl2 shift_tlwe, sign_tlwe6;
        uint32_t scale_bits = std::numeric_limits<Lvl2::T>::digits - plain_bits;
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] << (plain_bits - 6);
        }
        MSBGateBootstrapping(sign_tlwe6, shift_tlwe, ek, ARITHMETIC);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = shift_tlwe[i] - sign_tlwe6[i];
        }
        IdeGateBootstrapping(shift_tlwe, shift_tlwe, scale_bits, ek);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            res[i] = tlwe[i] - shift_tlwe[i];
        }
        MSBGateBootstrapping(res, res, ek, result_type);
    }

    void ExtractMSB16(TLWELvl2 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TLWELvl2 shift_tlwe, sign_tlwe6;
        uint32_t scale_bits = std::numeric_limits<Lvl2::T>::digits - plain_bits;
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] << (plain_bits - 6);
        }
        MSBGateBootstrapping(sign_tlwe6, shift_tlwe, ek, ARITHMETIC);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = shift_tlwe[i] - sign_tlwe6[i];
        }
        IdeGateBootstrapping(shift_tlwe, shift_tlwe, scale_bits, ek);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            res[i] = tlwe[i] - shift_tlwe[i];
        }
        ExtractMSB11(res, res, plain_bits - 5, ek, result_type);
    }

    void ExtractMSB21(TLWELvl2 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TLWELvl2 shift_tlwe, sign_tlwe6;
        uint32_t scale_bits = std::numeric_limits<Lvl2::T>::digits - plain_bits;
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] << (plain_bits - 6);
        }
        MSBGateBootstrapping(sign_tlwe6, shift_tlwe, ek, ARITHMETIC);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = shift_tlwe[i] - sign_tlwe6[i];
        }
        IdeGateBootstrapping(shift_tlwe, shift_tlwe, scale_bits, ek);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            res[i] = tlwe[i] - shift_tlwe[i];
        }
        ExtractMSB16(res, res, plain_bits - 5, ek, result_type);
    }

    void ImExtractMSB5(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TFHEpp::IdentityKeySwitch<TFHEpp::lvl21param>(res, tlwe, *ek.iksklvl21);
        ExtractMSB5(res, res, ek, result_type);
    }

    void ImExtractMSB9(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TFHEpp::IdentityKeySwitch<TFHEpp::lvl21param>(res, tlwe, *ek.iksklvl21);
        ExtractMSB9(res, res, plain_bits, ek, result_type);
    }

    void ImExtractMSB14(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TLWELvl2 shift_tlwe, sign_tlwe6;
        uint32_t scale_bits = std::numeric_limits<Lvl2::T>::digits - plain_bits;
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
             shift_tlwe[i] = tlwe[i] << (plain_bits - 6);
        }
        MSBGateBootstrapping(sign_tlwe6, shift_tlwe, ek, ARITHMETIC);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = shift_tlwe[i] - sign_tlwe6[i];
        }
        IdeGateBootstrapping(shift_tlwe, shift_tlwe, scale_bits, ek);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] - shift_tlwe[i];
        }
        TFHEpp::IdentityKeySwitch<TFHEpp::lvl21param>(res, shift_tlwe, *ek.iksklvl21);
        ExtractMSB9(res, res, plain_bits - 5, ek, result_type);
    }

    void ImExtractMSB19(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TLWELvl2 shift_tlwe, sign_tlwe6;
        uint32_t scale_bits = std::numeric_limits<Lvl2::T>::digits - plain_bits;
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] << (plain_bits - 6);
        }
        MSBGateBootstrapping(sign_tlwe6, shift_tlwe, ek, ARITHMETIC);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = shift_tlwe[i] - sign_tlwe6[i];
        }
        IdeGateBootstrapping(shift_tlwe, shift_tlwe, scale_bits, ek);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] - shift_tlwe[i];
        }
        ImExtractMSB14(res, shift_tlwe, plain_bits - 5, ek, result_type);
    }

    void ImExtractMSB24(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TLWELvl2 shift_tlwe, sign_tlwe6;
        uint32_t scale_bits = std::numeric_limits<Lvl2::T>::digits - plain_bits;
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] << (plain_bits - 6);
        }
        MSBGateBootstrapping(sign_tlwe6, shift_tlwe, ek, ARITHMETIC);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = shift_tlwe[i] - sign_tlwe6[i];
        }
        IdeGateBootstrapping(shift_tlwe, shift_tlwe, scale_bits, ek);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] - shift_tlwe[i];
        }
        ImExtractMSB19(res, shift_tlwe, plain_bits - 5, ek, result_type);
    }

    void ImExtractMSB29(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TLWELvl2 shift_tlwe, sign_tlwe6;
        uint32_t scale_bits = std::numeric_limits<Lvl2::T>::digits - plain_bits;
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] << (plain_bits - 6);
        }
        MSBGateBootstrapping(sign_tlwe6, shift_tlwe, ek, ARITHMETIC);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = shift_tlwe[i] - sign_tlwe6[i];
        }
        IdeGateBootstrapping(shift_tlwe, shift_tlwe, scale_bits, ek);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] - shift_tlwe[i];
        }
        ImExtractMSB24(res, shift_tlwe, plain_bits - 5, ek, result_type);
    }

    void ImExtractMSB33(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        TLWELvl2 shift_tlwe, sign_tlwe6;
        uint32_t scale_bits = std::numeric_limits<Lvl2::T>::digits - plain_bits;
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] << (plain_bits - 6);
        }
        MSBGateBootstrapping(sign_tlwe6, shift_tlwe, ek, ARITHMETIC);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = shift_tlwe[i] - sign_tlwe6[i];
        }
        IdeGateBootstrapping(shift_tlwe, shift_tlwe, scale_bits, ek);
        for (size_t i = 0; i <= Lvl2 :: n; i++)
        {
            shift_tlwe[i] = tlwe[i] - shift_tlwe[i];
        }
        ImExtractMSB29(res, shift_tlwe, plain_bits - 5, ek, result_type);
    }

    void HomMSB(TLWELvl1 &res, const TLWELvl1 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        if (plain_bits <= 5) ExtractMSB5(res, tlwe, ek, result_type);
        else if (plain_bits <= 9) ExtractMSB9(res, tlwe, plain_bits, ek, result_type);
        else if  (plain_bits == 10) ExtractMSB10(res, tlwe, plain_bits, ek, result_type);
        else throw std::invalid_argument("Plain bits out. ");
    }

    void HomMSB(TLWELvl1 &res, const TLWELvl2 &tlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type)
    {
        if (plain_bits <= 5) ImExtractMSB5(res, tlwe, plain_bits, ek, result_type);
        else if (plain_bits <= 9) ImExtractMSB9(res, tlwe, plain_bits, ek, result_type);
        else if (plain_bits <= 14) ImExtractMSB14(res, tlwe, plain_bits, ek, result_type);
        else if (plain_bits <= 19) ImExtractMSB19(res, tlwe, plain_bits, ek, result_type);
        else if (plain_bits <= 24) ImExtractMSB24(res, tlwe, plain_bits, ek, result_type);
        else if (plain_bits <= 29) ImExtractMSB29(res, tlwe, plain_bits, ek, result_type);
        else if (plain_bits <= 33) ImExtractMSB33(res, tlwe, plain_bits, ek, result_type);
        else throw std::invalid_argument("Plain bits out. ");
    }

    void HomMSB(std::vector<TLWELvl1> &res, const TRLWELvl1 &trlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type, size_t num_threads)
    {
        res.resize(Lvl1::n);
        ThreadPool pool(num_threads);
        const size_t work_load = (Lvl1::n + num_threads - 1) / num_threads;
        for (int w = 0; w < num_threads; ++w)
        {
            size_t start = w * work_load;
            size_t end = std::min<size_t>(start + work_load, Lvl1::n);

                if (end > start)
                {
                    pool.enqueue([&](size_t s, size_t e)
                    {
                        for (size_t j = s; j < e; ++j)
                        {
                            TFHEpp::SampleExtractIndex<Lvl1>(res[j], trlwe, j);
                            HomMSB(res[j], res[j], plain_bits, ek, result_type);
                        }
                        
                    }, 
                    start, end);
                }
        }
        pool.wait_until_empty();
        pool.wait_until_nothing_in_flight();
    }

    void HomMSB(std::vector<TLWELvl1> &res, const TRLWELvl2 &trlwe, uint32_t plain_bits, const TFHEEvalKey &ek, bool result_type, size_t num_threads)
    { 
        res.resize(Lvl2::n);
        ThreadPool pool(num_threads);
        const size_t work_load = (Lvl2::n + num_threads - 1) / num_threads;
        for (int w = 0; w < num_threads; ++w)
        {
            size_t start = w * work_load;
            size_t end = std::min<size_t>(start + work_load, Lvl1::n);

                if (end > start)
                {
                    pool.enqueue([&](size_t s, size_t e)
                    {
                        for (size_t j = s; j < e; ++j)
                        {
                            TLWELvl2 temp;
                            TFHEpp::SampleExtractIndex<Lvl2>(temp, trlwe, j);
                            HomMSB(res[j], temp, plain_bits, ek, result_type);
                        }
                        
                    }, 
                    start, end);
                }
        }
        pool.wait_until_empty();
        pool.wait_until_nothing_in_flight();
    }
} // namespace HEDB
