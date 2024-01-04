bool is_diagonal(int, int);

void move_piece(wchar_t **board, int *move) {
    // Move piece in board from origin to dest
    board[move[2]][move[3]] = board[*move][move[1]];
    board[*move][move[1]] = 0;
}

void freeAll(int *piece_team, int *x_moves, int *y_moves) {
    free(piece_team);
    free(x_moves);
    free(y_moves);
}

bool emit(int client, char *message, int message_size) {
    return true;
}

void translate_to_move(int *move, char *buffer) {
    printf("buffer: %s\n", buffer);
    *(move) = 8 - (*(buffer + 1) - '0');
    move[1] = (*(buffer) - '0') - 49;
    move[2] = 8 - (*(buffer + 4) - '0');
    move[3] = (*(buffer + 3) - '0') - 49;
}

void broadcast(wchar_t **board, char *one_dimension_board, int player_one, int player_two) {
    to_one_dimension_char(board, one_dimension_board);
    printf("\tSending board to %d and %d size(%lu)\n", player_one, player_two, sizeof(one_dimension_board));
    send(player_one, one_dimension_board, 64, 0);
    send(player_two, one_dimension_board, 64, 0);
    printf("\tSent board...\n");
}

int getManitud(int origin, int dest) {
    return (abs(origin - dest));
}



bool is_diagonal_clear(wchar_t **board, int *move) {
    int *x_moves = (int *)malloc(sizeof(int));
    int *y_moves = (int *)malloc(sizeof(int));

    *(x_moves) = *(move)-move[2];
    *(y_moves) = move[1] - move[3];

    int *index = (int *)malloc(sizeof(int));
    *(index) = abs(*x_moves) - 1;
    wchar_t *item_at_position = (wchar_t *)malloc(sizeof(wchar_t));

    // Iterate thru all items excepting initial posi
    while (*index > 0) {
        if (*x_moves > 0 && *y_moves > 0) {
            printf("%lc [%d,%d]\n", board[*move - *index][move[1] - *index], *move - *index, move[1] - *index);
            *item_at_position = board[*move - *index][move[1] - *index];
        }
        if (*x_moves > 0 && *y_moves < 0) {
            printf("%lc [%d,%d]\n", board[*move - *index][move[1] + *index], *move - *index, move[1] + *index);
            *item_at_position = board[*move - *index][move[1] + *index];
        }
        if (*x_moves < 0 && *y_moves < 0) {
            printf("%lc [%d,%d]\n", board[*move + *index][move[1] + *index], *move + *index, move[1] + *index);
            *item_at_position = board[*move + *index][move[1] + *index];
        }
        if (*x_moves < 0 && *y_moves > 0) {
            printf("%lc [%d,%d]\n", board[*move + *index][move[1] - *index], *move + *index, move[1] - *index);
            *item_at_position = board[*move + *index][move[1] - *index];
        }

        if (*item_at_position != 0) {
            free(index);
            free(x_moves);
            free(y_moves);
            free(item_at_position);
            return false;
        }

        (*index)--;
    }

    free(index);
    free(x_moves);
    free(y_moves);
    free(item_at_position);

    return true;
}

bool is_syntax_valid(int player, char *move) {
    // Look for -
    if (move[2] != '-') {
        send(player, "e-00", 4, 0);
        return false;
    }
    // First and 3th should be characters
    if (move[0] - '0' < 10) {
        send(player, "e-01", 4, 0);
        return false;
    }
    if (move[3] - '0' < 10) {
        send(player, "e-02", 4, 0);
        return false;
    }

    // Second and 5th character should be numbers
    if (move[1] - '0' > 10) {
        send(player, "e-03", 4, 0);
        return false;
    }
    if (move[1] - '0' > 8) {
        send(player, "e-04", 4, 0);
        return false;
    }
    if (move[4] - '0' > 10) {
        send(player, "e-05", 4, 0);
        return false;
    }
    if (move[4] - '0' > 8) {
        send(player, "e-06", 4, 0);
        return false;
    }
    // Move out of range
    if (move[0] - '0' > 56 || move[3] - '0' > 56) {
        send(player, "e-07", 4, 0);
        return false;
    }

    return true;
}

int get_piece_team(wchar_t **board, int x, int y) {
    switch (board[x][y]) {
        case white_king:
            return -1;
        case white_queen:
            return -1;
        case white_rook:
            return -1;
        case white_bishop:
            return -1;
        case white_knight:
            return -1;
        case white_pawn:
            return -1;
        case black_king:
            return 1;
        case black_queen:
            return 1;
        case black_rook:
            return 1;
        case black_bishop:
            return 1;
        case black_knight:
            return 1;
        case black_pawn:
            return 1;
    }

    return 0;
}

