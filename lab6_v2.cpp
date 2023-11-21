#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <limits.h>
#include <math.h>

int height(struct Treenode* root);
int getcol(int h);

struct Treenode {
    int data;
    struct Treenode* left;
    struct Treenode* right;
};

struct Tree {
    struct Treenode* root;
};

struct Treenode* createNode(int data) {
    struct Treenode* newNode = (struct Treenode*)malloc(sizeof(struct Treenode));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

struct Treenode* minValueNode(struct Treenode* node) {
    struct Treenode* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

int isBSTUtil(struct Treenode* root, int* prev) {
    // Base case: an empty tree is a BST
    if (root == NULL) {
        return 1;
    }

    // Check the left subtree
    if (!isBSTUtil(root->left, prev)) {
        return 0;
    }

    // Check the current node
    if (root->data <= *prev) {
        return 0;
    }

    // Update the previous value
    *prev = root->data;

    // Check the right subtree
    return isBSTUtil(root->right, prev);
}

int isBST(struct Treenode* root) {
    int prev = INT_MIN; // Initialize prev to the smallest possible integer
    return isBSTUtil(root, &prev);
}

struct Treenode* deleteNode(struct Treenode* root, int key) {
    if (root == NULL) {
        return root;
    }

    // Check if the tree is a BST
    if (!isBST(root)) {
        printf("The tree is not a Binary Search Tree (BST). Deleting a node may cause some issues.\n");
        return root;
    }

    // Recursive search for the node to be deleted
    if (key < root->data) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->data) {
        root->right = deleteNode(root->right, key);
    } else {
        // Node with only one child or no child
        if (root->left == NULL) {
            struct Treenode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Treenode* temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children: get the in-order successor (smallest in the right subtree)
        struct Treenode* temp = minValueNode(root->right);

        // Copy the in-order successor's data to this node
        root->data = temp->data;

        // Delete the in-order successor
        root->right = deleteNode(root->right, temp->data);
    }

    // After deletion, ensure that the resulting tree is still a BST
    if (!isBST(root)) {
        printf("After deletion, the tree is not a Binary Search Tree (BST). There might be issues with deleting the chosen node.\n");
        // You may choose to handle this situation further, e.g., by reverting the deletion.
    }

    return root;
}

int height(struct Treenode* root) {
    if (root == NULL)
        return 0;
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    return (leftHeight > rightHeight) ? (leftHeight + 1) : (rightHeight + 1);
}

int getcol(int h) {
    if (h == 1)
        return 1;
    return getcol(h - 1) + getcol(h - 1) + 1;
}

int isBalanced(struct Treenode* root) {
    if (root == NULL)
        return 1; // An empty tree is balanced

    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    if (abs(leftHeight - rightHeight) <= 1 && isBalanced(root->left) && isBalanced(root->right))
        return 1; // The tree is balanced

    return 0; // The tree is not balanced
}

unsigned int getLeafCount(struct Treenode* node) {
    if (node == NULL) {
        return 0;
    }
    if (node->left == NULL && node->right == NULL) {
        return 1;
    } else {
        return getLeafCount(node->left) + getLeafCount(node->right);
    }
}

void inorderTraversal(struct Treenode* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->data);
        inorderTraversal(root->right);
    }
}

void printTree(int **M, struct Treenode *root, int col, int row, int height) {
    if (root == NULL)
        return;
    M[row][col] = root->data;
    printTree(M, root->left, col - pow(2, height - 2), row + 1, height - 1);
    printTree(M, root->right, col + pow(2, height - 2), row + 1, height - 1);
}

