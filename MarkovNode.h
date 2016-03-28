#ifndef __MARKOV_NODE_H__
#define __MARKOV_NODE_H__

#include <map>
#include <string>
#include <vector>

class MarkovNode;
typedef std::map<std::string, float> TransitionProbs;

class MarkovNode {
    public:
        MarkovNode(std::vector<std::string> tokens, std::string next_value);
        void add_next_value(std::string next_value);
        MarkovNode* move_agent(std::map<std::string, MarkovNode*> nodes, std::vector<std::string> &output);
        void normalize();
        std::vector<std::string> getTokens();
    private:
        bool normalized_;
        int total_count_;
        std::vector<std::string> tokens_;
        TransitionProbs t_probs;
};

#endif
