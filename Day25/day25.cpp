#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <vector>

using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define STATE_START 0
#define STATE_PARSING 1
#define STATE_ZERO 2
#define STATE_ONE 3

#define LEN 100000000


struct _State {
  string state_name;

  int zwrite_value;
  string zmove_dir;
  string znext_state;

  int owrite_value;
  string omove_dir;
  string onext_state;
};

typedef _State State;


class BluePrint {
    string _start_state;
    long long _num_steps;
    map<string, State> mstates;
    vector<State> states;
    string current_state;
    vector<int> tape;
    int curr_index;
  public:
    void init();
    void set_steps(long long num_steps) { _num_steps = num_steps; }
    long long get_steps() { return _num_steps; }
    void set_start(string start_state) { _start_state = start_state; }
    void add_state(State state) { states.push_back(state); }
    void print_blueprint();
    void do_state();
    int get_ones();
};


void BluePrint::init() {
  current_state = "A";
  for(auto it = states.begin(); it != states.end(); ++it) {
    State state = *it;
    mstates[state.state_name] = state;
  }
  tape = vector<int>(LEN, 0);
  curr_index = LEN/2;
}


int BluePrint::get_ones() {
  int total = 0;
  for(auto it = tape.begin(); it != tape.end(); ++it) {
    if(*it == 1) {
      total++;
    }
  }
  return total;
}


void BluePrint::do_state() {
  State state = mstates[current_state];
  if(tape[curr_index] == 0) {
    tape[curr_index] = state.zwrite_value;
    if(state.zmove_dir == "left") {
      curr_index--;
    } else {
      curr_index++;
    }
    current_state = state.znext_state;
  } else {
    tape[curr_index] = state.owrite_value;
    if(state.omove_dir == "left") {
      curr_index--;
    } else {
      curr_index++;
    }
    current_state = state.onext_state;
  }
}


void BluePrint::print_blueprint() {
  for(auto it = states.begin(); it != states.end(); ++it) {
    cout << "State " << it->state_name << endl;
    cout << "   ZWrite val  " << it->zwrite_value << endl;
    cout << "   ZMove dir   " << it->zmove_dir << endl;
    cout << "   ZNext state " << it->znext_state << endl;
    cout << "   OWrite val  " << it->owrite_value << endl;
    cout << "   OMove dir   " << it->omove_dir << endl;
    cout << "   ONext state " << it->onext_state << endl;
  }
}


void solve_p1(BluePrint bp) {
  int index = 0;
  for(long long i = 0; i < bp.get_steps(); i++) {
    bp.do_state();
  }
  cout << "PART1: " << bp.get_ones() << endl;
}


void solve_p2(BluePrint bp) {
  cout << "PART2: free" << endl;
}


int parse_input(char *input_file, BluePrint &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }


  BluePrint bp;
  const regex rgx("checksum after ([0-9]+)");
  const regex rgx2("In state ([A-Z]+)");
  const regex current_rgx("current value is ([0-9]+)");

  const regex cond1("Write the value ([0-9]+)");
  const regex cond2("Move one slot to the ([a-z]+)");
  const regex cond3("Continue with state ([A-Z]+)");

  int state = STATE_START;

  State prog_state;
  while(getline(infile, line)) {
    smatch matches;
    if(regex_search(line, matches, rgx)) {
      bp.set_steps(stol(matches[1]));
    }

    if(state == STATE_START && regex_search(line, matches, rgx2)) {
      prog_state.state_name = matches[1];
      state = STATE_PARSING;
    } else if(state == STATE_PARSING) {
      if(regex_search(line, matches, current_rgx)) {
        if(stoi(matches[1]) == 1) {
          state = STATE_ONE;
        } else {
          state = STATE_ZERO;
        }
      }
    } else if(state == STATE_ZERO) {
      if(regex_search(line, matches, cond1)) {
        prog_state.zwrite_value = stoi(matches[1]);
      } else if(regex_search(line, matches, cond2)) {
        prog_state.zmove_dir = matches[1].str();
      } else if(regex_search(line, matches, cond3)) {
        prog_state.znext_state = matches[1].str();
        state = STATE_ONE;
      }
    } else if(state == STATE_ONE) {
      if(regex_search(line, matches, cond1)) {
        prog_state.owrite_value = stoi(matches[1]);
      } else if(regex_search(line, matches, cond2)) {
        prog_state.omove_dir = matches[1].str();
      } else if(regex_search(line, matches, cond3)) {
        prog_state.onext_state = matches[1].str();
        state = STATE_START;
        bp.add_state(prog_state);
      }
    }
  }
  bp.init();
  parsed_input = bp;

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, BluePrint &parsed_input) {
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
  BluePrint parsed_input;
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
