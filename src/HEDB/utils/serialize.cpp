#include "HEDB/utils/serialize.h"
#include <fstream>
#include <iostream>

namespace HEDB
{
    void generate_secret_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ofstream ofs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            sk.serialize(ar);
        }
    }

    void load_secret_key(TFHEpp::SecretKey &sk)
    {
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }
    }

    void generate_bklvl01_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }

        TFHEpp::EvalKey ek;
        ek.emplacebk<TFHEpp::lvl01param>(sk);
        {
            std::ofstream ofs{"./../evalkey/bklvl01.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ek.serialize_bklvl01(ar);
        }
    }

    void load_bklvl01_key(TFHEpp::EvalKey &ek)
    {
        {
            std::ifstream ifs{"./../evalkey/bklvl01.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            ek.serialize_bklvl01(ar);
        }
    }

    void generate_bklvl02_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }

        TFHEpp::EvalKey ek;
        ek.emplacebk<TFHEpp::lvl02param>(sk);
        {
            std::ofstream ofs{"./../evalkey/bklvl02.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ek.serialize_bklvl02(ar);
        }
    }

    void load_bklvl02_key(TFHEpp::EvalKey &ek)
    {
        {
            std::ifstream ifs{"./../evalkey/bklvl02.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            ek.serialize_bklvl02(ar);
        }
    }

    void generate_bkfftlvl01_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }

        TFHEpp::EvalKey ek;
        ek.emplacebkfft<TFHEpp::lvl01param>(sk);
        {
            std::ofstream ofs{"./../evalkey/bkfftlvl01.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ek.serialize_bkfftlvl01(ar);
        }
    }

    void load_bkfftlvl01_key(TFHEpp::EvalKey &ek)
    {
        {
            std::ifstream ifs{"./../evalkey/bkfftlvl01.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            ek.serialize_bkfftlvl01(ar);
        }
    }

    void generate_bkfftlvl02_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }

        TFHEpp::EvalKey ek;
        ek.emplacebkfft<TFHEpp::lvl02param>(sk);
        {
            std::ofstream ofs{"./../evalkey/bkfftlvl02.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ek.serialize_bkfftlvl02(ar);
        }
    }

    void load_bkfftlvl02_key(TFHEpp::EvalKey &ek)
    {
        {
            std::ifstream ifs{"./../evalkey/bkfftlvl02.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            ek.serialize_bkfftlvl02(ar);
        }
    }

    void generate_bknttlvl01_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }

        TFHEpp::EvalKey ek;
        ek.emplacebkntt<TFHEpp::lvl01param>(sk);
        {
            std::ofstream ofs{"./../evalkey/bknttlvl01.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ek.serialize_bknttlvl01(ar);
        }
    }

    void load_bknttlvl01_key(TFHEpp::EvalKey &ek)
    {
        {
            std::ifstream ifs{"./../evalkey/bknttlvl01.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            ek.serialize_bknttlvl01(ar);
        }
    }

    void generate_bknttlvl02_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }

        TFHEpp::EvalKey ek;
        ek.emplacebkntt<TFHEpp::lvl02param>(sk);
        {
            std::ofstream ofs{"./../evalkey/bknttlvl02.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ek.serialize_bknttlvl02(ar);
        }
    }

    void load_bknttlvl02_key(TFHEpp::EvalKey &ek)
    {
        {
            std::ifstream ifs{"./../evalkey/bknttlvl02.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            ek.serialize_bknttlvl02(ar);
        }
    }

    void generate_iksklvl10_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }

        TFHEpp::EvalKey ek;
        ek.emplaceiksk<TFHEpp::lvl10param>(sk);
        {
            std::ofstream ofs{"./../evalkey/iksklvl10.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ek.serialize_iksklvl10(ar);
        }
    }

    void load_iksklvl10_key(TFHEpp::EvalKey &ek)
    {
        {
            std::ifstream ifs{"./../evalkey/iksklvl10.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            ek.serialize_iksklvl10(ar);
        }
    }

    void generate_iksklvl11_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }

        TFHEpp::EvalKey ek;
        ek.emplaceiksk<TFHEpp::lvl11param>(sk);
        {
            std::ofstream ofs{"./../evalkey/iksklvl11.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ek.serialize_iksklvl11(ar);
        }
    }

    void load_iksklvl11_key(TFHEpp::EvalKey &ek)
    {
        {
            std::ifstream ifs{"./../evalkey/iksklvl11.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            ek.serialize_iksklvl11(ar);
        }
    }

    void generate_iksklvl20_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }

        TFHEpp::EvalKey ek;
        ek.emplaceiksk<TFHEpp::lvl20param>(sk);
        {
            std::ofstream ofs{"./../evalkey/iksklvl20.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ek.serialize_iksklvl20(ar);
        }
    }

    void load_iksklvl20_key(TFHEpp::EvalKey &ek)
    {
        {
            std::ifstream ifs{"./../evalkey/iksklvl20.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            ek.serialize_iksklvl20(ar);
        }
    }

    void generate_iksklvl21_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }

        TFHEpp::EvalKey ek;
        ek.emplaceiksk<TFHEpp::lvl21param>(sk);
        {
            std::ofstream ofs{"./../evalkey/iksklvl21.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ek.serialize_iksklvl21(ar);
        }
    }

    void load_iksklvl21_key(TFHEpp::EvalKey &ek)
    {
        {
            std::ifstream ifs{"./../evalkey/iksklvl21.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            ek.serialize_iksklvl21(ar);
        }
    }

    void generate_iksklvl22_key()
    {
        TFHEpp::SecretKey sk;
        {
            std::ifstream ifs{"./../evalkey/secret.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            sk.serialize(ar);
        }

        TFHEpp::EvalKey ek;
        ek.emplaceiksk<TFHEpp::lvl22param>(sk);
        {
            std::ofstream ofs{"./../evalkey/iksklvl22.key", std::ios::binary};
            cereal::PortableBinaryOutputArchive ar(ofs);
            ek.serialize_iksklvl22(ar);
        }
    }

    void load_iksklvl22_key(TFHEpp::EvalKey &ek)
    {
        {
            std::ifstream ifs{"./../evalkey/iksklvl22.key", std::ios::binary};
            cereal::PortableBinaryInputArchive ar(ifs);
            ek.serialize_iksklvl22(ar);
        }
    }

} // namespace HEDB