import sys
import re
import string

# ***** process command line arguments
if len(sys.argv) != 2:
    print >> sys.stderr, 'Incorrect input ', sys.argv[0], ' text_file'
    sys.exit(1)
try:
    data_file = open(sys.argv[1], 'r')
except IOError:
    print >> sys.stderr, 'Cannot open file:', sys.argv[1]
    sys.exit(2)

data_str = ""
sum = 0
count = 0
#convert strings to int and sum
for line in data_file:
    count += 1
    data_str = line
    num = int(data_str)
    sum += num
average = float(sum/count)
print average
#close files
data_file.close()








