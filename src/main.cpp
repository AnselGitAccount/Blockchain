/* 
 * Author: Ansel Blumers
 * Last edit: June, 2020
 * 
 * https://ansel.braket.net
 */

#include "Block.h"

int main(int argc, char **argv)
{
    using size_t = std::size_t;

    // Generate a Genesis Block
    // Information is hard-coded.
    auto currenttime = bclock::now();
    std::string Genesis_data = "Genesis Block";
    size_t Genesis_hash = ComputeHash(static_cast<unsigned>(0), static_cast<size_t>(0), currenttime, Genesis_data);

    auto *Genesis_block = new Block<std::string, size_t>(
        static_cast<unsigned>(0),
        static_cast<size_t>(0),
        currenttime,
        Genesis_data,
        Genesis_hash,
        static_cast<size_t>(0));


    // Start a Block-Chain with the Genesis Block
    BlockChain<std::string, size_t> *myblockchain = new BlockChain<std::string, size_t>(Genesis_block);
    PrintChain(myblockchain);


    // Mine a block in the Block-Chain
    Miner<std::string, size_t> *myworker = new Miner<std::string, size_t>(4);
    myworker->setleadingzeros(3);
    printf("       Previous hash                 Hash               Nonce\n");
    for (int i = 0; i < 10; i++)
    {
        PrintLastBlock(myblockchain);
        myblockchain->MiningNewBlock("New message", myworker);
    }

}

/* TODO:
 * [X] 1. Overload hash comparison operator.
 * [X] 2. Rewrite print functionality.
 * [-] 3. Printf should be Hashtype dependent.
 * [ ] 4. Display Mining progress.
 */
