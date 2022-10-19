#ifndef KANDINSKY_PIXEL_CACHE_H
#define KANDINSKY_PIXEL_CACHE_H

#include <kandinsky/color.h>
#include <kandinsky/context.h>
#include <kandinsky/rect.h>
#include "measuring_context.h"

class KDAbstractPixelCache {
public:
  KDAbstractPixelCache() : m_rect(KDRectZero) {}

  void save(KDContext * ctx, KDRect rect) {
    assert(rect.width() * rect.height() <= Size);
    ctx->getPixels(rect, buffer());
    m_rect = rect;
  }

  void restore(KDContext * ctx) {
    ctx->fillRectWithPixels(m_rect, buffer(), buffer());
    m_rect = KDRectZero;
  }

  /* Warning : the provided function is evaluated twice, the first time to
   * compute the bounding rect of the actual drawings and the second time will
   * real drawing calls.
   *
   * It is templated on the function to accept closures with bindings (that are
   * more readable than void * ones). Make sure it does not add too much code in
   * the flash when using it.
   */
  template<class T> void saveAndDraw(KDContext * ctx, T function) {
    KDMeasuringContext measuringContext(*ctx);
    function(&measuringContext);
    save(ctx, measuringContext.writtenRect());
    function(ctx);
  }

private:
  virtual KDColor * buffer() = 0;
  KDRect m_rect;
};

template <unsigned Size>
class KDPixelCache : public KDAbstractPixelCache {
public:
  using KDAbstractPixelCache::KDAbstractPixelCache;

private:
  KDColor * buffer() override { return m_buffer; }
  KDColor m_buffer[Size];
};

#endif
