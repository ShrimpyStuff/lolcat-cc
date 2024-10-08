/*
    lolcat-cc is a fast and cross-platform c++
    implementation of lolcat.

    GitHub: https://github.com/n-ham/lolcat-cc

    Copyright (c) 2020-present
    Creator: Nicholas Ham
*/

#include <cstdio>
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <time.h>

const int noColors = 30;
const std::string c_white  =  "\033[0m";
const std::string colors[] = {"\033[38;5;39m",
			      "\033[38;5;38m",
			      "\033[38;5;44m",
			      "\033[38;5;43m",
			      "\033[38;5;49m",
			      "\033[38;5;48m",
			      "\033[38;5;84m",
			      "\033[38;5;83m",
			      "\033[38;5;119m",
			      "\033[38;5;118m",
			      "\033[38;5;154m",
			      "\033[38;5;148m",
			      "\033[38;5;184m",
			      "\033[38;5;178m",
			      "\033[38;5;214m",
			      "\033[38;5;208m",
			      "\033[38;5;209m",
			      "\033[38;5;203m",
			      "\033[38;5;204m",
			      "\033[38;5;198m",
			      "\033[38;5;199m",
			      "\033[38;5;163m",
			      "\033[38;5;164m",
			      "\033[38;5;128m",
			      "\033[38;5;129m",
			      "\033[38;5;93m",
			      "\033[38;5;99m",
			      "\033[38;5;63m",
			      "\033[38;5;69m",
			      "\033[38;5;33m"};

bool file_exists(const std::string& path)
{
    struct stat info;

    if(stat(path.c_str(), &info) != 0) //path does not exist
        return 0;
    else if(info.st_mode & S_IFDIR)    //directory
        return 0;
    else                               //file
        return 1;
}

bool format = 0;
int tabWidth = 2;

int mod_val;
int mod(const int& x, const int& m)
{
	if(x >= 0)
		return x%m;
	else
	{
		mod_val = (-x)%m;
		if(mod_val)
			return m-mod_val;
		else
			return 0;
	}
}

double gradient = 999;
size_t width, cWidth;

bool addLineNo = 0, zigzag = 0;
int posGrad = 1;

int lolcat(std::istream& is)
{
	std::string inLine;
	int color,
	    lineNo = 0,
	    r = rand()%noColors;

	if(gradient < 0)
		gradient = -gradient;

	while(!is.eof())
	{
		if(!getline(is, inLine))
			break;

		if(addLineNo)
			inLine = std::to_string(lineNo) + ": " + inLine;

		if(zigzag)
		{
			if(int(std::floor(lineNo/noColors))%2)
				color = r = (r+1)%noColors;
			else
				color = r = mod(r-1, noColors);
		}
		else
		{
			if(posGrad)
			{
				cWidth = (1-((lineNo+r)*gradient - std::floor((lineNo+r)*gradient))/1.0)*width;
				color = int((lineNo+r)*gradient)%noColors;
			}
			else
			{
				cWidth = (((lineNo+r)*gradient - std::floor((lineNo+r)*gradient))/1.0)*width;
				color = mod(-int((lineNo+r)*gradient), noColors); 
				//color = mod(noColors-int((lineNo+r)*gradient)%noColors, noColors); //alternative to above
			}
		}
			

		size_t i=0;
		std::cout << colors[color];

		while(i < inLine.size())
		{
			for(size_t I=0; I<width && i<inLine.size(); ++I)
			{
				if(format)
				{
					while(inLine[i] == '\t')
					{
						std::cout << std::string(tabWidth, ' ');
						++i;
					}

				    //https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
					while(format && i < inLine.size() && ((inLine[i] == '\\' && (!i || inLine[i-1] != '\\')) || 
					                            inLine[i] == '\x0' ||
					                            inLine[i] == '\xB0' ||         // degree
					                            inLine.substr(i, 2) == "°" ||  // degree
					                            inLine[i] == '\xF0' ||         // emojis
					                            inLine[i] == '\xE2' ||         // emojis
					                            inLine[i] == '\xC2' ||         // emojis
					                            //inLine[i] == "\e" ||
					                            inLine[i] == '\033' ||         // octag
					                            inLine.substr(i, 2) == "^[" || // ctrl key
					                            inLine[i] == '\x1b' ||         // hexadecimal
					                            inLine[i] == '\u001b'))        // unicode
					{
						int start = i++;
						while(i < inLine.size() && /*inLine[i] != 'm' && */
						                           inLine[i] != -80 && 
						                           inLine[i] != -92 && 
                                                   inLine[i] != ' ' && 
                                                   inLine[i] != '\n' && 
                                                   inLine[i] != '\t')
							++i;
						std::cout << inLine.substr(start, i-start);
						
						if(inLine[i] == ' ')
							std::cout << colors[color];
					}
				}

				if(i < inLine.size())
					std::cout << inLine[i++];
			}

			if(width != cWidth)
				cWidth = width;
			color=(color+1)%noColors;
			std::cout << colors[color];
		}


		if(&is == &std::cin)
			std::cout << c_white;

		std::cout << std::endl;
		++lineNo;
	}
	
	std::cout << c_white << std::flush;

	return 0;
}

