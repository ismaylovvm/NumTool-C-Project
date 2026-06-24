
/*
 * Ad Soyad: Ali Tutkaç
 * Numara: 443557
 * Ders: Programlama II Dönem Ödevi
 * Proje: NumTool - Sayı Teorisi Hesaplama Aracı
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "math_utils.h"

// argc: Komut satırına girilen kelime sayısı.
// argv: O kelimelerin metin olarak dizisi. 
// Örn: "./numtool input.txt output.txt" için argc 3 olur.
int main(int argc, char* argv[]) {
    
    // 2.1 Argüman Kontrolü (ZORUNLU KURAL)
    // Beklenen format: 1. programın adı, 2. giriş dosyası, 3. çıkış dosyası.
    if (argc != 3) {
        printf("Kullanim: ./numtool <giris_dosyasi> <cikis_dosyasi>\n");
        return 1; // Programı hata koduyla (1) sonlandır.
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    int record_count = 0;
    
    // parser.c'de yazdığımız fonksiyonu çağırıyoruz. Dinamik dizi RAM'de oluşturulup pointer olarak dönüyor.
    CommandRecord* records = parse_file(input_file, &record_count);

    if (records == NULL) {
        printf("Dosya islenirken bir hata olustu veya dosya bos.\n");
        return 1;
    }

    // Çıktı dosyasını yazma ("w" - write) modunda açıyoruz.
    FILE* out = fopen(output_file, "w");
    if (out == NULL) {
        printf("Hata: Cikti dosyasi olusturulamadi!\n");
        free(records); // Çıkmadan önce RAM'i temizlemeyi unutmuyoruz.
        return 1;
    }

    // Dinamik dizimizdeki her bir komutun üzerinden tek tek geçiyoruz.
    for (int i = 0; i < record_count; i++) {
        // records[i]'nin adresini alıyoruz ki direkt onun üzerinde işlem yapalım.
        CommandRecord* cmd = &records[i];
        cmd->is_error = 0;

        // strcmp: İki metni karşılaştırır. Eşitlerse 0 döner.
        // Hangi komutun geldiğini tespit edip ilgili fonksiyonu çağırıyoruz.
        
        if (strcmp(cmd->command, "GCD") == 0) {
            // Hatalı argüman sayısı veya negatif/sıfır girdi kontrolü
            if (cmd->arg_count != 2 || cmd->args[0] <= 0 || cmd->args[1] <= 0) {
                sprintf(cmd->result, "ERROR_INVALID_INPUT");
                cmd->is_error = 1;
                // Beklenen çıktı formatında istenildiği gibi negatif girişleri de dosyaya yazdırıyoruz.
                fprintf(out, "GCD %lld %lld -> %s\n", 
                    (cmd->arg_count > 0 ? cmd->args[0] : 0), 
                    (cmd->arg_count > 1 ? cmd->args[1] : 0), 
                    cmd->result);
                continue; // Hata olduğu için işlem yapmadan bir sonraki komuta geç.
            } else {
                long long res = gcd(cmd->args[0], cmd->args[1]);
                fprintf(out, "GCD %lld %lld -> %lld\n", cmd->args[0], cmd->args[1], res);
            }
        } 
        else if (strcmp(cmd->command, "POW") == 0) {
            if (cmd->arg_count != 3) {
                sprintf(cmd->result, "ERROR_INVALID_INPUT");
                cmd->is_error = 1;
            } else {
                long long res = power_mod(cmd->args[0], cmd->args[1], cmd->args[2]);
                fprintf(out, "POW %lld %lld %lld -> %lld\n", cmd->args[0], cmd->args[1], cmd->args[2], res);
            }
        } 
        else if (strcmp(cmd->command, "PRIME") == 0) {
            if (cmd->arg_count != 1) continue;
            int res = is_prime(cmd->args[0]);
            if (res == -1) { // is_prime hata durumunda -1 dönüyordu
                fprintf(out, "PRIME %lld -> ERROR_INVALID_INPUT\n", cmd->args[0]);
                cmd->is_error = 1;
            } else {
                // Ternary operatör (res ? "YES" : "NO"): res 1 ise YES, 0 ise NO yazar.
                fprintf(out, "PRIME %lld -> %s\n", cmd->args[0], res ? "YES" : "NO");
            }
        } 
        else if (strcmp(cmd->command, "INV") == 0) {
            if (cmd->arg_count != 2) continue;
            int has_inverse = 0;
            // Pointer sayesinde fonksiyon has_inverse değişkeninin değerini içeriden değiştirebilecek.
            long long res = mod_inverse(cmd->args[0], cmd->args[1], &has_inverse);
            if (!has_inverse) {
                fprintf(out, "INV %lld %lld -> ERROR_NO_INVERSE\n", cmd->args[0], cmd->args[1]);
                cmd->is_error = 1;
            } else {
                fprintf(out, "INV %lld %lld -> %lld\n", cmd->args[0], cmd->args[1], res);
            }
        } 
        else if (strcmp(cmd->command, "PHI") == 0) {
            if (cmd->arg_count != 1) continue;
            long long res = euler_totient(cmd->args[0]);
            if (res == -1) {
                fprintf(out, "PHI %lld -> ERROR_INVALID_INPUT\n", cmd->args[0]);
                cmd->is_error = 1;
            } else {
                fprintf(out, "PHI %lld -> %lld\n", cmd->args[0], res);
            }
        } 
        else if (strcmp(cmd->command, "CHECK") == 0) {
            if (cmd->arg_count != 2) continue;
            int has_inverse = 0;
            long long inv = mod_inverse(cmd->args[0], cmd->args[1], &has_inverse);
            if (!has_inverse) {
                fprintf(out, "CHECK %lld %lld -> ERROR_NO_INVERSE\n", cmd->args[0], cmd->args[1]);
                cmd->is_error = 1;
            } else {
                // CHECK KOMUTU MANTIĞI: A'nın M modundaki tersini bulduk (inv). 
                // Şimdi (A * inv) % M işleminin sonucu 1 çıkıyorsa işlem CORRECT'tir.
                // Çarpım taşmasını önlemek için ikisini önce kendi modlarında küçültüp öyle çarpıyoruz.
                long long test = ((cmd->args[0] % cmd->args[1]) * (inv % cmd->args[1])) % cmd->args[1];
                if (test == 1) {
                    fprintf(out, "CHECK %lld %lld -> CORRECT\n", cmd->args[0], cmd->args[1]);
                } else {
                    fprintf(out, "CHECK %lld %lld -> FAILED\n", cmd->args[0], cmd->args[1]);
                }
            }
        }
    }

    // Dosyaya yazma işlemi bitti, dosyayı kapatıyoruz.
    fclose(out);
    
    // EN ÖNEMLİ KISIM (Memory Leak Koruması):
    // Program bitmeden hemen önce malloc ve realloc ile RAM'de ayırdığımız devasa dinamik alanı
    // free fonksiyonu ile işletim sistemine geri iade ediyoruz.
    free(records); 

    return 0; // Program başarıyla (0) sonlandı
}