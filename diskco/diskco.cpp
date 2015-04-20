#include "options.h"
#include "diskco.h"
using namespace std;

void Diskco::get_options(int argc, char* argv[]){
  try {
    _options = new Options(argc, argv);
    std::string err = _options->check_arguments();
    if (err != "") {
      printf("ERROR: %s\n", err.c_str());
      exit(1);
    }
  } catch (std::runtime_error e) {
    if(strcmp(e.what(), "help") == 0) {
     help(); 
    }else{
      printf("ERROR: %s\n", e.what());
    }
    exit(1);
  }
}

void Diskco::help() {
  printf("Usage: diskco [options] source_file destination_file\n\n");
  printf("\n");
  printf("Options:\n");
  printf("-h          Show this message\n");
  printf("-o [bytes]  Source offset in bytes\n");
  printf("-a          Append to target file instead of overwriting\n");
  printf("-b [bytes]  Use blocksize (default 32765)\n");
  printf("-q          Quiet, do not show progress\n");
  printf("-e [bytes]  End offset in bytes\n");
  printf("-l [bytes]  Length in bytes (specify either -l or -e)\n");
  printf("-s          Swap every two bytes\n");
  printf("-f          Search for byte sequence\n");
  printf("-O [bytes]  For each search match, start copying at offset\n");
  printf("-L [bytes]  For each search match, copy amount of bytes\n");
  printf("\nNote: Anywhere you have an argument in bytes, you can use suffixes like k, m, g and t\n\n");
}

Diskco::Diskco(int argc, char* argv[]) {
  get_options(argc, argv);
  initialize();
  
}

Diskco::Diskco(char* input, char* output, bool append, bool byteswap) {
  _options = new Options(input, output, append, byteswap);
  initialize();
}



Diskco::~Diskco() {
  if(_options){
    delete _options;
  }
}

void Diskco::initialize() {
  _pool = new BufferPool(256, _options->block_size());
  _reader = new FileReader(_options, NULL, _pool);
  _swapper = new ByteSwapper(_options, _reader, _pool);
  _searcher = new Searcher(_options, _swapper, _pool);
  _writer = new FileWriter(_options, _searcher, _pool);
  rewire();
}

/* 
Manipulates the parent of each buffer processor in the chain.
This makes it possible to switch processors on and off.
*/
void Diskco::rewire() {
  BufferProcessor* parent = _reader;
  if (_options->swap_bytes()) {
    parent = _swapper;
  }
  if (_options->search_bytes().length() != 0) {
    _searcher->set_parent(parent);
    parent = _searcher;
  }
  _writer->set_parent(parent);
  _chain_end = _writer;
}

void Diskco::set_search(std::string search_bytes, int64_t offset, int64_t length, int64_t segment_offset, int64_t segment_length) {
  _reader->initialize(offset, length);
  _options->set_search_bytes(search_bytes);
  _options->set_segment_offset(segment_offset);
  _options->set_segment_length(segment_length);
  //TODO We need to reinitialize the searcher to do this
  rewire();
}

void Diskco::unset_search() {
  _options->set_search_bytes("");
  rewire();
}

void Diskco::set_swap_bytes(bool swap_bytes){
  _options->set_swap_bytes(swap_bytes);
  rewire();
}

void Diskco::close() {
   _reader->close();
   _writer->close();
}

void Diskco::copy(int64_t offset, int64_t length){
  _reader->initialize(offset, length);
  run();
}

Buffer* Diskco::next_buffer() {
  return _chain_end->next_buffer();
}

void Diskco::run() {
  Buffer* result;
  while (1) {
    result = _chain_end->next_buffer();
    if(result == NULL) break;
  }
}

void Diskco::start(){
  
  try {
    run();
    close();
  } catch (std::runtime_error e) {
    printf("ERROR: %s\n", e.what());
  }
  printf("Thankyoubyebye\n");
}


int main(int argc, char* argv[]) {
  Diskco diskco(argc, argv);
  diskco.start();
}
  
