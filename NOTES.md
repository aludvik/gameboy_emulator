# Memory Bank Controllers
The memory state shall be stored in a struct 
"MEMState", which will hold the information
present in the ROM (cartridge type, ROM size,
RAM size, etc.). Each of the memory bank
controllers shall be encapsulated in an 
additional struct. Each memory bank controller
will exist as a static variable at the top level 
in the mem.c/h files. Functions shall be used
to extract relevant information from the memory 
state and memory bank controller structs.

# Static variables
Rather than storing everything in a struct to 
emulate OOP, Im trying something different and
using the file itself as the structure and 
restricting access to the variables to functions
within the file. Since CPU and MEM are singletons
I dont see any problems with this... yet.

# Bytes, Words, and Nibbles
In these notes and the documentation, I refer to
data of size 2^8 as a byte, 2^16 as a word, and
2^4 as a nibble. Memory is addressed by a word
and each address stores a byte.

# Endianess
"The z80 is little endian because if you were 
to store HL to memory, L would be written a byte 
before H. If you were to read, L would be read 
from the address before H."
http://stackoverflow.com/questions/21639597/z80-register-endianness

Also, for specifying an address, because the z80
is little endian, the least significant byte is
stored first. Eg. "JP, $0150" would store in ROM
as "0xC3 0x50 0x01".

Endianess only applies to how data is stored in 
memory, it is not related to how data is stored
in a register. Ie., the most significant byte of
an address is stored in H and the least significant
is stored in L.

# MBC
The Gameboy supports several ROM and RAM banking
modes. The first 16kB of ROM (Bank 0) are always
available at 0000-3FFF. The next 16kB of memory
depend on the banking mode used, which is indicated
by the value at 0147. Furthermore, I think that
"writing" different data to ranges of addresses 
in Bank 0 controls which banks are accessible at
4000-7FFF. The number of banks is stored at 0148.

# Map
At first I thought that there was a single RAM
being accessed by calls to addresses 0000-7FFF,
but I think there are just a collection of storage
devices mapped to by these addresses, so rather
than storing all memory in a single array, it makes
more sense to store the memory as separate arrays.

# Hexdump of ROM
Use "hexdump -Cv [filename] -n [count]" to dump
n bytes of a rom to the terminal.
