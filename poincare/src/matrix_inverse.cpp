#include <poincare/matrix_inverse.h>
#include <poincare/matrix.h>
#include <poincare/complex.h>
#include <poincare/fraction.h>
extern "C" {
#include <assert.h>
}
#include <cmath>

namespace Poincare {

Expression::Type MatrixInverse::type() const {
  return Type::MatrixInverse;
}

Expression * MatrixInverse::clone() const {
  MatrixInverse * a = new MatrixInverse(m_operands, true);
  return a;
}

bool MatrixInverse::isCommutative() const {
  return false;
}

template<typename T>
Evaluation<T> * MatrixInverse::templatedEvaluate(Context& context, AngleUnit angleUnit) const {
  Evaluation<T> * input = operand(0)->evaluate<T>(context, angleUnit);
  Evaluation<T> * result = input->createInverse();
  delete input;
  return result;
}

}

