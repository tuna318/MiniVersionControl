#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../helper/file_handler.h"


int main( int argc, char *argv[] )
{
  FILE *fi;
  if( (fi = fopen("test-a", "a+")) == NULL){
    exit(0);
  }
  fprintf(fi, "hello\n");
  return 1;
}