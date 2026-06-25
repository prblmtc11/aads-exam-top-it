#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include <iostream>
#include "person.hpp"

namespace yarmolinskaya {

  bool parse_person(
    const std::string& line,
    Person& person
  );

  int process_streams(
    std::istream& in,
    std::ostream& out
  );

  int parse_arguments(
    int argc,
    char* argv[],
    std::string& in_file,
    std::string& out_file
  );

}

#endif
