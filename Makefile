# CC değişkeni: Hangi derleyiciyi kullanacağımızı sisteme söyleriz. Standart C derleyicisi gcc'dir ama başka bir cihazda başka bir
# cihazda başka bir derleyici kullanılıyor olabilir.Satırlarda tek gcc yi değiştirmek yerine sadece CC yi guncelleyerek işimizi kolaylaştırırız.
CC = gcc

# CFLAGS değişkeni: Derleyiciye vereceğimiz ekstra komutlar (bayraklar).
# -Wall ve -Wextra: Kodumuzdaki olası mantık hatalarını veya kullanılmayan değişkenleri bize uyarı olarak gösterir.
# -O2: Kodun çalışma hızını optimize eder (Performans testleri için faydalıdır).
CFLAGS = -Wall -Wextra -O2

# Terminale sadece "make" yazıldığında çalışacak varsayılan (ana) hedef.
all: numtool

#all yazınca aşağıdaki komutların sırası artık önemsiz oluyor.
#birden fazla program oluşturulmak için yine all kullanılır.

# numtool (çalıştırılabilir dosyamız) hedefi.
# Bu dosyanın oluşması için main.o, parser.o ve math_utils.o dosyalarına ihtiyacımız var.
# Alt satırdaki komut bu .o (object) dosyalarını birleştirip (link) nihai programı üretir.
#object yani nesne dosyaları kodlarınn makine diline çevrilmiş halleridir.Bunları birleştiriyoruz.
numtool: main.o parser.o math_utils.o
	$(CC) $(CFLAGS) -o numtool main.o parser.o math_utils.o

# main.c'nin derlenme kuralı. -c bayrağı "Sadece derle, bağlama (link) yapma, .o dosyası üret" demektir.
main.o: main.c parser.h math_utils.h
	$(CC) $(CFLAGS) -c main.c

# parser.c'nin derlenme kuralı.
parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

# math_utils.c'nin derlenme kuralı.
math_utils.o: math_utils.c math_utils.h
	$(CC) $(CFLAGS) -c math_utils.c

# "make clean" yazıldığında projeyi temizlemek için kullanılacak kural.
# Üretilen geçici .o dosyalarını ve exe/program dosyasını siler.Mesela paylasım sırasına Veri ve bellek tasarrufu için kullanılabilir.
clean:
	rm -f *.o numtool numtool.exe