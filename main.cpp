#include <ncurses.h>
#include <string>
#include <iostream>
#include <sstream>

/*
    Used for displaying the score
*/
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 3){
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::move(out).str();
}

struct question{
    std::string question;
    int answer;
    bool isDivision = false;
};

// I determines the range of the numbers
// Generates a question that looks like this:
// 1 + 2 = ?
// ? + 2 = 3
// 10 - ? = 5
question generateQuestion(int range){
    if(range < 2){
        range = 2;
    }

    // Generate a random number from 0 to 3 to decide what operation to use
    // 0 = +
    // 1 = -
    // 2 = *
    // 3 = /
    int operation = rand() % 4;

    // If the question is multiplication, reduce the range by dividing it by 3
    if(operation == 2){
        range /= 3;
    }

    // Generate two random numbers from 1 to range
    int num1 = rand() % range + 1;
    int num2 = rand() % range + 1;


    // Generate a random number from 0 to 2 to decide where to put the ?
    // 0 = ? <op> x = y
    // 1 = x <op> ? = y
    // 2 = x <op> y = ?
    int where = rand() % 3;

    // If the question is division, make sure the numbers are divisible (num1 must always be bigger than num2)
    if(operation == 3){
        if(num1 < num2){
            int temp = num1;
            num1 = num2;
            num2 = temp;
        }
        // Make num2 quite small
        num2 /= 4;
        num2 += 2; // Make sure num2 is at least 2
        num1 += 2; // Also need to add 2 to num1 to ensure its still more than num2
        // Also force the ? to always be placed at the end
        where = 2;
    }

    // Generate the question
    std::string question;
    std::string operationStr;
    int eqResult;
    if(operation < 2){
        eqResult = (operation==0)?num1 + num2:num1 - num2;
        operationStr = (operation==0)?"+":"-";
    }else{
        eqResult = (operation==2)?num1 * num2:num1 / num2;
        operationStr = (operation==2)?"*":"/";
    }

    if (where == 0){
        question = "? " + operationStr + " " + std::to_string(num2) + " = " + std::to_string(eqResult);
    } else if (where == 1){
        question = std::to_string(num1) + " " + operationStr + " ? = " + std::to_string(eqResult);
    } else {
        question = std::to_string(num1) + " " + operationStr + " " + std::to_string(num2) + " = ?";
    }

    // Generate the answer
    int answer;
    if (where == 0){ // ? <op> x = y
        answer = num1;
    }else if (where == 1){ // x <op> ? = y
        answer = num2;
    }else{ // x <op> y = ?
        answer = eqResult;
    }

    // Return the question and answer
    return {question, answer, operation==3};
}

#define qWinWidth 50
#define aWinWidth 50
#define scoreWinWidth 20
#define difficultyMultiplier 1.2

#define GREEN 1
#define RED 2
#define PURPLE 3
#define WHITE 4

int main(){
    srand(time(NULL));

    initscr();
    noecho();
    cbreak();
    curs_set(0);

    start_color();
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(PURPLE, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);

    WINDOW* questionBox = newwin(3, qWinWidth, LINES/4, (COLS/2)-(qWinWidth/2));
    refresh();
    box(questionBox, '*', '*');

    WINDOW* answerBox = newwin(3, qWinWidth, LINES/2, (COLS/2)-(qWinWidth/2));
    refresh();
    box(answerBox, '*', '*');

    WINDOW* scorebox = newwin(3, scoreWinWidth, 1, 1);
    refresh();
    box(scorebox, '*', '*');

    float score = 0;
    int questionDifficulty = 10; // Range of numbers
    question q = generateQuestion(questionDifficulty);
    int qLength = q.question.length();
    while(true){
        int inputChar = ' ';
        std::string inputStr;
        do{
            wclear(questionBox);
            box(questionBox, '*', '*');
            wclear(answerBox);
            box(answerBox, '*', '*');
            wclear(scorebox);
            box(scorebox, '*', '*');

            mvwprintw(questionBox, 1, (qWinWidth/2)-(qLength/2), q.question.c_str());
            mvwprintw(answerBox, 1, (aWinWidth/2)-(inputStr.length()/2), inputStr.c_str());
            std::string scoreStr = "Score: " + to_string_with_precision(score, 2);
            mvwprintw(scorebox, 1, (scoreWinWidth/2)-(scoreStr.length()/2), scoreStr.c_str());
            if(q.isDivision){
                mvprintw(LINES-1, 0, "Integer division, ignore the remainder!");
            }

            // for debug, print the answer
            // mvprintw(LINES-3, 0, "(DEBUG) Answer: %d", q.answer);

            wrefresh(questionBox);
            wrefresh(answerBox);
            wrefresh(scorebox);
            
            inputChar = getch();
            if(inputChar == 127){
                if(inputStr.length() > 0){
                    inputStr.pop_back();
                }
            }else{
                inputStr += inputChar;
            }
        }while(inputChar != '\n');

        clear();
        int answerInt;
        try{
            answerInt = std::stoi(inputStr);
        }catch(...){
            std::string invalidInput = "Invalid input! Press any key to retry the question";
            wattr_on(stdscr, COLOR_PAIR(PURPLE), NULL);
            mvprintw(LINES/2, (COLS/2)-(invalidInput.length()/2), invalidInput.c_str());
            wattr_off(stdscr, COLOR_PAIR(PURPLE), NULL);
            refresh();
            getch();
            continue;
        }
        if(answerInt == q.answer){
            std::string correct = "Correct! Press any key to continue to the next question";
            wattr_on(stdscr, COLOR_PAIR(GREEN), NULL);
            mvprintw(LINES/2, (COLS/2)-(correct.length()/2), correct.c_str());
            wattr_off(stdscr, COLOR_PAIR(GREEN), NULL);
            refresh();
            getch();
            clear();
            refresh();
            questionDifficulty *= difficultyMultiplier; // Increase the difficulty
            q = generateQuestion(questionDifficulty);
            qLength = q.question.length();
            score += (float)questionDifficulty/10.0f;
        }else{
            std::string incorrect = "Incorrect! Press any key to retry the question";
            wattr_on(stdscr, COLOR_PAIR(RED), NULL);
            mvprintw(LINES/2, (COLS/2)-(incorrect.length()/2), incorrect.c_str());
            wattr_off(stdscr, COLOR_PAIR(RED), NULL);
            refresh();
            getch();
            clear();
            refresh();
        }
    }

    endwin();
    return 0;
}