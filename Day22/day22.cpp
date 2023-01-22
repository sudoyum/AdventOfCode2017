#include <algorithm>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>


using namespace std;


#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define EXPAND_SIZE 10000
#define NUM_ITERATIONS_P2 10000000
#define NUM_ITERATIONS_P1 10000

#define INFECTED 1
#define CLEAN 0
#define FLAGGED 2
#define WEAKENED 3

#define UP    1
#define RIGHT 2
#define DOWN  3
#define LEFT  4


tuple<int,int> get_start(vector<vector<int>> &vals) {
  int width = vals[0].size();
  int length = vals.size();
  return make_tuple(length/2, width/2);
}


void turn_direction(int* direction, int turn_dir) {
  if(turn_dir == RIGHT) {
    *direction = ((*direction) % 4) + 1;
  } else {
    *direction = (*direction) - 1;
    if(*direction == 0) {
      *direction = 4;
    }
  }
}


void update_position(int direction, int* y, int* x) {
  if(direction == UP) {
    *y -= 1;
  } else if(direction == RIGHT) {
    *x += 1;
  } else if(direction == DOWN) {
    *y += 1;
  } else if(direction == LEFT) {
    *x -= 1;
  }
}


void print_plane(vector<vector<int>> &plane, int x, int y) {
  for(int j = 0; j < EXPAND_SIZE; j++) {
    for(int i = 0; i < EXPAND_SIZE; i++) {
      if(j == y && i == x) {
        cout << "x";
      } else if(plane[j][i] == CLEAN) {
        cout << ".";
      } else if(plane[j][i] == INFECTED) {
        cout << "#";
      } else if(plane[j][i] == FLAGGED) {
        cout << "F";
      } else if(plane[j][i] == WEAKENED) {
        cout << "W";
      }
    }
    cout << endl;
  }
}

void solve_p1(vector<vector<int>> vals) {
  int x, y;
  tie(x, y) = get_start(vals);
  x += (EXPAND_SIZE/2 - vals.size()/2);
  y += (EXPAND_SIZE/2 - vals.size()/2);
  vector<vector<int>> plane;
  for(int j = 0; j < EXPAND_SIZE; j++) {
    vector<int> row;
    for(int i = 0; i < EXPAND_SIZE; i++) {
      row.push_back(0);
    }
    plane.push_back(row);
  }

  for(int j = 0; j < vals.size(); j++) {
    for(int i = 0; i < vals[0].size(); i++) {
      plane[j + (EXPAND_SIZE/2 - vals.size()/2)][i + (EXPAND_SIZE/2 - vals[0].size()/2)] = vals[j][i];
    }
  }


  int infections_caused = 0;
  int direction = UP;
  for(int i = 0; i < NUM_ITERATIONS_P1; i++) {

    if(plane[y][x] == INFECTED) {
      turn_direction(&direction, RIGHT);
      plane[y][x] = CLEAN;
    } else {
      turn_direction(&direction, LEFT);
      plane[y][x] = INFECTED;
      infections_caused++;
    }
    update_position(direction, &y, &x);
  }

  cout << "PART1: " << infections_caused << endl;
}


void solve_p2(vector<vector<int>> vals) {
  int x, y;
  tie(x, y) = get_start(vals);
  x += (EXPAND_SIZE/2 - vals.size()/2);
  y += (EXPAND_SIZE/2 - vals.size()/2);
  vector<vector<int>> plane;
  for(int j = 0; j < EXPAND_SIZE; j++) {
    vector<int> row;
    for(int i = 0; i < EXPAND_SIZE; i++) {
      row.push_back(0);
    }
    plane.push_back(row);
  }

  for(int j = 0; j < vals.size(); j++) {
    for(int i = 0; i < vals[0].size(); i++) {
      plane[j + (EXPAND_SIZE/2 - vals.size()/2)][i + (EXPAND_SIZE/2 - vals[0].size()/2)] = vals[j][i];
    }
  }

  int infections_caused = 0;
  int direction = UP;
  for(int i = 0; i < NUM_ITERATIONS_P2; i++) {

    if(plane[y][x] == INFECTED) {
      turn_direction(&direction, RIGHT);
      plane[y][x] = FLAGGED;
    } else if(plane[y][x] == CLEAN) {
      turn_direction(&direction, LEFT);
      plane[y][x] = WEAKENED;
    } else if(plane[y][x] == FLAGGED) {
      plane[y][x] = CLEAN;
      if(direction == LEFT) {
        direction = RIGHT;
      } else if(direction == RIGHT) {
        direction = LEFT;
      } else if(direction == UP) {
        direction = DOWN;
      } else if(direction == DOWN) {
        direction = UP;
      }

    } else if(plane[y][x] == WEAKENED) {
      plane[y][x] = INFECTED;
      infections_caused++;
    }
    update_position(direction, &y, &x);
  }

  cout << "PART2: " << infections_caused << endl;
}


int parse_input(char *input_file, vector<vector<int>> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    vector<int> row;
    for(char &c : line) {
      if(c == '.') {
        row.push_back(0);
      } else if(c == '#') {
        row.push_back(1);
      } else {
        cout << "parsing fail" << endl;
        exit(1);
      }
    }
    parsed_input.push_back(row);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<vector<int>> &parsed_input) {
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
  vector<vector<int>> parsed_input;
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
