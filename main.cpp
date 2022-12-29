#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

// Dies ist die Funktion, die wir zur Laufzeit duplizieren möchten
int original_function(int x, int y) {
  int result = 0;
  for (int i = 0; i < x; i++) {
    result += y;
  }
  return result;
}


int main() {
  // Öffne einen Dateideskriptor für die ausführbare Datei
  int fd = open("/proc/self/exe", O_RDONLY);
  if (fd < 0) {
    perror("Fehler beim Öffnen der ausführbaren Datei");
    return 1;
  }
  // Ermittle die Größe der originalen Funktion
  size_t size = (char*)original_function - (char*)main;
  
   // Mappe den Maschinencode der originalen Funktion mithilfe von mmap in den Speicher
  void* original_code = mmap(NULL, size, PROT_READ | PROT_EXEC, MAP_PRIVATE, fd, (char*)original_function - (char*)main);
  if (original_code == MAP_FAILED) {
    perror("Fehler beim Mappen der originalen Funktion");
    return 1;
  }
  
  // Alloziere Speicher für die Duplikatfunktion mithilfe von malloc
  void* duplicate = malloc(size);
  
   // Kopiere den Maschinencode der originalen Funktion in die Duplikatfunktion
  memcpy(duplicate, original_code, size);
  
  // Caste die Duplikatfunktion zu einem Funktionszeiger und rufe sie auf
  int (*duplicate_function)(int, int) = (int (*)(int, int))duplicate;
  printf("Result: %d\n", duplicate_function(5, 7));
  
  // Unmappe den originalen Code und gib den allozierten Speicher frei
  munmap(original_code, size);
  free(duplicate);

  return 0;
}
