#include "../include/uci.h"
#include "../include/moves.h"
#include "../include/moveGen.h"
#include "../include/search.h"
#include "../include/perft.h"

#include <chrono>

using namespace std;
using namespace std::chrono;


    void Uci::position(Board &board, std::istringstream& is) {

        using namespace std;

        Move m = 0xffffUL;
        string str, fen;

        is >> str;

        if (str == "startpos")
        {
            fen = start_position;
            is >> str; // Consume "moves" str if any
        }
        else if (str == "fen")
            while (is >> str && str != "moves")
                fen += str + " ";
        else
            return;

        board.clearBoard();
        board.setFromFEN(fen);

        // Parse move list (if any)
        deque<Move> moveList;

        while (is >> str && m != NO_MOVE){ 
            MoveGen::generatePseudoMoves<ALL_MOVES>(board, moveList);
            m = Uci::parseMove(moveList, str);
            if(m != NO_MOVE)
                Moves::makeMove(board,m);
            
        }
    }

    void Uci::go(Board& board, std::istringstream& is) {

        using namespace std;

        Search *search = new Search();

        string str;

        while (is >> str) {
            if (str == "depth") {
                is >> search->searchDepth;
            }
        }

        int64_t score = 0;
        std::deque<Move> line;

        for (int i = 1; i <= search->searchDepth; i++) {

            line.clear();
            score = search->pvSearch(board, -INF, +INF, i, line);

        }

        cout << "info score cp " << score
            << " depth " << search->searchDepth
            << " nodes " << search->nodesCount;
            
        cout << " pv ";

        for (Move pvMove : line) {
            cout << Uci::writeMove(pvMove) << " ";
        }
        
        cout << endl;;

        cout << "bestmove " << Uci::writeMove(line[0]) << endl;
    }


    std::string Uci::writeMove(const Move move){
        const Square from = decodeMove<SOURCE_SQUARE>(move);
        const Square to = decodeMove<TARGET_SQUARE>(move);

        const Piece promoted = decodeMove<PROMOTED_PIECE>(move);

        if(promoted == NO_PIECE){
            return (squareToString(from) + squareToString(to));
        }else{
            return (squareToString(from) + squareToString(to) + char(tolower(pieceToChar(promoted))));
        }

    }
    
    
    Move Uci::parseMove(const std::deque<Move> &moveList, std::string &move){

        if (move.length() == 5) 
            move[4] = char(tolower(move[4]));
        for (Move m : moveList){
            if( move == Uci::writeMove(m)){
                return m;
            }       
        }
        return NO_MOVE;
    }


    void Uci::loop(int argc, char* argv[]) {


        using namespace std;

        Board *uci = new Board(start_position);
        string str, cmd;

        for (int i = 1; i < argc; ++i)
            cmd += string(argv[i]) + " ";

        do {
            if (argc == 1 && !getline(cin, cmd))
                cmd = "quit";

            istringstream is(cmd);

            str.clear(); 
            is >> skipws >> str;

            if (str == "quit"
                || str == "stop")
                continue;

            else if (str == "uci")
                cout << "id name " << "Chess3001"
                //<< "\n"       << Options
                << "\nuciok" << endl;

            //else if (str == "setoption")  setoption(is);
            else if (str == "go") {
                Uci::go(*uci, is);
            }
            else if (str == "position") {
                Uci::position(*uci, is);
            }
            else if (str == "ucinewgame") {
                uci->clearBoard();
                uci->setFromFEN(start_position);
            }
            else if (str == "isready") {
                cout << "readyok" << endl;
            }

            else if (str == "board") uci->printBoard();
            else if (str == "perft") {     
                
                string depth_str;
                is >> depth_str;
                if(depth_str.length() == 0){
                    cerr << "Depth less or equal 0" << endl;
                }
                else{
                    int depth = stoi(depth_str);
                    string fen = trim_copy(is.str().substr(is.str().find_first_of(" \t") + 2));
                    perftTest(depth, fen, true);
                }
               
            }else if (str == "debug") perftTestDebug();
            else
               cout << "Invalid command " << cmd << endl;

        } while (str != "quit" && argc == 1); // Command line args are one-shot

        delete uci;
    }
