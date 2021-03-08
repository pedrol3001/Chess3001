#include"../include/board.h"

#ifdef _WIN64
    #include <io.h>
    #include <fcntl.h>
#endif

Board::Board() {

    this->clearBoard();

}

Board::Board(const std::string &fen){

    this->clearBoard();

    this->setFromFEN(fen);

}

void Board::clearBoard(){
    this->bitboards.clearBoard();
    this->history.clear();
    this->castlingRights    = NO_CASTLING;
    this->colorToMove       = WHITE;
    this->fullMoveClock     = 0;
    this->halfMoveCounter   = 0;
    this->passantSquare     = SQUARE_ZERO;
}

void Board::updateCastlingRights(const Square sq) {
    castlingRights &= castlingRightsConstants[sq];
}

boardHistory Board::generateBoardHitory(const Move m, const Piece capturedPiece){
    return boardHistory {
        m,
        capturedPiece,
        this->castlingRights,
        this->passantSquare
    };
}

boardHistory Board::popHistory(){
    boardHistory aux = history.back(); 
    history.pop_back();
    return aux;
}

void Board::pushHistory(const boardHistory &h){
    history.push_back(h);
}

Piece Board::getCurrentTurnPiece(PieceType pt)const {
    return pieceOfTypeColor(pt, colorToMove);
}

void Board::printBoard(){
    using namespace std;


    cout << "-------------------------- Game Board --------------------------" << endl;

    #ifdef _WIN64
    _setmode(_fileno(stdout), _O_WTEXT);
	#endif


    for(Rank r = RANK_8; r >= RANK_1; r--){

        
        for(File f = FILE_A; f < FILE_INIT; f++){

            // get square
            Square sq = Indx(f,r);
                
            // print ranks
            if(!f){
                wcout << r + 1;
            }

            Piece piece = this->bitboards.getSquareFromBoard(sq);
            //print piece
            wchar_t aux = '.';
            
            
            wcout << ' ' << (piece == NO_PIECE ? aux : unicodeChessPiece[piece]);
        }

        wcout << endl;
        
    }

    // print file
    wcout << "  a b c d e f g h" << endl << endl;

    
    #ifdef _WIN64
    _setmode(_fileno(stdout), _O_TEXT);
    #endif

    // print color turn
    cout << "Color to move: " << ((colorToMove == WHITE) ? 'w' : 'b') << endl;

    cout << "Castling rights: "    << (castlingRights & WHITE_OO ? "K" : "-") 
                                    << (castlingRights & WHITE_OOO ? "Q" : "-")
                                    << (castlingRights & BLACK_OO ? "k" : "-")
                                    << (castlingRights & BLACK_OOO ? "q" : "-") << endl;
    // print passant square
    cout << "Passant square " << (passantSquare == SQUARE_NONE ? "-" : squareToString(passantSquare)) << endl;
    //print halfmove counter
    cout << "Halfmove counter: " << halfMoveCounter << endl;
    //print fullmove clock
    cout << "Fullmove clock: " << fullMoveClock << endl;

}

bool Board::setFromFEN(const std::string &FEN_string){

    try{

        int phase_FEN = 0;
        Square square_counter = A8;
        
        for(char c : FEN_string){
        
            if(!isspace(c)){
                switch (phase_FEN){
                    case 0:
                        // Square numbers

                        if(c == '/'){
                            square_counter += SOUTH*2;

                        }else if(isdigit(c)){
                            square_counter += EAST*std::stoi(std::string(1,c));
                        
                        }else{
                            bitboards.setPiece(square_counter,pieceOfTypeColor(charToPieceType(c),charToPieceColor(c)));
                            square_counter+=EAST;
                        }
                        break;
                    case 1:
                        if(c=='w'){
                            colorToMove = WHITE;
                        }else if(c == 'b'){
                            colorToMove = BLACK;
                        }else{
                            std::cerr << "Invalid fen" << std::endl;
                            return false;
                        }
                        break;
                    case 2:
                        switch (tolower(c)){
                            case 'k':
                                if(charToPieceColor(c)==WHITE){
                                    castlingRights |= WHITE_OO;
                                }else if(charToPieceColor(c)==BLACK){
                                    castlingRights |= BLACK_OO;
                                }else{
                                    std::cerr << "Invalid fen" << std::endl;
                                    return false;
                                }
                                break;
                            case 'q':
                                if(charToPieceColor(c)==WHITE){
                                    castlingRights |= WHITE_OOO;
                                }else if(charToPieceColor(c)==BLACK){
                                    castlingRights |= BLACK_OOO;
                                }else{
                                    std::cerr << "Invalid fen" << std::endl;
                                    return false;
                                }
                                break;
                            case '-':
                                break;
                            default:
                                std::cerr << "Invalid fen" << std::endl;
                                return false;
                            }
                        break;
                    case 3:
                        if(c >= 'a' && c <= 'h'){
                            this->passantSquare += EAST*(c - 'a');
                        }else if( c >= '0' && c <= '8'){
                            this->passantSquare += NORTH*(c - '1');
                        }else if(c == '-'){
                            this->passantSquare = SQUARE_NONE;
                        }else{
                            std::cerr << "Invalid fen" << std::endl;
                            return false;
                        }
                        break;            
                    case 4:
                        halfMoveCounter = std::stoi(std::string(1,c));
                        break;  
                    case 5:
                        fullMoveClock = std::stoi(std::string(1,c));
                        return true;      
                    default:
                        std::cerr << "Invalid fen" << std::endl;
                        return false;

                }


            }else{
                phase_FEN++;
            }
            
        }

        return true;

    }catch(...){
        std::cerr << "Invalid fen" << std::endl;
        return false;
    }
}


