
#include <ion.h>
#include <ion/timing.h>
#include <assert.h>

#include <bootloader/boot.h>
#include <bootloader/interface/static/interface.h>
#include <bootloader/slots/slot.h>
#include <bootloader/slots/slot_exam_mode.h>
#include <bootloader/recovery.h>
#include <ion/src/device/shared/drivers/flash.h>

__attribute__ ((noreturn)) void ion_main(int argc, const char * const argv[]) {
  // Clear the screen
  Ion::Display::pushRectUniform(KDRect(0,0,320,240), KDColorBlack);
  // Initialize the backlight
  Ion::Backlight::init();

  // We check if there is a slot in exam_mode

  bool isSlotA = Bootloader::Slot::isFullyValid(Bootloader::Slot::A());

  if (isSlotA) {
    Bootloader::ExamMode::ExamMode SlotAExamMode = (Bootloader::ExamMode::ExamMode)Bootloader::ExamMode::SlotsExamMode::FetchSlotAExamMode(Bootloader::Slot::A().userlandHeader()->version());
    if (SlotAExamMode != Bootloader::ExamMode::ExamMode::Off && SlotAExamMode != Bootloader::ExamMode::ExamMode::Unknown) {
      // We boot the slot in exam_mode
      Bootloader::Slot::A().boot();
    }
  }

  bool isSlotB = Bootloader::Slot::isFullyValid(Bootloader::Slot::B());

  if (isSlotB) {
    Bootloader::ExamMode::ExamMode SlotBExamMode = (Bootloader::ExamMode::ExamMode)Bootloader::ExamMode::SlotsExamMode::FetchSlotBExamMode(Bootloader::Slot::B().userlandHeader()->version());
    if (SlotBExamMode != Bootloader::ExamMode::ExamMode::Off && SlotBExamMode != Bootloader::ExamMode::ExamMode::Unknown && isSlotB) {
      // We boot the slot in exam_mode
      Bootloader::Slot::B().boot();
    }
  }

  // I have no idea if this will work, but if Parisse did a good job, it should

  bool isKhiSlot = Bootloader::Slot::isFullyValid(Bootloader::Slot::Khi());

  if (isKhiSlot) {
    Bootloader::ExamMode::ExamMode KhiExamMode = (Bootloader::ExamMode::ExamMode)Bootloader::ExamMode::SlotsExamMode::FetchSlotKhiExamMode(Bootloader::Slot::Khi().userlandHeader()->version());
    if (KhiExamMode != Bootloader::ExamMode::ExamMode::Off && KhiExamMode != Bootloader::ExamMode::ExamMode::Unknown && isKhiSlot) {
      // We boot the slot in exam_mode
      Bootloader::Slot::Khi().boot();
    }
  }

  Ion::Timing::msleep(200);
  uint64_t scan = Ion::Keyboard::scan();
  // Show bootloader
  if (scan == Ion::Keyboard::State(Ion::Keyboard::Key::Back)) {
    if (Bootloader::Recovery::hasCrashed()) {
      Bootloader::Recovery::recoverData();
    }

    Bootloader::Interface::drawLoading();

    // Boot the firmware
    Bootloader::Boot::boot();
  // Launch Slot A
  } else if (scan == Ion::Keyboard::State(Ion::Keyboard::Key::Up) && Bootloader::Slot::isFullyValid(Bootloader::Slot::A())) {
    Bootloader::Slot::A().boot();
  // Launch Slot B
  } else if (scan == Ion::Keyboard::State(Ion::Keyboard::Key::Down) && Bootloader::Slot::isFullyValid(Bootloader::Slot::B())) {
    Bootloader::Slot::B().boot();
  // Launch Khi
  }  else if (scan == Ion::Keyboard::State(Ion::Keyboard::Key::Left) && Bootloader::Slot::isFullyValid(Bootloader::Slot::Khi())) {
    Bootloader::Slot::Khi().boot();
  // Default Slot
  } else if (Bootloader::Slot::isFullyValid(Bootloader::Slot::B())) {
    Bootloader::Slot::B().boot();
  // Boot Slot A if Slot B is not valid
  } else if (Bootloader::Slot::isFullyValid(Bootloader::Slot::A())) {
    Bootloader::Slot::A().boot();
  // Boot bootloader if nothing is valid
  } else {
    if (Bootloader::Recovery::hasCrashed()) {
      Bootloader::Recovery::recoverData();
    }

    Bootloader::Interface::drawLoading();

    // Boot the firmware
    Bootloader::Boot::boot();
  }
}
