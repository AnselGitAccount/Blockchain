#ifndef BLOCK_H_
#define BLOCK_H_

#include "Miner.h"

template<typename Datatype, typename Hashtype>
class Block {
public:
    Block(unsigned index, Hashtype previoushash, Typetime timestamp, Datatype data, Hashtype hash, size_t nounce):
        index_(index),
        previoushash_(previoushash),
        timestamp_(timestamp),
        data_(data),
        hash_(hash),
        nounce_(nounce) {}

    ~Block() {}

    auto get_index() 		{ return index_; }
    auto get_previoushash() { return previoushash_; }
    auto get_timestamp()	{ return timestamp_; }
    auto get_hash()			{ return hash_; }
    auto get_data()			{ return data_; }
    auto get_nounce()		{ return nounce_; }

private:
    unsigned 		index_;
    Hashtype	previoushash_;
    Typetime		timestamp_;
    Datatype 	data_;
    Hashtype	hash_;
    size_t 		nounce_;

};




template<typename Datatype, typename Hashtype>
class BlockChain {
public:
    // instantiate by providing a genesis block
    BlockChain( Block<Datatype,Hashtype>* FirstBlock ) {	AppendBlock( FirstBlock ); }

    Block<Datatype,Hashtype>* GenerateBlock( Block<Datatype,Hashtype>* PreviousBlock, Datatype new_data ) {
        auto new_index 		= PreviousBlock->get_index() + 1;
        auto previous_hash 	= PreviousBlock->get_hash();
        auto time_now 		= bclock::now();
        auto new_hash 		= ComputeHash<Datatype,Hashtype>( new_index, previous_hash, time_now, new_data );

        auto* new_Block = new Block<Datatype,Hashtype>( new_index, previous_hash, time_now, new_data, new_hash );
        return new_Block;
    }

    void MiningNewBlock( const Datatype& data, Miner<Datatype,Hashtype>* worker ) {
        size_t 		newnounce = 0;
        Hashtype 	newhash;
        auto* 		LastBlock = chain.back();
        worker->Mine( LastBlock->get_index(), data, LastBlock->get_hash(), newhash, newnounce );
        auto* 		MinedBlock = new Block<Datatype,Hashtype>(
                                LastBlock->get_index()+1, LastBlock->get_hash(), bclock::now(), data, newhash, newnounce);
        // std::cout << newhash << " " << std::bitset<64>(newhash) << std::endl;
        this->AppendBlock( MinedBlock );
    }


    void ValidateNewBlock( Block<Datatype,Hashtype>* NewBlock ) {
        if( chain.size() > 0 ) {
            auto* LastBlock = chain.back();
            if( LastBlock->get_index()+1 != NewBlock->get_index() )
                std::cerr << "Block #" << NewBlock->get_index() << " is invalid due to invalid index.\n";

            // Simple validation rule: Hash of previous block matches PreviousHash of this block.
            if( LastBlock->get_hash() != NewBlock->get_previoushash() )
                std::cerr << "Block #" << NewBlock->get_index() << " is invalid due to invalid previous-hash\n";
        } else {
            // first block in chain. Do nothing!
        }
    }

    void AppendBlock( Block<Datatype,Hashtype>* new_block ) {
        ValidateNewBlock( new_block );
        chain.push_back( new_block );
    }

    void AppendChain( BlockChain<Datatype,Hashtype>* new_chain ) {
        for( auto& B: new_chain->chain ) AppendBlock( B );
    }


    void PrintChain( bool flg_entire, size_t strtpos = 0, size_t endpos = 0 ) {  // Start and end position are inclusive.
        if( flg_entire ) {				// print entire chain
            DYNAMIC_ASSERT( strtpos==0,  "Invalid blockchain print instruction.")
            DYNAMIC_ASSERT( endpos==0,   "Invalid blockchain print instruction.")
            strtpos = 0;
            endpos = chain.size()-1;
        } else if( !flg_entire ) {		// print a segment
            DYNAMIC_ASSERT( strtpos<endpos,  "Invalid blockchain print instruction.")
        }
        printf("       Previous hash                 Hash               Nounce\n");
        for( size_t i=strtpos; i<=endpos; i++ ) {
            printf("%20zu %20zu %20zu\n", chain[i]->get_previoushash(), chain[i]->get_hash(), chain[i]->get_nounce());
        }
        std::cout << "Block #" << strtpos << " to Block #" << endpos << " out of total " << chain.size() << " Blocks are printed.\n";
    }

    void PrintLastBlock() {
        printf("%20zu %20zu %20zu\n", chain.back()->get_previoushash(), chain.back()->get_hash(), chain.back()->get_nounce());
    }

private:
    std::vector< Block<Datatype,Hashtype>* > chain;
    std::string name;
};




#endif