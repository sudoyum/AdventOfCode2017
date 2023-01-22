#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <vector>


using namespace std;


#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1


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


class Node {
  private:
    string id;
    vector<string> children;
  public:
    Node(string);
    void add_child(string);
    vector<string> get_children() { return children; }
    string get_id() { return id; }
};


Node::Node(string _id) {
  id = _id;
}


void Node::add_child(string child) {
  children.push_back(child);
}


Node get_by_id(vector<Node> &vals, string id) {
  for(auto it = vals.begin(); it != vals.end(); ++it) {
    if(it->get_id() == id) {
      return *it;
    }
  }
  cout << "couldn't find node, exiting" << endl;
  exit(1);
}


list<string> get_group_items(vector<Node> &vals, string start_id) {
  Node start = get_by_id(vals, start_id);
  vector<string> children = start.get_children();
  list<string> node_list;
  list<string> seen_list;

  for(int i = 0; i < children.size(); i++) {
    node_list.push_back(children[i]);
  }

  seen_list.push_back(start_id);

  while(!node_list.empty()) {
    string node_id = node_list.front();
    node_list.pop_front();
    seen_list.push_back(node_id);
    vector<string> children = get_by_id(vals, node_id).get_children();
    for(int i = 0; i < children.size(); i++) {
      if(find(seen_list.begin(), seen_list.end(), children[i]) == seen_list.end() &&
          find(node_list.begin(), node_list.end(), children[i]) == node_list.end()) {
        node_list.push_back(children[i]);
      }
    }
  }
  return seen_list;
}


void solve_p1(vector<Node> &vals) {
  cout << "PART1: " << get_group_items(vals, "0").size() << endl;
}


void solve_p2(vector<Node> &vals) {
  vector<list<string>> groups;
  groups.push_back(get_group_items(vals, "0"));

  for(int i = 0; i < vals.size(); i++) {
    bool found = false;
    for(int j = 0; j < groups.size(); j++) {
      list<string> search_list = groups[j];
      for(auto it = search_list.begin(); it != search_list.end(); ++it) {
        if(vals[i].get_id() == *it) {
          found = true;
          break;
        }
      }
    }
    if(!found) {
      groups.push_back(get_group_items(vals, vals[i].get_id()));
    }
  }
  cout << "PART2: " << groups.size() << endl;
}


void parse_input(char *input_file, char *part_num) {
  string line;
  ifstream infile(input_file);
  vector<Node> input_vals;

  if(!infile.is_open()) {
    cout << "file not successfully opened" << endl;
    return;
  }

  const regex rgx("([0-9]+) <-> ([,\\s0-9$]+)*");
  const regex rgx2("([0-9]+)");
  smatch matches;

  while(getline(infile, line)) {
    sregex_iterator it = sregex_iterator(line.begin(), line.end(), rgx);
    for(; it != sregex_iterator(); ++it) {
      smatch matches = *it;
      Node node(matches.str(1));
      vector<string> split_line = splitString(matches.str(2), ", ");
      for(int i = 0; i < split_line.size(); i++) {
        node.add_child(split_line[i]);
      }
      input_vals.push_back(node);
    }
  }

  string p_arg(part_num);
  if(p_arg  == "p1") {
    solve_p1(input_vals);
  } else if(p_arg  == "p2") {
    solve_p2(input_vals);
  } else if(p_arg  == "both") {
    solve_p1(input_vals);
    solve_p2(input_vals);
  } else {
    cout << "./puzzle input.txt p1|p2|both" << endl;
    return;
  }
}


int parse_input(char *input_file, vector<Node> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  const regex rgx("([0-9]+) <-> ([,\\s0-9$]+)*");
  const regex rgx2("([0-9]+)");
  smatch matches;

  while(getline(infile, line)) {
    sregex_iterator it = sregex_iterator(line.begin(), line.end(), rgx);
    for(; it != sregex_iterator(); ++it) {
      smatch matches = *it;
      Node node(matches.str(1));
      vector<string> split_line = splitString(matches.str(2), ", ");
      for(int i = 0; i < split_line.size(); i++) {
        node.add_child(split_line[i]);
      }
      parsed_input.push_back(node);
    }
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<Node> &parsed_input) {
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
  vector<Node> parsed_input;
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
