#include <iostream>
#include <sstream>
#include <cstdlib>
#include "Sorcerer.hpp"
#include "Peon.hpp"

static int	nbErrors = 0;
static bool	quitOnFail = false;
static bool	paddingEnabled = true;

void	mustPrint(const std::string);
void	mustPrint(bool const, const std::string);
void	redirectOutput();
void	test(const bool, const bool);
void	test(const int, const int);
void	test(const std::string &, const std::string &);
void	test(const void *, const void *);
void	testd(const void *, const void *);
int	res();

static std::streambuf		*cout_old = NULL;
static std::stringstream	output;

static void	padding()
{
	if (paddingEnabled)
		std::cout << std::endl;
}

int	res()
{
	std::cout << std::endl;
	if (nbErrors > 0)
		std::cout << "Fail." << std::endl;
	else
		std::cout << "OK !" << std::endl;
	return (nbErrors != 0);
}

void	test(bool b)
{
	if (b)
		std::cout << "OK";
	else
	{
		std::cout << "BAD";
		nbErrors++;
	}
	std::cout << std::endl;
	padding();
	if (nbErrors > 0 && quitOnFail)
		exit(res());
}

void	test(bool const a, bool const b)
{
	std::cout << std::boolalpha << a << " == " << b << ": " << std::noboolalpha;
	test(a == b);
}

void	test(int const a, int const b)
{
	std::cout << a << " == " << b << ": ";
	test(a == b);
}

void	test(std::string const & a, std::string const & b)
{
	std::cout << a << " == " << b << ": ";
	test(a == b);
}

void	test(void const *a, void const *b)
{
	std::cout << a << " == " << b << ": ";
	test(a == b);
}

void	testd(void const *a, void const *b)
{
	std::cout << a << " != " << b << ": ";
	test(a != b);
}

void	mustPrint(std::string const p)
{
	mustPrint(false, p);
}

void		mustPrint(bool const b, std::string const p)
{
	std::string	buffer;
	bool		noendl = false;

	if (cout_old == NULL)
		return;
	buffer = output.str();
	std::cout.rdbuf(cout_old);
	cout_old = NULL;
	noendl = ((!buffer.empty() && buffer.at(buffer.length() - 1) != '\n')
		  || (!p.empty() && p.at(p.length() - 1) != '\n'));

	std::cout << "Expected : ";
	if (b)
		std::cout << std::endl;
	std::cout << p;
	if (noendl)
		std::cout << std::endl;
	std::cout << "Your     : ";
	if (b)
		std::cout << std::endl;
	std::cout << buffer;
	if (noendl)
		std::cout << std::endl;
	std::cout << "Result   : ";
	test(p == buffer);
}

void	redirectOutput()
{
	output.str("");
	cout_old = std::cout.rdbuf(output.rdbuf());
}

int	main()
{
	{
		redirectOutput();
		Sorcerer robert("Robert", "the Magnificent");
		mustPrint("Robert, the Magnificent, is born!\n");

		redirectOutput();
		Victim jim("Jimmy");
		mustPrint("Some random victim called Jimmy just popped!\n");

		redirectOutput();
		Peon joe("Joe");
		mustPrint("Some random victim called Joe just popped!\nZog zog.\n");

		redirectOutput();
		std::cout << robert << jim << joe;
		mustPrint("I am Robert, the Magnificent, and I like ponies!\nI'm Jimmy and i like otters!\nI'm Joe and i like otters!\n");

		redirectOutput();
		robert.polymorph(jim);
		mustPrint("Jimmy has been turned into a cute little sheep!\n");

		robert.polymorph(joe);
		mustPrint("Joe has been turned into a cute little sheep!\n");

		redirectOutput();
	}
	mustPrint(true, "Bleuark...\nVictim Joe just died for no apparent reason!\nVictim Jimmy just died for no apparent reason!\nRobert, the Magnificent, is dead. Consequences will never be the same!\n");

	return (res());
}