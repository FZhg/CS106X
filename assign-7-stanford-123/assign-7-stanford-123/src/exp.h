/**
 * File: exp.h
 * -----------
 * This interface defines a class hierarchy for arithmetic expressions.
 */

#ifndef _exp_
#define _exp_

#include <string>
#include "ssmodel.h"
#include "cell.h"
#include "tokenscanner.h"

/*
 * Type: ExpressionType
 * --------------------
 * This enumerated type is used to differentiate the four different
 * expression types: DOUBLE, TEXTSTRING, IDENTIFIER, and COMPOUND.
 */

enum ExpressionType { DOUBLE, TEXTSTRING, IDENTIFIER, COMPOUND };

/**
 * Class: Expression
 * -----------------
 * This class is used to represent a node in an expression tree.
 * Expression itself is an abstract class, which means that there are
 * never any objects whose primary type is Expression.  All objects are
 * instead created using one of the four concrete subclasses:
 *
 *  1. DoubleExp     -- a numeric constant
 *  2. TextStringExp -- a text string constant
 *  3. IdentifierExp -- a string representing an identifier
 *  4. CompoundExp   -- two expressions combined by an operator
 *
 * The Expression class defines the interface common to all expressions;
 * each subclass provides its own implementation of the common interface.
 */

class Expression {

public:

/**
 * Constructor: Expression
 * -----------------------
 * Specifies the constructor for the base Expression class.  Each subclass
 * constructor will define its own constructor.
 */

   Expression();

/**
 * Destructor: ~Expression
 * Usage: delete exp;
 * ------------------
 * The destructor deallocates the storage for this expression.  This method
 * must be declared virtual to ensure that the correct subclass destructor
 * is called when deleting an expression.
 */

   virtual ~Expression();

/**
 * Method: eval
 * Usage: int value = exp->eval(model);
 * --------------------------------------
 * Evaluates this expression and returns its value in the to of
 * the specified SSmodel object.
 */

   virtual double eval(SSModel& model, Cell* newCell) const = 0;

/**
 * Method: toString
 * Usage: string str = exp->toString();
 * ------------------------------------
 * Returns a string representation of this expression.
 */

   virtual std::string toString() const = 0;

/**
 * Method: type
 * Usage: ExpressionType type = exp->getType();
 * --------------------------------------------
 * Returns the type of the expression, which must be one of the constants
 * DOUBLE, TEXTSTRING, IDENTIFIER, and COMPOUND.
 */

   virtual ExpressionType getType() const = 0;
};

/**
 * Subclass: DoubleExp
 * -------------------
 * This subclass represents a numeric constant.
 */

class DoubleExp : public Expression {

public:

/**
 * Constructor: DoubleExp
 * Usage: Expression *exp = new DoubleExp(value);
 * -------------------------------------------------
 * The constructor creates a new double constant expression.
 */

   DoubleExp(double value);

/* Prototypes for the virtual methods overridden by this class */

   double eval(SSModel& /*model*/, Cell* newCell) const;
   std::string toString() const;
   ExpressionType getType() const;
    
/* Prototypes of methods specific to this class */
   double getDoubleValue() const;

private:
   double value;                   /* The value of the double constant */
};

/**
 * Subclass: TextStringExp
 * -----------------------
 * This subclass represents a text string constant.
 */

class TextStringExp : public Expression {
    
public:
    
/**
 * Constructor: TextStringExp 
 * Usage: Expression *exp = new TextStringExp(str);
 * ------------------------------------------------
 * The constructor creates a new text string constant expression.
 */
    
    TextStringExp(const std::string& str);
    
/* Prototypes for the virtual methods overridden by this class */
    
    double eval(SSModel& /*model*/, Cell* /* newCell */) const;
    std::string toString() const;
    ExpressionType getType() const;

/* Prototypes of methods specific to this class */
    std::string getTextStringValue() const;
    
private:
    std::string str;              /* The value of the text string constant */
};

/**
 * Subclass: IdentifierExp
 * -----------------------
 * This subclass represents an identifier used as a variable name.
 */

class IdentifierExp : public Expression {

public:

/**
 * Constructor: IdentifierExp
 * Usage: Expression *exp = new IdentifierExp(name);
 * -------------------------------------------------
 * The constructor creates an identifier expression with the specified name.
 */

   IdentifierExp(const std::string& name);

/* Prototypes for the virtual methods overridden by this class */
   /**
    * Method: eval
    * Usage: exp.eval()
    * --------------------
    * Return the value in the indentifier.
    */
   double eval(SSModel& model, Cell* newCell) const;
   std::string toString() const;
   ExpressionType getType() const;

/* Prototypes of methods specific to this class */
   std::string getIdentifierName() const;
private:
   std::string name;            /* The name of the identifier */
};

/**
 * Subclass: CompoundExp
 * ---------------------
 * This subclass represents a compound expression consisting of
 * two subexpressions joined by an operator.
 */

class CompoundExp : public Expression {

public:

/**
 * Constructor: CompoundExp
 * Usage: Expression *exp = new CompoundExp(op, lhs, rhs);
 * -------------------------------------------------------
 * The constructor initializes a new compound expression composed of
 * the operator (op) and the left and right subexpression (lhs and rhs).
 */

   CompoundExp(const std::string& op, const Expression *lhs, const Expression *rhs);

/* Prototypes for the virtual methods overridden by this class */

   virtual ~CompoundExp();
   virtual double eval(SSModel& model, Cell* newCell) const;
   virtual std::string toString() const;
   virtual ExpressionType getType() const;

/* Prototypes of methods specific to this class */
   std::string getOperator() const;
   const Expression *getLHS() const;
   const Expression *getRHS() const;

private:
   std::string op;              /* The operator string (+, -, *, /)  */
   const Expression *lhs, *rhs; /* The left and right subexpression  */
};


#endif
