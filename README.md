## usage:
    nasm -f elf64 scaledownhor.asm -o scaledownhor.o
    gcc -no-pie main.c scaledownhor.o -o scaledownhor
    ./scaledownhor <input_file.bmp> <output_file.bmp> <scale>
## project - x86-64
Napisz program składający się z dwóch plików źródłowych: głównej funkcji programu
napisanej w C i modułu w asemblerze wywoływanego z programu w C.
Deklaracja procedury asemblerowej w C jest podana dla każdego projektu. Użyj
programu NASM (nasm.sf.net) do asemblacji modułu asemblerowego. Użyj
kompilatora C aby skompilować moduł w C oraz połączyć moduły w program.
Program w C powienien wczytywać argumenty wywołania procedury z argumentów
wywołania programu i wykonać wszystkie operacje I/O. Żadne funkcje
systemowe ani funkcje z bibliotek C nie powinny być wywoływane z procedury w
asemblerze. Argumenty do operacji na bitach powinny zostać podane w postaci
liczb szesnastkowych.
Procedury przetwarzające pliki .BMP mogą przyjmować jako argument albo
wskaźnik do całej struktury .BMP albo do zawartej w niej tablicy pikseli. Ekstrakcja
podstawowych parametrów obrazu może się odbywać albo w module
napisanym w C albo w procedurze napisanej w asemblerze. Procedury powinny
obsługiwać obrazy dowolnych rozmiarów, o ile nie określono w poleceniu inaczej.
Moduł napisanym w C albo w procedura napisana w asemblerze powinna
obliczać stride, czyli rozmiar wiersza pikseli zaokrąglony w górę do pełnych 4
bajtów. Lista argumentów przekazywana do procedury w asemblerze może być
w ramach potrzeb zmodyfikowana. Wymiary obrazu mogą być przekazywane
do procedury w asemblerze jako argumenty. Proszę zwrócić uwagę na to, że
program Paint w Windowsach 10 i 11 nie przetwarza poprawnie 1-bitowych
plików .BMP. Do przygotowania takich plikow konieczne jest wykorzystanie innego
narzędzia, na przykład gimp.
Programy powinny zostać przygotowane w dwóch wersjach, 32- i 64-bitowych,
zgodnie z konwencjami wołania Unix. Opisy obu konwencji są również opisane
na stronie www.agner.org. Podczas konwersji z wersji x86 do x86-64 należy
postarać się przenieść, w miarę możliwości, zmienne z pamięci do dodatkowych
rejestrów. Maksymalna liczba punktów za (jakąkolwiek) jedną wersję to 6 punktów.
Druga wersja warta jest 2 punkty. Program nie powinien zawierać żadnych
wyraźnie niewydajnych fragmentów, np. dzielenia i mnożenia przez stałe
będące potęgą dwójki. Unikaj sekwencji bezpośrednich rozejść, zwłaszcza rozejść
warunkowych bezpośrednio po których występuje bezwarunkowe (o ile nie
jest to konieczne).
Każda próba oddania projektu niezgodnego z konwencją wołanai będzie
skutkowała obniżeniem uzyskanej liczby punktów o 1.

## task
void scaledownhor(void *img, void *new_img, uint32_t width,
uint32_t height, uint32_t scale)
zeskaluj w dół poziomo 24-bitowy obraz .BMP o zadany czynnik. Określ
wartości nowych pikseli jako maksimum wartości starych.