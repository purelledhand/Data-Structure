#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct element {
    char name[10];
    char register_date[9];
    char register_region[20];
} element;

typedef struct node *treePointer;

typedef struct node {
    element data;
    treePointer leftChild;
    treePointer rightChild;
} node;

/**
 * findPosition(root, new_node) : 트리의 root노드부터 시작하여 new_node가 들어갈 위치의 부모 노드를 반환합니다.
*/
treePointer findPosition(treePointer tree, element new_node) {
    if (tree == NULL) return NULL;
    if (!strcmp(new_node.name, tree->data.name))
        printf("ERROR : [%s] is already exist!\n", new_node.name);

    if (strcmp(new_node.name, tree->data.name) < 0) {
        if (tree->leftChild) {
            return findPosition(tree->leftChild, new_node);
        } else {
            return tree;
        }
    } else {
        if (tree->rightChild) {
            return findPosition(tree->rightChild, new_node);
        } else {
            return tree;
        }
    }
}

/**
 * findParent(root, name) : name을 가지고 있는 노드의 부모 노드를 반환합니다.
 */
treePointer findParent(treePointer tree, char *name) {
    if (tree == NULL) return NULL;
    if (!strcmp(name, tree->data.name)) return NULL;

    if (strcmp(name, tree->data.name) < 0) {
        if (!strcmp(tree->leftChild->data.name, name)) {
            return tree;
        } else {
            return findParent(tree->leftChild, name);
        }
    } else {
        if (!strcmp(tree->rightChild->data.name, name)) {
            return tree;
        } else {
            return findParent(tree->rightChild, name);
        }
    }
}

/**
 * search_node(root, name) : BST의 'root'노드를 기준으로 name을 탐색합니다.
 * 탐색한 노드의 element 주소를 반환합니다.
 */
element *search_node(treePointer root, char *name) {
    treePointer iterator_node;
    iterator_node = root;

    while (iterator_node) {
        if (!strcmp(name, iterator_node->data.name)) {
            return &(iterator_node->data);
        }
        if (strcmp(name, iterator_node->data.name) < 0) iterator_node = iterator_node->leftChild;
        else iterator_node = iterator_node->rightChild;
    }

    printf("\n[*] [%s] DATA DOESN'T EXIST.\n", name);
    return NULL;
}

/**
 * delete_node(root, name) : 'root'를 루트노드로 하는 트리에서 name을 데이터로 가지는 노드를 삭제합니다.
 * 1. 삭제할 노드가 리프노드인 경우 : 부모노드의 treePointer를 NULL로 변경합니다.
 * 2. 삭제할 노드가 1개의 자식노드를 가지는 경우 : 부모노드의 treePointer를 자식노드로 연결합니다.
 * 3. 삭제할 노드가 2개의 자식노드를 가지는 경우 : left subtree의 inorder predecessor를 루트 노드에 배치합니다.
 */
