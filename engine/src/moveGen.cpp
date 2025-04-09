
#include "../include/moveGen.h"
#include "../include/moves.h"


template <MoveType MT>
void MoveGen::generatePseudoMoves(Board& board, std::deque<Move>& moveList) {
    moveList.clear();

    generatePseudoPawnMoves<MT>(board, moveList);
    generatePseudoKnightMoves<MT>(board, moveList);
    generatePseudoBishopMoves<MT>(board, moveList);
    generatePseudoRookMoves<MT>(board, moveList);
    generatePseudoQueenMoves<MT>(board, moveList);
    if constexpr (MT == ALL_MOVES || MT == QUIET_MOVES || MT == CASTLING_MOVES) {
        generatePseudoCastlingMoves(board, moveList);
    }
    generatePseudoKingMoves<MT>(board, moveList);

}

template void MoveGen::generatePseudoMoves<ALL_MOVES>(Board& board, std::deque<Move>& moveList);
template void MoveGen::generatePseudoMoves<QUIET_MOVES>(Board& board, std::deque<Move>& moveList);
template void MoveGen::generatePseudoMoves<CAPTURE_MOVES>(Board& board, std::deque<Move>& moveList);
template void MoveGen::generatePseudoMoves<CASTLING_MOVES>(Board& board, std::deque<Move>& moveList);

template <MoveType MT>
void MoveGen::generateLegalMoves(Board& board, std::deque<Move>& moveList) {

    moveList.clear();

    std::deque<Move> pseudoLegalMoves;

    MoveGen::generatePseudoMoves<MT>(board, pseudoLegalMoves);

    // copy only positive numbers:
    for (Move m : pseudoLegalMoves) {
        if (Moves::makeMove(board, m)) {
            Moves::unmakeMove(board);
            moveList.push_back(m);
        }
    }
}
template void MoveGen::generateLegalMoves<ALL_MOVES>(Board& board, std::deque<Move>& moveList);
template void MoveGen::generateLegalMoves<QUIET_MOVES>(Board& board, std::deque<Move>& moveList);
template void MoveGen::generateLegalMoves<CAPTURE_MOVES>(Board& board, std::deque<Move>& moveList);
template void MoveGen::generateLegalMoves<CASTLING_MOVES>(Board& board, std::deque<Move>& moveList);


