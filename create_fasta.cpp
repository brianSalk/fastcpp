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

int main(int argc, char** argv) {
	std::random_device rd;
	std::mt19937 rand(rd());
	// process CLAs
	bool is_nucl = true;
	size_t min_len = 20;
	size_t max_len = 100;
	size_t n = 10;
	std::string out_file_name = "out.out";
	for (size_t i = 1; i < argc; ++i) {
		if (std::strcmp(argv[i],"--type") == 0) {
			if (std::strcmp(argv[i+1], "nucl") == 0) {
				is_nucl = true;
			} else if (std::strcmp(argv[i+1], "prot") == 0) {
				is_nucl = false;
			} else {
				std::cerr << "invalid arg for --type \'" << argv[i+1] << "\', default value of \'nucl\' retained\n";
			}
			++i;
		}
		else if (std::strcmp(argv[i], "--min") == 0) {
			try {
				min_len = std::stoul(std::string(argv[i+1]));
			} catch(std::invalid_argument const& e) {
				std::cerr << argv[i+1] << " could not be converted to a number, default value of \'" << min_len << "\' retained\n";
			}
			catch (std::out_of_range const& e) {
				std::cerr << argv[i+1] << " out of range, default value of \'" << min_len << "\' retained\n";
			}
			++i;
		}
		else if (std::strcmp(argv[i], "--max") == 0) {
			try {
				max_len = std::stoul(argv[i+1]);
			} catch(std::invalid_argument const& e) {
				std::cerr << argv[i+1] << " could not be converted to a number, default value of \'" << max_len << "\' retained\n";
			}
			catch(std::out_of_range const& e) {
				std::cerr << argv[i+1] << " out of range, default value of \'" << max_len << "\' retained\n";
			}
			++i;
		}
		else if (std::strcmp(argv[i], "-n") == 0 || std::strcmp(argv[i], "--num_seqs") == 0) {
			try {
				n = std::stoul(argv[i+1]);
			} catch(std::invalid_argument const& e) {
				std::cerr << argv[i+1] << " could not be converted to a number, default value of \'" << n << "\' retained\n";
			}
			catch(std::out_of_range const& e) {
				std::cerr << argv[i+1] << " out of range, default value of \'" << n << "\' retained\n";
			}
			++i;
		}
		else if (std::strcmp(argv[i], "--out") == 0) {
			out_file_name = argv[i+1];
			++i;
		}
		else if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
			std::cerr << "display help and return 0";
			return 0;
		}
		else {
			std::cerr << "invalid argument \'" << argv[i] << "\'\n";
			return 1;
		}
	}
	std::ofstream out_file(out_file_name);
	if (is_nucl) {
		std::uniform_int_distribution<size_t> nuc_dist(0,3);
		std::uniform_int_distribution<size_t> len_dist(min_len,max_len);
		for (size_t each_seq{1}; each_seq <= n; ++each_seq) {
			std::string next_seq_name = "> seq" + std::to_string(each_seq) + '\n';
			size_t next_len = len_dist(rand);
			out_file << next_seq_name;
			for (size_t i{0}; i < next_len;++i) {
				size_t next_num{nuc_dist(rand)};
				switch (next_num) {
					case 0: out_file << 'A';
							break;
					case 1: out_file << 'C';
							break;
					case 2: out_file << 'G';
							break;
					case 3: out_file << 'T';
							break;
				}
			}
			out_file << '\n';
		}
	} else {
		std::uniform_int_distribution<size_t> nuc_dist(0,19);
		std::uniform_int_distribution<size_t> len_dist(min_len,max_len);
		for (size_t each_seq{1}; each_seq <= n; ++each_seq) {
			std::string next_seq_name = "> seq" + std::to_string(each_seq) + '\n';
			size_t next_len = len_dist(rand);
			out_file << next_seq_name;
			for (size_t i{0}; i < next_len;++i) {
				size_t next_num{nuc_dist(rand)};
				switch (next_num) {
					case 0: out_file << 'A';
							break;
					case 1: out_file << 'C';
							break;
					case 2: out_file << 'D';
							break;
					case 3: out_file << 'E';
							break;
					case 4: out_file << 'F';
							break;
					case 5: out_file << 'G';
							break;
					case 6: out_file << 'H';
							break;
					case 7: out_file << 'I';
							break;
					case 8: out_file << 'K';
							break;
					case 9: out_file << 'L';
							break;
					case 10: out_file << 'M';
							break;
					case 11: out_file << 'N';
							 break;
					case 12: out_file << 'P';
							 break;
					case 13: out_file << 'Q';
							 break;
					case 14: out_file << 'R';
							 break;
					case 15: out_file << 'S';
							 break;
					case 16: out_file << 'T';
							 break;
					case 17: out_file << 'V';
							 break;
					case 18: out_file << 'W';
							 break;
					case 19: out_file << 'Y';
							 break;
					case 20: out_file << 'O';
							 break;
					case 21: out_file << 'M';
							 break;
					case 22: out_file << 'M';
							 break;
					case 23: out_file << 'M';
							 break;
					case 24: out_file << 'M';
							 break;
				}

			}
			out_file << '\n';
		}
	}
	out_file.close();
}
