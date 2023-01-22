#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>


using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

struct _Instruction {
  string reg;
  bool increase;
  int amnt;
  string condition_reg;
  string condition_op;
  int condition_amnt;
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


void solve_p1_p2(vector<Instruction_t> &vals) {
  map<string, int> regs;
  int overall_max = 0;

  for(int i = 0; i < vals.size(); i++) {
    Instruction_t inst = vals[i];

    if(0 == regs.count(inst.reg)) {
      regs[inst.reg] = 0;
    }

    if(0 == regs.count(inst.condition_reg)) {
      regs[inst.condition_reg] = 0;
    }

    bool do_action;
    if(inst.condition_op == ">") {
      do_action = regs[inst.condition_reg] > inst.condition_amnt ? true:false;
    } else if (inst.condition_op == "<") {
      do_action = regs[inst.condition_reg] < inst.condition_amnt ? true:false;
    } else if (inst.condition_op == "<=") {
      do_action = regs[inst.condition_reg] <= inst.condition_amnt ? true:false;
    } else if (inst.condition_op == ">=") {
      do_action = regs[inst.condition_reg] >= inst.condition_amnt ? true:false;
    } else if (inst.condition_op == "==") {
      do_action = regs[inst.condition_reg] == inst.condition_amnt ? true:false;
    } else if (inst.condition_op == "!=") {
      do_action = regs[inst.condition_reg] != inst.condition_amnt ? true:false;
    } else {
      cout << inst.condition_op << " not found.. exiting" << endl;
      exit(1);
    }

    if(do_action) {
      if(inst.increase) {
        regs[inst.reg] += inst.amnt;
      } else {
        regs[inst.reg] -= inst.amnt;
      }
    }

    if(regs[inst.reg] > overall_max) {
      overall_max = regs[inst.reg];
    }
  }

  string max_reg;
  int max_val = 0;
  for(map<string, int>::iterator it = regs.begin(); it != regs.end(); it++) {
    if(it->second > max_val) {
      max_val = it->second;
      max_reg = it->first;
    }
  }

  cout << "PART1: " << max_val << endl;
  cout << "PART2: " << overall_max << endl;
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
    instruction.reg = split_line[0];
    instruction.increase = (split_line[1] == "inc") ? true:false;
    instruction.amnt = stoi(split_line[2]);
    instruction.condition_reg = split_line[4];
    instruction.condition_op = split_line[5];
    instruction.condition_amnt = stoi(split_line[6]);

    parsed_input.push_back(instruction);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<Instruction_t> &parsed_input) {
  if(p_arg == "p1") {
    solve_p1_p2(parsed_input);
  } else if(p_arg == "p2") {
    solve_p1_p2(parsed_input);
  } else if(p_arg == "both") {
    solve_p1_p2(parsed_input);
  }
}


int main(int argc, char *argv[]) {
  int rc = EXIT_SUCCESS;
  vector<Instruction_t> parsed_input;
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
