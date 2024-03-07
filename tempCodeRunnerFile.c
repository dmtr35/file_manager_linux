void remove_directory_recursive(const char *path, char *file_name)
{
    size_t size_new_path = strlen(path) + strlen(file_name) + 3;
    char full_path[size_new_path];
    snprintf(full_path, size_new_path, "%s/%s", path, file_name);

    struct stat statbuf;
    if (lstat(full_path, &statbuf) == -1) {
        char original_full_path[size_new_path];
        strcpy(original_full_path, full_path);
        char *ptr = strstr(original_full_path, " ->");
            if (ptr != NULL) {
            *ptr = '\0'; // Обрезаем строку до ' ->'
        }
        unlink(original_full_path);  // Удалить символическую ссылку
        perror("Error getting file status");
        return;
    }