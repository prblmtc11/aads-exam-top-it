#include "processing.hpp"
#include <fstream>

namespace yarmolinskaya {

  bool parse_person(const std::string& line, Person& person)
  {
    if (line.empty()) {
      return false;
    }
    size_t i = 0;
    while (i < line.size() &&
           (line[i] == ' ' || line[i] == '\t' || line[i] == '\r')) {
      i++;
    }
    if (i == line.size() || line[i] < '0' || line[i] > '9') {
      return false;
    }
    size_t id_val = 0;
    while (i < line.size() && line[i] >= '0' && line[i] <= '9') {
      id_val = id_val * 10 + (line[i] - '0');
      i++;
    }
    if (i < line.size() &&
        line[i] != ' ' && line[i] != '\t' && line[i] != '\r') {
      return false;
    }
    while (i < line.size() &&
           (line[i] == ' ' || line[i] == '\t' || line[i] == '\r')) {
      i++;
    }
    if (i == line.size()) {
      return false;
    }
    std::string info_val = line.substr(i);
    while (!info_val.empty() &&
           (info_val.back() == '\r' || info_val.back() == '\n')) {
      info_val.pop_back();
    }
    if (info_val.empty()) {
      return false;
    }
    person.id = id_val;
    person.info = info_val;
    return true;
  }

  int process_streams(std::istream& in, std::ostream& out)
  {
    size_t success_count = 0;
    size_t ignore_count = 0;
    size_t capacity = 10;
    size_t size = 0;
    Person* persons = new Person[capacity];
    std::string line;
    while (std::getline(in, line)) {
      Person p;
      if (!parse_person(line, p)) {
        ignore_count++;
        continue;
      }
      bool duplicate = false;
      for (size_t i = 0; i < size; ++i) {
        if (persons[i].id == p.id) {
          duplicate = true;
          break;
        }
      }
      if (duplicate) {
        ignore_count++;
        continue;
      }
      if (size >= capacity) {
        capacity *= 2;
        Person* const new_arr = new Person[capacity];
        for (size_t i = 0; i < size; ++i) {
          new_arr[i] = persons[i];
        }
        delete[] persons;
        persons = new_arr;
      }
      persons[size] = p;
      size++;
      success_count++;
    }
    for (size_t i = 0; i < size; ++i) {
      out << persons[i].id << " " << persons[i].info << "\n";
    }
    std::cerr << success_count << " " << ignore_count << "\n";
    delete[] persons;
    return 0;
  }

  int parse_arguments(
    const int argc,
    char* argv[],
    std::string& in_file,
    std::string& out_file
  )
  {
    if (argc > 3) {
      return 1;
    }
    bool has_in = false;
    bool has_out = false;
    for (int i = 1; i < argc; ++i) {
      const std::string arg = argv[i];
      if (arg.size() >= 3 && arg.substr(0, 3) == "in:") {
        if (has_in) {
          return 1;
        }
        in_file = arg.substr(3);
        has_in = true;
      } else if (arg.size() >= 4 && arg.substr(0, 4) == "out:") {
        if (has_out) {
          return 1;
        }
        out_file = arg.substr(4);
        has_out = true;
      } else {
        return 1;
      }
    }
    return 0;
  }

}
