#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring> 
#include <exception> 
#include <algorithm>

using namespace std; 

constexpr int get_unique_num(int len) { 
    return (len == 2) ? 1 : (len == 3) ? 7 : (len == 4) ? 4 : (len == 7) ? 8 : 0; 
}

struct group { 
    string identifier; 
    vector<string> members; 

    group(const string iden,vector<string> unknown); 

    void check_seg(const string new_mem); 

    const string remove_similar(const string tst); // gets the member that includes all characters of the test param, once retrieved it is removed from members and returned. 
    const string pop_smallest(); 
    const string pop_largest(); 

    void log(); 
};

struct input_form { 
    vector<string> known_patterns; 
    vector<string> unknown_patterns; 

    vector<string> output_patterns; 

    int num_unique; 

    input_form(); 

    int determine_value(); 
    int get_pattern_num(const string & pattern); 
    const string wrap_pop(const string v); 

    void log(); 
}; 

istream & operator >> (istream & current, input_form & in); 

// Solution Methods 

int total_unique_patterns(string file_name); 
int get_output_val(string filename);


int main() { 
    cout << get_output_val("./Input/Day8.txt") << endl;
    return 0; 
}

// input form stuff 

input_form::input_form() {
    known_patterns.resize(10, ""); 
    num_unique = 0; 
}

int input_form::determine_value() { 
    int deduced_val = 1; 

    group cf_grp(this->known_patterns[1], this->unknown_patterns); 
    
    this->known_patterns[3] = wrap_pop(cf_grp.pop_smallest()); 
    this->known_patterns[9] = wrap_pop(cf_grp.remove_similar(this->known_patterns[4])); 
    this->known_patterns[0] = wrap_pop(cf_grp.pop_smallest());

    group remaining_elems("any", unknown_patterns); 
    
    this->known_patterns[6] = wrap_pop(remaining_elems.pop_largest()); 
    this->known_patterns[5] = wrap_pop(remaining_elems.remove_similar(this->known_patterns[6])); 
    this->known_patterns[2] = wrap_pop(remaining_elems.pop_smallest()); 

    string current_v = ""; 

    for (int i = 0; i < output_patterns.size(); i++) {
        current_v += to_string(get_pattern_num(output_patterns[i])); 
    }

    return stoi(current_v); 
}

const string input_form::wrap_pop(const string v) {
    vector<string>::iterator found_iter = find(this->unknown_patterns.begin(), this->unknown_patterns.end(), v); 

    if (found_iter == this->unknown_patterns.end())  { 
        throw std::logic_error("Unexpected Situation : Unfound value when wrapping pop");
    }

    this->unknown_patterns.erase(found_iter); // remove it so we now know that pattern 
    return v; 
}

void input_form::log() {
    cout << "---------------------------------------------------------------------------------" << endl;
    cout << "Known patterns :" << endl;
    for (int i = 0; i < known_patterns.size(); i++) { 
        cout << " " << i << " : " <<  known_patterns[i] << endl;
    }
    cout << "Unknown patterns :" << endl;
    for (int i = 0; i < unknown_patterns.size(); i++) { 
        cout << " " << i << " : " << unknown_patterns[i] << endl;
    }
    cout << "Output patterns :" << endl;
    for (int i = 0; i < output_patterns.size(); i++) { 
        cout << " " << i << " : " << output_patterns[i] << endl; 
    }
    cout << "Num unique patterns : " << num_unique << endl;
}

int input_form::get_pattern_num(const string & pattern) { 
    int is_unique = get_unique_num(pattern.length());

    if (is_unique) { 
        return is_unique;
    }

    for (int i = 0; i < known_patterns.size(); i++) { 
        if (known_patterns[i] == pattern) { 
            return i; 
        }
    }
    return 0; 
}

// segment group stuff

