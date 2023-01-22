#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <map>
#include <vector>


using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1


struct _Column_t {
  int size;
  int index;
  bool decrease;
};

typedef _Column_t Column_t;


int check_caught(int layer_index, map<int, Column_t> &vals) {
  if(vals.count(layer_index) != 0) {
    if(vals[layer_index].index == 0) {
      return layer_index * vals[layer_index].size;
    }
  }
  return 0;
}


bool check_caught_all(int layer_index, map<int, Column_t> &vals) {
  if(vals.count(layer_index) != 0) {
    if(vals[layer_index].index == 0) {
      return true;
    }
  }
  return false;
}


void tick(map<int, Column_t> &vals) {
  map<int, Column_t>:: iterator itr;
  for (itr = vals.begin(); itr != vals.end(); itr++) {
    if(itr->second.decrease) {
      itr->second.index -= 1;
      if(itr->second.index == 0) {
        itr->second.decrease = false;
      }
    } else {
      itr->second.index += 1;
      if(itr->second.index == (itr->second.size - 1)) {
        itr->second.decrease = true;
      }
    }
  }
}


void solve_p1(map<int, Column_t> vals) {
  map<int, Column_t>:: iterator itr;

  int layer_index = 0;
  int total_caught = 0;

  itr = vals.end();
  int max = (--itr)->first;
  do {
    total_caught += check_caught(layer_index, vals);
    tick(vals);
    layer_index += 1;
  } while(layer_index <= max);

  cout << "PART1: " << total_caught << endl;
}


void solve_p2(map<int, Column_t> vals) {
  map<int, Column_t>:: iterator itr;
  long long total_delay = 0;
  long long delay = 0;
  long long layer_index = 0;

  itr = vals.end();
  long long max = (--itr)->first;
  bool passed;

  map<int, Column_t> start_vals = vals;
  map<int, Column_t> saved_vals;

  do {
    layer_index = 0;
    passed = true;
    tick(start_vals);
    total_delay++;
    saved_vals = start_vals;
    do {
      if(check_caught_all(layer_index, start_vals)) {
        passed = false;
        delay = total_delay;
        start_vals = saved_vals;
        break;
      }
      tick(start_vals);
      layer_index++;
    } while(layer_index <= (max + 1) && passed);

    if(passed) {
      break;
    }
  } while(1);

  cout << "PART2: " << total_delay << endl;
}


int parse_input(char *input_file, map<int, Column_t> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  const regex rgx("([0-9]+): ([0-9]+)$");
  smatch matches;

  while(getline(infile, line)) {
    if(regex_search(line, matches, rgx)) {
      Column_t column;
      column.index = 0;
      column.size = stoi(matches[2].str());
      column.decrease = false;
      parsed_input[stoi(matches[1].str())] = column;
    }
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, map<int, Column_t> &parsed_input) {
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
  map<int, Column_t> parsed_input;
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
