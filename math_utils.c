#include "math_utils.h"

// 6.1. Öklid Algoritması (O(log(min(a,b))))
// Mantık: İki sayının EBOB'u, büyük sayının küçük sayıya bölümünden kalanı ile küçük sayının EBOB'una eşittir.
long long gcd(long long a, long long b) {
    // a ve b'nin mutlak değerlerini alıyoruz çünkü EBOB negatif olmaz.
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    
    // b sıfır olana kadar döngü devam eder. Sıfır olduğunda a değeri EBOB'dur.
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 6.2. Genişletilmiş Öklid Algoritması ve Modüler Ters (O(log(min(a,m))))
// A'nın M modunda tersi, (A * x) % M = 1 denklemini sağlayan x değeridir.
long long mod_inverse(long long A, long long M, int* has_inverse) {
    long long m0 = M;
    long long y = 0, x = 1;

    // Modül 1 ise tersi yoktur.
    if (M == 1) {
        *has_inverse = 0;
        return 0;
    }

    long long a = A;
    long long m = M;

    // Kural: İki sayının aralarında asal olması (EBOB'un 1 olması) şarttır.
    // Eğer EBOB 1 değilse modüler tersi matematiksel olarak hesaplanamaz.
    if (gcd(a, m) != 1) {
        *has_inverse = 0; // Tersinin olmadığını pointer üzerinden main'e bildiriyoruz.
        return 0;
    }

    *has_inverse = 1; // Tersi var, hesaplamaya başla.
    
    // Genişletilmiş Öklid iterasyonu
    while (a > 1) {
        long long q = a / m;
        long long t = m;

        // Standart Öklid adımları
        m = a % m;
        a = t;
        t = y;

        // Katsayıları güncelliyoruz
        y = x - q * y;
        x = t;
    }

    // Algoritma sonucu x katsayısı negatif çıkabilir. 
    // C dilinde % operatörü negatif sonuç verebileceğinden pozitif modüler değere çeviriyoruz.
    if (x < 0) x = (x % m0 + m0) % m0;

    return x;
}

// 6.3. İkili Üs Alma (Binary Exponentiation - O(log b))
// Döngüyle tek tek çarpmak (örn: 2^100000) çok yavaş olacağı için üssü ikiye bölerek ilerliyoruz.
long long power_mod(long long base, long long exp, long long mod) {
    long long result = 1;
    
    // İşleme başlamadan önce tabanın modunu almak, sayının daha ilk adımda küçülmesini sağlar.
    base = base % mod;

    while (exp > 0) {
        // Eğer üs tek sayıysa (Örn: 2^5 = 2 * 2^4), mevcut sonucu tabanla bir kez çarpıp mod alıyoruz.
        // Mod alma işlemi (Taşma/Overflow Koruması): Çarpım sonucu long long sınırını aşmasın diye her adımda yapıyoruz.
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        
        // Üssü 2'ye böl (Bitwise right shift '>> 1', bölme işleminden daha hızlı çalışır).
        exp = exp >> 1; 
        
        // Üssü ikiye böldüğümüz için tabanın karesini alıyoruz (Örn: 2^4 = 4^2).
        base = (base * base) % mod;
    }
    return result;
}

// 6.4. Asallık Testi (O(sqrt(N)) Optimizasyonlu)
int is_prime(long long n) {
    if (n <= 1) return -1; // -1 dönmesi "Geçersiz Girdi (ERROR_INVALID_INPUT)" anlamına gelir.
    if (n == 2 || n == 3) return 1; // 2 ve 3 asaldır.
    
    // Çift sayıları ve 3'ün katlarını baştan eliyoruz ki döngü 6 kat hızlansın.
    if (n % 2 == 0 || n % 3 == 0) return 0;

    // Bütün asal sayılar (2 ve 3 hariç) 6k-1 veya 6k+1 formundadır.
    // Bu yüzden i'yi 5'ten başlatıp 6'şar 6'şar artırıyoruz.
    // Döngü sayının kareköküne kadar çalışır (i * i <= n matematiksel olarak karekök demektir).
    for (long long i = 5; i * i <= n; i += 6) {
        // i (6k-1) ve i+2 (6k+1) değerlerine bölünüyor mu kontrol et.
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1; // Hiçbir sayıya bölünmediyse asaldır.
}

// 6.5. Euler Totient Fonksiyonu (O(sqrt(N)))
// Mantık: Bir n sayısından küçük ve n ile aralarında asal olan sayıların adedini bulur.
long long euler_totient(long long n) {
    if (n < 1) return -1; // Invalid input durumu
    
    long long result = n;
    
    // Asal çarpanlara ayırma yöntemi (Kareköküne kadar)
    for (long long p = 2; p * p <= n; p++) {
        // p, n'in asal bir çarpanı mı?
        if (n % p == 0) {
            // p'ye bölünebildiği kadar bölüp sayıyı küçültüyoruz.
            while (n % p == 0) n /= p;
            
            // Matematiksel formül: result *= (1 - 1/p)
            // Ancak float kullanırsak precision (hassasiyet) kaybı olur.
            // Bu yüzden "result - (result / p)" şeklinde sadece tam sayı bölmesi kullanıyoruz.
            result = result - (result / p); 
        }
    }
    
    // Eğer geriye asal bir sayı kaldıysa (Karekökünden büyük bir asal çarpanı varsa)
    if (n > 1) {
        result = result - (result / n);
    }
    
    return result;
}