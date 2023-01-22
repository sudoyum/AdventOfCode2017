#include <iostream>
#include <vector>

#include "knothash.h"

using namespace std;


void do_hash(vector<int> &list, vector<int> &lengths, int *pcurr_pos, int *pskip_size) {
  int curr_pos = *pcurr_pos;
  int skip_size = *pskip_size;
  for(auto it = lengths.begin(); it != lengths.end(); ++it) {
    int length = *it;
    int end_index = (curr_pos + length - 1) % SIZE;
    int start = curr_pos;

    int last_start = start;
    int last_end = end_index;
    if(length <= SIZE && length >= 2) {
      while(1) {
        int temp = list[start];
        list[start] = list[end_index];
        list[end_index] = temp;
        start += 1;
        end_index -= 1;
        start %= SIZE;
        if(end_index < 0) {
          end_index = SIZE - 1;
        }
        if((last_start == end_index || last_end == start) || (end_index == start)) {
          break;
        }
        last_start = start;
        last_end = end_index;
      }
    }
    curr_pos += (length + skip_size++);
    curr_pos %= SIZE;
  }

  *pcurr_pos = curr_pos;
  *pskip_size = skip_size;
}


static char get_hex(int x) {
  if(x >=0 && x < 10) {
    return (char)(x + '0');
  } else if (x >= 10) {
    return (char)(x - 10 + 'a');
  } else {
    cout << "get_hex bad input" << endl;
    exit(1);
  }
}

// '0' == 48, 'a' == 97
string to_hex(int x) {
  char lower = get_hex(x & 0xF);
  char upper = get_hex((x & 0xF0) >> 4);
  string s;
  s.push_back(upper);
  s.push_back(lower);
  return s;
}


string hasher(string val) {
  vector<int> list;
  vector<int> lengths;

  for(int i = 0; i < SIZE; i++) {
    list.push_back(i);
  }

  for(const char &c: val) {
    lengths.push_back(int(c));
  }

  lengths.push_back(17);
  lengths.push_back(31);
  lengths.push_back(73);
  lengths.push_back(47);
  lengths.push_back(23);

  int curr_pos = 0;
  int skip_size = 0;

  for(int i = 0; i < NUM_ROUNDS; i++) {
    do_hash(list, lengths, &curr_pos, &skip_size);
  }

  vector<int> sparse_hash;
  for(int i = 0; i < SIZE; i+=16) {
    int xored = list[i];
    for(int j = i + 1; j < (i + 16); j++) {
      xored ^= list[j];
    }
    sparse_hash.push_back(xored);
  }
  string hash;
  for(auto it = sparse_hash.begin(); it != sparse_hash.end(); ++it) {
    hash += to_hex(*it);
  }
  return hash;
}
