#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char *argv[]){
    if(argc != 6){
        cout << "Please enter exactly 5 arguments other than the program name. " << endl;
        return 0;
    }

    string keys_file = argv[1];
    int mem_size = atoi(argv[2]); // size of available memory in number of blocks
    int key_size = atoi(argv[3]); // size of each key in bytes
    int num_keys = atoi(argv[4]); // total number of keys
    int block_size = atoi(argv[5]); // disk block size in bytes

    int num_keys_block = block_size / key_size;

    vector<vector<int>> memory; // max size = mem_size * num_keys_block

    int num_seeks = 0, num_transfers = 0;
    int num_merge_passes = 0;
    
    return 0;
}
