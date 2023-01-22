#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <vector>


using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

struct _Point_t {
  int x;
  int y;
};

typedef _Point_t Point_t;

//globals
vector<Point_t> DIR_TABLE = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
vector<Point_t> D_TABLE = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};


bool operator <(Point_t const& left, Point_t const& right) {
  return left.x < right.x || ( left.x == right.x && left.y < right.y );
}

int sum_adjacent(Point_t p, map<Point_t, int> &plane) {
  int sum = 0;


  for(auto d: DIR_TABLE) {
    sum += plane[{p.x + d.x, p.y + d.y}];
  }
  return sum;
}

int get_distance(Point_t p1, Point_t p2) {
  return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

void solve_p1(int target_square) {
  int distance;
  map <int, Point_t> plane;

  int x = 0, y = 0;
  int square = 1;
  int iter = 1;

  plane[square] = { x, y };

  while(square < target_square) {
    for(auto d: D_TABLE) {
      int loop_extra = (d.y == -1 || d.x == -1) ? 1:0;
      for(int i = 0; i < iter + loop_extra; i++) {
        x += d.x;
        y += d.y;
        plane[++square] = { x, y };
      }
    }
    iter += 2;
  }

  distance = get_distance(plane[1], plane[target_square]);

  cout << "PART1: " << distance << endl;
}


int find_first(int target_square) {
  map <Point_t, int> plane;

  int sum;
  int x = 0, y = 0;
  int iter = 1;

  plane[{ x, y }] = 1;

  while(1) {
    for(auto d: D_TABLE) {
      int loop_extra = (d.y == -1 || d.x == -1) ? 1:0;
      for(int i = 0; i < iter + loop_extra; i++) {
        x += d.x;
        y += d.y;
        sum = sum_adjacent({x, y}, plane);
        if(sum > target_square) {
          return sum;
        }
        plane[{x, y}] = sum;
      }
    }
    iter += 2;
  }
}

void solve_p2(int target_square) {
  cout << "PART2: " << find_first(target_square) << endl;
}


int parse_input(char *input_file, int *target_square) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  while(getline(infile, line)) {
    *target_square = stoi(line);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, int target_square) {
  if(p_arg == "p1") {
    solve_p1(target_square);
  } else if(p_arg == "p2") {
    solve_p2(target_square);
  } else if(p_arg == "both") {
    solve_p1(target_square);
    solve_p2(target_square);
  }
}


int main(int argc, char *argv[]) {
  int rc = EXIT_SUCCESS;
  int target_square;
  std::vector<std::string> cl_args = {"both", "p1", "p2"};

  if(argc != 3 || count(cl_args.begin(), cl_args.end(), argv[2]) <= 0) {
    cout << "Usage: ./day input.txt p1|p2|both" << endl;
    rc = EXIT_FAILURE;
    goto cleanup;
  }

  if(parse_input(argv[1], &target_square) == PARSE_FAILURE) {
    cout << "Issue parsing input, exiting" << endl;
    rc = EXIT_FAILURE;
    goto cleanup;
  }

  run_solutions(argv[2], target_square);

cleanup:
  return rc;
}
