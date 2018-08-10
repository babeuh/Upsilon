#include <poincare/matrix_complex.h>
//#include <poincare/matrix.h>
#include <poincare/expression.h>
//#include <poincare/undefined.h>
#include <ion.h>
#include <cmath>
#include <assert.h>
#include <float.h>

namespace Poincare {

template<typename T>
MatrixComplexNode<T> * MatrixComplexNode<T>::FailedAllocationStaticNode() {
  static AllocationFailureMatrixComplexNode<T> failure;
  TreePool::sharedPool()->registerStaticNodeIfRequired(&failure);
  return &failure;
}

template<typename T>
std::complex<T> MatrixComplexNode<T>::complexAtIndex(int index) const {
  EvaluationNode<T> * child = EvaluationNode<T>::childAtIndex(index);
  if (child->type() == EvaluationNode<T>::Type::Complex) {
    return *(static_cast<ComplexNode<T> *>(child));
  }
  return std::complex<T>(NAN, NAN);
}

template<typename T>
bool MatrixComplexNode<T>::isUndefined() const {
  if (numberOfRows() != 1 || numberOfColumns() != 1) {
    return false;
  }
  EvaluationNode<T> * child = const_cast<MatrixComplexNode<T> *>(this)->childAtIndex(0);
  if (child->type() == EvaluationNode<T>::Type::Complex && std::isnan(static_cast<ComplexNode<T> *>(child)->real()) && std::isnan(static_cast<ComplexNode<T> *>(child)->imag())) {
    return true;
  }
  return false;
}

template<typename T>
Expression MatrixComplexNode<T>::complexToExpression(Preferences::ComplexFormat complexFormat) const {
  return Undefined();/* TODO
  Matrix matrix;
  for (int i = 0; i < numberOfChildren(); i++) {
    EvaluationNode<T> * child = childAtIndex(i);
    if (child->type() == EvaluationNode<T>::Type::Complex) {
      matrix.addChildTreeAtIndex(child->complexToExpression(complexFormat), i, i);
    } else {
      matrix.addChildTreeAtIndex(Undefined(), i, i);
    }
  }
  matrix.setDimensions(numberOfRows(), numberOfColumns());
  return matrix;*/
}

template<typename T>
std::complex<T> MatrixComplexNode<T>::trace() const {
  if (numberOfRows() != numberOfColumns() || numberOfRows() == 0) {
    return std::complex<T>(NAN, NAN);
  }
  int dim = numberOfRows();
  std::complex<T> c = std::complex<T>(0);
  for (int i = 0; i < dim; i++) {
    c += complexAtIndex(i*dim+i);
    if (std::isnan(c.real()) || std::isnan(c.imag())) {
      return std::complex<T>(NAN, NAN);
    }
  }
  return c;
}

template<typename T>
std::complex<T> MatrixComplexNode<T>::determinant() const {
/* TODO  if (numberOfRows() != numberOfColumns() || numberOfChildren() == 0 || numberOfChildren() > Matrix::k_maxNumberOfCoefficients) {
    return std::complex<T>(NAN, NAN);
  }
  std::complex<T> operandsCopy[Matrix::k_maxNumberOfCoefficients];
  for (int i=0; i<m_numberOfRows*m_numberOfColumns; i++) {
    EvaluationNode<T> * child = childAtIndex(i);
    if (child->type() != EvaluationNode<T>::Type::Complex) {
      return std::complex<T>(NAN, NAN);
    }
    operandsCopy[i] = *child;
  }*/
  std::complex<T> determinant = std::complex<T>(1);
  //TODO Matrix::ArrayRowCanonize(operandsCopy, m_numberOfRows, m_numberOfColumns, &determinant);
  return determinant;
}

template<typename T>
MatrixComplex<T> MatrixComplexNode<T>::inverse() const {
/* TODO  if (numberOfRows() != numberOfColumns() || numberOfChildren() == 0 || numberOfChildren() > Matrix::k_maxNumberOfCoefficients) {
    return MatrixComplex<T>::Undefined();
  }
  std::complex<T> operandsCopy[Matrix::k_maxNumberOfCoefficients];
  for (int i=0; i<m_numberOfRows*m_numberOfColumns; i++) {
    EvaluationNode<T> * child = childAtIndex(i);
    if (child->type() != EvaluationNode<T>::Type::Complex) {
      return Evaluation<T>(EvaluationNode<T>::FailedAllocationStaticNode());
    }
    operandsCopy[i] = *child;
  }
  int result = Matrix::ArrayInverse(operandsCopy, m_numberOfRows, m_numberOfColumns);
  if (result == 0) {
    // Intentionally swapping dimensions for inverse, although it doesn't make a difference because it is square
    return MatrixComplex<T>(operandsCopy, m_numberOfColumns, m_numberOfRows);
  }*/
  return MatrixComplex<T>::Undefined();
}

template<typename T>
MatrixComplex<T> MatrixComplexNode<T>::transpose() const {
  // Intentionally swapping dimensions for transpose
  MatrixComplex<T> result;
  for (int i = 0; i < numberOfRows(); i++) {
    for (int j = 0; j < numberOfColumns(); j++) {
      result.addChildAtIndexInPlace(Complex<T>(complexAtIndex(i*numberOfColumns()+i)), j*numberOfRows()+i, j*numberOfRows()+i);
    }
  }
  result.setDimensions(numberOfColumns(), numberOfRows());
  return result;
}

// MATRIX COMPLEX REFERENCE

template<typename T>
MatrixComplex<T>::MatrixComplex(std::complex<T> * operands, int numberOfRows, int numberOfColumns) :
  MatrixComplex<T>()
{
  for (int i=0; i<numberOfRows*numberOfColumns; i++) {
    addChildAtIndexInPlace(Complex<T>(operands[i]), i, i);
  }
  setDimensions(numberOfRows, numberOfColumns);
}

template<typename T>
MatrixComplex<T> MatrixComplex<T>::createIdentity(int dim) {
  MatrixComplex<T> result;
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      Complex<T> c = i == j ? Complex<T>(1.0) : Complex<T>(0.0);
      result.addChildAtIndexInPlace(c, i*dim+j, i*dim+j);
    }
  }
  result.setDimensions(dim, dim);
  return result;
}

template<typename T>
void MatrixComplex<T>::setDimensions(int rows, int columns) {
  // FIXME: //assert(rows * columns == numberOfChildren());
  setNumberOfRows(rows);
  setNumberOfColumns(columns);
}

template<typename T>
void MatrixComplex<T>::addChildAtIndexInPlace(Complex<T> t, int index, int currentNumberOfChildren) {
  Evaluation<T>::addChildAtIndexInPlace(t, index, currentNumberOfChildren);
  setNumberOfRows(1);
  setNumberOfColumns(currentNumberOfChildren + 1);
}

template class MatrixComplex<float>;
template class MatrixComplex<double>;

}
