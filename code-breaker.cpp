// Code written by Jordan Martin on February 19th, 2020.
// - This program makes the assumption that user input should ALSO be limited to being only 4 digits long and with unique digits.
// - Making this assumption eliminates the ability for users to enter incomparable guesses or for repeat digits which will reduce their chances of winning.


#define NUM_DIGITS 4 // Number of digits in passcode, up to 10 (due to unique digits)
#define CHANCES 8


#include <time.h>
#include <iostream>
#include <algorithm> 
#include <vector>
#include <string>

// Creates a secret code that is 4 digits long and consists of unique digits only
// Does this by shuffling the possible base 10 digits and selecting 4 of them
int secret_code()
{
	std::vector<int> digits = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; // Base 10 digits
	// Shuffle digits, will take 4 out of 10
	random_shuffle(digits.begin(), digits.end()); 

	// Leading digit cannot be '0', if so grab from end
	int num = (digits.at(0) == 0) ? digits.at(9) : digits.at(0); 

	// Choose next 3 digits after first one from shuffled list
	for (int i = 1; i < NUM_DIGITS; i++) 
	{
		num *= 10; // Adds extra digit place
		num += digits.at(i);
	}
	return num;
}

// Recursively counts the digits in an integer and returns that count
int count_digits(int n) 
{
	if (n == 0) // No more digits
		return 0;

	// Remove digit and add 1 to count
	return 1 + count_digits(n / 10);
}

// Checks if a number had all unique digits.
// Does this by checking to see if any base 10 digits are seen twice
bool is_unique(int n)
{
	// All base 10 digits
	bool digits[10]; 

	// None have been seen yet, so mark all as false
	for (int i = 0; i < 10; i++)
		digits[i] = false; 

	while (n > 0) // While there are still digits
	{
		// Copy ending digit
		int digit = n % 10; 

		// If already seen this digit, number is not unique and can return false
		if (digits[digit]) 
			return false;

		// Mark this base 10 digit seen before
		digits[digit] = true; 

		// Remove ending digit
		n /= 10; 
	}
	return true;
}

// Asks player to enter a unique 4 digit guess and returns it
int user_input() 
{
	int guess;
	while (1) 
	{
		std::cout << "Please enter a unique 4 digit guess for the code: ";

		// Validating that input is a valid integer.
		if (std::cin >> guess)
		{
			// Validating that integer is 4 digits long.
			if (count_digits(guess) == 4)
			{
				// Validating that integer has unique digits
				if (is_unique(guess))
					return guess;
				else
					std::cout << "ERROR: Number does not have unique digits." << std::endl;
			}
			else
				std::cout << "ERROR: Number is not 4 digits long." << std::endl;
		}
		else
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "ERROR: Please enter a valid integer." << std::endl;
			continue;
		}
	}
	return guess;
}

// Gives a report after a guess on how many correct digits were guessed 
// and how many correct numbers in incorrect places were guessed
void report(int code, int answer)
{
	// Convert to strings to compare indices
	std::string answer_string = std::to_string(code);
	std::string code_string = std::to_string(answer);
	int correct_num_index = 0, correct_num_only = 0;

	// Check all combinations of indices for a match
	for (int i = 0; i < NUM_DIGITS; i++)
	{
		for (int j = 0; j < NUM_DIGITS; j++)
		{
			if (code_string.at(i) == answer_string.at(j)) // Found a match
			{
				if (i == j) // Same number, same index
					correct_num_index++;
				else // Same number, different index
					correct_num_only++;
			}
		}
	}

	std::cout << "- Found " << correct_num_index << " digits guessed correctly!" << std::endl;
	std::cout << "- Found " << correct_num_only << " correct numbers, but in the wrong place." << std::endl;
}

// Starts a new game of CodeBreaker with passcode provided
// Until the player guesses correctly OR runs out of chances
// Makes a report after each failed guess
void new_game(int code)
{
	int guess;
	for (int i = 0; i < CHANCES; i++) 
	{
		std::cout << "--------------------\nChances left: " << CHANCES - i << std::endl;

		guess = user_input(); // Take player guess
		
		if (guess == code)
		{
			std::cout << "Guessed the right code. You win!" << std::endl;
			return;
		}
		else 
		{ 
			std::cout << "That guess wasn't right." << std::endl;
			report(guess, code); // Report on false guess
		}
	}
	std::cout << "Out of chances. You lose!" << std::endl;
	std::cout << "The code was: " << code << std::endl;
}

// Asks player if they want to play again.
// Returns true if yes, false if no.
bool play_again()
{
	char answer;
	while (1)
	{
		std::cout << "Wanna play again? Y/N" << std::endl;

		// Validating that input is a valid character
		if (std::cin >> answer)
		{
			// Validating that character is either Y/N
			if (answer == 'Y' || answer == 'y')
				return true;
			else if (answer == 'N' || answer == 'n')
				return false;
			else
				std::cout << "ERROR: Character is not 'Y' or 'N'" << std::endl;
		}
		else
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "ERROR: Please enter a valid character." << std::endl;
			continue;
		}
	}
}

int main()
{
	time_t t;
	srand((unsigned)time(&t)); // Seed random shuffle

	char choice;
	while(1)
	{
		// Start new game
		new_game(secret_code());

		// Determines if player wants to play again
		choice = play_again();
		if (choice)
			continue;
		else
			break;
	}
	return 0;
}