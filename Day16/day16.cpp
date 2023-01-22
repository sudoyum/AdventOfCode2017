#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <vector>


using namespace std;


#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define SIZE 16


struct DanceMove {
  int type;
  int spin;
  char part_a, part_b;
  int  pos_a, pos_b;
};
typedef DanceMove DanceMove_t;


// Credit for this function:
// https://stackoverflow.com/questions/30050739/c-split-string-line-by-line-using-stdregex
vector<string> splitString(const std::string& stringToSplit, const std::string& regexPattern)
{
  vector<string> result;

  const regex rgx(regexPattern);
  sregex_token_iterator iter(stringToSplit.begin(), stringToSplit.end(), rgx, -1);

  for (sregex_token_iterator end; iter != end; ++iter)
  {
    result.push_back(iter->str());
  }

  return result;
}


string join_vector(vector<char> &v) {
  string s;
  for(auto it = v.begin(); it != v.end(); ++it) {
    s.push_back(*it);
  }
  return s;
}


void do_spin(vector<char> &programs, int spin_count) {
  char temp;
  for(int i = 0; i < spin_count; i++) {
    temp = programs[SIZE - 1];
    for(int j = SIZE - 1; j >= 0; j--) {
      programs[j] = programs[j-1];
    }
    programs[0] = temp;
  }
}


void swap_indeces(vector<char> &programs, int index1, int index2) {
  char temp = programs[index1];
  programs[index1] = programs[index2];
  programs[index2] = temp;
}


// Replace with find
int get_index(vector<char> &programs, char c) {
  for(int i = 0; i < programs.size(); i++) {
    if(programs[i] == c) {
      return i;
    }
  }
  return -1;
}


void solve_p1(vector<DanceMove_t> vals) {
  vector <char> programs;

  char c = 'a';
  for(int i = 0; i < SIZE; i++) {
    programs.push_back(c + i);
  }

  for(auto it = vals.begin(); it != vals.end(); ++it) {
    DanceMove_t dmove = *it;
    switch(dmove.type) {
      case 1: {
        do_spin(programs, dmove.spin % SIZE);
        break;
      }
      case 2: {
        swap_indeces(programs, dmove.pos_a, dmove.pos_b);
        break;
      }
      case 3: {
        swap_indeces(programs, get_index(programs, dmove.part_a), get_index(programs, dmove.part_b));
        break;
      }
    }
  }

  cout << "PART1: " << join_vector(programs) << endl;
}

// 34ms for p1, ((34*10^9/10^6)/3600) == 9444 hours
void solve_p2(vector<DanceMove_t> vals) {
  vector <char> programs;
  vector <char> start_programs;
  set <vector <char>> patterns;

  char c = 'a';
  for(int i = 0; i < SIZE; i++) {
    programs.push_back(c + i);
  }
  start_programs = programs;
  int start_len;
  int iter_num = 0;
  while(1) {
    start_len = patterns.size();
    for(auto it = vals.begin(); it != vals.end(); ++it) {
      DanceMove_t dmove = *it;
      switch(dmove.type) {
        case 1: {
          do_spin(programs, dmove.spin % SIZE);
          break;
        }
        case 2: {
          swap_indeces(programs, dmove.pos_a, dmove.pos_b);
          break;
        }
        case 3: {
          swap_indeces(programs, get_index(programs, dmove.part_a), get_index(programs, dmove.part_b));
          break;
        }
      }
    }
    iter_num++;
    patterns.insert(programs);
    if(programs == start_programs) {
      break;
    }

  }

  auto it = patterns.find(programs);
  vector <char> matching = *it;

  //1000000000 % 44 == 32
  for(int i = 0; i < 32; i++) {
    for(auto it = vals.begin(); it != vals.end(); ++it) {
      DanceMove_t dmove = *it;
      switch(dmove.type) {
        case 1: {
          do_spin(programs, dmove.spin % SIZE);
          break;
        }
        case 2: {
          swap_indeces(programs, dmove.pos_a, dmove.pos_b);
          break;
        }
        case 3: {
          swap_indeces(programs, get_index(programs, dmove.part_a), get_index(programs, dmove.part_b));
          break;
        }
      }
    }
  }

  cout << "PART2: " << join_vector(programs) << endl;
}


int parse_input(char *input_file, vector<DanceMove_t> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  vector<string> split_line;
  while(getline(infile, line)) {
    split_line = splitString(line, ",");
    for(auto it = split_line.begin(); it != split_line.end(); it++) {
      DanceMove_t dance_move;
      string dancemove_str = *it;
      switch(dancemove_str[0]) {
        case 's': {
          dance_move.type = 1;
          dance_move.spin = stoi(dancemove_str.erase(0, 1));
          break;
        }
        case 'x': {
          dance_move.type = 2;
          vector<string> split_x = splitString(dancemove_str.erase(0, 1), "/");
          dance_move.pos_a = stoi(split_x[0]);
          dance_move.pos_b = stoi(split_x[1]);
          break;
        }
        case 'p': {
          dance_move.type = 3;
          vector<string> split_p = splitString(dancemove_str.erase(0, 1), "/");
          dance_move.part_a = split_p[0].c_str()[0];
          dance_move.part_b = split_p[1].c_str()[0];
          break;
        }
        default: {
          cout << "Issue, first char not found" << endl;
          break;
        }
      }
      parsed_input.push_back(dance_move);
    }
  }
  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<DanceMove_t> &parsed_input) {
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
  vector <DanceMove_t> parsed_input;
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
