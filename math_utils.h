#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// Matematiksel algoritmalarımızın prototipleri.
// Burada int* has_inverse kullanmamızın sebebi: Fonksiyon hem sonucun sayısını döndürmeli
// hem de tersinin var olup olmadığını (hata durumunu) bildirebilmeli. Pointer üzerinden çift veri döndürüyoruz.
long long gcd(long long a, long long b);
long long mod_inverse(long long A, long long M, int* has_inverse);
long long power_mod(long long base, long long exp, long long mod);
int is_prime(long long n);
long long euler_totient(long long n);

#endif