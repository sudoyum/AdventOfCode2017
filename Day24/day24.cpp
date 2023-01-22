#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <tuple>
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


void print_bridges(vector<vector<tuple<int, int>>> &bridges) {
  for(int i = 0; i < bridges.size(); i++) {
    for(int j = 0; j < bridges[i].size(); j++) {
      cout << get<0>(bridges[i][j]) << "/" << get<1>(bridges[i][j]) << "--" << endl;
    }
    cout << endl;
  }
}


int total_strength(vector<tuple<int, int>> &bridge) {
  int total_strength = 0;
  for(int i = 0; i < bridge.size(); i++) {
    total_strength += (get<0>(bridge[i]) + get<1>(bridge[i]));
  }
  return total_strength;
}


vector<tuple<int, int>> get_possibles(int end_item, vector<tuple<int, int>> remaining_list) {
  vector<tuple<int, int>> possibles;
  for(int i = 0; i < remaining_list.size(); i++) {
    if(get<0>(remaining_list[i]) == end_item || get<1>(remaining_list[i]) == end_item) {
      possibles.push_back(remaining_list[i]);
    }
  }
  return possibles;
}


int get_end_item(vector<tuple<int, int>> &bridge) {
  if(bridge.size() == 1) {
    return get<1>(bridge[0]);
  } else if(get<0>(bridge.end()[-2]) == get<0>(bridge.back())) {
    return get<1>(bridge.back());
  } else if(get<0>(bridge.end()[-2]) == get<1>(bridge.back())) {
    return get<0>(bridge.back());
  } else if(get<1>(bridge.end()[-2]) == get<0>(bridge.back())) {
    return get<1>(bridge.back());
  } else if(get<1>(bridge.end()[-2]) == get<1>(bridge.back())) {
    return get<0>(bridge.back());
  }

  cout << "get_end_item: Issue" << endl;
  exit(1);
}


int calc_strongest(vector<vector<tuple<int, int>>> &all_bridges) {
  int strongest = -1;
  for(int i = 0; i < all_bridges.size(); i++) {
    if(total_strength(all_bridges[i]) > strongest) {
      strongest = total_strength(all_bridges[i]);
    }
  }
  return strongest;
}


int strength_of_longest(vector<vector<tuple<int, int>>> &all_bridges) {
  int largest_len = 0;
  int index = 0;
  for(int i = 0; i < all_bridges.size(); i++) {
    if(all_bridges[i].size() > largest_len) {
      largest_len = all_bridges[i].size();
      index = i;
    }
  }
  return total_strength(all_bridges[index]);
}

vector<vector<tuple<int, int>>> generate_all_bridges(vector<tuple<int, int>> &parsed_input) {
  vector<vector<tuple<int, int>>> all_bridges;
  vector<vector<tuple<int, int>>> bridges;
  for(int i = 0; i < parsed_input.size(); i++) {
    if(get<0>(parsed_input[i]) == 0) {
      vector<tuple<int, int>> start;
      start.push_back(parsed_input[i]);
      bridges.push_back(start);
      parsed_input.erase(parsed_input.begin() + i);
    }
  }

  vector <vector<tuple<int, int>>> remaining_lists;
  for(int i = 0; i < bridges.size(); i++) {
    remaining_lists.push_back(parsed_input);
  }

  all_bridges = bridges;

  while(true) {
    int num_found = 0;

    vector <vector<tuple<int, int>>> new_remaining_lists;
    vector<vector<tuple<int, int>>> new_bridges;
    for(int i = 0; i < bridges.size(); i++) {
      vector<tuple<int, int>> possibles = get_possibles(get_end_item(bridges[i]), remaining_lists[i]);
      for(int j = 0; j < possibles.size(); j++) {
        vector<tuple<int, int>> new_row = bridges[i];
        new_row.push_back(possibles[j]);
        new_bridges.push_back(new_row);
        vector<tuple<int, int>> new_list = remaining_lists[i];
        auto it = find(new_list.begin(), new_list.end(), possibles[j]);
        new_list.erase(it);
        new_remaining_lists.push_back(new_list);
        num_found++;
      }
    }
    all_bridges.insert(all_bridges.end(), new_bridges.begin(), new_bridges.end());
    bridges = new_bridges;
    remaining_lists = new_remaining_lists;

    if(num_found == 0) {
      break;
    }
  }
  return all_bridges;
}


void solve_p1(vector<vector<tuple<int, int>>> &all_bridges) {
  cout << "PART1: " << calc_strongest(all_bridges) << endl;
}


void solve_p2(vector<vector<tuple<int, int>>> &all_bridges) {
  cout << "PART2: " << strength_of_longest(all_bridges) << endl;
}


int parse_input(char *input_file, vector<tuple<int, int>> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    vector<string> split_line = splitString(line, "/");
    parsed_input.push_back(make_tuple(stoi(split_line[0]), stoi(split_line[1])));
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<tuple<int, int>> &parsed_input) {

  vector<vector<tuple<int, int>>> all_bridges = generate_all_bridges(parsed_input);

  if(p_arg == "p1") {
    solve_p1(all_bridges);
  } else if(p_arg == "p2") {
    solve_p2(all_bridges);
  } else if(p_arg == "both") {
    solve_p1(all_bridges);
    solve_p2(all_bridges);
  }
}


int main(int argc, char *argv[]) {
  int rc = EXIT_SUCCESS;
  vector<tuple<int, int>> parsed_input;
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
