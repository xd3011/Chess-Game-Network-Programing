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
    FILE *file = fopen("user.txt", "a");  // Mở file ở chế độ ghi thêm (append)

    if (file == NULL) {
        printf("Not Open File.\n");
        return 0;
    }

    // Kiểm tra xem username đã tồn tại chưa
    if (checkLogin(username, NULL)) {  // Sử dụng hàm checkLogin để kiểm tra username
        printf("Account has existed\n");
        fclose(file);
        return 0;
    }

    // Ghi username và password vào file
    fprintf(file, "\n%s %s", username, password);

    fclose(file);
    printf("Register Successfully!\n");
    return 1;
}