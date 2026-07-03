//bilet 1 Проход по дереву рекурсивно, вывод по возрастанию.

typedef struct TreeNode {
	void* data;
	TreeNode* left;	
	TreeNode* right;
}TreeNode

void treeTravel(TreeNode* root, void (*function)(void*)) {
	if (root) {
		treeTravel(root->left, function);
		printData(root->data);	
		treeTravel(root->right, function); 
	}
}

void printData(void* data) {
	printf("%d ", *((int*)data));
}