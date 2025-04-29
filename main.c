#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_LINE 128
#define MAX_ID 1000


typedef struct User {

    int id;
    struct User *friends[MAX_USERS];
    int friend_count;
}User;

//-----------------RED BLACK TREE---------------------//
typedef enum { RED, BLACK } Color;

typedef struct RBNode {

    int id;
    Color color;
    struct RBNode *left, *right, *parent;
}RBNode;

typedef struct RedBlackTree {

    RBNode *root;
}RedBlackTree;

//-----------------------------------------------------//

User *users[MAX_USERS];
int user_count;
int visited[MAX_ID];
int ziyaret_edildi[MAX_USERS] = {0};



User *findOrCreateUser(int id) {
    for (int i = 0; i < user_count; i++) {
        if (users[i] -> id == id) {
            return users[i];
        }
    }

    User *new_User = malloc(sizeof(User));
    new_User -> id = id;
    new_User -> friend_count = 0;
    users[user_count] = new_User;
    user_count++;
    return new_User;
}

//-----------------RED BLACK TREE---------------------//
RBNode *createNode(int id) {
    RBNode *node = malloc(sizeof(RBNode));

    node -> id = id;
    node -> color = RED;
    node -> left = NULL;
    node -> right = NULL;
    node -> parent = NULL;

    return node;
}

//-----------------------------------------------------//

void arkadaslikOlustur(int id1, int id2) {

    User *u1 = findOrCreateUser(id1);
    User *u2 = findOrCreateUser(id2);

    u1 -> friends[u1 -> /*friend_count++*/friend_count] = u2;
    u2 -> friends[u2 -> /*friend_count++*/friend_count] = u1;

    u1 -> friend_count++;
    u2 -> friend_count++;
}

void dosyaOku(const char *dosya_adi) {
    FILE *dosya = fopen(dosya_adi, "r");

    if (!dosya) {
        printf("Dosya acilamadi...");
        return;
    }

    else {
        char line[MAX_LINE];

        while (fgets(line, sizeof(line), dosya)) {
            if (strncmp(line, "USER", 4) == 0) {

                int id;
                sscanf(line, "USER %d", &id);
                findOrCreateUser(id);
            }

            else if (strncmp(line, "FRIEND", 6) == 0) {
                int id1, id2;
                sscanf(line, "FRIEND %d %d", &id1, &id2);
                arkadaslikOlustur(id1, id2);
            }
        }
    }
    fclose(dosya);
}

void dfs(User *user, int mevcut_derinlik, int hedef_derinlik) {

    if (mevcut_derinlik > hedef_derinlik) {
        return;
    }

    visited[user -> id] = 1;

    if (mevcut_derinlik == hedef_derinlik) {
        printf("Mesafe %d' de bulunan arkadas: %d\n", hedef_derinlik, user -> id);
        return;
    }


    for (int i = 0; i < user -> friend_count; i++) {
        if (!visited[user -> friends[i] -> id]) {
            dfs(user -> friends[i], mevcut_derinlik + 1, hedef_derinlik);
        }
    }
}

void mesafeGoreArkadasBul(int user_id, int distance) {

    memset(visited, 0, sizeof(visited));
    User *user = findOrCreateUser(user_id);

    printf("User %d 'nin %d adim uzagindaki arkadaslar:\n", user_id, distance);
    dfs(user, 0, distance);
}

User *kullaniciBul(int id) {    //asaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

    for (int i = 0; i < user_count; i++) {
        if (users[i] -> id == id) {
            return users[i];
        }
    }
    return NULL;
}

void ortakArkadasBul(int user1, int user2) {

    User *userA = findOrCreateUser(user1);
    User *userB = findOrCreateUser(user2);

    printf("Hem %d hemde %d ile arkadas olan kisiler: \n", userA -> id, userB -> id);
    int ortakVar = 0;

    for (int i = 0; i < userA -> friend_count; i++) {
        for (int j = 0; j < userB -> friend_count;  j++) {

            if (userA -> friends[i] == userB -> friends[j]) {
                printf("- User %d\n", userB -> friends[j] -> id);
                ortakVar = 1;
            }
        }
    }

    if (!ortakVar) {
        printf("%d ile %d nin ortak arkadasi bulunmamakta...", userA -> id, userB -> id);
    }
}




//----------------------RED BLACK TREE-----------------------\\

void leftRotate(RedBlackTree *tree, RBNode *x) {

    RBNode *y = x -> right;
    x -> right = y -> left;

    if (y -> left != NULL) {
        y -> left -> parent = x;
    }

    y -> parent = x -> parent;
    if (x -> parent == NULL) {
        tree -> root = y;
    }

    else if (x == x -> parent -> left) {
        x -> parent -> left = y;
    }

    else {
        x -> parent -> right = y;
    }

    y -> left = x;
    x -> parent = y;
}

