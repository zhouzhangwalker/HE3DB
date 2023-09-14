#pragma once

#ifdef USE_RANDEN
#include <randen.h>
#endif

#include <algorithm>
#include <array>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/array.hpp>

#include "lweParams.hpp"
#include "params.hpp"
#include "random"

namespace TFHEpp {
using namespace std;
struct lweKey {
    Key<lvl0param> lvl0;
    Key<lvl1param> lvl1;
    Key<lvl2param> lvl2;
    lweKey();
    template <class P>
    Key<P> get() const;
};

struct SecretKey {
    lweKey key;
    lweParams params;

    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(key.lvl0, key.lvl1, key.lvl2, params);
    }
};

template <class P>
void generate_hamming_secret_key(Key<P> &key, int hwt)
{
    random_device seed_gen;
    default_random_engine engine(seed_gen());
    key.fill(0);
    for (size_t i = 0; i < hwt; i++) {
        uniform_int_distribution<size_t> dist(0, P::k * P::n -1);
        size_t pos = dist(engine);
        key[pos] = 1;
  }
}
}  // namespace TFHEpp
