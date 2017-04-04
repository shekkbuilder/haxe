import re
import fileinput
for line in fileinput.input(r'whatever.txt', inplace = True):
   if not re.search(r'\bto_delete\b',line):
      print line,

