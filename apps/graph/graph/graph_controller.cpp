#include "graph_controller.h"
#include "../app.h"

using namespace Shared;

namespace Graph {

GraphController::GraphController(Responder * parentResponder, StorageCartesianFunctionStore * functionStore, Shared::InteractiveCurveViewRange * curveViewRange, CurveViewCursor * cursor, int * indexFunctionSelectedByCursor, uint32_t * modelVersion, uint32_t * rangeVersion, Poincare::Preferences::AngleUnit * angleUnitVersion, ButtonRowController * header) :
  StorageFunctionGraphController(parentResponder, header, curveViewRange, &m_view, cursor, indexFunctionSelectedByCursor, modelVersion, rangeVersion, angleUnitVersion),
  m_bannerView(),
  m_view(functionStore, curveViewRange, m_cursor, &m_bannerView, &m_cursorView),
  m_graphRange(curveViewRange),
  m_curveParameterController(curveViewRange, &m_bannerView, m_cursor, &m_view, this, functionStore),
  m_functionStore(functionStore),
  m_displayDerivativeInBanner(false)
{
  m_graphRange->setDelegate(this);
}

I18n::Message GraphController::emptyMessage() {
  if (m_functionStore->numberOfDefinedModels() == 0) {
    return I18n::Message::NoFunction;
  }
  return I18n::Message::NoActivatedFunction;
}

void GraphController::viewWillAppear() {
  m_view.drawTangent(false);
  StorageFunctionGraphController::viewWillAppear();
  selectFunctionWithCursor(indexFunctionSelectedByCursor()); // update the color of the cursor
}

bool GraphController::displayDerivativeInBanner() const {
  return m_displayDerivativeInBanner;
}

void GraphController::setDisplayDerivativeInBanner(bool displayDerivative) {
  m_displayDerivativeInBanner = displayDerivative;
}

float GraphController::interestingXRange() {
  float characteristicRange = 0.0f;
  TextFieldDelegateApp * myApp = (TextFieldDelegateApp *)app();
  for (int i = 0; i < functionStore()->numberOfActiveFunctions(); i++) {
    StorageFunction * f = functionStore()->activeFunctionAtIndex(i);
    float fRange = f->expression(myApp->localContext()).characteristicXRange(*(myApp->localContext()), Poincare::Preferences::sharedPreferences()->angleUnit());
    if (!std::isnan(fRange)) {
      characteristicRange = fRange > characteristicRange ? fRange : characteristicRange;
    }
  }
  return (characteristicRange > 0.0f ? 1.6f*characteristicRange : 10.0f);
}

int GraphController::estimatedBannerNumberOfLines() const {
  return 1 + m_displayDerivativeInBanner;
}

void GraphController::selectFunctionWithCursor(int functionIndex) {
  StorageFunctionGraphController::selectFunctionWithCursor(functionIndex);
  StorageCartesianFunction * f = m_functionStore->activeFunctionAtIndex(indexFunctionSelectedByCursor());
  m_cursorView.setColor(f->color());
}

BannerView * GraphController::bannerView() {
  return &m_bannerView;
}

void GraphController::reloadBannerView() {
  StorageFunctionGraphController::reloadBannerView();
  m_bannerView.setNumberOfSubviews(2+m_displayDerivativeInBanner);
  if (m_functionStore->numberOfActiveFunctions() == 0 || !m_displayDerivativeInBanner) {
    return;
  }
  StorageCartesianFunction * f = m_functionStore->activeFunctionAtIndex(indexFunctionSelectedByCursor());
  TextFieldDelegateApp * myApp = (TextFieldDelegateApp *)app();
  reloadDerivativeInBannerViewForCursorOnFunction(m_cursor, f, myApp);
}

bool GraphController::moveCursorHorizontally(int direction) {
  StorageCartesianFunction * f = m_functionStore->activeFunctionAtIndex(indexFunctionSelectedByCursor());
  TextFieldDelegateApp * myApp = (TextFieldDelegateApp *)app();
  return privateMoveCursorHorizontally(m_cursor, direction, m_graphRange, k_numberOfCursorStepsInGradUnit, f, myApp, cursorTopMarginRatio(), k_cursorRightMarginRatio, cursorBottomMarginRatio(), k_cursorLeftMarginRatio);
}

InteractiveCurveViewRange * GraphController::interactiveCurveViewRange() {
  return m_graphRange;
}

StorageCartesianFunctionStore * GraphController::functionStore() const {
  return m_functionStore;
}

GraphView * GraphController::functionGraphView() {
  return &m_view;
}

CurveParameterController * GraphController::curveParameterController() {
  return &m_curveParameterController;
}

}
