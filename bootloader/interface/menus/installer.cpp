#include "installer.h"
#include <bootloader/interface/static/messages.h>
#include <bootloader/usb_data.h>

#include <bootloader/interface/menus/dfu.h>

Bootloader::DfuMenu * Bootloader::InstallerMenu::SlotsDFU() {
  USBData data = USBData::DEFAULT();
  static DfuMenu * slotsDfu = new DfuMenu(Messages::dfuSlotsUpdate, &data);
  return slotsDfu;
}

Bootloader::DfuMenu * Bootloader::InstallerMenu::SlotADFU() {
  USBData data = USBData::SLOTA();
  static DfuMenu * slotADfu = new DfuMenu(Messages::dfuSlotAUpdate, &data);
  return slotADfu;
}

Bootloader::DfuMenu * Bootloader::InstallerMenu::SlotBDFU() {
  USBData data = USBData::SLOTB();
  static DfuMenu * slotBDfu = new DfuMenu(Messages::dfuSlotBUpdate, &data);
  return slotBDfu;
}


Bootloader::DfuMenu * Bootloader::InstallerMenu::BootloaderDFU() {
  USBData data = USBData::BOOTLOADER_UPDATE();
  static DfuMenu * bootloaderDfu = new DfuMenu(Messages::dfuBootloaderUpdate, &data);
  return bootloaderDfu;
}

Bootloader::InstallerMenu::InstallerMenu() : Menu(KDColorBlack, KDColorWhite, Messages::installerTitle, Messages::mainTitle) {
  setup();
}

bool slotsSubmenu() {
  Bootloader::InstallerMenu::SlotsDFU()->open();
  return true;
}
bool slotASubmenu() {
  Bootloader::InstallerMenu::SlotADFU()->open();
  return true;
}
bool slotBSubmenu() {
  Bootloader::InstallerMenu::SlotBDFU()->open();
  return true;
}

bool bootloaderSubmenu() {
  Bootloader::InstallerMenu::BootloaderDFU()->open();
  return true;
}

void Bootloader::InstallerMenu::setup() {
  m_defaultColumns[0] = Column(Messages::installerText1, k_large_font, 0, true);
  m_defaultColumns[1] = Column(Messages::installerSlotsSubmenu, Ion::Keyboard::Key::One, k_small_font, 30, false, &slotsSubmenu);
  m_defaultColumns[2] = Column(Messages::installerBootloaderSubmenu, Ion::Keyboard::Key::Two, k_small_font, 30, false, &bootloaderSubmenu);
  m_defaultColumns[3] = Column(Messages::installerSlotASubmenu, Ion::Keyboard::Key::Seven, k_small_font, 30, false, &slotASubmenu);
  m_defaultColumns[4] = Column(Messages::installerSlotBSubmenu, Ion::Keyboard::Key::Eight, k_small_font, 30, false, &slotBSubmenu);


  m_columns[0] = ColumnBinder(&m_defaultColumns[0]);
  m_columns[1] = ColumnBinder(&m_defaultColumns[1]);
  m_columns[2] = ColumnBinder(&m_defaultColumns[2]);
  m_columns[3] = ColumnBinder(&m_defaultColumns[3]);
  m_columns[4] = ColumnBinder(&m_defaultColumns[4]);
}
