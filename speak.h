#include <espeak-ng/speak_lib.h>
#include <string>

using namespace std;

class Speak{
public:
  void text_to_speech(string text);
  std::string remove_special_chars(const std::string& str);
  bool is_special_char(char c);
  std::string remove_newline(std::string te);
};
