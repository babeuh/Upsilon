#include "continuous_function_store.h"
#include <ion.h>

namespace Shared {

bool ContinuousFunctionStore::displaysNonCartesianFunctions(int * nbActiveFunctions) const {
  int nActive = numberOfActiveFunctions();
  if (nbActiveFunctions != nullptr) {
    *nbActiveFunctions = nActive;
  }
  return numberOfActiveFunctionsWithProperty(&ContinuousFunctionProperties::isCartesian) < nActive;
}

bool ContinuousFunctionStore::displaysFunctionsToNormalize(int * nbActiveFunctions) const {
  if (nbActiveFunctions != nullptr) {
    *nbActiveFunctions = numberOfActiveFunctions();
  }
  return numberOfActiveFunctionsWithProperty(&ContinuousFunctionProperties::enforcePlotNormalization) != 0;
}

Ion::Storage::Record::ErrorStatus ContinuousFunctionStore::addEmptyModel() {
  char name[ContinuousFunction::k_maxDefaultNameSize];
  const char * const extensions[1] = { modelExtension() };
  name[0] = ContinuousFunction::k_unnamedRecordFirstChar;
  Ion::Storage::FileSystem::sharedFileSystem->firstAvailableNameFromPrefix(name, 1, ContinuousFunction::k_maxDefaultNameSize, extensions, 1, 99);
  Ion::Storage::Record::ErrorStatus error = Ion::Storage::Record::ErrorStatus::RecordDoesNotExist;
  ContinuousFunction newModel = ContinuousFunction::NewModel(&error, name);
  return error;
}

ExpressionModelHandle * ContinuousFunctionStore::setMemoizedModelAtIndex(int cacheIndex, Ion::Storage::Record record) const {
  assert(cacheIndex >= 0 && cacheIndex < maxNumberOfMemoizedModels());
  m_functions[cacheIndex] = ContinuousFunction(record);
  return &m_functions[cacheIndex];
}

ExpressionModelHandle * ContinuousFunctionStore::memoizedModelAtIndex(int cacheIndex) const {
  assert(cacheIndex >= 0 && cacheIndex < maxNumberOfMemoizedModels());
  return &m_functions[cacheIndex];
}

}
