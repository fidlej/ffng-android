#ifndef FFNGFILES_H_
#define FFNGFILES_H_

#include <string>

class FFNGFiles {
public:
	static bool exists(const std::string &path, int type);
	static std::string read(const std::string &path, int type);
	static void createPath(const std::string &path);
	static bool write(const std::string &path, const std::string &data);
};

#endif /* FFNGFILES_H_ */
