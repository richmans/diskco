import diskco
d = diskco.Diskco("/tmp/test.txt", "/tmp/test2.txt")
d.search("prog", 0,-1, 1, 5)
a=d.next_buffer()
a
