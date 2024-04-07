//
//  main.cpp
//  PA2
//
//  Created by Bryce Blankinship on 4/6/24.
//

#include "parser.h"
#include "lex.h"
#include <iostream>
#include <cctype>

using namespace std;

int syntaxErrors = 0;

bool Prog(istream& in, int& line) {
  bool programFound = false;
  bool endProgramFound = false;

  while (true) {
    LexItem tok = getNextToken(in, line);
    if (tok == DONE)
      break;
    if (tok.GetToken() == PROGRAM) {
      programFound = true;
      LexItem program_name = getNextToken(in, line);
      if (program_name.GetToken() != IDENT) {
        cout << line << ": Program identifier expected." << endl;
        syntaxErrors++;
        return false;
      }
      while (true) {
        LexItem decl_token = getNextToken(in, line);
        if (decl_token.GetToken() == PRINT) {
            /**
             PROGRAM circle
                 !Variable declarations and simple if statement
                 real :: r, a, p, b
                 character :: str1, str2
                 if ( r == 5)
                   print *, str1, str2
             END PROGRAM circle
             
             returns:
             Print statement in a Simple If statement.
             (DONE)
             Successful Parsing
             */
            LexItem next_tok = getNextToken(in, line);
            cout << next_tok << endl;
        } else if (decl_token.GetToken() == END) {
          endProgramFound = true;
          LexItem end_program = getNextToken(in, line);
          if (end_program.GetToken() != PROGRAM) {
            cout << line << ": 'END PROGRAM' expected." << endl;
            syntaxErrors++;
            return false;
          }
          LexItem program_name_end = getNextToken(in, line);
          if (program_name_end.GetToken() != IDENT || program_name_end.GetLexeme() != program_name.GetLexeme()) {
            cout << line << ": Program name mismatch." << endl;
            syntaxErrors++;
            return false;
          }
          cout << "(DONE)" << endl;
          return true;
        } else if (decl_token.GetToken() == REAL ||
                   decl_token.GetToken() == INTEGER ||
                   decl_token.GetToken() == CHARACTER) {
          LexItem var_type = decl_token;
          LexItem colon = getNextToken(in, line);
          if (colon != DCOLON) {
            cout << line << ": Missing Double Colons" << endl;
            syntaxErrors++;
            return false;
          }
          while (true) {
            LexItem var = getNextToken(in, line);
            if (var.GetToken() != IDENT) {
              cout << line << ": Variable name expected." << endl;
              syntaxErrors++;
              return false;
            }
            LexItem comma_or_end = getNextToken(in, line);
            if (comma_or_end == COMMA) {
              continue;
            } else if (comma_or_end == END || (comma_or_end.GetToken() == IF && getNextToken(in, line).GetToken() != IDENT)) {
              break;
            } else {
              cout << line << ": Missing COMMA after variable declaration." << endl;
              syntaxErrors++;
              return false;
            }
          }
        }
      }
    }
  }

  if (!programFound) {
    cout << line << ": Missing PROGRAM statement." << endl;
    syntaxErrors++;
    return false;
  }

  if (!endProgramFound) {
    cout << line << ": Missing END of Program" << endl;
    syntaxErrors++;
    return false;
  }

  cout << "(DONE)" << endl;
  return true;
}

int ErrCount() {
    return syntaxErrors;
}
