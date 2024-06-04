// otwieranie i usuwanie pliku zamkowego (lockfile) - eksperymenty
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
 
int main (int argc, char* argv[]){
  int lf;  // deskryptor pliku zamkowego
  char *kto=argv[1];  //napis identyfikujacy, przekazny z linii polecen

  printf("%s:  otwieranie pliku zamkowego\n",kto);
  //  while ((lf = open("lockfile", O_WRONLY|O_CREAT, 0))<0){
  while ((lf = open("lockfile", O_WRONLY|O_CREAT|O_EXCL, 0))<0){
    if (errno == EACCES) printf ("%s EACCESS\n",kto);
    if (errno == EEXIST) printf ("%s EEXISTS\n",kto);
    sleep(1);
  }
  printf("%s:  plik zamkowy utworzony\n",kto);
  sleep(2);
  unlink("lockfile");
  printf("%s:  plik zamkowy usunięty\n",kto);
}
