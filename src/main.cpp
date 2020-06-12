/* 
 * Author: Ansel Blumers
 * Date: Jan, 2018
 * 
 * https://ansel.braket.net
 */


#include "Block.h"


int main (int argc, char** argv) {

    using size_t = std::size_t;

    // Generate Genesis Block
    auto currenttime 			= bclock::now();
    std::string Genesis_data 	= "Genesis Block";
    size_t Genesis_hash 		= ComputeHash( static_cast<unsigned>(0), static_cast<size_t>(0), currenttime, Genesis_data );

    auto* Genesis_block = new Block<std::string, size_t>(
            static_cast<unsigned>(0), 
            static_cast<size_t>(0), 
            currenttime, 
            Genesis_data, 
            Genesis_hash, 
            static_cast<size_t>(0) );

    // Start a Chain
    BlockChain<std::string,size_t>* myblockchain = new BlockChain<std::string,size_t>( Genesis_block ) ;
    myblockchain->PrintChain(1);
    std::cout << "\n";

    // Mine a block
    Miner<std::string,size_t>* myworker = new Miner<std::string,size_t>(4);
    printf("       Previous hash                 Hash               Nounce\n");
    for (int i=0; i<10; i++) {
        myblockchain->PrintLastBlock();
        myblockchain->MiningNewBlock("New message",myworker);
    }

}



/* TODO:
 * [X] 1. Overload hash comparison operator.
 * [ ] 2. Rewrite print functionality.
 * [ ] 3. Printf should be Hashtype dependent.
 * [ ] 4. Display Mining progress.
 */
