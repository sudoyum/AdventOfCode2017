#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <vector>


using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1


long long g_nummultiply = 0;

struct _Instruction {
  string type;
  string r0;
  string r1;
};

typedef _Instruction Instruction_t;


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


bool is_number(string test_string) {
  for(auto it = test_string.begin(); it != test_string.end(); ++it) {
    if(isdigit(*it)) {
      return true;
    }
  }
  return false;
}


map <string, long long> get_initial_regs(vector<Instruction_t> &vals) {
  map <string, long long> regs;
  set <string> names;
  for(auto it = vals.begin(); it != vals.end(); ++it) {
    if(!is_number(it->r0)) {
      names.insert(it->r0);
    }
  }
  for(auto it = names.begin(); it != names.end(); ++it) {
    regs[*it] = 0;
  }
  return regs;
}


bool do_instruction(map <string, long long> &regs, vector<Instruction_t> &vals, int *pc) {
  Instruction_t inst = vals[*pc];
  if(inst.type == "set") {
    if(is_number(inst.r1)) {
      regs[inst.r0] = stoi(inst.r1);
    } else {
      regs[inst.r0] = regs[inst.r1];
    }
  } else if(inst.type == "add") {
    if(is_number(inst.r1)) {
      regs[inst.r0] += stoi(inst.r1);
    } else {
      regs[inst.r0] += regs[inst.r1];
    }
  } else if(inst.type == "mul") {
    g_nummultiply++;
    if(is_number(inst.r1)) {
      regs[inst.r0] *= stoi(inst.r1);
    } else {
      regs[inst.r0] *= regs[inst.r1];
    }
  } else if(inst.type == "sub") {
    if(is_number(inst.r1)) {
      regs[inst.r0] -= stoi(inst.r1);
    } else {
      regs[inst.r0] -= regs[inst.r1];
    }
  }


  if(inst.type == "jnz") {
    if(is_number(inst.r0)) {
      if(stoi(inst.r0) != 0) {
        if(is_number(inst.r1)) {
          *pc += stoi(inst.r1);
        } else {
          *pc += regs[inst.r1];
        }
      } else {
        *pc += 1;
      }
    } else {
      if(regs[inst.r0] != 0) {
        if(is_number(inst.r1)) {
          *pc += stoi(inst.r1);
        } else {
          *pc += regs[inst.r1];
        }
      } else {
        *pc += 1;
      }
    }
  } else {
    *pc += 1;
  }

  if(*pc >= vals.size() || *pc < 0) {
    return false;
  }
  return true;
}


int check_prime(int a)
{
  int c;

  for ( c = 2 ; c <= a - 1 ; c++ )
  {
    if ( a%c == 0 )
      return 0;
  }
  return 1;
}


long long run_p2() {
  long long a = 1;
  long long b = 0;
  long long c = 0;
  long long d = 0;
  long long e = 0;
  long long f = 0;
  long long g = 0;
  long long h = 0;

  int iteration = 0;

  b = 84;
  c = b;
  if(a != 0) {
    b *= 100;
    b -= -100000;
    c = b;
    c -= -17000;
  }

  while(1) {
    f = 1;
    d = 2;
    if(check_prime(b) != 1) {
      f = 0;
    }
    if(f == 0) {
      h -= -1;
    }
    g = b;
    g -= c;
    if(g == 0) {
      return h;
    }
    b -= -17;
    iteration++;
  }
}


void solve_p1(vector<Instruction_t> vals) {
  int pc = 0;
  map <string, long long> regs = get_initial_regs(vals);
  regs["a"] = 0;

  while(true) {
    if(!do_instruction(regs, vals, &pc)) {
      break;
    }
  }

  cout << "PART1: " << g_nummultiply << endl;
}

void solve_p2(vector<Instruction_t> vals) {
  cout << "PART2: " << run_p2() << endl;
}


int parse_input(char *input_file, vector<Instruction_t> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    vector<string> split_line = splitString(line, "\\s+");
    Instruction_t instruction;
    instruction.type= split_line[0];
    instruction.r0 = split_line[1];
    if(split_line.size() > 2) {
      instruction.r1 = split_line[2];
    } else {
      instruction.r1 = -1;
    }

    parsed_input.push_back(instruction);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<Instruction_t> &parsed_input) {
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
  vector <Instruction_t> parsed_input;
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
