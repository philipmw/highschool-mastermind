// Programmer: PHILIP M. WHITE
// Last modified: 2004-10-17
// Last change: added more switch cases to set_time()
#include "philipw.h"

int get_digit(const int first, const int last)
{
	int temp,
	 flag;
	string tempstr;

	if (first < 0)
	{
		cout << "get_digit: The range of choices goes below zero." << endl;
	}
	if (last > 9)
	{
		cout << "get_digit: The number of suggested options exceeds one digit. Only one digit may be entered." << endl;
	}

	do
	{
		cin >> temp;
//		temp = getch();
		if (temp < first || temp > last)
		{
			flag = 1;
		}
		else
			flag = 0;
	}
	while (flag);

	cout << temp << endl;
	return temp;
}

// Function programmed mostly by Deborah Nasir
// Purpose: Converts an integer to a string and returns the string.
string int_string(int num)
{
	const int PLACES = 10;
	string temp;
	char stack[PLACES];		//initialize the stack
	int top = PLACES-1;		//initialize the top of stack
	bool is_negative=false;

	if (num < 0)
	{
		is_negative = true;
		num *= -1;
	}
	for (int i=0; i < PLACES; i++)
		stack[i] = '0';
	do
	{
		stack[top] = char (int ('0') + (num % 10));

		top--;
		num /= 10;
	}
	while (num > 0);
	top = 0;		//reinitialze top
	while (top < PLACES-1 && stack[top] == '0')	//filter out leading zeros
		top++;
	while (top < PLACES)		//put the digits in the string
	{
		temp += stack[top];
		top++;
	}
	if (is_negative)
		temp = "-"+temp;
	return temp;
}

string double_string(double num, unsigned int precision)
{
	string str="";

	// 1.00009, precision 3
	str += int_string(int(num));
	num -= int(num);
	num = abs(num);
	// 0.00009
	num *= pow(10.0, int(precision));
	num = double_to_int(num);
	// 0.00009 * 10^3 = 0.09
	str += ".";
	if (num > 0 && num < 1)
		num = 1;
	string decimals_str = int_string(int(num));
	if (decimals_str.length() <= precision)
	{
		for (unsigned int i=0; i < precision-decimals_str.length(); i++)
			str += "0";
		str += decimals_str;
	}
	else
	{
		// we need to round
		str += "r";
	}
	/*
	if (num >= pow(10.0, precision-1))
	{
		// if 100 >= 100
		// if 90 >= 10000
		// if 1 >= 10
	}
	else
	{
		for (int i=0; i < precision-double_to_int(log10(num)); i++)
			str += "0";
	}
	str += int_string(double_to_int(num));
	*/
	return str;
}

int string_int(const char charString[], const int length, unsigned int &output)
{
	int temp_output, retval;
	retval = string_int(charString, length, temp_output);
	output = unsigned(temp_output);
	return retval;
}

int string_int(const char charString[], const int length, int &output)
{
	const int MAX_DIGITS = 7;
	int intArray[MAX_DIGITS] = { 0 }, final_integer = 0;
	bool negative = 0;

	if (length < 1 || length > MAX_DIGITS)
		return 1;
	if (charString[0] == '-')
		negative = true;

	// Checking for non-numeric elements.
	for (int a = negative; a < length; a++)
		if (charString[a] < '0' || charString[a] > '9')
			return 2;

	// Before anything, the array contains something like {'3', '1', '7'}.

	for (int i = negative; i < length; i++)
		intArray[i] = int (charString[i]) - '0';

	// Now the intArray contains something like {3, 1, 7}.

	for (int j = negative; j < length; j++)
		intArray[j] *= int (pow(10.0, (length - 1 - j)));

	// Now the intArray contains something like {300, 10, 7}.

	for (int k = negative; k < length; k++)
		final_integer += intArray[k];

	if (negative)
		final_integer *= -1;

	output = final_integer;
	return 0;
}

