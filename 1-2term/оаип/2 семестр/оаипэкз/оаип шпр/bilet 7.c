//bilet 7 сортировка двунаправленного кольца методом замены указателей

typedef struct Node {
	int data;
	Node* next;
	Node* prev;
}Node;

typedef struct Ring {
	Node* current;
}Ring;


Ring* createRing() {
	Ring* ring = (Ring*)malloc(sizeof(Ring));
	if (!ring) {
		exit(error);
	}
	ring->current = NULL;
	return ring;
}

int compare_greater(int first_value, int second_value) {
	return first_value > second_value;
}

int compare_less(int first_value, int second_value) {
	return first_value < second_value;
}

Ring* sort(Ring* ring, int (*comparison_function)(int, int)) {
	Node* main_pointer = NULL,			
		* current_pointer = NULL,		
		* pointer_to_swap = NULL;		
	main_pointer = ring->current;
	do
	{
		current_pointer = main_pointer->next;                          
		pointer_to_swap = main_pointer;                            
		
		do {	// проходим по списку
			if (comparison_function(pointer_to_swap->data, current_pointer->data)) {
				pointer_to_swap = current_pointer;										
			}
			current_pointer = current_pointer->next;	
		} while (current_pointer != ring->current);                   
		
		if (pointer_to_swap != main_pointer) {
			
			if (ring->current == main_pointer) {
				ring->current = pointer_to_swap;
			}
			// и делаем свап указателей
			pointer_to_swap->prev->next = pointer_to_swap->next;
			pointer_to_swap->next->prev = pointer_to_swap->prev; 
			main_pointer->prev->next = pointer_to_swap;         
			pointer_to_swap->next = main_pointer;          
			pointer_to_swap->prev = main_pointer->prev;      
			main_pointer->prev = pointer_to_swap;            
		}
		else {
			main_pointer = main_pointer->next;
		}
	} while (main_pointer->next != ring->current);
	
	return ring;
}

int main() {
	Ring* ring = createRing();
	ring = sort(ring, compare_greater);
	ring = sort(ring, compare_less);
}
