/*
Author: Qizhi Ge
Date: 5/2/2025
Purpose: Project 10
*/

#include <stdio.h>
#include <stdbool.h>

//constants

#define WORD_LENGTH 5
#define MAX_GUESSES 6
#define INPUT_BUFFER_SIZE 100 



//brief Calculates the length of a null-terminated string.
size_t my_strlen(const char *str);


//brief Copies the source string (including null terminator) to the destination buffer.
char* my_strcpy(char *dest, const char *src);

 //brief Compares the first n characters of two strings for equality.
bool my_strncmp_equal(const char *s1, const char *s2, size_t n);


//brief Converts a character to lowercase if it's an uppercase letter.
char my_tolower(char c);

//brief Converts a character to uppercase if it's a lowercase letter.
char my_toupper(char c);


//brief Checks if a character is an alphabetic letter (a-z or A-Z).
bool my_isalpha(char c);


//brief Converts an entire string to lowercase, in place.
void my_stringToLower(char *str);


//brief Finds and replaces the first newline character ('\n') in a string with a null terminator ('\0').
void my_remove_newline(char *str);

//core GAm function
bool loadMysteryWord(char mysteryWord[WORD_LENGTH + 1]);
void getValidGuess(char guessBuffer[WORD_LENGTH + 1]);
void processGuess(const char mysteryWord[WORD_LENGTH + 1],
                  const char guess[WORD_LENGTH + 1],
                  char resultGuess[WORD_LENGTH + 1],
                  char feedback[WORD_LENGTH + 1]);
void displayGuesses(const char guesses[MAX_GUESSES][WORD_LENGTH + 1],
                    const char feedback[MAX_GUESSES][WORD_LENGTH + 1],
                    int guessCount);
void printEndMessage(bool won, const char mysteryWord[WORD_LENGTH + 1]);

//GAm main

int main() {
    char mysteryWord[WORD_LENGTH + 1];
    char userGuessRaw[WORD_LENGTH + 1]; // Stores validated input
    char userGuessLower[WORD_LENGTH + 1]; // Stores lowercase version for comparison
    char processedGuesses[MAX_GUESSES][WORD_LENGTH + 1]; // Stores results for display
    char feedbackStrings[MAX_GUESSES][WORD_LENGTH + 1];  // Stores feedback ('^') for display
    int guessCount = 0;
    bool playerWon = false;

    //Load the mystery word
    if (!loadMysteryWord(mysteryWord)) {
        return 1;
    }
    my_stringToLower(mysteryWord);

    //Let the pain beggin
    printf("Welcome to Wordup!\n");
    printf("Guess the %d-letter word. You have %d tries.\n\n", WORD_LENGTH, MAX_GUESSES);

    while (guessCount < MAX_GUESSES && !playerWon) {
        printf("--- Guess %d of %d ---\n", guessCount + 1, MAX_GUESSES);

        // Get guess (work?)
        getValidGuess(userGuessRaw);

        // 3.lower casing
        my_strcpy(userGuessLower, userGuessRaw);
        my_stringToLower(userGuessLower);

        // 4. win?
        if (my_strncmp_equal(userGuessLower, mysteryWord, WORD_LENGTH)) {
            playerWon = true;
            // win!!!
             processGuess(mysteryWord, userGuessLower, processedGuesses[guessCount], feedbackStrings[guessCount]);
        } else {
            // 5. feed baclk
             processGuess(mysteryWord, userGuessLower, processedGuesses[guessCount], feedbackStrings[guessCount]);
        }
        guessCount++;

        //disp guess
        displayGuesses(processedGuesses, feedbackStrings, guessCount);
        	printf("\n");
    }

    // 7. the end?
    printEndMessage(playerWon, mysteryWord);

    return 0;
}

// functions

size_t my_strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char* my_strcpy(char *dest, const char *src) {
    size_t i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

// Checks if the first 'n' characters are identical.
bool my_strncmp_equal(const char *s1, const char *s2, size_t n) {
     for (size_t i = 0; i < n; i++) {
        // If amt not match up
        if (s1[i] == '\0' || s2[i] == '\0' || s1[i] != s2[i]) {
            // Check the specific case where both end at the same time before n chars
             if (s1[i] == '\0' && s2[i] == '\0') {
                 return true; // Both ended, so they were equal up to this point
             }
            return false; // Mismatch or one ended early
        }
    }
    // If loop completes, the first n chars were identical
    return true;
}


char my_tolower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

char my_toupper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }
    return c;
}

bool my_isalpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void my_stringToLower(char *str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = my_tolower(str[i]);
    }
}

// FRS(Find, replace, stop)
void my_remove_newline(char *str) {
     for (size_t i = 0; str[i] != '\0'; i++) {
         if (str[i] == '\n') {
             str[i] = '\0';
             return; 
         }
     }
}


//CGF

