#include "../include/tarsau.h"

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    // --- ARŞİVLEME BÖLÜMÜ (-b) ---
    if (strcmp(argv[1], "-b") == 0) { 
        char *inputs[MAX_FILES];
        int count = 0;
        char *output = "a.sau"; 

        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) { 
                output = argv[++i];
            } else {
                // KURAL: Eğer mevcut dosya sayısı MAX_FILES (32) sınırına ulaştıysa
                // ve hala döngüde yeni bir dosya okunmaya çalışılıyorsa hata ver!
                if (count >= MAX_FILES) {
                    printf("32 den fazla dosya işlenemez!\n");
                    return 1;
                }
                inputs[count++] = argv[i];
            }
        }
        archive_files(count, inputs, output);
    } 
    // --- ARŞİVDEN ÇIKARMA BÖLÜMÜ (-a) ---
    else if (strcmp(argv[1], "-a") == 0) { 
        // 1. Parametre Sayısı Kontrolü: -a'dan sonra en fazla 2 (toplamda 4) parametre.
        if (argc < 3 || argc > 4) {
            printf("Arşiv dosyası uygunsuz veya bozuk!\n");
            return 1;
        }

        char *archive_name = argv[2];
        
        // 2. Uzantı Kontrolü: Dosya .sau ile bitmeli.
        char *ext = strrchr(archive_name, '.');
        if (!ext || strcmp(ext, ".sau") != 0) {
            printf("Arşiv dosyası uygunsuz veya bozuk!\n");
            return 1;
        }

        // 3. Dizin Belirleme: Parametre varsa o, yoksa "."
        char *target = (argc == 4) ? argv[3] : "."; 
        extract_archive(archive_name, target);
    }
    
    return 0;
}