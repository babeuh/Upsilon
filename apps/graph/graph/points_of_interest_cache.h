#ifndef GRAPH_POINTS_OF_INTEREST_CACHE
#define GRAPH_POINTS_OF_INTEREST_CACHE

#include <poincare/point_of_interest.h>
#include <poincare/range.h>
#include <ion/storage/record.h>

namespace Graph {

class PointsOfInterestCache {
public:
  PointsOfInterestCache() : m_start(NAN), m_end(NAN) {}

  void setRecord(Ion::Storage::Record record) { m_record = record; }
  Poincare::Range1D setBoundsAndCompute(float start, float end, float maxFloat);
  Poincare::PointOfInterest pointAtIndex(int i) const { return m_list.pointAtIndex(i); }
  Poincare::PointOfInterest firstPointInDirection(double start, double end, Poincare::Solver<double>::Interest interest = Poincare::Solver<double>::Interest::None) const;
  Poincare::PointOfInterest pointOfInterestAtAbscissa(double x) const;

  /* The following classes and methods are used to iterate other a certain type
   * of points of interest. */
  class Iterator {
  public:
    Iterator(int index, const PointsOfInterestCache * list, Poincare::Solver<double>::Interest interest) : m_index(index), m_list(list), m_interest(interest) {}
    Iterator & operator++();
    Poincare::PointOfInterest operator*() { return m_list->pointAtIndex(m_index); }
    bool operator!=(const Iterator & other) const { return m_index != other.m_index; }

  private:
    int m_index;
    const PointsOfInterestCache * m_list;
    Poincare::Solver<double>::Interest m_interest;
  };

  class Filter {
  public:
    Filter(const PointsOfInterestCache * list, Poincare::Solver<double>::Interest interest) : m_list(list), m_interest(interest) {}
    Iterator begin() const { return ++Iterator(-1, m_list, m_interest); }
    Iterator end() const { return Iterator(m_list->m_list.numberOfPoints(), m_list, m_interest); }

  private:
    const PointsOfInterestCache * m_list;
    Poincare::Solver<double>::Interest m_interest;
  };

  Filter filter(Poincare::Solver<double>::Interest interest = Poincare::Solver<double>::Interest::None) const { return Filter(this, interest); }

private:
  void stripOutOfBounds();
  void computeBetween(float start, float end, float maxFloat, Poincare::Range1D * dirtyRange);
  void append(double x, double y, float maxFloat, Poincare::Solver<double>::Interest, Poincare::Range1D * dirtyRange, uint32_t data = 0);

  Ion::Storage::Record m_record;
  Ion::Storage::Record m_computedRecord;
  uint32_t m_checksum;
  float m_start;
  float m_end;
  Poincare::PointsOfInterestList m_list;
};

}

#endif
