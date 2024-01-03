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