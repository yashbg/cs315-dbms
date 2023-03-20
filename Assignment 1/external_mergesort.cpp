#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std;

void read_keys(string keys_file_name, vector<vector<int>> &keys_file, int num_keys, int num_keys_block){
    int num_blocks_keys_file = keys_file.size();

    ifstream file(keys_file_name);

    int count = 0;
    for(int i = 0; i < num_blocks_keys_file; i++){
        for(int j = 0; j < num_keys_block; j++){
            if(count == num_keys){
                return;
            }

            string str;
            file >> str;
            keys_file[i].push_back(stoi(str));
            count++;
        }
    }
}

int main(int argc, char *argv[]){
    if(argc != 6){
        cout << "Please enter exactly 5 arguments other than the program name. " << endl;
        return 0;
    }

    string keys_file_name = argv[1];
    int mem_size = atoi(argv[2]); // size of available memory in number of blocks
    int key_size = atoi(argv[3]); // size of each key in bytes
    int num_keys = atoi(argv[4]); // total number of keys
    int block_size = atoi(argv[5]); // disk block size in bytes

    int num_keys_block = block_size / key_size;

    int num_blocks_keys_file = ceil((double) num_keys / num_keys_block);
    vector<vector<int>> keys_file(num_blocks_keys_file); // num_blocks_keys_file * num_keys_block
    read_keys(keys_file_name, keys_file, num_keys, num_keys_block);

    vector<vector<int>> memory; // max size = mem_size * num_keys_block

    int num_seeks = 0, num_transfers = 0;
    int num_merge_passes = 0;
    
    return 0;
}
