#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

using namespace std;


#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define NUM_ITERATIONS_P1 5
#define NUM_ITERATIONS_P2 18


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


class FractalRule {
  public:
    void push_start(string start) { _start.push_back(start); }
    void push_end(string end) { _end.push_back(end); }
    void set_size(int size) { _size = size; }
    bool is_match(vector<vector<int>> segment);
    int get_size() { return _size; }
    void transform(void);
    vector<vector<int>> get_resulting() { return _resulting; }
  private:
    vector<string> _start;
    vector<string> _end;
    vector<vector<int>> _beginning;
    vector<vector<int>> _resulting;
    vector<vector<vector<int>>> _possibles;
    int _size;
};


void rotate_by_one(vector<vector<int>> &segment) {
  int row_size = segment[0].size();

  for(int j = 0; j < row_size/2; j++) {
    for(int i = 0; i < row_size - j - 1; i++) {
      int tmp = segment[j][i];
      segment[j][i] = segment[i][row_size-1-j];
      segment[i][row_size - 1 - j] = segment[row_size-1-j][row_size-1-i];
      segment[row_size-1-j][row_size - 1 - i] = segment[row_size-1-i][j];
      segment[row_size-1-i][j] = tmp;
    }
  }
}


void flip_right(vector<vector<int>> &segment) {
  int row_size = segment[0].size();
  vector<vector<int>> original = segment;
  for(int j = 0; j < row_size; j++) {
    for(int i = 0; i < row_size; i++) {
      segment[j][i] = original[j][row_size-1-i];
    }
  }
}


vector<vector<int>> transform_vector(vector<vector<int>> &start, int num_rotations, int flip) {
  vector<vector<int>> transformed_vector = start;

  for(int i = 0; i < num_rotations; i++) {
    rotate_by_one(transformed_vector);
  }

  if(flip == 1) {
    flip_right(transformed_vector);
  }

  return transformed_vector;
}


void FractalRule::transform(void) {
  _size = _start[0].size();

  for(int j = 0; j < _start[0].size(); j++) {
    vector<int> row;
    string rule_str = _start[j];
    for(int i = 0; i < rule_str.length(); i++) {
      rule_str[i] == '#' ? row.push_back(1): row.push_back(0);
    }
    _beginning.push_back(row);
  }

  for(int j = 0; j < _end[0].size(); j++) {
    vector<int> row;
    string rule_str = _end[j];
    for(int i = 0; i < rule_str.length(); i++) {
      rule_str[i] == '#' ? row.push_back(1): row.push_back(0);
    }
    _resulting.push_back(row);
  }

  if(_beginning.size() == 2) {
    _possibles.push_back(_beginning);
    _possibles.push_back(transform_vector(_beginning, 1, 0));
    _possibles.push_back(transform_vector(_beginning, 2, 0));
    _possibles.push_back(transform_vector(_beginning, 3, 0));
  } else {
    _possibles.push_back(_beginning);
    _possibles.push_back(transform_vector(_beginning, 0, 1));
    _possibles.push_back(transform_vector(_beginning, 1, 0));
    _possibles.push_back(transform_vector(_beginning, 2, 0));
    _possibles.push_back(transform_vector(_beginning, 3, 0));
    _possibles.push_back(transform_vector(_beginning, 1, 1));
    _possibles.push_back(transform_vector(_beginning, 2, 1));
    _possibles.push_back(transform_vector(_beginning, 3, 1));
  }
}


bool FractalRule::is_match(vector<vector<int>> segment) {
  vector<vector<vector<int>>>::iterator it = find(_possibles.begin(), _possibles.end(), segment);
  if(it != _possibles.end()) {
    return true;
  } else {
    return false;
  }
}


void print_plane(vector<vector<int>> &plane) {
  for(int j = 0; j < plane.size(); j++) {
    for(int i = 0; i < plane.size(); i++) {
      if(plane[j][i] == 1) {
        cout << "#";
      } else {
        cout << ".";
      }
    }
    cout << endl;
  }
  cout << endl;
}


int get_numpixels(vector<vector<int>> &plane) {
  int num_pixels = 0;
  for(int j = 0; j < plane.size(); j++) {
    for(int i = 0; i < plane[0].size(); i++) {
      if(plane[j][i] == 1) {
        num_pixels++;
      }
    }
  }
  return num_pixels;
}


