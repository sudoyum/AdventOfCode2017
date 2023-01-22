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


class Disc {
    string name;
    int weight;
    vector <string> children;
  public:
    Disc(string, int);
    void push_child(string);
    void print_children();
    string get_name() { return name; }
    int get_weight() { return weight; }
    int num_children() { return children.size(); }
    vector <string> get_children() { return children; }
};


Disc::Disc(string _name, int _weight) {
  name = _name;
  weight = _weight;
}


void Disc::push_child(string child) {
  children.push_back(child);
}


void Disc::print_children() {
  vector<string>::iterator it;
  for(it = children.begin(); it != children.end(); it++) {
    cout << *it << endl;
  }
}


void solve_p1(vector<Disc *> vals) {
  vector<Disc *>::iterator it;
  vector<string> all_children;

  for(it = vals.begin(); it != vals.end(); it++) {
    vector <string> children = (*it)->get_children();
    all_children.insert(all_children.begin(), children.begin(), children.end());
  }

  string bottom;
  for(it = vals.begin(); it != vals.end(); it++) {
    if(find(all_children.begin(), all_children.end(), (*it)->get_name()) == all_children.end()) {
      bottom = (*it)->get_name();
    }
  }

  cout << "PART1: " << bottom << endl;
}


void solve_p2(vector<Disc *> vals) {
  int total_determined = 0;
  map<string, int> individual_weights;
  map<string, int> weights;
  vector<Disc *>::iterator it;

  for(it = vals.begin(); it != vals.end(); it++) {
    individual_weights[(*it)->get_name()] = (*it)->get_weight();
    if((*it)->num_children() == 0) {
      weights[(*it)->get_name()] = (*it)->get_weight();
      total_determined += 1;
    } else {
      weights[(*it)->get_name()] = -1;
    }
  }

  while(total_determined < vals.size()) {
    for(it = vals.begin(); it != vals.end(); it++) {
      vector <string> children = (*it)->get_children();
      vector<string>::iterator iter;
      int total_weight = 0;
      bool valid = true;
      for(iter = children.begin(); iter != children.end(); iter++) {
        if(weights[*iter] == -1) {
          valid = false;
          break;
        } else {
          total_weight += weights[(*iter)];
        }
      }
      if(valid && weights[(*it)->get_name()] == -1) {
        weights[(*it)->get_name()] = total_weight + (*it)->get_weight();
        total_determined += 1;
      }
    }
  }

  int total_diff = 0;
  int unbalanced_weight = 0;
  string unbalanced_child;
  for(it = vals.begin(); it != vals.end(); it++) {
    vector <string> children = (*it)->get_children();
    vector<string>::iterator iter;
    set<int> child_weights;
    map<int, string> kids;
    for(iter = children.begin(); iter != children.end(); iter++) {
      if(child_weights.insert(weights[(*iter)]).second) {
        kids[weights[(*iter)]] = *iter;
      }
    }

    if(child_weights.size() != 1 && child_weights.size() != 0) {

      int total_weight = 0;
      int first_val = 0, second_val = 0;
      bool first = true;
      for(int weight: child_weights) {
        if(first) {
          first_val = weight;
          first = false;
        } else {
          second_val = weight;
        }
      }
      total_diff = abs(first_val - second_val);
      unbalanced_child = kids[second_val];
    }
  }

  cout << "PART2: " << individual_weights[unbalanced_child] - total_diff << endl;
}


int parse_input(char *input_file, vector<Disc *> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  const regex rgx("([0-9a-z]+)");

  while(getline(infile, line)) {
    int count = 0;

    string name;
    Disc *disc;
    for(std::sregex_iterator i = std::sregex_iterator(line.begin(), line.end(), rgx); i != std::sregex_iterator(); ++i) {
      smatch matches = *i;
      count++;
      if(count == 1) {
        name = matches.str();
      } else if(count == 2) {
        disc = new Disc(name, stoi(matches.str()));
      } else if(count > 2) {
        disc->push_child(matches.str());
      }
    }
    parsed_input.push_back(disc);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<Disc *> &parsed_input) {
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
  vector<Disc *> parsed_input;
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
