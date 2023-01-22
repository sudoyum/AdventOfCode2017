#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <tuple>
#include <vector>

#include "knothash.h"

using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define GRID_SIZE 128
#define NUM_BITS 128
#define PRINT_GSIZE 8


void print_grid(vector<vector<bool>> &grid) {
  for(int j = 0; j < PRINT_GSIZE; j++) {
    for(int i = GRID_SIZE - 1; i >= GRID_SIZE - PRINT_GSIZE; i--) {
      if(grid[j][i]) {
        cout << "#";
      } else {
        cout << ".";
      }
    }
    cout << endl;
  }
}


void print_gridi(vector<vector<int>> &grid) {
  for(int j = 0; j < PRINT_GSIZE; j++) {
    for(int i = GRID_SIZE - 1; i >= GRID_SIZE - PRINT_GSIZE; i--) {
      if(grid[j][i] > 1) {
        cout << grid[j][i];
      } else {
        cout << ".";
      }
    }
    cout << endl;
  }
}


int count_grid(vector<vector<bool>> &grid) {
  int count = 0;
  for(int i = 0; i < GRID_SIZE; i++) {
    for(int j = 0; j < GRID_SIZE; j++) {
      if(grid[j][i]) {
        count++;
      }
    }
  }
  return count;
}


int count_groups(vector<vector<int>> &grid) {
  int group_num = 2;
  for(int j = 0; j < GRID_SIZE; j++) {
    for(int i = 0; i < GRID_SIZE; i++) {
      if(grid[j][i] == 1) {
        list<tuple<int, int>> stack;
        stack.push_back(make_tuple(j, i));
        int processed = 0;
        while(!stack.empty()) {
          tuple<int, int> first = stack.back();
          stack.pop_back();

          int y = get<0>(first);
          int x = get<1>(first);

          if(y - 1 >= 0) {
            if(grid[y - 1][x] == 1) {
              processed++;
              stack.push_back(make_tuple(y - 1, x));
            }
          }
          if(x - 1 >= 0) {
            if(grid[y][x - 1] == 1) {
              processed++;
              stack.push_back(make_tuple(y, x - 1));
            }
          }

          if(x + 1 < GRID_SIZE) {
            if(grid[y][x + 1] == 1) {
              stack.push_back(make_tuple(y, x + 1));
            }
          }
          if(y + 1 < GRID_SIZE) {
            if(grid[y + 1][x] == 1) {
              stack.push_back(make_tuple(y + 1, x));
            }
          }
          grid[y][x] = group_num;
        }
        group_num++;
      }
    }
  }
  return group_num - 2;
}


int char_to_int(char c) {
  if((int)c >= 48 && (int)c <= 58) {
    return c - 48;
  } else if(103 >= (int)c && (int)c >= 97) {
    return c - 87;
  } else {
    cout << "bad hex input " << c << endl;
    cout << "bad hex input " << typeid(c).name() << endl;
    exit(1);
  }
}


int get_int_fromhex(char upper, char lower) {
  int upper_i = char_to_int(upper) << 4;
  int lower_i = char_to_int(lower);
  return upper_i + lower_i;
}


vector<bool> hash_to_bits(string hash) {
  vector<bool> bits;
  for(int i = hash.size() - 1; i > 0; i-=2) {
    int intbyte = get_int_fromhex(hash[i-1], hash[i]);
    for(int j = 0; j < 8; j++) {
      if(intbyte & (0x1 << j)) {
        bits.push_back(true);
      } else {
        bits.push_back(false);
      }
    }
  }
  return bits;
}


void solve_p1(vector<string> vals) {
  vector<vector<bool>> grid;
  for(int i = 0; i < GRID_SIZE; i++) {
    vector<bool> inner;
    for(int j = 0; j < GRID_SIZE; j++) {
      inner.push_back(false);
    }
    grid.push_back(inner);
  }

  for(int j = 0; j < GRID_SIZE; j++) {
    string hash_data = vals[0] + string("-") + to_string(j);
    string hash = hasher(hash_data);
    vector<bool> bits = hash_to_bits(hash);

    for(int i = 0; i < bits.size(); i++) {
      if(bits[i]) {
        grid[j][i] = true;
      }
    }
  }

  cout << "PART1: " << count_grid(grid) << endl;
}


void solve_p2(vector<string> vals) {
  vector<vector<int>> grid;
  for(int i = 0; i < GRID_SIZE; i++) {
    vector<int> inner;
    for(int j = 0; j < GRID_SIZE; j++) {
      inner.push_back(0);
    }
    grid.push_back(inner);
  }

  for(int j = 0; j < GRID_SIZE; j++) {
    string hash_data = vals[0] + string("-") + to_string(j);
    string hash = hasher(hash_data);
    vector<bool> bits = hash_to_bits(hash);

    for(int i = 0; i < bits.size(); i++) {
      if(bits[i]) {
        grid[j][i] = 1;
      }
    }
  }

  cout << "PART2: " << count_groups(grid) << endl;
}


int parse_input(char *input_file, vector<string> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    parsed_input.push_back(line);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<string> &parsed_input) {
  if(p_arg == "p1") {
    solve_p1(parsed_input);
  } else if(p_arg == "p2") {
    solve_p2(parsed_input);
  } else if(p_arg == "both") {
    solve_p1(parsed_input);
    solve_p2(parsed_input);
  }
}


int main(int argc, char *argv[]) {
  int rc = EXIT_SUCCESS;
  vector <string> parsed_input;
  std::vector<std::string> cl_args = {"both", "p1", "p2"};

  if(argc != 3 || count(cl_args.begin(), cl_args.end(), argv[2]) <= 0) {
    cout << "Usage: ./day input.txt p1|p2|both" << endl;
    rc = EXIT_FAILURE;
    goto cleanup;
  }

  if(parse_input(argv[1], parsed_input) == PARSE_FAILURE) {
    cout << "Issue parsing input, exiting" << endl;
    rc = EXIT_FAILURE;
    goto cleanup;
  }

  run_solutions(argv[2], parsed_input);

cleanup:
  return rc;
}
