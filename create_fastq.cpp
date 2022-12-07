// this program reads from a fasta file and produces a fastq file from the fasta file.
// the amount of probability of producing an incorrect read can be adjusted.

#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iomanip>


std::string quality_string(size_t read_length, std::uniform_int_distribution<size_t> &, std::mt19937 &);
void insert_bad_base(std::string&, double bad_read_prob, std::mt19937&, std::uniform_real_distribution<double>&);
int main(int argc, char* argv[]) {
	size_t i = 1;
	// variables
	size_t read_length = 20;
	size_t num_reads = 100;
	size_t min_qual = '!';
	size_t max_qual = 'J';
	std::string fasta_file;
	std::fstream in_file;
	double bad_read_prob = 0;
	while (i < argc) {
		std::string next_arg = argv[i];
		if (next_arg == "--help" || next_arg == "-h") {
			// display help and exit with 0
			std::cerr << std::left;
			std::cerr << std::setw(30)<< "--help -h" << "display this help successfully and exit" << '\n';
			std::cerr << std::setw(30)<< "--read-length -l" << "set read lenght" << '\n';
			std::cerr << std::setw(30)<< "--number-of-reads -n" << "set number of reads" << '\n';
			std::cerr << std::setw(30)<< "--min-quality --min" << "set minimum quality of phred quality string, use char as value" << '\n';
			std::cerr << std::setw(30)<< "--max-quality --max" << "set maximum quality of phred quality string, use char as value" << '\n';
			std::cerr << std::setw(30)<< "--fasta-file -i" << "name of fasta input file"<< '\n';
			std::cerr << std::setw(30)<< "--bad-read-prob -brp [float]" << "probability of false read";

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
				std::cerr << "invalid argument for number of reads\n";
				return 1;
			}
		}
		else if (next_arg == "--max-quality" || next_arg == "--max") {
			try {
				max_qual = argv[++i][0];
			} catch(std::invalid_argument const& e) {
				std::cerr << "invalid argument for max-quality\n";
				return 1;
			}
		}
		else if (next_arg == "--min-quality" || next_arg == "--min") {
			try {
				min_qual = argv[++i][0];
			} catch(std::invalid_argument) {
				std::cerr << "invalid argument for min-quality\n";
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
		else if (next_arg == "--bad-read-prob" || next_arg == "--brp") {
			try {
				bad_read_prob = std::stod(argv[++i]);
			} catch(std::invalid_argument&) {
				std::cerr << "invalid probability\n";		
				return 1;
			}
		}
		else {
			std::cerr << "invalid command line argument\n";
		}
		++i;
	}
	std::string dna = "";
	std::string each_line;
	std::string title;
	std::getline(in_file, title);
	// remove '<' and trailing whitespace from title.
	for (i = 1; i < title.size() && title[i] != ' '; ++i) {}
	title = title.substr(i+1);

	while (in_file >> each_line && each_line[0] != '>') {
		dna += each_line;
	}
	std::random_device rd;
	std::mt19937 rand(rd());
	std::uniform_int_distribution<size_t> dist(0, dna.size()-read_length);
	std::uniform_real_distribution<double> double_dist(0,1);
	// use a different distribution for the qualities.
	std::uniform_int_distribution<size_t> q_dist(min_qual, max_qual);
	insert_bad_base(dna, bad_read_prob, rand, double_dist);
	for (size_t i = 0; i < num_reads; ++i) {
		size_t num = dist(rand);
		std::cout << '@' << title << '\n';
		std::cout << dna.substr(num, read_length) << '\n';
		std::cout << "+\n";
		std::cout << quality_string(read_length, q_dist, rand) << '\n';

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
void insert_bad_base(std::string& dna, double bad_read_prob, std::mt19937& rand, std::uniform_real_distribution<double> & real_dist) {
	std::uniform_int_distribution<size_t> base_dist(0,4) ;
	for (size_t i = 0; i < dna.size(); ++i) {
		if (real_dist(rand) <= bad_read_prob) {
			short num = base_dist(rand);
			switch(num) {
				case 0: dna[i] = 'A';
						break;
				case 1: dna[i] = 'C';
						break;
				case 2: dna[i] = 'G';
						break;
				case 3: dna[i] = 'T';
						break;
			}
		}
	}
}
