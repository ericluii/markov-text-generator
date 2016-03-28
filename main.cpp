#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <string>
#include <sstream>
#include <time.h>
#include <vector>

#include "MarkovNode.h"

#define ERROR_BAD_INPUT -1

#define PROCESS_CHAR 0
#define PROCESS_WORD 1

typedef short ProcessType;
typedef short OrderLevel;

void exitBadInput(char* programName) {
    std::cout << "Usage: " << programName << " [char|word] [1-9] [Output Size > 0] [Input Text]" << std::endl;
    exit(ERROR_BAD_INPUT);
}

void processInputs(int &argc, char** &argv, ProcessType &type,
                  OrderLevel &order, int &output_size,
                  char* &input)
{
    if (argc != 5) {
        exitBadInput(argv[0]);
    }

    if (strncmp(argv[1], "char", 4) == 0) {
        type = PROCESS_CHAR;
    } else if (strncmp(argv[1], "word", 4) == 0) {
        type = PROCESS_WORD;
    } else {
        exitBadInput(argv[0]);
    }

    order = atoi(argv[2]);
    if (order < 1 || order > 9) {
        exitBadInput(argv[0]);
    }

    output_size = atoi(argv[3]);
    if (output_size < 1) {
        exitBadInput(argv[0]);
    }

    input = argv[4];
}

static const char filter_arr[] = {'.', ',', '\'', '\"'};
std::vector<char> filter(filter_arr, filter_arr + sizeof(filter_arr) / sizeof(filter_arr[0]));
// Increments input and returns pointer for next token
void tokenize(std::vector<std::string> &tokens, char* input, ProcessType type) {
    std::stringstream ss;

    while (*input) {
        *input = tolower(*input);

        if (std::find(filter.begin(), filter.end(), *input) != filter.end()) {
            input++;
            continue;
        }

        if (type == PROCESS_CHAR) {
            if (*input == '\n') {
                tokens.push_back(" ");
            } else {
                tokens.push_back(std::string(1, *input));
            }
        } else if (type == PROCESS_WORD) {
            if (*input == ' ' || *input == '\n') {
                tokens.push_back(ss.str());
                ss.str(std::string());
            } else {
                ss << *input;
            }
        }

        input++;
    }

    // Clear remaining word from string stream buffer
    if (type == PROCESS_WORD) {
        tokens.push_back(ss.str());
    }
}

int main(int argc, char* argv[]) {
    ProcessType type;
    OrderLevel order;
    int output_size;
    char* input;

    processInputs(argc, argv, type, order, output_size, input);

    // Tokenize input
    std::vector<std::string> tokens;
    tokenize(tokens, input, type);

    // create nodes
    std::map<std::string, MarkovNode*> nodes;
    for (int i = (int)order; i < tokens.size(); i++) {
        std::vector<std::string> value;

        // Create values
        for (int j = 0; j < order; j++) {
            value.push_back(tokens[i - order + j]);
        }

        // Normalize values
        std::string norm_val;
        norm_val = accumulate(begin(value), end(value), norm_val); 

        // Markov Node
        if (nodes.find(norm_val) != nodes.end()) {
            nodes[norm_val]->add_next_value(tokens[i]);
        } else {
            nodes[norm_val] = new MarkovNode(value, tokens[i]);
        }
    }

    // normalize nodes
    for (
        std::map<std::string, MarkovNode*>::iterator it = nodes.begin();
        it != nodes.end();
        it++
    ) {
        it->second->normalize();
    }

    std::vector<std::string> output;
    MarkovNode* current_position;

    // Pick random start point
    // Set random seed
    srand(time(NULL));
    int idx = rand() % (nodes.size() + 1);
    std::map<std::string, MarkovNode*>::iterator it = nodes.begin();
    for (int i = 0; i < idx; i++) {
        it++;
    }

    current_position = it->second;
    std::vector<std::string> current_tokens = current_position->getTokens();
    // Add starting node tokens to output
    for (int i = 0; i < current_tokens.size(); i++) {
        output.push_back(current_tokens[i]);
    }
    
    // BUild output
    while (output.size() < output_size) {
        current_position = current_position->move_agent(nodes, output);
    }

    std::cout << "Output:";
    for (int i = 0; i < output.size(); i++) {
        if (output[i].size() > 0) {
            std::cout << output[i];

            if (type == PROCESS_WORD && i != output.size() - 1) {
                std::cout << ' ';
            }
        }
    }
    std::cout << std::endl;

    return 0;
}
