//bilet 16 Используя поля бит, найти остаток от деления на 2 и 4

//В 1 бит мы можем записать только 1 или 0, ост при делении на 2
//в два бита мы можем записать 0 1 2 3 - ост при делении на 4

struct pole {
	unsigned int four : 2;
	unsigned int two : 1;
	int num;
};

int main() {
	printf("%d", sizeof(pole));
	pole *a;
	a = (pole*)malloc(sizeof(pole));
	scanf_s("%d", &a->num);
	//присваиваются только первые биты
	a->four = a->num;
	a->two = a->num;
	printf("four: %d\ntwo: %d\n", a->four, a->two);
}