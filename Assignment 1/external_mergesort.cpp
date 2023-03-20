#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std;

void read_keys(string keys_file, vector<vector<int>> &keys, int num_keys, int num_keys_block){
    int num_blocks_keys_file = keys.size();

    ifstream file(keys_file);

    int count = 0;
    for(int i = 0; i < num_blocks_keys_file; i++){
        for(int j = 0; j < num_keys_block; j++){
            if(count == num_keys){
                return;
            }

            string str;
            file >> str;
            keys[i].push_back(stoi(str));
            count++;
        }
    }
}

void create_sorted_runs(vector<vector<int>> &keys, vector<vector<int>> &sorted_runs, vector<int> &memory, int mem_size, int num_keys_block){
    int num_blocks_keys_file = keys.size();

    for(int i = 0; i < num_blocks_keys_file; i += mem_size){
        // reading mem_size blocks at a time
        for(int j = 0; j < mem_size; j++){
            if(i + j == num_blocks_keys_file){
                break;
            }

            // reading each block
            for(int k = 0; k < keys[i + j].size(); k++){
                memory.push_back(keys[i + j][k]);
            }
        }

        // sorting the memory
        sort(memory.begin(), memory.end());

        // writing the sorted run to disk
        for(int j = 0; j < memory.size(); j++){
            if(sorted_runs.back().size() == num_keys_block){
                sorted_runs.push_back(vector<int>());
            }

            sorted_runs.back().push_back(memory[j]);
        }

        // clearing the memory
        memory.clear();
    }
}

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

    int num_blocks_keys_file = ceil((double) num_keys / num_keys_block);
    vector<vector<int>> keys(num_blocks_keys_file); // num_blocks_keys_file * num_keys_block
    read_keys(keys_file, keys, num_keys, num_keys_block);

    vector<int> memory; // max size = mem_size * num_keys_block

    vector<vector<int>> sorted_runs;
    create_sorted_runs(keys, sorted_runs, memory, mem_size, num_keys_block);

    int num_seeks = 0, num_transfers = 0;
    int num_merge_passes = 0;
    
    return 0;
}
