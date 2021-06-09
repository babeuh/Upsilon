#include "distribution_graph_view.h"

#include <kandinsky/color.h>

namespace Probability {

GraphView::GraphView(StatisticViewRange * rangeLeft,
                     StatisticViewRange * rangeRight) :
    m_separatorView(Palette::WallScreen),
    m_curveViewLeft(rangeLeft),
    m_curveViewRight(rangeRight) {
}

void GraphView::setMode(GraphDisplayMode m) {
  m_mode = m;
  markRectAsDirty(bounds());
  layoutSubviews();  // TODO have to call here for the second time to account for mode change
}

KDSize GraphView::minimalSizeForOptimalDisplay() const {
  return KDSize(10000, 10000);  // TODO compute pixel perfect
}

void GraphView::layoutSubviews(bool force) {
  int availableHeight = m_frame.height();
  int availableWidth = m_frame.width();
  int curveViewHeight = availableHeight - k_conclusionViewHeight;
  // Layout curve views and sep according to mode
  if (m_mode == GraphDisplayMode::OneCurveView) {
    m_curveViewLeft.setFrame(KDRect(KDPointZero, KDSize(availableWidth, curveViewHeight)), force);
    m_separatorView.setFrame(KDRectZero, force);
    m_curveViewRight.setFrame(KDRectZero, force);
  } else {
    int width = (availableWidth - k_separatorWidth) / 2;
    KDSize size(width, curveViewHeight);
    m_curveViewLeft.setFrame(KDRect(KDPointZero, size), force);
    m_separatorView.setFrame(KDRect(KDPoint(width, 0), KDSize(k_separatorWidth, curveViewHeight)),
                             force);
    m_curveViewRight.setFrame(KDRect(KDPoint(width + k_separatorWidth, 0), size), force);
  }
  m_conclusionView.setFrame(
      KDRect(KDPoint(0, curveViewHeight), KDSize(availableWidth, k_conclusionViewHeight)), force);
  KDSize k_legendSize = m_legend.minimalSizeForOptimalDisplay();
  m_legend.setFrame(KDRect(KDPoint(availableWidth - k_legendSize.width() - k_legendMarginRight, 20),
                           k_legendSize),
                    force);
}

Escher::View * GraphView::subviewAtIndex(int i) {
  assert(i < numberOfSubviews());
  Escher::View * subviews[] = {&m_curveViewLeft, &m_curveViewRight, &m_separatorView,
                               &m_conclusionView, &m_legend};
  return subviews[i];
}

}  // namespace Probability
