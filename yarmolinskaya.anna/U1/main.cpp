#include <iostream>
#include <fstream>
#include "processing.hpp"

int main(const int argc, char* argv[])
{
  std::string in_filename;
  std::string out_filename;
  if (yarmolinskaya::parse_arguments(
        argc, argv, in_filename, out_filename) != 0) {
    return 1;
  }
  if (!in_filename.empty() && in_filename == out_filename) {
    std::ifstream in_test(in_filename);
    if (!in_test.is_open()) {
      return 2;
    }
    size_t capacity = 10;
    size_t size = 0;
    std::string* lines = new std::string[capacity];
    std::string line;
    while (std::getline(in_test, line)) {
      if (size >= capacity) {
        capacity *= 2;
        std::string* const new_lines = new std::string[capacity];
        for (size_t i = 0; i < size; ++i) {
          new_lines[i] = lines[i];
        }
        delete[] lines;
        lines = new_lines;
      }
      lines[size] = line;
      size++;
    }
    in_test.close();
    std::ofstream out(out_filename);
    if (!out.is_open()) {
      delete[] lines;
      return 2;
    }
    size_t success_count = 0;
    size_t ignore_count = 0;
    size_t p_capacity = 10;
    size_t p_size = 0;
    yarmolinskaya::Person* persons =
      new yarmolinskaya::Person[p_capacity];
    for (size_t i = 0; i < size; ++i) {
      yarmolinskaya::Person p;
      if (!yarmolinskaya::parse_person(lines[i], p)) {
        ignore_count++;
        continue;
      }
      bool duplicate = false;
      for (size_t j = 0; j < p_size; ++j) {
        if (persons[j].id == p.id) {
          duplicate = true;
          break;
        }
      }
      if (duplicate) {
        ignore_count++;
        continue;
      }
      if (p_size >= p_capacity) {
        p_capacity *= 2;
        yarmolinskaya::Person* const new_arr =
          new yarmolinskaya::Person[p_capacity];
        for (size_t j = 0; j < p_size; ++j) {
          new_arr[j] = persons[j];
        }
        delete[] persons;
        persons = new_arr;
      }
      persons[p_size] = p;
      p_size++;
      success_count++;
    }
    for (size_t i = 0; i < p_size; ++i) {
      out << persons[i].id << " " << persons[i].info << "\n";
    }
    std::cerr << success_count << " " << ignore_count << "\n";
    delete[] lines;
    delete[] persons;
    return 0;
  }
  if (!in_filename.empty() && !out_filename.empty()) {
    std::ifstream in(in_filename);
    std::ofstream out(out_filename);
    if (!in.is_open() || !out.is_open()) {
      return 2;
    }
    return yarmolinskaya::process_streams(in, out);
  } else if (!in_filename.empty()) {
    std::ifstream in(in_filename);
    if (!in.is_open()) {
      return 2;
    }
    return yarmolinskaya::process_streams(in, std::cout);
  } else if (!out_filename.empty()) {
    std::ofstream out(out_filename);
    if (!out.is_open()) {
      return 2;
    }
    return yarmolinskaya::process_streams(std::cin, out);
  }
  return yarmolinskaya::process_streams(std::cin, std::cout);
}
