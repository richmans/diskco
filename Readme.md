# Disk Copier (Diskco)

Diskco was built out of frustration with the dd program. I needed to copy parts of a large file out at different offsets. DD can do this, but you can only define the offset in blocks. The only way to use an arbitrary offset is to set the blocksize to 1, which makes it very slow.

So what i needed was a program that can copy large amounts of data like dd at any offset while still using a larger blocksize. This is what diskco does.

In adition to the executable, it also provides a python3 extension which gives you the same functionality directly in python without using system()

## Installation

You need python3 development, and standard compiler tools like make and g++. For ubuntu, this should work.

  apt-get install python3-dev build-essential
  
Next, simply use the `make` command to compile and install everything.

## Usage (cmd)

    Usage: diskco [options] source_file destination_file
    Options:
    -h          Show this message
    -o [bytes]  Source offset in bytes
    -a          Append to target file instead of overwriting
    -b [bytes]  Use blocksize (default 32765)
    -q          Quiet, do not show progress
    -e          End offset in bytes
    -l          Length in bytes (specify either -l or -e)
    -s          Swap every two bytes
    Note: Anywhere you have an argument in bytes, you can use suffixes like k, m, g and t

Here are some examples:

copy bytes 32766 to 32770 from input to output

    diskco -o 32766 -e 32770 input.dd output.dd

copy 100 bytes from offset 20, using blocksize 10, and swapping every two bytes

    diskco -o 20 -l 100 -b 10 -s input.dd output.dd

## Usage (python3)

Here's how to use diskco in python

    import diskco
    diskco = diskco.Diskco("image.001", "myfile.dd", append=False, byteswap=False)
    offset = 140
    length = 100
    # copy 100 bytes into the new file
    diskco.copy(offset, length=-1)
    
    # copy another 100 bytes (swapping the bytes)
    offset += 100
    diskco.copy(offset, length)
    diskco.close()
    
    # or do everything in one command!
    diskco.copy_file("image.001", offset, length, "myfile.dd", byteswap=False, append=False)

That's it! hope this is usefull to you.

## License

This work is licensed under a Creative Commons CC BY 4.0 license. The text of the license is available at http://creativecommons.org/licenses/by/4.0