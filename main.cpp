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

}