int lolfilter(std::istream &is)
{
	if(width == 0)
	{
		#if defined _WIN32 || defined _WIN64
			width = 7;
		#else
			width = 4;
		#endif
	}
	
	if(gradient != 999)
		return lolcat(is);
	else if(zigzag)
		return lolcat(is);
	#if defined _WIN32 || defined _WIN64
	#else
		else if(!(rand()%3))
		{
			zigzag = 1;
			return lolcat(is);
		}
	#endif
	else
	{
		gradient = 0.6;
		posGrad = rand()%2;
	}

	return lolcat(is);
}


int main(int argc, char ** argv)
{
	srand(time(NULL));
	
	std::string param;
	for(int i=1; i<argc; i++)
	{
		param = argv[i];

		if(param == "-f")
			format = 1;
		else if(param.substr(0, 3) == "-g=")
		{
			gradient = std::strtod(param.substr(3, param.size()-3).c_str(), NULL);
			if(gradient < 0)
			{
				posGrad = 0;
				gradient = -gradient;
			}
		}
		else if(param == "-ln")
			addLineNo = 1;
		else if(param == "-ps")
		{
		}
		else if(param.substr(0, 4) == "-tw=")
			tabWidth = std::atoi(param.substr(4, param.size()-4).c_str());
		else if(param.substr(0, 3) == "-w=")
			width = std::atoi(param.substr(3, param.size()-3).c_str());
		else if(param == "-zz")
			zigzag = 1;
		else if(file_exists(param))
		{
			std::ifstream ifs(param);
			int ret_val = lolfilter(ifs);
			ifs.close();
			return ret_val;
		}
		else if(param == "-h" || param == "-help")
		{
			std::stringstream ss;

			ss << argv[0] << " is a cross-platform c++ implementation of the original lolcat,";
			ss << " which writes output to the terminal with rainbow colours" << std::endl;

			ss  << "=>" << " usage:" << std::endl;
			ss << " command | " << argv[0] << " (options)" << std::endl;
			ss << " " << argv[0] << " (options) file" << std::endl;

			ss << "=>" << " options:" << std::endl;
			ss << "      -f" << ": format" << std::endl;
			ss << "  -g=[d]" << ": set gradient" << std::endl;
			ss << "     -ln" << ": add line numbers" << std::endl;
			ss << "     -ps" << ": powershell colors" << std::endl;
			ss << " -tw=[i]" << ": set tab width" << std::endl;
			ss << "      -v" << ": print version" << std::endl;
			ss << "  -w=[i]" << ": set width" << std::endl;
			ss << "     -zz" << ": +/- gradient" << std::endl;

			return lolfilter(ss);
		}
		else if(param == "-v" || param == "-version")
		{
			std::stringstream ss;
			ss << "v1.0.1" << std::endl;

			return lolfilter(ss);
		}
		else
		{
			std::cout << "error: " << argv[0] << ": ";

			std::stringstream ss;
			ss << "do not recognise '" << param << "'" << std::endl;

			return lolfilter(ss);
		}
	}

	return lolfilter(std::cin);
}
