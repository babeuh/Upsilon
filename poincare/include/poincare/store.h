#ifndef POINCARE_STORE_H
#define POINCARE_STORE_H

#include <poincare/expression.h>
#include <poincare/symbol.h>
#include <poincare/static_hierarchy.h>
#include <poincare/layout_helper.h>
#include <poincare/evaluation.h>

namespace Poincare {

class Store : public StaticHierarchy<2> {
public:
  using StaticHierarchy<2>::StaticHierarchy;
  Type type() const override;
  int polynomialDegree(char symbolName) const override;
private:
  /* Simplification */
  Expression shallowReduce(Context& context, Preferences::AngleUnit angleUnit) const override;
  /* Layout */
  LayoutRef createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;
  /* Evalutation */
  Evaluation<float> approximate(SinglePrecision p, Context& context, Preferences::AngleUnit angleUnit) const override { return templatedApproximate<float>(context, angleUnit); }
  Evaluation<double> approximate(DoublePrecision p, Context& context, Preferences::AngleUnit angleUnit) const override { return templatedApproximate<double>(context, angleUnit); }
  template<typename T> Evaluation<T> templatedApproximate(Context& context, Preferences::AngleUnit angleUnit) const;

  const Symbol * symbol() const { return static_cast<const Symbol *>(operand(1)); }
  const Expression * value() const { return operand(0); }
};

}

#endif
