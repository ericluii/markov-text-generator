#include <assert.h>
#include <iostream>
#include <numeric>
#include <string>
#include <time.h>

#include "MarkovNode.h"

MarkovNode::MarkovNode(std::vector<std::string> tokens, std::string next_value)
{
    normalized_ = false;
    tokens_ = tokens;
    total_count_ = 1;
    t_probs[next_value] = 1;
}

void MarkovNode::add_next_value(std::string next_value)
{
    if (t_probs.find(next_value) != t_probs.end()) {
        t_probs[next_value]++;
    } else {
        t_probs[next_value] = 1;
    }

    total_count_++;
}

void MarkovNode::normalize() {
    assert(!normalized_);

    for (
        std::map<std::string, float>::iterator it = t_probs.begin();
        it != t_probs.end();
        it++
    ) {
        t_probs[it->first] = it->second / total_count_;
    }

    normalized_ = true;
}

MarkovNode* MarkovNode::move_agent(std::map<std::string, MarkovNode*> nodes, std::vector<std::string> &output)
{
    assert(normalized_);
    assert(output.size() >= tokens_.size());

    srand(time(NULL));
    float rand_num = ((float) rand() / (RAND_MAX));
    float accum = 0;
    for (
        std::map<std::string, float>::iterator it = t_probs.begin();
        it != t_probs.end();
        it++
    ) {
        accum += it->second;

        if (accum > rand_num) {
            output.push_back(it->first);
            break;
        }
    }

    std::string norm_val;
    norm_val = accumulate(end(output) - tokens_.size(), end(output), norm_val);

    if (nodes.find(norm_val) != nodes.end()) {
        return nodes[norm_val];
    } else {
        int idx = rand() % (nodes.size() + 1);
        std::map<std::string, MarkovNode*>::iterator it = nodes.begin();
        std::next(it, idx);

        std::vector<std::string> new_t = it->second->getTokens();
        for (int i = 0; i < new_t.size(); i++) {
            output.push_back(new_t[i]);
        }

        return it->second;
    }
}

std::vector<std::string> MarkovNode::getTokens() {
    return tokens_;
}
