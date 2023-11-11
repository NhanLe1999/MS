#include "GAFWrapper.h"

USING_NS_CC;
using namespace std;

class Fruit : public GAFWrapper {
public:
	static Fruit* Init(string GAF);
	string gaf;
	void idle(bool loop);
	void start(bool loop,function<void()> call_back = nullptr);
	void fast(bool loop, function<void()> call_back = nullptr);
	void stop(bool loop);
	void mchoice(bool loop);
	void wrong(bool loop);
	void correct(bool loop);
	bool isIdle;
private:
	GAFWrapper* content;
};
