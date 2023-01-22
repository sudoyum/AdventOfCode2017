#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <vector>


using namespace std;

#define PARSE_SUCCESS 0
#define PARSE_FAILURE -1

#define LOOP_COUNT 50000


struct Particle {
  int index;
  long long prev_distance;
  long long ax, ay, az;
  long long vx, vy, vz;
  long long px, py, pz;
  bool destroyed;
};

typedef Particle Particle_t;

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


// Manhattan distance to origin
long long manhattan_distance(Particle_t *p) {
  return abs(p->px) + abs(p->py) + abs(p->pz);
}


void update_particle(Particle_t *p) {
  p->vx += p->ax;
  p->vy += p->ay;
  p->vz += p->az;
  p->px += p->vx;
  p->py += p->vy;
  p->pz += p->vz;
}


bool compare_position(Particle_t *a, Particle *b) {
  return a->px == b->px && a->py == b->py && a->pz == b->pz;
}


void solve_p1(vector<Particle_t> vals) {
  int count = 0;
  while(count < LOOP_COUNT) {
    for(int i = 0; i < vals.size(); i++) {
      Particle_t *particle = &vals[i];
      update_particle(particle);
      particle->prev_distance = manhattan_distance(particle);
    }
    count++;
  }

  auto min_distance = minmax_element(vals.begin(), vals.end(),
      [] (Particle_t const& lhs, Particle_t const& rhs) {return lhs.prev_distance < rhs.prev_distance;});


  cout << "PART1: " << min_distance.first->index << endl;
}


void solve_p2(vector<Particle_t> vals) {
  int count = 0;
  while(count < LOOP_COUNT) {
    for(int i = 0; i < vals.size(); i++) {
      Particle_t *particle = &vals[i];
      update_particle(particle);
      particle->prev_distance = manhattan_distance(particle);
    }

    for(int i = 0; i < vals.size(); i++) {
      for(int j = i + 1; j < vals.size(); j++) {
        if(compare_position(&vals[i], &vals[j])) {
          vals[i].destroyed = true;
          vals[j].destroyed = true;
        }
      }
    }
    count++;
  }

  count = 0;
  for(auto it = vals.begin(); it != vals.end(); it++) {
    if(!it->destroyed) {
      count++;
    }
  }

  cout << "PART2: " << count << endl;
}


int parse_input(char *input_file, vector<Particle_t> &parsed_input) {
  string line;
  ifstream infile(input_file);

  if(!infile.is_open()) {
    cout << "File: " << input_file << " not successfully opened" << endl;
    return PARSE_FAILURE;
  }

  vector<string> split_line;
  int count = 0;
  const regex rgx("([pva])=<([-0-9]+),([-0-9]+),([-0-9]+)>");
  while(getline(infile, line)) {
    Particle_t particle = { 0 };
    particle.index = count++;
    particle.prev_distance = 0x7FFFFFFF;
    particle.destroyed = false;
    split_line = splitString(line, ",\\s+");
    for(auto it = split_line.begin(); it != split_line.end(); it++) {
      smatch matches;
      if(regex_search(*it, matches, rgx)) {
        if(matches[1] == "p") {
          particle.px = stoi(matches[2]);
          particle.py = stoi(matches[3]);
          particle.pz = stoi(matches[4]);
        } else if(matches[1] == "v") {
          particle.vx = stoi(matches[2]);
          particle.vy = stoi(matches[3]);
          particle.vz = stoi(matches[4]);
        } else if(matches[1] == "a") {
          particle.ax = stoi(matches[2]);
          particle.ay = stoi(matches[3]);
          particle.az = stoi(matches[4]);
        }
      } else {
        cout << "Issue, no regex match" << endl;
        return PARSE_FAILURE;
      }
    }
    parsed_input.push_back(particle);
  }

  return PARSE_SUCCESS;
}


void run_solutions(string p_arg, vector<Particle_t> &parsed_input) {
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
  vector <Particle_t> parsed_input;
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
