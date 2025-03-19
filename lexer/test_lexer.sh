#!/bin/bash

# Path to your minishell binary (update if needed)
MINISHELL=./minishell

# Define test cases
tests=(
    'echo hello world'
    'ls -l'
    'cat file.txt'
    'echo hello | grep foo'
    'cat < file.txt'
    'echo hello > output.txt'
    'echo "hello world"'
    'echo "hello$HOME world"'
    'echo "$HOME"'
    'echo "$"'
    'echo "$?"'
    'echo "$$"'
    'echo "$HOME$USER"'
    'echo "$HOME is home"'
    'echo "$HOME""$USER"'
    'echo "$HOME and $USER are different"'
    'echo "$HOME$USER$?"'
    'echo "$HOME$USER=homeuser"'
    'echo "$HOME_USER"'
    'echo "Special chars: \& \!"'
    'echo "Quotes inside: '\''Hello world'\''"'
    'echo "Mixed quotes: hello '\''world'\''"'
    'echo "$MYVAR is not set"'
    'echo "User: $USER, Home: $HOME"'
    'echo "Test empty var: $NOTSET"'
    'echo "Unclosed quote'
    'echo '\''Unclosed single'
    'echo "Mixing ""quotes"'
    'echo "hello""world"'
    'echo "$HOME" | grep /home'
    'echo "Text and special | chars"'
    'echo "This is a long test case to check if everything works correctly with many words"'
    'echo -n "No newline"'
    'ls | wc -l'
    'cat << EOF'
    'echo "Testing backslash \\"'
    'echo "hello" > file.txt'
    'echo "hello" >> file.txt'
    'echo "hello" 2> error.log'
    'echo hello | cat | grep test'
    'echo ls -l | grep "file" | wc -l'
)

# Run tests
echo "====================="
echo "RUNNING LEXER TESTS"
echo "====================="

for test in "${tests[@]}"; do
    echo "------------------------------"
    echo "TEST: $test"
    echo "------------------------------"
    echo -e "$test" | $MINISHELL
    echo
done

echo "====================="
echo "TESTING COMPLETE"
echo "====================="