vector<vector<int>> divide_by_two(vector<vector<int>> &plane, vector<FractalRule> &rules) {
  vector<vector<int>> new_plane;

  int total_size = plane[0].size() * plane[0].size();
  int row_len = plane[0].size();
  int x_start = 0, y_start = 0;

  vector<int> new_plane_row1;
  vector<int> new_plane_row2;
  vector<int> new_plane_row3;
  for(int z = 0; z < total_size/4; z++) {
    vector<vector<int>> segment;
    for(int j = y_start; j < y_start + 2; j++) {
      vector<int> row;
      for(int i = x_start; i < x_start + 2; i++) {
        row.push_back(plane[j][i]);
      }
      segment.push_back(row);
    }

    for(auto it = rules.begin(); it != rules.end(); ++it) {
      FractalRule rule = *it;
      if(rule.is_match(segment)) {
        vector<vector<int>> three_segment = rule.get_resulting();
        for(int i = 0; i < 3; i++) {
          new_plane_row1.push_back(three_segment[0][i]);
          new_plane_row2.push_back(three_segment[1][i]);
          new_plane_row3.push_back(three_segment[2][i]);
        }
      }
    }

    x_start += 2;
    x_start %= row_len;
    if(x_start == 0) {
      y_start += 2;
      new_plane.push_back(new_plane_row1);
      new_plane.push_back(new_plane_row2);
      new_plane.push_back(new_plane_row3);
      new_plane_row1.clear();
      new_plane_row2.clear();
      new_plane_row3.clear();
    }
  }

  return new_plane;
}


vector<vector<int>> divide_by_three(vector<vector<int>> &plane, vector<FractalRule> &rules) {
  vector<vector<int>> new_plane;

  int total_size = plane[0].size() * plane[0].size();
  int row_len = plane[0].size();
  int x_start = 0, y_start = 0;

  vector<int> new_plane_row1;
  vector<int> new_plane_row2;
  vector<int> new_plane_row3;
  vector<int> new_plane_row4;
  for(int z = 0; z < total_size/9; z++) {
    vector<vector<int>> segment;
    for(int j = y_start; j < y_start + 3; j++) {
      vector<int> row;
      for(int i = x_start; i < x_start + 3; i++) {
        row.push_back(plane[j][i]);
      }
      segment.push_back(row);
    }

    for(auto it = rules.begin(); it != rules.end(); ++it) {
      FractalRule rule = *it;
      if(rule.is_match(segment)) {
        vector<vector<int>> four_segment = rule.get_resulting();
        for(int i = 0; i < 4; i++) {
          new_plane_row1.push_back(four_segment[0][i]);
          new_plane_row2.push_back(four_segment[1][i]);
          new_plane_row3.push_back(four_segment[2][i]);
          new_plane_row4.push_back(four_segment[3][i]);
        }
      }
    }

    x_start += 3;
    x_start %= row_len;
    if(x_start == 0) {
      y_start += 3;
      new_plane.push_back(new_plane_row1);
      new_plane.push_back(new_plane_row2);
      new_plane.push_back(new_plane_row3);
      new_plane.push_back(new_plane_row4);
      new_plane_row1.clear();
      new_plane_row2.clear();
      new_plane_row3.clear();
      new_plane_row4.clear();
    }
  }
  return new_plane;
}


void solve_p1(vector<FractalRule> vals) {
  vector<vector<int>> plane { {0, 1, 0}, {0, 0, 1}, {1, 1, 1} };
  vector<FractalRule> two_rules;
  vector<FractalRule> three_rules;
  copy_if(vals.begin(), vals.end(), std::back_inserter(two_rules), [](FractalRule i){return i.get_size() == 2;} );
  copy_if(vals.begin(), vals.end(), std::back_inserter(three_rules), [](FractalRule i){return i.get_size() == 3;} );

  for(int i = 0; i < NUM_ITERATIONS_P1; i++) {
    if(plane.size() % 2 == 0) {
      plane = divide_by_two(plane, two_rules);
    } else {
      plane = divide_by_three(plane, three_rules);
    }
  }

  cout << "PART1: " << get_numpixels(plane) << endl;
}


void solve_p2(vector<FractalRule> vals) {
  vector<vector<int>> plane { {0, 1, 0}, {0, 0, 1}, {1, 1, 1} };
  vector<FractalRule> two_rules;
  vector<FractalRule> three_rules;
  copy_if(vals.begin(), vals.end(), std::back_inserter(two_rules), [](FractalRule i){return i.get_size() == 2;} );
  copy_if(vals.begin(), vals.end(), std::back_inserter(three_rules), [](FractalRule i){return i.get_size() == 3;} );

  for(int i = 0; i < NUM_ITERATIONS_P2; i++) {
    if(plane.size() % 2 == 0) {
      plane = divide_by_two(plane, two_rules);
    } else {
      plane = divide_by_three(plane, three_rules);
    }
  }
  cout << "PART2: " << get_numpixels(plane) << endl;
}


int parse_input(char *input_file, vector<FractalRule> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  vector<string> split_line;
  while(getline(infile, line)) {
    split_line = splitString(line, " => ");
    bool start = true;
    FractalRule fr;
    for(auto it = split_line.begin(); it != split_line.end(); ++it) {
      vector<string> rule = splitString(*it, "/");
      for(auto it2 = rule.begin(); it2 != rule.end(); ++it2) {
        if(start) {
          fr.push_start(*it2);
        } else {
          fr.push_end(*it2);
        }
      }
      start = false;
    }
    fr.transform();
    parsed_input.push_back(fr);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<FractalRule> &parsed_input) {
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
  vector <FractalRule> parsed_input;
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
