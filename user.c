int checkLogin(char *username, char *password) {
    FILE *file = fopen("user.txt", "r");
    char line[100];

    if (file == NULL) {
        printf("Not Open File.\n");
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *storedUsername = strtok(line, " ");
        char *storedPassword = strtok(NULL, " \n");
        if (storedUsername != NULL && storedPassword != NULL) {
            if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

int registerAccount(char *username, char *password) {
    FILE *file = fopen("user.txt", "a");

    if (file == NULL) {
        printf("Not Open File.\n");
        return 0;
    }

    // Check if the username already exists
    if (checkLogin(username, NULL)) {
        printf("Account has existed\n");
        fclose(file);
        return 0;
    }

    fprintf(file, "\n%s %s", username, password);

    fclose(file);
    printf("Register Successfully!\n");
    return 1;
}

void logLogin(char *username) {
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);

    FILE *file = fopen("login.txt", "a");
    if (file == NULL) {
        printf("Not Open File.\n");
        return;
    }

    fprintf(file, "%s %d-%d-%d %d:%d:%d\n", username, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    fclose(file);
}

void logEndGame(char *username1, char *username2, int status) {
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);

    FILE *gameFile = fopen("game.txt", "a");
    if (gameFile == NULL) {
        printf("Not Open Log Game File.\n");
        return;
    }
    if (status == 1) {
        fprintf(gameFile, "%s > %s %d-%d-%d %d:%d:%d\n",
                username1, username2,
                tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    } else if (status == 2) {
        fprintf(gameFile, "%s < %s %d-%d-%d %d:%d:%d\n",
                username1, username2,
                tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    }
    fclose(gameFile);
    return;
}
void logStart(char *username1, char *username2, char *filename) {
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);

    FILE *gameFile = fopen(filename, "a");
    if (gameFile == NULL) {
        printf("Cannot open log file: %s\n", filename);
        return;
    }

    fprintf(gameFile, "%s vs %s %d-%d-%d %d:%d:%d\n",
            username1, username2,
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    fclose(gameFile);
    return;
}

void logOnGame(char *username, char *buffer, char *filename) {
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);

    FILE *gameFile = fopen(filename, "a");
    if (gameFile == NULL) {
        printf("Not Open Log Game File.\n");
        return;
    }
    fprintf(gameFile, "%s move: %s %d-%d-%d %d:%d:%d\n",
            username, buffer,
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    fclose(gameFile);
    return;
}

int changePassword(char *username, char *oldPassword, char *newPassword) {
    FILE *file = fopen("user.txt", "r");
    char line[100];
    int userFound = 0;

    if (file == NULL) {
        printf("Not Open File.\n");
        return 0;
    }

    FILE *tempFile = fopen("temp_user.txt", "w");

    if (tempFile == NULL) {
        fclose(file);
        printf("Not Open Temp File.\n");
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *storedUsername = strtok(line, " ");
        char *storedPassword = strtok(NULL, " \n");

        if (storedUsername != NULL && storedPassword != NULL) {
            if (strcmp(username, storedUsername) == 0) {
                userFound = 1;
                if (strcmp(oldPassword, storedPassword) == 0) {
                    // Nếu tên người dùng và mật khẩu cũ khớp, thay đổi mật khẩu
                    fprintf(tempFile, "%s %s\n", username, newPassword);
                } else {
                    // Mật khẩu cũ không khớp
                    printf("Incorrect old password.\n");
                    fclose(file);
                    fclose(tempFile);
                    remove("temp_user.txt");  // Xóa tệp tạm thời
                    return 0;
                }
            } else {
                // Không phải tên người dùng cần thay đổi mật khẩu, ghi lại thông tin người dùng
                fprintf(tempFile, "%s %s\n", storedUsername, storedPassword);
            }
        }
    }

    fclose(file);
    fclose(tempFile);

    // Xóa tệp cũ và đổi tên tệp tạm thời thành tên tệp gốc
    remove("user.txt");
    rename("temp_user.txt", "user.txt");

    if (userFound) {
        printf("Password changed successfully.\n");
        return 1;
    } else {
        printf("User not found.\n");
        return 0;
    }
}