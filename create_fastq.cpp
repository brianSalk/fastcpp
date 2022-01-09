// this program reads from a fasta file and produces a fastq file from the fasta file.
// the amount of probability of producing an incorrect read can be adjusted.

#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iomanip>


std::string quality_string(size_t read_length, std::uniform_int_distribution<size_t> &, std::mt19937 &);
int main(int argc, char* argv[]) {
	size_t i = 1;

	// variables
	size_t read_length = 20;
	size_t num_reads = 100;
	size_t min_qual = '!';
	size_t max_qual = 'j';
	std::string fasta_file;
	std::fstream in_file;
	while (i < argc) {
		std::string next_arg = argv[i];

		if (next_arg == "--help" || next_arg == "-h") {
			// display help and exit with 0
			std::cout << std::left;
			std::cout << std::setw(30)<< "--help -h" << "display this help successfully and exit" << '\n';
			std::cout << std::setw(30)<< "--read-length -l" << "set read lenght" << '\n';
			std::cout << std::setw(30)<< "--number-of-reads -n" << "set number of reads" << '\n';
			std::cout << std::setw(30)<< "--min-quality --min" << "set minimum quality of phred quality string, use char as value" << '\n';
			std::cout << std::setw(30)<< "--max-quality --max" << "set maximum quality of phred quality string, use char as value" << '\n';
			std::cout << std::setw(30)<< "--fasta-file -i" << "name of fasta input file"<< '\n';

			return 0;
		}
		else if (next_arg == "--read-length" || next_arg == "-l") {
			try {
				read_length = std::stoul(argv[++i]);
			} catch(std::invalid_argument) {
				std::cerr << "invalid argument for read length\n";
				return 1;
			}
		}
		else if (next_arg == "--number-of-reads" || next_arg == "-n") {
			try {
				num_reads = std::stoul(argv[++i]);
			} catch(std::invalid_argument) {
				std::cerr << "invalid argument for read length\n";
				return 1;
			}
		}
		else if (next_arg == "--max-quality" || next_arg == "--max") {
			try {
				max_qual = std::stoul(argv[++i]);
			} catch(std::invalid_argument) {
				std::cerr << "invalid argument for read length\n";
				return 1;
			}
		}
		else if (next_arg == "--min-quality" || next_arg == "--min") {
			try {
				max_qual = std::stoul(argv[++i]);
			} catch(std::invalid_argument) {
				std::cerr << "invalid argument for read length\n";
				return 1;
			}
		}
		else if (next_arg == "--fasta-file" || next_arg == "-i") {
			fasta_file = argv[++i];
			in_file = std::fstream(fasta_file, std::ios::in);	
			if (!in_file.is_open()) {
				std::cerr << "problem opening file " << fasta_file << '\n';
				return 1;
			}
		}
		++i;
	}
	std::string dna = "";
	std::string each_line;
	std::string title;
	std::getline(in_file, title);
	// remove '<' and trailing whitespace from title.
	for (size_t i = 1; i < title.size() && title[i] != ' '; ++i) {}
	title = title.substr(i-1);

	while (in_file >> each_line && each_line[0] != '>') {
		dna += each_line;
	}
	std::random_device rd;
	std::mt19937 rand(rd());
	std::uniform_int_distribution<size_t> dist(0, dna.size()-read_length);
	// use a different distribution for the qualities.
	std::uniform_int_distribution<size_t> q_dist(min_qual, max_qual);
	for (size_t i = 0; i < num_reads-read_length; ++i) {
		size_t num = dist(rand);
		std::cout << "@" << title << i+1 << '\n';
		std::cout << dna.substr(num,read_length) << '\n';
		std::cout << "+\n";
		std::cout << quality_string(read_length, q_dist, rand) << std::endl;
	}	
}

std::string quality_string(size_t read_length, std::uniform_int_distribution<size_t> &dist, std::mt19937 & rand) {
	std::string ans;
	ans.reserve(read_length);
	for (size_t i = 0; i < read_length; ++i) {
		ans.push_back(dist(rand));
	}
	return ans;
}