int string_double(const char charString[], const int length, double &output)
{
	const int MAX_DIGITS = 8;
	char charArray[MAX_DIGITS];
	int counter=0, temp_int=0;
	double final_double;

	if (length < 1 || length > MAX_DIGITS)
		return 1;

	while (charString[counter] != '.' && counter < length)
	{
		charArray[counter] = charString[counter];
		counter++;
	}
	// now we have the [negative?] integer part of the double in integer[].
	// time to convert that string into an integer (for now).
	if (counter != 0 && string_int(charArray, counter, temp_int))
	{
		// unsuccessfully converted the first part.  Poop.
//		cout << "Unsuccessfully converted the integer part of the double." << endl;
		return 2;
	}
	final_double = double(temp_int); // depositing the first part.
	// Time to deal with the fractional part.
	if (counter >= length-1) // Is there a fractional part at all?
	{
		// no fractional part.  We're done.
		output = final_double;
		return 0;
	} else {
		counter++; // getting past the dot.
		for (int i=counter; i < length; i++)
			charArray[i-counter] = charString[i];
		if (string_int(charArray, length-counter, temp_int))
		{
//			cout << "Unsuccessfully converted the decimal part of the double." << endl;
			return 4; // the second part proved invalid.
		}
		if (charString[0] == '-') // if it's a negative number...
			temp_int *= -1;
		final_double += double(temp_int) * pow(10.0, counter-length);
	}
	output = final_double;
	return 0;
}

void upper(char &letter)
{
	if (int (letter) >= 97 && int (letter) <= 122)
		letter -= char (32);
}

int alert(const string msg, const bool query)
{
	const int DELAY = 5;
	static int alerts = 0;

	if (query)
	{
		return alerts;
	}
	else
	{
		alerts++;
		cerr << '\a' << msg << endl << endl;
		#ifdef __MSDOS__
		delay(DELAY*1000);
		#else
		sleep(DELAY);
		#endif
	}
	return 0;
}

string center(const string str, const int linesize)
{
	string new_str;
	int var = str.length();	// var is the length of the string

	var = linesize - var;	// var is the amount of blank space on the line
	var = double_to_int(var/2.0);	// var is the amount of space to indent to center the line.

	for (int i = 1; i <= var; i++)
		new_str += " ";
	new_str += str;
	return new_str;
}

string right(const string str, const int linesize)
{
	string new_str;
	int var = str.length();	// var now contains the length of the string

	var = linesize - var;	// var contains the empty space on the line
	for (int i = 1; i <= var; i++)
		new_str += " ";
	new_str += str;
	return new_str;
}

// Returns 1 if answer is Y, 0 if N.
bool prompt_yn(const int defau)
{
	bool good_response = false;
	char choice;
	string temp;

#ifdef __MSDOS__
	switch (defau)
	{
	case 1:
		cout << " (Y/n): ";
		break;
	case 0:
		cout << " (y/N): ";
		break;
	case -1:
		cout << " (y/n): ";
		break;
	default:
		alert("prompt_yn: invalid parameter!", 0);
		break;
	}
#else
	cout << " ";
#endif

	do
	{
#ifdef __MSDOS__
		temp = getch();
#else
		do
		{
			cout << "(y/n): ";	// we don't care about what the user picks on Linux.
			getline(cin, temp);
		}
		while (temp.length() != 1);
#endif
		choice = temp[0];
		upper(choice);

#ifdef __MSDOS__
		if (choice == '\r' && defau == 1)
			choice = 'Y';
		else if (choice == '\r' && defau == 0)
			choice = 'N';
#endif

		if (choice == 'Y' || choice == 'N')
			good_response = true;
	}
	while (!good_response);

#ifdef __MSDOS__
	cout << choice << endl;
	if (choice == '\r')
		return defau;
#endif
	if (choice == 'Y')
		return 1;
	return 0;
}

