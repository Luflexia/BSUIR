//Проход по дереву рекурсивно, вывод по возрастанию.


// структура узла дерева
typedef struct TreeNode {
	void* data;				// данные в void, чтобы можно было юзать любой тип
	TreeNode* left;			// левый наследник
	TreeNode* right;		// правый наследник
}TreeNode;

// рекурсивный проход по дереву
// передаем указатель на корень, и функцию которую будем применять во время обхода

void treeTravel(TreeNode* root, void (*function)(void*)) {
	if (root) {
		treeTravel(root->left, function);	// сначала левое поддерево
		function(root->data);				// выводим данные
		treeTravel(root->right, function);  // потом правое поддерево
	}
}