bool loadMysteryWord(char mysteryWord[WORD_LENGTH + 1]) {
    FILE *filePtr = fopen("word.txt", "r");
    if (filePtr == NULL) {
        fprintf(stderr, "Error: Could not open word.txt.\n");
        fprintf(stderr, "Please ensure 'word.txt' exists in the same directory.\n");
        return false;
    }


    if (fscanf(filePtr, "%5s", mysteryWord) != 1) { 
        fprintf(stderr, "Error: Could not read a word from word.txt or word is too short.\n");
        fclose(filePtr);
        return false;
    }
    
    
    mysteryWord[WORD_LENGTH] = '\0';

    // word too long?
    int extraChar = fgetc(filePtr);
    if (extraChar != EOF && extraChar != '\n' && extraChar != ' ' && extraChar != '\t' && extraChar != '\r') {
         fprintf(stderr, "Error: Word in word.txt seems longer than %d letters.\n", WORD_LENGTH);
         fclose(filePtr);
         return false;
    }


    fclose(filePtr);

    // vibe check/length check
    if (my_strlen(mysteryWord) != WORD_LENGTH) {
        fprintf(stderr, "Error: The word '%s' in word.txt is not %d letters long.\n", mysteryWord, WORD_LENGTH);
        return false;
    }

    // CC(character checker)
     for (int i = 0; i < WORD_LENGTH; i++) {
         if (!my_isalpha(mysteryWord[i])) {
              fprintf(stderr, "Error: The word '%s' in word.txt contains non-letter characters.\n", mysteryWord);
              return false;
         }
     }

    return true;
}

void getValidGuess(char guessBuffer[WORD_LENGTH + 1]) {
    char inputBuffer[INPUT_BUFFER_SIZE]; // Temp buffer?

    while (true) {
        printf("Enter your %d-letter guess: ", WORD_LENGTH);
        if (fgets(inputBuffer, INPUT_BUFFER_SIZE, stdin) == NULL) {
             printf("Error reading input or end-of-file detected. Exiting.\n");
             guessBuffer[0] = '\0'; // 
             return;
        }

        my_remove_newline(inputBuffer);

        // Validate vibe(length)
        size_t len = my_strlen(inputBuffer);
        if (len != WORD_LENGTH) {
            printf("Invalid input: Your guess must be exactly %d letters long (you entered %zu).\n", WORD_LENGTH, len);
             if (len == INPUT_BUFFER_SIZE - 1 && inputBuffer[len-1] != '\0') {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
            continue;
        }

        // Validate character
        bool allLetters = true;
        for (int i = 0; i < WORD_LENGTH; i++) {
            if (!my_isalpha(inputBuffer[i])) {
                printf("Invalid input: Please enter only letters (A-Z).\n");
                allLetters = false;
                break;
            }
        }

        if (allLetters) {
            my_strcpy(guessBuffer, inputBuffer); // Copy valid guess using custom function
            return;
        }
    }
}


void processGuess(const char mysteryWord[WORD_LENGTH + 1],
                  const char guess[WORD_LENGTH + 1],
                  char resultGuess[WORD_LENGTH + 1],
                  char feedback[WORD_LENGTH + 1])
{
    // Initialize result/feedback
    my_strcpy(resultGuess, guess); // Start with the lowercase guess (custom strcpy)
    for (int i = 0; i < WORD_LENGTH; i++) {
        feedback[i] = ' '; // Fill feedback with spaces initially
    }
    feedback[WORD_LENGTH] = '\0'; // Null-terminate feedback string

    bool mysteryLetterUsed[WORD_LENGTH] = {false}; // Keep track of mystery letters matched

    // First Pass: Check for correct letter in correct position (Green/Capital)
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == mysteryWord[i]) {
            resultGuess[i] = my_toupper(guess[i]); // Capitalize (custom toupper)
            mysteryLetterUsed[i] = true;         // Mark this mystery letter as used correctly
        }
    }

    // Second Pass: Check for correct letter in wrong position (Yellow/^)
    for (int i = 0; i < WORD_LENGTH; i++) {
        bool is_upper = (resultGuess[i] >= 'A' && resultGuess[i] <= 'Z');
        if (!is_upper) {
            // Search for this guess letter elsewhere in the word
            for (int j = 0; j < WORD_LENGTH; j++) {
                if (guess[i] == mysteryWord[j] && !mysteryLetterUsed[j]) {
                    feedback[i] = '^';          // Place the feedback marker
                    mysteryLetterUsed[j] = true; // Mark this mystery letter as used
                    break;
                }
            }
        }
    }
     resultGuess[WORD_LENGTH] = '\0';
}


void displayGuesses(const char guesses[MAX_GUESSES][WORD_LENGTH + 1],
                    const char feedback[MAX_GUESSES][WORD_LENGTH + 1],
                    int guessCount)
{
    printf("\n--- Your Guesses So Far ---\n");
    for (int i = 0; i < guessCount; i++) {
        printf("%s\n", guesses[i]);
        printf("%s\n", feedback[i]);
    }
    printf("---------------------------\n");
}

void printEndMessage(bool won, const char mysteryWord[WORD_LENGTH + 1]) {
    printf("===========================\n");
    if (won) {
        printf("Congratulations! You guessed the word!\n");
    } else {
        printf("Game Over! You ran out of guesses.\n");
        // Convert mystery word to uppercase for the reveal using custom functions
        char upperMystery[WORD_LENGTH + 1];
        my_strcpy(upperMystery, mysteryWord); // custom strcpy
         for(int i=0; upperMystery[i] != '\0'; ++i) {
             upperMystery[i] = my_toupper(upperMystery[i]);
         }
        printf("The mystery word was: %s\n", upperMystery);
    }
    printf("===========================\n");
}
