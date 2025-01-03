//
// Created by Snehasis Sheat on 03/01/25.
//
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "parser.h"
using namespace std;

struct FetchNode;
struct WhereNode;
struct GroupByNode;
struct HavingNode;
struct OrderByNode;
struct ExpressionNode;

enum class OrderDirection {
    ASC,
    DESC,
};
enum class ComparisonOp {
    EQ,
    NEQ,
    LT,
    GT,
    LTE,
    GTE,
    IN,
    LIKE
};
struct Astnode {
    virtual ~Astnode() = default;
};

struct ExpressionNode : Astnode {
    enum class Type {
        COLUMN,
        OPERATOR,
        LITERAL,

    };
    Type type;
    string value;
    vector<unique_ptr<ExpressionNode>> arguments;

    ExpressionNode(Type t, const string& val):type(t),value(val){};
};

struct FetchNode :Astnode {
    vector<unique_ptr<ExpressionNode>> fetchList;
    unique_ptr<WhereNode> whereClause;
    unique_ptr<GroupByNode> groupByClause;
    unique_ptr<HavingNode> havingClause;
    unique_ptr<OrderByNode> orderByClause;

};

struct WhereNode : Astnode {
    unique_ptr<ExpressionNode> condition;
};

struct GroupByNode : Astnode {
    vector<unique_ptr<ExpressionNode>> columns;
};

struct HavingNode : Astnode {
    unique_ptr<ExpressionNode> condition;

};

struct OrderByNode : Astnode {
    struct OrderElement {
        unique_ptr<ExpressionNode> expression;
        OrderDirection direction;
    };
    vector<OrderElement> elements;

};

class Parser {
        vector<string>::const_iterator currentToken;
        vector<string>::const_iterator end;

        string current() const {
            return currentToken != end ? *currentToken : "";
        }

        void advance() {
            if (currentToken == end)
                ++currentToken;
        }

        bool match(const string& expected) {
            if (current() == expected) {
                advance();
                return true;
            }
            return false;
        }

   public:
        Parser(const vector<string>& tokens) {
            currentToken = tokens.begin();
            end = tokens.end();
        }

        unique_ptr<FetchNode> parseSelect() {
            auto fetchNode = make_unique<FetchNode>();

            if (!match("SELECT")) {
                throw runtime_error("Expected 'SELECT'");
            }

            do {
                fetchNode->fetchList.push_back(parseExpression());
            }while (match(","));

            if (!match("FROM")) {
                throw runtime_error("Expected 'FROM'");
            }

            if (match("WHERE")) {
                fetchNode->whereClause = make_unique<WhereNode>();
                fetchNode->whereClause->condition = parseExpression();
            }
            if (match("GROUP")) {
                if (!match("BY")) {
                    throw runtime_error("Expected BY after GROUP");
                }
                fetchNode->groupByClause = make_unique<GroupByNode>();
            }
            if (match("ORDER")) {
                if (!match("BY")) {
                    throw runtime_error("Expected BY after ORDER");
                }
                fetchNode->orderByClause = parseOrderBy();
            }
            return fetchNode;

        }
    private:
        unique_ptr<ExpressionNode> parseExpression() {
            string value = current();
            advance();
            return make_unique<ExpressionNode>(ExpressionNode::Type::COLUMN, value);
        }

        unique_ptr<GroupByNode> parseGroupBy() {
            auto groupBy = make_unique<GroupByNode>();
            do {
                groupBy->columns.push_back(parseExpression());
            }while (match(","));
            return groupBy;
        }
        unique_ptr<OrderByNode> parseOrderBy() {
            auto orderBy = make_unique<OrderByNode>();
            do {
                OrderByNode::OrderElement element;
                element.expression = parseExpression();
                element.direction = match("ASC") ? OrderDirection::DESC : OrderDirection::ASC;
                orderBy->elements.push_back(move(element));
            }while (match(","));
            return orderBy;
        }


};
