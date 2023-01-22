#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define GRID_SPACE 0
#define GRID_DOWN 1
#define GRID_HORI 2
#define GRID_PLUS 3


class Pathing {
    int length;
    int width;
    int start_index;
    int iterations;
    vector<vector<int>> grid;
    string path;
  public:
    Pathing(vector<string>);
    int get_length() { return length; }
    int get_width() { return width; }
    void navigate();
    string get_path() { return path; }
    int get_iterations() { return iterations; }
};


Pathing::Pathing(vector<string> input_vals) {
  length = input_vals.size();
  int max = 0;
  for(const auto& line: input_vals) {
    if(line.length() > max) {
      max = line.length();
    }
  }
  width = max;
  grid.resize(length, vector<int>(width));

  int j = 0;
  for(const auto& line: input_vals) {
    int i = 0;
    for(; i < line.length(); i++) {
      if(line[i] == ' ') {
        grid[j][i] = GRID_SPACE;
      } else if(line[i] == '|') {
        if(j == 0) {
          start_index = i;
        }
        grid[j][i] = GRID_DOWN;
      } else if(line[i] == '-') {
        grid[j][i] = GRID_HORI;
      } else if(line[i] == '+') {
        grid[j][i] = GRID_PLUS;
      } else if(line[i] != '\n') {
        grid[j][i] = (int)line[i];
      }
    }
    while(i < width) {
      grid[j][++i] = GRID_SPACE;
    }
    j++;
  }
}


void Pathing::navigate() {
  int curr_y = 0;
  int curr_x = start_index;
  int dy = 1, dx = 0;
  iterations = 1;
  while(1) {
    iterations++;
    curr_x += dx;
    curr_y += dy;
    if(curr_x >= width || curr_y >= length) {
      cout << "breaking here" << endl;
      break;
    }

    if(grid[curr_y][curr_x] == GRID_PLUS) {
      bool found = false;
      if(dx == -1 || dx == 1) {
        if(curr_y + 1 < length) {
          if(grid[curr_y + 1][curr_x] == GRID_DOWN || isalpha(grid[curr_y + 1][curr_x])) {
            found = true;
            dx = 0;
            dy = 1;
          }
        }
        if(curr_y - 1 >= 0) {
          if(grid[curr_y - 1][curr_x] == GRID_DOWN || isalpha(grid[curr_y - 1][curr_x])) {
            found = true;
            dx = 0;
            dy = -1;
          }
        }
      } else if(dy == -1 || dy == 1) {
        if(curr_x + 1 < width) {
          if(grid[curr_y][curr_x + 1] == GRID_HORI || isalpha(grid[curr_y][curr_x + 1])) {
            found = true;
            dx = 1;
            dy = 0;
          }
        }
        if(curr_x - 1 >= 0) {
          if(grid[curr_y][curr_x - 1] == GRID_HORI || isalpha(grid[curr_y][curr_x - 1])) {
            found = true;
            dx = -1;
            dy = 0;
          }
        }

      }
      if(!found) {
        break;
      }
    } else if(isalpha(grid[curr_y][curr_x])) {
      if(string::npos != path.find(grid[curr_y][curr_x])) {
        break;
      }
      path.push_back(grid[curr_y][curr_x]);
      if(curr_x + dx >= width || curr_y + dy >=length || curr_x + dx < 0 || curr_y + dy < 0) {
        break;
      }
      // PART 2
      if(path == "XYFDJNRCQA") {
        break;
      }
    }
  }
}


void solve_p1(vector<string> vals) {
  Pathing path(vals);
  path.navigate();

  cout << "PART1: " << path.get_path() << endl;
}


void solve_p2(vector<string> vals) {
  Pathing path(vals);
  path.navigate();
  cout << "PART2: " << path.get_iterations() << endl;
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
  vector <string> parsed_input;
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
