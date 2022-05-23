# AL_Project2
## Manual
Input interger N for number of entry.
Then input assembly code.
Please input only code, do not enter address or note.
<br />
There are some sample I/O in the file.
<br />

## Code Intro<br/>

### Variables:<br/>
**Line 9** for each input instruction.<br/>
**Line 10,11** are for instruction input,<br/>
then store in struct Inst, last, push into Insts.<br/>
**Line 12,13** stores number of entry, total instructions,<br/>
currently used entry, and PC & next_PC.<br/>
**Line 15** for history, then store in each entry's BTB.<br/>
**Line 16**, a table for Labels, shows which address should PC be.<br/>
**Line 17**, struct include one BTB with 3-bit-history, 8BCs,<br/>
number of misprediction, and wether it has been used.<br/>
Then push into BTBs.<br/>
**Line 34** for Regester storing.<br/>
<br/>

### Functions:<br/>
**fetch(string): ** for each input instruction.<br/>
**predict(): ** for each input instruction.<br/>
**outcome(): ** for each input instruction.<br/>
**print(): ** for each input instruction.<br/>
**update(char,char): ** for each input instruction.<br/>

