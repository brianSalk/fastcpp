// this program reads from a fasta file and produces a fastq file from the fasta file.
// the amount of probability of producing an incorrect read can be adjusted.

#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iomanip>

// organize all command line switches into one struct along
// with help functions
struct options {
	size_t read_length = 100;
	size_t num_reads = 10;	
	size_t min_qual = '1';
	size_t max_qual = '5';
	double bad_read_prob = 0;
	std::string fasta_file_name; 
	std::string out_file_name;
	std::string char_set;
	void h(std::ostream & os) const {
		os << "[--help,-h],[--read-length,-l],[--number-of-reads, -n], [--min-max_quality,--min][--max-quality, --max], [--fasta-file, -i], [--bad-read-prob, --brp], [--out,-o]\n";
	}
	void help(std::ostream & os) const {
		this->h(os);
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
std::string quality_string(size_t const read_length, 
		std::uniform_int_distribution<size_t> &, std::mt19937 &);
void insert_bad_char(std::string&,std::string const& char_set, 
		double bad_read_prob, std::mt19937&, std::uniform_real_distribution<double>&);
void output(std::ostream &os, std::string& bio_string, options & flags, std::string const& title); 

bool parse_args(options & flags, char** argv, size_t const argc);

int main(int argc, char* argv[]) {
	options flags;
	bool helped = false;
	// parse command line arguments, if user entered --help or -h flag, exit successfully.
	// if there is a malformed command line argument, exit with error code 1
	try {
		if (parse_args(flags, argv, argc)) {
			return 0;
		}
	}
	catch (...) {
		return 1;
	}
	std::ifstream in_file(flags.fasta_file_name);
	if (!in_file.is_open()) {
		std::cerr << "please provide a fasta file\n";
		return 1;
	}
	std::string bio_string = "";
	std::string each_line;
	std::string title;
	std::getline(in_file, title);
	// remove '<' and trailing whitespace from title.
	size_t i = 0;
	for (i = 1; i < title.size() && title[i] != ' '; ++i) {}
	title = title.substr(i+1);
	// read each line from the fasta bio sequence
	while (in_file >> each_line && each_line[0] != '>') {
		bio_string += each_line;
	}
	// write new fastq file to stdout if no file specified, else write to specified outfile
	try {
		if (flags.out_file_name == "") {
			output(std::cout,bio_string, flags, title);
		}
		else {
			std::ofstream ofs(flags.out_file_name);
			output(ofs,bio_string, flags, title);
			ofs.close();
		}
	}
	catch (...) {
		return 9;
	}
}

// generate a random phred quality string between --min and --max
// inclusive
std::string quality_string(size_t read_length, std::uniform_int_distribution<size_t> &dist, std::mt19937 & rand) {
	std::string ans;
	ans.reserve(read_length);
	for (size_t i = 0; i < read_length; ++i) {
		ans.push_back(dist(rand));
	}
	return ans;
}
// inserts a bad char from the char_set var at probability --brp
void insert_bad_char(std::string& bio_string, std::string const& char_set, double bad_read_prob, std::mt19937& rand, std::uniform_real_distribution<double> & real_dist) {
	std::uniform_int_distribution<size_t> base_dist(0,char_set.size()) ;
	for (size_t i = 0; i < bio_string.size(); ++i) {
		if (real_dist(rand) <= bad_read_prob) {
			short num = base_dist(rand);
			if (char_set[num] == bio_string[i]) {
				if (++num == char_set.size()) {
					num = 0;
				}
			}
			char bad_char = char_set[num];
			bio_string[i] = bad_char;
		}
	}
}
// output the fastq sequence with bad insertions at probability
// --bad_base_probability
void output(std::ostream &os, std::string& bio_string, options & flags, std::string const& title) {
	std::random_device rd;
	std::mt19937 rand(rd());
	std::uniform_int_distribution<size_t> dist(0, bio_string.size()-flags.read_length);
	std::uniform_real_distribution<double> double_dist(0,1);
	// use a different distribution for the qualities.
	std::uniform_int_distribution<size_t> q_dist(flags.min_qual, flags.max_qual);
	insert_bad_char(bio_string, flags.char_set, flags.bad_read_prob, rand, double_dist);
	// randomly select an index in the fasta file DNA and read from that 
	if (bio_string.size() < flags.read_length) {
		std::cerr << "read length longer than input sequence\n";
		throw std::invalid_argument("");
	}
	for (size_t i = 0; i < flags.num_reads; ++i) {
		size_t num = dist(rand);
		os << '@' << title << '\n';
		os << bio_string.substr(num, flags.read_length) << '\n';
		os << "+\n";
		os << quality_string(flags.read_length, q_dist, rand) << '\n';

	}	
}
// parse command line arguments, if -h or --help then return True
bool parse_args(options & flags, char** argv, size_t const argc) {
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
				flags.read_length = std::stoul(argv[++i]);
			} catch(std::invalid_argument) {
				std::cerr << "invalid argument for read length\n";
				throw;
			}
		}
		else if (next_arg == "--number-of-reads" || next_arg == "-n") {
			try {
				flags.num_reads = std::stoul(argv[++i]);
			} catch(std::invalid_argument) {
				std::cerr << "invalid argument for number of reads\n";
				throw;
			}
		}
		else if (next_arg == "--max-quality" || next_arg == "--max") {
			try {
				flags.max_qual = argv[++i][0];
			} catch(std::invalid_argument const& e) {
				std::cerr << "invalid argument for max-quality\n";
				throw;
			}
		}
		else if (next_arg == "--min-quality" || next_arg == "--min") {
			try {
				flags.min_qual = argv[++i][0];
			} catch(std::invalid_argument) {
				std::cerr << "invalid argument for min-quality\n";
				throw;
			}
		}
		else if (next_arg == "--fasta-file" || next_arg == "-i") {
			 flags.fasta_file_name = argv[++i];
			if (std::count(flags.fasta_file_name.begin(), flags.fasta_file_name.end(), '/')) {
				std::cerr << "cannot create file named " << flags.fasta_file_name << '\n';
				throw;
			}
		}
		else if (next_arg == "--bad-read-prob" || next_arg == "--brp") {
			try {
				flags.bad_read_prob = std::stod(argv[++i]);
				if (flags.bad_read_prob < 0) {
					throw std::invalid_argument("");
				}
			} catch(std::invalid_argument&) {
				std::cerr << "invalid argument to probability\n";		
				throw;
			}
		}
		else if (next_arg == "--out" || next_arg == "-o") {
			flags.out_file_name = argv[++i];
			if (std::count(flags.out_file_name.begin(),flags.out_file_name.end(),'/')) {
				std::cerr << "cannot create fastq file with title " << flags.out_file_name << '\n';
				throw std::invalid_argument("");
			}
		}
		else if (next_arg == "--type") {
			std::string type = argv[++i];
			if (type == "dna") {
				flags.char_set = "ACGT";
			} else if (type == "rna") {
				flags.char_set = "ACGU";
			} else if (type == "prot") {
				flags.char_set = "YZ";
				for (char c = 'A'; c <= 'W'; ++c) {
					flags.char_set.push_back(c);
				}
			} else {
				std::cerr << "invalid argument to --type: " << type << '\n';
				std::cerr << "valid arguments to --type are: dna|rna|prot\n";
				throw std::invalid_argument("");
			}
		}
		else if (next_arg == "--cust_charset") {
			flags.char_set = argv[++i];
		}
		else {
			std::cerr << "invalid command line argument: " << next_arg << "\n";
			throw std::invalid_argument("");
		}
		++i;
	}
	return false;
}