int move_file(const string src, const string dst)
{
	string temp;
	int num_lines = 0,
	 line_count = 1;
	ofstream destination;
	ifstream source;

	source.open(src.c_str());
	if (src == dst)
	{
		source.close();
		destination.close();
		alert("Cannot move data: source filename may not equal destination filename.", 0);
		return -1;
	}
	if (source.fail())
	{
		source.close();
		alert("The source file ("+src+") could not be opened for copying data.", 0);
		return -1;
	}
	remove(dst.c_str());
	// Getting the number of lines in source file
	while (!source.eof())
	{
		getline(source, temp);
		num_lines++;
	}
	source.clear();
	source.seekg(0);
	/* ***************** SAMPLE CODE *******************
	get length of file:
	is.seekg (0, ios::end);
	length = is.tellg();
	is.seekg (0, ios::beg);
	***************************************************/

	//cout << "Moving data from " << src << " to " << dst << "..." << endl;
	destination.open(dst.c_str());
	while (line_count <= num_lines)
	{
		getline(source, temp);
		//cout << "Writing: \"" << temp << "\"\n";
		destination << temp;
		if (line_count != num_lines)
			destination << endl;
		line_count++;
	}

	source.close();
	destination.close();

	remove(src.c_str());	// deleting the old file.
	return 1;
}

string spread(const string str1, const string str2, const unsigned int line_len)
{
	string result;

	result = str1;
	for (unsigned int i = 1; i <= line_len - (str1.length() + str2.length()); i++)
		result += " ";
	result += str2;
	return result;
}

string my_excuse()
{
	const int EXCUSE_SIZE = 21;
	string excuses[EXCUSE_SIZE] = {
		"That's weird...",
		"It's never done that before.",
		"It worked yesterday.",
		"How is that possible?",
		"It must be a hardware problem.",
		"What did you type in wrong to get it to crash?",
		"There's something funky in your data.",
		"I haven't touched that module in weeks!",
		"You must have the wrong version.",
		"It's just some unlucky coincidence.",
		"I can't test everything!",
		"THIS can't be the source of THAT!",
		"It works, but it hasn't been tested.",
		"Somebody must've changed my code.",
		"Did you check for a virus on your system?",
		"Even though it doesn't work, how does it feel?",
		"You can't use this version on your system.",
		"Why do you want to do it that way?",
		"Where were you when the program blew up?",
		"It works on my machine.",
		"It's not a bug -- it's a feature!"
	};
	string excuse;

	excuse = "Philip exclaims, \"";
	srand(time(NULL));
	excuse += excuses[rand() % EXCUSE_SIZE];
	excuse += "\"\n";

	return excuse;
}

void progress_bar(const int curr_done, const int workload)
{
	// this many workunits required to draw a pixel.
	double pixel_count = double(workload) / (SCREEN_LINE-2);
	// this many pixels are done; everything else is empty.
	int bars = double_to_int(curr_done / pixel_count),
	 percent = double_to_int(curr_done*100.0 / workload),
	 // how many pixels to draw before inserting the percentage
	 half_bars = double_to_int(bars/2.0)-2;
	static int last_barcount;
	int counter;

	if (half_bars < 0)
		half_bars = 0;
	if (bars == last_barcount)
		return;

//	for (counter=0; erase_prev && counter < 2; counter++)
//		cout << char(8) << endl; // erasing the screen
	cout << '[';
	for (counter=0; counter < half_bars; counter++)
		cout << '|';
	if (percent == 100)
		cout << percent << '%';
	else if (percent >= 10)
		cout << percent << " %";
	else
		cout << ' ' << percent << "% ";
	for (counter=0; counter < half_bars; counter++)
		cout << '|';
	for (counter=0; counter < (SCREEN_LINE-2)-(2*half_bars)-4; counter++)
		cout << ' ';
	cout << ']' << endl;
	last_barcount = bars;
}

string pluralize(string str, const int amount)
{
	if (amount != 1)
	{
		if (str == "was")
			str = "were";
		else if (str == "is")
			str = "are";
		else
			str += "s";
	}
	return str;
}

string possessivize(string str)
{
	if (str[str.length()-1] == 's')
		str += "'";
	else
		str += "'s";
	return str;
}

int double_to_int(const double num)
{
	int rounded;
	if (num >= 0)
		rounded = int(num+0.5);
	else
		rounded = int(num-0.5);
	return rounded;
}

