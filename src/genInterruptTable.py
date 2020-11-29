with open("interrupt.c", "w") as intfile:
	intfile.write("#include \"../header/interrupt.h\"\n\n")
	intfile.write("/* Code generated with genInterruptTable.py */\n")
	#Generate all declarations
	for i in range (0, 256):
		intfile.write("extern void interrupt_handler_" + str(i) + "();\n")
	intfile.write("\n")
	#Write the idt to memory
	intfile.write("void generate_idt(unsigned int * idtptr)\n")
	intfile.write("{\n")
	intfile.write("\tunsigned int funcptr;\n")
	for i in range(0, 256):
		intfile.write("\tfuncptr = (unsigned int)interrupt_handler_" + str(i) + ";\n")
		intfile.write("\tidtptr[" + str(2 * i + 1) + "] = (funcptr & 0xFFFF0000) + 0x8E00;\n")
		intfile.write("\tidtptr[" + str(2 * i) + "] = (funcptr & 0x0000FFFF) + 0x00080000;\n")
	intfile.write("}")
