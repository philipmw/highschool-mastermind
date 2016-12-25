// Programmers: Philip M. White (pwhite@mailhaven.com) and Clinton A. Nicholls III (novathestarlightknight@yahoo.com)
// Requires GCC 3.x
// Released under the GPL license.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <time.h>
#include <string>
#include "philipw.h"
using namespace std;

const bool DEBUG = 0;

#define VERSION "0.3"
//#define BUILDDATE "20021122"
const unsigned int MIN_SLOTS = 4;
const unsigned int MAX_SLOTS = 12;
const unsigned int MIN_COLORS = 2;
const unsigned int MAX_COLORS = 9;
const unsigned int MIN_BOARD = 3;
const unsigned int MAX_BOARD = 40;
const unsigned int MAX_MINUTES = 120;
const string LOGFILE = "~/mastermind.log";

typedef struct Board {
	unsigned int slots, colors, size, moves;
	unsigned int compseq[MAX_SLOTS];
        bool allow_multi, allow_blanks;
};

typedef struct Game {
	unsigned int duration, secs_allowed;
	time_t starttime, endtime;
};

void print_intro()
{
	cout << "MasterMind " << VERSION << "\t\t\t\t\tbuild " << BUILDDATE << endl;
	cout << "Written by Philip M. White and Clinton A. Nicholls III" << endl;
	cout << "Source code is available at: http://pmw.myip.org/~pwhite/programs/mastermind.cpp" << endl;
	cout << "Licensed under the GNU General Public License v2" << endl << endl;
	cout << "The log file is saved to: " << LOGFILE << endl;
}

void write_logs(const Game game, const Board board)
{
	ofstream outfile;
	
	outfile.open(LOGFILE.c_str(), ios::app);
	if (outfile.fail()) {
		cout << "MasterMind is unable to open " << LOGFILE << " for writing, so it cannot save a log of this game." << endl;
		return;
	}
	outfile << endl << get_time("Y--d H:i:s") << '\t';
	outfile	<< "boardsize:" << setw(2) << board.size << " slots:" << setw(2) << board.slots << " colors:" << board.colors << " moves:" << setw(2) << board.moves << " secs:" << game.duration << " secs allowed:" << game.secs_allowed << endl;
	outfile << "Status: " << (board.moves != 0 ? "won " : "lost");
	outfile << "\tCombination: ";
	for (unsigned int i=0; i < board.slots; i++)
	{
		outfile << board.compseq[i];
		if (i != board.slots-1)
			outfile << ' ';
	}
	outfile << endl;
	outfile.close();
}

void collect_data(Game &game, Board &board)
{
	char choice;

	do
	{
		cout << "How many slots would you like? (" << MIN_SLOTS << "-" << MAX_SLOTS << "): ";
		cin >> board.slots;
	} while (board.slots < MIN_SLOTS || board.slots > MAX_SLOTS);
	
	do
	{
		cout << "How many possible colors would you like? (" << MIN_COLORS << "-" << MAX_COLORS << "): ";
		cin >> board.colors;
	} while (board.colors < MIN_COLORS || board.colors > MAX_COLORS);

	do
	{
		cout << "Would you like the computer to generate a sequence or a partner? (c/p): ";
		cin >> choice;
	} while (choice != 'c' && choice != 'p');
	getline();
	
	if (board.colors >= board.slots && choice == 'c')
	{
		cout << "Allow multiples of a color?";
		board.allow_multi = prompt_yn(1);
	}
	else
		board.allow_multi = true;

	cout << "Allow input of blanks?";
	board.allow_blanks = prompt_yn(-1);
	

	do
	{
		cout << "How many moves can the board handle? (" << MIN_BOARD << "-" << MAX_BOARD << "): ";
		cin >> board.size;
	} while (board.size < MIN_BOARD || board.size > MAX_BOARD);
	do
	{
		cout << "Enter the max game duration in minutes, or zero for unlimited (0-" << MAX_MINUTES << "): ";
		cin >> game.secs_allowed;
	} while (game.secs_allowed < 0 || game.secs_allowed > MAX_MINUTES); // not real values here...
	game.secs_allowed *= 60;
		
	if (choice == 'c')
	{
		bool used_colors[MAX_COLORS] = {0};
		if (DEBUG)
			cout << "Computed sequence: ";
		for (unsigned int i=0; i < board.slots; i++)
		{
			int tempcolor;
			do
			{
				tempcolor = random()%board.colors+1;
			} while (used_colors[tempcolor] == true);
			if (!board.allow_multi)
			{
				used_colors[tempcolor] = true;
			}
			board.compseq[i] = tempcolor;
			if (DEBUG)
				cout << board.compseq[i] << ' ';
		}
		if (DEBUG)
			cout << endl;
	}
	else
	{
		bool error;
		do
		{
			error = false;
			cout << "Enter " << board.slots << " digits (1 to " << board.colors << "): " << endl;
			for (unsigned int i=0; i < board.slots; i++)
			{
				cin >> board.compseq[i];
				if (board.compseq[i] > board.colors)
					error = true;
			}
		} while (error);
		for (int i=0; i < 50; i++) {
			cout << endl;	// to semi-clear the terminal
		}
	}
}

// Postcondition: returns index value of needle, else -1 if not found.
int search_seq(const unsigned int haystack[], const unsigned int needle, const bool ignorelist[], const unsigned int haysize)
{
	int found=-1;
	for (unsigned int i=0; i < haysize; i++)
	{
		if (haystack[i] == needle && ignorelist[i] == false)
			found = i;
	}
	if (found != -1)
	{
		return found;
	}
	return -1;
}

