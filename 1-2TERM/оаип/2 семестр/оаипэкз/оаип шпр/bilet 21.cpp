//Билет 21 Даны массивы структур из двух элементов Найти студента с наибольшим баллом.

struct inf {
	char name[30];
	int group;

};
struct mark {
	int mark;
};
struct student {
	union {
		inf a;
		mark b;
	}un;
	
};

int main() {
	student** mas;
	int n;
	char name[30];

	mas = (student**)calloc(n, sizeof(student*));
      if (!mas) {
		   exit(error);
	   }

	for (int i = 0; i < n; i++) {
		mas[i] = (student*)calloc(2, sizeof(student));
	}

	int max = 0, maxi;     

	for (int i = 0; i < n; i++) {
		if (mas[i][1].un.b.mark > max) {
			max = mas[i][1].un.b.mark;
			maxi = i;
		}
	}

	printf("\nСтудент с наивысшей отметкой\n");   
	printf("Имя %s\n", mas[maxi][0].un.a.name);
	printf("Группа %d\n", mas[maxi][0].un.a.group);
	printf("Отметка %d\n", mas[maxi][1].un.b.mark);

	for (int i = 0; i < n; i++) {
		free(mas[i]);
	}
	return 0;
}