void MoveGen::generatePseudoCastlingMoves(Board& board, std::deque<Move>& moveList) {

    // CASTLING MOVES
    Piece piece = board.getCurrentTurnPiece(KING);
    Piece rook_piece = board.getCurrentTurnPiece(ROOK);

    const Square OO_RookSquares = (board.colorToMove == WHITE ? H1 : H8);
    const Square OOO_RookSquares = (board.colorToMove == WHITE ? A1 : A8);

    const Square OO_KingPassSquares[3] = { (board.colorToMove == WHITE ? E1 : E8) ,(board.colorToMove == WHITE ? F1 : F8),(board.colorToMove == WHITE ? G1 : G8) };
    const Square OOO_KingPassSquares[3] = { (board.colorToMove == WHITE ? E1 : E8) ,(board.colorToMove == WHITE ? D1 : D8),(board.colorToMove == WHITE ? C1 : C8) };

    const Square OO_CastBlockSquares[2] = { (board.colorToMove == WHITE ? F1 : F8),(board.colorToMove == WHITE ? G1 : G8) };
    const Square OOO_CastBlockSquares[3] = { (board.colorToMove == WHITE ? B1 : B8),(board.colorToMove == WHITE ? C1 : C8),(board.colorToMove == WHITE ? D1 : D8) };

    Square fromSquare = (board.colorToMove == WHITE ? E1 : E8);

    if (board.bitboards.getSquareFromBoard(fromSquare) == piece) {

        if (board.castlingRights & (board.colorToMove == WHITE ? WHITE_OO : BLACK_OO)) {
            if (!getBit(OO_CastBlockSquares[0], board.bitboards.getTypeBitBoard(ALL_PIECES))
                && !getBit(OO_CastBlockSquares[1], board.bitboards.getTypeBitBoard(ALL_PIECES))
                && board.bitboards.getSquareFromBoard(OO_RookSquares) == rook_piece) {

                if (!board.bitboards.isSquareAttacked(OO_KingPassSquares[0], !board.colorToMove)
                    && !board.bitboards.isSquareAttacked(OO_KingPassSquares[1], !board.colorToMove)
                    && !board.bitboards.isSquareAttacked(OO_KingPassSquares[2], !board.colorToMove)) {

                    Square toSquare = OO_CastBlockSquares[1];

                    //std::cout << "Castling " << (board.colorToMove == WHITE ? "White ": "Black ") << "OO"<<std::endl;
                    moveList.push_back(std::move(encodeMove(fromSquare, toSquare, piece, NO_PIECE, false, false, false, true)));
                }
            }
        }

        if (board.castlingRights & (board.colorToMove == WHITE ? WHITE_OOO : BLACK_OOO)) {
            if (!getBit(OOO_CastBlockSquares[0], board.bitboards.getTypeBitBoard(ALL_PIECES))
                && !getBit(OOO_CastBlockSquares[1], board.bitboards.getTypeBitBoard(ALL_PIECES))
                && !getBit(OOO_CastBlockSquares[2], board.bitboards.getTypeBitBoard(ALL_PIECES))
                && board.bitboards.getSquareFromBoard(OOO_RookSquares) == rook_piece) {

                if (!board.bitboards.isSquareAttacked(OOO_KingPassSquares[0], !board.colorToMove)
                    && !board.bitboards.isSquareAttacked(OOO_KingPassSquares[1], !board.colorToMove)
                    && !board.bitboards.isSquareAttacked(OOO_KingPassSquares[2], !board.colorToMove)) {

                    Square toSquare = OOO_CastBlockSquares[1];
                    //std::cout << "Castling " << (board.colorToMove == WHITE ? "White ": "Black ") << "OOO"<<std::endl;
                    moveList.push_back(std::move(encodeMove(fromSquare, toSquare, piece, NO_PIECE, false, false, false, true)));

                }
            }
        }
    }
}

