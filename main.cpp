/*
 *  Student:     Michael Kim
 *  Class:       CSS 449 
 *  Assignment:  Project 3
 *  Date:        5/29/2026
 */

#include "BipartiteMatcher.h"

#include <iostream>
#include <climits>
#include <fstream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// main(): Our program's entry point.
int main() {
    // filename can be specified in the below parameter argument
    BipartiteMatcher matcher("program3data.txt");
    matcher.solve();
    // exit program
    return 0;
}
