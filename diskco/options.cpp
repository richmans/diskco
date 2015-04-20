#include "options.h"
Options::Options(char* input, char* output, bool append, bool byteswap) {
   _input_filename = input;
   _output_filename = output;
   _append = append;
   _swap_bytes = byteswap;
   _quiet = true;

   _offset = 0;
   _block_size = 32768;
   _length = -1;
   _end = 0;
}

Options::Options(int argc, char* argv[]) {
  int option_character, option_index = 0;
  static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"offset", required_argument, 0, 'o'},
    {"block-size", required_argument, 0, 'b'},
    {"length", required_argument, 0, 'l'},
    {"end", required_argument, 0, 'e'},
    {"swap-bytes", no_argument, 0, 's'},
    {"quiet", no_argument, 0, 'q'},
    {"append", no_argument, 0, 'a'},
    {"search-chars", required_argument, 0, 'c'},
    {"search-bytes", required_argument, 0, 'f'},
    {"segment_offset", required_argument, 0, 'O'},
    {"segment_length", required_argument, 0, 'L'},
    {0,0,0,0}
  };

  // reset the opt parser, to fix problems with unit tests
  optind = 1;
  // default values
  _offset = 0;
  _swap_bytes = false;
  _quiet = false;
  _append = false;
  _block_size = 32768;
  _length = -1;
  _end = 0;
  _search_bytes = new char[0];
  while (1) {
    option_character = getopt_long(argc, argv, "ho:b:l:e:sqac:f:O:L:", long_options, &option_index);
    if( option_character == -1) break;
    switch(option_character) {
      case 'o':
        _offset = bytesize(optarg);
        break;
      case 'b':
        _block_size = bytesize(optarg);
        break;
      case 'l':
        _length = bytesize(optarg);
        break;
      case 'e':
        _end = bytesize(optarg);
        break;
      case 's':
        _swap_bytes = true;
        break;
      case 'q':
        _quiet = true;
        break;
      case 'c':
        _search_bytes = new char[strlen(optarg)+1];
        strcpy(_search_bytes, optarg);
        // TODO set length
        break;
      case 'f':
        // TODO break on odd length argument
        _search_bytes = hextobytes(optarg);
        // TODO set length
        break;
      case 'a':
        _append = true;
        break;
      case 'O':
        _segment_offset = bytesize(optarg);
        break;
      case 'L':
        _segment_length = bytesize(optarg);
        break;
      case 'h':
        throw std::runtime_error("help");
      default:
        throw std::runtime_error("Invalid argument found");
    }
  }
  int tailing_arguments = argc - optind;
  if (tailing_arguments !=2 ) throw std::runtime_error("Please provide input and output");
  _input_filename = argv[optind];
  _output_filename = argv[optind+1];


}

/* Checks if the passed arguments are valid */
/* return: empty string if everything is valid */
std::string Options::check_arguments() {
  if (_end != 0) {
    if (_end <= _offset) {
      return "End needs to be bigger than offset";
    }
    _length = _end - _offset;
  }

  if (_swap_bytes) {
    if (_block_size % 2 == 1) {
      return "You can only use swap-bytes with an even block size";
    }
    if (_length % 2 == 1) {
      return "You can only use swap-bytes with an even length";
    }
    if (_offset % 2 == 1) {
      printf("WARING: using swap-bytes at an odd offset. Are you sure this is what you want?\n");
    }
  }

  if (_block_size < 64) {
    return "The minimum block-size is 64 bytes";
  }
  return "";
}


/* Setters */
void Options::set_length(int64_t length) {
  _length = length;
}

void Options::set_offset(int64_t offset) {
  _offset = offset;
}

void Options::set_swap_bytes(bool swap_bytes) {
  _swap_bytes = swap_bytes;
}

void set_search_bytes(std::string search_bytes) {
  _search_bytes = search_bytes;
}

void set_segment_offset(int64_t segment_offset) {
  _segment_offset = segment_offset;
}

void set_segment_length(int64_t segment_offset) {
  _segment_length = segment_length;
}

/* Getters */
std::string Options::input_filename() { return _input_filename; }
std::string Options::output_filename(){ return _output_filename; }
char* Options::search_bytes(){ return _search_bytes; }
int64_t Options::block_size() { return _block_size; }
int64_t Options::offset() { return _offset; }
int64_t Options::length() { return _length; }
int64_t Options::segment_offset() { return _segment_offset; }
int64_t Options::segment_length() { return _segment_length; }
bool Options::swap_bytes() { return _swap_bytes; }
bool Options::quiet() { return _quiet; }
bool Options::append() { return _append; }
