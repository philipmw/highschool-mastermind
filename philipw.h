// See philipw.cpp for last modification information.
#ifndef _PHILIP_H
#define _PHILIP_H

#ifdef __MSDOS__
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <dos.h>
#include <conio.h>
const int true = 1, false = 0;
#else
#include <iostream>
#include <string>
using namespace std;
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#endif

#include <stdio.h>
#include <time.h>

#define SCREEN_LINE 80

// Converts an integer to an string. (written by D. Nasir)
string int_string(int);
// Converts a double to an string.
string double_string(double, unsigned int);
// Converts a string into an integer and saves to the last parameter.
// 0 upon success
int string_int(const char [], const int, int &);
int string_int(const char [], const int, unsigned int &);
// Converts a string into a double and saves to the last parameter.  Returns true upon success.
int string_double(const char [], const int, double &);
// Centers a string.
string center(const string, const int);
// Aligns a string to the right.
string right(const string, const int);
// Upper-cases a letter.
void upper(char &);
// Alerts the operator with a message and keeps track of errors
int alert(const string, const bool);
// Moves a file from source to destination; returns -1 if unsuccessful.
int move_file(const string, const string);
// Print two strings on the same line, spaced out over the entire line.
string spread(const string, const string, const unsigned int);
// Gets a single-digit integer with a given range, with error-checking.
//int get_integer(const, const int);
// Prompts for either a 'Y'es or 'N'o, and returns 1 or 0, respectively.
bool prompt_yn(const int);
// Returns a believable and a persuasive excuse for one of my numerous mistakes.
string my_excuse();
// Prints a progress bar on the screen
void progress_bar(const int, const int);
// Pluralizes a string if necessary
string pluralize(string, const int);
// Adds an apostrophe and an optional 's' to the end of a string
string possessivize(string);
// Rounds doubles properly (up if >= .5, else down) and converts them to an integer
int double_to_int(const double);
// Cut off various whitespaces from start and end of the string, then return it
string trim_whitespace(string);
// Capitalizes every letter of a given string and returns it
string capitalize(string);
// Pads the string to a given length with a user-defined char, aligns to the right
string pad_str_left(string, const unsigned int, const char);
// Pads the string to a given length with a user-defined char, aligns to the left
string pad_str_right(string, const unsigned int, const char);
// Returns a suffix string for a given number
string get_num_suffix(int);
// Converts a number grade to a letter grade (with a plus-minus)
string num2let_grade(const int);
// Returns the time in a custom format
string get_time(string);
// Evaluates an equation
//double eval_equation(string);

#endif