string trim_whitespace(string str)
{
	const int WHITESPACE_SIZE = 3;
	const char whitespace[WHITESPACE_SIZE] = { ' ', '\t', '\n' };

	int counter=0;
	bool chop_off;
	do
	{
		chop_off = false;
		for (int i=0; i < WHITESPACE_SIZE; i++)
		{
			if (str[counter] == whitespace[i])
				chop_off = true;
		}
		if (chop_off)
			str = str.substr(1, str.length()-1);
		counter++;
	} while (chop_off);
	// the first part is chopped off.
	do
	{
		chop_off = false;
		for (int j=0; j < WHITESPACE_SIZE; j++)
		{
			if (str[str.length()-1] == whitespace[j])
				chop_off = true;
		}
		if (chop_off)
		{
			str = str.substr(0, str.length()-1);
		}
	} while (chop_off);
	// done
	return str;
}

string capitalize(string str)
{
	for (unsigned int i=0; i < str.length(); i++)
		str[i] = toupper(str[i]);
	return str;
}

string pad_str_right(string str, const unsigned int length_needed, const char padding)
{
	while (str.length() < length_needed)
		str += padding;
	return str;
}

string get_num_suffix(int num)
{
	if (num > 13)
		num = num % 10;
	switch (num)
	{
		case 1: return "st";
		case 2: return "nd";
		case 3: return "rd";
		default: return "th";
	}
	return "ERROR!";
}

string num2let_grade(const int grade)
{
	if (grade >= 98)
		return "A+";
	else if (grade >= 92)
		return "A";
	else if (grade >= 90)
		return "A-";
	else if (grade >= 88)
		return "B+";
	else if (grade >= 82)
		return "B";
	else if (grade >= 80)
		return "B-";
	else if (grade >= 75)
		return "C";
	else if (grade >= 70)
		return "D";
	else
		return "F";
}

string pad_str_left(string str, const unsigned int length_needed, const char padding)
{
	if (str.length() >= length_needed)
		return str;
	while (str.length() < length_needed)
		str = padding+str;
	return str;
}

string get_time(string fmt)
{
	string our_time;
	time_t temptime;
	tm timestruct;
	int tempval;

	const string MONTH_FULL[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	const string DAY_FULL[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

	time(&temptime);
	timestruct = *localtime(&temptime);

	for (unsigned int i=0; i < fmt.length(); i++)
	{
		switch (fmt[i])
		{
			case 'a': if (timestruct.tm_hour < 12)
					our_time += "am";
				else
					our_time += "pm";
				break;
			case 'A': if (timestruct.tm_hour < 12)
					our_time += "AM";
				else
					our_time += "PM";
				break;
			case 'D': our_time += DAY_FULL[timestruct.tm_wday].substr(0, 3);
				break;
			case 'd': our_time += pad_str_left(int_string(timestruct.tm_mday), 2, '0');
				break;
			case 'F': our_time += MONTH_FULL[timestruct.tm_mon];
				break;
			case 'G': our_time += int_string(timestruct.tm_hour);
				break;
			case 'g': tempval = timestruct.tm_hour % 12;
				if (tempval == 0)
					tempval = 12;
				our_time += int_string(tempval);
				break;
			case 'H': our_time += pad_str_left(int_string(timestruct.tm_hour), 2, '0');
				break;
			case 'h': tempval = timestruct.tm_hour % 12;
				if (tempval == 0)
					tempval = 12;
				our_time += pad_str_left(int_string(tempval), 2, '0');
				break;
			case 'i': our_time += pad_str_left(int_string(timestruct.tm_min), 2, '0');
				break;
			case 'l': our_time += DAY_FULL[timestruct.tm_wday];
				break;
			case 'm': our_time += pad_str_left(int_string(timestruct.tm_mon), 2, '0');
				break;
			case 'M': our_time += MONTH_FULL[timestruct.tm_mon].substr(0, 3);
				break;
			case 'n': our_time += int_string(timestruct.tm_mon);
				break;
			case 's': our_time += pad_str_left(int_string(timestruct.tm_sec), 2, '0');
				break;
			case 'Y': our_time += int_string(timestruct.tm_year+1900);
				break;
			case 'y': our_time += pad_str_left(int_string(timestruct.tm_year%100), 2, '0');
				break;
			default: our_time += fmt[i];
				break;
		}
	}

	return our_time;
}
/*
double eval_equation(string eq)
{
	List llist;
	double result = 0;
	string chunk = "";

	while (eq.find('+') != npos || eq.find('-') != npos || eq.find('*') != npos || eq.find('/'!= npos))
	{
		
	}
}
*/
