#include <iostream>

struct Boo {
	template <class T>
	const Boo &operator() (T &thing) const
	{
		std::cout << thing << "\n";
		return *this;
	}
} BOO;

int main()
{
	BOO	("hi")
		("There")
		("Mr")
		("Man");
}

