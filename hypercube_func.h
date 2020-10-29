#ifndef CUBE_H
#define CUBE_H


#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <vector>
#include <cstring>
#include <fstream>
#include <byteswap.h>
#include <climits>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>
#include <bits/stdc++.h> 
#include <bitset>
#include <chrono> 

#include "util.h"
#include "hash.h"


vector <neighbor> knn_hypercube(hypercube_HT* ht,image query, int k,int limit,int probes);
vector <unsigned int> range_search(hypercube_HT* ht,image query, double r, int c, int limit, int probes);

#endif
