#ifndef CODE_EDIT_CELL_H
#define CODE_EDIT_CELL_H

#include <escher/responder.h>
#include <escher/highlight_cell.h>
#include <escher/text_field.h>
#include <escher/text_field_delegate.h>

namespace Code {

class ConsoleEditCell : public HighlightCell, public Responder {
public:
  ConsoleEditCell(Responder * parentResponder = nullptr, TextFieldDelegate * delegate = nullptr);

  // View
  int numberOfSubviews() const override;
  View * subviewAtIndex(int index) override;
  void layoutSubviews() override;

  // Responder
  void didBecomeFirstResponder() override;

  // Edit cell
  void setEditing(bool isEditing);

private:
  char m_textBuffer[TextField::maxBufferSize()];
  char m_draftTextBuffer[TextField::maxBufferSize()];
  TextField m_textField;
};

}

#endif
