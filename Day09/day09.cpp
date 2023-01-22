#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;

#define GROUP 1
#define GARBAGE 2
#define GARBAGE_IGN 3

#define INNER 2

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1


int get_total_score(vector<char> &stack) {
  int total_score = 0, score = 0;
  int state = GROUP;
  for(auto it = stack.begin(); it != stack.end(); ++it) {
    switch(state) {
      case GROUP: {
        if(*it == '{') {
          score += 1;
        } else if(*it == '}') {
          total_score += score;
          state = INNER;
        }
        break;
      }
      case INNER: {
        if(*it == '{') {
          state = GROUP;
        } else if(*it == '}') {
          score -= 1;
          total_score += score;
        }
        break;
      }
    }
  }
  return total_score;
}


void solve_p1(vector<string> vals) {
  vector<char> stack;
  int state = GROUP;
  for(char& c : vals[0]) {
    switch(state) {
      case GROUP: {
        if(c == '{' || c == '}') {
          stack.push_back(c);
        }
        if(c == '<') {
          state = GARBAGE;
        }
        break;
      }
      case GARBAGE: {
        if(c == '!') {
          state = GARBAGE_IGN;
        }
        if(c == '>') {
          state = GROUP;
        }
        break;
      }
      case GARBAGE_IGN: { state = GARBAGE; break; }
    }
  }

  cout << "PART1: " << get_total_score(stack) << endl;
}


void solve_p2(vector<string> vals) {
  vector<char> stack;
  int state = GROUP;
  int num_garbage = 0;
  for(char& c : vals[0]) {
    switch(state) {
      case GROUP: {
        if(c == '<') {
          state = GARBAGE;
        }
        break;
      }
      case GARBAGE: {
        if(c == '!') {
          state = GARBAGE_IGN;
        } else if(c == '>') {
          state = GROUP;
        } else {
          num_garbage += 1;
        }
        break;
      }
      case GARBAGE_IGN: { state = GARBAGE; break; }
    }
  }

  cout << "PART2: " << num_garbage << endl;
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
  vector<string> parsed_input;
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
