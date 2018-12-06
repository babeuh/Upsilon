#include <poincare/square_root.h>
#include <poincare/power.h>
#include <poincare/addition.h>
#include <poincare/subtraction.h>
#include <poincare/layout_helper.h>
#include <poincare/serialization_helper.h>
#include <poincare/simplification_helper.h>
#include <poincare/nth_root_layout.h>
#include <poincare/division.h>
#include <poincare/sign_function.h>
#include <assert.h>
#include <cmath>
#include <ion.h>

namespace Poincare {

constexpr Expression::FunctionHelper SquareRoot::s_functionHelper;

int SquareRootNode::numberOfChildren() const { return SquareRoot::s_functionHelper.numberOfChildren(); }

Expression SquareRootNode::complexNorm(Context & context, Preferences::AngleUnit angleUnit) const {
  Expression r = childAtIndex(0)->complexNorm(context, angleUnit);
  if (r.isUninitialized()) {
    return Expression();
  }
  // R = sqrt(r)
  return SquareRoot::Builder(r).shallowReduce(context, angleUnit, ReductionTarget::BottomUpComputation);
}

/*Expression SquareRootNode::complexArgument(Context & context, Preferences::AngleUnit angleUnit) const {
  Expression th = childAtIndex(0)->complexArgument(context, angleUnit);
  if (th.isUninitialized()) {
    return Expression();
  }
  // TH = th/2
  return Division(th, Rational(2)).shallowReduce(context, angleUnit, ReductionTarget::BottomUpComputation);
}*/

Expression SquareRootNode::complexPart(Context & context, Preferences::AngleUnit angleUnit, bool real) const {
  // real: (1/2)*sqrt(2*(sqrt(x^2+y^2)+x))
  // imag: (1/2)*sqrt(2*(sqrt(x^2+y^2)-x)*sign(y))
  SquareRoot e(this);
  Expression x = e.childAtIndex(0).realPart(context, angleUnit);
  Expression y = e.childAtIndex(0).imaginaryPart(context, angleUnit);
  Expression norm = e.childAtIndex(0).complexNorm(context, angleUnit);
  if (x.isUninitialized() || y.isUninitialized()) {
    return Expression();
  }
  // a = sqrt(x^2+y^2)+x or
  Expression a = real ? static_cast<Expression>(Addition(norm, x)) : static_cast<Expression>(Subtraction(norm, x));
  Multiplication res =
    Multiplication(
      Rational(1,2),
      SquareRoot::Builder(
        Multiplication(
          Rational(2),
          a.shallowReduce(context, angleUnit, ReductionTarget::BottomUpComputation)
        ).shallowReduce(context, angleUnit, ReductionTarget::BottomUpComputation)
      ).shallowReduce(context, angleUnit, ReductionTarget::BottomUpComputation)
    );
  if (!real) {
    res.addChildAtIndexInPlace(SignFunction::Builder(y).shallowReduce(context, angleUnit), res.numberOfChildren(), res.numberOfChildren());
  }
  return res.shallowReduce(context, angleUnit, ReductionTarget::BottomUpComputation);
}

Layout SquareRootNode::createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return NthRootLayout(childAtIndex(0)->createLayout(floatDisplayMode, numberOfSignificantDigits));
}

int SquareRootNode::serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const {
  return SerializationHelper::Prefix(this, buffer, bufferSize, floatDisplayMode, numberOfSignificantDigits, SquareRoot::s_functionHelper.name());
}

template<typename T>
Complex<T> SquareRootNode::computeOnComplex(const std::complex<T> c, Preferences::AngleUnit angleUnit) {
  std::complex<T> result = std::sqrt(c);
  /* Openbsd trigonometric functions are numerical implementation and thus are
   * approximative.
   * The error epsilon is ~1E-7 on float and ~1E-15 on double. In order to avoid
   * weird results as sqrt(-1) = 6E-16+i, we compute the argument of the result
   * of sqrt(c) and if arg ~ 0 [Pi], we discard the residual imaginary part and
   * if arg ~ Pi/2 [Pi], we discard the residual real part.*/
  return Complex<T>(ApproximationHelper::TruncateRealOrImaginaryPartAccordingToArgument(result));
}

Expression SquareRootNode::shallowReduce(Context & context, Preferences::AngleUnit angleUnit, ReductionTarget target) {
  return SquareRoot(this).shallowReduce(context, angleUnit, target);
}

Expression SquareRoot::shallowReduce(Context & context, Preferences::AngleUnit angleUnit, ExpressionNode::ReductionTarget target) {
  {
    Expression e = Expression::defaultShallowReduce(context, angleUnit);
    if (e.isUndefined()) {
      return e;
    }
  }
#if MATRIX_EXACT_REDUCING
  if (childAtIndex(0).type() == ExpressionNode::Type::Matrix) {
    return SimplificationHelper::Map(this, context, angleUnit);
  }
#endif
  Power p = Power(childAtIndex(0), Rational(1, 2));
  replaceWithInPlace(p);
  return p.shallowReduce(context, angleUnit, target);
}

}
