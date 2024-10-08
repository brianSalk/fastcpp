/*
 * This program will create randomly generated fasta files when run from the command line
 * 
 * 
 */
#include <random>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <limits>
// store command line flags in the convenient options datatype, if adding a command line argument, add it here first.
struct options {
	std::string type = "dna";
	size_t min_len = 20;
	size_t max_len = 100;
	size_t length = 0;
	size_t n = 10;
	std::string out_file_name = "";
	std::string seq = "seq";
	std::string cust_charset = "";
	void h(std::ostream &os) const {
		os << "[--length,-l], [--min], [--max], [--num_seqs,-n], [--out,-o], [--seq], [--type]\n";
	}
	void help(std::ostream &os) const {
		this->h(os);
		os << "create_fasta is a command line tool that creates randomly generated fasta files\n";
		os << "==========================================================================\n\n";
		os << "\n --length, -l: specify length of each sequence in fasta.  Overrides --min and --max\n";
		os << "--min: minimum length of a randomly generated sequence.(ignored if -l or --length is also used)\n";
		os << "--max: maximum length of a randomly generated sequence.(ignored if -l or --length is also used)\n";
		os << "--num_seqs, -n: specify number of sequences to generate.\n";
		os << "--out, -o: outfile to print fasta file to.  (print to stdout if nothing is specified)\n";
		os << "--seq: base name of the sequence, if multiple sequences are generated, ascending integers starting at 1 will be appended to the base name\n";
		os << "--type: specify the type of fasta file to create, (valid options are 'dna', 'rna', and 'prot')\n";
		os << "--cust_charset: specify your own character set to use.  Overrides value of --type\n";
	}
};
// this function validates numeric arguments.
// if the numeric argument is a valid positive integer, we simply return it
// if the numeric argument is not a valid positive integer, throw or propogate exception that can be handled in main
size_t get_numeric_arg(std::string const& n,std::string const& cmd);

// if help exists in argv, print the help menue and return true,
// else set each flag to its corresponding value and return false
// if invalid flag used or mandatory argument missing, throw exception
bool parse_args(int argc, char** argv, options & flags);

// writes the new fasta file to out, selects one of 'letters' at random for each position.
void create_fasta(std::ostream &out, options const& flags, std::vector<char> const& letters);

// used for debugging only, print the name and value of each member of 'flags'
void __dump_args(options const& flags);


int main(int argc, char** argv) {
	options flags;
	// process Command Line Arguments
	try {
		bool helped = parse_args(argc,argv,flags);
		if (helped) {
			return 0;
		}
	}
	catch(...) {
		return 1;
	}
	// END process command line arguments, uncomment next line to debug CLAs
	//__dump_args(flags);
	// if length specified, override values for min_len and max_len
	if (flags.length != 0) {
		flags.max_len = flags.length;
		flags.min_len = flags.length;
	}
	std::ostream *os;
	std::ofstream of = std::ofstream(flags.out_file_name);
	// if out_file_name specified, write to that file, else to stdout
	if (flags.out_file_name != "") {
		os = &of;
	}
	else {
		os = &std::cout;
	}
	// if cust_charset is set, use cust_charset and ignore argument to --type
	if (flags.cust_charset != "") {
		std::vector<char> cust_vec(flags.cust_charset.begin(), flags.cust_charset.end());
		create_fasta(*os, flags, cust_vec);
	} else if (flags.type == "dna") {
		create_fasta(*os, flags, {'A','C','G','T'});
	} else if (flags.type == "rna") {
		create_fasta(*os, flags, {'A', 'C', 'G', 'U'});
	} else if (flags.type == "prot") {
		std::vector<char> prot;
		for (short i{0}; i + 'A' <= 'W'; ++i) {
			prot.push_back(i+'A');
		}
		prot.push_back('Y');
		prot.push_back('Z');
		create_fasta(*os, flags, prot);
	}
	of.close();	

	
} // end main
  
// --------------------------- FUNCTION DEFINITIONS -----------

size_t get_numeric_arg(std::string const& n,std::string const& cmd) {
	long long ans;
	try {
		ans = std::stoll(n);
		if (ans <= 0) {
			throw std::out_of_range(n);
		}
	}
	catch(std::invalid_argument const&) {
		std::cerr << "invalid argument to " << cmd << ": " << n << '\n';
		std::cerr << "argument must be a positive integer\n";
		throw;
	} catch(std::out_of_range const&) {
		std::cerr << "invalid argument to " << cmd << ": " << n << '\n';
		std::cerr << n << " is too large, please choose a number between 1 and " << std::numeric_limits<long long>::max() << " inclusive" << '\n';
		throw;
	}
	return ans;
}
bool parse_args(int argc, char** argv, options& flags) {
    for (size_t i{1}; i < argc; ++i) {
        const std::string arg = argv[i];

        // Print help message and return true if the next arg is --help or -h
        if (arg == "--help" || arg == "-h") {
            flags.help(std::cout);
            return true;
        }

        // Check for missing required argument
        if (i+1 == argc) {
            std::cerr << "missing required argument for: " << arg << '\n';
            throw std::invalid_argument("");
        }

        // Handle different flags
        if (arg == "--type") {
            const std::string type = argv[++i];
            if (type == "dna" || type == "rna" || type == "prot") {
                flags.type = type;
            } else {
                std::cerr << "invalid argument to --type: " << type << '\n'
                          << "choose one of ['dna','rna','prot']\n";
                throw std::invalid_argument("");
            }
        } else if (arg == "--min") {
            flags.min_len = get_numeric_arg(argv[++i], arg);
        } else if (arg == "--max") {
            flags.max_len = get_numeric_arg(argv[++i], arg);
        } else if (arg == "--seq") {
            flags.seq = argv[++i];
        } else if (arg == "--length" || arg == "-l") {
            flags.length = get_numeric_arg(argv[++i], arg);
        } else if (arg == "-n" || arg == "--num_seqs") {
            flags.n = get_numeric_arg(argv[++i], arg);
        } else if (arg == "--out" || arg == "-o") {
            flags.out_file_name = argv[++i];
        } else if (arg == "--cust_charset") {
            flags.cust_charset = argv[++i];
        } else {
            std::cerr << "unrecognized flag: " << arg << '\n';
            throw std::invalid_argument("");
        }
    }

    return false;
}
void create_fasta(std::ostream &out, options const& flags, std::vector<char> const& letters) {
	std::random_device rd;
	std::mt19937 rand(rd());
	std::uniform_int_distribution<size_t> nuc_dist(0,letters.size()-1);
	std::uniform_int_distribution<size_t> len_dist(flags.min_len,flags.max_len);
	for (size_t each_seq{1}; each_seq <= flags.n; ++each_seq) {
		std::string next_seq_name = "> " + flags.seq + std::to_string(each_seq) + '\n';
		size_t next_len = len_dist(rand);
		out << next_seq_name;
		for (size_t i{0}; i < next_len;++i) {
			size_t next_num{nuc_dist(rand)};
			out << letters[next_num];
		}
		out << '\n';
	}
}
void __dump_args(options const& flags) {
	std::cout << "PRINT ALL FLAGS:\n";
	std::cout << flags.length << '\n' << flags.max_len << '\n' << flags.min_len << '\n' << flags.n << '\n' << flags.out_file_name << '\n' << flags.seq << '\n' << flags.type << '\n' << flags.cust_charset << '\n';
}
