#include "dynamic_cells_data_source.h"
#include "probability/app.h"
#include <new>

namespace Probability {

template <typename T, int N>
void DynamicCellsDataSource<T,N>::createCells() {
  if (m_cells == nullptr) {
    static_assert(sizeof(T) * N <= Probability::App::k_bufferSize, "Probability::App::m_buffer is not large enough");
    m_cells = new (Probability::App::app()->buffer()) T[N];
    Probability::App::app()->setBufferDestructor(this);
    for (int i = 0; i < N; i++) {
      m_delegate->initCell(&m_cells[i], i);
    }
    // TODO: reloadData(setFirstResponder = false, setSelection = false),
    // selSelection argument was made especially for this local use. Should we
    // use 'm_delegate->tableView()->deselectTable();' and ensure that the
    // SelectionDataSource reprensents the selection correctly (before/after
    // the table first/last cell to make sure to re-select the right cell when
    // selecting the table).
    m_delegate->tableView()->reloadData(false, false);
  }
}

template <typename T, int N>
void DynamicCellsDataSource<T,N>::destroyCells() {
  /* We manually call T destructor since we cannot use 'delete' due to the
   * placement new.
   * Note Bene: we qualify the destructor call (by prefixing it by its class
   * name) to avoid a compiler warning: T is not a final class and has virtual
   * methods but no virtual destructor; the compiler might think we forgot some
   * virtualization here but we didn't - we don't want to call a derived
   * destructor of children T class. */
  for (int i = 0; i < N; i++) {
    m_cells[i].T::~T();
  }
  m_cells = nullptr;
}

template <typename T, int N>
Escher::HighlightCell * DynamicCellsDataSource<T,N>::cell(int i) {
  createCells();
  return &m_cells[i];
}

template class DynamicCellsDataSource<Escher::EvenOddBufferTextCell, HomogeneityTableDataSource::k_numberOfReusableCells>;
template class DynamicCellsDataSource<Escher::EvenOddEditableTextCell, HomogeneityTableDataSource::k_numberOfReusableCells>;
template class DynamicCellsDataSource<Escher::EvenOddEditableTextCell, k_inputGoodnessTableNumberOfReusableCells>;

}