void TreePrinter(struct Tree tree) {
    int h = height(tree.root);
    int col = getcol(h);

    // Dynamically allocate memory for the 2D array
    int **M = (int **)malloc(h * sizeof(int *));
    for (int i = 0; i < h; i++) {
        M[i] = (int *)malloc(col * sizeof(int));
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < col; j++) {
            M[i][j] = 0;
        }
    }

    // Populate the array with tree data
    printTree(M, tree.root, col / 2, 0, h);

    // Print the array
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < col; j++) {
            if (M[i][j] == 0)
                printf("   ");
            else
                printf("%d ", M[i][j]);
        }
        printf("\n");
    }

    // Free the dynamically allocated memory
    for (int i = 0; i < h; i++) {
        free(M[i]);
    }
    free(M);
}

void buildTreeRecursive(struct Treenode* node) {
    int data;
    printf("Enter the left child of %d (0 = NULL): ", node->data);
    scanf("%d", &data);
    if (data != 0) {
        node->left = createNode(data);
        buildTreeRecursive(node->left);
    }
    printf("Enter the right child of %d (0 = NULL): ", node->data);
    scanf("%d", &data);
    if (data != 0) {
        node->right = createNode(data);
        buildTreeRecursive(node->right);
    }
}

void buildTree(struct Tree* tree) {
    int data;
    printf("Enter the root value: ");
    scanf("%d", &data);
    tree->root = createNode(data);
    buildTreeRecursive(tree->root);
}

void Interface(struct Tree* myTree) {
    int choice;
    int data;
    do {
        system("cls");
        printf("-----<Main Menu>-----\n");
        printf("1. Display Tree\n");
        printf("2. Count Leaves\n");
        printf("3. Inorder Traversal\n");
        printf("4. Height\n");
        printf("5. IsBalanced\n");
        printf("6. IsBST\n");
        printf("7. Delete a Node\n");
        printf("0. Exit\n");
        printf("---------------------\n\n\n");
        printf("Enter a choice: ");
        int inputResult;
        choice = getch();
        switch (choice) {
            case '1':{
                system("cls");
                TreePrinter(*myTree);
                printf("\nPress a key to return...");
                getch();
                system("cls");
                break;}

            case '2':{
            	system("cls");
                printf("Leaf count of the tree is %u\n", getLeafCount(myTree->root));
                getch();
                system("cls");
				break;}
				
            case '3':{
            	system("cls");
                printf("Inorder Traversal: ");
                inorderTraversal(myTree->root);
                printf("\n");
                getch();
                system("cls");
				break;}
				
			case '4':{
				system("cls");
	            printf("Height of the tree is %d\n", height(myTree->root));
	            getch();
	            system("cls");
				break;}
				
			case '5':{
                system("cls");
                if (isBalanced(myTree->root))
                    printf("The tree is balanced.\n");
                else
                    printf("The tree is not balanced.\n");
                getch();
                system("cls");
                break;}
             
			case '6':{
				system("cls");
                if (isBST(myTree->root))
                    printf("The tree is a Binary Search Tree (BST).\n");
                else
                    printf("The tree is not a Binary Search Tree (BST).\n");
                getch();
                system("cls");
				break;} 
				  
			case '7': {
		        system("cls");
		        int valueToDelete;
		        printf("Enter the value to delete: ");
		        scanf("%d", &valueToDelete);
		        myTree->root = deleteNode(myTree->root, valueToDelete);
		        printf("Node with value %d deleted successfully.\n", valueToDelete);
		        getch();
		        system("cls");
		        break;}
		    
            case '0':{
            	system("cls");
                printf("\033[0;32m");
                printf("Exiting... Goodbye!\n");
                printf("\033[0m");
				break;}
                
            case 27:{
            	system("cls");
                printf("\033[0;32m");
                printf("Exiting... Goodbye!\n");
                printf("\033[0m");
				break;}

            default:{
            	system("cls");
                printf("\033[0;31m");
                printf("Invalid choice. Please try again.\n");
                printf("\033[0m");
                system("pause");
                system("cls");
				break;}
			
        }
    } while (choice != '0' && choice != 27);
}


int main() {
    struct Tree myTree;
    buildTree(&myTree);
    Interface(&myTree);
    return 0;
}