void rightRotate(RedBlackTree *tree, RBNode *y) {

    RBNode *x = y -> left;
    y -> left = x -> right;

    if (x -> right != NULL) {
        x -> right -> parent = y;
    }

    x -> parent = y -> parent;
    if (y -> parent == NULL) {
        tree -> root = x;
    }

    else if (y == y -> parent -> left) {
        y -> parent -> left = x;
    }

    else {
        y -> parent -> right = x;
    }

    x -> right = y;
    y -> parent = x;
}

void fixInsert (RedBlackTree *tree, RBNode *node) {

    while (/*node != NULL && node -> parent -> color == RED*/node != NULL && node -> parent != NULL && node -> parent -> color == RED) {
        RBNode *grandParent = node -> parent -> parent;

        if (node -> parent == grandParent -> left) {
            RBNode *uncle = grandParent -> right;

            if (uncle != NULL  && uncle -> color == RED) {

                node -> parent -> color = BLACK;
                uncle -> color = BLACK;
                grandParent -> color = RED;
                node = grandParent;
            }

            else {
                // Case 2: Node is right child -> Left rotate
                if (node == node -> parent -> right) {
                    node = node -> parent;
                    leftRotate(tree, node);
                }

                // Case 3: Node is left child -> Right rotate
                node -> parent -> color = BLACK;
                grandParent -> color = RED;
                rightRotate(tree, grandParent);
            }
        }

        else {
            // Sağ taraf için simetrik durumlar
            RBNode *uncle = grandParent -> left;

            if (uncle != NULL && uncle -> color == RED) {

                node -> parent -> color = BLACK;
                uncle -> color = BLACK;
                grandParent -> color = RED;
                node = grandParent;
            }

            else {
                if (node == node -> parent -> left) {
                    node = node -> parent;
                    rightRotate(tree, node);
                }

                node -> parent -> color = BLACK;
                grandParent -> color = RED;
                leftRotate(tree, grandParent);
            }
        }
    }

    tree -> root -> color = BLACK;
}// fixInsert de uncle RED ise uncle BLACK yapilir, uncle BLACK ise child a göre rotate yapilir

void insert(RedBlackTree *tree, int id) {

    RBNode *newNode = createNode(id);
    RBNode *y = NULL;
    RBNode *x = tree -> root;

    while (x != NULL) {
        y = x;

        if (newNode -> id < x -> id) {
            x = x -> left;
        }

        else {
            x = x -> right;
        }
    }

    newNode -> parent = y;
    if (y == NULL) {
        tree -> root = newNode;
    }

    else if (newNode -> id < y -> id) {
        y -> left = newNode;
    }

    else {
        y -> right = newNode;
    }

    fixInsert (tree, newNode);
}

RBNode *search(RedBlackTree *tree, int id) {
    RBNode *current = tree -> root;

    while (current != NULL) {
        if (id == current -> id) {
            return current;
        }

        else if (id < current -> id) {
            current = current -> left;
        }

        else {
            current = current -> right;
        }
    }
    return NULL;
}

void innorderTraversal(RBNode *node) {

    if (node != NULL) {
        innorderTraversal(node -> left);
        printf("User ID: %d (%s)\n", node -> id, node -> color == RED ? "RED" : "BLACK");
        innorderTraversal(node -> right);
    }
}

int main(void) {

    dosyaOku("veriseti.txt");




    printf("\n*****************************************\n");

    printf("User sayisi: %d\n", user_count);

    printf("\n*****************************************\n");

    for (int i = 0; i < user_count; i++) {
        printf("-----------------\n");
        printf("User %d\n", users[i] -> id);
        for (int j = 0; j < users[i] -> friend_count; j++) {
            printf("%d.Arkadas: %d\n", j+1, users[i] -> friends[j] -> id);
        }
    }

    printf("\n*****************************************\n");

    printf("-------------------\n");
    mesafeGoreArkadasBul(users[0] -> id, 1);
    printf("-------------------\n");

    printf("\n*****************************************\n");

    printf("-------------------\n");
    ortakArkadasBul(105, 102);
    printf("-------------------\n");

    printf("\n*****************************************\n");

    RedBlackTree rbt;
    rbt.root = NULL;


    for (int i = 0; i < user_count; i++) {
        insert(&rbt, users[i] -> id);
    }

    printf("RedBlackTree ekleme yapildi...\n");

    printf("\n*****************************************\n");

    printf("Red Black Tree in-order Traversal: \n\n");
    innorderTraversal(rbt.root);

    printf("\n*****************************************\n");

    int aranan_id;
    char renk[20];
    printf("---RED-Black Tree kullanici arama--- \n");
    printf("Aramak istedigin kullanicinin ID si: "); scanf("%d",&aranan_id);

    RBNode *bulunan = search(&rbt, aranan_id);


    if (bulunan != NULL) {
        printf("User %d bulundu! Rengi: %s\n", aranan_id, bulunan -> color == RED ? "RED" : "BLACK");
    }

    else {
        printf("User %d bulunamadi! \n", aranan_id);
    }

    printf("\n*****************************************\n");
}