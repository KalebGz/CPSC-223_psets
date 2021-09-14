
typedef struct node{
    char chr;
    int numSubNodes;
    Node* subnodes;
}Node* ;

struct trie
{
Node* alphabet[26];
};

typedef struct counter{
    Counter* words;
    int numItems;
}Counter*;