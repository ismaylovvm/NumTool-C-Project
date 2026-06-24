#ifndef PARSER_H
#define PARSER_H

// typedef struct: Kendi özel veri tipimizi oluşturuyoruz.
// Her bir komut satırı okunduğunda verileri bu kalıba göre RAM'de tutacağız.
typedef struct {
    // Komutun adını metin olarak tutar (Örn: "GCD", "POW". 20 karakter fazlasıyla yeterli)
    char command[20];       
    
    // Komutun aldığı sayısal argümanlar. Büyük sayılar test edileceği için int yerine long long seçtik.
    long long args[4];      
    
    // O satırda okuduğumuz komutun kaç tane parametresi olduğunu sayarız.
    int arg_count;          
    
    // İşlem sonucunu (sayı veya hata mesajı olarak) metne çevirip burada saklarız.
    char result[100];       
    
    // Bir nevi bayrak (flag) değişkeni. İşlemde hata çıkarsa 1, başarılıysa 0 yaparız. 
    // Hata durumunda çıktıya farklı format basmak için main.c'de kullanacağız.
    int is_error;           
} CommandRecord;

// Ayrıştırma fonksiyonumuzun prototipi.
// main.c dosyası bu fonksiyonu çağırabilmek için önceden adını ve parametrelerini bilmelidir.
CommandRecord* parse_file(const char* filename, int* record_count);

#endif