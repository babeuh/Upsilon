#ifndef POINCARE_LIST_SUM_H
#define POINCARE_LIST_SUM_H

#include <poincare/list.h>

namespace Poincare {

class ListSumNode : public ExpressionNode {
public:
  template<typename T> static Evaluation<T> SumOfListNode(ListNode * list, ApproximationContext approximationContext);

  size_t size() const override { return sizeof(ListSumNode); }
  int numberOfChildren() const override;
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "ListSum";
  }
#endif
  Type type() const override { return Type::ListSum; }
  LayoutShape leftLayoutShape() const override { return LayoutShape::MoreLetters; };
  LayoutShape rightLayoutShape() const override { return LayoutShape::BoundaryPunctuation; }

private:
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;
  Layout createLayout(Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;

  Expression shallowReduce(ReductionContext reductionContext) override;

  Evaluation<float> approximate(SinglePrecision p, ApproximationContext approximationContext) const override { return templatedApproximate<float>(approximationContext); }
  Evaluation<double> approximate(DoublePrecision p, ApproximationContext approximationContext) const override { return templatedApproximate<double>(approximationContext); }
  template<typename T> Evaluation<T> templatedApproximate(ApproximationContext approximationContext) const;
};

class ListSum : public Expression {
public:
  static constexpr FunctionHelper s_functionHelper = FunctionHelper("sum", 1, &UntypedBuilderOneChild<ListSum>);

  ListSum(const ListSumNode * n) : Expression(n) {}
  static ListSum Builder(Expression list) { return TreeHandle::FixedArityBuilder<ListSum, ListSumNode>({list}); }

  Expression shallowReduce(ExpressionNode::ReductionContext reductionContext);
};

}

#endif