void deleteNode(treePointer *root, char *name) {
    treePointer *parent;
    treePointer temp;
    treePointer *predecessor;
    treePointer *predecessor_parent;
    treePointer temp2;
    treePointer target_node;
    treePointer iter;
    int is_left = 0;
    int is_right = 0;
    int delete_root = 0;
    static treePointer temp_parent;
    if (!root) {
        printf("BST에 아무 노드도 존재하지 않습니다.\n");
        return;
    }

    temp = findParent(*root, name);
    parent = &temp;

    if (!*parent) {
        delete_root = 1;
    }

    if (!delete_root) {
        if (!strcmp((*parent)->leftChild->data.name, name)) {
            target_node = (*parent)->leftChild;
            is_left = 1;
        } else {
            target_node = (*parent)->rightChild;
            is_right = 1;
        }
    } else {
        target_node = *root;
    }

    /**
     * leftChild, rightChild는 node struct의 포인터입니다.
     * 부모 노드의 leftChild, rightChild를 변경하는 것은 포인터를 변경하는 것이므로,
     * 이중포인터를 이용하여 접근해야 합니다.
     */

    // 1. 삭제할 노드가 리프노드인 경우
    if (!target_node->leftChild && !target_node->rightChild) {
        if (delete_root) {
            *root = NULL;
        } else if (is_left) {
            (*parent)->leftChild = NULL;
            free(target_node);
        } else {
            (*parent)->rightChild = NULL;
            free(target_node);
        }

        printf("[*] 삭제가 완료되었습니다.\n");
        return;
    }

    // 2. 삭제할 노드가 한 개의 자식 노드를 가진 경우
    if (target_node->leftChild && !target_node->rightChild) {
        if (delete_root) {
            *root = (*root)->leftChild;
        } else if (is_left) {
            (*parent)->leftChild = target_node->leftChild;
        } else {
            (*parent)->rightChild = target_node->leftChild;
        }

        printf("[*] 삭제가 완료되었습니다.\n");
        return;
    } else if (!target_node->leftChild && target_node->rightChild) {
        if (delete_root) {
            *root = (*root)->rightChild;
        } else if (is_left) {
            (*parent)->leftChild = target_node->rightChild;
        } else {
            (*parent)->rightChild = target_node->rightChild;
        }

        printf("[*] 삭제가 완료되었습니다.\n");
        return;
    }

    /**
     * 3. 삭제할 노드가 두 개의 자식 노드를 가진 경우
     * 1) Inorder predecessor를 구합니다.
     * 2) Inorder predecessor를 현재 위치에서 삭제합니다.
     * 3) Inorder predecessor를 target node의 위치로 옮깁니다.
     */

    /**
     * *predecessor = iter로 하면 안되는 이유
     * 이 시점은 predecessor 자체에 값이 초기화 되지 않은 상태입니다.
     * 이 때 값이 초기화 되지 않은 상태에서 *연산을 할 수 없기 때문에
     * 접근할 수 없는 부분에 접근함으로써 발생하는 segment fault를 발생 시킵니다.
    */

    iter = target_node->leftChild;
    while (iter->rightChild) {
        iter = iter->rightChild;
    }

    predecessor = &iter;

    temp2 = findParent(*root, (*predecessor)->data.name);
    *predecessor_parent = temp2;

    /**
     * inorder predecessor를 루트로 올리기 전, 해당 노드를 삭제하는 작업을 합니다.
     * inorder predecessor는 무조건 부모노드의 right child이고,
     * inorder predecessor는 리프노드이거나, 1개의 자식 노드만 가지고 있기 때문에
     * 위 두가지 경우에 대해서 노드를 삭제 하는 작업을 수행합니다.
     */

    if ((*predecessor)->leftChild) {
        if ((*predecessor) == target_node->leftChild) (*predecessor_parent)->leftChild = (*predecessor)->leftChild;
        else (*predecessor_parent)->rightChild = (*predecessor)->leftChild;
    } else {
        if ((*predecessor) == target_node->leftChild) (*predecessor_parent)->leftChild = NULL;
        else (*predecessor_parent)->rightChild = NULL;
    }

    /**
     * inorder predecessor를 서브트리의 루트로 올리는 작업(target node를 inorder predecessor로 교체)을 합니다.
     * target node의 leftChild, rightChild를 inorder predecessor의 자식 노드로 설정합니다.
     */

    (*predecessor)->leftChild = target_node->leftChild;
    (*predecessor)->rightChild = target_node->rightChild;


    /**
     * target node의 부모 노드가 inorder predecesor를 가리키도록 합니다.
     */
    if (delete_root) {
        *root = *predecessor;
    } else if (is_left) {
        (*parent)->leftChild = *predecessor;
    } else {
        (*parent)->rightChild = *predecessor;
    }

    printf("[*] 삭제가 완료되었습니다.\n");
}

/**
 * modify_record(root, name, info) : 'root'노드를 루트노드로 하는 트리에서 name 노드의 정보를 info로 변경합니다.
 * 이 때 info가 참가날짜인지, 참가장소인지를 판별하여 올바른 레코드를 변경합니다.
 */
