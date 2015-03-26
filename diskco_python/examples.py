import diskco
diskco = diskco.Diskco("image.001", "myfile.dd", append=False, byteswap=False)
offset = 140
length = 100
# copy 100 bytes into the new file
diskco.copy(offset, length=-1)

# copy another 100 bytes (swapping the bytes)
offset += 100
diskco.copy(offset, length)

# OR AUTOCLOSE????
diskco.close()



diskco.copy_file("image.001", offset, length, "myfile.dd", byteswap=False, append=False)