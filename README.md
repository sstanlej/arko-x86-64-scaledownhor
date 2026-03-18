# Horizontal Scale Down (x86-64)

Projekt realizuje operację poziomego skalowania w dół obrazów w formacie **BMP (24-bit)** o zadany współczynnik. Nowe wartości pikseli są wyznaczane jako maksimum z wartości składowych (RGB) pikseli źródłowych objętych skalowaniem.

## Struktura projektu
* `main.c`: Główny moduł w języku C odpowiedzialny za argumenty wiersza poleceń, operacje wejścia/wyjścia (I/O) na plikach BMP oraz przygotowanie struktur danych.
* `scaledownhor.asm`: Moduł asemblerowy x86-64 realizujący algorytm przetwarzania tablicy pikseli.



## Kompilacja i uruchomienie

Program należy skompilować i połączyć za pomocą następujących poleceń:

```bash
# Asemblacja modułu x86-64
nasm -f elf64 scaledownhor.asm -o scaledownhor.o

# Kompilacja modułu C i linkowanie
gcc -no-pie main.c scaledownhor.o -o scaledownhor

# Uruchomienie programu
./scaledownhor <input_file.bmp> <output_file.bmp> <scale>
