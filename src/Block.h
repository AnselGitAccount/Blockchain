#ifndef BLOCK_H_
#define BLOCK_H_

#include "Miner.h"

template <typename Datatype, typename Hashtype>
class Block
{
public:
    Block(unsigned index, Hashtype previoushash, Typetime timestamp, Datatype data, Hashtype hash, size_t nounce) : index_(index),
                                                                                                                    previoushash_(previoushash),
                                                                                                                    timestamp_(timestamp),
                                                                                                                    data_(data),
                                                                                                                    hash_(hash),
                                                                                                                    nounce_(nounce) {}

    auto get_index() { return index_; }
    auto get_previoushash() { return previoushash_; }
    auto get_timestamp() { return timestamp_; }
    auto get_hash() { return hash_; }
    auto get_data() { return data_; }
    auto get_nounce() { return nounce_; }

private:
    unsigned index_;
    Hashtype previoushash_;
    Typetime timestamp_;
    Datatype data_;
    Hashtype hash_;
    size_t nounce_;
};

template <typename Datatype, typename Hashtype>
class BlockChain
{
public:
    // Block-Chain starts with a Genesis block
    explicit BlockChain(Block<Datatype, Hashtype> *FirstBlock) { AppendBlock(FirstBlock); }

    // Disable copy constructor and copy assignment.
    BlockChain(const BlockChain &) = delete;
    BlockChain &operator=(const BlockChain &) = delete;

    // Generate a new block by hashing previous block's information.
    // No mining involved.
    Block<Datatype, Hashtype> *GenerateBlock(Block<Datatype, Hashtype> *PreviousBlock, Datatype new_data)
    {
        auto new_index = PreviousBlock->get_index() + 1;
        auto previous_hash = PreviousBlock->get_hash();
        auto time_now = bclock::now();
        auto new_hash = ComputeHash<Datatype, Hashtype>(new_index, previous_hash, time_now, new_data);

        auto *new_Block = new Block<Datatype, Hashtype>(new_index, previous_hash, time_now, new_data, new_hash);
        return new_Block;
    }

    // Mining a new block & append to the Chain;
    // Requires a new message and previous block's data.
    void MiningNewBlock(const Datatype &data, Miner<Datatype, Hashtype> *worker)
    {
        size_t newnounce = 0;
        Hashtype newhash;
        auto *LastBlock = chain.back();
        worker->Mine(LastBlock->get_index(), data, LastBlock->get_hash(), newhash, newnounce);

        auto *MinedBlock = new Block<Datatype, Hashtype>(
            LastBlock->get_index() + 1, LastBlock->get_hash(), bclock::now(), data, newhash, newnounce);
        // std::cout << newhash << " " << std::bitset<64>(newhash) << std::endl;
        this->AppendBlock(MinedBlock);
    }

    // Validate the integrity of blocks for appending to the chain.
    void ValidateNewBlock(Block<Datatype, Hashtype> *NewBlock)
    {
        // Do nothing for first block in chain.
        if (chain.size() > 0)
        {
            auto *LastBlock = chain.back();
            if (LastBlock->get_index() + 1 != NewBlock->get_index())
                std::cerr << "Block #" << NewBlock->get_index() << " is invalid due to invalid index.\n";

            // Simple validation rule: Hash of previous block matches PreviousHash of this block.
            if (LastBlock->get_hash() != NewBlock->get_previoushash())
                std::cerr << "Block #" << NewBlock->get_index() << " is invalid due to invalid previous-hash\n";
        }
    }

    /*
    ValidateIncomingChain
    ConflictResolution
    */

    // Append a block to my Chain.
    void AppendBlock(Block<Datatype, Hashtype> *new_block)
    {
        ValidateNewBlock(new_block);
        chain.push_back(new_block);
    }

    // Append another Chain to my Chain.
    void AppendChain(BlockChain<Datatype, Hashtype> *new_chain)
    {
        for (auto &B : new_chain->chain)
            AppendBlock(B);
    }

    int size() const
    {
        return chain.size();
    }

    const auto *getChain() const
    {
        return &chain;
    }

private:
    std::vector<Block<Datatype, Hashtype> *> chain;
    std::string name;
};

#endif