void modify_record(treePointer root, char *name, char *info) {
    element *target_node;
    char first_char = info[0];
    int modify_date = 0;
    int modify_region = 0;

    target_node = search_node(root, name);

    // info의 타입을 판별합니다.
    if (!atoi(&first_char)) modify_region = 1;
    else modify_date = 1;

    // record를 변경합니다.
    if (modify_date) {
        printf("[*] Before modify data : %s\n", target_node->register_date);
        strcpy(target_node->register_date, info);
        printf("[*] After modify data : %s\n", target_node->register_date);
        printf("변경이 완료되었습니다.\n");
    } else if (modify_region) {
        printf("[*] Before modify data : %s\n", target_node->register_region);
        strcpy(target_node->register_region, info);
        printf("[*] After modify data : %s\n", target_node->register_region);
        printf("변경이 완료되었습니다.\n");
    }
}

/**
 * insert_node(root, new_node) : root가 root_node인 BST에 new_node를 삽입합니다.
 */
treePointer insert_node(treePointer root, element new_node) {

    treePointer inserting_node;
    treePointer parent = findPosition(root, new_node);

    if (!parent && root) return root;

    inserting_node = (treePointer) calloc(1, sizeof(node));
    strcpy(inserting_node->data.name, new_node.name);
    strcpy(inserting_node->data.register_date, new_node.register_date);
    strcpy(inserting_node->data.register_region, new_node.register_region);
    inserting_node->leftChild = inserting_node->rightChild = NULL;

    if (root) {
        if (strcmp(new_node.name, parent->data.name) < 0) {
            parent->leftChild = inserting_node;
            printf("[*] %s is inserted as the leftChild of %s\n", inserting_node->data.name, parent->data.name);
        } else {
            parent->rightChild = inserting_node;
            printf("[*] %s is inserted as the rightChild of %s\n", inserting_node->data.name, parent->data.name);
        }
    } else {
        root = inserting_node;
        printf("[*] The first node\n");
    }

    return root;
}

/**
 * print_node(root) : root 노드를 기준으로 BST의 값을 Inorder Traverse로 출력합니다.\
 */

void print_node(treePointer root) {
    if (!root) return;
    print_node(root->leftChild);
    printf("%s\n", root->data.name);
    print_node(root->rightChild);
}

int main(void) {
    int input;
    element new_node;
    element *result;
    char search_name[10];
    char modify_info[20];
    treePointer root = NULL;
    printf("1. REGISTER | 2. SEARCH | 3. MODIFY | 4. CANCEL | 5. PRINT | 6. EXIT\n");
    scanf("%d", &input);

    while (input != 6) {
        switch (input) {
            case 1:
                printf("###REGISTER###\n");
                printf("name : ");
                scanf("%s", new_node.name);
                printf("registered date : ");
                scanf("%s", new_node.register_date);
                printf("registered region : ");
                scanf("%s", new_node.register_region);

                root = insert_node(root, new_node);
                break;
            case 2:
                printf("###SEARCH###\n");
                printf("name for search : ");
                scanf("%s", search_name);
                result = search_node(root, search_name);
                if (result) {
                    printf("\n[*] SEARCH RESULT\n");
                    printf("name : %s\nregistered date : %s\nregistered region : %s\n", result->name,
                           result->register_date,
                           result->register_region);
                }
                break;
            case 3:
                printf("###MODIFY###\n");
                printf("name for modify : ");
                scanf("%s", search_name);
                if (!search_node(root, search_name)) {
                    printf("[*] CANNOT MODIFY THE RECORD : Doesn't existing record.\n");
                    break;
                }
                printf("info for modify : ");
                scanf("%s", modify_info);
                modify_record(root, search_name, modify_info);
                break;
            case 4:
                printf("###DELETE###\n");
                printf("name for delete : ");
                scanf("%s", search_name);
                deleteNode(&root, search_name);
                break;
            case 5:
                printf("###PRINT GRAPH###\n");
                print_node(root);
                break;
            default:
                printf("Invalid input.\n");
                break;
        }

        printf("\n1. REGISTER | 2. SEARCH | 3. MODIFY | 4. CANCEL | 5.PRINT | 6. EXIT\n");
        scanf("%d", &input);
    }

    printf("[*] PROGRAM TERMINATING\n");
    return 0;

}