group::group(const string iden, vector<string> unknown) { 
    this->identifier = iden; 

    if (this->identifier == "any") { 
        copy(unknown.begin(), unknown.end(), back_inserter(members)); 
    } else {
        for (int i = 0; i < unknown.size(); i++) { 
            this->check_seg(unknown[i]); // get these segments into the group 
        }
    }
}

void group::check_seg(const string new_mem) { 
    for (int i = 0; i < this->identifier.length(); i++) { 
        char current_check = this->identifier[i]; 

        if (new_mem.find(current_check) == new_mem.npos) { 
            return; // throw out early 
        }
    }

    this->members.push_back(new_mem); 
}

void group::log() { 
    cout << "---------------------------------------------------------------------------------" << endl;
    cout << "Group Identifier : " << identifier << endl;
    cout << "Members :" << endl;
    for (int i = 0; i < members.size(); i++) { 
        cout << "   " << members[i] << endl;
    }
}

const string group::remove_similar(const string tst) { // ugly function but ye it works :D 
    vector<string>::iterator m_it = members.begin(); // this is the active iterator 

    vector<string>::iterator nearest = members.end(); // redemption for if the === iteration pass doesn't pan out 

    int prev_length = 0; 

    while (m_it != this->members.end()) {
        string matched_chars = ""; 

        for (int i = 0; i < tst.length(); i++) { 
            if (m_it->find(tst[i]) != m_it->npos) { 
                matched_chars += tst[i]; 
            }
        }
        if (matched_chars.length() == tst.length()) { 
            const string v = *m_it;
            members.erase(m_it);
           return v; 
        } else if (prev_length < matched_chars.length()) { 
            prev_length = matched_chars.length(); 
            nearest = m_it; 
        }
        m_it++; 
    }

    if (nearest != members.end()) { 
        const string v = *nearest;
        members.erase(nearest);
        return v; 
    }


    throw std::logic_error("Unexpected Situation : Could not find a similar match"); 
}

const string group::pop_smallest() { 
    vector<string>::iterator min_len_str = members.begin(); 

    for (vector<string>::iterator current_str = ++members.begin(); current_str != members.end(); current_str++) { 
        if (current_str->length() < min_len_str->length()) { 
            min_len_str = current_str; 
        }
    }

    const string v = *min_len_str; 
    members.erase(min_len_str);
    return v;
}

const string group::pop_largest() { 
    vector<string>::iterator max_len_str = members.begin();

    for (vector<string>::iterator current_str = ++members.begin(); current_str != members.end(); current_str++) { 
        if (current_str->length() > max_len_str->length()) { 
            max_len_str = current_str;
        }
    }

    const string v = *max_len_str;
    members.erase(max_len_str);
    return v; 
}

// handle rest of program related stuff 

istream & operator >> (istream & current, input_form & in) { 
    for (int i = 0; i < 14; i++) { 
        string current_str; 
        current >> current_str;

        if (current_str == "|") { 
            current >> current_str; 
        }

        sort(current_str.begin(), current_str.end()); 

        int unique_val = get_unique_num(current_str.length());

        if (i > 9) { 
            in.num_unique += (unique_val != 0); 
            in.output_patterns.push_back(current_str); 
        } else { 
            if (unique_val) { 
                in.known_patterns[unique_val] = current_str; 
            } else {
                in.unknown_patterns.push_back(current_str); 
            }
        }
    }

    return current; 
}

// total unique patterns 

int total_unique_patterns(string file_name) {
    int current_count = 0; 

    ifstream i_stream(file_name); 

    while (!i_stream.eof()) { 
        input_form to_count; 
        i_stream >> to_count; 
        current_count += to_count.num_unique; 
        to_count.determine_value(); 
    }

    return current_count; 
}

int get_output_val(string filename) { 
    int current_v = 0; 

    ifstream i_stream(filename);

    while (!i_stream.eof()) { 
        input_form in;
        i_stream >> in;
        current_v += in.determine_value();
    }

    return current_v;
}
