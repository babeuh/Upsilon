#ifndef GRAPH_GRAPH_CURVE_PARAMETER_CONTROLLER_H
#define GRAPH_GRAPH_CURVE_PARAMETER_CONTROLLER_H

#include "../../shared/storage_function_curve_parameter_controller.h"
#include "calculation_parameter_controller.h"
#include "banner_view.h"

namespace Graph {

class GraphController;

class CurveParameterController : public Shared::StorageFunctionCurveParameterController {
public:
  CurveParameterController(Shared::InteractiveCurveViewRange * graphRange, BannerView * bannerView, Shared::CurveViewCursor * cursor, GraphView * graphView, GraphController * graphController, StorageCartesianFunctionStore * functionStore);
  const char * title() override;
  bool handleEvent(Ion::Events::Event event) override;
  int numberOfRows() override;
  HighlightCell * reusableCell(int index) override;
  int reusableCellCount() override;
  void willDisplayCellForIndex(HighlightCell * cell, int index) override;
private:
  Shared::StorageFunctionGoToParameterController * goToParameterController() override;
  Shared::StorageFunctionGoToParameterController m_goToParameterController;
  GraphController * m_graphController;
  constexpr static int k_totalNumberOfCells = 3;
  MessageTableCellWithChevron m_calculationCell;
  MessageTableCellWithSwitch m_derivativeCell;
  CalculationParameterController m_calculationParameterController;
};

}

#endif
