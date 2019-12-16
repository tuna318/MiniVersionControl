#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../helper/file_handler.h"


int main( int argc, char *argv[] )
{
  char *db;
  db = get_db_location();
  printf("%s\n", db);
}