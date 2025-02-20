//bilet 16

//В 1 бит мы можем записать 1 или 0,
//и это все возможные варианты остатков при деление на 2
//а в два бита мы можем записать 0 1 2 3 и это уже все возможные варианты остатка от деления на 4
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