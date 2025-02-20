//Билет 20 Dля k-ого элемента: если левое поддерево, то 2k правое, то 2k + 1

void add(int** derevo, int& numeretic, bool** set) {  

    int enter_number;
    while (!scanf("%d", &enter_number) {
		   printf("error\n");
		   rewind(stdin);
	   }

    int k = 1;
    while (1) {

        if (k > numeretic) {
            numeretic = k;
            (*derevo) = (int*)realloc((*derevo), (numeretic + 1) * sizeof(int));  

            (*set) = (bool*)realloc((*set), (numeretic + 1) * sizeof(bool));
            continue;
        }

        if ((*derevo)[k] == enter_number) {
            printf("number already exist!\n");
            system("pause");
            return;
        }

        if ((*set)[k]) {
            (*derevo)[k] = enter_number;
            (*set)[k] = false;
            break;
        }
        else if (enter_number < (*derevo)[k]) {
            k *= 2;
            continue;
        }
        else {
            k = k * 2 + 1;
            continue;
        }
    }
}

int main() {
    int* derevo, numeretic = 1;
    bool* set;

    derevo = (int*)malloc((numeretic + 2) * sizeof(int));
	if (!derevo) {
		exit(error);
	}

    set = (bool*)malloc((numeretic + 2) * sizeof(bool));
        if (!set) {
		exit(error);
	}
        add(&derevo, numeretic, &set);
       return 0;
}