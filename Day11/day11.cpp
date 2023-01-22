#include <fstream>
#include <iostream>
#include <regex>
#include <vector>


#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1


using namespace std;


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


//https://www.redblobgames.com/grids/hexagons/#neighbors
class Axial {
    int q, r, s;
    int x, y, z;
  public:
    Axial() {  q = 0; r = 0; s = 0; };
    int get_q() { return q; }
    int get_r() { return r; }
    int get_s() { return s; }
    void do_nw() { q -= 1; }
    void do_n()  { r -= 1; }
    void do_ne() { q += 1; r -= 1; }
    void do_sw() { q -= 1; r += 1; }
    void do_s()  { r += 1; }
    void do_se() { q += 1; }
    void axial_to_cube();
    long long distance();
};


void Axial::axial_to_cube() {
  x = q;
  z = r;
  y = -x - z;
}

long long Axial::distance() {
  return (abs(q) + abs(q + r) + abs(r))/2;
}


void solve_p1(vector<string> vals) {
  Axial grid;
  for(auto it = vals.begin(); it != vals.end(); ++it) {
    if(*it == "nw") {
      grid.do_nw();
    } else if(*it == "n") {
      grid.do_n();
    } else if(*it == "ne") {
      grid.do_ne();
    } else if(*it == "sw") {
      grid.do_sw();
    } else if(*it == "s") {
      grid.do_s();
    } else if(*it == "se") {
      grid.do_se();
    }
  }

  cout << "PART1: " << grid.distance() << endl;
}


void solve_p2(vector<string> vals) {
  long long max_distance = 0;
  Axial grid;
  for(auto it = vals.begin(); it != vals.end(); ++it) {
    if(*it == "nw") {
      grid.do_nw();
    } else if(*it == "n") {
      grid.do_n();
    } else if(*it == "ne") {
      grid.do_ne();
    } else if(*it == "sw") {
      grid.do_sw();
    } else if(*it == "s") {
      grid.do_s();
    } else if(*it == "se") {
      grid.do_se();
    }
    if(grid.distance() > max_distance) {
      max_distance = grid.distance();
    }
  }
  cout << "PART2: " << max_distance << endl;
}


int parse_input(char *input_file, vector<string> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  vector<string> split_line;
  while(getline(infile, line)) {
    split_line = splitString(line, ",");
  }
  parsed_input = split_line;

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
  vector<string> parsed_input;
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
