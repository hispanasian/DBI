/*
 * Expression.cc
 *
 *  Created on: Apr 3, 2015
 *      Author: cvasquez
 */

#include "Expression.h"

using namespace std;

Expression::Expression(): stat(stat), type(), op() {}
Expression::Expression(ExpressionType _type, Statistics &_stat, int _op): stat(_stat),
		type(_type), op(_op) {}

Expression::~Expression() {}

BinaryExpression::BinaryExpression(Statistics &_stat, string _rel1, string _att1, string _rel2,
		string _att2, int _op): Expression(BINARY, _stat, _op), rel1(_rel1), att1(_att1),
				rel2(_rel2), att2(_att2) {}

BinaryExpression::~BinaryExpression() {}

bool BinaryExpression::Combine(Expression &e) {
	return false;
}

double BinaryExpression::Tuples() {
	return 0;
}

double BinaryExpression::Numerator() {
	return 0;
}

double BinaryExpression::Denominator() {
	return 0;
}

UnaryExpression::UnaryExpression(Statistics &_stat, string _rel, string _att, string lit, int _op):
		Expression(UNARY, _stat, _op), rel(_rel), att(_att) {
	literals.insert(lit);
}

UnaryExpression::~UnaryExpression() {}

bool UnaryExpression::Combine(Expression &e) {
	return false;
}

double UnaryExpression::Tuples() {
	return 0;
}

double UnaryExpression::Numerator() {
	return 0;
}

double UnaryExpression::Denominator() {
	return 0;
}
