#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "board.c"

#define RED "\x1B[31m"
#define RESET "\x1B[0m"
#define GREEN "\x1B[32m"

int loggedIn = 0;

void menuLogin(void *sockfd) {
    int mainChoice;
    char username[24], password[24];
    char buffer[4];
    int n;
    int socket = *(int *)sockfd;
    while (1) {
        printf("Menu:\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Select: ");
        scanf("%d", &mainChoice);
        switch (mainChoice) {
            case 1:
                n = write(socket, "login", 5);
                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                printf("Input user name: ");
                scanf("%s", username);
                printf("Input password: ");
                scanf("%s", password);
                strcat(username, " ");
                strcat(username, password);
                n = write(socket, username, strlen(username));
                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                while (1) {
                    bzero(buffer, 4);
                    n = read(socket, buffer, 4);
                    if (n < 0) {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    if (buffer[0] == 't') {
                        printf("Login success\n");
                        loggedIn = 1;
                        return;
                    } else if (buffer[0] == 'f') {
                        printf("Login false\n");
                        break;
                    }
                }
                break;
            case 2:
                n = write(socket, "register", 8);
                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                printf("Input user name: ");
                scanf("%s", username);
                printf("Input password: ");
                scanf("%s", password);
                strcat(username, " ");
                strcat(username, password);
                n = write(socket, username, strlen(username));
                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                while (1) {
                    bzero(buffer, 4);
                    n = read(socket, buffer, 4);
                    if (n < 0) {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    if (buffer[0] == 't') {
                        printf("Register success\n");
                        break;
                    } else if (buffer[0] == 'f') {
                        printf("Register false\n");
                        break;
                    }
                }
                break;
            case 3:
                n = write(socket, "exit", 4);
                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                printf("Exit Success.\n");
                exit(1);
            default:
                printf("Reselect.\n");
                break;
        }
    }
}

void menuGame(void *sockfd) {
    int mainChoice;
    char buffer[8];
    char invite[6];
    char username[24];
    char oldPassword[24], newPassword[24];
    int n, check;
    int socket = *(int *)sockfd;
    while (1) {
        printf("Menu:\n");
        printf("1. Create Room\n");
        printf("2. Waiting\n");
        printf("3. Change Password\n");
        printf("4. Logout\n");
        printf("Select: ");
        scanf("%d", &mainChoice);
        switch (mainChoice) {
            case 1:
                n = write(socket, "cre-room", 8);
                while (1) {
                    bzero(buffer, 8);
                    n = read(socket, buffer, 8);
                    if (n < 0) {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    if (strcmp(buffer, "cre-true")) {
                        printf("Create room\n");
                        loggedIn = 2;
                        return;
                    }
                }
                break;
            case 2:
                n = write(socket, "waitting", 8);
                while (1) {
                    // Code waiting invite
                    bzero(invite, 6);
                    n = read(socket, invite, 6);
                    if (n < 0) {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    if (invite[0] == 'i' && invite[1] == 'n') {
                        bzero(username, 24);
                        n = read(socket, username, 24);
                        if (n < 0) {
                            perror("ERROR reading from socket");
                            exit(1);
                        }
                        printf("Player %s invites you into the room\n", username);
                        printf("1. Accept\n");
                        printf("2. Refuses\n");
                        printf("Input Chooses: ");
                        scanf("%d", &check);
                        if (check == 1) {
                            n = write(socket, "accept", 6);
                            if (n < 0) {
                                perror("ERROR writing to socket");
                                exit(1);
                            }
                            n = write(socket, username, strlen(username));
                            if (n < 0) {
                                perror("ERROR writing to socket");
                                exit(1);
                            }
                            loggedIn = 3;
                            return;
                        } else if (check == 2) {
                            n = write(socket, "refuse", 6);
                            if (n < 0) {
                                perror("ERROR writing to socket");
                                exit(1);
                            }
                            n = write(socket, username, strlen(username));
                            if (n < 0) {
                                perror("ERROR writing to socket");
                                exit(1);
                            }
                            break;
                        }
                    }
                }
                break;
            case 3:
                n = write(socket, "changepa", 8);
                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                printf("Input Old Password: ");
                scanf("%s", oldPassword);
                printf("Input New Password: ");
                scanf("%s", newPassword);
                strcat(oldPassword, " ");
                strcat(oldPassword, newPassword);
                n = write(socket, oldPassword, strlen(oldPassword));
                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                while (1) {
                    bzero(buffer, 4);
                    n = read(socket, buffer, 4);
                    if (n < 0) {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    if (buffer[0] == 't') {
                        printf("Change Password success\n");
                        break;
                    } else if (buffer[0] == 'f') {
                        printf("Change Password false\n");
                        break;
                    }
                }
                break;
            case 4:
                n = write(socket, "log--out", 8);
                while (1) {
                    bzero(buffer, 8);
                    n = read(socket, buffer, 8);
                    if (n < 0) {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    buffer[8] = '\0';
                    printf("%s-%d\n", buffer, strcmp(buffer, "log-true"));
                    if (strcmp(buffer, "log-true") == 0) {
                        printf("Logout Success\n");
                        loggedIn = 0;
                        return;
                    }
                }
                break;
            default:
                printf("Reselect.\n");
                break;
        }
    }
}

// void *readGame(void *sockfd) {
//     int n, check;
//     char invite[8];
//     char username[24];
//     int socket = *(int *)sockfd;
//     while (1) {
//         // Code waiting invite
//         bzero(invite, 8);
//         n = read(socket, invite, 8);
//         invite[8] = '\0';
//         if (n < 0) {
//             perror("ERROR reading from socket");
//             exit(1);
//         }
//         if (invite[0] == 'i' && invite[1] == 'n') {
//             bzero(username, 24);
//             n = read(socket, username, 24);
//             if (n < 0) {
//                 perror("ERROR reading from socket");
//                 exit(1);
//             }
//             printf("Player %s invites you into the room\n", username);
//             printf("1. Accept\n");
//             printf("2. Refuses\n");
//             printf("Input Chooses: ");
//             scanf("%d", &check);
//             if (check == 1) {
//                 n = write(socket, "accept", 6);
//                 if (n < 0) {
//                     perror("ERROR writing to socket");
//                     exit(1);
//                 }
//                 n = write(socket, username, strlen(username));
//                 if (n < 0) {
//                     perror("ERROR writing to socket");
//                     exit(1);
//                 }
//                 loggedIn = 3;
//                 return NULL;
//             } else if (check == 2) {
//                 n = write(socket, "refuse", 6);
//                 if (n < 0) {
//                     perror("ERROR writing to socket");
//                     exit(1);
//                 }
//                 n = write(socket, username, strlen(username));
//                 if (n < 0) {
//                     perror("ERROR writing to socket");
//                     exit(1);
//                 }
//                 break;
//             }
//         } else if (strcmp(invite, "cre-room") == 0) {
//             printf("Create room\n");
//             loggedIn = 2;
//             return NULL;
//         }
//     }
// }

// void *menuGame(void *sockfd) {
//     int mainChoice;
//     char buffer[8];
//     int n;
//     int socket = *(int *)sockfd;
//     while (1) {
//         printf("Menu:\n");
//         printf("1. Create Room\n");
//         printf("2. Logout\n");
//         printf("Select: ");
//         scanf("%d", &mainChoice);
//         switch (mainChoice) {
//             case 1:
//                 n = write(socket, "cre-room", 8);
//                 while (1) {
//                     bzero(buffer, 8);
//                     n = read(socket, buffer, 8);
//                     if (n < 0) {
//                         perror("ERROR reading from socket");
//                         exit(1);
//                     }
//                     printf("%s\n", buffer);
//                     if (strcmp(buffer, "cre-true")) {
//                         printf("Create room\n");
//                         loggedIn = 2;
//                         return NULL;
//                     }
//                 }
//                 break;
//             case 2:
//                 n = write(socket, "log--out", 8);
//                 while (1) {
//                     bzero(buffer, 8);
//                     n = read(socket, buffer, 8);
//                     if (n < 0) {
//                         perror("ERROR reading from socket");
//                         exit(1);
//                     }
//                     if (strcmp(buffer, "log-true") == 0) {
//                         printf("Logout Success\n");
//                         loggedIn = 0;
//                         return NULL;
//                     }
//                 }
//                 break;
//             default:
//                 printf("Reselect.\n");
//                 break;
//         }
//     }
// }

void menuOnRoom(void *sockfd) {
    int mainChoice;
    char buffer[1024];
    char dataread[7];
    char username[24];
    int n;
    int socket = *(int *)sockfd;
    while (1) {
        printf("Menu:\n");
        printf("1. Get User Online\n");
        printf("2. Invite Players\n");
        printf("3. Remove Room\n");
        printf("Select: ");
        scanf("%d", &mainChoice);
        switch (mainChoice) {
            case 1:
                n = write(socket, "get-user", 8);
                while (1) {
                    bzero(buffer, 1024);
                    n = read(socket, buffer, 1024);
                    if (n < 0) {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    printf("User Online:\n");
                    printf("%s\n", buffer);
                    if (n >= 0) {
                        break;
                    }
                }
                break;
            case 2:
                n = write(socket, "invite--", 8);
                printf("Input opponent: ");
                scanf("%s", username);
                n = write(socket, username, strlen(username));
                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                while (1) {
                    bzero(dataread, 6);
                    n = read(socket, dataread, 6);
                    if (n < 0) {
                        perror("ERROR reading from socket");
                        exit(1);
                    }
                    dataread[6] = '\0';
                    if (strcmp(dataread, "accept") == 0) {
                        printf("The opponent party success\n");
                        n = write(socket, "accept", 6);
                        if (n < 0) {
                            perror("ERROR writing to socket");
                            exit(1);
                        }
                        loggedIn = 3;
                        return;
                    } else if (strcmp(dataread, "refuse") == 0) {
                        printf("The opponent party refuses\n");
                        n = write(socket, "refuse", 6);
                        if (n < 0) {
                            perror("ERROR writing to socket");
                            exit(1);
                        }
                        break;
                    }
                }
                break;
            case 3:
                n = write(socket, "remove--", 8);
                printf("Remove Success!\n");
                loggedIn = 1;
                return;
            default:
                printf("Reselect.\n");
                break;
        }
    }
}

void *on_signal(void *sockfd) {
    char buffer[64];
    int n;
    int socket = *(int *)sockfd;
    int *player = (int *)malloc(sizeof(int *));

    while (1) {
        bzero(buffer, 64);
        n = read(socket, buffer, 64);

        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        if (buffer[0] == 'i' || buffer[0] == 'e' || buffer[0] == '\0') {
            if (buffer[0] == 'i') {
                if (buffer[2] == 't') {
                    printf("\nMake your move: \n");
                }
                if (buffer[2] == 'n') {
                    printf("\nWaiting for opponent...\n");
                }
                if (buffer[2] == 'p') {
                    *player = atoi(&buffer[3]);
                    if (*player == 2) {
                        printf("You're blacks (%c)\n", buffer[3]);
                    } else {
                        printf("You're whites (%c)\n", buffer[3]);
                    }
                }
            } else if (buffer[0] == 'e') {
                // Syntax errors
                if (buffer[2] == '0') {
                    switch (buffer[3]) {
                        case '0':
                            printf(RED "  ↑ ('-' missing)\n" RESET);
                            break;
                        case '1':
                            printf(RED "↑ (should be letter)\n" RESET);
                            break;
                        case '2':
                            printf(RED "   ↑ (should be letter)\n" RESET);
                            break;
                        case '3':
                            printf(RED " ↑ (should be number)\n" RESET);
                            break;
                        case '4':
                            printf(RED " ↑ (out of range)\n" RESET);
                            break;
                        case '5':
                            printf(RED "   ↑ (should be number)\n" RESET);
                            break;
                        case '6':
                            printf(RED "   ↑ (out of range)\n" RESET);
                            break;
                        case '7':
                            printf(RED "(out of range)\n" RESET);
                            break;
                    }
                }
                printf("\nerror %s\n", buffer);
            }
            // Check if it's an informative or error message
        } else if (buffer[0] == 'w') {
            printf("You win\n");
            exit(1);
        } else if (buffer[0] == 'l') {
            printf("You lose\n");
            exit(1);
        } else if (buffer[0] == 'b') {
            printf("Your opponent has left the room. Please leave here\n");
            exit(1);
        } else if (buffer[0] == 'g' && buffer[2] == 'e') {
            printf("The opponent surrendered. Yes or No\n");
        } else if (buffer[0] == 'g' && buffer[2] == 'o') {
            printf("You lose\n");
            exit(1);
        } else if (buffer[0] == 'g' && buffer[2] == 'n') {
            printf("The opponent refused. Let's move\n");
            continue;
        } else {
            // Print the board
            system("clear");
            if (*player == 1) {
                print_board_buff(buffer);
            } else {
                print_board_buff_inverted(buffer);
            }
        }

        bzero(buffer, 64);
    }
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    setlocale(LC_ALL, "en_US.UTF-8");
    char buffer[64];

    if (argv[2] == NULL) {
        portno = 80;
    } else {
        portno = atoi(argv[2]);
    }

    printf("Connecting to %s:%d\n", argv[1], portno);

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    /* Now ask for a message from the user, this message
     * will be read by server
     */
    while (1) {
        if (loggedIn == 0) {
            menuLogin(&sockfd);
        } else if (loggedIn == 1) {
            // pthread_t menuThreadId, readThreadId;
            // if (pthread_create(&menuThreadId, NULL, &menuGame, &sockfd) != 0) {
            //     perror("ERROR creating menu thread");
            //     exit(1);
            // }

            // // Tạo thread cho đọc dữ liệu từ server
            // if (pthread_create(&readThreadId, NULL, &readGame, &sockfd) != 0) {
            //     perror("ERROR creating read thread");
            //     exit(1);
            // }
            // while (1) {
            //     if (loggedIn != 1) {
            //         break;
            //     }
            // }
            menuGame(&sockfd);
        } else if (loggedIn == 2) {
            menuOnRoom(&sockfd);
        } else if (loggedIn == 3) {
            pthread_t tid[1];

            pthread_create(&tid[0], NULL, &on_signal, &sockfd);

            while (1) {
                bzero(buffer, 64);
                fgets(buffer, 64, stdin);

                /* Send message to the server */
                n = write(sockfd, buffer, strlen(buffer));

                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
            }
        }
    }

    return 0;
}
