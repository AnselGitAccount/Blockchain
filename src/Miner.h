#ifndef MINER_H_
#define MINER_H_

#include "Utility.h"

template <typename Datatype, typename Hashtype>
struct Miner
{
    Miner(const unsigned i) : nleadzero(i) {}
    void setleadingzeros(const unsigned i) { nleadzero = i; }

    // count leading zero bits.
    void Mine(const unsigned index, const Datatype input, const Hashtype previoushash, Hashtype &newhash, size_t &newnonce)
    {
        size_t nonce = 0;   // initial guess.
        while (1)
        {
            auto data_hash = std::hash<Datatype>{}(std::to_string(index) + input + std::to_string(nonce));
            if (__builtin_clzll(data_hash) == nleadzero)
            {
                newhash = data_hash;
                newnonce = nonce;
                //			std::cout << data_hash << " " << count_leading_zero(data_hash) << std::endl;
                break;
            }
            nonce++;
        }
    }

    unsigned nleadzero = 0;
};

#endif