int get_piece_type(wchar_t piece) {
    switch (piece) {
        case white_king:
            return 0;
        case white_queen:
            return 1;
        case white_rook:
            return 2;
        case white_bishop:
            return 3;
        case white_knight:
            return 4;
        case white_pawn:
            return 5;
        case black_king:
            return 0;
        case black_queen:
            return 1;
        case black_rook:
            return 2;
        case black_bishop:
            return 3;
        case black_knight:
            return 4;
        case black_pawn:
            return 5;
    }
    return -1;
}
bool eat_piece(wchar_t **board, int x, int y) {
    return (get_piece_team(board, x, y) != 0);
}

bool is_rect(int *move) {
    int *x_moves = (int *)malloc(sizeof(int));
    int *y_moves = (int *)malloc(sizeof(int));

    *x_moves = *(move)-move[2];
    *y_moves = move[1] - move[3];

    if ((*x_moves != 0 && *y_moves == 0) || (*y_moves != 0 && *x_moves == 0)) {
        free(x_moves);
        free(y_moves);
        return true;
    }

    free(x_moves);
    free(y_moves);
    return false;
}

int is_rect_clear(wchar_t **board, int *move, int x_moves, int y_moves) {
    // Is a side rect
    int *index = (int *)malloc(sizeof(int));

    if (abs(x_moves) > abs(y_moves)) {
        *index = abs(x_moves) - 1;
    } else {
        *index = abs(y_moves) - 1;
    }

    // Iterate thru all items excepting initial position
    while (*index > 0) {
        if (*(move)-move[2] > 0) {
            if (board[*move - (*index)][move[1]] != 0) {
                free(index);
                return false;
            }
        }
        if (*(move)-move[2] < 0) {
            if (board[*move + (*index)][move[1]] != 0) {
                free(index);
                return false;
            }
        }
        if (move[1] - move[3] > 0) {
            if (board[*move][move[1] - (*index)] != 0) {
                free(index);
                return false;
            }
        }
        if (move[1] - move[3] < 0) {
            if (board[*move][move[1] + (*index)] != 0) {
                free(index);
                return false;
            }
        }

        (*index)--;
    }

    free(index);
    return true;
}

void promote_piece(wchar_t **board, int destX, int destY, int team) {
    if (team == 1) {
        board[destX][destY] = black_queen;
    } else if (team == -1) {
        board[destX][destY] = white_queen;
    }
}

bool is_diagonal(int x_moves, int y_moves) {
    if ((abs(x_moves) - abs(y_moves)) != 0) {
        return false;
    }

    return true;
}

