//2014510028 Çağrı Anıl Erbey

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




int main(){
   struct struct_example 

{ 

    int number; 

    float decimal; 

    char name[20]; 

};

typedef struct struct_example myStruct;

FILE *fp;

wItem.number = 3;

wItem.decimal = 3.45;

strcpy(wItem.name, "ayşe");

  fwrite(&myStruct.number, sizeof(int), 1, fp)
fwrite(&myStruct.decimal, sizeof(float), 1, fp)

fwrite(&myStruct.name, sizeof(char), 1, fp)

  fprintf(fp, "%d %f %c", myStruct.number, myStruct.decimal, myStruct.name)
  fprintf(fp, "%d %f %s", myStruct.number, myStruct.decimal, myStruct.name)
  printf("%d %f %c", myStruct.number, myStruct.decimal, myStruct.name)
  fwrite(&myStruct, sizeof(myStruct), 1, fp)
}