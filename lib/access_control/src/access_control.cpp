#include "access_control.h"

bool AccessControl::begin() {
  autoMatchArmed = true;
  return true;
}

void AccessControl::armAutoMatch() {
  autoMatchArmed = true;
}

bool AccessControl::canTryAutoMatch(bool enrollInProgress) const {
  return autoMatchArmed && !enrollInProgress;
}

bool AccessControl::handleMatchResult(bool matchOk) {
  if (matchOk) {
    autoMatchArmed = false;
    return true;
  }

  return false;
}