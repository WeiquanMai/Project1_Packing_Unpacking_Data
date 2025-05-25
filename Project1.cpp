/*
* CSC-237- C++
* Project1: Text Packing/ Unpacking Operations
* Student: Weiquan Mai
* Date: February 21, 2025
* Description: 
* This program reads a text document, "packs" the ASCII characters from that document into unsigned int variables,a nd outputs those variables to another text file as integers.
* This program also reverses the process, converting the unsigned int numbers back into a copy of the original text document.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

// Function Prototypes
void outputHelpText();
void packData();
void unpackData();
bool confirmYN();
bool quit();

int main()
{
	string command;
	bool keepRunning = true; // flag to control exit from program
	
	while (keepRunning == true)
	{
		// Prompt user for command input. Use command input to control function of the program
		cout << "\nCommand: ";
		getline(cin, command);

		if (command == "") {
			// ignore empty commands
		}
		else if (command == "p" || command == "P")
		{
			packData();
		}
		else if (command == "u" || command == "U")
		{
			unpackData();
		}
		else if (command == "h" || command == "H")
		{
			outputHelpText();
		}
		else if (command == "q" || command == "Q")
		{
			keepRunning = !quit();
		}
		else
		{
			cout << "Invalid command: " << command << endl;
		}
	} // end while keepRunning == true
	
	cout << "Exiting the program." << endl;
	return 0;
} // End of function main

// Function outputHelpText
// Description: Displays a list of commands that the program supports.
void outputHelpText()
{
	// Output list of supported commands
	cout << "Supported commands: " << endl;
	cout << "	p	Build packed data file." << endl;
	cout << "	u	Create unpacked (text) data from packed data." << endl;
	cout << "	h	Print this list of supported commands." << endl;
	cout << "	q	Quit (exit) the program.";
} // End of function outputHelpText

/*________________________________________________________________________________________
* Function: confirmYN
* Inputs: prompt Y or N from user
* Outputs: Returns true or false
* Description: Ask user to confirm if they want to quit the program.
* If user enters y or Y, then the program ends.
* If user enters n or N, then the program continues running.
* If user inputs any other value, then loop disregards value and runs again.
*/
bool confirmYN()
{
	string inputString;
	bool confirmationValue = false;
	bool inputIsOk = false;
	do
	{
		// Obtain input from user. If input is y or Y, close do-while loop, and return true to quit function. If input is n or N, close do-while loop, and return false to quit function
		// For any other input, tell user that the input is invalid, and prompt for another input.
		cout << "Are you sure that you want to exit the program? (y/n)" << endl;
		getline(cin, inputString);

		if (inputString == "y" || inputString == "Y")
		{
			confirmationValue = true;
			inputIsOk = true;
		}
		else if (inputString == "n" || inputString == "N") 
		{
			confirmationValue = false;
			inputIsOk = true;
		}
		else 
		{
			cout << "Invalid input: " << inputString << ". Please input y or n." << endl;
		}
	} while (inputIsOk == false);
	return confirmationValue;
} // End of function confirmYN

/*________________________________________________________________________________________
* Function: quit
* Output: returns TRUE if user wants to exit the program
* Description: Calls confirmYN function to confirm if user wants to exit the program
*/
bool quit()
{
	return confirmYN();
} // End of function quit.

