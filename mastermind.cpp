/*
 * Author  : Berru Lafci
 * Language: C++
 * Purpose : This is an interactive program which enables user to play the game mastermind.
 *           The idea of the game is, one side holds a secret number and the other side(user) 
 *           proposes numbers and gets useful hints at each turn. The aim of the user is to 
 *           guess the secret number.
 *
 * This program takes command-line arguments.
 * Example Compile: g++ mysource.cpp -o myprogram
 * Example Run    : ./myprogram -r 4  or  ./myprogram -u 123
 *
 */

#include <iostream>
#include <cstdlib> // for rand()
#include <ctime>  // for rand()
#include <cmath>  // for pow()

using namespace std;

// Function prototypes==========================

//Find errors and exit from 
//the code if there is en error.
void error0(int argc, char *argv[]);
void error1(int digit, int num);
void error2(int randNum, int guessNum, int digit);

int digitOfNum(int num);
int randomNum(int digit);
int strToInt(string str);

//Check the number is same in Guess and if 
//it is not, give a Hint.
void guess(int randNum, int digit);
void hint(int randNum, int guessNum, int digit);

//Find errors, control the input.
bool isString(string s);
bool isUnique(int num, int digit);

//==============================================

int main(int argc, char *argv[]){

	cout << "==================Welcome to mastermind game!==================\n\n";
	cout << "If the first argument is -r, user has to provide another argument: The number of digits." << endl;
	cout << "Program will create a random number which has N digits." << endl;
	cout << "If the first argument is -u, user has to provide another argument which will be used as a secret number.\n" << endl;
	cout << "At each turn user enters a number, your program has to capture the number correctly, compare it with the secret number and print hints." << endl;
	cout << "If the user enters a wrong or invalid argument, it prints the corresponding error.\n(E0 - E1 - E2)" << endl;

	cout << "===============================================================" << endl;

	// Holds command-line arguments.
	string s1 = argv[1];  
	string s2 = argv[2];

	srand(time(NULL));

	error0(argc, argv); // Sends the values in the program call.

	if(s1 == "-r"){

		int digit;
		int guessNum;
		int randNum;

		digit = strToInt(s2);

		// If the generated random number is
		// not unique, generates again.
		do{  
			randNum = randomNum(digit);

		}while(!isUnique(randNum,digit));

		guess(randNum, digit);
	}

	else if(s1 == "-u"){

		int originalNum; // Holds the number to guess
		int digit;

		originalNum = strToInt(s2);
		digit = digitOfNum(originalNum);

		guess(originalNum,digit);
	}

	return 0;

} /* End of Main */

//==============================================

/* 
  Finds the causes of Error0 in "program call"
  and Exits.
*/
void error0(int argc, char *argv[]){

	string arg1 = argv[1];
	string arg2 = argv[2];

	// Wrong parameters.
	if(!((arg1 == "-r") || (arg1 == "-u"))){ 
		cout << "E0" << endl;
		exit(1);
	}

	// Missing or too many parameters.
	else if(argc != 3){  
		cout << "E0" << endl;
		exit(1);
	}

	else if(arg1 == "-r"){  

		int num;
		num = strToInt(arg2);

		// Not a number.
		if(!isString(arg2)){
			cout << "E0" << endl;
			exit(1);
		}

		// Digit overflow.
		if((num <= 0) || (num > 9)){
			cout << "E0" << endl;
			exit(1);
		}
	}

	else if(arg1 == "-u"){

		int num = strToInt(arg2);
		int digit = digitOfNum(num);

		// Not a number.
		if(!isString(arg2)){
			cout << "E0" << endl;
			exit(1);
		}

		// Number starts with 0.
		else if(arg2.at(0) == '0'){ 
			cout << "E0" << endl;
			exit(1);
		}
	
		// Digit overflow.
		else if(arg2.size() > 9){ 
			cout << "E0" << endl;
			exit(1);
		}

		// Not a Unique number.
		else if(!isUnique(num, digit)){
			cout << "E0" << endl;
			exit(1);
		}
	}
}

