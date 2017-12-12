/***********************************************************************************************
Copyright (C) 2010 Anvesh Komuravelli, Paolo Zuliani and Edmund M. Clarke.  All rights reserved.
By using this software the USER indicates that he or she has read, understood and will comply
with the following:

1. The USER is hereby granted non-exclusive permission to use, copy and/or
modify this software for internal, non-commercial, research purposes only. Any
distribution, including commercial sale or license, of this software, copies of
the software, its associated documentation and/or modifications of either is
strictly prohibited without the prior consent of the authors. Title to copyright
to this software and its associated documentation shall at all times remain with
the authors. Appropriated copyright notice shall be placed on all software
copies, and a complete copy of this notice shall be included in all copies of
the associated documentation. No right is granted to use in advertising,
publicity or otherwise any trademark, service mark, or the name of the authors.

2. This software and any associated documentation is provided "as is".

THE AUTHORS MAKE NO REPRESENTATIONS OR WARRANTIES, EXPRESSED OR IMPLIED,
INCLUDING THOSE OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT
USE OF THE SOFTWARE, MODIFICATIONS, OR ASSOCIATED DOCUMENTATION WILL NOT
INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS OR OTHER INTELLECTUAL PROPERTY
RIGHTS OF A THIRD PARTY.

The authors shall not be liable under any circumstances for any direct,
indirect, special, incidental, or consequential damages with respect to any
claim by USER or any third party on account of or arising from the use, or
inability to use, this software or its associated documentation, even if the
authors have been advised of the possibility of those damages.
***********************************************************************************************/


/**
 * test function which drives inputs to the Checker
 * needs two file inputs - first one contains a BLTL property in a single line
 *                       - second one contains the trace; the first line of this
 *                           file should be of the form
 *                            "# <signal 1> <signal 2> ..."
 *                           every other line should be of the form
 *                            "<time-stamp> <value of signal 1> <value of
 *                                signal 2> ..."
 *
 * while the actual Checker code needs the property as a separate file, the trace
 * is something specific to this test file and can of course, be generated online
 * (see interface.cpp in this module for more details)
 */

#include <ctime>
#include <fstream>
#include "defs.hpp"

#define MAX_LINE_LEN  50000	// max length of each line in the trace file
#define EXIT_SUCCESS_SAT    51
#define EXIT_SUCCESS_UNSAT  52


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
extern int yyparse (void *YYPARSE_PARAM);
#else
extern int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
extern int yyparse (void);
#else
extern int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

extern FILE *yyin;

double *obs;

void setIdArithmetic(const string &s, int id) {
  Arithmetic *a;
  for (vector<Arithmetic *>::iterator it = var.begin(); it != var.end(); it++) {
    a = (*it);
    if ((a -> var).compare(s) == 0) {
      a -> id = id;
    }
  }
}

int main(int argc, char **argv) {
  //vector<string> state_vars;
  string stok;
  char *line, *temp, *tok;
  double f;

  // time code
  vector<string> lines; // stores all the lines of the trace file
  string strline;
  clock_t ticks = 0, start;

  int numVars;

  if (argc != 3) {
    fprintf(stderr, "ERROR: Format is %s <property-file> <trace-file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  yyin = fopen(argv[1], "r");
  if (!yyin) {
    fprintf(stderr, "ERROR: Property file does not exist\n");
    exit(EXIT_FAILURE);
  }
  yyparse();
  fclose(yyin);

  line = (char *)calloc(sizeof(char), MAX_LINE_LEN);

  // open trace file 
  ifstream input(argv[2]);
  if (!input.is_open()) {
    cerr << "cannot open trace file: " << argv[2] << endl;
    exit(EXIT_FAILURE);
  }
  
  // read the first line
  getline(input, strline);
  string::traits_type::copy(line, strline.c_str(), strline.length() + 1);
  temp = line;
  tok = strtok(temp, " #\t\n");

  if (strcmp(tok, "time") != 0) {
    cerr << "incorrect format - first line of trace file" << endl;
    exit(EXIT_FAILURE);
  }
  for (numVars = 0;(tok = strtok(NULL, " #\t\n")) != NULL; numVars++) {
    stok = string(tok);
    setIdArithmetic(stok, numVars);
    //state_vars.push_back(string(tok));
  }

  // check whether all the variables in the property are in the trace file
  Arithmetic *a;
  for (vector<Arithmetic *>::iterator it = var.begin(); it != var.end(); it++) {
    a = (*it);
    if (a -> id == -1) {
      cerr << "Error : Variable " << a -> var << " is not in the trace file." << endl;
      exit(EXIT_FAILURE);
    }
  }

  result = UNDEF;

  future = new Block*[blockCount];
  current = new vector<Block *>;
  current_old = new vector<Block *>;
  current_size = 0;
  char *tmptok;
  unsigned long int numlines = 1;

  obs = new double[numVars];

  // read the trace file line by line
  while( getline(input, strline) ) {
    numlines++;
    string::traits_type::copy(line, strline.c_str(), strline.length() + 1);

    temp = line;
    tmptok = strtok(temp, " \t\n");
    if (tmptok == NULL) {
        cerr << "Error: cannot find timestamp in trace at line " << numlines << "." << endl;
        exit(EXIT_FAILURE);
    }
    ts_new = atof(tmptok);
    for (int i = 0; i < numVars; i++) {
      tmptok = strtok(NULL, " \t\n");
      if (tmptok == NULL) {
        cerr << "Error: wrong trace format at line " << numlines << "." << endl;
        exit(EXIT_FAILURE);
      }
      f = atof(tmptok);
      obs[i] = f;
    }

    //start = clock();

    advance();

    //ticks += (clock() - start);

    if (result == TRUE) {
      cout << "satisfiable!" << endl;
//      exit(EXIT_SUCCESS_SAT);
      exit(0);
    } else if (result == FALSE) {
      cout << "unsatisfiable!" << endl;
      exit(EXIT_SUCCESS_UNSAT);
    }
  }

  if (result == UNDEF) {
    cout << "cannot decide : insufficient trace!" << endl;
  }

  //double elapsedTime = ticks / (double)CLOCKS_PER_SEC;
  //cout << "Elapsed time: " << elapsedTime << " seconds" << endl;

  exit(EXIT_SUCCESS);
}