void print_results(const unsigned int blacks, const unsigned int whites, const unsigned int slots)
{
	unsigned int lcv;

	if (blacks+whites > slots)
		cout << "ERROR: the calculated number of white and black pegs exceeds the number of slots!" << endl;
	for (lcv = 0; lcv < slots-(blacks+whites); lcv++)
		cout << "_ ";
	for (lcv = 0; lcv < whites; lcv++)
		cout << "O ";
	for (lcv = 0; lcv < blacks; lcv++)
		cout << "X ";
}

void print_time(const unsigned int seconds)
{
	if (seconds < 60)
	{
		cout << seconds << " second" << (seconds == 1 ? "" : "s");
		return;
	}
	int minutes = int(seconds/60.0+0.5);
	cout << minutes << " minute" << (minutes == 1 ? "" : "s");
}

// Postcondition: returns the number of moves the user took to guess the sequence, or 0 if the game exceeded board size
int run_game(const Game game, const Board board)
{
	unsigned int movecount=0, lcv=0, secs_elapsed=0;
	unsigned int userseq[MAX_SLOTS];
	unsigned int blacks, whites;
	bool error;

	cout << endl << "Begin entering sets of " << board.slots << " digits, ranging 1 to " << board.colors << ", separated by spaces.";
	if (board.allow_blanks)
		cout << "\nUse a zero to simulate a blank.\t";
	else
		cout << endl;
	if (game.secs_allowed != 0)
	{
		cout << "You have ";
		print_time(game.secs_allowed);
		cout << "." << endl;
	}
	cout << endl;
	do
	{
		blacks = whites = 0;
		movecount++;
		do
		{
			error = false;
			cout << "Enter sequence: ";
			for (lcv=0; lcv < board.slots; lcv++)
			{
				cin >> userseq[lcv];
				if (userseq[lcv] < !board.allow_blanks || userseq[lcv] > board.colors)
					error = true;
			}
			if (error)
				cout << "Your input was incorrect.  Please try again." << endl;
		} while (error);
		// User has entered their sequence.  First, we find the blacks.
		bool comp_ignore[MAX_SLOTS] = {false}, user_ignore[MAX_SLOTS] = {false};
		for (lcv=0; lcv < board.slots; lcv++)
		{
			if (board.compseq[lcv] == userseq[lcv])
			{
				blacks++;
				comp_ignore[lcv] = user_ignore[lcv] = true;
				if (DEBUG)
					cout << "Black count adding value " << userseq[lcv] << " (element " << lcv << ") to both ignorelists." << endl;
			}
		}
		// Calculated and blocked the index of blacks; time for whites.
		// Now only those pegs that are not blocked will be considered
		for (lcv=0; lcv < board.slots; lcv++)
		{
			if (!user_ignore[lcv])
			{
				int location = search_seq(board.compseq, userseq[lcv], comp_ignore, board.slots);
				if (location != -1) // if the user's number in question was found in computer's combination
				{
					whites++;
					if (DEBUG)
						cout << "Computer's index " << location << ", value " << board.compseq[location] << ", is now spoken for." << endl;
					comp_ignore[location] = true;
				}
			}
			else if (DEBUG)
			{
				cout << "Value " << userseq[lcv] << " (lcv=" << lcv << ") is ignored in user's combination, so not checking it." << endl;
			}
					
		}
		// Calculated the number of black and white pegs
		cout << "Move " << setw(2) << movecount << ":  ";
		cout << "\t\t\t\t";
		print_results(blacks, whites, board.slots);
		if (game.secs_allowed != 0)
		{
			secs_elapsed = (int)difftime(time(NULL), game.starttime);
			if (((secs_elapsed > (unsigned int)(game.secs_allowed*(1.9/3)) || secs_elapsed > game.secs_allowed-180) && secs_elapsed < game.secs_allowed) || DEBUG)
			{
				cout << "   (";
				print_time(game.secs_allowed-secs_elapsed);
				cout << " remaining)";
			}
			if (secs_elapsed >= game.secs_allowed)
			{
				cout << endl << "*** YOU'VE RUN OUT OF TIME! ***";
			}
		}
		cout << endl;
	} while (movecount < board.size && blacks != board.slots && (game.secs_allowed == 0 || secs_elapsed < game.secs_allowed));
	if (blacks != board.slots)
		return 0;
	return movecount;
}

int main()
{
	Game game;
	Board board;

	print_intro();
	srandom(time(NULL));
	collect_data(game, board);
	cout << "It is " << get_time("l, H:i:s") << ".  Integrity #: " << hex << (0x100 + random()%0xEFF) << dec << endl;
	
	time(&game.starttime);
	board.moves = run_game(game, board);
	time(&game.endtime);
	game.duration = int(difftime(game.endtime, game.starttime));
	cout << endl;
	if (board.moves > 0)
	{
		cout << "YOU WON IN " << board.moves << " MOVES!" << endl;
		cout << "This game took you ";
		print_time(game.duration);
		cout << "." << endl;
	}
	else
	{
		cout << "Sorry, you lost this round and wasted ";
		print_time(game.duration);
		cout << " of your life on it." << endl;
		cout << "The elusive combination was:\t";
		for (unsigned int i=0; i < board.slots; i++)
			cout << board.compseq[i] << ' ';
	}
	cout << endl;
	write_logs(game, board);
	sleep(5); //some terminals immediately close the window upon disconnection.  Letting players see their info.
	return 0;
}
