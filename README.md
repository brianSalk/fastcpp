# fastcpp
Create fasta and fastq files for practicing with alignment tools.  These files are generated randomly and can be a conveniant alternative to real data
when practicing with alignment tools or comparing performance of various algorithms.  I use these programs to help create educational content.
## How to use
**1)** simply clone this repo to your computer `git clone <link to this repo>`\
**2)** compile the programs with your favorite c++ compiler. \  
eg. `g++ create_fasta.cpp -o create_fasta` and `g++ create_fasta.cpp -o create_fastq`\
**3)** run the code and use the option `-h` for instructions. eg. `./create_fasta -h` 

## Examples
**create a fasta file called random_reads.fasta with 25 randomly generated sequences all between the length 250-500bp** \
 ```
 ./create_fasta -n 25 --min 250 --max 500 --out random_reads.fasta
 ```
 **create a fastq file that simulates 100 reads from 'human_genome.fasta'.  each read is 200bp, phred quality is between ':' and 'A', where 1 in 50 reads are misreads.**\
 ```
 ./create_fastq -n 100 --min ':' --max 'A' -l 200 --brp .02 -i human_genome.fasta
 ```

