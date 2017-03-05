import sys
import re
import string

def heart_rate(str, open_file):
    str = str.lstrip("HR")
    open_file.write(str + '\n')
    #print str
    return

def spo2(str, open_file):
    str = str.lstrip("SpO2%")
    open_file.write(str + '\n')
    #print str
    return

def BP(str, open_file_1, open_file_2):
    str = str.lstrip("BP")
    denom = str
    for c in denom:
        if c == "/":
            denom = denom.lstrip(c)
            break
        denom = denom.lstrip(c)
    str = str.replace("/" + denom, "")
    open_file_1.write(str + '\n')
    open_file_2.write(denom + '\n')
    return



# ***** process command line arguments
if len(sys.argv) != 2:
    print >> sys.stderr, 'Incorrect input ', sys.argv[0], ' text_file'
    sys.exit(1)
try:
    data_file = open(sys.argv[1], 'r')
except IOError:
    print >> sys.stderr, 'Cannot open file:', sys.argv[1]
    sys.exit(2)

#create copy of text file data
input = data_file.read()

#open output files
HR_file = open("Heart_Rate_Parsed.txt", "w");
BPnum_file = open("Blood_Pressure_numerator_parsed.txt", "w");
BPdenom_file = open("Blood_Pressure_denomenator_parsed.txt", "w");
SP_file = open("Blood_Oxygen_Parsed.txt", "w");

data_str = ""
#loop through output to separate data strings such as HR60
for c in input:
    if c == "|":
        data_str = data_str.lstrip("|")
        if data_str[0] == "H":
            heart_rate(data_str, HR_file)
        elif data_str[0] == "S":
            spo2(data_str, SP_file)
        elif data_str[0] == "B":
            BP(data_str, BPnum_file, BPdenom_file)
        else:
            continue
        data_str = ""
    data_str += c

#close files
data_file.close()
HR_file.close()
BPnum_file.close()
BPdenom_file.close()
SP_file.close()









