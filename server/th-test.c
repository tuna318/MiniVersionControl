#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../helper/file_handler.h"


int main( int argc, char *argv[] )
{
  FilePathInfo* path_info = NULL;
  int i;
  get_folder_structure("/home/tuna/Programming/Hedspi/Ki_7/lap_trinh_mang/MiniVersionControl", 
                       "/home/tuna/Programming/Hedspi/Ki_7/lap_trinh_mang/MiniVersionControl",
                       &path_info);
  FilePathInfo* temp;
  temp = path_info;
  while (temp != NULL) {
    printf("absolute: %s\n", temp->absolute_path);
    printf("relative: %s\n", temp->relative_folder);
    printf("filename: %s\n", temp->file_name);

    temp = temp->next;
  }
}