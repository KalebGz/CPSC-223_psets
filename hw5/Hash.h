typedef struct hash *Hash;
 

typedef struct triple
{
    char *position; // key
    char *from;     // position from where it was first reached by a single move
    int nSteps;     // number of steps required to get to position from initial position
}* Triple;


Hash createH();

// Insert 
void insertH(Hash h, char* positon, char *from, int nSteps);

// searches through the hash table and returns the 
// value assiiated with the most recent key
Triple searchH(Hash h, char *key);

// //delete the most recent value with the given key
// bool deleteH(Hash h, const char *key);

//destroys the Hash table
void destroyH(Hash h);


