#include <iostream>
#include "LabTree.h"
#include "source.h"

using namespace std;

int main() {

	node* root = NULL;
	node_data z;
	/*add_tree(&root, 9, NULL);
	add_tree(&root, 16, NULL);
	add_tree(&root, 6, NULL);
	add_tree(&root, 90, NULL);
	add_tree(&root, 70, NULL);*/

	init_from_file(&root, "tree.txt");

	cout << find_max(root)->item << endl;

	//del_element(root, 9);

	node* t = find_node(root, 9);

	if (t != NULL)
		cout << t->item << endl;


	cout << "\n\n__Preorder:___\n";
	preorder(root);

	cout << "\n\n__Inorder:___\n";
	inorder(root);

	cout << "\n\n__Postorder:___\n";
	postorder(root);

	return 0;
}

