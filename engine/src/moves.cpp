#include "../include/moves.h"
#include "../include/attacks.h"


bool Moves::makeMove(Board &board, const Move move){

    Square source = decodeMove<SOURCE_SQUARE>(move);
    Square target = decodeMove<TARGET_SQUARE>(move);
    Piece targetPiece = board.bitboards.getSquareFromBoard(target);
    Piece piece = decodeMove<PIECE>(move);
    Color colorTomove = colorOfPiece(piece);
    boardHistory history = board.generateBoardHitory(move, targetPiece);
    
    Bitboards copyBB;
    memcpy(&copyBB, &board.bitboards, sizeof(Bitboards));


    // CAPTURE
    bool capture = decodeMove<CAPTURE>(move);
    if(capture){
        board.bitboards.rmPiece(target,board.bitboards.getSquareFromBoard(target));
    }

    // ENPASSANT
    bool passant = decodeMove<ENPASSANT>(move);

    if(passant){
        board.bitboards.rmPiece(Square(target + (colorTomove == WHITE ? -8 : +8 )), (colorTomove == WHITE ? p : P ));
    }


    board.bitboards.movePiece(source, target, piece);

    // MOVE ROOKS IN CASTLING
    bool castling = decodeMove<CASTLING>(move);
    if(castling){
        switch (target)
        {
        case G1:
            board.bitboards.movePiece(H1, F1, R);
            break;
        case G8:
            board.bitboards.movePiece(H8, F8, r);
            break;
        case C1:
            board.bitboards.movePiece(A1, D1, R);
            break;
        case C8:
            board.bitboards.movePiece(A8, D8, r);
            break;
        default:
            std::cerr << "Invalid Castling Square" << std::endl;
            abort();
            break;
        }
    }


    // TRASNFORM PIECE 
    Piece promotion_piece = decodeMove<PROMOTED_PIECE>(move);
    
    if(promotion_piece != NO_PIECE){
        board.bitboards.rmPiece(target,(colorTomove == WHITE ? P : p ));
        board.bitboards.setPiece(target,promotion_piece);
    }

    
    assert((    board.bitboards.getTypeBitBoard(ALL_PIECES) 
                    ^ board.bitboards.getTypeBitBoard(KING) 
                    ^ board.bitboards.getTypeBitBoard(QUEEN)
                    ^ board.bitboards.getTypeBitBoard(KNIGHT)
                    ^ board.bitboards.getTypeBitBoard(ROOK)
                    ^ board.bitboards.getTypeBitBoard(BISHOP)
                    ^ board.bitboards.getTypeBitBoard(PAWN)) == 0);

    assert((board.bitboards.getTypeBitBoard(ALL_PIECES) 
                ^ board.bitboards.getColorBitboard(BLACK) 
                ^ board.bitboards.getColorBitboard(WHITE)) == 0);
    


    if(!board.bitboards.isKingAttacked(colorTomove)){

        // UPDATE CASTLING RIGHTS
        board.updateCastlingRights(source);
        board.updateCastlingRights(target);


        // SET ENPASSANT SQUARE
        bool doublePush = decodeMove<DOUBLE_PUSH>(move);

        if(doublePush){
            board.passantSquare = Square(source + (colorTomove == WHITE ? +8 : -8 ));
        }else{
            board.passantSquare = (SQUARE_NONE);
        }

        // UPDATE COLOR TO MOVE AND COUNTERS
        board.colorToMove = (!board.colorToMove);
        board.halfMoveCounter+=1;
        if(colorTomove == BLACK){
            board.fullMoveClock=+1;
        }
        board.pushHistory(history);
        return true;



    }else{

        memcpy(&board.bitboards,&copyBB, sizeof(Bitboards));
        return false;
    }
}

void Moves::unmakeMove(Board &board){
    boardHistory history = board.popHistory();
    Move move = history.move;
    Square source = decodeMove<SOURCE_SQUARE>(move);
    Square target = decodeMove<TARGET_SQUARE>(move);
    Piece piece = decodeMove<PIECE>(move);
    Color colorTomove = colorOfPiece(piece);

    
    // UPDATE CASTLING RIGHTS
    board.castlingRights = (history.castlingRights);

    // SET ENPASSANT SQUARe
    board.passantSquare = (history.passantSquare);


    // UPDATE COLOR TO MOVE AND COUNTERS
    board.colorToMove = (!board.colorToMove);
    board.halfMoveCounter-=1;
    if(colorTomove == BLACK){
        board.fullMoveClock-=1;
    }

    // TRASNFORM PIECE 
    Piece promotion_piece = decodeMove<PROMOTED_PIECE>(move);
    
    if(promotion_piece != NO_PIECE){
        board.bitboards.rmPiece(target,promotion_piece);
        board.bitboards.setPiece(target,(colorTomove == WHITE ? P : p ));
    }

    // MOVE ROOKS IN CASTLING
    bool castling = decodeMove<CASTLING>(move);
    if(castling){
        switch (target)
        {
        case G1:
            board.bitboards.movePiece(F1, H1, R);
            break;
        case G8:
            board.bitboards.movePiece(F8, H8, r);
            break;
        case C1:
            board.bitboards.movePiece(D1, A1, R);
            break;
        case C8:
            board.bitboards.movePiece(D8, A8, r);
            break;
        default:
            std::cerr << "Invalid Castling Square" << std::endl;
            abort();
            break;
        }
    }

    board.bitboards.movePiece(target, source, piece);

    
    // ENPASSANT
    bool passant = decodeMove<ENPASSANT>(move);

    if(passant){
        board.bitboards.setPiece(Square(target + (colorTomove == WHITE ? -8 : +8 )), (colorTomove == WHITE ? p : P ));
    }

    // CAPTURE
    bool capture = decodeMove<CAPTURE>(move);
    if(capture){
        board.bitboards.setPiece(target,history.capturedPiece);
    }

    
    assert((    board.bitboards.getTypeBitBoard(ALL_PIECES) 
                    ^ board.bitboards.getTypeBitBoard(KING) 
                    ^ board.bitboards.getTypeBitBoard(QUEEN)
                    ^ board.bitboards.getTypeBitBoard(KNIGHT)
                    ^ board.bitboards.getTypeBitBoard(ROOK)
                    ^ board.bitboards.getTypeBitBoard(BISHOP)
                    ^ board.bitboards.getTypeBitBoard(PAWN)) == 0);

    assert((board.bitboards.getTypeBitBoard(ALL_PIECES) 
                ^ board.bitboards.getColorBitboard(BLACK) 
                ^ board.bitboards.getColorBitboard(WHITE)) == 0);


}


////////////////////////////////////////////////////////////////////////////////////////////////
