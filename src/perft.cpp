#include "../include/perft.h"
#include "../include/moves.h"
#include "../include/moveGen.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

// Coint node into a given depth
uint64_t perftDriver(Board &board, int depth){

    assert(board.bitboards.getTypeByColorBB(KING,WHITE) != 0ULL);
    assert(board.bitboards.getTypeByColorBB(KING,BLACK) != 0ULL);
  
    uint64_t nodes=0;

    if (depth == 0){
        return 1ULL;
    }

    std::deque<Move> moveList;

    MoveGen::generatePseudoMoves<ALL_MOVES>(board, moveList);
    
    for (int i = 0; i < moveList.size(); i++) {

        if (Moves::makeMove(board, moveList[i])){

            nodes+=perftDriver(board, depth - 1);
            
            Moves::unmakeMove(board);
        }
    }

    return nodes;
}

uint64_t perftTest(int depth, std::string fen_string, bool divide){

    cout << "\n\n-------------------------- Perft Test --------------------------" << endl;
    cout << "Fen: " << fen_string << endl;
    cout << "Depth: " << depth << endl << endl;
    Board board(fen_string);

    board.printBoard();

    cout << "\n-------------------------- Results --------------------------" << endl;

    uint64_t total_nodes = 0;
    std::deque<Move> moveList;

    auto preft_init = high_resolution_clock::now(); 

    MoveGen::generatePseudoMoves<ALL_MOVES>(board, moveList);

    if (divide){

        for (int i = 0; i < moveList.size(); i++) {

            if (Moves::makeMove(board, moveList[i])){

                uint64_t divided_node = perftDriver(board, depth-1);
                total_nodes += divided_node;

                Moves::unmakeMove(board);

                cout    << "Move: "         << squareToString(decodeMove<SOURCE_SQUARE>(moveList[i]))
                                            << squareToString(decodeMove<TARGET_SQUARE>(moveList[i]))
                        << " --- Nodes: "   << divided_node << endl;


            }
        }
        
    }else{
        total_nodes = perftDriver(board, depth);
    }

    auto perft_duration = duration_cast<chrono::milliseconds>(high_resolution_clock::now()  - preft_init); 
    cout << "Duration: " << perft_duration.count() << endl;
    cout << "Total Nodes: " << total_nodes << endl;

    return total_nodes;
    
}

void perftTestDebug(){

    vector<string> result;

    int i=0;
    while(perftests[i].fen != NULL){
        if(perftTest(perftests[i].depth,perftests[i].fen) == perftests[i].nodes){
            result.push_back(string(perftests[i].fen) + " --- OK");
        }else{
            result.push_back(string(perftests[i].fen) + " --- ERROR");
        }
        i++;
    }

    cout << endl << "------------------------------------------------------------" << endl <<"Full Report: " << endl;
    for(string res : result){
        cout << res << endl;
    }
}