#include "../include/tarsau.h"

void extract_archive(char *archive_name, char *target_dir) {
    FILE *src = fopen(archive_name, "r");
    if (!src) {
        printf("Arşiv dosyası uygunsuz veya bozuk!\n");
        return;
    }

    // 1. Organizasyon Bölümü Boyutunu Oku (İlk 10 bayt)
    char size_str[11] = {0};
    if (fread(size_str, 1, 10, src) != 10) { 
        printf("Arşiv dosyası uygunsuz veya bozuk!\n"); 
        fclose(src);
        return;
    }
    
    long metadata_size = atol(size_str);
    // Eğer metadata boyutu geçersizse (0 veya negatifse) hata ver
    if (metadata_size <= 0) {
        printf("Arşiv dosyası uygunsuz veya bozuk!\n");
        fclose(src);
        return;
    }

    // 2. Metadata'yı Belleğe Al
    char *metadata = (char *)malloc(metadata_size + 1);
    char *metadata_copy = (char *)malloc(metadata_size + 1); 
    if (!metadata || !metadata_copy) {
        fclose(src);
        return;
    }
    
    fread(metadata, 1, metadata_size, src);
    metadata[metadata_size] = '\0';
    strcpy(metadata_copy, metadata);

    // 3. Hedef Dizini Oluştur
    struct stat st = {0};
    if (stat(target_dir, &st) == -1) {
        mkdir(target_dir, 0777); 
    }

    // 4. Metadata'yı Parçala ve Dosyaları Yaz
    char *entry = strtok(metadata, "|"); 
    long current_pos = 10 + metadata_size; 

    while (entry != NULL) {
        char name[256], mode_str[10];
        long f_size;
        
        sscanf(entry, "%[^,],%[^,],%ld", name, mode_str, &f_size); 

        char full_path[512];
        sprintf(full_path, "%s/%s", target_dir, name);

        FILE *dest = fopen(full_path, "w");
        if(dest) {
            fseek(src, current_pos, SEEK_SET);
            for (long i = 0; i < f_size; i++) {
                int ch = fgetc(src);
                if (ch != EOF) fputc(ch, dest);
            }
            fclose(dest);

            // İzinleri (okuma, yazma, çalıştırma) Geri Yükle 
            mode_t mode = (mode_t)strtol(mode_str, NULL, 8);
            chmod(full_path, mode); 
        }

        current_pos += f_size;
        entry = strtok(NULL, "|"); 
    }

    // 5. Çıktı Formatını Düzenle: t1, t2 ve t3 dosyaları açıldı formatı
    printf("%s dizininde ", target_dir); 
    char *list_entry = strtok(metadata_copy, "|");
    char *file_names[MAX_FILES];
    int file_idx = 0;

    while (list_entry != NULL && file_idx < MAX_FILES) {
        char name[256];
        sscanf(list_entry, "%[^,]", name);
        file_names[file_idx++] = strdup(name);
        list_entry = strtok(NULL, "|");
    }

    for (int i = 0; i < file_idx; i++) {
        if (i == file_idx - 1 && file_idx > 1) {
            printf("ve ");
        }
        printf("%s", file_names[i]);
        
        if (i < file_idx - 2) {
            printf(", ");
        } else if (i == file_idx - 2) {
            printf(" ");
        }
        free(file_names[i]);
    }
    printf(" dosyaları açıldı.\n"); 

    free(metadata);
    free(metadata_copy);
    fclose(src);
}