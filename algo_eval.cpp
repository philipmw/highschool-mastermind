#include <iostream>
#include <unistd.h>

#define MAX_SLOTS 12
#define MAX_COLORS 9

using namespace std;

int get_min(int s, int c)
{
	return ((c*s)/((c+s)/2));
}

int get_max(int s, int c)
{
	return int((c*s-(c+s))/1.5);
}

int main()
{
	for (int s=4; s <= MAX_SLOTS; s++)
	{
		for (int c=3; c <= MAX_COLORS; c++)
		{
			cout << s << " slots, " << c << " colors:\t\tmin:" << get_min(s, c) << " max:" << get_max(s, c) << endl;
			sleep(1);
		}
	}
	return 0;
}
