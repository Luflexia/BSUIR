#include "structures.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if(argc>1) {
        FILE *f = fopen(argv[1], "rb");                                             // Открыть файл.
        if(f==NULL) {
            printf("Error while open file.\n");
            exit(0);
        }

        index_hdr_s* data = (index_hdr_s*)malloc(sizeof(index_hdr_s));              
        if(!fread(&data->records, sizeof(uint64_t), 1, f)) {                        // Чтение шапки.
            printf("Error while reading records.\n");
            fclose(f);
            exit(0);
        }
        data->idx = (index_record*)malloc(data->records*sizeof(index_record));
        if(!fread(data->idx, sizeof(index_record), data->records, f)) {             // Чтение массива.
            printf("Error while reading idx.\n");
            fclose(f);
            exit(0);
        }

        //Выводим значение time_mark каждой записи
        for(int i = 0; i<data->records; i++)
            printf("%lf\n", data->idx[i].time_mark);

        fclose(f);
    }
    return 0;
}
