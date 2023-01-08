// this program reads from a fasta file and produces a fastq file from the fasta file.
// the amount of probability of producing an incorrect read can be adjusted.

#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iomanip>

enum class bio_type {
	dna,
	rna,
	prot,
	cust_charstring,
};
struct options {
	size_t read_length;
	size_t num_reads;	
	size_t min_qual;
	size_t max_qual;
	bio_type bio_type;
	double bad_read_prob;
	std::string fasta_file_name; 
	std::string out_file_name;

	void h(ostream & os) const {
		os << "[--help,-h],[--read-length,-l],[--number-of-reads, -n], [--min-max_quality,--min][--max-quality, --max], [--fasta-file, -i], [--bad-read-prob, --brp], [--out,-o]\n";
	}
	void help() const {
		this->h(ostream & os);
		os << std::left;
		os << std::setw(30)<< "--help -h" << "display this help successfully and exit" << '\n';
		os << std::setw(30)<< "--read-length -l" << "set read lenght" << '\n';
		os << std::setw(30)<< "--number-of-reads -n" << "set number of reads" << '\n';
		os << std::setw(30)<< "--min-quality --min" << "set minimum quality of phred quality string, use char as value" << '\n';
		os << std::setw(30)<< "--max-quality --max" << "set maximum quality of phred quality string, use char as value" << '\n';
		os << std::setw(30)<< "--fasta-file -i" << "name of fasta input file"<< '\n';
		os << std::setw(30)<< "--bad-read-prob -brp [float]" << "probability of false read\n";
		os << std::setw(30)<< "--out -o out_file" << "specify output fastq file\n";
	}
};
std::string quality_string(size_t read_length, std::uniform_int_distribution<size_t> &, std::mt19937 &);
void insert_bad_base(std::string&, double bad_read_prob, std::mt19937&, std::uniform_real_distribution<double>&);
void output(std::ostream &os, std::string& dna, size_t read_length, char min_qual, char max_qual, double bad_read_prob, size_t num_reads, std::string const& title) {
	std::random_device rd;
	std::mt19937 rand(rd());
	std::uniform_int_distribution<size_t> dist(0, dna.size()-read_length);
	std::uniform_real_distribution<double> double_dist(0,1);
	// use a different distribution for the qualities.
	std::uniform_int_distribution<size_t> q_dist(min_qual, max_qual);
	insert_bad_base(dna, bad_read_prob, rand, double_dist);
	// randomly select an index in the fasta file DNA and read from that 
	for (size_t i = 0; i < num_reads; ++i) {
		size_t num = dist(rand);
		os << '@' << title << '\n';
		os << dna.substr(num, read_length) << '\n';
		os << "+\n";
		os << quality_string(read_length, q_dist, rand) << '\n';

	}	
}

void parse_args(options & flags, char** argv, size_t const argc) {
	size_t i = 1;
	while (i < argc) {
		std::string next_arg = argv[i];
		if (next_arg == "--help") {
			// display help and exit with 0
			flags.help(std::cout);
			return true;
		}
		else if (next_arg == "-h") {
			flags.h(std::cout);
			return true;
		}
		else if (next_arg == "--read-length" || next_arg == "-l") {
			try {
				flags::read_length = std::stoul(argv[++i]);
			} catch(std::invalid_argument) {
				std::cerr << "invalid argument for read length\n";
				throw;
			}
		}
		else if (next_arg == "--number-of-reads" || next_arg == "-n") {
			try {
				flags::num_reads = std::stoul(argv[++i]);
			} catch(std::invalid_argument) {
				std::cerr << "invalid argument for number of reads\n";
				throw;
			}
		}
		else if (next_arg == "--max-quality" || next_arg == "--max") {
			try {
				flags::max_qual = argv[++i][0];
			} catch(std::invalid_argument const& e) {
				std::cerr << "invalid argument for max-quality\n";
				throw;
			}
		}
		else if (next_arg == "--min-quality" || next_arg == "--min") {
			try {
				flags::min_qual = argv[++i][0];
			} catch(std::invalid_argument) {
				std::cerr << "invalid argument for min-quality\n";
				throw;
			}
		}
		else if (next_arg == "--fasta-file" || next_arg == "-i") {
			flags::fasta_file_name = argv[++i];
			in_file = std::fstream(flags::fasta_file_name, std::ios::in);	
			if (!in_file.is_open()) {
				std::cerr << "problem opening file " << fasta_file << '\n';
				throw;
			}
		}
		else if (next_arg == "--bad-read-prob" || next_arg == "--brp") {
			try {
				flags::bad_read_prob = std::stod(argv[++i]);
				if (flags::bad_read_prob) {
					throw std::invalid_argument("");
				}
			} catch(std::invalid_argument&) {
				std::cerr << "invalid argument to probability\n";		
				throw;
			}
		}
		else if (next_arg == "--out" || next_arg == "-o") {
			flags::out_file_name = argv[++i];
		}
		else {
			std::cerr << "invalid command line argument: " << next_arg << "\n";
		}
		++i;
	}
	
}
int main(int argc, char* argv[]) {
	options flags;
	bool helped = false;
	try {
		if (parse_args(flags, argv, argc)) {
			return 0;
		}
	}
	if (!in_file.is_open()) {
		std::cerr << "please provide a fasta file\n";
		return 1;
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
	if (out_file == "") {
		output(std::cout,dna, read_length, min_qual, max_qual,bad_read_prob, num_reads, title);
	}
	else {
		std::ofstream ofs(out_file);
		output(ofs,dna, read_length, min_qual, max_qual,bad_read_prob, num_reads, title);
		ofs.close();

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
				case 0: dna[i] = (dna[i] != 'A') ? 'A' : 'C';
						break;
				case 1: dna[i] = (dna[i] != 'C') ? 'C' : 'G';
						break;
				case 2: dna[i] = (dna[i] != 'G') ? 'G' : 'T';
						break;
				case 3: dna[i] = (dna[i] != 'T') ? 'T' : 'A';
						break;
			}
		}
	}
}