bool is_move_valid(wchar_t **board, int player, int team, int *move) {
    int *piece_team = (int *)malloc(sizeof(int *));
    int *x_moves = (int *)malloc(sizeof(int *));
    int *y_moves = (int *)malloc(sizeof(int *));

    *piece_team = get_piece_team(board, *(move), move[1]);
    *x_moves = getManitud(*move, move[2]);
    *y_moves = getManitud(move[1], move[3]);

    // General errors
    if (board[*(move)][move[1]] == 0) {
        send(player, "e-08", 4, 0);
        return false;
    }  // If selected piece == 0 there's nothing selected
    if (*piece_team == get_piece_team(board, move[2], move[3])) {
        send(player, "e-09", 4, 0);
        return false;
    }  // If the origin piece's team == dest piece's team is an invalid move
    // Check if user is moving his piece
    if (team != *piece_team) {
        send(player, "e-07", 4, 0);
        return false;
    }

    printf("Player %d(%d) [%d,%d] to [%d,%d]\n", player, *piece_team, move[0], move[1], move[2], move[3]);

    // XMOVES = getManitud(*move, move[2])
    // YMOVES = getManitud(move[1], move[3])
    printf("Moved piece -> %d \n", get_piece_type(board[*(move)][move[1]]));
    switch (get_piece_type(board[*(move)][move[1]])) {
        case 0: /* --- ♚ --- */
            if (*x_moves > 1 || *y_moves > 1) {
                send(player, "e-10", 5, 0);
                freeAll(piece_team, x_moves, y_moves);
                return false;
            }
            break;
        case 1:
            if (!is_rect(move) && !is_diagonal(*x_moves, *y_moves)) {
                send(player, "e-queen", 4, 0);
                freeAll(piece_team, x_moves, y_moves);
                return false;
            }
            if (!is_rect_clear(board, move, *x_moves, *y_moves)) {
                send(player, "e-31", 4, 0);
                freeAll(piece_team, x_moves, y_moves);
                return false;
            }
            if (!is_diagonal_clear(board, move)) {
                send(player, "e-41", 4, 0);
                freeAll(piece_team, x_moves, y_moves);
                return false;
            }
            if (eat_piece(board, move[2], move[3])) {
                send(player, "i-99", 4, 0);
                freeAll(piece_team, x_moves, y_moves);
                return true;
            }
            break;
        case 2: /* --- ♜ --- */
            if (!is_rect(move)) {
                send(player, "e-30", 5, 0);
                freeAll(piece_team, x_moves, y_moves);
                return false;
            }
            if (!is_rect_clear(board, move, *x_moves, *y_moves)) {
                send(player, "e-31", 4, 0);
                freeAll(piece_team, x_moves, y_moves);
                return false;
            }
            if (eat_piece(board, move[2], move[3])) {
                send(player, "i-99", 4, 0);
                freeAll(piece_team, x_moves, y_moves);
                return true;
            }
            break;
        case 3: /* ––– ♝ ––– */
            if (!is_diagonal(*x_moves, *y_moves)) {
                send(player, "e-40", 4, 0);
                freeAll(piece_team, x_moves, y_moves);
                return false;  // Check if it's a valid diagonal move
            }
            if (!is_diagonal_clear(board, move)) {
                send(player, "e-41", 4, 0);
                freeAll(piece_team, x_moves, y_moves);
                return false;
            }
            if (eat_piece(board, move[2], move[3])) {
                send(player, "i-99", 4, 0);
                freeAll(piece_team, x_moves, y_moves);
                return true;
            }
            break;
        case 4: /* --- ♞ --- */
            if ((abs(*x_moves) != 1 || abs(*y_moves) != 2) && (abs(*x_moves) != 2 || abs(*y_moves) != 1)) {
                send(player, "e-50", 4, 0);
                freeAll(piece_team, x_moves, y_moves);
                return false;
            }
            if (eat_piece(board, move[2], move[3])) {
                send(player, "i-99", 4, 0);
                freeAll(piece_team, x_moves, y_moves);
                return true;
            }
            break;
        case 5: /* --- ♟ --- */
            // Moving in Y axis
            if (*y_moves == 1) {
                if (*piece_team == 1 && (*move - move[2]) == 1) {
                    if (eat_piece(board, move[2], move[3])) {
                        send(player, "i-99", 4, 0);
                        if (move[2] == 0) {
                            promote_piece(board, *move, move[1], *piece_team);
                            send(player, "i-98", 4, 0);
                        }
                    }
                    freeAll(piece_team, x_moves, y_moves);
                    return true;
                } else if (*piece_team == -1 && (*move - move[2]) == -1) {
                    if (eat_piece(board, move[2], move[3])) {
                        send(player, "i-99", 4, 0);
                        if (move[2] == 7) {
                            promote_piece(board, *move, move[1], *piece_team);
                            send(player, "i-98", 4, 0);
                        }
                    }
                    freeAll(piece_team, x_moves, y_moves);
                    return true;
                }
                freeAll(piece_team, x_moves, y_moves);
                return false;
            } else if (*y_moves == 0) {
                // Check if it's the first move
                if (*x_moves >= 2) {
                    if (!is_rect_clear(board, move, *x_moves, *y_moves)) {
                        send(player, "e-31", 4, 0);
                        freeAll(piece_team, x_moves, y_moves);
                        return false;
                    }else if (move[0] == 6 && *piece_team == 1 && (*move - move[2]) == 2 && get_piece_type(board[move[2]][move[3]])==-1) {
                        printf("First move 1\n");
                        freeAll(piece_team, x_moves, y_moves);
                        return true;
                    } else if (move[0] == 1 && *piece_team == -1 && (*move - move[2]) == -2 && get_piece_type(board[move[2]][move[3]])==-1) {
                        printf("First move 2\n");
                        freeAll(piece_team, x_moves, y_moves);
                        return true;
                    } else {
                        send(player, "e-62", 5, 0);
                        freeAll(piece_team, x_moves, y_moves);
                        return false;
                    } 
                    
                } else if (*x_moves == 1) {
                    if ((*piece_team == 1 && (*move - move[2]) == 1 && get_piece_type(board[move[2]][move[3]])==-1)) {
                        if (move[2] == 0) {
                            promote_piece(board, *move, move[1], *piece_team);
                            send(player, "i-98", 4, 0);
                        }
                        freeAll(piece_team, x_moves, y_moves);
                        return true;
                    } else if ((*piece_team == -1 && (*move - move[2]) == -1 && get_piece_type(board[move[2]][move[3]])==-1)) {
                        if (move[2] == 7) {
                            promote_piece(board, *move, move[1], *piece_team);
                            send(player, "i-98", 4, 0);
                        }
                        freeAll(piece_team, x_moves, y_moves);
                        return true;
                    } else {
                        freeAll(piece_team, x_moves, y_moves);
                        return false;
                    }
                } else {
                    freeAll(piece_team, x_moves, y_moves);
                    return false;
                }
            }
            break;
        default:
            break;
    }

    freeAll(piece_team, x_moves, y_moves);
    return true;
}

bool check_end_game(wchar_t **board) {
    int game_over = 2;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (get_piece_type(board[i][j]) == 0) {
                game_over--;
            }
        }
    }
    if (game_over == 0) {
        return false;
    } else {
        return true;
    }
}