template <MoveType MT>
void MoveGen::generatePseudoPawnMoves(Board& board, std::deque<Move>& moveList) {

    Attacks* attacks = Attacks::GetInstance();

    // Copy bitboard of piece
    Bitboard cpBB = board.bitboards.getTypeByColorBB(PAWN, board.colorToMove);


    const Color curr_color = board.colorToMove; // current color to move
    const Piece curr_piece = board.getCurrentTurnPiece(PAWN); // current piece to move
    const Square kingSquare = board.bitboards.getKingSquare(curr_color); // current king square 

    // Pawns variables of WHITE and BLACK

    const Square initCeilFileSquare = curr_color == WHITE ? H2 : H7;
    const Square initFloorFileSquare = curr_color == WHITE ? A2 : A7;
    const int offset = curr_color == WHITE ? 8 : -8;

    while (cpBB) {

        // Get piece square
        Square fromSquare = getLs1b(cpBB);

        Square toSquare = Square(fromSquare + offset);

        // Pawn attacks
        Bitboard atks = (attacks->getPawnAttack(fromSquare, curr_color) & board.bitboards.getColorBitboard(!curr_color));


        bool validMove = curr_color == WHITE ? (toSquare <= H8) : (toSquare >= A1);
        bool lastFileFloorComparation = curr_color == WHITE ? (toSquare >= A8) : (toSquare <= H1);


        if (!board.bitboards.isKingAttacked(curr_color)) {
            board.bitboards.rmPiece(fromSquare, curr_piece);
            if (board.bitboards.isKingAttacked(curr_color)) {

                if (squaresOnSameFile(fromSquare, kingSquare)) {
                    atks = 0ULL;
                }
                else if (squaresOnSameRank(fromSquare, kingSquare)) {
                    atks = 0ULL;
                    validMove = false;
                }
                else if (squaresOnSameDiagonal(fromSquare, kingSquare)) {
                    atks &= diagonalMask(kingSquare);
                    validMove = false;
                }
                else if (squaresOnSameAntiDiagonal(fromSquare, kingSquare)) {
                    atks &= antiDiagMask(kingSquare);
                    validMove = false;
                }
                else {
                    std::cerr << "Pawn can't be pinned by a non rank-file or non diagonal sliding piece" << std::endl;
                    abort();
                }
            }
            board.bitboards.setPiece(fromSquare, curr_piece);
        }

        //Pawn moves
        if constexpr (MT == QUIET_MOVES || MT == ALL_MOVES) {

            if (validMove && !getBit(toSquare, board.bitboards.getTypeBitBoard(ALL_PIECES))) {

                // Promotion
                if (lastFileFloorComparation && validMove) {
                    // cout << "Promotion Queen: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    moveList.push_back(std::move(encodeMove(fromSquare, toSquare, curr_piece, board.getCurrentTurnPiece(QUEEN), false, false, false, false)));
                    // cout << "Promotion Knight: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    moveList.push_back(std::move(encodeMove(fromSquare, toSquare, curr_piece, board.getCurrentTurnPiece(KNIGHT), false, false, false, false)));
                    // cout << "Promotion Rook: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    moveList.push_back(std::move(encodeMove(fromSquare, toSquare, curr_piece, board.getCurrentTurnPiece(ROOK), false, false, false, false)));
                    // cout << "Promotion Bishop: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    moveList.push_back(std::move(encodeMove(fromSquare, toSquare, curr_piece, board.getCurrentTurnPiece(BISHOP), false, false, false, false)));
                }
                //Normal move
                else {
                    // 1 pawn push
                    // cout << "Panw Push: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    moveList.push_back(std::move(encodeMove(fromSquare, toSquare, curr_piece, NO_PIECE, false, false, false, false)));
                    // 2 pawn push

                    if (fromSquare >= initFloorFileSquare && fromSquare <= initCeilFileSquare && !getBit(Square(toSquare + offset), board.bitboards.getTypeBitBoard(ALL_PIECES))) {
                        // cout << "Panw Double Push: " << squareToString(fromSquare) << squareToString(Square(toSquare + offset)) << endl;
                        moveList.push_back(std::move(encodeMove(fromSquare, Square(toSquare + offset), curr_piece, NO_PIECE, false, true, false, false)));
                    }
                }
            }
        }

        // Captures
        if constexpr (MT == CAPTURE_MOVES || MT == ALL_MOVES) {
            while (atks) {
                toSquare = getLs1b(atks);

                // Promotion
                if (lastFileFloorComparation && validMove) {
                    // cout << "Promotion Capture Queen: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    moveList.push_front(std::move(encodeMove(fromSquare, toSquare, curr_piece, board.getCurrentTurnPiece(QUEEN), true, false, false, false)));
                    // cout << "Promotion Capture Knight: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    moveList.push_front(std::move(encodeMove(fromSquare, toSquare, curr_piece, board.getCurrentTurnPiece(KNIGHT), true, false, false, false)));
                    // cout << "Promotion Capture Rook: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    moveList.push_front(std::move(encodeMove(fromSquare, toSquare, curr_piece, board.getCurrentTurnPiece(ROOK), true, false, false, false)));
                    // cout << "Promotion Capture Bishop: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    moveList.push_front(std::move(encodeMove(fromSquare, toSquare, curr_piece, board.getCurrentTurnPiece(BISHOP), true, false, false, false)));
                }
                else {
                    // Pawn Captuew
                    // cout << "Panw Capture: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    moveList.push_front(std::move(encodeMove(fromSquare, toSquare, curr_piece, NO_PIECE, true, false, false, false)));
                }

                popLs1b(atks);
            }

            //Passant capture
            if (board.passantSquare != SQUARE_NONE) {
                Bitboard passant_attacks = (attacks->getPawnAttack(fromSquare, curr_color) & (0x1ULL << board.passantSquare));
                if (passant_attacks) {
                    toSquare = getLs1b(passant_attacks);
                    // cout << "Panw Passant Capture: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    moveList.push_front(std::move(encodeMove(fromSquare, toSquare, curr_piece, NO_PIECE, false, false, true, false)));
                }
            }
        }

        // Remove bit from copy bitboard
        popLs1b(cpBB);

    }
}

