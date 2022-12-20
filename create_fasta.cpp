/*
 * do I want to add capabilities to this to allow user to pick
 * between RNA and DNA?
 * Do I want to have similar protein options?
 */
#include <random>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <fstream>
#include <limits>
struct options {
	std::string type = "dna";
	size_t min_len = 20;
	size_t max_len = 100;
	size_t length = 0;
	size_t n = 10;
	std::string out_file_name = "out.out";
	std::string seq = "seq";
};
// this function validates numeric arguments.
// if the numeric argument is a valid positive integer, we simply return it
// if the numeric argument is not a valid positive integer, throw or propogate exception that can be handled in main
size_t get_numeric_arg(std::string const& n,std::string const& cmd);
/*void parse_args(int argc, char** argv, options & flags) {

}*/
int main(int argc, char** argv) {
	std::random_device rd;
	std::mt19937 rand(rd());
	options flags;
	// process Command Line Arguments
	try {
		for (size_t i{1}; i < argc; ++i) {
			std::string arg = argv[i];
			if (i+1 == argc) {
				std::cerr << "missing required argument for: " << arg;
				return 1;
			}
			if (arg == "--type") { // handle --type
				std::string type = argv[++i];
				if (type == "dna" || type == "rna" || type == "prot") {
					flags.type = type;
				} else {
					std::cerr << "invalid argument to --type: " << type << '\n' << "choose one of 'dna','rna','prot'\n";
					return 1;
				}
			}
			else if (arg == "--min") { // handle --min
				flags.min_len = get_numeric_arg(argv[++i], arg);
			}
			else if (arg == "--max") { // handle --max
				flags.max_len = get_numeric_arg(argv[++i], arg);
			} 
			else if (arg == "--seq") { // handle seq
				flags.seq = argv[++i];
			}
			else if (arg == "--length" || arg == "-l") { // handle length
				flags.length = get_numeric_arg(argv[++i], arg);
			}
			else if (arg == "-n") { // handle n
				flags.n = get_numeric_arg(argv[++i], arg);
			}
			else if (arg == "--out") { // handle --out
				flags.out_file_name = argv[++i];
			}
			else {
				std::cerr << "unrecognized flag: " << arg << '\n';
				return 1;
			}
		}
	}
	catch(...) {
		return 1;
	}
	// END process command line arguments

	std::cout << "PRINT ALL FLAGS:\n";
	std::cout << flags.length << '\n' << flags.max_len << '\n' << flags.min_len << '\n' << flags.n << '\n';
	
//	std::ofstream out_file(out_file_name);
//	if (is_nucl) {
//		std::uniform_int_distribution<size_t> nuc_dist(0,3);
//		std::uniform_int_distribution<size_t> len_dist(min_len,max_len);
//		for (size_t each_seq{1}; each_seq <= n; ++each_seq) {
//			std::string next_seq_name = "> " + title + std::to_string(each_seq) + '\n';
//			size_t next_len = len_dist(rand);
//			out_file << next_seq_name;
//			for (size_t i{0}; i < next_len;++i) {
//				size_t next_num{nuc_dist(rand)};
//				switch (next_num) {
//					case 0: out_file << 'A';
//							break;
//					case 1: out_file << 'C';
//							break;
//					case 2: out_file << 'G';
//							break;
//					case 3: out_file << 'T';
//							break;
//				}
//			}
//			out_file << '\n';
//		}
//	} else {
//		std::uniform_int_distribution<size_t> nuc_dist(0,19);
//		std::uniform_int_distribution<size_t> len_dist(min_len,max_len);
//		for (size_t each_seq{1}; each_seq <= n; ++each_seq) {
//			std::string next_seq_name = "> seq" + std::to_string(each_seq) + '\n';
//			size_t next_len = len_dist(rand);
//			out_file << next_seq_name;
//			for (size_t i{0}; i < next_len;++i) {
//				size_t next_num{nuc_dist(rand)};
//				switch (next_num) {
//					case 0: out_file << 'A';
//							break;
//					case 1: out_file << 'C';
//							break;
//					case 2: out_file << 'D';
//							break;
//					case 3: out_file << 'E';
//							break;
//					case 4: out_file << 'F';
//							break;
//					case 5: out_file << 'G';
//							break;
//					case 6: out_file << 'H';
//							break;
//					case 7: out_file << 'I';
//							break;
//					case 8: out_file << 'K';
//							break;
//					case 9: out_file << 'L';
//							break;
//					case 10: out_file << 'M';
//							break;
//					case 11: out_file << 'N';
//							 break;
//					case 12: out_file << 'P';
//							 break;
//					case 13: out_file << 'Q';
//							 break;
//					case 14: out_file << 'R';
//							 break;
//					case 15: out_file << 'S';
//							 break;
//					case 16: out_file << 'T';
//							 break;
//					case 17: out_file << 'V';
//							 break;
//					case 18: out_file << 'W';
//							 break;
//					case 19: out_file << 'Y';
//							 break;
//					case 20: out_file << 'O';
//							 break;
//					case 21: out_file << 'M';
//							 break;
//					case 22: out_file << 'M';
//							 break;
//					case 23: out_file << 'M';
//							 break;
//					case 24: out_file << 'M';
//							 break;
//				}
//
//			}
//			out_file << '\n';
//		}
//	}
//	out_file.close();
} // end main
// FUNCTION DEFINITIONS
// this function validates numeric arguments.
// if the numeric argument is a valid positive integer, we simply return it
// if the numeric argument is not a valid positive integer, throw or propogate exception that can be handled in main
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
