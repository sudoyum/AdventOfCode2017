#ifndef _KNOTHASH_H
#define _KNOTHASH_H

#include <string>
#include <vector>

#define SIZE 256
#define NUM_ROUNDS 64

std::string hasher(std::string val);
std::string to_hex(int x);

void do_hash(std::vector<int> &list, std::vector<int> &lengths, int *pcurr_pos, int *pskip_size);
#endif
