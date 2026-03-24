#pragma once

class AccessControl {
 public:
  bool begin();

  void armAutoMatch();
  bool canTryAutoMatch(bool enrollInProgress) const;
  bool handleMatchResult(bool matchOk);

 private:
  bool autoMatchArmed = true;
};