template <MoveType MT>
void MoveGen::generatePseudoKnightMoves(Board& board, std::deque<Move>& moveList) {

    Attacks* attacks = Attacks::GetInstance();

    Bitboard cpBB = board.bitboards.getTypeByColorBB(KNIGHT, board.colorToMove);

    const Color curr_color = board.colorToMove; // current color to move
    const Piece curr_piece = board.getCurrentTurnPiece(KNIGHT); // current piece to move
    //const Square kingSquare = board.bitboards.getKingSquare(curr_color); // current king square 

    while (cpBB) {

        Square fromSquare = getLs1b(cpBB);
        Bitboard atks = (attacks->getNonSliddingAttack(fromSquare, KNIGHT) & ~(board.bitboards.getColorBitboard(curr_color)));

        if (!board.bitboards.isKingAttacked(curr_color)) {
            board.bitboards.rmPiece(fromSquare, curr_piece);
            if (board.bitboards.isKingAttacked(curr_color)) {
                atks = 0ULL;
            }
            board.bitboards.setPiece(fromSquare, curr_piece);
        }

        while (atks) {
            Square toSquare = Square(getLs1b(atks));

            if (!getBit(toSquare, board.bitboards.getTypeByColorBB(ALL_PIECES, !curr_color))) {
                if constexpr (MT == QUIET_MOVES || MT == ALL_MOVES) {
                    // cout << "Knight move: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    const Move move = encodeMove(fromSquare, toSquare, curr_piece, NO_PIECE, false, false, false, false);
                    moveList.push_back(std::move(move));
                }
            }
            else {
                if constexpr (MT == CAPTURE_MOVES || MT == ALL_MOVES) {
                    // cout << "Knight capture: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    const Move move = encodeMove(fromSquare, toSquare, curr_piece, NO_PIECE, true, false, false, false);
                    moveList.push_front(std::move(move));
                }
            }

            popLs1b(atks);
        }

        popLs1b(cpBB);
    }

}

template <MoveType MT>
void MoveGen::generatePseudoKingMoves(Board& board, std::deque<Move>& moveList) {
    Attacks* attacks = Attacks::GetInstance();

    Bitboard cpBB = board.bitboards.getTypeByColorBB(KING, board.colorToMove);

    while (cpBB) {

        Square fromSquare = getLs1b(cpBB);
        Bitboard atks = (attacks->getNonSliddingAttack(fromSquare, KING) & ~(board.bitboards.getColorBitboard(board.colorToMove)));

        while (atks) {
            Square toSquare = Square(getLs1b(atks));

            board.bitboards.rmPiece(fromSquare, board.getCurrentTurnPiece(KING));
            if (!board.bitboards.isSquareAttacked(toSquare, !board.colorToMove)) {

                if (!getBit(toSquare, board.bitboards.getTypeByColorBB(ALL_PIECES, !board.colorToMove))) {
                    if constexpr (MT == QUIET_MOVES || MT == ALL_MOVES) {
                        //  cout << "King move: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                        const Move move = encodeMove(fromSquare, toSquare, board.getCurrentTurnPiece(KING), NO_PIECE, false, false, false, false);
                        moveList.push_back(std::move(move));
                    }
                }
                else {
                    if constexpr (MT == CAPTURE_MOVES || MT == ALL_MOVES) {
                        // cout << "King capture: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                        const Move move = encodeMove(fromSquare, toSquare, board.getCurrentTurnPiece(KING), NO_PIECE, true, false, false, false);
                        moveList.push_front(std::move(move));
                    }
                }
            }
            board.bitboards.setPiece(fromSquare, board.getCurrentTurnPiece(KING));
            popLs1b(atks);
        }

        popLs1b(cpBB);
    }

}

