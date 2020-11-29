#Appends all of the interrupt declarations to the bottom of the file
errorGeneratingInterrupts = [8, 10, 11, 12, 13, 14, 17]

with open("interrupt_setup.s", "a") as intfile:
	for i in range (0, 256):
		if(i in errorGeneratingInterrupts):
			intfile.write("\terror_code_interrupt_handler " + str(i) + "\n")
		else:
			intfile.write("\tno_error_code_interrupt_handler " + str(i) + "\n")
