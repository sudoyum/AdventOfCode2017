#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <set>
#include <vector>


using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define WAITING_ON_RECV 0xfff1


struct _Instruction {
  string type;
  string r0;
  string r1;
};

typedef _Instruction Instruction_t;

//Globals
int g_mostrecent = 0;
bool g_savedfirst = false;
int g_numsent = 0;
list <long long> g_queue0;
list <long long> g_queue1;


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


bool do_instruction(map <string, long long> &regs, vector<Instruction_t> &vals, int *pc) {
  Instruction_t inst = vals[*pc];
  if(inst.type == "snd") {
    if(!g_savedfirst) {
      g_mostrecent = regs[inst.r0];
    }
  } else if(inst.type == "set") {
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
    if(is_number(inst.r1)) {
      regs[inst.r0] *= stoi(inst.r1);
    } else {
      regs[inst.r0] *= regs[inst.r1];
    }
  } else if(inst.type == "mod") {
    if(is_number(inst.r1)) {
      regs[inst.r0] %= stoi(inst.r1);
    } else {
      regs[inst.r0] %= regs[inst.r1];
    }
  } else if(inst.type == "rcv") {
    if(regs[inst.r0] != 0) {
      if(!g_savedfirst) {
        g_savedfirst = true;
      }
      return false;
    }
  }

  if(inst.type == "jgz") {
    if(is_number(inst.r0)) {
      if(stoi(inst.r0) > 0) {
        if(is_number(inst.r1)) {
          *pc += stoi(inst.r1);
        } else {
          *pc += regs[inst.r1];
        }
      } else {
        *pc += 1;
      }
    } else {
      if(regs[inst.r0] > 0) {
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


bool do_instruction_p2(map <string, long long> &regs, vector<Instruction_t> &vals, int *pc, int process, int *state) {
  Instruction_t inst = vals[*pc];
  if(inst.type == "snd") {
    if(process == 1) {
      g_numsent++;
      if(is_number(inst.r0)) {
        g_queue1.push_back(stoi(inst.r0));
      } else {
        g_queue1.push_back(regs[inst.r0]);
      }
    } else {
      if(is_number(inst.r0)) {
        g_queue0.push_back(stoi(inst.r0));
      } else {
        g_queue0.push_back(regs[inst.r0]);
      }
    }
  } else if(inst.type == "set") {
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
    if(is_number(inst.r1)) {
      regs[inst.r0] *= stoi(inst.r1);
    } else {
      regs[inst.r0] *= regs[inst.r1];
    }
  } else if(inst.type == "mod") {
    if(is_number(inst.r1)) {
      regs[inst.r0] %= stoi(inst.r1);
    } else {
      regs[inst.r0] %= regs[inst.r1];
    }
  } else if(inst.type == "rcv") {
    if(process == 1) {
      if(g_queue0.size() == 0) {
        *state = WAITING_ON_RECV;
      } else {
        regs[inst.r0] = g_queue0.front();
        g_queue0.pop_front();
        *state = 0;
      }
    } else {
      if(g_queue1.size() == 0) {
        *state = WAITING_ON_RECV;
      } else {
        regs[inst.r0] = g_queue1.front();
        g_queue1.pop_front();
        *state = 0;
      }
    }
  }

  if(inst.type == "jgz") {
    if(is_number(inst.r0)) {
      if(stoi(inst.r0) > 0) {
        if(is_number(inst.r1)) {
          *pc += stoi(inst.r1);
        } else {
          *pc += regs[inst.r1];
        }
      } else {
        *pc += 1;
      }
    } else {
      if(regs[inst.r0] > 0) {
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
    if(*state == 0) {
      *pc += 1;
    }
  }

  if(*pc >= vals.size() || *pc < 0) {
    return false;
  }
  return true;
}


void solve_p1(vector<Instruction_t> vals) {
  map <string, long long> regs = get_initial_regs(vals);

  int pc = 0;
  while(true) {
    if(!do_instruction(regs, vals, &pc)) {
      break;
    }
  }

  cout << "PART1: " << g_mostrecent << endl;
}


void solve_p2(vector<Instruction_t> vals) {
  map <string, long long> regs0 = get_initial_regs(vals);
  map <string, long long> regs1 = get_initial_regs(vals);
  vector<Instruction_t> insts0 = vals;
  vector<Instruction_t> insts1 = vals;

  regs1["p"] = 1;

  bool p0_done = false;
  bool p1_done = false;
  int state0 = 0;
  int state1 = 0;
  int pc0 = 0;
  int pc1 = 0;

  while(true) {
    if(!p0_done) {
      if(!do_instruction_p2(regs0, insts0, &pc0, 0, &state0)) {
        p0_done = true;
      }
    }
    if(!p1_done) {
      if(!do_instruction_p2(regs1, insts1, &pc1, 1, &state1)) {
        p1_done = true;
      }
    }
    if((p1_done && p0_done) || (state0 == WAITING_ON_RECV && state1 == WAITING_ON_RECV)) {
      break;
    }
  }

  cout << "PART2: " << g_numsent << endl;
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
