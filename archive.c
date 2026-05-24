#include "../include/tarsau.h"

// Giriş dosyalarının yalnızca metin dosyası (ASCII) olup olmadığını kontrol eder [cite: 12]
int is_ascii(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    int c;
    while ((c = fgetc(f)) != EOF) {
        // Karakter başına 1 bayt kontrolü [cite: 12]
        if (!isascii(c)) {
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return 1;
}

void archive_files(int file_count, char *filenames[], char *output_name) {
    // Giriş dosyası sayısı en fazla 32 olabilir 
    if (file_count > MAX_FILES) {
        printf("Hata: Giriş dosyası sayısı en fazla 32 olabilir!\n");
        exit(0);
    }

    char metadata[8192] = ""; // Stack smashing'i önlemek için kapasite artırıldı
    long total_size = 0;

    for (int i = 0; i < file_count; i++) {
        struct stat st;
        // Dosya varlık ve ASCII format kontrolü [cite: 12, 20]
        if (stat(filenames[i], &st) != 0 || !is_ascii(filenames[i])) {
            printf("%s giriş dosyasının formatı uyumsuzdur!\n", filenames[i]); // 
            exit(0); // Sorunsuz bir şekilde programdan çıkılır 
        }
        total_size += st.st_size;
        
        // Kayıttaki alanlar: |Dosya adı, izinler, boyut| [cite: 36]
        char entry[512];
        sprintf(entry, "|%s,%04o,%ld|", filenames[i], (unsigned int)(st.st_mode & 0777), (long)st.st_size);
        strcat(metadata, entry);
    }

    // Giriş dosyalarının toplam boyutu 200 MB'ı geçemez [cite: 18]
    if (total_size > MAX_TOTAL_SIZE) {
        printf("Hata: Toplam boyut 200 MB'ı geçemez.\n");
        return;
    }

    FILE *out = fopen(output_name, "w");
    if (!out) return;

    // İlk 10 bayt, organizasyon bölümünün ASCII formatındaki sayısal boyutunu içerir [cite: 34]
    fprintf(out, "%010lu", (unsigned long)strlen(metadata)); 
    
    // Organizasyon (içerik) bölümü yazılır [cite: 32, 33]
    fputs(metadata, out); 

    // Arşivlenmiş dosyalar, ASCII formatında art arda yerleştirilir [cite: 38, 39]
    for (int i = 0; i < file_count; i++) {
        FILE *in = fopen(filenames[i], "r");
        if (in) {
            int ch;
            while ((ch = fgetc(in)) != EOF) {
                fputc(ch, out);
            }
            fclose(in);
        }
    }
    
    fclose(out);
    printf("Dosyalar birleştirildi.\n"); // [cite: 5, 8]
}