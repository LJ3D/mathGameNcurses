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

    question q = generateQuestion(10);
    int qLength = q.question.length();
    while(true){

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
            std::string invalidInput = "Invalid input";
            mvprintw(LINES/2, (COLS/2)-(invalidInput.length()/2), invalidInput.c_str());
            refresh();
            getch();
            continue;
        }


        if(answerInt == q.answer){
            std::string correct = "Correct! Press any key to continue to the next question";
            mvprintw(LINES/2, (COLS/2)-(correct.length()/2), correct.c_str());
            refresh();
            getch();
            clear();
            refresh();
            q = generateQuestion(10);
            qLength = q.question.length();
        }else{
            std::string incorrect = "Incorrect! Press any key to retry the question";
            mvprintw(LINES/2, (COLS/2)-(incorrect.length()/2), incorrect.c_str());
            refresh();
            getch();
            clear();
            refresh();
        }

    }

    endwin();
    return 0;
}