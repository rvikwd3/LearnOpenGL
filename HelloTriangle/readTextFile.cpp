#include<fstream>
#include<iostream>
#include<string>

std::string readTextFile(const char* filename){/*{{{ */
		std::ifstream file(filename);
		std::string str;
		std::string file_contents;

		if(!file){
				std::cout << "Error in opening " << filename << "!" << std::endl;
				return 0;
		}

		std::cout << "FILE content:" << std::endl;
		
		while(std::getline(file, str)){
				file_contents += str;
				file_contents.push_back('\n');

				std::cout << str << std::endl;
		}

		return file_contents;
}/*}}}*/

int main(){                                    /*{{{ */
		std::string test;
		test = readTextFile("vertexShaderSource.glsl");

		std::cout << "[MAIN]\tContents:\n" << test <<  std::endl;

		return 0;
}/*}}}*/
