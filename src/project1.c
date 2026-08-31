#include "project1.h"
#include "project2.h"

// Define our constants that will be widely used
#define TEXT_COLOR 0x07
#define VIDEO_MEM 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// Track the current cursor's row and column
static int cursorCol = 0;
static int cursorRow = 0;

#if PROJECT == 1
//Checking if setcursor() works 
/* int kernel()
{
    clearscreen();

    setcursor(10, 5);
    print("Test");

    return 0;
} */
// Using this to check if clearscreen() works 
/* int kernel()
{
    print("This should disappear");

    clearscreen();

    print("Clear worked!");

    return 0;
} */

//Using this to check column wraping and scrolling 
/* int kernel()
{
    clearscreen();

    setcursor(81, 24);
    print("Test");

    return 0;
} */

// This function runs when compiled with the #define PROJECT 1 flag
   int kernel()
{
	// Print hello world to the display
	//print("Hello World!\n");
	clearscreen();
	print("Hello World!");
	return 0;
}   
#endif

// Setting the cursor does not display anything visually
// Setting the cursor is simply used by putchar() to find where to print next
// This can also be set independently of putchar() to print at any x, y coordinate on the screen
int setcursor(int x, int y)
{
	cursorCol = x % SCREEN_WIDTH;
	cursorRow = y + (x/SCREEN_WIDTH); 
	if (cursorRow >= SCREEN_HEIGHT) {
		scroll(cursorRow - SCREEN_HEIGHT + 1);
		cursorRow = SCREEN_HEIGHT - 1;
	}
	return 0;
}

// Using a pointer to video memory we can put characters to the display
// Every two addresses contain a character and a color
char putchar(char character)
{
	// If the character is a newline
	if (character == '\n')
	{
		// Set cursor to next row, column 0, and return
		setcursor(0, cursorRow + 1);
		return character;
	}

	// Get a pointer into video memory
	uint8 *vidmem = (uint8 *)VIDEO_MEM;

	// Calculate how far into video memory to go to display the character
	int index = (cursorCol + cursorRow * SCREEN_WIDTH) * 2;

	// Index the pointer (like an array) and set the character and color
	vidmem[index] = character;
	vidmem[index + 1] = TEXT_COLOR;

	// Increment the column and return
	setcursor(cursorCol + 1, cursorRow);

	return character;
}

// Print the character array (string) using putchar()
// Print until we find a NULL terminator (0)
int print(char string[]) 
{
	int characterCount = 0;
	for (int i = 0; string[i] != 0; i++) {
		putchar(string[i]);
	    characterCount++;
	}
	return characterCount;
}

// Clear the screen by placing a ' ' character in every character location
void clearscreen()
{ 
	uint8 *vidmem = (uint8 *)VIDEO_MEM;
	int i = 0;
	while (i < SCREEN_WIDTH * SCREEN_HEIGHT) {
		vidmem[i * 2 ] = ' ';
		i++;
	}
	setcursor(0, 0);
	return;
}