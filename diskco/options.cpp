#include "options.h"

#define DEFAULT_BLOCK_SIZE 32768
#define DEFAULT_PROCESSING_BLOCK_SIZE 512
#ifdef __MINGW32__
#define ftello ftello64
#define fseeko fseeko64
#endif
Options::Options(char* input, char* output, bool append, bool byteswap) {
   _input_filename = input;
   _output_filename = output;
   _append = append;
   _swap_bytes = byteswap;
   _quiet = true;
   _entropy_algorithm = none;
   _search_bytes_length = 0;
   _segment_length = 0;
   _segment_offset = 0;
   _offset = 0;
   _block_size = DEFAULT_BLOCK_SIZE;
   _processing_block_size = DEFAULT_PROCESSING_BLOCK_SIZE;
   _sample_blocks = 1;
   _length = -1;
   _end = 0;
}

Options::Options(int argc, char* argv[]) {
  int option_character, option_index = 0;

  if(argc == 1) {
    throw std::runtime_error("help");
  }
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
    {"segment-offset", required_argument, 0, 'O'},
    {"segment-length", required_argument, 0, 'L'},
    {"sample-blocks", required_argument, 0, 'S'},
    {"entropy-algorithm", optional_argument, 0, 'E'},
    {"processing-block-size", required_argument, 0, 'B'},
    {0,0,0,0}
  };

  // reset the opt parser, to fix problems with unit tests
  optind = 1;
  // default values
  _offset = 0;
  _swap_bytes = false;
  _quiet = false;
  _append = false;
  _entropy_algorithm = none;
  _block_size = DEFAULT_BLOCK_SIZE;
  _processing_block_size = DEFAULT_PROCESSING_BLOCK_SIZE;
  _segment_length = 0;
  _segment_offset = 0;
  _sample_blocks = 1;
  _length = -1;
  _end = 0;
  _search_bytes = new char[0];
  _search_bytes_length = 0;
  while (1) {
    option_character = getopt_long(argc, argv, "ho:b:l:e:sqac:f:O:L:S:E::B:", long_options, &option_index);
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
        _search_bytes_length = strlen(optarg);
        _search_bytes = new char[_search_bytes_length];
        memcpy(_search_bytes, optarg, _search_bytes_length);
        break;
      case 'f':
        if (strlen(optarg) % 2 != 0) {
          throw std::runtime_error("Invalid hex value");
        }
        _search_bytes_length = strlen(optarg) / 2;
        _search_bytes = hextobytes(optarg);
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
      case 'S':
        _sample_blocks = bytesize(optarg);
        break;
      case 'E':
        _entropy_algorithm = (optarg != NULL && strcmp(optarg, "log") == 0) ? logarithmic : nulls;
        break;
      case 'B':
        _processing_block_size = bytesize(optarg);
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

  // Do a first initial check, note that some
  // some other fields are prefilled.
  check_arguments();
}

/* Checks if the passed arguments are valid */
/* return: empty string if everything is valid */
std::string Options::check_arguments() {
  int64_t file_size = input_file_size();
  if (file_size < 0)
  {
    return "Could not open input file, file size < 0";
  }

  if (_end != 0) {
    if (_end <= _offset) {
      return "End needs to be bigger than offset";
    }
    if (_end > file_size) {
      return "End should not be bigger than the filesize";
    }
    _length = _end - _offset;
  }

  // if length is not set, read the entire file!
  if (_length == -1)
  {
    _length = file_size - _offset;
    //printf("Warning: length was changed to: %lld \n", _length);
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

  if (_sample_blocks > 1) {
    if (_block_size != DEFAULT_BLOCK_SIZE && _block_size != _processing_block_size) {
      return "When using the option to sample, you should not overrule the blocksize";
    }
    _block_size = _processing_block_size;
  }

  if (_block_size < 64) {
    return "The minimum block-size is 64 bytes";
  }

  if (_block_size % _processing_block_size != 0){
    if (_processing_block_size > _block_size){
      if (_processing_block_size != DEFAULT_PROCESSING_BLOCK_SIZE)
      {
        return "The processing blocksize must be bigger than the (reading) blocksize";
      }
      _processing_block_size = _block_size;
    } else {
      return "The blocksize should always be a multiple of the processing blocksize";
    }
  }

  return "";
}

int64_t Options::input_file_size(){
  if (_input_filename.empty()) return -1;
  FILE* file = fopen(_input_filename.c_str(), "rb");
  if(!file) return -1;
  fseeko(file, 0, SEEK_END);
  int64_t file_size = ftello(file);
  fclose(file);
  return file_size;
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

void Options::set_search_bytes(std::string search_bytes) {
  _search_bytes_length = search_bytes.length();
  _search_bytes = new char(_search_bytes_length);
  memcpy(_search_bytes, search_bytes.c_str(), _search_bytes_length);
}

void Options::set_segment_offset(int64_t segment_offset) {
  _segment_offset = segment_offset;
}

void Options::set_segment_length(int64_t segment_length) {
  _segment_length = segment_length;
}

/* Getters */
std::string Options::input_filename() { return _input_filename; }
std::string Options::output_filename(){ return _output_filename; }
char* Options::search_bytes(){ return _search_bytes; }
int64_t Options::block_size() { return _block_size; }
int64_t Options::processing_block_size() { return _processing_block_size; }
int64_t Options::offset() { return _offset; }
int64_t Options::length() { return _length; }
int64_t Options::sample_blocks() { return _sample_blocks; }
int64_t Options::segment_offset() { return _segment_offset; }
int64_t Options::segment_length() { return _segment_length; }
EntropyAlgorithm Options::entropy_algorithm() { return _entropy_algorithm; }
bool Options::swap_bytes() { return _swap_bytes; }
bool Options::quiet() { return _quiet; }
bool Options::append() { return _append; }
int Options::search_bytes_length() { return _search_bytes_length;}
