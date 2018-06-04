#ifndef REGRESSION_LINEAR_MODEL_H
#define REGRESSION_LINEAR_MODEL_H

#include "regression_model.h"

namespace Regression {

class LinearModel : public RegressionModel {
public:
  using RegressionModel::RegressionModel;
  double evaluate(double * modelCoefficients, double x) const override;
  double partialDerivate(double * modelCoefficients, int derivateCoefficientIndex, double x) const override;
  double numberOfCoefficients() const override { return 2; }
};

}


#endif
