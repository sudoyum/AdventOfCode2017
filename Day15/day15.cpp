#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>


using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define P1_LOOP_NUM 40000000
#define P2_LOOP_NUM 5000000


class AdvGenerator {
  long long value;
  int factor;
  int lower;
  public:
  AdvGenerator(int, int);
  void do_tick();
  long long get_value() { return value; }
  int get_lower() { return lower; }
};


AdvGenerator::AdvGenerator(int _start, int _factor) {
  value = _start;
  factor = _factor;
}


void AdvGenerator::do_tick() {
  value *= factor;
  value %= 0x000000007FFFFFFF;
  lower = value & 0xFFFF;
}


void solve_p1(vector<string> vals) {
  int num_matches = 0;
  AdvGenerator A(stoi(vals[0]), 16807);
  AdvGenerator B(stoi(vals[1]), 48271);

  for(int i = 0; i < P1_LOOP_NUM; i++) {
    A.do_tick();
    B.do_tick();
    if(A.get_lower() == B.get_lower()) {
      num_matches += 1;
    }
  }
  cout << "PART1: " << num_matches << endl;
}


void solve_p2(vector<string> vals) {
  int num_matches = 0;
  AdvGenerator A(stoi(vals[0]), 16807);
  AdvGenerator B(stoi(vals[1]), 48271);

  vector <long long> avals;
  vector <long long> bvals;
  int found = 0;
  while(1) {
    A.do_tick();
    if(A.get_value() % 4 == 0) {
      found++;
      avals.push_back(A.get_value());
    }
    if(found == P2_LOOP_NUM) {
      break;
    }
  }
  found = 0;
  while(1) {
    B.do_tick();
    if(B.get_value() % 8 == 0) {
      found++;
      bvals.push_back(B.get_value());
    }
    if(found == P2_LOOP_NUM) {
      break;
    }
  }

  for(int i = 0; i < P2_LOOP_NUM; i++) {
    if((avals[i] & 0xFFFF) == (bvals[i] & 0xFFFF)) {
      num_matches += 1;
    }
  }
  cout << "PART2: " << num_matches << endl;
}


int parse_input(char *input_file, vector<string> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  const regex rgx("starts with ([0-9]+)");
  while(getline(infile, line)) {
    smatch matches;
    if(regex_search(line, matches, rgx)) {
      parsed_input.push_back(matches[1]);
    }
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