template <MoveType MT>
void MoveGen::generatePseudoBishopMoves(Board& board, std::deque<Move>& moveList) {
    Attacks* attacks = Attacks::GetInstance();

    Bitboard cpBB = board.bitboards.getTypeByColorBB(BISHOP, board.colorToMove);

    const Color curr_color = board.colorToMove; // current color to move
    const Piece curr_piece = board.getCurrentTurnPiece(BISHOP); // current piece to move
    const Square kingSquare = board.bitboards.getKingSquare(curr_color); // current king square 

    while (cpBB) {

        Square fromSquare = getLs1b(cpBB);
        Bitboard atks = (attacks->getSliddingAttack(fromSquare, BISHOP, board.bitboards.getTypeBitBoard(ALL_PIECES)) & ~(board.bitboards.getColorBitboard(curr_color)));

        if (!board.bitboards.isKingAttacked(curr_color)) {
            board.bitboards.rmPiece(fromSquare, curr_piece);
            if (board.bitboards.isKingAttacked(curr_color)) {

                if (squaresOnSameDiagonal(fromSquare, kingSquare)) {
                    atks &= diagonalMask(kingSquare);
                }
                else if (squaresOnSameAntiDiagonal(fromSquare, kingSquare)) {
                    atks &= antiDiagMask(kingSquare);
                }
                else {
                    atks = 0ULL;
                }
            }
            board.bitboards.setPiece(fromSquare, curr_piece);
        }

        while (atks) {
            Square toSquare = Square(getLs1b(atks));

            if (!getBit(toSquare, board.bitboards.getTypeByColorBB(ALL_PIECES, !curr_color))) {
                if constexpr (MT == QUIET_MOVES || MT == ALL_MOVES) {
                    // cout << "Bishop move: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    const Move move = encodeMove(fromSquare, toSquare, curr_piece, NO_PIECE, false, false, false, false);
                    moveList.push_back(std::move(move));
                }
            }
            else {
                if constexpr (MT == CAPTURE_MOVES || MT == ALL_MOVES) {
                    // cout << "Bishop capture: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    const Move move = encodeMove(fromSquare, toSquare, curr_piece, NO_PIECE, true, false, false, false);
                    moveList.push_front(std::move(move));
                }
            }
            popLs1b(atks);
        }

        popLs1b(cpBB);
    }

}

template <MoveType MT>
void MoveGen::generatePseudoRookMoves(Board& board, std::deque<Move>& moveList) {

    Attacks* attacks = Attacks::GetInstance();

    Bitboard cpBB = board.bitboards.getTypeByColorBB(ROOK, board.colorToMove);

    const Color curr_color = board.colorToMove; // current color to move
    const Piece curr_piece = board.getCurrentTurnPiece(ROOK); // current piece to move
    const Square kingSquare = board.bitboards.getKingSquare(curr_color); // current king square 

    while (cpBB) {

        Square fromSquare = getLs1b(cpBB); // Square of piece to move

        //Bitboard of possoble attacks of the piece to move
        Bitboard atks = (attacks->getSliddingAttack(fromSquare, ROOK, board.bitboards.getTypeBitBoard(ALL_PIECES)) & ~(board.bitboards.getColorBitboard(curr_color)));

        if (!board.bitboards.isKingAttacked(curr_color)) {
            board.bitboards.rmPiece(fromSquare, curr_piece);
            if (board.bitboards.isKingAttacked(curr_color)) {

                if (squaresOnSameFile(fromSquare, kingSquare)) {
                    atks &= fileMask(file_of(kingSquare));
                }
                else if (squaresOnSameRank(fromSquare, kingSquare)) {
                    atks &= rankMask(rank_of(kingSquare));
                }
                else {
                    atks = 0ULL;
                }
            }
            board.bitboards.setPiece(fromSquare, curr_piece);
        }
        while (atks) {
            Square toSquare = Square(getLs1b(atks));

            if (!getBit(toSquare, board.bitboards.getTypeByColorBB(ALL_PIECES, !curr_color))) {
                if constexpr (MT == QUIET_MOVES || MT == ALL_MOVES) {
                    // cout << "Rook move: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    const Move move = encodeMove(fromSquare, toSquare, curr_piece, NO_PIECE, false, false, false, false);
                    moveList.push_back(std::move(move));
                }
            }
            else {
                if constexpr (MT == CAPTURE_MOVES || MT == ALL_MOVES) {
                    // cout << "Rook capture: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    const Move move = encodeMove(fromSquare, toSquare, curr_piece, NO_PIECE, true, false, false, false);
                    moveList.push_front(std::move(move));
                }
            }

            popLs1b(atks);
        }

        popLs1b(cpBB);
    }

}

