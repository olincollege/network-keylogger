typedef struct {
  char* key;        // a string representing a key that was pressed
  char* timestamp;  // the approximate time (UTC) that the key was pressed
  // ^ or maybe make the time for when the key was stored? might be easier
  char* application;  // represents the active application of the user when they
                      // pressed the key
} key_info;
