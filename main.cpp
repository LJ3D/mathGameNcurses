#include <ncurses.h>
#include <string>
#include <iostream>

struct question{
    std::string question;
    int answer;
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

    // Generate two random numbers from 0 to range
    int num1 = rand() % range;
    int num2 = rand() % range;

    // Generate a random number from 0 to 2 to decide where to put the ?
    int where = rand() % 3;

    // Generate a random number from 0 to 1 to decide what operation to use
    // 0 = +
    // 1 = -
    int operation = rand() % 2;

    // Generate the question
    std::string question;
    std::string op = (operation==0)?"+":"-";
    int eqResult = (operation==0)?num1 + num2:num1 - num2;
    if (where == 0){
        question = "? " + op + " " + std::to_string(num2) + " = " + std::to_string(eqResult);
    } else if (where == 1){
        question = std::to_string(num1) + " " + op + " ? = " + std::to_string(eqResult);
    } else {
        question = std::to_string(num1) + " " + op + " " + std::to_string(num2) + " = ?";
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
    return {question, answer};
}

const int qWinWidth = 50;
const int aWinWidth = 50;

int main(){
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    
    WINDOW* questionBox = newwin(3, qWinWidth, LINES/4, (COLS/2)-(qWinWidth/2));
    refresh();
    box(questionBox, '*', '*');

    WINDOW* answerBox = newwin(3, qWinWidth, LINES/2, (COLS/2)-(qWinWidth/2));
    refresh();
    box(answerBox, '*', '*');

    while(true){
        question q = generateQuestion(10);
        int qLength = q.question.length();

        int inputChar = ' ';
        std::string inputStr;

        do{
            wclear(questionBox);
            box(questionBox, '*', '*');
            wclear(answerBox);
            box(answerBox, '*', '*');
            
            mvwprintw(questionBox, 1, (qWinWidth/2)-(qLength/2), q.question.c_str());
            mvwprintw(answerBox, 1, (aWinWidth/2)-(inputStr.length()/2), inputStr.c_str());

            wrefresh(questionBox);
            wrefresh(answerBox);
            
            inputChar = getch();
            inputStr += inputChar;
        }while(inputChar != '\n');

    }

    endwin();
    return 0;
}