/*________________________________________________________________________________________
* Function: packData
* Input: Ask user for input filename and output filename
* Output: A series of unsigned int into specified output file
* Description: Reads a complete line of text from input file, saving the text into a string object. Processes string object into a group of four ASCII characters merged into an unsigned int.
* Then outputs the unsigned int into the output file on a line by itself.
* Repeats process until end of string object has been reached.
*/
void packData()
{
	// Variables
	string inputText = "";
	string inputFileName;
	string outputFileName;
	ifstream inputFile;
	ofstream outputFile;
	int inputLineLength = 0;
	int outputFileLength = 0;
	unsigned int textCharacter;
	unsigned int mergeValue = 0;

	// Input file: prompt user for filename, open ifstream object
	cout << "Enter name of input file: ";
	getline(cin, inputFileName);
	inputFile.open(inputFileName);

	// Check for inputFile open error
	while (!inputFile)
	{
		cout << "Error opening file: " << inputFileName << endl;
		cout << "Please enter another file to be opened: ";
		getline(cin, inputFileName); // Ask user to input another file
		inputFile.open(inputFileName);
	}

	// Output file: prompt user for filename, open ofstream object
	cout << "Enter name of output file: ";
	getline(cin, outputFileName);
	outputFile.open(outputFileName);

	// Outer loop for reading line of text from input file and adding "\n" after end of each line
	while (getline(inputFile, inputText))
	{
		inputLineLength = inputText.length();
		inputText += '\n'; // Append newline character to end of each string
		cout << "Input text (length=" << inputLineLength << ") :" << left << setw(80) << inputText << endl;

		// Inner loop to read one character at a time from inputText string
		for (int position = 0; position < inputText.length(); position++)
		{
			textCharacter = inputText[position]; 

			// Packs the textCharacter into bit position based on offset. When four ASCII characters are packed, sends the entire value into output file
			int offset = position % 4;

			switch (offset)
			{
			case 0:
				mergeValue |= (textCharacter << 24); 
				break;
			case 1:
				mergeValue |= (textCharacter << 16); 
				break;
			case 2:
				mergeValue |= (textCharacter << 8); 
				break;
			case 3:
				mergeValue |= textCharacter; 
				outputFile << mergeValue << endl; 
				mergeValue = 0;
				break;
			} // End of switch
		} // End of for loop

		// Output final partially filled unsigned int value to output file
		if (inputText.length() % 4 != 0)
		{
			outputFile << mergeValue << endl;
			mergeValue = 0;
		}

		// Output a blank line to output file for readability
		outputFile << endl;
	}

	// Close files
	inputFile.close();
	outputFile.close();
}

/*
* Function: unpackData
* Input: Ask user for input filename and output filename
* Output: Each ASCII character into output file
* Description: Converts the unsigned int values from input file and processes them into ASCII characters. Writes ASCII characters into output file.
*/
void unpackData()
{
	// Variables
	string inputFileName;
	string outputFileName;
	ifstream inputFile;
	ofstream outputFile;
	unsigned int inputNumber, result;
	unsigned int mask = 0xFF;
	char ch;

	// Input file: prompt user for filename, open ifstream object
	cout << "Enter the input filename: ";
	getline(cin, inputFileName);
	inputFile.open(inputFileName);

	// Check for inputFile open error
	while (!inputFile)
	{
		cout << "Error opening file: " << inputFileName << endl;
		cout << "Please enter another file to be opened: ";
		getline(cin, inputFileName); // Ask user to input another file name
		inputFile.open(inputFileName);
	}

	// Output file: prompt user for filename, open ofstream object
	cout << "Enter the output filename: ";
	getline(cin, outputFileName);
	outputFile.open(outputFileName);

	// Outer loop to read unsigned int values from input file
	while (inputFile >> inputNumber)
	{
		// Inner loop to extract the four ASCII characters from the unsigned int using bitwise & and the right-shift operation
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				result = (inputNumber >> 24) & mask; 
				break;
			case 1:
				result = (inputNumber >> 16) & mask;
				break;
			case 2:
				result = (inputNumber >> 8) & mask; 
				break;
			case 3:
				result = inputNumber & mask; 
				break;
			} // end of switch

			// Change unsigned int into a character and output character into file if character is not hex 0x00
			ch = static_cast<char> (result); 
			if (result != 0x00)
				{
				cout << ch;
				outputFile << ch; 
				}
		} // End of for loop
	} // End of while (inputFile >> inputNumber)

	// Close files
	inputFile.close();
	outputFile.close();

} // End of function unpackData
