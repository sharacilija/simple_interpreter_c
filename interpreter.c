#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LENGTH 1000

#define INT 0
#define OPERATOR 1
#define ERR -1
#define INT_STR "INT"
#define OPERATOR_STRING "OPERATOR"

#define EOF_TOKEN 2
#define EOF_STR "EOF"

typedef struct {
    int type; // either INT or OP
    char operator_value;
    int number_value;
} Token;

size_t current_position = 0;
Token current_token;

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

bool is_operator(char c)
{
    return c == '+' || c == '-';
}

bool is_whitespace(char c)
{
    return c == ' ' ||
           c == '\t' ||
           c == '\n' ||
           c == '\r';
}

// sets the current token and advances the current
// position pointer
void get_next_token(char *text)
{
    char c = text[current_position];
    
    // ignore whitespaces
    while (is_whitespace(c))
    {
        current_position++;
        c = text[current_position];
    }

    // Is this character a digit? -> 
    // number token, else it's an operator
    if (c == '\0')
    {
        current_token.type = EOF_TOKEN;
        return;
    }

    if (is_digit(c))
    {
        int current_number = c - '0';
        current_position++;
        while (is_digit(text[current_position]))
        {
            current_number *= 10;
            current_number += (text[current_position] - '0');
            current_position++;
        }

        current_token.type = INT;
        current_token.number_value = current_number;
        current_token.operator_value = ' ';
    }
    else if (is_operator(c))
    {
        current_token.type = OPERATOR;
        current_token.number_value = 0;
        current_token.operator_value = c;
        current_position++;
    }
    else 
    {
        printf("Invalid token recieved at pos: %zu: %c\n", current_position+1, c);
        exit(-1);
    }
}

// represents the token as string in given string s
void token_str(Token token, char *s)
{
    sprintf(s, "Token (type=%s, number_value=%d, operator_value=%c)", 
        token.type == INT ? INT_STR : OPERATOR_STRING, 
        token.number_value, 
        token.operator_value);
}

void parse(int type)
{
    if (current_token.type != type)
    {
        char s[100];
        token_str(current_token, s);
        printf("Syntax error at pos %zu: Expected token of type %s, but recieved token %s\n", 
            current_position,
            type == INT ? INT_STR : OPERATOR_STRING, 
            s);
        exit(-1);
    }
}

int interpret(char *text)
{
    get_next_token(text);
    parse(INT);

    int result = current_token.number_value;

    while (true)
    {
        get_next_token(text);

        if (current_token.type == EOF_TOKEN)
            break;

        parse(OPERATOR);
        char operator = current_token.operator_value;

        get_next_token(text);
        parse(INT);

        if (operator == '+')
            result += current_token.number_value;
        else
            result -= current_token.number_value; 
    }

    current_position = 0;
    return result;
}

int main()
{
    char s[INPUT_LENGTH];
    printf("Interpreter running...\n");

    int result;
    while (true)
    {
        printf(">>> ");
        fgets(s, INPUT_LENGTH, stdin);
        result = interpret(s);
        printf("%d\n", result);
    }
    return 0;
} 
