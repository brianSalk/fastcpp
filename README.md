# fastcpp
Create fasta and fastq files for practicing with alignment tools
## How to use
**1)** simply clone this repo to your computer `clone <link to this repo>`\
**2)** compile the programs with your favorite c++ compiler. eg. `g++ create_fasta.cpp -o create_fasta` and `g++ create_fasta.cpp -o create_fastq`\
**3)** run the code and use the option `-h` for instructions. eg. `./create_fasta -h` \
\
## Examples
**create a fasta file called random_reads.fasta with 25 randomly generated sequences all between the length 250-500bp**\
 `./create_fasta -n 25 --min 250 --max 500 --out fandom_reads.fasta`

