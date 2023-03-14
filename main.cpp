#include <ncurses.h>
#include <string>
#include <iostream>

struct question{
    std::string question;
    int answer;
};

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


int main(){
    // Print 100 random questions and answers
    for (int i = 0; i < 100; i++){
        question q = generateQuestion(i);
        std::cout << q.question << std::endl;
        std::cout << q.answer << std::endl;
        std::cout << "====================" << std::endl;
    }
    return 0;
}