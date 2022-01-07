// this program reads from a fasta file and produces a fastq file from the fasta file.
// the amount of probability of producing an incorrect read can be adjusted.

#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
	size_t i = 1;

	// variables
	size_t read_length = 20;
	size_t num_reads = 100;
	size_t min_qual = '!';
	size_t max_qual = 'j';
	while (i < argc) {
		std::string next_arg = argv[i];

		if (next_arg == "--read-length" || next_arg == "-l") {
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
		else if (next_arg == "--min-quality" || next_arg == "--min") {
			try {
				min_qual = std::stoul(argv[++i]);
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
		++i;
	}

}