template <MoveType MT>
void MoveGen::generatePseudoQueenMoves(Board& board, std::deque<Move>& moveList) {
    Attacks* attacks = Attacks::GetInstance(); // Get instance os precal attacks

    // Bitboard of queens
    Bitboard cpBB = board.bitboards.getTypeByColorBB(QUEEN, board.colorToMove);

    const Color curr_color = board.colorToMove; // current color to move
    const Piece curr_piece = board.getCurrentTurnPiece(QUEEN); // current piece to move
    const Square kingSquare = board.bitboards.getKingSquare(curr_color); // current king square 

    // Run over queens bitboards
    while (cpBB) {

        // Square of piece to move
        Square fromSquare = getLs1b(cpBB);

        // Bitboard os current piece attack
        Bitboard atks = (attacks->getSliddingAttack(fromSquare, QUEEN, board.bitboards.getTypeBitBoard(ALL_PIECES)) & ~(board.bitboards.getColorBitboard(curr_color)));

        if (!board.bitboards.isKingAttacked(curr_color)) {
            // Remove pice
            board.bitboards.rmPiece(fromSquare, curr_piece);
            // Check if still not in check
            if (board.bitboards.isKingAttacked(curr_color)) {

                // Ajust attks bitboard acoardingly to the pinned piece
                if (squaresOnSameFile(fromSquare, kingSquare)) {
                    atks &= fileMask(file_of(kingSquare));
                }
                else if (squaresOnSameRank(fromSquare, kingSquare)) {
                    atks &= rankMask(rank_of(kingSquare));
                }
                else if (squaresOnSameDiagonal(fromSquare, kingSquare)) {
                    atks &= diagonalMask(kingSquare);
                }
                else if (squaresOnSameAntiDiagonal(fromSquare, kingSquare)) {
                    atks &= antiDiagMask(kingSquare);
                }
                else {
                    std::cerr << "Queen can't be pinned by a non rank-file or non diagonal sliding piece" << std::endl;
                    abort();
                }
            }
            // Set piece again
            board.bitboards.setPiece(fromSquare, curr_piece);
        }

        // Run over all posible attacks
        while (atks) {
            Square toSquare = Square(getLs1b(atks));

            if (!getBit(toSquare, board.bitboards.getTypeByColorBB(ALL_PIECES, !curr_color))) {
                if constexpr (MT == QUIET_MOVES || MT == ALL_MOVES) {
                    // cout << "QUEEN move: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    const Move move = encodeMove(fromSquare, toSquare, curr_piece, NO_PIECE, false, false, false, false);
                    moveList.push_back(std::move(move));
                }
            }
            else {
                if constexpr (MT == CAPTURE_MOVES || MT == ALL_MOVES) {
                    // cout << "QUEEN capture: " << squareToString(fromSquare) << squareToString(toSquare) << endl;
                    const Move move = encodeMove(fromSquare, toSquare, curr_piece, NO_PIECE, true, false, false, false);
                    moveList.push_front(std::move(move));
                }
            }

            // remove actual attack
            popLs1b(atks);
        }

        // remove actual piece
        popLs1b(cpBB);
    }

}
