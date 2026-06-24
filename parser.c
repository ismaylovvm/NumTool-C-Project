#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

// trim_whitespace: Bir metnin başındaki ve sonundaki gereksiz boşlukları siler.
// Pointer aritmetiği kullanıyoruz çünkü bellekteki orijinal metin üzerinde doğrudan değişiklik yapmak istiyoruz.
void trim_whitespace(char *str) {
    char *end;

    // isspace: Karakter boşluk, tab (\t) veya yeni satır (\n) ise true döner.
    // Metnin başındaki boşlukları geçene kadar pointer'ı bir sağa kaydır (str++).
    while(isspace((unsigned char)*str)) str++;

    // Eğer string tamamen boşluktan oluşuyorsa veya bitmişse fonksiyonu sonlandır.
    if(*str == 0) return;

    // Metnin sonuna git (NULL karakterinden bir önceki karakter).
    end = str + strlen(str) - 1;

    // Sondan başa doğru gelerek sondaki boşlukları bul.
    while(end > str && isspace((unsigned char)*end)) end--;

    // Bulduğumuz son anlamlı karakterin hemen sağına \0 (string bitirici) koyarak stringi kesiyoruz.
    end[1] = '\0';
}

CommandRecord* parse_file(const char* filename, int* record_count) {
    // Dosyayı sadece okuma ("r" - read) modunda açıyoruz.
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Hata: %s dosyasi acilamadi!\n", filename);
        return NULL; // Dosya yoksa programın çökmemesi için işlemi iptal et.
    }

    int capacity = 10; // Dizimizin başlangıç kapasitesi.
    *record_count = 0; // Başlangıçta hiç geçerli komut okumadık.
    
    // malloc: İşletim sisteminden "Kapasite çarpı Struct'ın boyutu" kadar byte alan istiyoruz.
    CommandRecord* records = (CommandRecord*)malloc(capacity * sizeof(CommandRecord));
    if (records == NULL) { // RAM'de yer kalmamış olabilir.
        fclose(file); // Açık kalan dosyayı kapat (Memory leak olmaması için).
        return NULL;
    }

    char line[512]; // Okunacak her bir satırı geçici olarak tutacağımız tampon (buffer) dizi.
    
    // fgets: Dosyadan satır satır okuma yapar. Satır bittiğinde veya dosya bittiğinde döngüden çıkar.
    while (fgets(line, sizeof(line), file)) {
        // Okunan satırın başındaki ve sonundaki boşlukları/yeni satır (\n) karakterini temizle.
        trim_whitespace(line);
        
        // Eğer satır tamamen boşsa (strlen 0) veya yorum satırıysa ('#') bu satırı atla, döngünün başına dön.
        if (strlen(line) == 0 || line[0] == '#') continue;

        // strtok: Bir metni belirli bir karaktere (delimiter) göre parçalar. Biz noktalı virgüle (;) göre bölüyoruz.
        // DİKKAT: strtok orijinal metnin içindeki ';' karakterlerini '\0' ile değiştirerek metni fiziksel olarak parçalar.
        char* token = strtok(line, ";");
        
        // Satırda ayrıştırılacak parça kalmayana kadar döngüye devam et.
        while (token != NULL) {
            char cmd_buffer[256];
            strcpy(cmd_buffer, token); // Parçalanan komutu geçici bir değişkene kopyala.
            trim_whitespace(cmd_buffer); // O parçanın da başındaki/sondaki boşlukları temizle.

            if (strlen(cmd_buffer) > 0) {
                // EĞER DİZİ DOLDUYSA KAPASİTEYİ ARTIR (Dinamik Dizi Mantığı)
                if (*record_count >= capacity) {
                    capacity *= 2; // Kapasiteyi 2 katına çıkar.
                    // realloc: Mevcut dizideki verileri silmeden hafızada daha geniş bir alan ayarlar.
                    CommandRecord* temp = (CommandRecord*)realloc(records, capacity * sizeof(CommandRecord));
                    if (temp == NULL) { // Realloc başarısız olursa orijinal veriyi free yapmalıyız.
                        free(records);
                        fclose(file);
                        return NULL;
                    }
                    records = temp; // Yeni ve geniş alanı ana pointerımıza atıyoruz.
                }

                // Dizideki sıradaki boş CommandRecord alanının adresini alıyoruz.
                CommandRecord* current = &records[*record_count];
                
                // memset: O alanı RAM'de kalmış olabilecek eski çöp verilerden (garbage data) temizleyip sıfırlıyoruz.
                memset(current, 0, sizeof(CommandRecord));
                
                // sscanf: Metnin içinden formatlı okuma yapar. %s diyerek ilk kelimeyi (Örn: "GCD") komut alanına çekiyoruz.
                sscanf(cmd_buffer, "%s", current->command);
                
                // Komut adından (Örn: "GCD ") sonraki kısmın bellek adresini hesaplıyoruz ki sadece sayıları okuyabilelim.
                char* args_str = cmd_buffer + strlen(current->command);
                long long val;
                int offset;
                
                // %lld: Long long bir sayı oku.
                // %n: Kaç karakter okuduğunu say ve offset değişkenine yaz.
                // Bu döngü, string'in içinde okunacak sayı bitene kadar sayıları arka arkaya çıkarır.
                while (sscanf(args_str, "%lld%n", &val, &offset) == 1) {
                    if (current->arg_count < 4) { // En fazla 4 parametre alabiliriz.
                        current->args[current->arg_count++] = val;
                    }
                    // Okuduğumuz sayının karakter uzunluğu kadar metinde ileri gidiyoruz (pointer aritmetiği).
                    args_str += offset;
                }
                
                (*record_count)++; // Geçerli bir komut okuduk, toplam sayacı 1 artır.
            }
            // strtok'a NULL parametresi vermek, "kaldığın yerden ayrıştırmaya devam et" demektir.
            token = strtok(NULL, ";"); 
        }
    }

    fclose(file); // İşimiz bitince dosyayı kapatıyoruz.
    return records; // Doldurduğumuz dinamik dizinin başlangıç adresini döndürüyoruz.
}