//==============================================

/*
  Returns false if the value is not a number
  or number is negative.
*/
bool isString(string s){ 

	for(int i=0 ; i < s.size() ; i++){

		if((s[i] < '0') || (s[i] > '9') ){ 
			return false;
		}
	}
	return true; 
}

//==============================================

/*
  Finds the causes of Error1 in "guessing" part
  and Exits.
*/
void error1(int digit, int num){ 

	int inputNumDigit;
	inputNumDigit = digitOfNum(num);

	// Incompatible digits.
	if(inputNumDigit != digit){
		cout << "E1" << endl;
		exit(1);
	}

	// Not a Unique number.
	else if(!isUnique(num, inputNumDigit)){
		cout << "E1" << endl;
		exit(1);
	}
}

//==============================================

/*
  Returns digit of num.
*/
int digitOfNum(int num){

	int count = 0;

    while(num != 0){
        num = num / 10;
        count++;
    }

    return count;
}

//==============================================

/*
  Returns a random number at the given digit.
*/
int randomNum(int digit){

	int num;

	num = rand() % ( ((int) pow(10,digit)-1)-((int) pow(10,digit-1)) +1 ) + ( (int)pow(10,digit-1) );

	return num;
}

//==============================================

/*
  Returns false if the number is not Unique.
*/
bool isUnique(int num, int digit){

	int digit_arr[digit];
	int temp;

	// Sends digits to int array.
	for(int i=0 ; i<digit ; i++){

		digit_arr[i] = num % 10;
		num = num/10;
	}

	for(int i=0 ; i<digit ; i++){

		temp = digit_arr[i];

    // Controls the same digits
		for(int k=i+1 ; k<digit ; k++){

			if(digit_arr[i] == digit_arr[k]){

				return false;
			}
		}
	}
	return true;
}

//==============================================

/*
  Controls the number is valid with Error1 and Error2.
  User enters a number.
  If predicted number is not the answer, gives hint.
  If it is the answer, prints FOUND.
*/
void guess(int randNum, int digit){

	int guessNum;
	int count=0;  // Count of predicted number.
	int firstCount, secondCount;
	char ch;

	do{

		// Exits if the value is not a number or 
		// number is negative with Error2.
		cin >> guessNum;
		ch = getchar();

		if( (!isdigit(ch) && ch != '\n') || (guessNum < 0)){
			cout << "E2" << endl;
			exit(1);
		}

		error1(digit,guessNum);

		if(guessNum != randNum){
			hint(randNum, guessNum, digit);
		}
		
		count++;

		if(count == 100){
			cout << "FAILED" << endl;
			exit(1); 
		}

	}while(guessNum != randNum);

	cout << "FOUND in " << count << " steps." << endl;
}

//==============================================

/*
  Prints the count of hints. 
*/
void hint(int randNum, int guessNum, int digit){

	int randNum_arr[digit];
	int guessNum_arr[digit];

	int firstCount = 0;  // The same number in same place.
	int secondCount = 0;  // The same number in different place.

	// Sends digits to int array
	for(int i=0 ; i<digit ; i++){

		randNum_arr[i] = randNum % 10;
		randNum = randNum / 10;

		guessNum_arr[i] = guessNum % 10;
		guessNum = guessNum / 10;
	}

	for(int i=0 ; i<digit ; i++){

		if(randNum_arr[i] == guessNum_arr[i])
			firstCount++;

		for(int k=0 ; k<digit ; k++){

			if( (randNum_arr[i] == guessNum_arr[k]) && (i!=k))
				secondCount++;
		}
	}

	cout << "Correct place and number:" << firstCount << endl;
	cout << "Wrong place but correct number:" << secondCount << endl;
}

//==============================================

/* 
  Returns the number which turned from a string.
*/
int strToInt(string str){

	int num = 0;

	for(int i=0 ; i<str.size() ; i++){

		num = (num*10) + (str.at(i) - '0');  // Converts char to int
	}
	
	return num;
}


