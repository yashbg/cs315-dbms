#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <set>
using namespace std;

void read_keys(string keys_file, vector<vector<int>> &keys, int num_keys, int num_keys_block){
    ifstream file(keys_file);

    for(int i = 0; i < num_keys; i++){
        if(keys.empty() || keys.back().size() == num_keys_block){
            keys.push_back(vector<int>());
        }

        string str;
        file >> str;
        keys.back().push_back(stoi(str));
    }
}

void create_sorted_runs(vector<vector<int>> &keys, vector<vector<vector<int>>> &sorted_runs, vector<int> &memory, int mem_size, int num_keys_block){
    int num_blocks_keys_file = keys.size();

    for(int i = 0; i < num_blocks_keys_file; i += mem_size){
        // reading mem_size blocks into memory
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
        sorted_runs.push_back(vector<vector<int>>());
        for(int j = 0; j < memory.size(); j++){
            if(sorted_runs.back().empty() || sorted_runs.back().back().size() == num_keys_block){
                sorted_runs.back().push_back(vector<int>());
            }

            sorted_runs.back().back().push_back(memory[j]);
        }

        // clearing the memory
        memory.clear();
    }
}

void merge_pass(vector<vector<vector<int>>> &merge_pass_input, vector<vector<vector<int>>> &merge_pass_output, vector<int> &memory, int mem_size, int num_keys_block){
    int num_runs = merge_pass_input.size();
    vector<int> block_idx(num_runs, 0);
    vector<int> num_keys_remaining(num_runs, 0);
    
    for(int i = 0; i < num_runs; i += mem_size - 1){
        merge_pass_output.push_back(vector<vector<int>>());
        multiset<pair<int, int>> memory_ordered;

        // reading the first blocks of mem_size - 1 runs into memory
        for(int j = 0; j < mem_size - 1; j++){
            if(i + j == num_runs){
                break;
            }

            for(int k = 0; k < merge_pass_input[i + j][block_idx[i + j]].size(); k++){
                memory_ordered.insert({merge_pass_input[i + j][block_idx[i + j]][k], i + j});
            }
            num_keys_remaining[i + j] = merge_pass_input[i + j][block_idx[i + j]].size();
            block_idx[i + j]++;
        }

        // merging mem_size - 1 runs
        while(!memory_ordered.empty()){
            pair<int, int> elem_pair = *memory_ordered.begin();
            int element = elem_pair.first;
            int run_idx = elem_pair.second;
            memory_ordered.erase(memory_ordered.begin());
            num_keys_remaining[run_idx]--;

            memory.push_back(element);
            
            if(memory.size() == num_keys_block){
                // write to disk
                merge_pass_output.back().push_back(memory);
                memory.clear();
            }
            
            if(num_keys_remaining[run_idx] == 0 && block_idx[run_idx] < merge_pass_input[run_idx].size()){
                // read next block
                for(int j = 0; j < merge_pass_input[run_idx][block_idx[run_idx]].size(); j++){
                    memory_ordered.insert({merge_pass_input[run_idx][block_idx[run_idx]][j], run_idx});
                }
                num_keys_remaining[run_idx] = merge_pass_input[run_idx][block_idx[run_idx]].size();
                block_idx[run_idx]++;
            }
        }

        if(!memory.empty()){
            // writing the remaining elements in memory to disk
            merge_pass_output.back().push_back(memory);
            memory.clear();
        }
    }
}

void merge(vector<vector<vector<int>>> &sorted_runs, vector<vector<vector<int>>> &merge_pass_output, vector<int> &memory, int mem_size, int num_keys_block, long long &num_merge_passes){
    vector<vector<vector<int>>> merge_pass_input(sorted_runs);
    int num_runs = merge_pass_input.size();

    while(num_runs >= mem_size){
        merge_pass(merge_pass_input, merge_pass_output, memory, mem_size, num_keys_block);
        num_merge_passes++;

        merge_pass_input = merge_pass_output;
        num_runs = merge_pass_input.size();
        merge_pass_output.clear();
    }

    merge_pass(merge_pass_input, merge_pass_output, memory, mem_size, num_keys_block);
    num_merge_passes++;
}

void external_mergesort(vector<vector<int>> &keys, vector<vector<vector<int>>> &mergesort_output, vector<int> &memory, int mem_size, int num_keys_block, long long &num_merge_passes){
    vector<vector<vector<int>>> sorted_runs; // runs * blocks * keys

    create_sorted_runs(keys, sorted_runs, memory, mem_size, num_keys_block);
    merge(sorted_runs, mergesort_output, memory, mem_size, num_keys_block, num_merge_passes);
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

    long long num_seeks = 0, num_transfers = 0;
    long long num_merge_passes = 0;

    int num_keys_block = block_size / key_size;
    vector<vector<int>> keys; // blocks * keys
    read_keys(keys_file, keys, num_keys, num_keys_block);

    vector<int> memory; // max size = mem_size * num_keys_block
    vector<vector<vector<int>>> mergesort_output;
    external_mergesort(keys, mergesort_output, memory, mem_size, num_keys_block, num_merge_passes);
    
    return 0;
}
