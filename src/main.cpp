#include "../include/uci.h"
#include "../include/attacks.h"

uint64_t debugCounter = 0;


int main(int argc, char* argv[]) {


    using namespace std;

    Attacks::GetInstance();
    
    Uci::loop(argc, argv);


} 


